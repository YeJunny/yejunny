#pragma once

#include "Engine.h"

using namespace DirectX;

class Monster;

struct CBAlpha
{
	float Alpha;
};

class Player
{
public:
	void Init(Engine* engine);
	void Update(double deltaTime);
	void Draw();
	void CleanUp();
	void UpdateDeadImage(double deltaTime);
	void DrawDeadImage() const;
	void Shoot();
	void ChangeWeapon();

private:
	void ConvertMouseToRay(int mouseX, int mouseY, XMVECTOR* pos, XMVECTOR* dir);
	void DrawAim() const;
	void DrawDamageImage() const;
	void DrawMuzzleflash();
	void InitWeapon();
	void InitDamageImage(std::string damageImagePath);
	void InitDeadImage(std::string deadImagePath);

public:
	std::string const& GetWeaponName() const { return mWeaponName; }
	int GetScore() const { return mScore; }
	int GetHP() const { return mHP; }
	XMFLOAT3 const& GetSpawnPos() const { return mSpawnPos; }
	int GetPickedDist() const { return mPickedDist; }
	float GetAlpha() const { return mAlpha; }
	bool IsDead() const { return mbIsDead; }

private:
	const float					BOUND_TIME					= 0.1f;

	int							mScore						= 0;
	int							mHP							= 50;
	bool						mbIsDead					= false;
	XMFLOAT3					mSpawnPos;

	// Muzzle flash
	ID3D11ShaderResourceView*	mTexMuzzleflash;
	ID3D11Buffer*				mCBMuzzleflash;
	DirectX::XMMATRIX			mWorldMuzzleflash;
	XMFLOAT3					mMuzzleflashPos				= { 0.3f, 0.6f, 0.4f };
	float						mMuzzleflashScale			= 0.015f;

	// Attacked
	ID3D11ShaderResourceView*	mBloodTex;
	ID3D11Buffer*				mAlphaCB;
	bool						mbIsAttacked				= false;
	float						mAlpha						= 1.0f;

	// Attack
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
	XMFLOAT3					mWeaponPos					= { 0.3f, 0.6f, 0.4f };
	float						mWeaponScale				= 0.015f;
	std::string					mWeaponName;

	// Game over
	ID3D11ShaderResourceView*	mGameoverTex;

	// Core
	Engine*						mEngine;
};

