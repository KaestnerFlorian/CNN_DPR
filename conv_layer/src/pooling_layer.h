#ifndef POOLING_LAYER_H
#define POOLING_LAYER_H
#include "conv_layer.h"


void poolHW_depth(hls::stream<float>  &input, hls::stream<float> &outputStream);


template <class type>
class pooling_layer

{
public:
	pooling_layer();
	pooling_layer(struct layerParameter<type> lp, type *inputData, type *outputData);
	void forward();
	~pooling_layer();


private:
	int kernelSize;
	int stride;
	int inputHeight;
	int inputWidth;
	int inputDepth;
	type *inputData;
	type *outputData;
	int pool ;


};


// IMPLEMENTATION

template <class type>
pooling_layer<type>::pooling_layer()
{

}

template <class type>
pooling_layer<type>::pooling_layer(struct layerParameter<type> lp, type *inputData, type *outputData)
{
	this->pool = lp.pool;
	this->stride = lp.stride;
	this->kernelSize = lp.kernelSize;
	this->inputData = inputData;
	this->outputData = outputData;
	this->inputWidth = lp.inputWidth;
	this->inputHeight = lp.inputHeight;
	this->inputDepth = lp.inputDepth;
}

// doing forward path (pooling)
//*******************************************************************************
// TODO: PictureSize has to be changed for unsymetric input!!!!!!!!!!
// DONE:	Check passing pictureDepth and Picture Size via parameter
//
//*******************************************************************************
template <class type>
void pooling_layer<type>::forward()
{

	// Parameters read from Layer parameter struct
	int stride = this->stride;
	int pictureDepth = this->inputDepth;
	int pictureSize = this->inputHeight;   // has to be changed for unsymetric input!!!!!!!!!!
	type tempMax = 0;
	int kernelSize = this->kernelSize;

	int outputX = 0; // necessary for finding right output position if stride is not equal to 1 ( maybe necessary in Conv)

	for (int depth = 0; depth < pictureDepth; depth++) {

		for (int picturey = 0; picturey < pictureSize - 1; picturey += stride) { // pictureSize -1 or -2 depends on KernelSize

			for (int picturex = 0; picturex < pictureSize - 1; picturex += stride) {  // pictureSize -1 or -2 depends on KernelSize

				for (int filtery = 0; filtery < kernelSize; filtery++) { // first moving filter down ( y-Direction ) [][Y]

					for (int filterx = 0; filterx < kernelSize; filterx++) {

						int xn = picturex + filterx; // coordinates picture where the filter "lays" on
						int yn = picturey + filtery;

						if (inputData[xn + yn*pictureSize + pictureSize*pictureSize*depth] > tempMax){
							tempMax = inputData[xn + yn*pictureSize + pictureSize*pictureSize*depth];
						}
					}
				}
					outputData[outputX] = tempMax;
					tempMax = 0;
					outputX++;
			}

		}

	}

}





template <class type>
pooling_layer<type>::~pooling_layer()
{
}



#endif
