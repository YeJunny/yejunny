#include "Game.h"
#include "Sequence.h"
#include "SelectStage.h"
#include "Title.h"

Sequence::Sequence()
	: mSeq(ESeq::TITLE)
	, mSelectStage(nullptr)
	, mGame(nullptr)
	, mTitle(nullptr)
{
}

Sequence::~Sequence()
{
	delete mSelectStage;
	delete mGame;
	delete mTitle;
}

Sequence::ESeq Sequence::GetSeq() const
{
	return mSeq;
}

void Sequence::SetSeq(Sequence::ESeq seq)
{
	mSeq = seq;
}

void Sequence::SetStage(int stage)
{
	mStage = stage;
}

int Sequence::GetStage()
{
	return mStage;
}

void Sequence::Update()
{
	if (mSelectStage)
	{
		mSelectStage->Update(this);
	}
	else if (mGame)
	{
		mGame->Update(this);
	}
	else if (mTitle)
	{
		mTitle->Update(this);
	}

	switch (mSeq)
	{
	case ESeq::SELECT:
	{
		delete mTitle;
		delete mGame;
		mTitle = nullptr;
		mGame = nullptr;
		mSelectStage = new SelectStage("./data/image/select.dds");
		break;
	}
	case ESeq::GAME:
	{
		delete mTitle;
		delete mSelectStage;
		mTitle = nullptr;
		mSelectStage = nullptr;
		mGame = new Game(mStage);
		break;
	}
	case ESeq::TITLE:
	{
		delete mSelectStage;
		delete mGame;
		mSelectStage = nullptr;
		mGame = nullptr;
		mTitle = new Title("./data/image/title.dds");
		break;
	}
	}

	mSeq = ESeq::NONE;
}