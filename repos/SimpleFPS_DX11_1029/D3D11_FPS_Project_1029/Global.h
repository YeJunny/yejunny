#pragma once

#include "framework.h"

#define BULLET_COUNT (20)
#define TEXTURE_LEN (512)
#define Assert(x) assert(x)

class Global
{
public:
	static HRESULT CompileShaderFromFile(
		const WCHAR* szFileName, const LPCSTR szEntryPoint, 
		const LPCSTR szShaderModel, ID3DBlob** ppBlobout);
};
