#pragma once

#include "framework.h"
#include "SimpleMath.h"
#include <memory>
using namespace Microsoft::WRL;
using namespace DirectX;

class Camera
{
public:
	bool Init(const ComPtr<ID3D11Device> pD3DDevice, const XMMATRIX view);
	void UpdateLocation(const XMFLOAT3 pos, const XMFLOAT3 rot);
	void Release();

	Camera();

	XMMATRIX GetViewMatrix();

private:
	ComPtr<ID3D11Device> mpD3DDevice;
	ComPtr<ID3D11DeviceContext> mpD3DContext;

	XMFLOAT3 mRot;
	XMMATRIX mView;
};

