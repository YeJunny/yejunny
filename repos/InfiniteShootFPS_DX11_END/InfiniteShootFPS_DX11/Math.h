#pragma once

#include <DirectXMath.h>

class Math
{
public:
	static DirectX::XMVECTOR LookRotationQuaternion(DirectX::XMVECTOR forward, DirectX::XMVECTOR up);
};

