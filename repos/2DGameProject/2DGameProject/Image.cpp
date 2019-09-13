#include "File.h"
#include "GameLib/Framework.h"
#include "Image.h"

using namespace GameLib;

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

void Image::Draw(double x, double y, int iX, int iY, int eX, int eY) const
{
	unsigned int* vram = Framework::instance().videoMemory();
	unsigned int windowWidth = Framework::instance().width();

	for (int dy = 0; dy < eY - iY; dy++)
	{
		for (int dx = 0; dx < eX - iX; dx++)
		{
			unsigned int src = mData[(iY + dy) * mWidth + iX + dx];
			unsigned int* dst = &vram[static_cast<int>(y + dy) * windowWidth + static_cast<int>(x + dx)];

			unsigned int srcA = (src & 0xff000000) >> 24;
			unsigned int srcR = src & 0x00ff0000;
			unsigned int srcG = src & 0x0000ff00;
			unsigned int srcB = src & 0x000000ff;

			unsigned int dstR = *dst & 0x00ff0000;
			unsigned int dstG = *dst & 0x0000ff00;
			unsigned int dstB = *dst & 0x000000ff;

			unsigned int r = (srcR - dstR) * srcA / 255 + dstR;
			unsigned int g = (srcG - dstG) * srcA / 255 + dstG;
			unsigned int b = (srcB - dstB) * srcA / 255 + dstB;

			*dst = (r & 0x00ff0000) | (g & 0x0000ff00) | b;
		}
	}
}