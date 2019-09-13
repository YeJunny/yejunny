#include "Game.h"
#include "GameLib/Framework.h"
#include "Image.h"
#include "Menu.h"

using namespace GameLib;

Menu::Menu(const char* imageName, Game* parent)
{
	mMenu = new Image(imageName);
	parent->PlayGame();
	mMenu->Draw(0, 0, 0, 0, mMenu->GetWidth(), mMenu->GetHeight());
}

Menu::~Menu()
{
	delete mMenu;
}

void Menu::Update(Game* parent)
{
	auto i = Framework::instance();

	if (i.isKeyTriggered('1'))
	{
		parent->SetSeq(Game::ESeq::PLAY);
		return;
	}
	else if (i.isKeyTriggered('2'))
	{
		parent->SetSeq(Game::ESeq::SELECT);
		return;
	}
	else if (i.isKeyTriggered('3'))
	{
		parent->SetSeq(Game::ESeq::TITLE);
		return;

	}
	else if (i.isKeyTriggered('4'))
	{
		parent->SetSeq(Game::ESeq::CONTINUE); 
		return;
	}

	parent->SetSeq(Game::ESeq::NONE);
}