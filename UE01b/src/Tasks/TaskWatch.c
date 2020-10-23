#include "BSP/TftDisplay.h"
#include "StdDef.h"
#include "TaskWatch.h"
#include "stm32f0xx.h"
#include "BSP/systick.h"
#include <stdio.h>

#define INTERVAL_WATCH 2
#define NUM_CHAR 14
static char output[NUM_CHAR];

void TaskWatch (void)
{
	static uint32_t lastTick = 0;
	uint32_t tick = Systick_GetTick();
	
	if((tick-lastTick) >= INTERVAL_WATCH) {
		uint8_t sec = tick / 1000;
		uint8_t min = sec / 60;
		
		sec %= 60;
		
		snprintf(output, NUM_CHAR, "Watch: %02d:%02d", min, sec);

		Tft_DrawString(10, 18+3*24, output);	
	}
}