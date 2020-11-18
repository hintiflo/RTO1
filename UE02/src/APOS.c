
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


static void copyTasks(APOS_TCB_STRUCT** source, APOS_TCB_STRUCT** dest, uint32_t size)
{	for(int i = 0; i < size; i++) 
	{	(*dest)[i] = (*source)[i];
	}
}

void APOS_Init(void)  													// Initialisert das Echtzeitbetriebssystem
{	
	NVIC_SetPriority(PendSV_IRQn, 0xF0 );					// h�chster Wert: kleinste Prio
																								// 0xF im oberen nibble: 0b11110000
	for(int i = 0; i < numTasks; i++) 
	{		pTasks[i] = NULL;
	}
	
}

void APOS_TASK_Create( APOS_TCB_STRUCT* pTask,  	// TaskControlBlock
					#ifdef DEBUG
						const char* pTaskName, 								// Task Name � nur f�r Debug-Zwecke
					#endif
						uint32_t Priority,  									// Priorit�t des Tasks (vorerst nicht in Verwendung)
						void (*pRoutine)(void),  							// Startadresse Task (ROM)
						void* pStack, 												// Startadresse Stack des Tasks (RAM)
						uint32_t StackSize,  									// Gr��e des Stacks
						uint32_t TimeSlice  									// Time-Slice f�r Round Robin Scheduling
						)
{
	if(pRoutine == NULL)														// Abbruch wenn keine g�ltige Funktions-adresse
		return;
	
	if(!pTask)																			// notwendigen Speicher f�r Task-Daten allokiern
		pTask = calloc(sizeof(APOS_TCB_STRUCT), 1);
	
	#ifdef DEBUG
	if(!pTaskName)
		return;
	pTask->pTaskName = calloc(sizeof(char), strlen(pTaskName));
	strcpy(pTask->pTaskName, pTaskName);
	#endif
	pTask->prio = Priority;													// Task-Daten in den control block �bernehmen
	pTask->routine = pRoutine;
	pTask->pStack = (uint32_t)pStack;
	pTask->timeSlice = TimeSlice;
	
	if(numTasks < maxTasks) {												
		pTasks[numTasks] = pTask;											// in der Task-Liste eintragen
		numTasks++;
	}
}

void APOS_Start(void)  														// Starten des Echtzeitbetriebssystems
{
	APOS_SetPSP();
	__set_CONTROL(1);
	while(1)
		APOS_Scheduler();															// Scheduler in Endlos-Schleife ausf�hren
}
						
void APOS_Scheduler(void)
{
	static int lastTick = 0;
	int tick = Systick_GetTick();
	if(lastTick == 0)
		lastTick = tick;
	
	
	if((tick - lastTick) >= pTasks[currentTask]->timeSlice) // wenn timeSlice des aktuellen abgelaufen, dann
	{	currentTask++;															// auf n�chsten Task schalten
		currentTask %= numTasks;
		lastTick = Systick_GetTick();
		setPendSV();																// PendSV Handler ausl�sen
	} else 
	{	pTasks[currentTask]->routine();	 						// akutelle Task-Funktion callen
	}
}

void APOS_SetPSP(void)
{	__set_PSP(pTasks[currentTask]->pStack);
}

void	setPendSV(void)
{ SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

