#include "BSP/TftDisplay.h"
#include "StdDef.h"
#include "TaskPoti.h"
#include "stm32f0xx.h"

void TaskPoti (void)
{
	
	Tft_DrawString(10, 18+4*24, "Poti ");	
	
}