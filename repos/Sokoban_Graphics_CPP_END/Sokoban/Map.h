#pragma once

class GLManager;

class Map
{
public:
	enum EObject
	{
		// Original
		Wall,
		Target,
		Background,

		// Can changed
		Box, PlayerFrom, PlayerTo, PlayerInPlace, Filled, Other
	};

	Map(const char* mapFileName);
	~Map();

	void Update(GLManager* glManager);
	void UpdateMap();
	void RenderConsole() const;
	bool IsClear() const;

	EObject* GetMap() const { return mObjectMap; }
	int GetPrevPlayerIndex() const { return mPrevPlayerIndex; }
	int GetPlayerIndex() const { return mPlayerIndex; }
	int* GetPrevBoxIndex() const { return mPrevBox; }
	int* GetBoxIndex() const { return mBox; }
	int GetWidth() const { return mWidth; }
	int GetHeight() const { return mHeight; }

private:
	// Map Data
	int mWidth;
	int mHeight;
	int mBoxCount;
	EObject* mOriginalMap;
	EObject* mObjectMap;

	// Player Data
	int mPlayerIndex;
	int mPrevPlayerIndex;

	// Temp
	int* mBox;
	int* mPrevBox;
	int* mFilled;

	// Render Data
	char* mRenderConsoleArray;

	double mNumO;
	double mNumP;

	//Image* mImage;
	bool mbP;
	bool mbO;
};

