
#include "APOS.h"

void FillTaskA(void)
{

}


void TaskA (void) 
{
		FillTaskA();
		while (1) 
		{
			// Debug_TaskOn_A();
			// enter Code
			// Debug_TaskOff_A();
			APOS_Scheduler();
		}
}



void		FillTaskB(void)
{

}

void TaskB (void) 
{
		FillTaskB();
		while (1) 
		{
			// Debug_TaskOn_B();
			// enter Code
			// Debug_TaskOff_B();
			APOS_Scheduler();
		}
}


void		FillTaskC(void)
{

}

void TaskC (void) 
{
		FillTaskC();
		while (1) 
		{
			// Debug_TaskOn_C();
			// enter Code
			// Debug_TaskOff_C();
			APOS_Scheduler();
		}
}

