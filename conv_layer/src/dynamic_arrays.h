#ifndef DYN_ARRAY_H
#define DYN_ARRAY_H

#include "dynamic_arrays.cpp"

// 4D Array
template <class T>
T **** build4DArray(int width, int height, int depth, int amountCount);

template <class T>
void free4DArray(int width, int height ,int depth, T ****p);


// 3D Array
template <class T>
T *** build3DArray(int width, int height, int depth);

// 3D Array
template <class T>
void free3DArray(int width, int height, T ***p);


// 2D Array
template <class T>
T ** build2DArray(int width, int height);


template <class T>
void free2DArray(int width, T **p);

// 1D Array
template <class T>
T * build1DArray(int width);

template <class T>
void free1DArray(T* p);

#endif
