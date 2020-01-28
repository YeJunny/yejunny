#pragma once

#include "framework.h"
#include "Global.h"
#include "Timer.h"
using namespace Microsoft::WRL;
using namespace DirectX;

struct VertexElements
{
	XMFLOAT3 Pos;
	XMFLOAT2 UV;
	XMFLOAT3 Normal;
};

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

class Object
{
public:
	Object();
	~Object();

	virtual void Init(const ComPtr<ID3D11Device> d3dDevice, HWND hWnd, 
		const WCHAR* shaderFile, const char* fbxFile, const WCHAR textureFiles[][TEXTURE_LEN],
		const XMMATRIX& projectionMat, std::shared_ptr<Timer> timer);
	virtual void InitDetail(HWND hWnd);
	virtual void Update(const XMMATRIX& viewMat);
	virtual void Render(const XMFLOAT3& playerPos);

protected:
	// Shared Pointers
	ComPtr<ID3D11Device> mD3DDevice;
	ComPtr<ID3D11DeviceContext> mD3DContext;
	ComPtr<ID3D11RasterizerState> mRasterizerState;

	// Non shared pointers
	ComPtr<ID3D11VertexShader> mVertexShader;
	ComPtr<ID3D11PixelShader> mPixelShader;
	ComPtr<ID3D11InputLayout> mVertexLayout;

	ComPtr<ID3D11Buffer>* mVertexBuffer;
	ComPtr<ID3D11Buffer> mCBufferMatrix;
	ComPtr<ID3D11Buffer> mCBufferLight;

	std::unique_ptr<D3D11_INPUT_ELEMENT_DESC[]> mLayout;
	UINT mLayoutElementNumber;
	
	ComPtr<ID3D11Resource> mTexture2D;
	ComPtr<ID3D11ShaderResourceView>* mTextureResourceView;
	ComPtr<ID3D11SamplerState> mSamplerState;

	XMMATRIX mWorldMat;
	XMMATRIX mViewMat;
	XMMATRIX mProjectionMat;

	size_t* mVertexCount;
	size_t mMeshCount;
	std::shared_ptr<Timer> mTimer;
};