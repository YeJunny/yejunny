#include "Array2D.h"

Array2D::Array2D(int width, int height)
	: mWidth(width)
{
	mArray2D = new char[width * height];
}

Array2D::~Array2D()
{
	delete[] mArray2D;
}

char& Array2D::operator()(int width, int height)
{
	return mArray2D[height * mWidth + width];
}

const char& Array2D::operator()(int width, int height) const
{
	return mArray2D[height * mWidth + width];
}