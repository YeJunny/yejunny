#define _CRT_NON_CONFORMING_SWPRINTFS
#include "Camera.h"
#include <math.h>
using namespace DirectX::SimpleMath;

Camera::Camera()
	: mRot()
	, mView()
{
}

bool Camera::Init(const ComPtr<ID3D11Device> pD3DDevice, const XMMATRIX view)
{
	mpD3DDevice = pD3DDevice;
	mpD3DDevice->GetImmediateContext(&mpD3DContext);

	mView = view;

	return true;
}

void Camera::UpdateLocation(const XMFLOAT3 pos, const XMFLOAT3 rot)
{
	float rotX = XMConvertToRadians(rot.x);
	float rotY = XMConvertToRadians(rot.y);
	float rotZ = XMConvertToRadians(rot.z);

	XMMATRIX matRotation = XMMatrixRotationRollPitchYaw(rotX, rotY, rotZ);
	XMFLOAT3 atFloat = { 0.0f, 0.0f, 1.0f };
	XMFLOAT3 upFloat = { 0.0f ,1.0f, 0.0f };

	XMVECTOR eye = XMLoadFloat3(&pos);
	XMVECTOR at = XMVector3TransformCoord(XMLoadFloat3(&atFloat), matRotation);
	XMVECTOR up = XMVector3TransformCoord(XMLoadFloat3(&upFloat), matRotation);
	at = XMVectorAdd(eye, at);

	mView = XMMatrixLookAtLH(eye, at, up);
}

void Camera::Release()
{
}

XMMATRIX Camera::GetViewMatrix() const
{
	return mView;
}
