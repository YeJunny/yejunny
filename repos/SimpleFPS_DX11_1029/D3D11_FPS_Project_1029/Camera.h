#pragma once

#include "framework.h"
#include "SimpleMath.h"
#include <memory>
using namespace Microsoft::WRL;
using namespace DirectX;

class Camera
{
public:
	Camera();
	~Camera();

	void Init(const ComPtr<ID3D11Device> d3dDevice, const XMMATRIX viewMat);
	void UpdateLocation(const XMFLOAT3 pos, const XMFLOAT3 rot);
	XMMATRIX GetViewMatrix() const;

private:
	ComPtr<ID3D11Device> mD3DDevice;
	ComPtr<ID3D11DeviceContext> mD3DContext;

	XMFLOAT3 mRot;
	XMMATRIX mViewMat;
};

