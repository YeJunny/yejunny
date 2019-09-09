#pragma once

class Image
{
public:
	Image(const char* fileName);
	~Image();
	int GetWidth() const;
	int GetHeight() const;
	unsigned int GetData(int index) const;

private:
	int mWidth;
	int mHeight;
	unsigned int* mData;
};