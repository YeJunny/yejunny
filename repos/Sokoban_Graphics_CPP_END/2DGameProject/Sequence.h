#pragma once

class Title;
class SelectStage;
class Game;

class Sequence
{
public:
	enum ESeq
	{
		GAME,
		SELECT,
		TITLE,

		NONE
	};
	Sequence();
	virtual ~Sequence();
	ESeq GetSeq() const;
	void SetSeq(ESeq seq);
	void SetStage(int level);
	int GetStage();
	void Update();

private:
	ESeq mSeq;
	int mStage;

	Title* mTitle;
	SelectStage* mSelectStage;
	Game* mGame;
};

