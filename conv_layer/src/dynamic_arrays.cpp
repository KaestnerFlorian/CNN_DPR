//4D Array
template <class T>
T **** build4DArray(int width, int height, int depth, int amountCount) {
	T ****arr;
	arr = new T ***[width];
	for (int i = 0; i < width; i++) {
		arr[i] = new T**[height];
		for (int j = 0; j < height; j++) {
			arr[i][j] = new T*[depth];
			for (int k = 0; k < depth; k++) {
				arr[i][j][k] = new T[amountCount];
			}
		}
	}


	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			for (int k = 0; k < depth; k++) {
				for (int l = 0; l < amountCount; l++) {
					arr[i][j][k][l] = 0;
				}
			}
		}
	}
	return arr;
}


template <class T>
void free4DArray(int width, int height, int depth, T ****p) {

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			for (int k=0; k< depth; k++){
				delete[] p[i][j][k];

			}
			delete[] p[i][j];
		}
		delete[] p[i];
	}
	delete[] p;
}





// 3D Array
template <class T>
T *** build3DArray(int width, int height, int depth) {
	T ***arr;
	arr = new T **[width];
	for (int i = 0; i < width; i++) {
		arr[i] = new T*[height];
		for (int j = 0; j < height; j++) {
			arr[i][j] = new T[depth];
		}
	}

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			for (int k = 0; k < depth; k++) {
				arr[i][j][k] = 0;
			}
		}
	}
	return arr;
}


// 3D Array
template <class T>
void free3DArray(int width, int height, T ***p) {

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			delete[] p[i][j];
		}
		delete[] p[i];
	}
	delete[] p;
}


// 2D Array
template <class T>
T ** build2DArray(int width, int height) {
	T **arr;
	arr = new T *[width];
	for (int i = 0; i < width; i++) {
		arr[i] = new T[height];
	}

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
				arr[i][j] = 0;
		}
	}
	return arr;
}


template <class T>
void free2DArray(int width, T **p) {

	for (int i = 0; i < width; i++) {
		delete[] p[i];
	}

	delete[] p;
}



// 1D Array
template <class T>
T * build1DArray(int width) {
	T *arr;
	arr = new T [width];

	for (int i = 0; i < width; i++) {
			arr[i] = 0;
	}
	return arr;
}

// Free 1D Array
template <class T>
void free1DArray(T *p) {
	delete [] p;
}

