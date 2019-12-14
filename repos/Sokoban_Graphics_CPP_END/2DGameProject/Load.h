#pragma once

class Game;
class Image;

class Load
{
public:
	Load(const char* imageName);
	virtual ~Load();
	void Update(Game* parent);

private:
	Image* mLoad;
	int mCount;
};