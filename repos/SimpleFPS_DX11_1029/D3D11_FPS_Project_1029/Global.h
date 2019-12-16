#pragma once

#include "framework.h"

#define BULLET_COUNT 20

class Global
{
public:
	static HRESULT CompileShaderFromFile(
		const WCHAR* szFileName, const LPCSTR szEntryPoint, 
		const LPCSTR szShaderModel, ID3DBlob** ppBlobout);
};

