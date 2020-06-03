#pragma once

#include "Object.h"

class Monster : public Object
{
public:
	bool Initalize(const int numMonster, const WCHAR shaderFileName[]);
	virtual void Update(double deltaTime) override;
	virtual void CleanUp() override;
	float Picked(int num, DirectX::XMVECTOR& pos, DirectX::XMVECTOR& dir);

public:
	int GetNumMonster() const { return mNumMonster; }
	int GetMonsterHit(int index) const { return mMonsterHit[index]; }
	void SetMonsterHit(int value, int index) { mMonsterHit[index] = value; }

private:
	int						mNumMonster;
	int*					mMonsterHit;
	DirectX::XMMATRIX*		mWorld;

};

