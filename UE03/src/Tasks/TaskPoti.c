#include "BSP/TftDisplay.h"
#include "StdDef.h"
#include "TaskPoti.h"
#include <stdio.h>
#include "stm32f0xx.h"
#include "BSP/Adc.h"
#include "APOS.h"

#define MAX_ADC_LEN 14		// Maximale Anzahl Zeichen pro Zeile
static char adcBuf[MAX_ADC_LEN];


void TaskPoti (void)
{
	// @param timeout Timeout to wait for the end of the conversion.
								 // A value of 0 will cause infinite timeout.
	// @return The adc conversion value in mV or
	// Adc_GetValue(uint32_t const timeout);
	
	int32_t adcValue = 0;
	adcValue = Adc_GetValue(0);
	
	snprintf(adcBuf, MAX_ADC_LEN, "%d", adcValue);
	APOS_EnterRegion();
	Tft_DrawString(10, 18+4*24, "Poti ");	
	Tft_DrawString(10 + 16*7, 18+4*24, adcBuf);		
	APOS_LeaveRegion();
		while (1) 
		{
				int32_t adcValue = 0;
				adcValue = Adc_GetValue(0);
				
				snprintf(adcBuf, MAX_ADC_LEN, "%d", adcValue);
				APOS_EnterRegion();
				Tft_DrawString(10, 18+4*24, "Poti ");	
				Tft_DrawString(10 + 16*7, 18+4*24, adcBuf);		
				APOS_LeaveRegion();
			//APOS_Scheduler();
		}
}






	


