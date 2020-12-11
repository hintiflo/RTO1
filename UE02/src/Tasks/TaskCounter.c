#include "BSP/TftDisplay.h"
#include "StdDef.h"
#include "TaskCounter.h"
#include <stdio.h>
#include "stm32f0xx_gpio.h"
#include "BSP/systick.h"
#include	"APOS.h"

#define INTERVAL_COUNT 5

#define MAX_LEN 14		// Maximale Anzahl Zeichen pro Zeile
static char tmpBuf[MAX_LEN];

void TaskCounter (void)
{
	static uint32_t counter = 0;
	
	while(1)
	{
		counter++;
		snprintf(tmpBuf, MAX_LEN, "%d", counter);
		APOS_EnterRegion();
		Tft_DrawString(10, 18+0*24, "Cnt ");
		Tft_DrawString(10 + 16*7, 18+0*24, tmpBuf);
		APOS_LeaveRegion();
		//APOS_Scheduler();

	}
	
}
