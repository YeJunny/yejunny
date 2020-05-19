#pragma once

#include <DirectXMath.h>
#include <vector>

struct Vertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT2 UV;
	DirectX::XMFLOAT3 Normal;
	// Animation infomation
	DirectX::XMFLOAT4 Weights;
	DirectX::XMINT4 Indices;
};

class Model
{
	
};