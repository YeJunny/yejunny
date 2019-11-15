#pragma once

#include "Sequence.h"

class Load;
class Play;
class Clear;
class Menu;
class Stage;

class Game
{
public:
	enum ESeq
	{
		SELECT,
		TITLE,
		LOAD,
		PLAY,
		CLEAR,
		MENU,
		CONTINUE,

		NONE
	};
	Game(int stage);
	virtual ~Game();
	ESeq GetSeq() const;
	void SetSeq(ESeq seq);
	void SetLocation();
	void PlayGame();
	void Update(Sequence* parent);

private:
	ESeq mSeq;
	char mStageLevel;
	char* mStageLocation;
	Stage* mStage;

	Load* mLoad;
	Play* mPlay;
	Clear* mClear;
	Menu* mMenu;
};

