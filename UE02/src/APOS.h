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

// setzt des PendSV Bit um einen TaskSwitch auszuloesen
void	setPendSV(void);


// Schritt 4:
void APOS_EnterRegion(void);
void APOS_LeaveRegion(void);
int	 APOS_TestRegion(void);

// Schritt 5:
void APOS_Delay (uint32_t ticks);
	 

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
						const char* pTaskName, 								// Task Name – nur für Debug-Zwecke
					#endif
						uint32_t Priority,  									// Priorität des Tasks (vorerst nicht in Verwendung)
						void (*pRoutine)(void),  							// Startadresse Task (ROM)
						void* pStack, 												// Startadresse Stack des Tasks (RAM)
						uint32_t StackSize,  									// Größe des Stacks
						uint32_t TimeSlice  									// Time-Slice für Round Robin Scheduling
						);

void APOS_Start(void);  // Starten des Echtzeitbetriebssystems
void APOS_Scheduler(void);  // OS Scheduler
void APOS_SetPSP(void);
						
#endif // __APOS
