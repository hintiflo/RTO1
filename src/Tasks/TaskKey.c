#include "BSP/TftDisplay.h"
#include "StdDef.h"
#include "TaskKey.h"
#include "stm32f0xx.h"

void TaskKey (void)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_3, Bit_SET);
	Tft_DrawString(10, 18+1*24, "Key ");	
	GPIO_WriteBit(GPIOB, GPIO_Pin_3, Bit_RESET);

}
