#include "BSP/TftDisplay.h"
#include "StdDef.h"
#include "TaskLed.h"
#include "stm32f0xx.h"


void TaskLed (void)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET);
	Tft_DrawString(10, 18+2*24, "Led ");	
	GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_RESET);
}
