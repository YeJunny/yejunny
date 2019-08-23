#pragma once

class Array2D
{
public:
	Array2D(int width, int height);
	~Array2D();
	char& operator()(int width, int height);
	const char& operator()(int width, int height) const;

private:
	char* mArray2D;
	int mWidth;
};