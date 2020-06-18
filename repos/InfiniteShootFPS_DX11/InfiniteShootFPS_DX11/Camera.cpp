#include "Camera.h"
#include "ErrorLogger.h"
#include "Engine.h"
#include "Player.h"
#include "Object.h"
#include "InputManager.h"

void Camera::Init(Engine* engine)
{
	mEngine = engine;

	ResetCamera();
}

void Camera::UpdateCamera()
{
	const XMFLOAT2 boundX = { -18.5f, 18.5f };
	const XMFLOAT2 boundZ = { -9.0f, 17.0f };


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

	// Detect wall
	XMFLOAT3 camPosition;
	XMStoreFloat3(&camPosition, mCamPosition);

	if (camPosition.x <= boundX.x)
	{
		camPosition.x = boundX.x;
	}
	else if (camPosition.x >= boundX.y)
	{
		camPosition.x = boundX.y;
	}
	if (camPosition.z <= boundZ.x)
	{
		camPosition.z = boundZ.x;
	}
	else if (camPosition.z >= boundZ.y)
	{
		camPosition.z = boundZ.y;
	}

	mCamPosition = XMVectorSet(camPosition.x, camPosition.y, camPosition.z, 0.0f);


	mMoveLeftRight = 0.0f;
	mMoveBackForward = 0.0f;

	mCamTarget = mCamPosition + mCamTarget;

	mCamView = XMMatrixLookAtLH(mCamPosition, mCamTarget, mCamUp);
}

void Camera::CleanUp()
{
	
}

void Camera::ResetCamera()
{
	XMFLOAT3 camPos = mEngine->GetPlayer()->GetSpawnPos();

	mCamYaw = 0.0f;
	mCamPitch = 0.0f;

	mCamPosition = XMVectorSet(camPos.x, camPos.y, camPos.z, 0.0f);
	mCamTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	mCamUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	mCamView = XMMatrixLookAtLH(mCamPosition, mCamTarget, mCamUp);
	mCamProjection = XMMatrixPerspectiveFovLH(0.4f * XM_PI, 800 / 600.0f, 0.1f, 1000.0f);
}

void Camera::Update(double deltaTime)
{
	InputManager* inputManager = mEngine->GetInputManager();

	float dist = 15.0f * deltaTime;

	if (inputManager->IsPressKey(DIK_A))
	{
		mMoveLeftRight -= dist;
	}

	if (inputManager->IsPressKey(DIK_D))
	{
		mMoveLeftRight += dist;
	}

	if (inputManager->IsPressKey(DIK_W))
	{
		mMoveBackForward += dist;
	}

	if (inputManager->IsPressKey(DIK_S))
	{
		mMoveBackForward -= dist;
	}

	if (inputManager->IsMouseClick(0))
	{
		mEngine->GetPlayer()->Shoot();
	}

	if (inputManager->IsChangedMouseCoord())
	{
		DIMOUSESTATE mouseCurrState = inputManager->GetMouserCurrState();

		mCamYaw += (mouseCurrState.lX * 0.001f);
		mCamPitch += (mouseCurrState.lY * 0.001f);
		inputManager->UpdateLastMouseState();
	}

	UpdateCamera();
}
