#include "pooling_layer.h"
#include <hls_video.h>
#include <hls_stream.h>
#include <ap_int.h>
#include <iostream>

#define INPUT_HEIGHT 	36
#define INPUT_WIDTH 	36
#define INPUT_DEPTH		20
#define KERNELSIZE		2
#define NUMOUTPUT		20
#define OUTPUT_HEIGHT 	18  // calculated with formula
#define OUTPUT_WIDTH	18
#define STRIDE			2

typedef ap_fixed<32,6> fixedData;

void poolHW_depth(hls::stream<float>  &input, hls::stream<float> &outputStream){

	// Port Description

	// Linebuffer for Filter Neighbourhood
	hls::LineBuffer<KERNELSIZE*INPUT_HEIGHT,1,fixedData> linBuff;

	// Inital Buffer Filling
	// Buffer filled like 1. Collum 1. depth; 2. Collom 1. depth; 3. Collum 1. depth.....1. Collum 2. depth
	for (int i = 0; i< KERNELSIZE*INPUT_HEIGHT; i++){

		linBuff.shift_pixels_up(0);
		float insertBuff = input.read();
		fixedData insertBufffix = (fixedData)insertBuff;
		linBuff.insert_bottom_row(insertBufffix,0);

	}


	fixedData Buffer [OUTPUT_HEIGHT*OUTPUT_WIDTH*NUMOUTPUT];


	int nBorder1 = 0; // 36 Schritte hier weil es 36 Nachbarschaften in jede RIchtugn gibt
	int nBorder2 = 0; // 36 Schritte hier weil es 36 Nachbarschaften in jede RIchtugn gibt
	int counter1 = 0;
	int counter2 = 0;
	int depthCounter = 0;
	fixedData tempMax = 0;
	int tempcount1 = 0;

	while (depthCounter < INPUT_DEPTH){



			while ( nBorder1<OUTPUT_HEIGHT){

				while (nBorder2<OUTPUT_WIDTH){

						for (counter1=0; counter1<KERNELSIZE;counter1++ ){ // läuft 0 1 2

							for(counter2 = 0; counter2 <KERNELSIZE; counter2++){ //counter 3 ist KernelSize Erhöhrt counter 1+3 -> 0 1 2 /3 4 5 / 6 7 8

								fixedData getValBuff = linBuff.getval(counter1*INPUT_HEIGHT+counter2,0);
								if(getValBuff > tempMax){
									tempMax = getValBuff;
								}
								else{}// do nothing


							}
						}

						outputStream.write(tempMax);
						tempMax = 0;
						for (int i = 0; i<STRIDE; i++){
							linBuff.shift_up(0);

							if (!input.empty()){
								float inTemp = input.read();
								fixedData inTempfix = fixedData(inTemp);
								linBuff.insert_bottom_row(inTempfix,0);
							}
						}

					nBorder2++;

				}

				for ( int i = 0; i<INPUT_HEIGHT-1+(KERNELSIZE-1); i++){
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

		}



}
