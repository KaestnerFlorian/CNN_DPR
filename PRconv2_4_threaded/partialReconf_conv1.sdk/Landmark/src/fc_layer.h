#ifndef FC_LAYER_H
#define FC_LAYER_H


template <class type>
class fc_layer :
	public base_layer<type>
{
public:
	fc_layer();
	fc_layer(struct layerParameter<type> lp, type *inputData, type *outputData);

	~fc_layer();
	void forward();


private:
	type *inputData;
	type *outputData;
	type **fcWeights;
	type *bias;
	int inputLenght;
	int numOutput = 0;

};




// IMPLEMENTATION

template <class type>
fc_layer<type>::fc_layer()
{

}


template <class type>
fc_layer<type>::fc_layer(struct layerParameter<type> lp, type *inputData, type *outputData)
{
	this->bias = lp.bias;
	this->fcWeights = lp.fcWeights;
	this->inputData = inputData;
	this->outputData = outputData;
	this->inputLenght = lp.inputLenght;
	this->numOutput = lp.numOutput;

}





template <class type>
void fc_layer<type>::forward()
{
	for (int outputCounter = 0; outputCounter < numOutput; outputCounter++) {

		for (int inputCounter = 0; inputCounter < this->inputLenght; inputCounter++) {
			//cout << outputCounter << " " << inputCounter << " " << inputCounter << " " << outputCounter << endl;
			outputData[outputCounter] += inputData[inputCounter] * fcWeights[inputCounter][outputCounter];
			
		}
		outputData[outputCounter] = outputData[outputCounter] + bias[outputCounter];
		if (outputData[outputCounter] < 0)
			outputData[outputCounter] = 0;
		//cout << "Position: " << outputCounter << endl;
		//cout << "Data: " << outputData[outputCounter] << endl;
	}



}

template <class type>
fc_layer<type>::~fc_layer()
{

}






#endif
