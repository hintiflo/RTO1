#include "BSP/TftDisplay.h"
#include "StdDef.h"
#include "TaskPoti.h"
#include "stm32f0xx.h"

void TaskPoti (void)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_SET);
	Tft_DrawString(10, 18+4*24, "Poti ");	
	GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_RESET);
}
