#include "File.h"
#include "GameLib/Framework.h"
#include "Image.h"
#include "Vector2.h"

using namespace GameLib;

static int powerOfTwo(int a) 
{
	int r = 1;
	while (r < a) 
	{
		r *= 2;
	}

	return r;
}

Image::Image(const char* fileName)
	: mTexture(nullptr)
{
	File* file = new File(fileName);
	char* str = file->GetReadingFile();
	mHeight = file->GetUnsignedInt(&str[12]);
	mWidth = file->GetUnsignedInt(&str[16]);
	unsigned int* buffer = new unsigned int[mWidth * mHeight];
	for (int i = 0; i < mWidth * mHeight; ++i)
	{
		buffer[i] = file->GetUnsignedInt(&str[128 + i * 4]);
	}

	mTextureWidth = powerOfTwo(mWidth);
	mTextureHeight = powerOfTwo(mHeight);

	Framework::instance().createTexture(&mTexture, mTextureWidth, mTextureHeight, buffer, mWidth, mHeight);

	delete file;
	delete buffer;
}

Image::~Image()
{
	Framework::instance().destroyTexture(&mTexture);
}

int Image::GetWidth() const
{
	return mWidth;
}

int Image::GetHeight() const
{
	return mHeight;
}

GameLib::Texture* Image::GetData() const
{
	return mTexture;
}

void Image::Draw(double x, double y, int iX, int iY, int eX, int eY) const
{
	double imageWidth = static_cast<double>(eX - iX);
	double imageHeight = static_cast<double>(eY - iY);

	// 정점 구하기
	double x0 = x;
	double y0 = y;
	double x1 = x0 + imageWidth;
	double y1 = y0 + imageHeight;

	Vector2 p0(x0, y0);
	Vector2 p1(x1, y0);
	Vector2 p2(x0, y1);
	Vector2 p3(x1, y1);


	// UV 구하기
	double reciprocalTW = 1.0 / mTextureWidth;
	double reciprocalTH = 1.0 / mTextureHeight;
	
	double u0 = static_cast<double>(iX) * reciprocalTW;
	double u1 = static_cast<double>(iX + imageWidth) * reciprocalTW;
	double v0 = static_cast<double>(iY) * reciprocalTH;
	double v1 = static_cast<double>(iY + imageHeight) * reciprocalTH;

	Vector2 t0(u0, v0);
	Vector2 t1(u1, v0);
	Vector2 t2(u0, v1);
	Vector2 t3(u1, v1);


	// 그리기
	Framework f = Framework::instance();
	f.setTexture(mTexture);
	f.setBlendMode(Framework::BLEND_LINEAR);
	f.drawTriangle2D(&p0.X, &p1.X, &p2.X, &t0.X, &t1.X, &t2.X);
	f.drawTriangle2D(&p3.X, &p1.X, &p2.X, &t3.X, &t1.X, &t2.X);
}
