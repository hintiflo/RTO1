
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "Services/StdDef.h"
#include "APOS.h"
#include "BSP/systick.h"

static const uint32_t numTasks = 10;
static APOS_TCB_STRUCT* pTasks[numTasks];

static void copyTasks(APOS_TCB_STRUCT** source, APOS_TCB_STRUCT** dest, uint32_t size)
{
	for(int i = 0; i < size; i++) {
		(*dest)[i] = (*source)[i];
	}
}

void APOS_Init(void)  													// Initialisert das Echtzeitbetriebssystem
{
	for(int i = 0; i < numTasks; i++) {
		pTasks[i] = NULL;
	}
}

void APOS_TASK_Create( APOS_TCB_STRUCT* pTask,  	// TaskControlBlock
#ifdef DEBUG
						const char* pTaskName, 								// Task Name – nur für Debug-Zwecke
#endif
						uint32_t Priority,  									// Priorität des Tasks (vorerst nicht in Verwendung)
						void (*pRoutine)(void),  							// Startadresse Task (ROM)
						void * pStack, 												// Startadresse Stack des Tasks (RAM)
						uint32_t StackSize,  									// Größe des Stacks
						uint32_t TimeSlice  									// Time-Slice für Round Robin Scheduling
						)
{
	static uint32_t currentTask = 0;	

	
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
	if(pStack == NULL)
		pTask->pStack = calloc(sizeof(uint8_t), StackSize);
	else
		pTask->pStack = pStack;
	
	pTask->timeSlice = TimeSlice;
	
	if(currentTask < numTasks)
		pTasks[currentTask] = pTask;

	
}

void APOS_Start(void)  // Starten des Echtzeitbetriebssystems
{
	while(1)
		APOS_Scheduler();
}
						
void APOS_Scheduler(void)
{
	static int lastTick = 0;
	static uint32_t currentTask;
	int tick = Systick_GetTick();
	if(lastTick == 0)
		lastTick = tick;
	
	
	// exec task
	while(tick - lastTick < pTasks[currentTask]->timeSlice) {
		pTasks[currentTask]->routine();
	}
	
	// taskswitch
	currentTask++;
	currentTask %= numTasks;
	
	lastTick = Systick_GetTick();
}
