
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "Services/StdDef.h"
#include "APOS.h"
#include "BSP/systick.h"
#include "stm32f0xx_gpio.h"

#define DEBUG

static const uint32_t maxTasks = 10;
static uint32_t numTasks = 0;
static uint32_t currentTask = 0;
static APOS_TCB_STRUCT* pTasks[maxTasks];
static uint32_t IS_Initialized = 0;

static void copyTasks(APOS_TCB_STRUCT** source, APOS_TCB_STRUCT** dest, uint32_t size)
{	for(int i = 0; i < size; i++) 
	{	(*dest)[i] = (*source)[i];
	}
}

static BOOL APOS_Running(void);
 
void APOS_Init(void)  													// Initialisert das Echtzeitbetriebssystem
{	
	// NVIC_InitTypeDef NVIC_InitStruct;
	// NVIC_InitStruct.NVIC_IRQChannel = PendSV_IRQn;
	// NVIC_InitStruct.NVIC_IRQChannelPriority = 0xF0;	// höchster Wert: kleinste Prio
	// 																								// 0xF im oberen nibble: 0b11110000
	// NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;	
	// NVIC_Init(&NVIC_InitStruct);
	// 
	// NVIC_SetPriority(PendSV_IRQn, 0xF0 );				
	
	for(int i = 0; i < numTasks; i++) 
	{		pTasks[i] = NULL;
	}
	
}

static void APOS_STACK_INIT(APOS_TCB_STRUCT* pTask)
{
//	if(currentTask == 0)
//		return;
	pTask->pStack[pTask->stackSize-1] = 0x01000000; // xPSR ( thumb mode )
	pTask->pStack[pTask->stackSize-2] = (uint32_t)pTask->routine; // PC
	pTask->pStack[pTask->stackSize-3] = (uint32_t)APOS_Scheduler; // LR
	for(int i = 0; i < 5; i++)
	{
		pTask->pStack[pTask->stackSize-i-4] = i+1; // r12, r3, r2, r1, r0
	}
	pTask->pStack = pTask->pStack + pTask->stackSize-7-1;
}

void APOS_TASK_Create( APOS_TCB_STRUCT* pTask,  	// TaskControlBlock
					#ifdef DEBUG
						const char* pTaskName, 								// Task Name – nur für Debug-Zwecke
					#endif
						uint32_t Priority,  									// Priorität des Tasks (vorerst nicht in Verwendung)
						void (*pRoutine)(void),  							// Startadresse Task (ROM)
						uint32_t* pStack, 												// Startadresse Stack des Tasks (RAM)
						uint32_t StackSize,  									// Größe des Stacks
						uint32_t TimeSlice  									// Time-Slice für Round Robin Scheduling
						)
{
	if(pRoutine == NULL)														// Abbruch wenn keine gültige Funktions-adresse
		return;
	
	if(!pTask)																			// notwendigen Speicher für Task-Daten allokiern
		pTask = calloc(sizeof(APOS_TCB_STRUCT), 1);
	
	
	#ifdef DEBUG
	if(!pTaskName)
		return;
	pTask->pTaskName = calloc(sizeof(char), strlen(pTaskName));
	strcpy(pTask->pTaskName, pTaskName);
	#endif
	pTask->prio = Priority;													// Task-Daten in den control block übernehmen
	pTask->routine = pRoutine;
	pTask->pStack = pStack;
	pTask->timeSlice = TimeSlice;
	pTask->stackSize = StackSize / 4;
	pTask->status = RUNNING;
	pTask->statusTime = Systick_GetTick();
	pTask->delay = 0;
	APOS_STACK_INIT(pTask);
	if(numTasks < maxTasks) {												
		pTasks[numTasks] = pTask;											// in der Task-Liste eintragen
		numTasks++;
	}
}

void APOS_Start(void)  														// Starten des Echtzeitbetriebssystems
{
	// __set_PSP((uint32_t)pTasks[currentTask]->pStack);
	__set_PSP( __get_MSP() );
	__set_CONTROL(2);				// [0]=0 	privileged mode um IRQs enable/disable zu koennen
	//setPendSV();												// [1]=1 thread mode - Alternate stack pointer PSP is used. 
	IS_Initialized = 1;
	// pTasks[currentTask]->routine();				// Scheduler in Endlos-Schleife ausführen
	
	while(1)
		{	}
	
}
						
void APOS_Scheduler(void)
{
	static int lastTick = 0;
	if(IS_Initialized)
	{
		int tick = Systick_GetTick();
		if(lastTick == 0)
		{	lastTick = tick;
			setPendSV();
		}	
		else if((tick - lastTick) >= pTasks[currentTask]->timeSlice || !APOS_Running()) // wenn timeSlice des aktuellen abgelaufen, dann
		{																// Task Switch ausloesen

			if(!verifyStackEnd())			// Intergrität des aktuellen Stacks verifizieren
				{	// __aeabi_assert("corrupt Stack", "APOS.c", 125);
				}

			lastTick = Systick_GetTick();
			setPendSV();																// PendSV Handler auslösen
		} //else 
//		{	
//			pTasks[currentTask]->routine();	 						// akutelle Task-Funktion callen
//		}
	}
}

void APOS_SetPSP(void)
{	
	static uint8_t startup = 0;
	if ( startup == 0 )
	{	startup++;
	}	else{
		pTasks[currentTask]->pStack = (uint32_t*)__get_PSP();
		currentTask++;															// auf nächsten Task schalten
		currentTask %= numTasks;
	}
	__set_PSP((uint32_t)pTasks[currentTask]->pStack);
}

void	setPendSV(void)
{	// NVIC_SetPendingIRQ(PendSV_IRQn); ... wirkungslos, beobachtet beim debuggen
	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

// betreten einer critical region durch deaktivieren aller Interrupts
void APOS_EnterRegion()	// um einen task switch zu verhindern
{	// uint32_t primask = __get_PRIMASK();
	__disable_irq();
	// primask = __get_PRIMASK(); 				...zum debuggen, ob IRQ wirklich deaktiviert wurde
																				// nach disable muss primask == 1 sein
}

// verlassen einer critical region durch aktivieren aller Interrupts
void APOS_LeaveRegion()	// um task switch wieder zu ermöglichen
{	//	uint32_t primask = __get_PRIMASK();
	__enable_irq();	//  wirkungslos?
	// primask = __get_PRIMASK();					...zum debuggen, ob IRQ wirklich wieder aktiviert wurde
																				// nach disable muss primask == 0 sein
}

// prufen, ob critical region schon vergeben ist, durch prüfen 
// return 1 ... region vergeben
// return 0 ... region ist gerade frei
int APOS_TestRegion()	// ob priority mask der IRQs gesetzt wurde
{	uint32_t primask = __get_PRIMASK();		
	return primask;
}	

void APOS_Delay (uint32_t ticks)
{	
	pTasks[currentTask]->status = SUSPENDED;
	pTasks[currentTask]->delay = ticks;
	pTasks[currentTask]->statusTime = Systick_GetTick();
}

static BOOL APOS_Running(void) {
	switch(pTasks[currentTask]->status) {
		case RUNNING:
			return TRUE;
			break;
		case SUSPENDED:
			if((Systick_GetTick() - pTasks[currentTask]->statusTime) > pTasks[currentTask]->delay) {
				pTasks[currentTask]->status = RUNNING;
				pTasks[currentTask]->statusTime = Systick_GetTick();
				pTasks[currentTask]->delay = 0;
				return TRUE;
			}
			return FALSE;
			break;
	}
}




// Testen, ob das Ende des Stacks = "STACK-END", also nicht ueberschrieben wurde
// retourniert '1' bei korrektem Stack, '0' bei Fehler
int verifyStackEnd(void) 
{    
        if(STACK_END != GET_STACK_END(pTasks[currentTask]->pStack, pTasks[currentTask]->stackSize))
                return 0;
        else    return 1;
}