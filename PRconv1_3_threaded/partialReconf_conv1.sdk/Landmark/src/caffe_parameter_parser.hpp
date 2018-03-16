#ifndef CAFFE_PARAM_PARSE
#define CAFFE_PARAM_PARSE

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <typeinfo>
#include <cstdlib>
#include "dynamic_arrays.h"


int getDimension(std::ifstream &file, char delimiter1, char delimiter2) {
	std::string line;
	int count = 0;
	if ((delimiter1 == (';')) | (delimiter1 == ('\0'))) {
		count = 1;
	}
	std::getline(file, line, delimiter1);
	for (unsigned int a = 0; a <= line.length(); a++) {
		if (line[a] == delimiter2) {
			count++;
		}
	}
	std::cout << "Dimension: " << count << std::endl;
	return count;
}

int getDimension(std::ifstream &file, std::string dimension) {
	std::string line;
	char delimiter1 = 0;
	char delimiter2 = 0;
	int count = 0;
	if (dimension == "spatial") {
		delimiter1 = ';';
		delimiter2 = ',';
		count = 1;
	}
	else if (dimension == "depth") {
		delimiter1 = '#';
		delimiter2 = '\n';
		count = 0;
	}
	else if (dimension == "amount") {
		delimiter1 = '\0';
		delimiter2 = '#';
		count = 1;
	}
	std::getline(file, line, delimiter1);
	for (unsigned int a = 0; a <= line.length(); a++) {
		if (line[a] == delimiter2) {
			count++;
		}
	}
	std::cout << "Dimension: " << count << std::endl;
	return count;
}

template <class T>
T **** parseConvWeights2array(std::string filename) {

	std::ifstream file;
	file.open(filename.c_str());
	std::string line;
	T ****arr = NULL;

	if (file.is_open()) {
		std::cout << "File \" " << filename << "\" successfully opened" << std::endl;

		int commaCount = getDimension(file, "spatial");
		int depthCount = getDimension(file, "depth");
		int amountCount = getDimension(file, "amount");

		file.close(); // closing file

		arr = build4DArray<T>(commaCount, commaCount, depthCount, amountCount);

		// Getting values from txt, and store them to array
		file.open(filename.c_str());
		std::string textLine;
		int height = 0;
		int width = 0;
		int depth = 0;
		int amount = 0;
		while (std::getline(file, textLine)) {
			std::istringstream lineStream(textLine);
			while (std::getline(lineStream, textLine, ';')) {
				//std::cout << textLine << " after it" << std::endl;
				std::istringstream lineStream(textLine);
				while (std::getline(lineStream, textLine, ',')) {
					//std::cout << textLine << std::endl;
					if (textLine[0] == '#') {
						textLine.erase(textLine.begin(), textLine.end());
					}
					else {
						arr[height][width][depth][amount] = atof(textLine.c_str()); // solving problem with different datatypes
						height++;
						if (height == commaCount) {
							height = 0;
							width++;
						}
						if (width == commaCount) {
							width = 0;
							depth++;
						}
						if (depth == depthCount) {
							depth = 0;
							amount++;
						}
						if (amount == amountCount) {
							break; // not neccessary
						}
					}
				}
			}
		}

		std::cout << "File content successfully parsed into c++ array" << std::endl;
		// Printing Array content just for Debugging
		/*int counter = 0;
		for (int l = 0; l < amountCount; l++){
		for (int k = 0; k < depthCount; k++){
		for (int j = 0; j < commaCount; j++){
		for (int i = 0; i < commaCount; i++){
		std::cout << "Array Values: " << arr[i][j][k][l] << std::endl;
		counter++;
		std::cout << "Counter: " << counter<< std::endl;
		}
		}
		}
		}*/
	}

	else {
		std::cerr << "***ERROR: File \" " << filename << "\" could not be opened" << std::endl;
	}

	return arr;
}


template <class T>
T * parseBias2array(std::string filename) {
	std::ifstream file; // stream for input file
	std::string textLine; // string for content
	T* arr = NULL; // pointer to array
	int count = 0; // counter for filling array

	file.open(filename.c_str());
	if (file.is_open()) {
		std::cout << "File \" " << filename << "\" successfully opened" << std::endl;

		int dimension = getDimension(file, "depth");
		arr = new T[dimension];
		//*arr = { 0 }; // initialise array with 0
		file.close();
	}
	else {
		std::cerr << "***ERROR: File \" " << filename << "\" could not be opened" << std::endl;
	}

	file.open(filename.c_str());
	if (file.is_open()) {
		std::cout << "File \" " << filename << "\" successfully opened" << std::endl;

		while (std::getline(file, textLine)) {
			arr[count] = atof(textLine.c_str());
			//std::cout << "Array values: " << arr[count] << std::endl;
			count++;
		}
		std::cout << "File content successfully parsed into c++ array" << std::endl;
	}
	else {
		std::cerr << "***ERROR: File \" " << filename << "\" could not be opened" << std::endl;
	}

	return arr;
}

template <class T>
T ** parseFcWeights2array(std::string filename) {
	std::ifstream file; // stream for input file
	std::string textLine; // string for content
	T ** arr = NULL;

	file.open(filename.c_str());
	if (file.is_open()) {
		std::cout << "File \" " << filename << "\" successfully opened" << std::endl;

		int inputs = 0;
		int outputs = 1;

		// getting Dimensions
		std::getline(file, textLine);
		for (unsigned int a = 0; a <= textLine.length(); a++) {
			if (textLine[a] == ',') {
				inputs++;
			}
		}
		std::cout << "Inputs: " << inputs << std::endl;
		file.close();

		file.open(filename.c_str());
		std::getline(file, textLine, '\0');
		for (unsigned int a = 0; a <= textLine.length(); a++) {
			if (textLine[a] == '\n') {
				outputs++;
			}
		}
		std::cout << "Outputs: " << outputs << std::endl;
		file.close();

		// building 2D array
		arr = new T *[inputs];
		for (int i = 0; i < inputs; i++) {
			arr[i] = new T[outputs];
		}
		//**arr = { 0 }; // initialising array with zero

					   // getting values and write them into array
		file.open(filename.c_str());
		int inputCount = 0;
		int outputCount = 0;
		while (std::getline(file, textLine, ',')) {
			arr[inputCount][outputCount] = atof(textLine.c_str());
			//std::cout << "Array values: " << inputCount << ' ' << outputCount << ' ' << arr[inputCount][outputCount] << std::endl;
			inputCount++;
			if (inputCount == inputs) {
				inputCount = 0;
				outputCount++;
			}
		}
	}

	else {
		std::cerr << "***ERROR: File \" " << filename << "\" could not be opened" << std::endl;
	}

	return arr;
}

// HLS only works with 1D Arrays
template <class T>
T *parseConvWeights2_1D_array(std::string filename) {

	std::ifstream file;
	file.open(filename.c_str());
	std::string line;
	T *arr = NULL;

	if (file.is_open()) {
		std::cout << "File \" " << filename << "\" successfully opened" << std::endl;

		int commaCount = getDimension(file, "spatial");
		int depthCount = getDimension(file, "depth");
		int amountCount = getDimension(file, "amount");

		file.close(); // closing file

		int temp = commaCount*commaCount*depthCount*amountCount;
		arr = new T[temp];
		//std::cout << commaCount*commaCount*depthCount*amountCount << std::endl;

		// Getting values from txt, and store them to array
		file.open(filename.c_str());
		std::string textLine;
		int height = 0;
		int width = 0;
		int depth = 0;
		int amount = 0;
		while (std::getline(file, textLine)) {
			std::istringstream lineStream(textLine);
			while (std::getline(lineStream, textLine, ';')) {
				//std::cout << textLine << " after it" << std::endl;
				std::istringstream lineStream(textLine);
				while (std::getline(lineStream, textLine, ',')) {
					//std::cout << textLine << std::endl;
					if (textLine[0] == '#') {
						textLine.erase(textLine.begin(), textLine.end());
					}
					else {
						register int pos = height+width*commaCount+depth*commaCount*commaCount+amount*commaCount*commaCount*depthCount;
						arr[pos]= atof(textLine.c_str());
						//std::cout << pos<< std::endl;
						//std::cout << arr[pos]<< std::endl;
						//std::cout << arr << std::endl;
						//arr[height][width][depth][amount] = atof(textLine.c_str()); // solving problem with different datatypes
						height++;
						if (height == commaCount) {
							height = 0;
							width++;
						}
						if (width == commaCount) {
							width = 0;
							depth++;
						}
						if (depth == depthCount) {
							depth = 0;
							amount++;
						}
						if (amount == amountCount) {
							break; // not neccessary
						}
					}
				}
			}
		}

		std::cout << "File content successfully parsed into c++ array" << std::endl;
		// Printing Array content just for Debugging
		/*int counter = 0;
		for (int l = 0; l < amountCount; l++){
		for (int k = 0; k < depthCount; k++){
		for (int j = 0; j < commaCount; j++){
		for (int i = 0; i < commaCount; i++){
		std::cout << "Array Values: " << arr[i][j][k][l] << std::endl;
		counter++;
		std::cout << "Counter: " << counter<< std::endl;
		}
		}
		}
		}*/
	}

	else {
		std::cerr << "***ERROR: File \" " << filename << "\" could not be opened" << std::endl;
	}

	return arr;
}

#endif
