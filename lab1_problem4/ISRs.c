// Welch, Wright, & Morrow, 
// Real-time Digital Signal Processing, 2011

///////////////////////////////////////////////////////////////////////
// Filename: ISRs.c
//
// Synopsis: Interrupt service routine for codec data transmit/receive
//
///////////////////////////////////////////////////////////////////////

#include "DSP_Config.h"
#include <math.h>
  
// Data is received as 2 16-bit words (left/right) packed into one
// 32-bit word.  The union allows the data to be accessed as a single 
// entity when transferring to and from the serial port, but still be 
// able to manipulate the left and right channels independently.

#define LEFT 0
#define RIGHT 1

volatile union {
	Uint32 UINT;
	Int16 Channel[2];
} CodecDataIn, CodecDataOut;

extern int alpha;
extern int beta;
extern int gamma;
extern float GUI_out[300];

#define NumTableEntries 100
float f1 = 12000.0;
float f2 = 8000.0;
float f3 = 4800.0;
float SineTable[NumTableEntries];
int itr = 0;

void FillSineTable()
{
	Int32 i;

	for(i = 0;i < NumTableEntries;i++)
		SineTable[i] = 1000 * sinf(i * (float)(6.283185307 / NumTableEntries));
}

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
		float sin1;
		float sin2;
		float sin3;
		static float index1 = 0;
		static float index2 = 0;
		static float index3 = 0;
		float out;

		ReadCodecData();

	 	if(CheckForOverrun())					// overrun error occurred (i.e. halted DSP)
			return;								// so serial port is reset to recover

		index1 += f1;	/* calculate the next phase  */
		if (index1 >= GetSampleFreq()) 	/* maintain the phase between 0 and 2*pi  */
			index1 -= GetSampleFreq();

		sin1 = SineTable[(Int32)(index1 / GetSampleFreq() * NumTableEntries)];

		index2 += f2;	/* calculate the next phase  */
		if (index2 >= GetSampleFreq()) 	/* maintain the phase between 0 and 2*pi  */
			index2 -= GetSampleFreq();

		sin2 = SineTable[(Int32)(index2 / GetSampleFreq() * NumTableEntries)];

		index3 += f3;	/* calculate the next phase  */
		if (index3 >= GetSampleFreq()) 	/* maintain the phase between 0 and 2*pi  */
			index3 -= GetSampleFreq();

		sin3 = SineTable[(Int32)(index3 / GetSampleFreq() * NumTableEntries)];

		out = alpha*sin1 + beta*sin2 + gamma*sin3;

		CodecDataOut.Channel[LEFT] = out;
		CodecDataOut.Channel[RIGHT] = CodecDataOut.Channel[LEFT]; /* copy the left channel to the right channel  */

		GUI_out[itr++] = out;
		if (itr > 300)
			itr = 0;

		WriteCodecData(CodecDataOut.UINT);		// send output data to  portort

}
