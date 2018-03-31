// Welch, Wright, & Morrow, 
// Real-time Digital Signal Processing, 2011

///////////////////////////////////////////////////////////////////////
// Filename: ISRs.c
//
// Synopsis: Interrupt service routine for codec data transmit/receive
//
///////////////////////////////////////////////////////////////////////

#include "DSP_Config.h" 
  
// Data is received as 2 16-bit words (left/right) packed into one
// 32-bit word.  The union allows the data to be accessed as a single 
// entity when transferring to and from the serial port, but still be 
// able to manipulate the left and right channels independently.

#define LEFT 0
#define RIGHT 1

volatile union {
	Uint32 ABC;
	Int16 Channel[2];
} CodecDataIn, CodecDataOut;

interrupt void Codec_ISR()
{
///////////////////////////////////////////////////////////////////////
// Purpose:   Codec interface interrupt service routine  
//
// Input:     None
//
// Returns:   Nothing
//
// Calls:     ReadCodecData, WriteCodecData
//
// Notes:     Plays input (line in) audio given switch controls
//			  for channels to line out
///////////////////////////////////////////////////////////////////////

	CodecDataIn.ABC = ReadCodecData();

	CodecDataOut.Channel[LEFT] = CodecDataIn.Channel[LEFT];
	CodecDataOut.Channel[RIGHT]= CodecDataIn.Channel[RIGHT];

	Int32 SW = ReadSwitches();

	if(SW % 2 == 1) // ignore SW5 (= 2^0)
		SW -= 1;

	// check switches: SW6 = 2^1, SW7 = 2^2; 0 = up, 1 = down
	// LEDs: D4 = 2^0, D5 = 2^1; 0 = off, 1 = on
	if(SW == 2) { // SW6 down, SW7 up: right out off, D4 off and D5 on
		CodecDataOut.Channel[RIGHT]= 0;
		WriteLEDs(2);
	}
	else if(SW == 4) { // SW6 up, SW7 down: left out off, D4 on and D5 off
		CodecDataOut.Channel[LEFT] = 0;
		WriteLEDs(1);
	}
	else if(SW == 6) { // SW6 and SW7 down: right and left out off, D4 and D5 off
		CodecDataOut.Channel[LEFT] = 0;
		CodecDataOut.Channel[RIGHT]= 0;
		WriteLEDs(0);
	}
	else if(SW == 0) { // SW6 and SW7 up: right and left out on, D4 and D5 on
		WriteLEDs(3);
	}

	WriteCodecData(CodecDataOut.ABC); // write to line out
}

