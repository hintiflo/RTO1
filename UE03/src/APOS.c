
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

void Debug_TaskOn_A(){	GPIO_WriteBit(GPIOB, GPIO_Pin_5,	Bit_SET);		};
void Debug_TaskOff_A(){	GPIO_WriteBit(GPIOB, GPIO_Pin_5,	Bit_RESET);	};
void Debug_TaskOn_B(){	GPIO_WriteBit(GPIOB, GPIO_Pin_6,	Bit_SET);		};
void Debug_TaskOff_B(){	GPIO_WriteBit(GPIOB, GPIO_Pin_6,	Bit_RESET);	};
void Debug_TaskOn_C(){ 	GPIO_WriteBit(GPIOB, GPIO_Pin_7,	Bit_SET);		};
void Debug_TaskOff_C(){	GPIO_WriteBit(GPIOB, GPIO_Pin_7,	Bit_RESET);	};

void TaskA (void) 
{		FillTaskA();
		while (1) 
		{
			Debug_TaskOn_A();
			// enter Code
			Debug_TaskOff_A();
			APOS_Scheduler();
		}
}

void TaskB (void) 
{		FillTaskB();
		while (1) 
		{ Debug_TaskOn_B();
			// enter Code
			Debug_TaskOff_B();
			APOS_Scheduler();
		}
}

void TaskC (void) 
{		FillTaskC();
		while (1) 
		{ Debug_TaskOn_C();
			// enter Code
			Debug_TaskOff_C();
			APOS_Scheduler();
		}
}



static void copyTasks(APOS_TCB_STRUCT** source, APOS_TCB_STRUCT** dest, uint32_t size)
{	for(int i = 0; i < size; i++) 
	{	(*dest)[i] = (*source)[i];
	}
}

void APOS_Init(void)  													// Initialisert das Echtzeitbetriebssystem
{	for(int i = 0; i < numTasks; i++) 
	{	pTasks[i] = NULL;
	}
}

void APOS_TASK_Create( APOS_TCB_STRUCT* pTask,  	// TaskControlBlock
#ifdef DEBUG
						const char* pTaskName, 								// Task Name – nur für Debug-Zwecke
#endif
						uint32_t Priority,  									// Priorität des Tasks (vorerst nicht in Verwendung)
						void (*pRoutine)(void),  							// Startadresse Task (ROM)
						void* pStack, 												// Startadresse Stack des Tasks (RAM)
						uint32_t StackSize,  									// Größe des Stacks
						uint32_t TimeSlice  									// Time-Slice für Round Robin Scheduling
						)
{

	
	if(pRoutine == NULL)
		return;
	
	if(!pTask)
		pTask = calloc(sizeof(APOS_TCB_STRUCT), 1);
	
#ifdef DEBUG
	if(!pTaskName)
		return;
	pTask->pTaskName = calloc(sizeof(char), strlen(pTaskName));
	strcpy(pTask->pTaskName, pTaskName);
#endif

	
	pTask->prio = Priority;
	pTask->routine = pRoutine;
//	if(pStack == NULL)
//		pTask->pStack = calloc(sizeof(uint8_t), StackSize);
//	else
		pTask->pStack = (uint32_t)pStack;
	
	pTask->timeSlice = TimeSlice;
	
	if(numTasks < maxTasks) {
		pTasks[numTasks] = pTask;
		numTasks++;
	}

	
}

void APOS_Start(void)  // Starten des Echtzeitbetriebssystems
{
	APOS_SetPSP();
	__set_CONTROL(1);
	while(1)
		APOS_Scheduler();
}
						
void APOS_Scheduler(void)
{
	static int lastTick = 0;

	int tick = Systick_GetTick();
	if(lastTick == 0)
		lastTick = tick;
	

	

	
	// switch task
	if((tick - lastTick) >= pTasks[currentTask]->timeSlice) {
		currentTask++;
		currentTask %= numTasks;
		lastTick = Systick_GetTick();
		setPendSV();
	} else { // exec task
		pTasks[currentTask]->routine();
	}

}

void APOS_SetPSP(void)
{
	__set_PSP(pTasks[currentTask]->pStack);
}

void	setPendSV(void)
{
  SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
  
	// SCB->ICSR &= ~SCB_ICSR_PENDSVSET_Msk;	// clear PendSV, useful for Handler
}

