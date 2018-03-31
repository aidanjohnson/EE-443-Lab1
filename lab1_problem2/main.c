// Welch, Wright, & Morrow, 
// Real-time Digital Signal Processing, 2011
 
///////////////////////////////////////////////////////////////////////
// Filename: main.c
//
// Synopsis: Main program file for demonstration code
//
///////////////////////////////////////////////////////////////////////

#include "DSP_Config.h"

int main()
{
	// initialize DSP board
	DSP_Init();

	// call StartUp for application specific code
	// defined in each application directory
	StartUp();

	// Plays sine wave given DIP switch input, and outputs to indicator LED.
	// main stalls here, interrupts drive operation
  	while(1) {
  		Int32 SW = ReadSwitches(); // check switches: SW5 = 2^0; ; 0 = up, 1 = down
  		if(SW % 2 == 0) { // if DIP SW5 0 or up:
  			// enable sine wave; turn on (1) LED D4
  			;
  			WriteLEDs(1); // D4 = 2^0; 0 = off, 1 = on
  		}
  		else { // else (SW5 down or 1): disable sine wave; turn off (0) D4
  			WriteCodecData(0);
  			WriteLEDs(0);
  		}
  	}
}




