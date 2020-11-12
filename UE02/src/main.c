#include "Services/StdDef.h"
#include "BSP/Key.h"
#include "BSP/Led.h"
#include "BSP/Adc.h"
#include "BSP/systick.h"
#include "stm32f0xx_gpio.h"
#include "BSP/TftDisplay.h"
#include "Fonts/TftFont_16x24.h"
#include "TaskAll.h"

#include "TestTask.h"
#include "APOS.h"

#include <stdio.h>


// Abstraction of GPIO-Manipulations into LED-Functions
static void setSysTickLED()		{	GPIO_WriteBit(GPIOB, GPIO_Pin_0,	Bit_SET);}
static void resSysTickLED()		{	GPIO_WriteBit(GPIOB, GPIO_Pin_0,	Bit_RESET);}

static void setCounterLED()		{	GPIO_WriteBit(GPIOB, GPIO_Pin_1,	Bit_SET);}
static void resCounterLED()		{	GPIO_WriteBit(GPIOB, GPIO_Pin_1,	Bit_RESET);}

static void setKeyLED()			{	GPIO_WriteBit(GPIOB, GPIO_Pin_3,	Bit_SET);}
static void resKeyLED()			{	GPIO_WriteBit(GPIOB, GPIO_Pin_3,	Bit_RESET);}

static void setLedLED()			{	GPIO_WriteBit(GPIOB, GPIO_Pin_12,	Bit_SET);}	
static void resLedLED()			{	GPIO_WriteBit(GPIOB, GPIO_Pin_12,	Bit_RESET);}	

static void setWatchLED()		{	GPIO_WriteBit(GPIOB, GPIO_Pin_13,	Bit_SET);}
static void resWatchLED()		{	GPIO_WriteBit(GPIOB, GPIO_Pin_13,	Bit_RESET);}

static void setPotiLED()		{	GPIO_WriteBit(GPIOB, GPIO_Pin_14,	Bit_SET);}	
static void resPotiLED()		{	GPIO_WriteBit(GPIOB, GPIO_Pin_14,	Bit_RESET);}	

static void setMandelbrotLED()	{	GPIO_WriteBit(GPIOB, GPIO_Pin_15,	Bit_SET);}		
static void resMandelbrotLED()	{	GPIO_WriteBit(GPIOB, GPIO_Pin_15,	Bit_RESET);}		


static void sleep()
{
	for(int i = 0; i<10000; i++)
		__NOP;
}


// sequential setting/resetting of GPIOs to help during wiring of the logic analyzer
// additional sleep-phase in the end for snycronosation with Analyzer-View
static void tryGPIO()	
{
	while(1)
	{
			setSysTickLED();
		sleep();
			resSysTickLED();
			setCounterLED();
		sleep();
			resCounterLED();
			setKeyLED();			
		sleep();
			resKeyLED();			
			setLedLED();			
		sleep();
			resLedLED();			
			setWatchLED();	
		sleep();
			resWatchLED();	
			setPotiLED()	;	
		sleep();
			resPotiLED()	;	
			setMandelbrotLED();	
		sleep();
			resMandelbrotLED();	
		sleep();
		sleep();
		sleep();
		sleep();
		sleep();
		sleep();
	}
}
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
	
	// tryGPIO();
	
	// Hinweise von Langer aus der letzten Ü:
//	PSP porcess stack pointer
//	MSP master stack pointer
//	PndSV interrupt for context switch
//	pTask enthält zB stackpointer
//	TimeSlice = wieviel Slices möcht ich für den Task
// bei HardFaults: Stack, Speicher, Register anschauen, Screenshots machen beim Debuggen, wie is vorher/nacher
//	sysTick auf 1 ms
//	sysTick setzt PenSV (handelt auch Interrupts, die ich nicht behandle zB SPIs oä)
//	-> interrupt
//	-> APUS Scheduler
	
//	Context Switch, sichern:
//		SPH SPL
//		Register R4...R11
//		PC
	
//	befüllen:
//		xPSR 						enthält zB T-Switch
//		ProgramCounter	zum rückspringen
	
	// 0xFFFFFFFD ins LR beim rücksprung?

	
	
	
  while (1)
  {	
		FillTaskA();
		FillTaskB();
		FillTaskC();

		setSysTickLED();

		setCounterLED();
		TaskCounter();
		resCounterLED();
		
		setKeyLED();
		TaskKey();
		resKeyLED();

		setLedLED();
		TaskLed();	
		resLedLED();

		setWatchLED();
		TaskWatch();
		resWatchLED();

		setPotiLED();
		TaskPoti();	
		resPotiLED();


		 setMandelbrotLED();
		 TaskMandelbrot();		
		 resMandelbrotLED();		

		resSysTickLED();
  }
}


// void TaskA (void) 
// {
// 	FillTaskA();
// 	// while (1) 
// 	// { Debug_TaskOn_A();
// 		// enter Code
// 		// Debug_TaskOff_A();
// 		// APOS_Scheduler();
// 	// 	}
// }

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
