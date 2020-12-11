#ifndef __APOS
#define __APOS


#include "Services/StdDef.h"
// #include "BSP/systick.h"
// #include "stm32f0xx_gpio.h"
// #include "TaskAll.h"
// #include <stdio.h>

// for debugging
#define DEBUG

typedef void (*Routine)(void);


// Schritt 1:
void TaskA(void);
void TaskB(void);
void TaskC(void);
void FillTaskA(void);
void FillTaskB(void);
void FillTaskC(void);

// set the PendSV to trigger a task Switch
void	setPendSV(void);


// Schritt 4:

// void APOS_EnterRegion(){};
// void APOS_LeaveRegion(){}; 
// void APOS_TestRegion(){};

// Schritt 5:
// void APOS_Delay (uint32_t ticks){};
	 

typedef struct
{
	uint32_t prio;
	Routine routine;
	uint32_t pStack;
	uint32_t stackSize;
	uint32_t timeSlice;
#ifdef DEBUG
	char * pTaskName;
#endif
	
}APOS_TCB_STRUCT;

void APOS_Init(void);  													// Initialisert das Echtzeitbetriebssystem




void APOS_TASK_Create( APOS_TCB_STRUCT* pTask,  	// TaskControlBlock
#ifdef DEBUG
						const char* pTaskName, 								// Task Name � nur f�r Debug-Zwecke
#endif
						uint32_t Priority,  									// Priorit�t des Tasks (vorerst nicht in Verwendung)
						void (*pRoutine)(void),  							// Startadresse Task (ROM)
						void* pStack, 												// Startadresse Stack des Tasks (RAM)
						uint32_t StackSize,  									// Gr��e des Stacks
						uint32_t TimeSlice  									// Time-Slice f�r Round Robin Scheduling
						);

void APOS_Start(void);  // Starten des Echtzeitbetriebssystems
void APOS_Scheduler(void);  // OS Scheduler
// ...um den Task-Switch zu triggern. Das Triggern des Task-Switchs erfolgt durch Setzen des 
// PEND-SV Interrupt Pending Bits. Der Task-Switch wird im PendSV_Handler programmiert.
// Die Priorit�t des PendSV_Handler soll auf den geringsten Wert eingestellt werden
void APOS_SetPSP(void);
						
#endif // __APOS
