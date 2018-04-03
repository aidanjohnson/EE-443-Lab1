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

int volume = 100;
int ii=0;
// samples per period = (sampling frequency)/(sine frequency)
// 10 = 12 kHz / 1.2 kHz = number of samples
// sin(2*pi*ii/10)
short sinewave[10]={0,588,951,951,588,0,-588,-951,-951,-588};

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
// Notes:     Plays sine wave tone
///////////////////////////////////////////////////////////////////////

	CodecDataOut.Channel[LEFT] = 0; // zero out the lower 2 bytes
	CodecDataOut.Channel[RIGHT]= volume*sinewave[ii++]; // upper 2 bytes
	if(ii>9) ii=0; // number of samples - 1

	WriteCodecData(CodecDataOut.ABC);// send output data to port

}

