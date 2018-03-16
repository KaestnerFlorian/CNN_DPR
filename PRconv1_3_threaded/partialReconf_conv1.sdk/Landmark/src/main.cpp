#include "main.h" // struct definitions
#include "dynamic_arrays.h"
#include "caffe_parameter_parser.hpp"
#include "virtual_mem_calc.h"
#include "dma_driver.h"
//#include "ipDriver.h"


// CNN Layer classes
#include "calc_shape.h"
#include "base_layer.h"
#include "conv_layer.h"
#include "pooling_layer.h"
#include "ReLu_layer.h"
#include "fc_layer.h"
#include "flatten_layer.h"
#include "concat_layer.h"

// OpenCV for image Processing
#include "opencv2/core/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

// Standard Includes
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include <pthread.h>


using namespace cv;

// definition of actual used datatype for weights
typedef float usedDataType;


// Interrupt Singal for while loop
volatile sig_atomic_t flag = 1;
void sig_irq(int sig){
  flag = 0;
}

// function for partial reconfiguration Conv1
void* PartialReconfigureConv1 (void *vpmmio){

	unsigned int * pmmio = (unsigned int *) vpmmio;
	//cout<< "Partial Reconfiguration Thread "<< endl;
	system("echo 1 > /sys/devices/soc0/amba/f8007000.devcfg/is_partial_bitstream");
	system("cat pynq/bitstream/conv1ip_pb_convHW_partial.bit > /dev/xdevcfg");
	*pmmio = 0b0;
	*pmmio= 0b1;
	//cout << "Partial Reconfigured to Conv1" << endl;
	pthread_exit(NULL);

}

// function for partial reconfiguration Conv3
void* PartialReconfigureConv3 (void *vpmmio){

	unsigned int * pmmio = (unsigned int *) vpmmio;
	//cout<< "Partial Reconfiguration Thread "<< endl;
	system("echo 1 > /sys/devices/soc0/amba/f8007000.devcfg/is_partial_bitstream");
	system("cat pynq/bitstream/conv3ip_pb_convHW_partial.bit > /dev/xdevcfg");
	*pmmio = 0b0;
	*pmmio = 0b1;
	//cout << "Partial Reconfigured to Conv3" << endl;
	pthread_exit(NULL);

}



int main() {

	// Registering Signals for interrupting infinity while loop
	signal(SIGINT, sig_irq);

	// Memory Sizes for HDMI
	uint32_t InHDMISize = 0x5EEC00;
	uint32_t OutHDMISize = 0x5EEC00;

	// Physical Addresses of HDMI-I/O and Input Data
	uint32_t InHDMIAddress = 0x17b00000; // determined by python startup script
	uint32_t OutHDMIAddress = 0x16900000; // determined by python startup script

	// Virtual Address HDMI IN
	virtualAddress HDMIIn;
	HDMIIn = virtual_mem_calc(InHDMIAddress, InHDMISize);
	//cout<< "Virtual Address HDMI IN: " << HDMIIn.virt_addr << endl;
	void * pHDMIIn = (void*) HDMIIn.virt_addr;

	// Virtual Address HDMI OUT
	virtualAddress HDMIOut;
	HDMIOut = virtual_mem_calc(OutHDMIAddress, OutHDMISize);
	//cout<< "Virtual Address HDMI OUT: " << HDMIOut.virt_addr << endl;
	void * pHDMIOut = (void*) HDMIOut.virt_addr;

	// Virtual Address IP Core
	virtualAddress IPcore;
	IPcore = virtual_mem_calc(0x43C30000,0xffff);
	//cout<< "Virtual Address ConvHW: " << IPcore.virt_addr << endl;
	unsigned int* pConvHW = (unsigned int*) IPcore.virt_addr;

	// Virtual Address DMA0 (Picture)
	virtualAddress Dma0;
	Dma0 = virtual_mem_calc(0x40400000, 0xffff);
	//cout<< "Virtual Address DMA0: " << Dma0.virt_addr << endl;
	unsigned int* dma0_reg = (unsigned int*) Dma0.virt_addr;

	//Virtual Address MMIO
	virtualAddress mmio;
	mmio= virtual_mem_calc(0x41220000, 0x50); // length in Byte
	unsigned int* pmmio = (unsigned int*) mmio.virt_addr;
	//cout<< "Virtual Address MMIO: " << pmmio << endl;


	// Thread for conv1 Partial Reconf
	pthread_t partialConv1;
	int iret = 0;

	// Thread for conv3 Partial Reconf
	pthread_t partialConv3;
	int iret3 = 0;


	// Parsing Caffe Weights
	usedDataType ****conv2Weights = parseConvWeights2array<usedDataType>("resources/weights/1_F.caffemodel_conv2_weights.txt");
	usedDataType *conv2bias = parseBias2array<usedDataType>("resources/weights/1_F.caffemodel_conv2_bias.txt");
	usedDataType ****conv4Weights = parseConvWeights2array<usedDataType>("resources/weights/1_F.caffemodel_conv4_weights.txt");
	usedDataType *conv4bias = parseBias2array<usedDataType>("resources/weights/1_F.caffemodel_conv4_bias.txt");
	usedDataType **fc1Weights = parseFcWeights2array<usedDataType>("resources/weights/1_F.caffemodel_fc1_weights.txt");
	usedDataType *fc1Bias = parseBias2array<usedDataType>("resources/weights/1_F.caffemodel_fc1_bias.txt");
	usedDataType **fc2Weights = parseFcWeights2array<usedDataType>("resources/weights/1_F.caffemodel_fc2_weights.txt");
	usedDataType *fc2Bias = parseBias2array<usedDataType>("resources/weights/1_F.caffemodel_fc2_bias.txt");


	// Instatiations for Input Image
	int width = 1080;
	int height = 1920;
	int channels = 3;
	Mat Img_in( width, height, CV_8UC3);
	Mat Img_in_grey( width, height, CV_8UC3);
	Size network_fitting_size(420,300);
	Mat Img_primed;

	// Instatiations for post processing
	struct BBox {
		int left;
		int right;
		int top;
		int bottom;
		int x;
		int y;
		int w;
		int h;
	};

	struct BBoxDelta {
		int leftDelta;
		int rightDelta;
		int topDelta;
		int bottomDelta;
		int left;
		int right;
		int top;
		int bottom;
	};


	// Instaiations for face detection
	CascadeClassifier face_cascade("resources/openCV_pre_post_processing/haarcascade_frontalface_alt.xml");
	std::vector<Rect> faces;


	// Image Preprocessing
	Mat image, cropped;
	Mat resizedImg;
	Mat resultFrame;

	// Face detection
	BBox bbox;
	BBoxDelta subbox;
	BBox bboxFromSubbox;
	cv::Rect roi;

	// Image Preprocessing ( subtracton of Mean and Derivation)
	Mat converted;
	Scalar mean;
	Scalar std;

/*************CONV1**************************************/
	// Sizes for conv1 in HW
	uint32_t inputSize = 0x17C4;		// depending on network layer size
	uint32_t outputSize = 0x19500;		// depending on network layer size
	uint32_t weightSize = 0x500;		// depending on network layer size
	uint32_t biasSize = 0x50;			// depending on network layer size

	// Addresses for conv1 in HW
	uint32_t InputAddress = 0x16846000; 	// determined by python startup script
	uint32_t OutputAddress = 0x16860000; 	// determined by python startup script
	uint32_t WeightsAddress = 0x16848000;	// determined by python startup script
	uint32_t BiasAddress = 0x16849000; 		// determined by python startup script


	// Output for conv1 in HW
	usedDataType ***pVirtout3D = 0;
	pVirtout3D = build3DArray<usedDataType>(36, 36, 20);


	// Virtual Address Input Data
	virtualAddress Input;
	Input = virtual_mem_calc(InputAddress, inputSize); // length in Byte
	float* pvirtInput = (float*) Input.virt_addr;
	//cout<< "Virtual Address Input: " << pvirtInput << endl;

	//Virtual Address Output Data
	virtualAddress Output;
	Output= virtual_mem_calc(OutputAddress, outputSize); // length in Byte
	float* pvirtOutput = (float*) Output.virt_addr;
	//cout<< "Virtual Address Output: " << pvirtOutput << endl;
	for (unsigned int i = 0; i< (outputSize/sizeof(float)) ;i++){
		pvirtOutput[i] = 0;
	};


	//Virtual Address Weights
	virtualAddress Weights;
	Weights = virtual_mem_calc(WeightsAddress, weightSize);
	float* pvirt_weight = (float*) Weights.virt_addr;
	//cout<< "Virtual Weight Input: " << pvirt_weight << endl;
	usedDataType *conv1Weights_1D =parseConvWeights2_1D_array<usedDataType>("resources/weights/1_F.caffemodel_conv1_weights.txt");
	std::memcpy (pvirt_weight ,conv1Weights_1D, 320*sizeof(float));

	//Virtual Address Bias
	virtualAddress Bias;
	Bias = virtual_mem_calc(BiasAddress, biasSize);
	float* pvirt_bias = (float*) Bias.virt_addr;
	//cout<< "Virtual Bias Input: " << pvirt_bias << endl;
	usedDataType *conv1bias = parseBias2array<usedDataType>("resources/weights/1_F.caffemodel_conv1_bias.txt");
	std::memcpy (pvirt_bias ,conv1bias, 20*sizeof(float));


	/*************CONV3**************************************/

	// Output for conv3 in HW
	usedDataType ***pVirtout3Dconv3 = 0;
	pVirtout3Dconv3 = build3DArray<usedDataType>(6, 6, 60);

	// Sizes for conv3 in HW
	uint32_t outputSizeConv3 = 0x21C0;		// depending on network layer size
	uint32_t inputSizeConv3 = 0x2800;		// depending on network layer size
	uint32_t weightSizeConv3 = 0x15180;		// depending on network layer size
	uint32_t biasSizeConv3 = 0xF0;			// depending on network layer size

	// Addresses for conv3 in HW
	uint32_t WeightsAddressConv3 = 0x168a0000; 	// determined by python startup script
	uint32_t BiasAddressConv3 = 0x1684b000;		// determined by python startup script
	uint32_t InputAddressConv3 = 0x1684c000;	// determined by python startup script
	uint32_t OutputAddressConv3 = 0x1687c000;	// determined by python startup script


	//Virtual Address Output Data
	virtualAddress Outputconv3;
	Outputconv3= virtual_mem_calc(OutputAddressConv3, outputSizeConv3); // length in Byte
	float* pvirtOutputconv3 = (float*) Outputconv3.virt_addr;
	//cout<< "Virtual Address Output Conv3: " << pvirtOutputconv3 << endl;
	for (unsigned int i = 0; i< (outputSizeConv3/sizeof(float)) ;i++){
		pvirtOutputconv3[i] = 0;
	};

	// Virtual Address Input Data
	virtualAddress InputConv3;
	InputConv3 = virtual_mem_calc(InputAddressConv3, inputSizeConv3); // length in Byte
	float* pvirtInputConv3 = (float*) InputConv3.virt_addr;
	//cout<< "Virtual Address Input Conv3: " << pvirtInputConv3 << endl;

	//Virtual Address WeightsConv3
	virtualAddress WeightsConv3;
	WeightsConv3 = virtual_mem_calc(WeightsAddressConv3, weightSizeConv3);
	float* pvirt_weightConv3 = (float*) WeightsConv3.virt_addr;
	//cout<< "Virtual Weight3 Input: " << pvirt_weightConv3 << endl;
	usedDataType *pvirt_weightConv3_1D =parseConvWeights2_1D_array<usedDataType>("resources/weights/1_F.caffemodel_conv3_weights.txt");
	std::memcpy (pvirt_weightConv3 ,pvirt_weightConv3_1D, weightSizeConv3);

	//Virtual Address BiasConv3
	virtualAddress BiasConv3;
	BiasConv3 = virtual_mem_calc(BiasAddressConv3, biasSizeConv3);
	float* pvirt_biasConv3 = (float*) BiasConv3.virt_addr;
	//cout<< "Virtual BiasConv3 Input: " << pvirt_biasConv3 << endl;
	usedDataType *conv3bias = parseBias2array<usedDataType>("resources/weights/1_F.caffemodel_conv3_bias.txt");
	std::memcpy (pvirt_biasConv3 ,conv3bias, biasSizeConv3);


/*********** Network Layer Declarations******************************************/
// 	Replaced by Hardware
//	Some parts are used to determine the output shape of the layer, used for layer following
//	//conv1
//	usedDataType ****conv1Weights = parseConvWeights2array<usedDataType>("resources/weights/1_F.caffemodel_conv1_weights.txt");
	layerParameter<usedDataType> lp_real_conv1;
//	lp_real_conv1.layerName = "Real Conv1";
	lp_real_conv1.stride = 1;
	lp_real_conv1.kernelSize = 4;
	lp_real_conv1.numOutput = 20;
//	lp_real_conv1.convWeights = conv1Weights;
//	lp_real_conv1.bias = conv1bias;
	lp_real_conv1.inputHeight = 39;
	lp_real_conv1.inputWidth = 39;
	lp_real_conv1.inputDepth = 1;
	outputShape conv_real_out;
	conv_real_out = conv_output_shape(39, 39, 1, lp_real_conv1.stride, 0, lp_real_conv1.numOutput, lp_real_conv1.kernelSize);
//	usedDataType ***output_real_Conv1 = 0;
//	output_real_Conv1 = build3DArray<usedDataType>(conv_real_out.outputWidth, conv_real_out.outputHeight, conv_real_out.outputDepth);
//	conv_layer<usedDataType> convLayer_real(lp_real_conv1, inputPicture, output_real_Conv1);


	// Relu1
	layerParameter<usedDataType> lp_real_ReLu;
	lp_real_ReLu.inputWidth =36;
	lp_real_ReLu.inputHeight = 36;
	lp_real_ReLu.inputDepth = 20;
	usedDataType ***output_real_ReLu = 0;
	output_real_ReLu = build3DArray<usedDataType>(lp_real_ReLu.inputWidth, lp_real_ReLu.inputHeight, lp_real_ReLu.inputDepth);
	relu_layer<usedDataType> ReLuLayer_real(lp_real_ReLu,pVirtout3D /*output_real_Conv1*/, output_real_ReLu);

	// Pool1
	layerParameter<usedDataType> lp_real_pool1;
	lp_real_pool1.layerName = "Real Pool 1";
	lp_real_pool1.stride = 2;
	lp_real_pool1.kernelSize = 2;
	lp_real_pool1.inputHeight = 36;
	lp_real_pool1.inputWidth = 36;
	lp_real_pool1.inputDepth = 20;
	outputShape pool1_real_out;
	pool1_real_out = pool_output_shape(lp_real_pool1.inputWidth, lp_real_pool1.inputHeight, lp_real_pool1.inputDepth, lp_real_pool1.stride, lp_real_pool1.kernelSize);
	usedDataType ***output_real_pool1 = 0;
	output_real_pool1 = build3DArray<usedDataType>(pool1_real_out.outputWidth, pool1_real_out.outputHeight, pool1_real_out.outputDepth);
	pooling_layer<usedDataType> Pool1Layer_real(lp_real_pool1, output_real_ReLu, output_real_pool1);

	// Conv2
	layerParameter<usedDataType> lp_real_conv2;
	lp_real_conv2.layerName = "Real Conv2";
	lp_real_conv2.stride = 1;
	lp_real_conv2.kernelSize = 3;
	lp_real_conv2.numOutput = 40;
	lp_real_conv2.convWeights = conv2Weights;
	lp_real_conv2.bias = conv2bias;
	lp_real_conv2.inputHeight = pool1_real_out.outputHeight;
	lp_real_conv2.inputWidth = pool1_real_out.outputWidth;
	lp_real_conv2.inputDepth = pool1_real_out.outputDepth;
	outputShape conv2_real_out;
	conv2_real_out = conv_output_shape(lp_real_conv2.inputHeight, lp_real_conv2.inputWidth, lp_real_conv2.inputDepth, lp_real_conv2.stride, 0, lp_real_conv2.numOutput, lp_real_conv2.kernelSize);
	usedDataType ***output_real_Conv2 = 0;
	output_real_Conv2 = build3DArray<usedDataType>(conv2_real_out.outputWidth, conv2_real_out.outputHeight, conv2_real_out.outputDepth);
	conv_layer<usedDataType> convLayer2_real(lp_real_conv2, output_real_pool1, output_real_Conv2);

	// Relu2
	layerParameter<usedDataType> lp_real_ReLu2;
	lp_real_ReLu2.inputWidth = conv2_real_out.outputWidth;
	lp_real_ReLu2.inputHeight = conv2_real_out.outputHeight;
	lp_real_ReLu2.inputDepth = conv2_real_out.outputDepth;
	usedDataType ***output_real_ReLu2 = 0;
	output_real_ReLu2 = build3DArray<usedDataType>(conv2_real_out.outputWidth, conv2_real_out.outputHeight, conv2_real_out.outputDepth);
	relu_layer<usedDataType> ReLuLayer2_real(lp_real_ReLu2, /*pVirtout3Dconv2*/output_real_Conv2, output_real_ReLu2);

	// Pool2
	layerParameter<usedDataType> lp_real_pool2;
	lp_real_pool2.layerName = "Real Pool 2";
	lp_real_pool2.stride = 2;
	lp_real_pool2.kernelSize = 2;
	lp_real_pool2.inputHeight = conv2_real_out.outputHeight;
	lp_real_pool2.inputWidth = conv2_real_out.outputWidth;
	lp_real_pool2.inputDepth = conv2_real_out.outputDepth;
	outputShape pool2_real_out;
	pool2_real_out = pool_output_shape(conv2_real_out.outputWidth, conv2_real_out.outputHeight, conv2_real_out.outputDepth, lp_real_pool2.stride, lp_real_pool2.kernelSize);
	usedDataType ***output_real_pool2 = 0;
	output_real_pool2 = build3DArray<usedDataType>(pool2_real_out.outputWidth, pool2_real_out.outputHeight, pool2_real_out.outputDepth);
	pooling_layer<usedDataType> Pool2Layer_real(lp_real_pool2, output_real_ReLu2, output_real_pool2);

// 	Replaced by Hardware
//	// conv3
//	Some parts are used to determine the output shape of the layer, used for layer following
	layerParameter<usedDataType> lp_real_conv3;
//	lp_real_conv3.layerName = "Real Conv3";
	lp_real_conv3.stride = 1;
	lp_real_conv3.kernelSize = 3;
	lp_real_conv3.numOutput = 60;
//	lp_real_conv3.convWeights = conv3Weights;
//	lp_real_conv3.bias = conv3bias;
	lp_real_conv3.inputHeight = pool2_real_out.outputHeight;
	lp_real_conv3.inputWidth = pool2_real_out.outputWidth;
	lp_real_conv3.inputDepth = pool2_real_out.outputDepth;
	outputShape conv3_real_out;
	conv3_real_out = conv_output_shape(lp_real_conv3.inputHeight, lp_real_conv3.inputWidth, lp_real_conv3.inputDepth, lp_real_conv3.stride, 0, lp_real_conv3.numOutput, lp_real_conv3.kernelSize);
//	usedDataType ***output_real_Conv3 = 0;
//	output_real_Conv3 = build3DArray<usedDataType>(conv3_real_out.outputWidth, conv3_real_out.outputHeight, conv3_real_out.outputDepth);
//	conv_layer<usedDataType> convLayer3_real(lp_real_conv3, output_real_pool2, output_real_Conv3);

	// ReLu3
	layerParameter<usedDataType> lp_real_ReLu3;
	lp_real_ReLu3.inputWidth = conv3_real_out.outputWidth;
	lp_real_ReLu3.inputHeight = conv3_real_out.outputHeight;
	lp_real_ReLu3.inputDepth = conv3_real_out.outputDepth;
	usedDataType ***output_real_ReLu3 = 0;
	output_real_ReLu3 = build3DArray<usedDataType>(conv3_real_out.outputWidth, conv3_real_out.outputHeight, conv3_real_out.outputDepth);
	relu_layer<usedDataType> ReLuLayer3_real(lp_real_ReLu3, pVirtout3Dconv3 /*output_real_Conv3*/, output_real_ReLu3);

	// Pool3
	layerParameter<usedDataType> lp_real_pool3;
	lp_real_pool3.layerName = "Real Pool 3";
	lp_real_pool3.stride = 2;
	lp_real_pool3.kernelSize = 2;
	lp_real_pool3.inputHeight = conv3_real_out.outputHeight;
	lp_real_pool3.inputWidth = conv3_real_out.outputWidth;
	lp_real_pool3.inputDepth = conv3_real_out.outputDepth;
	outputShape pool3_real_out;
	pool3_real_out = pool_output_shape(conv3_real_out.outputWidth, conv3_real_out.outputHeight, conv3_real_out.outputDepth, lp_real_pool3.stride, lp_real_pool3.kernelSize);
	usedDataType ***output_real_pool3 = 0;
	output_real_pool3 = build3DArray<usedDataType>(pool3_real_out.outputWidth, pool3_real_out.outputHeight, pool3_real_out.outputDepth);
	pooling_layer<usedDataType> Pool3Layer_real(lp_real_pool3, output_real_ReLu3, output_real_pool3);

	// Conv4
	layerParameter<usedDataType> lp_real_conv4;
	lp_real_conv4.layerName = "Real Conv4";
	lp_real_conv4.stride = 1;
	lp_real_conv4.kernelSize = 2;
	lp_real_conv4.numOutput = 80;
	lp_real_conv4.convWeights = conv4Weights;
	lp_real_conv4.bias = conv4bias;
	lp_real_conv4.inputHeight = pool3_real_out.outputHeight;
	lp_real_conv4.inputWidth = pool3_real_out.outputWidth;
	lp_real_conv4.inputDepth = pool3_real_out.outputDepth;
	outputShape conv4_real_out;
	conv4_real_out = conv_output_shape(lp_real_conv4.inputHeight, lp_real_conv4.inputWidth, lp_real_conv4.inputDepth, lp_real_conv4.stride, 0, lp_real_conv4.numOutput, lp_real_conv4.kernelSize);
	usedDataType ***output_real_Conv4 = 0;
	output_real_Conv4 = build3DArray<usedDataType>(conv4_real_out.outputWidth, conv4_real_out.outputHeight, conv4_real_out.outputDepth);
	conv_layer<usedDataType> convLayer4_real(lp_real_conv4, output_real_pool3, output_real_Conv4);

	// ReLu4
	layerParameter<usedDataType> lp_real_ReLu4;
	lp_real_ReLu4.inputWidth = conv4_real_out.outputWidth;
	lp_real_ReLu4.inputHeight = conv4_real_out.outputHeight;
	lp_real_ReLu4.inputDepth = conv4_real_out.outputDepth;
	usedDataType ***output_real_ReLu4 = 0;
	output_real_ReLu4 = build3DArray<usedDataType>(conv4_real_out.outputWidth, conv4_real_out.outputHeight, conv4_real_out.outputDepth);
	relu_layer<usedDataType> ReLuLayer4_real(lp_real_ReLu4, output_real_Conv4, output_real_ReLu4);

	// Flatten Pool 3
	layerParameter<usedDataType> lp_real_pool3_flatten;
	lp_real_pool3_flatten.inputWidth = pool3_real_out.outputWidth;
	lp_real_pool3_flatten.inputHeight = pool3_real_out.outputHeight;
	lp_real_pool3_flatten.inputDepth = pool3_real_out.outputDepth;
	int pool3_flatten_shape = pool3_real_out.outputWidth* pool3_real_out.outputHeight* pool3_real_out.outputDepth;
	usedDataType *output_real_pool3_flatten = 0;
	output_real_pool3_flatten = build1DArray<usedDataType>(pool3_flatten_shape);
	flatten_layer<usedDataType> pool3_flatten_real(lp_real_pool3_flatten, output_real_pool3, output_real_pool3_flatten);

	// Flatten Conv4
	layerParameter<usedDataType> lp_real_conv4_flatten;
	lp_real_conv4_flatten.inputWidth = conv4_real_out.outputWidth;
	lp_real_conv4_flatten.inputHeight = conv4_real_out.outputHeight;
	lp_real_conv4_flatten.inputDepth = conv4_real_out.outputDepth;
	int conv4_flatten_shape = conv4_real_out.outputWidth* conv4_real_out.outputHeight* conv4_real_out.outputDepth;
	usedDataType *output_real_conv4_flatten = 0;
	output_real_conv4_flatten = build1DArray<usedDataType>(conv4_flatten_shape);
	flatten_layer<usedDataType> conv4_flatten_real(lp_real_conv4_flatten, output_real_ReLu4, output_real_conv4_flatten);

	// Concat
	int inputData1Size = 540;
	int inputData2Size = 320;
	usedDataType *output_real_concat = 0;
	output_real_concat = build1DArray<usedDataType>(inputData1Size*inputData2Size);
	concat_layer<usedDataType> concatLayer(inputData1Size, output_real_pool3_flatten, inputData2Size, output_real_conv4_flatten, output_real_concat);

	// FC1
	layerParameter<usedDataType> lp_real_fc1;
	lp_real_fc1.fcWeights = fc1Weights;
	lp_real_fc1.bias = fc1Bias;
	lp_real_fc1.numOutput = 120;
	lp_real_fc1.inputLenght = 860;
	usedDataType *output_real_fc1 = 0;

	// FC2
	layerParameter<usedDataType> lp_real_fc2;
	lp_real_fc2.fcWeights = fc2Weights;
	lp_real_fc2.bias = fc2Bias;
	lp_real_fc2.numOutput = 10;
	lp_real_fc2.inputLenght = 120;
	usedDataType *output_real_fc2 = 0;

	// OpenCV Post Processing
	int x = 0;
	int y = 0;
	int h = 0;
	int size = 0;
	int w = 0;

	// Output Image
	Size size_original(1920,1080);
	Mat ImgOut;

	cout << "*****Face and Landmark Detection Starting*****" << endl << endl;
	cout << "Reset hardware"<< endl;
	*pmmio = 0b0;
	*pmmio= 0b1;
	sleep(1);

	while(flag){


		// Reconfiguration Thread for convolution 1
		iret = pthread_create( &partialConv1, NULL, PartialReconfigureConv1,(void *)pmmio);
		if(iret){
			fprintf(stderr,"Error - pthread_create() for conv1 return code: %d\n",iret);
			exit(EXIT_FAILURE);
		}

		// Image from Memory (HDMI)
		// Memcopy copying data from memory to new Mat
		std::memcpy (Img_in.data, pHDMIIn, sizeof(unsigned char)*width*height*channels);
		cv::cvtColor(Img_in, Img_in_grey, CV_RGB2GRAY);

		// preparation of image for CNN
		resize(Img_in_grey,Img_primed,network_fitting_size);//resize image
		image = Img_primed;

		face_cascade.detectMultiScale(image, faces, 1.2, 2,  CASCADE_SCALE_IMAGE, Size(30, 30));


		if (!faces.empty()) { // Make Sure a face is detected -> vector out of Range Issue

			bbox.left = faces[0].x;
			bbox.right = faces[0].x + faces[0].width;
			bbox.top = faces[0].y;
			bbox.bottom = faces[0].y + faces[0].height;
			bbox.x = faces[0].x;
			bbox.y = faces[0].y;
			bbox.w = faces[0].width;
			bbox.h = faces[0].height;

			subbox.leftDelta = bbox.w * 0.1;
			subbox.rightDelta = bbox.w * 0.9;
			subbox.topDelta = bbox.h * 0.2;
			subbox.bottomDelta = bbox.h*1;
			subbox.left = bbox.left + subbox.leftDelta;
			subbox.right = bbox.left + subbox.rightDelta;
			subbox.top = bbox.top + subbox.topDelta;
			subbox.bottom = bbox.bottom + subbox.topDelta;

			bboxFromSubbox.left = subbox.left;
			bboxFromSubbox.right = subbox.right;
			bboxFromSubbox.top = subbox.top;
			bboxFromSubbox.bottom = subbox.bottom;
			bboxFromSubbox.x = subbox.left;
			bboxFromSubbox.y = subbox.top;
			bboxFromSubbox.w = subbox.right - subbox.left;
			bboxFromSubbox.h = subbox.bottom - subbox.top;

			roi.x = bboxFromSubbox.x-1;
			roi.y = bboxFromSubbox.y-1;
			roi.width = bboxFromSubbox.w-1;
			roi.height = bboxFromSubbox.h-1;
			cropped = image(roi);

		}

		else{
			cropped = image; // if faces == empty neccessary
		}


		resize(cropped, resizedImg, cvSize(39, 39));
		resizedImg.convertTo(converted, CV_32F);


		// calculation standard derivation and mean pixel value to create same conditions with TB
		// Input Picture for HW
		float *inputPicture = new float [converted.cols*converted.rows];
		meanStdDev(converted, mean, std);
		for (int i = 0; i < converted.cols; i++) {
			for (int j = 0; j < converted.rows; j++) {
				inputPicture[i+converted.cols*j] = (converted.at<float>(i, j) - mean[0]) / std[0];

			}
		};
		std::memcpy (pvirtInput ,inputPicture, sizeof(float)*39*39);


		// Thread for Conv1 reconfiguration must be ready
		pthread_join(partialConv1, NULL);

		// Time measuring
		double ALL = 0;
		double tstart2 = clock();

		// IP Core configuration
		pConvHW[4] = WeightsAddress;
		pConvHW[6] = BiasAddress;
		pConvHW[0] = 0x1; // starting

		// DMA Reset
		dma_set(dma0_reg, MM2S_CONTROL_REGISTER,4);
		dma_set(dma0_reg, S2MM_CONTROL_REGISTER,4);

		// DMA halting
		dma_set(dma0_reg, S2MM_CONTROL_REGISTER, 0);
		dma_set(dma0_reg, MM2S_CONTROL_REGISTER, 0);

		// DMA set addresses
		dma_set(dma0_reg, MM2S_START_ADDRESS_LSB, InputAddress);
		dma_set(dma0_reg, S2MM_DESTINATION_ADDRESS_LSB, OutputAddress);

		// DMA starting
		dma_set(dma0_reg, S2MM_CONTROL_REGISTER, 0x1);
		dma_set(dma0_reg, MM2S_CONTROL_REGISTER, 0x1);

		// DMA setting length registers, transfer starts afterwards
		dma_set(dma0_reg, MM2S_LENGTH, inputSize);
		usleep(8000); // determined empirically
		dma_set(dma0_reg, S2MM_LENGTH, outputSize);

		int o = 0;
		for ( int i = 0; i < 20;i++){
			for (int a = 0; a<36; a++){
				for (int b = 0; b<36;b++){
					 pVirtout3D[b][a][i] = pvirtOutput[o];
					 o++;
				}
			}
		}

		// Reconfiguration Thread for convolution 3
		iret3 = pthread_create( &partialConv3, NULL, PartialReconfigureConv3,(void *)pmmio);
		if(iret3){
			fprintf(stderr,"Error - pthread_create() for conv3 return code: %d\n",iret3);
			exit(EXIT_FAILURE);
		}

// 		Replaced by Hardware
//		//Conv1
//		convLayer_real.forward();

		//ReLu1
		ReLuLayer_real.forward();

		// pool1
		Pool1Layer_real.forward();

		//conv2
		convLayer2_real.forward();

		//ReLu2
		ReLuLayer2_real.forward();

		// pool2
		Pool2Layer_real.forward();

		o=0;
		for ( int i = 0; i < 40;i++){
			for (int a = 0; a<8; a++){
				for (int b = 0; b<8;b++){
					pvirtInputConv3[o] = output_real_pool2[b][a][i];
					o++;
				}
			}
		}

		// Thread for Conv3 reconfiguration must be ready
		pthread_join(partialConv3, NULL);

		// IP Core configuration
		pConvHW[4] = WeightsAddressConv3;
		pConvHW[6] = BiasAddressConv3;
		pConvHW[0] = 0x1; // starting

		// DMA Reset
		dma_set(dma0_reg, S2MM_CONTROL_REGISTER,4);
		dma_set(dma0_reg, MM2S_CONTROL_REGISTER,4);

		// DMA halting
		dma_set(dma0_reg, S2MM_CONTROL_REGISTER, 0);
		dma_set(dma0_reg, MM2S_CONTROL_REGISTER,0);

		// DMA set addresses
		dma_set(dma0_reg, MM2S_START_ADDRESS_LSB, InputAddressConv3);
		dma_set(dma0_reg, S2MM_DESTINATION_ADDRESS_LSB, OutputAddressConv3);

		// DMA starting
		dma_set(dma0_reg, S2MM_CONTROL_REGISTER, 0x1);
		dma_set(dma0_reg, MM2S_CONTROL_REGISTER, 0x1);

		// DMA setting length registers, transfer starts afterwards
		dma_set(dma0_reg, MM2S_LENGTH, inputSizeConv3);
		usleep(8000); // determined empirically
		dma_set(dma0_reg, S2MM_LENGTH, outputSizeConv3);

		o = 0;
		for ( int i = 0; i < 60;i++){
			for (int a = 0; a<6; a++){
				for (int b = 0; b<6;b++){
					pVirtout3Dconv3[b][a][i] = pvirtOutputconv3[o];
					 o++;
				}
			}
		}


// 		Replaced by Hardware
//		//conv3
//		convLayer3_real.forward();

		//ReLu3
		ReLuLayer3_real.forward();

		// pool3
		Pool3Layer_real.forward();

		//conv4
		convLayer4_real.forward();

		//ReLu4
		ReLuLayer4_real.forward();

		// flatten pool3
		pool3_flatten_real.forward();

		// flatten conv4
		conv4_flatten_real.forward();

		// concat
		concatLayer.forward();

		// Fc1
		output_real_fc1 = build1DArray<usedDataType>(lp_real_fc1.numOutput);
		fc_layer <usedDataType> fc1_layer(lp_real_fc1, output_real_concat, output_real_fc1);
		fc1_layer.forward();


	/*
		//ReLu Fc1
		layerParameter<usedDataType> lp_real_ReLuFc1;
		lp_real_ReLuFc1.inputWidth = 1;
		lp_real_ReLuFc1.inputHeight = 120;
		lp_real_ReLuFc1.inputDepth = 1;
		usedDataType ***output_real_ReLuFc1 = 0;
		output_real_ReLuFc1 = build3DArray<usedDataType>(lp_real_ReLuFc1.inputWidth, lp_real_ReLuFc1.inputHeight, lp_real_ReLuFc1.inputDepth);
		relu_layer<usedDataType> ReLuLayer4_real(lp_real_ReLuFc1, output_real_fc1, output_real_ReLuFc1);
		ReLuLayer4_real.forward();
	*/

		// FC2
		output_real_fc2 = build1DArray<usedDataType>(lp_real_fc2.numOutput);
		fc_layer <usedDataType> fc2_layer(lp_real_fc2, output_real_fc1, output_real_fc2);
		fc2_layer.forward();

		ALL+= clock() - tstart2;    // end
		ALL = ALL/CLOCKS_PER_SEC;  // rescale to seconds
		cout << "Time for network execution = " << ALL << " sec." << endl;


		// OpenCV Post Processing
		x = bboxFromSubbox.x;
		y = bboxFromSubbox.y;
		h = bboxFromSubbox.h;
		w = bboxFromSubbox.w;
		size = 1;

		// OpenCV Post Processing
		if (!faces.empty()){ // Make Sure a face is detected -> vector out of Range Issue
			Point topLeft (bboxFromSubbox.left, bboxFromSubbox.top);
			Point bottomRight (bboxFromSubbox.right, bboxFromSubbox.bottom);

			cv::cvtColor(image, image, cv::COLOR_GRAY2RGB);
			cv::rectangle(image, topLeft, bottomRight,(0, 0, 255),size);
			cv::circle(image, cv::Point(int(x + w*output_real_fc2[0]), int(y + h*output_real_fc2[1])), size, Scalar(0, 255, 0),  -1);
			cv::circle(image, cv::Point(int(x + w*output_real_fc2[2]), int(y + h*output_real_fc2[3])), size, Scalar(0, 255, 0),  -1);
			cv::circle(image, cv::Point(int(x + w*output_real_fc2[4]), int(y + h*output_real_fc2[5])), size, Scalar(0, 255, 0),  -1);
			cv::circle(image, cv::Point(int(x + w*output_real_fc2[6]), int(y + h*output_real_fc2[7])), size, Scalar(0, 255, 0),  -1);
			cv::circle(image, cv::Point(int(x + w*output_real_fc2[8]), int(y + h*output_real_fc2[9])), size, Scalar(0, 255, 0),  -1);
		}

		else{
			cv::cvtColor(image, image, cv::COLOR_GRAY2RGB);
		}

		// Resizing to Original Image Size
		resize(image,ImgOut,size_original);//resize image

		// Copy to HDMI Out Memory Location
		std::memcpy (pHDMIOut,ImgOut.data, sizeof(unsigned char)*width*height*channels);
		delete[] inputPicture;

	}


	// Cleanup
	cout << "Interrupt Signal Received" <<endl;
	cout << "******Cleanup*******" << endl;
	cout << "Memory Pointer Cleanup"<<endl;

	// unmapping Virtual Addresses
	munmap(IPcore.mem_pointer, 0xfff);
	munmap(Dma0.mem_pointer, 0xfff);
	munmap(mmio.mem_pointer,0x50);

	munmap(HDMIIn.mem_pointer,InHDMISize);
	munmap(HDMIOut.mem_pointer,OutHDMISize);

	munmap(Input.mem_pointer, inputSize);
	munmap(Output.mem_pointer, outputSize);
	munmap(Weights.mem_pointer, 0x500);
	munmap(Bias.mem_pointer, 0x50);

	munmap(InputConv3.mem_pointer,inputSizeConv3);
	munmap(Outputconv3.mem_pointer,outputSizeConv3);
	munmap(WeightsConv3.mem_pointer,weightSizeConv3);
	munmap(BiasConv3.mem_pointer,biasSizeConv3);



	cout << "Freeing all dynamic Arrays......" ;
//	free3DArray<usedDataType>(conv_real_out.outputWidth, conv_real_out.outputHeight, output_real_Conv1);
//	free3DArray<usedDataType>(conv_real_out.outputWidth, conv_real_out.outputHeight, output_real_ReLu );
	free3DArray<usedDataType>(pool1_real_out.outputWidth, pool1_real_out.outputHeight, output_real_pool1 );

	free3DArray<usedDataType>(conv2_real_out.outputWidth, conv2_real_out.outputHeight, output_real_Conv2 );
	free3DArray<usedDataType>(conv2_real_out.outputWidth, conv2_real_out.outputHeight, output_real_ReLu2 );
	free3DArray<usedDataType>(pool2_real_out.outputWidth, pool2_real_out.outputHeight, output_real_pool2);

//	free3DArray<usedDataType>(conv3_real_out.outputWidth, conv3_real_out.outputHeight, output_real_Conv3 );
//	free3DArray<usedDataType>(conv3_real_out.outputWidth, conv3_real_out.outputHeight, output_real_ReLu3 );
	free3DArray<usedDataType>(pool3_real_out.outputWidth, pool3_real_out.outputHeight, output_real_pool3 );

	free3DArray<usedDataType>(conv4_real_out.outputWidth, conv4_real_out.outputHeight, output_real_Conv4 );
	free3DArray<usedDataType>(conv4_real_out.outputWidth, conv4_real_out.outputHeight, output_real_ReLu4 );
	free1DArray<usedDataType>(output_real_pool3_flatten);

	free1DArray<usedDataType>(output_real_conv4_flatten);
	free1DArray<usedDataType>(output_real_concat);

	free1DArray<usedDataType>(output_real_fc1);
	free1DArray<usedDataType>(output_real_fc2);

	cout << "Done" << endl;

return 0;
}


