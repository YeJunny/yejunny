#pragma once

class Image
{
public:
	Image(const char* fileName);
	~Image();
	int GetWidth() const;
	int GetHeight() const;
	unsigned int GetData(int index) const;
	void Draw(double x, double y, int iX, int iY, int eX, int eY) const;

private:
	int mWidth;
	int mHeight;
	unsigned int* mData;
};