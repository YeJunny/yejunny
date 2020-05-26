#include "Camera.h"
#include "ErrorLogger.h"

Camera::Camera(HINSTANCE hInstance, HWND hwnd)
{
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

	mCamPosition = XMVectorSet(0.0f, 0.0f, -9.0f, 0.0f);
	mCamTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	mCamUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	mCamView = XMMatrixLookAtLH(mCamPosition, mCamTarget, mCamUp);
	mCamProjection = XMMatrixPerspectiveFovLH(0.4f * XM_PI, 800 / 600.0f, 0.1f, 1000.0f);
}

void Camera::UpdateCamera()
{
	mCamRotationMat = XMMatrixRotationRollPitchYaw(mCamPitch, mCamYaw, 0);
	mCamTarget = XMVector3TransformCoord(DEFAULT_FORWARD, mCamRotationMat);
	mCamTarget = XMVector3Normalize(mCamTarget);

	XMMATRIX rotationYTempMat;
	rotationYTempMat = XMMatrixRotationY(mCamYaw);

	mCamRight = XMVector3TransformCoord(DEFAULT_RIGHT, rotationYTempMat);
	mCamUp = XMVector3TransformCoord(mCamUp, rotationYTempMat);
	mCamForward = XMVector3TransformCoord(DEFAULT_FORWARD, rotationYTempMat);

	mCamPosition += mMoveLeftRight * mCamRight;
	mCamPosition += mMoveBackForward * mCamForward;

	mMoveLeftRight = 0.0f;
	mMoveBackForward = 0.0f;

	mCamTarget = mCamPosition + mCamTarget;

	mCamView = XMMatrixLookAtLH(mCamPosition, mCamTarget, mCamUp);
}

void Camera::CleanUp()
{
	mDIKeyboard->Release();
	mDIMouse->Release();
	mDirectInput->Release();
}

void Camera::DetectInput(double deltaTime)
{
	DIMOUSESTATE mouseCurrState;
	BYTE keyboardState[256];

	mDIKeyboard->Acquire();
	mDIMouse->Acquire();

	mDIKeyboard->GetDeviceState(sizeof(keyboardState), keyboardState);
	mDIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);

	float dist = 15.0f * deltaTime;

	if (keyboardState[DIK_A] & 0x80)
	{
		mMoveLeftRight -= dist;
	}

	if (keyboardState[DIK_D] & 0x80)
	{
		mMoveLeftRight += dist;
	}

	if (keyboardState[DIK_W] & 0x80)
	{
		mMoveBackForward += dist;
	}

	if (keyboardState[DIK_S] & 0x80)
	{
		mMoveBackForward -= dist;
	}

	if (mouseCurrState.lX != mMouseLastState.lX || mouseCurrState.lY != mMouseLastState.lY)
	{
		mCamYaw += (mouseCurrState.lX * 0.001f);
		mCamPitch += (mouseCurrState.lY * 0.001f);
		mMouseLastState = mouseCurrState;
	}

	UpdateCamera();
}