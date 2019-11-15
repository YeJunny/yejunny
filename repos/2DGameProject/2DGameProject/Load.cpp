#define _CRT_SECURE_NO_WARNINGS

#include <cstring>
#include "Game.h"
#include "Image.h"
#include "Load.h"
#include <sstream>

Load::Load(const char* imageName)
	: mCount(0)
{
	mLoad = new Image(imageName);
}

Load::~Load()
{
	delete mLoad;
}

void Load::Update(Game* parent)
{
	mLoad->Draw(0, 0, 0, 0, mLoad->GetWidth(), mLoad->GetHeight());

	if (mCount == 60)
	{
		parent->SetLocation();
		parent->SetSeq(Game::ESeq::PLAY);
		return;
	}
	
	parent->SetSeq(Game::ESeq::NONE);
	mCount++;
}