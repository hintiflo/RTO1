#include "BSP/TftDisplay.h"
#include "StdDef.h"
#include "TaskWatch.h"


void TaskWatch (void)
{
	Tft_DrawString(10, 18+3*24, "Watch ");	
}
