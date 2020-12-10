#include "BSP/TftDisplay.h"
#include "StdDef.h"
#include "TaskMandelbrot.h"
#include "stm32f0xx.h"
#include "BSP/systick.h"

#include	"APOS.h"

// Quelle Algorithmus Mandelbrot: http://warp.povusers.org/Mandelbrot/ 

#define ImageHeight 150
#define ImageWidth  150
#define OFFSET			150
#define TEXT_INTER 500

static void MandelBrot (void)
{
	static unsigned y = 0;
	static unsigned x = 0;
	static unsigned n = 0;
	static unsigned char isInside = TRUE;
	static double c_im = 0.0;
	static double c_re = 0.0;
	static double Z_re = 0.0;
	static double Z_im = 0.0;
	static double Z_re2 = 0.0;
	static double Z_im2 = 0.0;
	
	double MinRe = -2.0;
	double MaxRe = 1.0;
	double MinIm = -1.2;
	double MaxIm = MinIm+(MaxRe-MinRe)*ImageHeight/ImageWidth;
	double Re_factor = (MaxRe-MinRe)/(ImageWidth-1);
	double Im_factor = (MaxIm-MinIm)/(ImageHeight-1);
	unsigned MaxIterations = 30;


	if(y<ImageHeight)
	{
			if(x == 0 && n == 0)
				c_im = MaxIm - y*Im_factor;

			if(x<ImageWidth)
			{
				if(n == 0) {
					c_re = MinRe + x*Re_factor;
					Z_re = c_re, Z_im = c_im;
				}
					if(n<MaxIterations)
					{
							Z_re2 = Z_re*Z_re, Z_im2 = Z_im*Z_im;
							if(Z_re2 + Z_im2 > 4)
							{
									isInside = FALSE;
									n = MaxIterations; // break iterations
							} else {
								Z_im = 2*Z_re*Z_im + c_im;
								Z_re = Z_re2 - Z_im2 + c_re;
								n++;
							}

					} else {
						if(isInside) { 
						Tft_DrawPixel(y, x + OFFSET);
						}
						isInside = TRUE;
						x++;
						n = 0;
					}
			} else {
				x = 0;
				y++;
				c_im = MaxIm - y*Im_factor;
			}
	} else {
		y = 0;
	}
}


void TaskMandelbrot (void)
{
	static uint32_t last = 0;

	if((Systick_GetTick() - last) > TEXT_INTER) {
		Tft_DrawString(10, 18+5*24, "ManBr ");
		last = Systick_GetTick();
	}

		while (1) 
		{
			MandelBrot();	
			// enter Code
			//APOS_Scheduler();
		}
}