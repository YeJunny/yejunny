#include "File.h"
#include "Image.h"

Image::Image(const char* fileName)
{
	File* file = new File(fileName);
	char* str = file->GetReadingFile();
	mHeight = file->GetUnsignedInt(&str[12]);
	mWidth = file->GetUnsignedInt(&str[16]);
	mData = new unsigned int[mWidth * mHeight];
	for (int i = 0; i < mWidth * mHeight; ++i)
	{
		mData[i] = file->GetUnsignedInt(&str[128 + i * 4]);
	}
	delete file;
}

Image::~Image()
{
	delete[] mData;
}

int Image::GetWidth() const
{
	return mWidth;
}

int Image::GetHeight() const
{
	return mHeight;
}

unsigned int Image::GetData(int index) const
{
	return mData[index];
}
