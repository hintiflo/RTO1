#include "BSP/TftDisplay.h"
#include "StdDef.h"
#include "TaskLed.h"


void TaskLed (void)
{
	Tft_DrawString(10, 18+2*24, "Led ");	
}
