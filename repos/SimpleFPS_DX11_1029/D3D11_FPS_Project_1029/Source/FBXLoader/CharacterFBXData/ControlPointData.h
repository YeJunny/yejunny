#pragma once

#include "../../Settings/Include/Framework.h"
#include <fbxsdk.h>
#include <vector>
using namespace DirectX;

struct BlendControlPointData
{
	float BlendWeight;
	int BlendIndex;
};

class ControlPointData
{
public:
	ControlPointData();
	XMFLOAT3 GetPosition() const;
	std::vector<BlendControlPointData>* GetBlendData();
	void SetPosition(FbxVector4* vectice);
	//void SetBlendData(BlendControlPointData* blendData);

private:
	XMFLOAT3 mPosition;
	std::vector<BlendControlPointData> mBlendData;
};