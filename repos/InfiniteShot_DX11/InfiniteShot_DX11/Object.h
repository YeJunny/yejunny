#pragma once

#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include "ErrorLogger.h"

#ifndef null
	#define null (NULL)
#endif

struct Vertex
{
	Vertex(float x, float y, float z, float u, float v)
		: Pos(x, y, z)
		, TexCoord(u, v)
	{
	}

	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT2 TexCoord;
};

struct CBPerObject
{
	DirectX::XMMATRIX WVP;
};

class Engine;

class Object
{
public:
	bool Initalize(const WCHAR shaderFileName[], ID3D11Device* d3d11Device, ID3D11DeviceContext* d3d11DevCon, Engine* engine);
	void Update();
	void Draw();
	void CleanUp() const;

public:
	void* operator new(size_t i);
	void operator delete(void* p);

protected:
	HRESULT CompileShader(const LPCWSTR shaderFileName, LPCSTR entryPointName, LPCSTR shaderModelName, ID3DBlob** shaderBlob);

protected:
	// Texture
	ID3D11ShaderResourceView* mCubesTexture;
	ID3D11SamplerState* mCubesSamplerState;

	// Constant buffer
	ID3D11Buffer* mCBPerObjectBuffer;

	// Vertex & index buffer
	ID3D11Buffer* mTrianglesVertBuffer;
	ID3D11Buffer* mTriIndexBuffer;

	// Shader
	ID3D11VertexShader* mVS;
	ID3D11PixelShader* mPS;

	// Input layout
	ID3D11InputLayout* mVertLayout;

	// Matrics
	DirectX::XMVECTOR mCamPosition;
	DirectX::XMMATRIX mCamView;
	DirectX::XMMATRIX mCamProjection;
	DirectX::XMMATRIX mCube1World;
	DirectX::XMMATRIX mCube2World;

	// Direct 3D Core
	ID3D11Device* mD3d11Device;
	ID3D11DeviceContext* mD3d11DevCon;
	Engine* mEngine;
};

