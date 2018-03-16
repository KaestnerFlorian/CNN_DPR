#ifndef BASE_LAYER_H
#define BASE_LAYER_H

template <class type>
class base_layer
{
public:
	// default constructor
	base_layer();

	// specified constructor
	base_layer(struct layerParameter<type> lp, type ***inputData, type ***outputData);

	type ****getInput();
	type ****getOutput();


	// destructor
	~base_layer();

private:
	type **** inputData;
	type **** outputData;
};



template <class type>
base_layer<type>::base_layer()
{
}

template <class type>
base_layer<type>::base_layer(struct layerParameter<type> lp, type ***inputData, type ***outputData)
{
	this->inputData = inputData;
	this->outputData = outputData;
}

template <class type>
type ****base_layer<type>::getInput()
{
	return this->inputData;
}

template <class type>
type ****base_layer<type>::getOutput()
{
	return this->outputData;
}
template <class type>
base_layer<type>::~base_layer()
{
}



#endif
