#ifndef CALC_SHAPE_H
#define CALC_SHAPE_H

struct outputShape {
	int outputWidth;
	int outputHeight;
	int outputDepth;

};

// output Volume of convolution Layer
outputShape conv_output_shape(float inputWidth, float inputHeight, float inputDepth, float stride, float padding, float filterNum, float filterSize);

// output Volume of pooling layer
outputShape pool_output_shape(float inputWidth, float inputHeight, float inputDepth, float stride, float filterSize);


#endif
