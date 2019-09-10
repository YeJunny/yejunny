#include "GameLib/Framework.h"
#include "Stage.h"

using namespace std;
using namespace GameLib;

void GameLoop();

void Framework::update()
{
	GameLoop();
}

Stage gStage("stageData.txt", "stageImage.dds");
unsigned int gPreTime[10];
const int gFrameTime = 16;

void GameLoop()
{
	auto f = Framework::instance();
	while (f.time() - gPreTime[9] < gFrameTime)
	{
		f.sleep(1);
	}

	unsigned int currentTime = f.time();
	unsigned int frameTime10 = currentTime - gPreTime[0];
	cout << "frameTime10 : " << frameTime10 / 10;
	cout << " FrameRate: " << 10 * 1000 / frameTime10 << endl;

	for (int i = 0; i < 9; ++i)
	{
		gPreTime[i] = gPreTime[i + 1];
	}
	gPreTime[9] = currentTime;

	int move = gStage.Input();
	gStage.Update(move);
	gStage.Draw();

	bool bIsClear = gStage.IsClear();
	if (bIsClear == true)
	{
		return;
	}
}
