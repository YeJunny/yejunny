#pragma once


namespace GameLib 
{
	class Texture;
}
class Image
{
public:
	Image(const char* fileName);
	~Image();
	int GetWidth() const;
	int GetHeight() const;
	GameLib::Texture* GetData() const;
	void Draw(double x, double y, int iX, int iY, int eX, int eY) const;

private:
	int mWidth;
	int mHeight;
	int mTextureWidth;
	int mTextureHeight;
	GameLib::Texture* mTexture;
};