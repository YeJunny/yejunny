#pragma once

#pragma comment(lib, "d3dcompiler.lib")

#include "nlohmann/json.hpp"

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

using json = nlohmann::json;

class Engine;

const static DirectX::XMVECTOR	DEFAULT_FORWARD		= DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
const static DirectX::XMVECTOR	DEFAULT_RIGHT		= DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

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
	virtual bool Init(const std::string name, const std::string type, std::string modelPath, std::vector<std::string> const& texturesPath, const std::string shaderPath, Engine* engine);
	virtual void Update(double deltaTime);
	virtual void Draw();
	virtual void CleanUp();
	void CreateBoxCollider(float width, float height, float depth);

public:
	std::string GetName() const { return mName; }
	void SetMatWorld(DirectX::XMMATRIX& worldMat) { mWorld = worldMat; };
	void SetColliWorld(DirectX::XMMATRIX& colliWorld) { mColliWorld = colliWorld; }
	double GetCurrTimeAnim() const { return mCurrTimeAnim; }
	std::string GetType() const { return mType; }

private:
	HRESULT ImportModel(std::string modelPath);
	HRESULT ImportTextures(std::vector<std::string> const& texturesPath);

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
	DirectX::XMMATRIX			mColliWorld;

	// Transform
	DirectX::XMMATRIX			mWorld;

	// Model
	Model_::Model				mModel;
	ID3D11ShaderResourceView**	mModelTextureParts;
	std::string					mName;
	std::string					mType;
	ID3D11Buffer*				mAnimMatCB;
	double						mCurrTimeAnim				= 0;

	// Texture
	ID3D11SamplerState*			mObjectSamplerState;

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
	Engine*						mEngine;
};

