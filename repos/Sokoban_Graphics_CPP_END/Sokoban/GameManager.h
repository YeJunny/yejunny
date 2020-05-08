#pragma once

#include "GLManager.h"
#include "Player.h"
#include "Map.h"

class GameManager
{
public:
	GameManager(GameManager&) = delete;
	GameManager(int width, int height, const char winName[]);
	~GameManager();

	void Init(GLFWkeyfun keyFun) const;
	void Input();
	void Update();
	void Render();
	bool IsQuit() const;

	void SetErrorCallBackGLFW(GLFWerrorfun fun);

	GLManager* GetGLManaer() const { return mGLManager; }

private:
	GLManager* mGLManager;
	Player* mPlayer;
	Map* mMap;
};

