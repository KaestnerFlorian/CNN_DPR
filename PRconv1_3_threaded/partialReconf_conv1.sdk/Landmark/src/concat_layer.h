#ifndef CONCAT_LAYER_H
#define CONCAT_LAYER_H


template <class type>
class concat_layer :
	public base_layer<type>
{
public:
	concat_layer();
	concat_layer(int sizeInputData1 ,type *inputData1, int sizeInputData2, type *inputData2, type *outputData);

	~concat_layer();
	void forward();


private:
	type *inputData1;
	type *inputData2;
	type *outputData;
	int sizeInputData1;
	int sizeInputData2;

};



// IMPLEMENTATION

template <class type>
concat_layer<type>::concat_layer()
{

}


template <class type>
concat_layer<type>::concat_layer(int sizeInputData1, type *inputData1, int sizeInputData2, type *inputData2, type *outputData)
{
	this->inputData1 = inputData1;
	this->inputData2 = inputData2;
	this->outputData = outputData;
	this->sizeInputData1 = sizeInputData1;
	this->sizeInputData2 = sizeInputData2;
}





template <class type>
void concat_layer<type>::forward()
{
	int handover = 0;

	for (int counter1 = 0; counter1 < sizeInputData1; counter1++) {
		outputData[counter1] = inputData1[counter1];
		handover++;
		//cout << "Data: " << outputData[counter1] << endl;
		//cout << "position: " << counter1 << endl;
	}

	for (int counter2 = 0; counter2 < sizeInputData2; counter2++) {
		outputData[counter2 + handover] = inputData2[counter2];
		//cout << "Data: " << outputData[counter2+handover] << endl;
		//cout << "position: " << counter2+handover << endl;
	}



}

template <class type>
concat_layer<type>::~concat_layer()
{

}


#endif