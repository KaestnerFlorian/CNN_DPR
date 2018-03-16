#include "conv_layer.h"
#include <string.h> // memcpy
#include <ap_fixed.h>


#define INPUT_HEIGHT 	39
#define INPUT_WIDTH 	39
#define INPUT_DEPTH		1
#define KERNELSIZE		4
#define NUMOUTPUT		20
#define OUTPUT_HEIGHT 	36  // calculated with formula
#define OUTPUT_WIDTH	36


typedef ap_fixed<32,8> fixedData;

void convHW(hls::stream<float>  &input, volatile float  *inputWeights, volatile float  *inputBias, hls::stream<float> &outputStream){
#pragma HLS DEPENDENCE intra true
#pragma HLS DEPENDENCE inter true

	// Port Description
	#pragma HLS INTERFACE m_axi depth=20 port=inputBias offset=slave bundle=WeightsIn
	#pragma HLS INTERFACE m_axi depth=320 port=inputWeights offset=slave bundle=WeightsIn
	#pragma HLS INTERFACE axis port=outputStream
	#pragma HLS INTERFACE axis port=input
	#pragma HLS INTERFACE s_axilite port=return

	float weightsTemp[NUMOUTPUT*KERNELSIZE*KERNELSIZE]; //320
	float BiasTemp[NUMOUTPUT]; //20

	memcpy(weightsTemp,(const float*)inputWeights,320*sizeof(float));
	memcpy(BiasTemp,(const float*)inputBias,20*sizeof(float));


	float resultF = 0;

	// Linebuffer for Filter Neighbourhood
	hls::LineBuffer<KERNELSIZE*INPUT_HEIGHT,1,fixedData> linBuff;
	#pragma HLS DEPENDENCE variable=linBuff intra true
	#pragma HLS DEPENDENCE variable=linBuff inter true

	// Inital Buffer Filling
	// Buffer filled like 1. Collum 1. depth; 2. Collom 1. depth; 3. Collum 1. depth.....1. Collum 2. depth
	for (int i = 0; i< KERNELSIZE*INPUT_HEIGHT; i++){
	#pragma HLS DEPENDENCE intra true
	#pragma HLS DEPENDENCE inter true

		linBuff.shift_pixels_up(0);
		float insertBuff = input.read();
		fixedData insertBufffix = (fixedData)insertBuff;
		linBuff.insert_bottom_row(insertBufffix,0);

	}




	fixedData Results[NUMOUTPUT][OUTPUT_HEIGHT][OUTPUT_WIDTH]; // 20*1292
	for ( int i = 0; i < NUMOUTPUT;i++){
	#pragma HLS PIPELINE II=1
		for (int a = 0; a<OUTPUT_HEIGHT; a++){
		#pragma HLS PIPELINE II=1
			for (int b = 0; b<OUTPUT_WIDTH;b++){
			#pragma HLS PIPELINE II=1
				Results[i][a][b]=(fixedData)0;
			}
		}
	}


	int nBorder1 = 0; // 36 Schritte hier weil es 36 Nachbarschaften in jede RIchtugn gibt
	int nBorder2 = 0; // 36 Schritte hier weil es 36 Nachbarschaften in jede RIchtugn gibt
	int counter3 = 0;
	int counter1 = 0;
	int counter2 = 0;

	while ( nBorder1<OUTPUT_HEIGHT){
	#pragma HLS DEPENDENCE inter true
	#pragma HLS DEPENDENCE intra true


		while (nBorder2<OUTPUT_WIDTH){
		#pragma HLS DEPENDENCE intra true
		#pragma HLS DEPENDENCE inter true

			for ( int i = 0; i< NUMOUTPUT;i++){
			#pragma HLS DEPENDENCE intra true
			#pragma HLS DEPENDENCE inter true

				for (counter1=0; counter1<KERNELSIZE;counter1++ ){
				#pragma HLS DEPENDENCE intra true
				#pragma HLS DEPENDENCE inter true

					for(counter2 = 0; counter2 <KERNELSIZE; counter2++, counter3+=KERNELSIZE){
					#pragma HLS DEPENDENCE intra true
					#pragma HLS DEPENDENCE inter true

						fixedData getValBuff = linBuff.getval(counter1*INPUT_HEIGHT+counter2,0);
						fixedData tempWeights = (fixedData)weightsTemp[counter1+counter3+KERNELSIZE*KERNELSIZE*i];
						Results[i][nBorder1][nBorder2]+=getValBuff*tempWeights; // 16 kommt von 4x4 KernelSize und 39 von InputPicture Size

					}
					counter3=0;
				}
			}
			#pragma HLS DEPENDENCE intra true
			#pragma HLS DEPENDENCE inter true
			linBuff.shift_up(0);


			if (!input.empty()){
			#pragma HLS DEPENDENCE intra true
			#pragma HLS DEPENDENCE inter true
				float inTemp = input.read();
				fixedData inTempfix = fixedData(inTemp);
				linBuff.insert_bottom_row(inTempfix,0);
			}

			nBorder2++;

		}
		for ( int i = 0; i<KERNELSIZE-1; i++){
		#pragma HLS DEPENDENCE intra true
		#pragma HLS DEPENDENCE inter true
			linBuff.shift_up(0);

			if (!input.empty()){
			#pragma HLS DEPENDENCE intra true
			#pragma HLS DEPENDENCE inter true
				float inTemp = input.read();
				fixedData inTempfix = (fixedData) inTemp;
				linBuff.insert_bottom_row(inTempfix,0);

			}
		}
		#pragma HLS DEPENDENCE intra true
		#pragma HLS DEPENDENCE inter true
		nBorder2 = 0;
		nBorder1++;
	}



	// Outputting Data to Stream
	for ( int i = 0; i < NUMOUTPUT;i++){
	//#pragma HLS PIPELINE
	#pragma HLS DEPENDENCE intra true
	#pragma HLS DEPENDENCE inter true

		for (int a = 0; a<OUTPUT_HEIGHT; a++){
		//#pragma HLS PIPELINE
		#pragma HLS DEPENDENCE intra true
		#pragma HLS DEPENDENCE inter true

			for (int b = 0; b<OUTPUT_WIDTH;b++){
			#pragma HLS PIPELINE
			#pragma HLS DEPENDENCE intra true
			#pragma HLS DEPENDENCE inter true
				fixedData temp = Results[i][a][b];
				fixedData tempBias = (fixedData) BiasTemp[i];
				fixedData result = temp+tempBias;
				resultF= result.to_float();
				outputStream.write(resultF);
				resultF=0;

			}
		}
	}

}














