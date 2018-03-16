#include "conv_layer.h"
#include <string.h> // memcpy
#include <ap_fixed.h>


#define INPUT_HEIGHT 18
#define INPUT_WIDTH 18
#define INPUT_DEPTH 20
#define KERNELSIZE 3
#define NUMOUTPUT 40
#define OUTPUT_HEIGHT 16
#define OUTPUT_WIDTH 16

typedef ap_fixed<32,8> fixedData;

void convHW_depth(hls::stream<float>  &input, volatile float  *inputWeights, volatile float  *inputBias, hls::stream<float> &outputStream){


	// Port Description
	#pragma HLS INTERFACE m_axi depth=40 port=inputBias offset=slave bundle=WeightsIn
	#pragma HLS INTERFACE m_axi depth=7200 port=inputWeights offset=slave bundle=WeightsIn
	#pragma HLS INTERFACE axis port=outputStream
	#pragma HLS INTERFACE axis port=input
	#pragma HLS INTERFACE s_axilite port=return

	float weightsTemp[NUMOUTPUT*KERNELSIZE*KERNELSIZE*INPUT_DEPTH];
	#pragma HLS RESOURCE variable=weightsTemp core=RAM_2P_BRAM latency=2
	#pragma HLS ARRAY_PARTITION variable=weightsTemp cyclic factor=3 dim=1

	float BiasTemp[NUMOUTPUT];

	memcpy(weightsTemp,(const float*)inputWeights,KERNELSIZE*KERNELSIZE*INPUT_DEPTH*NUMOUTPUT*sizeof(float));
	memcpy(BiasTemp,(const float*)inputBias,NUMOUTPUT*sizeof(float));

	float resultF = 0;

	// Linebuffer for Filter Neighbourhood
	hls::LineBuffer<KERNELSIZE*INPUT_HEIGHT,1,fixedData> linBuff;
	#pragma HLS ARRAY_RESHAPE variable=linBuff cyclic factor=3 dim=1

	// Inital Buffer Filling
	// Buffer filled like 1. Collum 1. depth; 2. Collom 1. depth; 3. Collum 1. depth.....1. Collum 2. depth
	for (int i = 0; i< KERNELSIZE*INPUT_HEIGHT; i++){
		#pragma HLS PIPELINE
		linBuff.shift_pixels_up(0);
		float insertBuff = input.read();
		fixedData insertBufffix = (fixedData)insertBuff;
		linBuff.insert_bottom_row(insertBufffix,0);
	}


	// Inital Buffer Filling
	// Buffer filled like 1. Collum 1. depth; 2. Collom 1. depth; 3. Collum 1. depth.....1. Collum 2. depth
	fixedData Results[NUMOUTPUT][OUTPUT_HEIGHT][OUTPUT_WIDTH];
	#pragma HLS RESOURCE variable=Results core=RAM_2P_BRAM latency=2
	for ( int i = 0; i < NUMOUTPUT;i++){
		for (int a = 0; a<OUTPUT_HEIGHT; a++){
			#pragma HLS PIPELINE
			for (int b = 0; b<OUTPUT_WIDTH;b++){
					Results[i][a][b]=0;
			}
		}
	}

	int nBorder1 = 0;
	int nBorder2 = 0;
	int counter1 = 0;
	int counter2 = 0;
	int counter3 = 0;
	int depthCounter = 0;

	int index=0;
	fixedData tempdata=0;
	int indexstream=0;
	int kernelconst=KERNELSIZE*KERNELSIZE*INPUT_DEPTH;
	int kernelSquare=KERNELSIZE*KERNELSIZE;
	int kernelVol=0;
	while (depthCounter < INPUT_DEPTH){

		kernelVol=kernelSquare*depthCounter;
		while ( nBorder1<OUTPUT_HEIGHT){
			while (nBorder2<OUTPUT_WIDTH){
				for ( int i = 0; i< NUMOUTPUT;i++){
					#pragma HLS PIPELINE rewind

					// Can be used to reduce Resource consumption
					// Leads to not ending C/RTL cosimulation while testing
					//#pragma HLS ALLOCATION instances=mul limit=19 operation
					//#pragma HLS ALLOCATION instances=sub limit=14 operation
					//#pragma HLS ALLOCATION instances=add limit=25 operation
					//#pragma HLS ALLOCATION instances=icmp limit=25 operation
					//#pragma HLS ALLOCATION instances=shl limit=20 operation
					//#pragma HLS ALLOCATION instances=ashr limit=18 operation

					for (counter1=0; counter1<KERNELSIZE;counter1++ ){

						for(counter2 = 0; counter2 <KERNELSIZE; counter2++, counter3+=KERNELSIZE){

							indexstream=counter1*INPUT_HEIGHT+counter2;
							fixedData getValBuff = linBuff.getval(indexstream,0);
							index= counter1+counter3+i*kernelconst+kernelVol;
							fixedData tempWeights = (fixedData)weightsTemp[index];
							tempdata+=getValBuff*tempWeights;
						}
						counter3=0;
						Results[i][nBorder1][nBorder2]+=tempdata;
						tempdata=0;

					}

				}

				linBuff.shift_up(0);
				if (!input.empty()){
					float inTemp = input.read();
					fixedData inTempfix = fixedData(inTemp);
					linBuff.insert_bottom_row(inTempfix,0);
				}

				nBorder2++;

			}
			for ( int i = 0; i<KERNELSIZE-1; i++){
			#pragma HLS PIPELINE
				linBuff.shift_up(0);
				if (!input.empty()){
					float inTemp = input.read();
					fixedData inTempfix = (fixedData) inTemp;
					linBuff.insert_bottom_row(inTempfix,0);

				}
			}
			nBorder2 = 0;
			nBorder1++;
		}
		nBorder2 = 0;
		nBorder1 = 0;
		depthCounter++;


		for (int i = 0; i< 2*INPUT_HEIGHT; i++){
			if (!input.empty()){
				linBuff.shift_pixels_up(0);
				float insertBuff = input.read();
				fixedData insertBufffix = (fixedData)insertBuff;
				linBuff.insert_bottom_row(insertBufffix,0);
			}

		}

	}

	// Outputting Data to Stream
	for ( int i = 0; i < NUMOUTPUT;i++){
		for (int a = 0; a<OUTPUT_HEIGHT; a++){
			#pragma HLS PIPELINE
			for (int b = 0; b<OUTPUT_WIDTH;b++){
				fixedData result = Results[i][a][b] + (fixedData) BiasTemp[i];
				outputStream.write(result);
				resultF=0;

			}
		}
	}

}
