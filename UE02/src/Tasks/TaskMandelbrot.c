#include "BSP/TftDisplay.h"
#include "StdDef.h"
#include "TaskMandelbrot.h"
#include "stm32f0xx.h"
#include "BSP/systick.h"

// Quelle Algorithmus Mandelbrot: http://warp.povusers.org/Mandelbrot/ 

#define ImageHeight 150
#define ImageWidth  150
#define OFFSET			150

#define INTERVAL_MANDEL 400

static unsigned von = 0;
static unsigned bis = ImageHeight/INTERVAL_MANDEL;
static unsigned intervalCnt = 0;


static void MandelBrot (void)
{
	double MinRe = -2.0;
	double MaxRe = 1.0;
	double MinIm = -1.2;
	double MaxIm = MinIm+(MaxRe-MinRe)*ImageHeight/ImageWidth;
	double Re_factor = (MaxRe-MinRe)/(ImageWidth-1);
	double Im_factor = (MaxIm-MinIm)/(ImageHeight-1);
	unsigned MaxIterations = 30;

	static uint32_t lastTick = 0;
	uint32_t tick = Systick_GetTick();

	unsigned y = von; 
	// bis = ImageHeight/INTERVAL_MANDEL;
	
	while(y<ImageHeight)
	{		
		y++;
			
			double c_im = MaxIm - y*Im_factor;
			for(unsigned x=0; x<ImageWidth; ++x)
			{
				unsigned char isInside = TRUE;

				double c_re = MinRe + x*Re_factor;

					double Z_re = c_re, Z_im = c_im;
					isInside = TRUE;
					for(unsigned n=0; n<MaxIterations; ++n)
					{
							double Z_re2 = Z_re*Z_re, Z_im2 = Z_im*Z_im;
							if(Z_re2 + Z_im2 > 4)
							{
									isInside = FALSE;
									break;
							}
							Z_im = 2*Z_re*Z_im + c_im;
							Z_re = Z_re2 - Z_im2 + c_re;
					}
				
				if(isInside) { Tft_DrawPixel(y, x + OFFSET); }
			}
			if(y > bis)			
			{
				intervalCnt++;
				von = bis;
				bis = intervalCnt*ImageHeight/INTERVAL_MANDEL;
				break;				
			}
	}
}

void TaskMandelbrot (void)
{	Tft_DrawString(10, 18+5*24, "ManBr ");
	MandelBrot();	
}
