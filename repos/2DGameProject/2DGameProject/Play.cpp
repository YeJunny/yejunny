#include "Game.h"
#include "GameLib/Framework.h"
#include "Play.h"

using namespace GameLib;

void Play::Update(Game* parent)
{
	parent->PlayGame();

	if (Framework::instance().isKeyTriggered(' '))
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