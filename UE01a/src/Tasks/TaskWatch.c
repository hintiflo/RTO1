#include "BSP/TftDisplay.h"
#include "StdDef.h"
#include "TaskWatch.h"
#include "stm32f0xx.h"


void TaskWatch (void)
{
	

	Tft_DrawString(10, 18+3*24, "Watch ");	

}
