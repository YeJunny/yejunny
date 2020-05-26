#pragma once

#include <DirectXMath.h>
#include <Windows.h>

class Light
{
public:
	Light();

private:
	DirectX::XMFLOAT3 mDir;
	float pad;
	DirectX::XMFLOAT4 mAmbient;
	DirectX::XMFLOAT4 mDiffuse;
};

