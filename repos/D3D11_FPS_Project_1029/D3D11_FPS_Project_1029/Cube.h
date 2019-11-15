#pragma once

#include "Object.h"

class Cube : public Object
{
public:
	virtual void InitIndexBuffer();
	virtual void Update(const XMMATRIX view);
	virtual void Render();
	virtual void Release();

	Cube();

private:
	ID3D11Buffer* mpIndexBuffer;
	UINT mIndexCount;
};

