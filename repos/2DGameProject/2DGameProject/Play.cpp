#include "Game.h"
#include "GameLib/Framework.h"
#include "GameLib/Input/Keyboard.h"
#include "GameLib/Input/Manager.h"
#include "Play.h"

using namespace GameLib;

void Play::Update(Game* parent)
{
	parent->PlayGame();

	if (Input::Manager::instance().keyboard().isTriggered(' '))
	{
		parent->SetSeq(Game::ESeq::MENU);
		return;
	}

	if (parent->GetSeq() == Game::ESeq::CLEAR)
	{
		return;
	}

	parent->SetSeq(Game::ESeq::NONE);
}