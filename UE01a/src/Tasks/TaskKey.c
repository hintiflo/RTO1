#include "BSP/TftDisplay.h"
#include "StdDef.h"
#include "TaskKey.h"
#include "stm32f0xx.h"

void TaskKey (void)
{
	Tft_DrawString(10, 18+1*24, "Key ");	

}