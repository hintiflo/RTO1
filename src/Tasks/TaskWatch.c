#include "BSP/TftDisplay.h"
#include "StdDef.h"
#include "TaskWatch.h"
#include "stm32f0xx.h"


void TaskWatch (void)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_13, Bit_SET);
	Tft_DrawString(10, 18+3*24, "Watch ");	
	GPIO_WriteBit(GPIOB, GPIO_Pin_13, Bit_RESET);
}
