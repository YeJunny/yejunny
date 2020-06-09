#pragma once

#include "Object.h"

class Monster : public Object
{
public:
	bool Initalize(const int numMonster, const WCHAR shaderFileName[]);
	virtual void Update(double deltaTime) override;
	virtual void CleanUp() override;
	virtual void Draw() override;
	float Picked(const int numMon, const DirectX::XMVECTOR& pos, const DirectX::XMVECTOR& dir);
	bool IsContainPointInTriangle(const DirectX::XMVECTOR& triV1, const DirectX::XMVECTOR& triV2, const DirectX::XMVECTOR& triV3, const DirectX::XMVECTOR& point);

public:
	int GetNumMonster() const { return mNumMonster; }
	int GetMonsterHit(int index) const { return mMonsterHit[index]; }
	void SetMonsterHit(int index, int value) { mMonsterHit[index] = value; }

private:
	int						mNumMonster;
	std::vector<int>		mMonsterHit;
	DirectX::XMMATRIX*		mWorlds;

};

