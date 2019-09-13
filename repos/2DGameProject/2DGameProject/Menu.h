#pragma once

class Game;
class Image;

class Menu
{
public:
	Menu(const char* imageName, Game* parent);
	virtual ~Menu();
	void Update(Game* parent);

private:
	Image* mMenu;
};

