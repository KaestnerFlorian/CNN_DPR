#include "calc_shape.h"
#include <exception>
#include <iostream>


outputShape conv_output_shape(float inputWidth, float inputHeight, float inputDepth, float stride, float padding, float filterNum, float filterSize) {

	outputShape convOut;
	convOut.outputDepth = filterNum;
	float outputWidth = (inputWidth - filterSize + (2 * padding)) / ((stride))+1;
	float outputHeight = (inputHeight - filterSize + (2 * padding)) / (stride)+1;
	if (outputHeight == (int)outputHeight) {
		convOut.outputHeight = outputHeight;
	}
	else {
		std::cerr << "***Error: conv_output_shape-> Output Volumen not integer, Check Hyper Parameter" << std::endl;
		//exit(1); // error 
	}

	if (outputWidth == (int)outputWidth) {
		convOut.outputWidth = outputWidth;
	}

	else {
		std::cerr << "***Error: conv_output_shape-> Output Volumen not integer, Check Hyper Parameter" << std::endl;
		//exit(1);
	}

	return convOut;
}

outputShape pool_output_shape(float inputWidth, float inputHeight, float inputDepth, float stride, float filterSize) {

	outputShape poolOut;
	poolOut.outputDepth = inputDepth;
	float outputWidth = (inputWidth - filterSize )/ (stride) + 1;
	float outputHeight = (inputHeight - filterSize) / (stride)+1;


	if (outputHeight == (int)outputHeight) {
		poolOut.outputHeight = outputHeight;
	}
	else {
		std::cerr << "***Error: pool_output_shape-> Output Volumen not integer, Check Hyper Parameter" << std::endl;
		//exit(1); // error 
	}

	if (outputWidth == (int)outputWidth) {
		poolOut.outputWidth = outputWidth;
	}

	else {
		std::cerr << "***Error: pool_output_shape-> Output Volumen not integer, Check Hyper Parameter" << std::endl;
		//exit(1);
	}

	return poolOut;

}