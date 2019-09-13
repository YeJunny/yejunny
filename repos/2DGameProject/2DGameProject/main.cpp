#include "GameLib/Framework.h"
#include "Image.h"
#include "Sequence.h"
#include "Stage.h"

using namespace GameLib;

Sequence* gSeq = nullptr;
int gCounter = 0;

void Framework::update()
{
	auto i = Framework::instance();

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


	if (i.isKeyTriggered('q') || i.isKeyTriggered('Q'))
	{
		delete gSeq;
		return;
	}
}