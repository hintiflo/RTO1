#ifndef __TICK_H
#define __TICK_H
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void 						Tick_Delay (uint32_t dlyTicks);
void						Tick_InitSysTick(void);
void 						SysTick_Handler (void);
int 						Systick_GetTick(void);

#endif /* __TICK_H */
