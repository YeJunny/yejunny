#include "Light.h"

Light::Light()
{
	ZeroMemory(this, sizeof(Light));
	
	mDir = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	mAmbient = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDiffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
}
