#ifndef CONV_LAYER_H
#define CONV_LAYER_H

#include "base_layer.h"
#include "main.h"

// debug printing
#include <iostream>
using namespace std;



// DEFINITION
template <class type>
class conv_layer :
	public base_layer<type>
{
public:
	conv_layer();
	conv_layer(struct layerParameter<type> lp, type ***inputData, type ***outputData);
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
	type ****convWeights;
	type *bias;
	type ***inputData;
	type ***outputData;
	string layerName;
};



// IMPLEMENTATION
template <class type>
conv_layer<type>::conv_layer(struct layerParameter<type> lp, type ***inputData, type ***outputData)
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
	this->layerName = lp.layerName;
	this->inputWidth = lp.inputWidth;
	this->inputHeight = lp.inputHeight;
	this->inputDepth = lp.inputDepth;

}
template <class type>
conv_layer<type>::conv_layer()
{

}
template <class type>
conv_layer<type>::~conv_layer()
{
}


// printing private members
template <class type>
void conv_layer<type>::printLayerParam()
{
	cout << "Layer Name: " << this->layerName << endl;
	cout << "KernelSize: " << this->kernelSize << endl;
	cout << "NumOutputs: " << this->numOutputs << endl;
	cout << "Padding: " << this->padding << endl;
	cout << "Stride: " << this->stride << endl;
	cout << "Bias Adress: 0x" << this->bias << endl;
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
	//int filterDepth = pictureDepth; // mandatory for CNN convultion layers
	
	// Temporary calculation of pictureSize subtraction value -> needs to be veriefied
	int correctionFactorX = pictureSize - ( (pictureSize - kernelSize + (2 * this->padding)) / ((stride)) + 1);
	int correctionFactorY = pictureSize - ((pictureSize - kernelSize + (2 * this->padding)) / ((stride)) + 1);
	//cout << "Korrektur Faktor: " << correctionFactorX << endl;

	float oneDepth = 0; // cummulative value of one 2D Convolution
	float allDepth = 0; // cummulative value of all 2D Convolutions in Depth

	for (int filterNr = 0; filterNr < filterNum; filterNr++) {

		for (int picturey = 0; picturey < pictureSize - correctionFactorY; picturey += stride) { // pictureSize -1 or -2 depends on KernelSize -3 for real Conv

			for (int picturex = 0; picturex < pictureSize - correctionFactorX; picturex += stride) {  // pictureSize -1 or -2 depends on KernelSize  -3 for real Conv

				for (int depth = 0; depth < pictureDepth; depth++) {

					//cout << "Filter Depth: " << depth << endl;

					for (int filterx = 0; filterx < kernelSize; filterx++) { // first moving filter down ( y-Direction ) [][Y] 

						 for (int filtery = 0; filtery < kernelSize; filtery++) {

							int xn = picturex + filtery; // coordinates picture where the filter "lays" on 
							int yn = picturey + filterx;

							oneDepth = oneDepth + (convWeights[filterx][filtery][depth][filterNr] * inputData[xn][yn][depth]);

						}

					}//after 2D conv loop in Depth Loop
					allDepth = allDepth + oneDepth;
					oneDepth = 0;
				}

				outputData[picturex][picturey][filterNr] = allDepth+bias[filterNr]; // maybe wrong with bigger Stride
				allDepth = 0;
			}
		}
	}
}

#endif
