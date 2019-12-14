#include "GameLib/Framework.h"
#include "GameLib/Input/Keyboard.h"
#include "GameLib/Input/Manager.h"
#include "Image.h"
#include "Sequence.h"
#include "Stage.h"

using namespace GameLib;

Sequence* gSeq = nullptr;
int gCounter = 0;

void Framework::update()
{
	auto keyboard = Input::Manager::instance().keyboard();

	if (!gSeq)
	{
		gSeq = new Sequence();
	}

	setFrameRate(60);
	if (gCounter % 60 == 0)
	{
		cout << " FrameRate:" << frameRate() << endl;
	}
	++gCounter;


	gSeq->Update();


	if (keyboard.isTriggered('q') || keyboard.isTriggered('Q'))
	{
		delete gSeq;
		return;
	}
}