#pragma once

#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include "ErrorLogger.h"
#include "Model.h"
#include "Light.h"

#ifndef null
	#define null (NULL)
#endif

class Engine;

struct CBPerObject
{
	DirectX::XMMATRIX WVP;
	DirectX::XMMATRIX World;
};

struct CBPerFrame
{
	Light Light;
};

class Object
{
public:
	void Setup(ID3D11Device* d3d11Device, ID3D11DeviceContext* d3d11DevCon, Engine* engine);
	bool Initalize(const WCHAR shaderFileName[]);
	HRESULT ImportModel(const CHAR fbxFileName[], const std::vector<std::wstring> textureFileNames);
	void Update(double deltaTime);
	void Draw();
	void CleanUp();

public:
	void SetName(std::wstring name) { mName = name; }

public:
	void* operator new(size_t i);
	void operator delete(void* p);

protected:
	HRESULT CompileShader(const LPCWSTR shaderFileName, LPCSTR entryPointName, LPCSTR shaderModelName, ID3DBlob** shaderBlob);

protected:
	// Model
	Model_::Model mModel;
	ID3D11ShaderResourceView** mModelTextureParts;
	std::wstring mName;

	// Texture
	ID3D11SamplerState* mObjectSamplerState;

	// Constant buffer
	ID3D11Buffer* mCBPerObjectBuffer;
	ID3D11Buffer* mCBPerFrameBuffer;

	// Vertex & index buffer
	ID3D11Buffer** mModelBufferParts;

	// Shader
	ID3D11VertexShader* mVS;
	ID3D11PixelShader* mPS;
	ID3D11PixelShader* mD2d_PS;

	// Input layout
	ID3D11InputLayout* mVertLayout;

	// Direct 3D Core
	ID3D11Device* mD3d11Device;
	ID3D11DeviceContext* mD3d11DevCon;
	Engine* mEngine;
};

