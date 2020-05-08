#pragma once

class GLManager;

class Player
{
public:
	enum EDirection
	{
		None = 0,

		// Moving
		Left = -1,
		Right = 1,
		Up = -2,
		Down = 2
	};

	enum EState
	{
		Title,
		Menu,
		Select,
		Load,
		Play,
		Clear
	};

	enum EStage
	{
		Easy,
		Normal,
		Hard,
	};

public:
	Player();
	~Player();

	void Input(int key);
	void Update(GLManager* glManager, double deletaTime);

	EDirection GetDirection() const { return mDirection; }
	EState GetState() const { return mState; }
	EStage GetStage() const { return mStage; }
	bool GetWantsDelMap() const { return mbWantsDelMap; }

	void SetState(EState state) { mState = state; }
	void SetWantsDelMap(bool bIsWantsDelMap) { mbWantsDelMap = bIsWantsDelMap; }

private:
	EDirection mDirection;
	EState mState;
	EStage mStage;

	bool mbWantsDelMap;
};

