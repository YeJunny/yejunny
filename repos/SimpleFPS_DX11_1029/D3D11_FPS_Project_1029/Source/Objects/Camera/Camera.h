#pragma once

#include "../../Settings/Include/Framework.h"
#include "SimpleMath.h"
using namespace Microsoft::WRL;
using namespace DirectX;

class Camera
{
public:
	Camera();
	~Camera();

	void Init();
	void UpdateLocation();
};

