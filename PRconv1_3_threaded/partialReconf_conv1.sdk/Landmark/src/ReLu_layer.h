#ifndef RELU_LAYER_H
#define RELU_LAYER_H


template <class type>
class relu_layer :
	public base_layer<type>
{
public:
	relu_layer();
	relu_layer(struct layerParameter<type> lp, type ***inputData, type ***outputData);
	relu_layer(struct layerParameter<type> lp, type *inputData, type *outputData);

	~relu_layer();
	void forward();


private:
	type ***inputData;
	type ***outputData;
	int inputWidth;
	int inputHeight;
	int inputDepth;

};


// IMPLEMENTATION

template <class type>
relu_layer<type>::relu_layer()
{

}


template<class type>
relu_layer<type>::relu_layer(struct layerParameter<type> lp, type ***inputData, type ***outputData)
{
	this->inputData = inputData;
	this->outputData = outputData;
	this->inputWidth = lp.inputWidth;
	this->inputHeight = lp.inputHeight;
	this->inputDepth = lp.inputDepth;
}

template<class type>
relu_layer<type>::relu_layer(struct layerParameter<type> lp, type *inputData, type *outputData)
{
	this->inputData = inputData;
	this->outputData = outputData;
	this->inputWidth = lp.inputWidth;
	this->inputHeight = lp.inputHeight;
	this->inputDepth = lp.inputDepth;
}

// doing forward path (ReLu)
//*******************************************************************************
// TODO:
// DONE: Yet hardcoded width, height, depht should be somehow passed to object
//
//*******************************************************************************

template<class type>
void relu_layer<type>::forward()
{

	int inputWidth = this->inputWidth;
	int inputHeight = this->inputHeight;
	int inputDepth = this->inputDepth;

	for (int depthCount = 0; depthCount < inputDepth; depthCount++) {
		for (int widthCount = 0; widthCount < inputWidth; widthCount++) {
			for (int heightCount = 0; heightCount < inputHeight; heightCount++) {

				if ((this->inputData[heightCount][widthCount][depthCount]) < 0)
					this->outputData[heightCount][widthCount][depthCount] = 0;
				else 
					this->outputData[heightCount][widthCount][depthCount] = this->inputData[heightCount][widthCount][depthCount];
				//cout << "Data Position: " << widthCount << " " << heightCount << " " << depthCount << endl;
				//cout << "Data: " << outputData[heightCount][widthCount][depthCount] << endl;
			}   
		}
	}

}


template <class type>
relu_layer<type>::~relu_layer()
{
}



#endif
