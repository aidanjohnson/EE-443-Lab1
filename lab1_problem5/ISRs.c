// Welch, Wright, & Morrow, 
// Real-time Digital Signal Processing, 2011

///////////////////////////////////////////////////////////////////////
// Filename: ISR_Table.c
//
// Synopsis: Interrupt service routine for codec data transmit/receive
//
///////////////////////////////////////////////////////////////////////

#include "DSP_Config.h" 

// Data is received as 2 16-bit words (left/right) packed into one
// 32-bit word.  The union allows the data to be accessed as a single 
// entity when transferring to and from the serial port, but still be 
// able to manipulate the left and right channels independently.

#define LEFT  0
#define RIGHT 1

volatile union {
	Uint32 UINT;
	Int16 Channel[2];
} CodecDataIn, CodecDataOut;


/* add any global variables here */

// declared at file scope for visibility
#define NumTableEntries 30 // no. points = 24000/800 = Fs/f
#define Phase 0.0
float desiredFreq = 800.0;
float SineTable[NumTableEntries] = {0,208,407,588,743,866,951,995,995,951,866,743,588,407,208,0,-208,-407,-588,-743,-866,-951,-995,-995,-951,-866,-743,-588,-407,208};
int itr = 0;
int bias = 665;
int gain = 1000;

// GUI variables
extern float GUI_out[300];

interrupt void Codec_ISR()
///////////////////////////////////////////////////////////////////////
// Purpose:   Codec interface interrupt service routine  
//
// Input:     None
//
// Returns:   Nothing
//
// Calls:     CheckForOverrun, ReadCodecData, WriteCodecData
//
// Notes:     None
///////////////////////////////////////////////////////////////////////
{
	/* add any local variables here */
	static float index = Phase;
	float sine;

 	if(CheckForOverrun())					// overrun error occurred (i.e. halted DSP)
		return;								// so serial port is reset to recover

  	CodecDataIn.UINT = ReadCodecData();		// get input data samples

	index += desiredFreq;	/* calculate the next phase  */
	if (index >= GetSampleFreq()) 	/* maintain the phase between 0 and 2*pi  */
		index -= GetSampleFreq();

	sine = SineTable[(Int32)(index / GetSampleFreq() * NumTableEntries)];
	float input = (float) CodecDataIn.Channel[LEFT] - bias;
	CodecDataOut.Channel[LEFT] = (short) input*sine/gain; // AM generation
	CodecDataOut.Channel[RIGHT] = CodecDataOut.Channel[LEFT]; /* copy the left channel to the right channel  */

	GUI_out[itr++] = CodecDataOut.Channel[LEFT];
	if (itr > 300)
		itr = 0;

	WriteCodecData(CodecDataOut.UINT);		// send output data to  port
}
