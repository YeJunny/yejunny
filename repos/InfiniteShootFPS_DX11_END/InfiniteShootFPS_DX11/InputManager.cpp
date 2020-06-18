#include "InputManager.h"
#include "ErrorLogger.h"

void InputManager::Init(HINSTANCE hInstance, HWND hwnd, Engine* engine)
{
	mEngine = engine;


	HRESULT hr = DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		reinterpret_cast<void**>(&mDirectInput),
		nullptr
	);
	AssertInitialization(hr, "Direct Input Create - Failed");

	hr = mDirectInput->CreateDevice(GUID_SysKeyboard, &mDIKeyboard, nullptr);
	AssertInitialization(hr, "Direct Input Create Device - Failed");

	hr = mDirectInput->CreateDevice(GUID_SysMouse, &mDIMouse, nullptr);
	AssertInitialization(hr, "Direct Input Create Device - Failed");

	hr = mDIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	AssertInitialization(hr, "Direct Input Set Data Format - Failed");

	hr = mDIKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	AssertInitialization(hr, "Direct Input Set Cooperative Level - Failed");

	hr = mDIMouse->SetDataFormat(&c_dfDIMouse);
	AssertInitialization(hr, "Direct Input Set Data Format - Failed");

	hr = mDIMouse->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
	AssertInitialization(hr, "Direct Input Set Cooperative Level - Failed");
}

void InputManager::Update()
{
	ZeroMemory(&mKeyboardState, sizeof(mKeyboardState));
	ZeroMemory(&mMouseCurrState, sizeof(mMouseCurrState));

	mDIKeyboard->Acquire();
	mDIMouse->Acquire();

	mDIKeyboard->GetDeviceState(sizeof(mKeyboardState), mKeyboardState);
	mDIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mMouseCurrState);
}

void InputManager::CleanUp()
{
	mDIKeyboard->Release();
	mDIMouse->Release();
	mDirectInput->Release();
}

void InputManager::UpdateLastMouseState()
{
	mMouseLastState = mMouseCurrState;
}

int InputManager::IsPressKey(int key)
{
	return mKeyboardState[key] & 0x80;
}

int InputManager::IsMouseClick(int button)
{
	return mMouseCurrState.rgbButtons[button] & 0x80;
}

int InputManager::IsChangedMouseCoord()
{
	return mMouseCurrState.lX != mMouseLastState.lX || mMouseCurrState.lY != mMouseLastState.lY;
}
