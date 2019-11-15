#include "Clear.h"
#include "Game.h"
#include "Image.h"

Clear::Clear(const char* imageName)
	: mCount(0)
{
	mClear = new Image(imageName);
}

Clear::~Clear()
{
	delete mClear;
}

void Clear::Update(Game* parent)
{
	if (mCount == 60)
	{
		parent->SetSeq(Game::ESeq::SELECT);
		return;
	}

	parent->PlayGame();
	mClear->Draw(0, 0, 0, 0, mClear->GetWidth(), mClear->GetHeight());
	parent->SetSeq(Game::ESeq::NONE);

	mCount++;
}