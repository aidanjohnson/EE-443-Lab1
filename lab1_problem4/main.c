// Welch, Wright, & Morrow, 
// Real-time Digital Signal Processing, 2011
 
///////////////////////////////////////////////////////////////////////
// Filename: main.c
//
// Synopsis: Main program file for demonstration code
//
///////////////////////////////////////////////////////////////////////

#include "DSP_Config.h"   

Int alpha = 1;
Int beta = 1;
Int gamma = 1;
Int GUI_out;

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
