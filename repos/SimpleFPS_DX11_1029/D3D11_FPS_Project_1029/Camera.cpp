#define _CRT_NON_CONFORMING_SWPRINTFS
#include "Camera.h"
#include <math.h>
using namespace DirectX::SimpleMath;

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Init(const ComPtr<ID3D11Device> d3dDevice)
{
	mD3DDevice = d3dDevice;
	mD3DDevice->GetImmediateContext(&mD3DContext);
}

void Camera::UpdateLocation(const XMFLOAT3 pos, const XMFLOAT3 rot)
{
	float rotX = XMConvertToRadians(rot.x);
	float rotY = XMConvertToRadians(rot.y);
	float rotZ = XMConvertToRadians(rot.z);

	XMMATRIX rotationMat = XMMatrixRotationRollPitchYaw(rotX, rotY, rotZ);
	XMFLOAT3 atFloat = { 0.0f, 0.0f, 1.0f };
	XMFLOAT3 upFloat = { 0.0f ,1.0f, 0.0f };

	XMVECTOR eye = XMLoadFloat3(&pos);
	XMVECTOR at = XMVector3TransformCoord(XMLoadFloat3(&atFloat), rotationMat);
	XMVECTOR up = XMVector3TransformCoord(XMLoadFloat3(&upFloat), rotationMat);
	at = XMVectorAdd(eye, at);

	mViewMat = XMMatrixLookAtLH(eye, at, up);
}

XMMATRIX Camera::GetCameraViewMatrix() const
{
	return mViewMat;
}
