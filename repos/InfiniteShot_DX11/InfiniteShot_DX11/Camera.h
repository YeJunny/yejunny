#pragma once

#pragma comment(lib, "dinput8.lib")

#include <DirectXMath.h>
#include <dinput.h>
#include <d3d11.h>
using namespace DirectX;

class Camera
{
public:
	Camera() = delete;
	Camera(Camera& camera) = delete;
	Camera(HINSTANCE hInstance, HWND hwnd);
	void DetectInput(double deltaTime);
	void CleanUp();

private:
	void UpdateCamera();

public:
	XMMATRIX const& GetViewMat() const { return mCamView; }
	XMMATRIX const& GetProjMat() const { return mCamProjection; }

private:
	const XMVECTOR DEFAULT_FORWARD = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DEFAULT_RIGHT = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	// Input Hardware
	IDirectInputDevice8*	mDIKeyboard = nullptr;
	IDirectInputDevice8*	mDIMouse = nullptr;
	DIMOUSESTATE			mMouseLastState;
	LPDIRECTINPUT8			mDirectInput = nullptr;

	XMVECTOR mCamForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR mCamRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR mCamPosition;
	XMMATRIX mCamView;
	XMMATRIX mCamProjection;
	XMVECTOR mCamTarget;
	XMVECTOR mCamUp;

	XMMATRIX mCamRotationMat;
	XMMATRIX mGroundMat;

	float mMoveLeftRight = 0.0f;
	float mMoveBackForward = 0.0f;

	float mCamYaw = 0.0f;
	float mCamPitch = 0.0f;

	
};

