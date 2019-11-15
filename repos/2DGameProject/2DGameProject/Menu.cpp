#include "Game.h"
#include "GameLib/Framework.h"
#include "GameLib/Input/Keyboard.h"
#include "GameLib/Input/Manager.h"
#include "Image.h"
#include "Menu.h"

using namespace GameLib;

Menu::Menu(const char* imageName, Game* parent)
{
	mMenu = new Image(imageName);
}

Menu::~Menu()
{
	delete mMenu;
}

void Menu::Update(Game* parent)
{
	parent->PlayGame();
	mMenu->Draw(0, 0, 0, 0, mMenu->GetWidth(), mMenu->GetHeight());

	auto keyboard = Input::Manager::instance().keyboard();
	if (keyboard.isTriggered('1'))
	{
		parent->SetSeq(Game::ESeq::PLAY);
		return;
	}
	else if (keyboard.isTriggered('2'))
	{
		parent->SetSeq(Game::ESeq::SELECT);
		return;
	}
	else if (keyboard.isTriggered('3'))
	{
		parent->SetSeq(Game::ESeq::TITLE);
		return;

	}
	else if (keyboard.isTriggered('4'))
	{
		parent->SetSeq(Game::ESeq::CONTINUE); 
		return;
	}

	parent->SetSeq(Game::ESeq::NONE);
}