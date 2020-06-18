#pragma once

#pragma comment(lib, "dinput8.lib")

#include <DirectXMath.h>
#include <dinput.h>

using namespace DirectX;

class Engine;

class InputManager
{
public:
	void Init(HINSTANCE hInstance, HWND hwnd, Engine* engine);
	void Update();
	void CleanUp();
	void UpdateLastMouseState();

public:
	int IsPressKey(int key);
	int IsMouseClick(int button);
	int IsChangedMouseCoord();
	DIMOUSESTATE GetMouserCurrState() const { return mMouseCurrState; }

private:
	IDirectInputDevice8*		mDIKeyboard = nullptr;
	IDirectInputDevice8*		mDIMouse = nullptr;
	DIMOUSESTATE				mMouseLastState;
	LPDIRECTINPUT8				mDirectInput = nullptr;
	DIMOUSESTATE				mMouseCurrState;
	BYTE						mKeyboardState[256];

	// Core
	Engine*						mEngine;
};

