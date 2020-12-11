#include "BSP/TftDisplay.h"
#include "StdDef.h"
#include "TaskLed.h"
#include "stm32f0xx.h"
#include "BSP/Led.h"
#include "BSP/Systick.h"
#include	"APOS.h"

#define INTERVAL_LED 100

#define NUM_LEDS 3
static LEDType LED[NUM_LEDS] = {  LEDType_LED3, LEDType_LED4, LEDType_LED5};


void TaskLed (void)
{
	//Tft_DrawString(10, 18+2*24, "Led ");	
	static uint32_t tick, lastTick;
	static uint8_t i = NUM_LEDS;
	while(1)
	{
		tick = Systick_GetTick();
		if((tick-lastTick) >= INTERVAL_LED)
		{
			Led_TurnOff(LED[i]);
			i = (i + 1) % NUM_LEDS;
			Led_TurnOn(LED[i]);
			lastTick = tick;
		}
		//APOS_Scheduler();
	}
}
