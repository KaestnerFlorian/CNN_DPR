#ifndef __FILTER_H__
#define __FILTER_H__

#include <hls_video.h>
#include <hls_stream.h>
#include <hls_math.h>
#include <ap_int.h>
#include <hls_math.h>
#include <hls_stream.h>
#include "ap_axi_sdata.h"
#include <ap_shift_reg.h>

// debug printing
#include <iostream>
using namespace std;




void convHW(hls::stream<float>  &input, volatile float *inputWeights , volatile float  *inputBias,hls::stream<float> &outputStream );
void convHW_depth(hls::stream<float>  &input, volatile float *inputWeights , volatile float  *inputBias,hls::stream<float> &outputStream );


template <class type>
struct layerParameter {

	int numOutput ;
	int kernelSize ;
	int stride ;
	int padding ;
	int concatDim ;
	int pool ;
	int inputWidth ;
	int inputHeight ;
	int inputDepth ;
	int inputLenght ;
	type *convWeights ;
	type * fcWeights ;
	type *bias ;
};





// DEFINITION
template <class type>
class conv_layer
{
public:
	conv_layer();
	conv_layer(struct layerParameter<type> lp, type *inputData, type *outputData);
	~conv_layer();
	void printLayerParam();
	void forward();

private:
	int numOutputs;
	int kernelSize;
	int stride;
	int padding;
	int inputHeight;
	int inputWidth;
	int inputDepth;
	type *convWeights;
	type *bias;
	type *inputData;
	type *outputData;

};

// IMPLEMENTATION
template <class type>
conv_layer<type>::conv_layer(struct layerParameter<type> lp, type *inputData, type *outputData)
{
	// Parameters to private members
	this->numOutputs = lp.numOutput;
	this->kernelSize = lp.kernelSize;
	this->stride = lp.stride;
	this->padding = lp.padding;
	this->convWeights = lp.convWeights;
	this->bias = lp.bias;
	this->inputData = inputData;
	this->outputData = outputData;
	this->inputWidth = lp.inputWidth;
	this->inputHeight = lp.inputHeight;
	this->inputDepth = lp.inputDepth;

}
template <class type>
conv_layer<type>::conv_layer()
{
	this->numOutputs = 0;
	this->kernelSize = 0;
	this->stride = 0;
	this->padding =0;
	this->convWeights = 0;
	this->bias = 0;
	this->inputData = 0;
	this->outputData = 0;
	this->inputWidth = 0;
	this->inputHeight = 0;
	this->inputDepth = 0;
}
template <class type>
conv_layer<type>::~conv_layer()
{
}


// printing private members
template <class type>
void conv_layer<type>::printLayerParam()
{
	cout << "KernelSize: " << this->kernelSize << endl;
	cout << "NumOutputs: " << this->numOutputs << endl;
	cout << "Padding: " << this->padding << endl;
	cout << "Stride: " << this->stride << endl;
	cout << "Bias Address: 0x" << this->bias << endl;
	cout << "Weights Address: 0x" << this->convWeights << endl;
	cout << "InputData Address: 0x" << this->inputData << endl;
	cout << "OutputData Address: 0x" << this->outputData << endl;
	cout << "Object Address: 0x" << this << endl << endl;

}

// doing forward path (convolution)
//*******************************************************************************
//TODO: pictureSize has to be changed for unsymetric Pictures
//TEMP: Check Picture Size: has to be changed with different Kernel Sizes
//		(line 109/111)
//
// DONE: Adding Bias Addition
//		 Check passing pictureDepth and Picture Size via parameter(line 100 - 102)
//*******************************************************************************

template <class type>
void conv_layer<type>::forward()
{

	// Parameters read from Layer parameter struct
	int stride = this->stride;
	int kernelSize = this->kernelSize;
	int filterNum = this->numOutputs;
	int pictureSize = this->inputHeight; // has to be changed for unsymetric input!!!!!!!!!!
	int pictureDepth = this->inputDepth;
	//int filterDepth = pictureDepth; // mandatory for CNN convolution layers

	// Temporary calculation of pictureSize subtraction value -> needs to be verified
	int correctionFactorX = pictureSize - ((pictureSize - kernelSize + (2 * this->padding)) / ((stride)) + 1);
	int correctionFactorY = pictureSize - ((pictureSize - kernelSize + (2 * this->padding)) / ((stride)) + 1);

	float oneDepth = 0; // cummulative value of one 2D Convolution
	float allDepth = 0; // cummulative value of all 2D Convolutions in Depth

	for (int filterNr = 0; filterNr < filterNum; filterNr++) {

		for (int picturey = 0; picturey < pictureSize - correctionFactorY; picturey += stride) { // pictureSize -1 or -2 depends on KernelSize -3 for real Conv

			for (int picturex = 0; picturex < pictureSize - correctionFactorX; picturex += stride) {  // pictureSize -1 or -2 depends on KernelSize  -3 for real Conv

				for (int depth = 0; depth < pictureDepth; depth++) {

					for (int filterx = 0; filterx < kernelSize; filterx++) { // first moving filter down ( y-Direction ) [][Y]

						 for (int filtery = 0; filtery < kernelSize; filtery++) {

							int xn = picturex + filtery; // coordinates picture where the filter "lays" on
							int yn = picturey + filterx;
							oneDepth = oneDepth + (convWeights[filtery*kernelSize + filterx + kernelSize*kernelSize*depth + kernelSize*kernelSize*pictureDepth*filterNr] * inputData[xn + yn*pictureSize + pictureSize*pictureSize*depth]);
						}
					}
					//after 2D conv loop in Depth Loop
					allDepth = allDepth + oneDepth;
					oneDepth = 0;
				}
				outputData[picturex + picturey*(pictureSize - correctionFactorY) + (pictureSize-correctionFactorY)*(pictureSize-correctionFactorX)*filterNr] = allDepth + bias[filterNr];
				allDepth = 0;
			}
		}
	}
}


#endif

