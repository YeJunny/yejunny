#pragma once

#include "Object.h"

class Monster : public Object
{
public:
	bool Init(const std::string name, const std::string type, const std::string modelPath, const std::vector<std::string> const& texturesPath, const std::string shaderPath, const int numMonster, Engine* engine);
	virtual void Update(double deltaTime) override;
	virtual void CleanUp() override;
	virtual void Draw() override;
	float Picked(const int numMon, const DirectX::XMVECTOR& pos, const DirectX::XMVECTOR& dir);
	bool IsContainPointInTriangle(const DirectX::XMVECTOR& triV1, const DirectX::XMVECTOR& triV2, const DirectX::XMVECTOR& triV3, const DirectX::XMVECTOR& point);
	void SubHP(int index, int diff) { mMonHP[index] -= diff; }
	void SpawnMonsters();

public:
	bool IsDeadMonster(int index) { return !mMonHP[index]; }
	int GetNumMonster() const { return mNumMonster; }
	int GetMonHP(int index) const { return mMonHP[index]; }
	float GetPlayerDist(int index) const { return mPlayerDist[index]; }
	float GetAttackTimeAnim() const { return mAttackTimeAnim; }

private:
	void SpawnMonster(int index);

private:
	float						mAttackTimeAnim;

	int							mMaxHP;
	DirectX::XMFLOAT3			mColliOffset;

	int							mNumMonster;
	int*						mMonHP;
	float*						mPlayerDist;
	DirectX::XMVECTOR*			mPos;
	DirectX::XMMATRIX*			mWorldMon;
	DirectX::XMMATRIX*			mWorldHP;
	DirectX::XMMATRIX*			mWorldColli;
	DirectX::XMVECTOR*			mMonRight;
	DirectX::XMVECTOR*			mMonForward;
	ID3D11ShaderResourceView*	mTexHP;
};

