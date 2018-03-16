#ifndef FLATTEN_LAYER
#define FLATTEN_LAYER


template <class type>
class flatten_layer :
	public base_layer<type>
{
public:
	flatten_layer();
	flatten_layer(struct layerParameter<type> lp, type ***inputData, type *outputData);

	~flatten_layer();
	void forward();


private:
	type ***inputData;
	type *outputData;
	int inputWidth;
	int inputHeight;
	int inputDepth;
};




// IMPLEMENTATION

template <class type>
flatten_layer<type>::flatten_layer()
{

}


template <class type>
flatten_layer<type>::flatten_layer(struct layerParameter<type> lp, type ***inputData, type *outputData)
{
	this->inputData = inputData;
	this->outputData = outputData;
	this->inputWidth = lp.inputWidth;
	this->inputHeight = lp.inputHeight;
	this->inputDepth = lp.inputDepth;

}


// caffe flattening implementation: NxCxWxH H first then W and then C
// 1. first channels first rows first element
// 2. first channels first rows second element
// 3. .................
// 4. first channels second rows first element
// 5. .............
// 6. second channels first rows first element

template <class type>
void flatten_layer<type>::forward()
{
	int inputWidth = this->inputWidth;
	int inputHeight = this->inputHeight;
	int inputDepth = this->inputDepth;
	int count = 0;
	for (int depth = 0; depth < inputDepth; depth++) {
		for (int lines = 0; lines < inputWidth; lines++) {
			for (int row = 0; row < inputHeight; row++) {
		
				outputData[count] = inputData[lines][row][depth];
				//cout << "Data: " <<inputData[lines][row][depth] << endl;
				//cout << "Position: " << lines<< " " << row << " " <<depth << endl;
				//cout << "Data: " << outputData[count] << endl;
				//cout << "Position: " << count << endl;
				count++;

			}
		}
	}







}

template <class type>
flatten_layer<type>::~flatten_layer()
{

}






#endif
