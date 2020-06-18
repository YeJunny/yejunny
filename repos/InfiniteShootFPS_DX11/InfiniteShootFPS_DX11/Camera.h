#pragma once

#include <DirectXMath.h>
#include <d3d11.h>
#include <string>
#include <vector>
using namespace DirectX;

class Engine;
class Monster;
class MonsterInfo;
class Object;

class Camera
{
public:
	Camera() = default;
	Camera(Camera& camera) = delete;
	void Init(Engine* engine);
	void Update(double deltaTime);
	void CleanUp();
	void ResetCamera();

private:
	void UpdateCamera();

public:
	float GetCamYaw() const { return mCamYaw; }
	float GetCamPitch() const { return mCamPitch; }
	XMVECTOR const& GetCamRight() { return mCamRight; }
	XMVECTOR const& GetCamForward() { return mCamForward; }
	XMVECTOR const& GetCamPos() const { return mCamPosition; }
	XMVECTOR const& GetCamUp() const { return mCamUp; }
	XMMATRIX const& GetViewMat() const { return mCamView; }
	XMMATRIX const& GetProjMat() const { return mCamProjection; }

private:
	XMVECTOR					mCamForward					= XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR					mCamRight					= XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR					mCamPosition;
	XMMATRIX					mCamView;
	XMMATRIX					mCamProjection;
	XMVECTOR					mCamTarget;
	XMVECTOR					mCamUp;

	XMMATRIX					mCamRotationMat;
	XMMATRIX					mGroundMat;

	float						mMoveLeftRight				= 0.0f;
	float						mMoveBackForward			= 0.0f;

	float						mCamYaw						= 0.0f;
	float						mCamPitch					= 0.0f;

	// Core
	Engine*						mEngine;
};

