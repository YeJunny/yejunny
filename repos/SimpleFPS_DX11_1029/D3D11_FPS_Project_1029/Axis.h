#pragma once

#include "Object.h"

class Axis : public Object
{
public:
	void Init(const ComPtr<ID3D11Device> d3dDevice,
		const WCHAR* shaderFile, const XMMATRIX& projectionMat, std::shared_ptr<Timer> timer);
	void Render();

private:

};

