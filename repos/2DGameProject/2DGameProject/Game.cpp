#define _CRT_SECURE_NO_WARNINGS

#include "Clear.h"
#include "Game.h"
#include "Load.h"
#include "Menu.h"
#include "Play.h"
#include "Stage.h"
#include <cstring>
#include <sstream>

Game::Game(int stage)
	: mStageLevel(stage)
	, mSeq(ESeq::LOAD)
	, mStageLocation(nullptr)
	, mStage(nullptr)
	, mLoad(nullptr)
	, mPlay(nullptr)
	, mClear(nullptr)
	, mMenu(nullptr)
{
}

Game::~Game()
{
	delete mStageLocation;
	delete mStage;
	delete mLoad;
	delete mPlay;
	delete mClear;
	delete mMenu;
}

Game::ESeq Game::GetSeq() const
{
	return mSeq;
}

void Game::SetSeq(Game::ESeq seq)
{
	mSeq = seq;
}

void Game::SetLocation()
{
	std::ostringstream oss;
	oss << "./data/stageData/" << mStageLevel << ".txt";
	mStageLocation = new char[256];
	strcpy(mStageLocation, oss.str().c_str());
}

void Game::PlayGame()
{
	int move = mStage->Input();
	mStage->Update(move);
	mStage->Draw();

	if (mStage->IsClear())
	{
		mSeq = ESeq::CLEAR;
	}
}

void Game::Update(Sequence* parent)
{
	if (mLoad)
	{
		mLoad->Update(this);
	}
	else if (mPlay)
	{
		mPlay->Update(this);
	}
	else if (mClear)
	{
		mClear->Update(this);
	}
	else if (mMenu)
	{
		mMenu->Update(this);
	}

	switch (mSeq)
	{
	case ESeq::LOAD:
	{
		delete mStage;
		delete mStageLocation;
		delete mPlay;
		delete mClear;
		delete mMenu;
		mStage = nullptr;
		mPlay = nullptr;
		mClear = nullptr;
		mMenu = nullptr;
		mLoad = new Load("./data/image/loading.dds");
		break;
	}
	case ESeq::PLAY:
	{
		delete mStage;
		delete mLoad;
		delete mPlay;
		delete mClear;
		delete mMenu;
		mStage = nullptr;
		mLoad = nullptr;
		mPlay = nullptr;
		mClear = nullptr;
		mMenu = nullptr;
		mPlay = new Play();
		mStage = new Stage(mStageLocation, "./data/image/stageImage.dds");
		break;
	}
	case ESeq::CLEAR:
	{
		delete mLoad;
		delete mPlay;
		delete mMenu;
		mLoad = nullptr;
		mPlay = nullptr;
		mMenu = nullptr;
		mClear = new Clear("./data/image/clear.dds");
		break;
	}
	case ESeq::MENU:
	{
		delete mLoad;
		delete mPlay;
		delete mClear;
		mLoad = nullptr;
		mPlay = nullptr;
		mClear = nullptr;
		mMenu = new Menu("./data/image/menu.dds", this);
		break;
	}
	case ESeq::SELECT:
	{
		delete mStage;
		delete mLoad;
		delete mPlay;
		delete mMenu;
		delete mClear;
		delete mStageLocation;
		mStage = nullptr;
		mLoad = nullptr;
		mPlay = nullptr;
		mMenu = nullptr;
		mClear = nullptr;
		mStageLocation = nullptr;
		parent->SetSeq(Sequence::ESeq::SELECT);
		break;
	}
	case ESeq::TITLE:
	{
		delete mStage;
		delete mLoad;
		delete mPlay;
		delete mMenu;
		delete mClear;
		delete mStageLocation;
		mStage = nullptr;
		mLoad = nullptr;
		mPlay = nullptr;
		mMenu = nullptr;
		mClear = nullptr;
		mStageLocation = nullptr;
		parent->SetSeq(Sequence::ESeq::TITLE);
		break;
	}
	case ESeq::CONTINUE:
	{
		delete mLoad;
		delete mPlay;
		delete mClear;
		delete mMenu;
		mLoad = nullptr;
		mPlay = nullptr;
		mClear = nullptr;
		mMenu = nullptr;
		mPlay = new Play();
		break;
	}
	}
}