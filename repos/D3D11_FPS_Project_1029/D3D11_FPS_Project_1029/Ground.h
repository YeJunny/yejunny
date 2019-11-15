#pragma once

#include "Object.h"

class Ground : public Object
{
public:
	virtual void InitIndexBuffer();
	virtual void Render();
	virtual void Release();

	Ground();

private:
	int mRow;
	int mCol;
	ID3D11Buffer* mpIndexBuffer;
	UINT mIndexCount;
};

