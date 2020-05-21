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
	Vertex(float x, float y, float z,
		float u, float v,
		float nx, float ny, float nz)
		: Pos(x, y, z)
		, TexCoord(u, v)
		, Normal(nx, ny, nz)
	{
	}

	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT2 TexCoord;
	DirectX::XMFLOAT3 Normal;
};

struct Light
{
	Light()
	{
		ZeroMemory(this, sizeof(Light));
	}

	DirectX::XMFLOAT3 Dir;
	float Pad;
	DirectX::XMFLOAT3 Pos;
	float Range;
	DirectX::XMFLOAT3 Att;
	float pad2;
	DirectX::XMFLOAT4 Ambient;
	DirectX::XMFLOAT4 Diffuse;
};

struct CBPerFrame
{
	Light Light;
	DirectX::XMMATRIX Rotation;
};

struct CBPerObject
{
	DirectX::XMMATRIX WVP;
	DirectX::XMMATRIX World;
};

class Engine;

class Object
{
public:
	bool Initalize(const WCHAR shaderFileName[], ID3D11Device* d3d11Device, ID3D11DeviceContext* d3d11DevCon, Engine* engine);
	void Update(double deltaTime);
	void Draw();
	void CleanUp() const;

public:
	void* operator new(size_t i);
	void operator delete(void* p);

protected:
	HRESULT CompileShader(const LPCWSTR shaderFileName, LPCSTR entryPointName, LPCSTR shaderModelName, ID3DBlob** shaderBlob);

protected:
	// Light
	ID3D11Buffer* mCBPerFrameBuffer;
	ID3D11PixelShader* mD2d_PS;
	Light mLight;

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

