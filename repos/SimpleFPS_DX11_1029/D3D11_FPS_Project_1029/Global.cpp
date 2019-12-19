#include "Global.h"

HRESULT Global::CompileShaderFromFile(const WCHAR* szFileName, const LPCSTR szEntryPoint, const LPCSTR szShaderModel, ID3DBlob** ppBlobout)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined _DEBUG
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DCompileFromFile(szFileName, NULL, NULL,
		szEntryPoint, szShaderModel, dwShaderFlags,
		0, ppBlobout, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
		{
			char* errorString = (char*)pErrorBlob->GetBufferPointer();
			OutputDebugStringA("===========Shader Error============\n");
			OutputDebugString(szFileName);
			OutputDebugStringA("\n=====================================\n");
			OutputDebugStringA(errorString);
			OutputDebugStringA("=====================================\n");
		}
		if (pErrorBlob) pErrorBlob->Release();
		assert(false);
		return hr;
	}

	if (pErrorBlob)
	{
		pErrorBlob->Release();
	}

	return S_OK;
}