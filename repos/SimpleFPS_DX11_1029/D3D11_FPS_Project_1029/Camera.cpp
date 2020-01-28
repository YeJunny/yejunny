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

void Camera::UpdateLocation(const XMFLOAT3& playerPos, const XMFLOAT3& playerRot)
{
	float rotX = XMConvertToRadians(playerRot.x);
	float rotY = XMConvertToRadians(playerRot.y);
	float rotZ = XMConvertToRadians(playerRot.z);

	XMMATRIX rotationMat = XMMatrixRotationRollPitchYaw(rotX, rotY, rotZ);
	XMFLOAT3 atFloat = { 0.0f, 0.0f, 10.0f };
	XMFLOAT3 upFloat = { 0.0f ,1.0f, 0.0f };

	XMVECTOR eye = XMLoadFloat3(&playerPos);
	XMVECTOR at = XMVectorAdd(eye, XMVector3TransformCoord(XMLoadFloat3(&atFloat), rotationMat));
	XMVECTOR up = XMVector3TransformCoord(XMLoadFloat3(&upFloat), rotationMat);

	mViewMat = XMMatrixLookAtLH(eye, at, up);
}

XMMATRIX Camera::GetCameraViewMatrix() const
{
	return mViewMat;
}
