#pragma once

#include "../../Settings/Include/Framework.h"
#include <fbxsdk.h>
using namespace DirectX;

struct PureVertexData
{
	XMFLOAT4 Pos;
	XMFLOAT2 UV;
	XMFLOAT3 Normal;
	XMFLOAT4 Weights;
	XMINT4 Indices;
};