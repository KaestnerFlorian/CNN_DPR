#ifndef MAIN_H
#define MAIN_H
#include <string>




template <class type>
struct layerParameter {

	std::string layerName = "";
	int numOutput = 0;
	int kernelSize = 0;
	int stride = 0;
	int padding = 0;
	int concatDim = 0;
	int pool = 0;
	int inputWidth = 0;
	int inputHeight = 0;
	int inputDepth = 0;
	int inputLenght = 0;
	type ****convWeights = 0;
	type ** fcWeights = 0;
	type *bias = 0;
};

// DMA Register Address Definition

/*
#define MM2S_CONTROL_REGISTER 0
#define MM2S_STATUS_REGISTER 1
#define MM2S_START_ADDRESS_LSB 6
#define MM2S_START_ADDRESS_MSB 7
#define MM2S_LENGTH 10
*/





#endif
