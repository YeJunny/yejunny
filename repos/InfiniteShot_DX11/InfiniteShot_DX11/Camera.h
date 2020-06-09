#pragma once

#pragma comment(lib, "dinput8.lib")

#include <DirectXMath.h>
#include <dinput.h>
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
	void Init(HINSTANCE hInstance, HWND hwnd, Engine* engine);
	void DetectInput(double deltaTime);
	void CleanUp();
	void DrawWeapon() const;

private:
	void UpdateCamera();
	void DrawAim() const;
	void ConvertMouseToRay(int mouseX, int mouseY, XMVECTOR* pos, XMVECTOR* dir);
	void InitWeapon();
	void InitMonster();

public:
	XMVECTOR const& GetPosMat() const { return mCamPosition; }
	XMMATRIX const& GetViewMat() const { return mCamView; }
	XMMATRIX const& GetProjMat() const { return mCamProjection; }
	XMINT3 GetLastCursorPos() const
	{ 
		XMINT3 pos = { mMouseLastState.lX, mMouseLastState.lY, mMouseLastState.lZ };
		return pos;
	}
	int GetScore() const { return mScore; }
	int GetPickedDist() const { return mPickedDist; }

private:
	const XMVECTOR				DEFAULT_FORWARD				= XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR				DEFAULT_RIGHT				= XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	int							mScore						= 0;

	// Attack
	std::vector<Monster*>		mMonsters;
	float						mPickedDist					= 0;

	// Aim
	ID3D11ShaderResourceView*	mTexAim;
	ID3D11Buffer*				mCBAim;
	DirectX::XMMATRIX			mWorldAim;

	// Weapon
	Object*						mWeapon;
	XMMATRIX					mWorldWeapon;
	float						mWeaponY					= 0.6f;
	float						mWeaponX					= 0.3f;
	float						mWeaponZ					= 0.4f;
	float						mWeaponScale				= 0.015f;
	std::wstring				mWeaponName;

	// Input Hardware
	IDirectInputDevice8*		mDIKeyboard					= nullptr;
	IDirectInputDevice8*		mDIMouse					= nullptr;
	DIMOUSESTATE				mMouseLastState;
	LPDIRECTINPUT8				mDirectInput				= nullptr;

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

