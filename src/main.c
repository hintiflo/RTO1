#include "Services/StdDef.h"
#include "BSP/Key.h"
#include "BSP/Led.h"
#include "BSP/Adc.h"
#include "BSP/systick.h"
#include "stm32f0xx_gpio.h"
#include "BSP/TftDisplay.h"
#include "Fonts/TftFont_16x24.h"
#include "TaskAll.h"

#include <stdio.h>

void GPIO_setup( void );
	
	
int main(void)
{
  Key_Init();
  Led_Init();
  Tft_Init();
  Tft_SetFont(&TftFont_16x24);
	Tft_ClearScreen();	
  Adc_Init(ADC_CHANNEL_POTENTIOMETER);	
	Tick_InitSysTick();
	
	GPIO_setup();
	
	
	
	
  while (1)
  {
		
		
	// GPIO_SetBits(GPIOB, GPIO_Pin_0);
	// void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
		
		TaskCounter();
		TaskKey();
		TaskLed();	
		TaskWatch();
		TaskPoti();	

		// TaskMandelbrot();		
  }
}

void GPIO_setup( void )
{

	
	/* Enable clock for LED gpio port registers */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

  /* Init necessary GPIOs */
  GPIO_InitTypeDef gpioInit;
  GPIO_StructInit(&gpioInit);
  
  gpioInit.GPIO_Mode = GPIO_Mode_OUT;
  gpioInit.GPIO_OType = GPIO_OType_PP;
  gpioInit.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;

  GPIO_Init(GPIOB, &gpioInit);
  
}

/* #define NDEBUG to ignore all asserts */
#define ASSERT_BUFF_SIZE 150u
/**
 * @brief  Reports the name of the source file and the source line number
 *					where the assert error has occurred.
 * @param	expr: expression that resulted to false
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
*/
void __aeabi_assert(const char * expr, const char * file, int line)
{
  /* User can add his own implementation to report the expression, file name
	 and line number, ex:
	 printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  volatile uint32_t linev;
	/* Buffer size, because of long file names */
  volatile uint8_t tmpBuf[ASSERT_BUFF_SIZE];

	if(expr != NULL_PTR && file != NULL_PTR)
	{
		for (uint8_t i = 0; i < ASSERT_BUFF_SIZE; i++)
		{
			/* insert expression */
			if((*expr) != '\0')
			{
				tmpBuf[i] = *expr++;
			}
			/* insert filename */
			else if((*file) != '\0')
			{
				tmpBuf[i] = *file++;
			}
		}
	}
	linev = line;

  /* Infinite loop */
	/* Now Debug with debugger (tmpBuf, Linev) */
  while (1);
}
