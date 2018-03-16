#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "caffe_parameter_parser.hpp"
#include "conv_layer.h"
#include "calc_shape.hpp"
#include "opencv2/opencv.hpp"

#include "conv_layer.h"
#include "pooling_layer.h"
#include "ReLU_layer.h"

#include <hls_stream.h>
#include "ap_axi_sdata.h"

#include <ctime>

using namespace std;
using namespace cv;

int main()
{

	/* Getting Picture Block
	 * Picture is read to be stored in a simple c array
	 ***************************************************/

	Mat image = imread("resources/01.jpg", IMREAD_GRAYSCALE);
	Mat resizedImg, converted;
	Scalar mean, std;

	resize(image, resizedImg, cvSize(39, 39));
	resizedImg.convertTo(converted, CV_32F);

	// calculation standard derivation and mean pixel value to create same conditions with TB
	meanStdDev(converted, mean, std);

	// Moving Picture from Mat format to simple array
	float *inputPicture = new float [converted.cols*converted.rows];
	for (int i = 0; i < converted.cols; i++) {
		for (int j = 0; j < converted.rows; j++) {
			inputPicture[i+converted.cols*j] = (converted.at<float>(i, j) - mean[0]) / std[0];

		}
	};


/******************************************************************************************************/

	/* Network Configuration Block
	 * Here all Weights are loaded
	 *******************************/


	// getting Weights and Bias from Caffe model ( only convolution layer 1)
	float *conv1bias = parseBias2array<float>("resources/weights/1_F.caffemodel_conv1_bias.txt");
	float *conv1Weights1D = parseConvWeights2_1D_array<float>("resources/weights/1_F.caffemodel_conv1_weights.txt");

	float *conv2bias = parseBias2array<float>("resources/weights/1_F.caffemodel_conv2_bias.txt");
	float *conv2Weights = parseConvWeights2_1D_array<float>("resources/weights/1_F.caffemodel_conv2_weights.txt");


	/* Building Software Network
	 * used for later verification
	 *******************************/

	// Layer Parameter for Conv1 Layer
	layerParameter<float> lp_conv1;
	lp_conv1.stride = 1;
	lp_conv1.kernelSize = 4;
	lp_conv1.numOutput = 20;
	lp_conv1.convWeights = conv1Weights1D;
	lp_conv1.bias = conv1bias;
	lp_conv1.inputHeight = 39;
	lp_conv1.inputWidth = 39;
	lp_conv1.inputDepth = 1;
	lp_conv1.padding = 0;
	outputShape conv_out;
	conv_out = conv_output_shape(lp_conv1.inputHeight, lp_conv1.inputWidth, lp_conv1.inputDepth, lp_conv1.stride, 0, lp_conv1.numOutput, lp_conv1.kernelSize);
	float *output_Conv1 = 0;
	output_Conv1 = new float [conv_out.outputWidth*conv_out.outputHeight*conv_out.outputDepth];
	conv_layer<float> conv1(lp_conv1, inputPicture, output_Conv1);


	// Relu1
	layerParameter<float> lp_real_ReLu;
	lp_real_ReLu.inputWidth =36;
	lp_real_ReLu.inputHeight = 36;
	lp_real_ReLu.inputDepth = 20;
	float *output_real_ReLu = 0;
	output_real_ReLu = build1DArray<float>(lp_real_ReLu.inputWidth*lp_real_ReLu.inputHeight*lp_real_ReLu.inputDepth);
	relu_layer<float> ReLuLayer_real(lp_real_ReLu, output_Conv1, output_real_ReLu);


	// Pool1
	layerParameter<float> lp_real_pool1;
	lp_real_pool1.stride = 2;
	lp_real_pool1.kernelSize = 2;
	lp_real_pool1.inputHeight = 36;
	lp_real_pool1.inputWidth = 36;
	lp_real_pool1.inputDepth = 20;
	outputShape pool1_real_out;
	pool1_real_out = pool_output_shape(lp_real_pool1.inputWidth, lp_real_pool1.inputHeight, lp_real_pool1.inputDepth, lp_real_pool1.stride, lp_real_pool1.kernelSize);
	float *output_real_pool1 = 0;
	//cout << pool1_real_out.outputWidth << pool1_real_out.outputDepth << endl;
	output_real_pool1 = build1DArray<float>(pool1_real_out.outputWidth*pool1_real_out.outputHeight*pool1_real_out.outputDepth);
	pooling_layer<float> Pool1Layer_real(lp_real_pool1, output_real_ReLu, output_real_pool1);


	// Conv2
	layerParameter<float> lp_real_conv2;
	lp_real_conv2.stride = 1;
	lp_real_conv2.kernelSize = 3;
	lp_real_conv2.numOutput = 40;
	lp_real_conv2.convWeights = conv2Weights;
	lp_real_conv2.bias = conv2bias;
	lp_real_conv2.inputHeight = pool1_real_out.outputHeight;
	lp_real_conv2.inputWidth = pool1_real_out.outputWidth;
	lp_real_conv2.inputDepth = pool1_real_out.outputDepth;
	lp_real_conv2.padding = 0;
	outputShape conv2_real_out;
	conv2_real_out = conv_output_shape(lp_real_conv2.inputHeight, lp_real_conv2.inputWidth, lp_real_conv2.inputDepth, lp_real_conv2.stride, 0, lp_real_conv2.numOutput, lp_real_conv2.kernelSize);
	float *output_real_Conv2 = 0;
	output_real_Conv2 = build1DArray<float>(conv2_real_out.outputWidth*conv2_real_out.outputHeight*conv2_real_out.outputDepth*2);
	conv_layer<float> convLayer2_real(lp_real_conv2, output_real_pool1, output_real_Conv2);


    /********************************************************************************************************************/

	// Forwarding Layer
	// To generate Results for verification
	conv1.forward();
	ReLuLayer_real.forward();
	Pool1Layer_real.forward();
	convLayer2_real.forward();

	// Building Streams as Input for HW
	hls::stream<float> inputConv2 ;
	hls::stream<float> OutputConv2;

	// Filling Input Stream
	for (int i = 0; i< 18*18*20;i++){
		inputConv2.write(output_real_pool1[i]);

	};

	// Execute hardware function
	convHW_depth(inputConv2,conv2Weights,conv2bias,OutputConv2);


	// Verification Section
	// Comparing HW and SW results
	cout << "Results Conv2"<< endl;
    int errorCount=0;
	for  (int i = 0; i<16*16*40; i++){
		float tempOut = OutputConv2.read();
		float diff = abs(output_real_Conv2[i])-abs(tempOut);

		cout <<"Nr: "<< i << " " << output_real_Conv2[i] << " " << tempOut << " Diff: "<< abs(diff) <<endl;
		if (abs(diff) >= 0.00001 ){
			errorCount++;
		}

	}

	if (errorCount !=0 ){
		cout << "****ERROR: Difference between HW and SW implementation" << endl;
		return -1;
	}
	else{
		cout <<"Test Passed!" << endl;
		return 0;
	}



}


