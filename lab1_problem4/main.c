// Welch, Wright, & Morrow, 
// Real-time Digital Signal Processing, 2011
 
///////////////////////////////////////////////////////////////////////
// Filename: main.c
//
// Synopsis: Main program file for demonstration code
//
///////////////////////////////////////////////////////////////////////

#include "DSP_Config.h"   

int alpha = 0;
int beta = 0;
int gamma = 0;
float GUI_out[300];

int main()
{
	// initialize DSP board
  	DSP_Init();

	// call StartUp for application specific code
	// defined in each application directory
	StartUp();

	// main stalls here, interrupts drive operation
  	while(1) {
  		;
  	}
}
