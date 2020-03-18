#define _CRT_NON_CONFORMING_SWPRINTFS
#include "Camera.h"
#include "../../Settings/Global.h"
#include <math.h>
using namespace DirectX::SimpleMath;

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Init()
{
}

void Camera::UpdateLocation()
{
	XMFLOAT3& playerRot = Global::GetPlayerRot();
	XMFLOAT3 rot =
	{
		XMConvertToRadians(playerRot.x),
		XMConvertToRadians(playerRot.y),
		XMConvertToRadians(playerRot.z)
	};
	XMMATRIX rotationMat = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);

	XMFLOAT3 atFloat = { 0.0f, 0.0f, 10.0f };
	XMFLOAT3 upFloat = { 0.0f ,1.0f, 0.0f };

	XMVECTOR eye = XMLoadFloat3(&Global::GetPlayerPos());
	XMVECTOR at = XMVectorAdd(eye, XMVector3TransformCoord(XMLoadFloat3(&atFloat), rotationMat));
	XMVECTOR up = XMVector3TransformCoord(XMLoadFloat3(&upFloat), rotationMat);

	Global::SetViewMat(XMMatrixLookAtLH(eye, at, up));
}