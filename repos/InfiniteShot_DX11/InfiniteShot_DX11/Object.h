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

struct CBAnimMat
{
	DirectX::XMMATRIX AnimMat[50];
};

class Object
{
public:
	void Setup(ID3D11Device* d3d11Device, ID3D11DeviceContext* d3d11DevCon, Engine* engine);
	virtual bool Initalize(const WCHAR shaderFileName[]);
	HRESULT ImportModel(const CHAR fbxFileName[]);
	HRESULT ImportTextures();
	virtual void Update(double deltaTime);
	virtual void Draw();
	virtual void CleanUp();

public:
	void SetName(std::wstring name) { mName = name; }
	std::wstring GetName() const { return mName; }
	void SetTextureFileNamesVector(std::vector<std::wstring> const& textureFileNames) { mTextureFileNames = textureFileNames; }
	void SetMatWorld(DirectX::XMMATRIX& worldMat) { mWorld = worldMat; };
	void SetBoxCollider(float width, float height, float depth);

public:
	/*void* operator new(size_t i);
	void operator delete(void* p);*/

protected:
	// Box Collider
	DirectX::XMFLOAT3			mColliVert[8];
	DWORD						mColliIndex[36];
	ID3D11Buffer*				mColliVertBuf				= nullptr;
	ID3D11Buffer*				mColliIndexBuf				= nullptr;
	ID3D11InputLayout*			mColliVertLayout			= nullptr;

	// Transform
	DirectX::XMMATRIX			mWorld;

	// Model
	Model_::Model				mModel;
	ID3D11ShaderResourceView**	mModelTextureParts;
	std::wstring				mName;
	ID3D11Buffer*				mAnimMatCB;
	double						mCurrTimeAnim				= 0;

	// Texture
	ID3D11SamplerState*			mObjectSamplerState;
	std::vector<std::wstring>	mTextureFileNames;

	// Constant buffer
	ID3D11Buffer*				mCBPerObjectBuffer;
	ID3D11Buffer*				mCBPerFrameBuffer;

	// Vertex & index buffer
	ID3D11Buffer**				mModelVertBufParts;

	// Shader
	ID3D11VertexShader*			mVS;
	ID3D11PixelShader*			mPS;

	// Input layout
	ID3D11InputLayout*			mVertLayout;

	// Direct 3D Core
	ID3D11Device*				mD3d11Device;
	ID3D11DeviceContext*		mD3d11DevCon;
	Engine*						mEngine;
};

