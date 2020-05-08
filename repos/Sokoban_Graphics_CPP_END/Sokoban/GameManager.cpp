#include "GameManager.h"

#pragma warning(disable:4996)

GameManager::GameManager(int width, int height, const char winName[])
{
	mGLManager = new GLManager(width, height, winName);
	mPlayer = new Player();
}

GameManager::~GameManager()
{
	delete mGLManager;
}

void GameManager::Init(GLFWkeyfun keyFun) const
{
	mGLManager->Init(keyFun);
}

void GameManager::Input()
{
}

void GameManager::Update()
{
	double deltaTime = mGLManager->GetDeltaTime();

	mPlayer->Update(mGLManager, deltaTime);

	Player::EState playerState = mPlayer->GetState();

	if (playerState == Player::EState::Play)
	{
		if (!mMap)
		{
			Player::EStage stage = mPlayer->GetStage();
			char* fileName = new char[64];
			strcpy(fileName, "GameData\\MapData\\");

			switch (stage)
			{
			case Player::EStage::Easy:
			{
				strcat(fileName, "1.txt");
				
				break;
			}
			case Player::EStage::Normal:
			{
				strcat(fileName, "2.txt");

				break;
			}
			case Player::EStage::Hard:
			{
				strcat(fileName, "3.txt");

				break;
			}
			}
			
			mMap = new Map(fileName);
			delete[] fileName;
		}
		else
		{
			mMap->Update(mGLManager);
			mMap->UpdateMap();

			if (mMap->IsClear())
			{
				mMap->Update(mGLManager);
				mMap->UpdateMap();
				mPlayer->SetState(Player::EState::Clear);
			}
		}
	}
	else if (mMap && mPlayer->GetWantsDelMap() && playerState != Player::EState::Menu)
	{
		mPlayer->SetWantsDelMap(false);
		delete mMap;
		mMap = NULL;
	}

	mGLManager->Update();
}

void GameManager::Render()
{
	Player::EState playerState = mPlayer->GetState();

	switch (playerState)
	{
	case Player::EState::Play:
	case Player::EState::Menu:
	case Player::EState::Clear:
	{
		mGLManager->RenderPlay(
			mMap->GetMap(), mMap->GetWidth(), mMap->GetHeight(), playerState);

		//mMap->RenderConsole();

		break;
	}
	case Player::EState::Select:
	{
		mGLManager->RenderOnlyBackground(GLManager::ETexture::Select);

		break;
	}
	case Player::EState::Load:
	{
		mGLManager->RenderOnlyBackground(GLManager::ETexture::Load);

		break;
	}
	case Player::EState::Title:
	{
		mGLManager->RenderOnlyBackground(GLManager::ETexture::Title);
		
		break;
	}
	}
}

bool GameManager::IsQuit() const
{
	return mGLManager->IsQuit();
}

void GameManager::SetErrorCallBackGLFW(GLFWerrorfun fun)
{
	mGLManager->SetErrorCallBackGLFW(fun);
}

