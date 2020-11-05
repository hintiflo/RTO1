#include "Services/StdDef.h"
#include "APOS.h"

void APOS_Init(void)  													// Initialisert das Echtzeitbetriebssystem
{


}

void APOS_TASK_Create( APOS_TCB_STRUCT* pTask,  	// TaskControlBlock
						const char* pTaskName, 								// Task Name – nur für Debug-Zwecke
						uint32_t Priority,  									// Priorität des Tasks (vorerst nicht in Verwendung)
						void (*pRoutine)(void),  							// Startadresse Task (ROM)
						void * pStack, 												// Startadresse Stack des Tasks (RAM)
						uint32_t StackSize,  									// Größe des Stacks
						uint32_t TimeSlice  									// Time-Slice für Round Robin Scheduling
						)
{


}

void APOS_Start(void)  // Starten des Echtzeitbetriebssystems
{


}
						
void APOS_Scheduler(void)
{


}
