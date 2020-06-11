#pragma once

#include "Engine.h"

using namespace DirectX;

class Monster;

class Player
{
public:
	void Shoot();
	void ChangeWeapon();
	void Init(Engine* engine);
	void Update(double deltaTime);
	void Draw();
	void CleanUp();

private:
	void ConvertMouseToRay(int mouseX, int mouseY, XMVECTOR* pos, XMVECTOR* dir);
	void InitMonsterVector();
	void DrawAim() const;
	void DrawMuzzleFlash();
	void InitWeapon();

public:
	std::wstring const& GetWeaponName() const { return mWeaponName; }
	int GetScore() const { return mScore; }
	int GetHP() const { return mHP; }
	XMFLOAT3 const& GetPos() const { return mPos; }
	int GetPickedDist() const { return mPickedDist; }

private:
	const float					BOUND_TIME					= 0.1f;

	int							mScore						= 0;
	int							mHP							= 50;
	XMFLOAT3					mPos;

	// Muzzle Flash
	ID3D11ShaderResourceView*	mTexMuzzleFlash;
	ID3D11Buffer*				mCBMuzzleFlash;
	DirectX::XMMATRIX			mWorldMuzzleFlash;
	float						mMuzzleFlashY				= 0.6f;
	float						mMuzzleFlashX				= 0.3f;
	float						mMuzzleFlashZ				= 0.4f;
	float						mMuzzleFlashScale			= 0.015f;

	// Attack
	std::vector<Monster*>		mMonsters;
	float						mPickedDist					= 0;
	float						mAccTime					= 0;
	bool						mbIsShoot					= false;

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

	// Core
	Engine*						mEngine;
};

