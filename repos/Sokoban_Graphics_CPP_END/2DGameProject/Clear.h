#pragma once

class Game;
class Image;

class Clear
{
public:
	Clear(const char* imageName);
	~Clear();
	void Update(Game* parent);

private:
	Image* mClear;
	int mCount;
};

