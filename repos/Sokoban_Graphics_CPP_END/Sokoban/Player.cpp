#include "Player.h"
#include "GLManager.h"
#include <iostream>

Player::Player()
	: mState(EState::Title)
	, mDirection()
	, mbWantsDelMap(false)
{
}

Player::~Player()
{
}

void Player::Update(GLManager* glManager, double deltaTime)
{
	static double accTime = 0.0f;

	switch (mState)
	{
	case EState::Play:
	{
		if (glManager->GetKeyDown(GLFW_KEY_TAB))
		{

			mState = EState::Menu;
		}

		break;
	}
	case EState::Title:
	{
		if (glManager->GetKeyDown(GLFW_KEY_SPACE))
		{
			mState = EState::Select;
		}

		break;
	}
	case EState::Select:
	{
		if (glManager->GetKeyDown(GLFW_KEY_1))
		{
			mStage = EStage::Easy;
			mState = EState::Load;
		}
		else if (glManager->GetKeyDown(GLFW_KEY_2))
		{
			mStage = EStage::Normal;
			mState = EState::Load;
		}
		else if (glManager->GetKeyDown(GLFW_KEY_3))
		{
			mStage = EStage::Hard;
			mState = EState::Load;
		}

		break;
	}
	case EState::Load:
	{
		if (accTime >= 30.0)
		{
			accTime = 0;
			mState = EState::Play;
		}
		else
		{
			accTime += deltaTime;
		}

		break;
	}
	case EState::Menu:
	{
		mbWantsDelMap = true;

		if (glManager->GetKeyDown(GLFW_KEY_1))
		{
			mState = EState::Load;
		}
		else if (glManager->GetKeyDown(GLFW_KEY_2))
		{
			mState = EState::Select;
		}
		else if (glManager->GetKeyDown(GLFW_KEY_3))
		{
			mState = EState::Title;
		}
		else if (glManager->GetKeyDown(GLFW_KEY_4))
		{
			mState = EState::Play;
			mbWantsDelMap = false;
		}

		break;
	}
	case EState::Clear:
	{
		if (accTime >= 50.0)
		{
			accTime = 0;
			mState = EState::Select;
			mbWantsDelMap = true;
		}
		else
		{
			accTime += deltaTime;
		}

		break;
	}
	}
}
