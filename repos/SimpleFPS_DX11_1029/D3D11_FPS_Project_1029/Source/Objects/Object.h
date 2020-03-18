#pragma once

#include "../Settings/Include/Framework.h"
#include "../Settings/Global.h"
#include "../Settings/Time/Time.h"
#include "../FBXLoader/CharacterFBXData/CharacterFbxData.h"
using namespace Microsoft::WRL;
using namespace DirectX;

struct CBufferMatrix
{
	XMMATRIX WorldMat;
	XMMATRIX ViewMat;
	XMMATRIX ProjectionMat;
};

struct CBufferLight
{
	XMFLOAT4 WorldLightPos;
	XMFLOAT4 WorldCameraPos;
};

struct CBufferAnimMatrix
{
	XMMATRIX FinalMatrices[50];
};

class Object
{
public:
	Object();
	~Object();

	virtual void Init(const WCHAR* shaderFile, const char* fbxFile, const WCHAR textureFiles[][TEXTURE_LEN]);
	virtual void InitDetail();
	virtual void Update();
	virtual void Render();

protected:
	ComPtr<ID3D11RasterizerState> mRasterizerState;

	ComPtr<ID3D11VertexShader> mVertexShader;
	ComPtr<ID3D11PixelShader> mPixelShader;
	ComPtr<ID3D11InputLayout> mVertexLayout;

	ComPtr<ID3D11Buffer>* mVertexBuffer;
	ComPtr<ID3D11Buffer> mCBufferMatrix;
	ComPtr<ID3D11Buffer> mCBufferLight;
	ComPtr<ID3D11Buffer> mCBufferAnimMatrix;

	std::unique_ptr<D3D11_INPUT_ELEMENT_DESC[]> mLayout;
	UINT mLayoutElementNumber;

	ComPtr<ID3D11Resource> mTexture2D;
	ComPtr<ID3D11ShaderResourceView>* mTextureResourceView;
	ComPtr<ID3D11SamplerState> mSamplerState;

	XMMATRIX mWorldMat;

	size_t* mVertexCount;
	size_t mMeshCount;

	CharacterFbxData* mCharFbxData;
};