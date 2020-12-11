#include "BSP/TftDisplay.h"
#include "StdDef.h"
#include "TaskKey.h"
#include "stm32f0xx.h"
#include "BSP/Key.h"
#include <string.h>


#define NUM_COLORS 3
static const uint16_t color[NUM_COLORS] = { TFT_COLOR_BLACK, TFT_COLOR_GREEN, TFT_COLOR_BLUE };

#define LEN_KEY 4
#define LEN_WAK (LEN_KEY+7)
#define LEN_USR (LEN_KEY+6)
#define MAX_OUTPUT (LEN_WAK+1)
static char output[MAX_OUTPUT];

void TaskKey (void)
{
	static int i = 0;
	static BOOL printKey = TRUE;
	BOOL pressed = FALSE, print = FALSE;

	if(Key_GetState(KeyType_WAKEUP)) {
		strncpy(output, "Key: WAKEUP", LEN_WAK);
		pressed = TRUE;
		print = TRUE;
		printKey = TRUE;
	}
	
	if(Key_GetState(KeyType_USER0)) {
		Tft_SetForegroundColourRgb16(color[i]);
		i = (i + 1) % 3;
		strncpy(output, "Key: USER0 ", LEN_WAK);
		pressed = TRUE;
		print = TRUE;
		printKey = TRUE;
	}
	
	if(Key_GetState(KeyType_USER1)) {
		strncpy(output, "Key: USER1 ", LEN_WAK);
		pressed = TRUE;
		print = TRUE;
		printKey = TRUE;
	}
	
	if(!pressed ) {
		if(printKey) {
			strncpy(output, "Key:       ", LEN_WAK);
			print = TRUE;
			printKey = FALSE;
		}
	}
	
	if(print) {
		Tft_DrawString(10, 18+1*24, output);
		print = FALSE;
	}

}
