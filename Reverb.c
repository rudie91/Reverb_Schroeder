
/*
 * Reverb.c
 *
 *  Created on: Mar 14, 2018
 *      Author: Yoon mo Yang
 * This C script generates generates reverbration based on Schroeder's 
 * Reverbration papers: Schroeder_1961 and Schroeder_1962
 * 
 * 
 */
#include "L138_LCDK_aic3106_init.h"
#include "math.h"
#include <stdio.h>
#define FS 44100
#define LENGTH 2048

int16_t Linput,Rinput;
AIC31_data_type codec_data;

// buffer for delay
volatile float buffer[6][LENGTH];


static int readptr[6];
static int writeptr;

volatile float tau1;
volatile float gain1;

float T;
float taus[6];
float gains[7];

//combfilter output 
float y1_comb;
float y2_comb;
float y3_comb;
float y4_comb;
float t_comb; // total:summed output

//allpass filter output
float y1_allpass;
float y2_allpass;
float w_allpass1;
float w_allpass2;

float finaloutput;

//testing buffer
//float buffer_t[LENGTH];
//float test = 0.0;

void Combfiltering_DelayL(float tau, float gain, int num, float x, float *y);

interrupt void interrupt4(void) // interrupt service routine
{

// input the actual data
    codec_data.uint = input_sample();
// store input sample to a variable
    Linput = codec_data.channel[LEFT];
    Rinput = codec_data.channel[RIGHT];

// Downmix here
    input = 0.5*(Linput + Rinput);

// parameters: these are based on a room which is 30*50*10 meters big with 85% sound reflection
    T = 3.5435; // Time for 60dB decay
    taus[0] = tau1;
    taus[1] = 1.25 * taus[0];
    taus[2] = 1.35 * taus[0];
    taus[3] = 1.5 * taus[0];
    taus[4] = 0.005;
    taus[5] = 0.017;

    gains[0] = gain1;
    gains[1] = powf(10,(-3*taus[1]/T));
    gains[2] = powf(10,(-3*taus[2]/T));
    gains[3] = powf(10,(-3*taus[3]/T));
    gains[4] = 0.7;
    gains[5] = 0.7;
    gains[6] = 0.5;

// comb filtering (parallel part)
    Combfiltering_DelayL(taus[0],gains[0],0,input,&y1_comb);
    Combfiltering_DelayL(taus[1],gains[1],1,input,&y2_comb);
    Combfiltering_DelayL(taus[2],gains[2],2,input,&y3_comb);
    Combfiltering_DelayL(taus[3],gains[3],3,input,&y4_comb);
// sum them up!
    t_comb = y1_comb+y2_comb+y3_comb+y4_comb;

    Combfiltering_DelayL(taus[4],gains[4],4,t_comb,&y1_allpass);
    w_allpass1 = -t_comb*(gains[4]) + y1_allpass*(1-(gains[4]*gains[4]));

    Combfiltering_DelayL(taus[5],gains[5],5,w_allpass1,&y2_allpass);
    w_allpass2 = -w_allpass1*(gains[5]) + y2_allpass*(1-gains[4]*gains[4]);

    finaloutput = w_allpass2*gains[6]+input;
    
    // output
    codec_data.channel[LEFT] =  finaloutput;
    codec_data.channel[RIGHT] = finaloutput;

//final output stage
  output_sample(codec_data.uint);
  return;
}

int main(void)
{
    int i;
    memset(readptr,0,6*sizeof(int));
    //initialize buffer for delay
    for (i = 0;i<6;i++)
    {
    	memset(buffer[i],0,LENGTH*sizeof(float));
    }

  	//interrupt function triggers runs each time an audio sample is received
	L138_initialise_intr(FS_44100_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  	while(1);
}

void Combfiltering_DelayL(float tau, float gain, int num, float x, float *y)
{
	int M = round(FS*tau);
	*y = buffer[num][readptr[num]]; //delayed output
	writeptr = (readptr[num]+M)%(M+1); //update write ptr
	readptr[num] = (readptr[num]+1)%(M+1); //update readptr
	buffer[num][writeptr] = gain*(*y)+x; //difference equation

} 
