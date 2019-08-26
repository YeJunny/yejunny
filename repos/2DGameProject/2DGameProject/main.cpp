#include "GameLib/Framework.h"
#include "Stage.h"

using namespace std;
using namespace GameLib;

void GameLoop();

void Framework::update()
{
	GameLoop();
}

Stage stage("stageData.txt", "stageImage.dds");

void GameLoop()
{
	int move = stage.Input();
	stage.Update(move);
	stage.Draw();

	bool bIsClear = stage.IsClear();
	if (bIsClear == true)
	{
		return;
	}
}
