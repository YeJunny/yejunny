#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3d10_1.lib")
#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "D2D1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "dxguid.lib")

#include <d3d11.h>
#include <d3d10_1.h>
#include <dxgi.h>
#include <d2d1.h>
#include <dwrite.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <sstream>
#include <vector>

#include "Object.h"
#include "nlohmann/json.hpp"
#include "WindowManager.h"

using json = nlohmann::json;

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

class Camera;
class InputManager;
class MonsterManager;
class Player;
class Skymap;
class SoundManager;

class Engine : public WindowManager
{
public:
	HRESULT InitializeWindow(HINSTANCE hInstance, int showWnd, bool bIsWindowed, const TCHAR* titleName, const TCHAR* className, INT width, INT height);
	HRESULT InitializeDirect3d11App(HINSTANCE hInstance);
	bool ProcessMessage();
	HRESULT InitScene();
	HRESULT LoadResource();
	void UpdateScene();
	void DrawScene();
	void RenderText(std::wstring text, int inInt);
	void RenderPlayerInfo();
	void ReleaseObject();
	static HRESULT CompileShader(const LPCWSTR shaderFileName, LPCSTR entryPointName, LPCSTR shaderModelName, ID3DBlob** shaderBlob);

public:
	ID3D11Device* GetD3d11Deivce() const { return mD3d11Device; }
	ID3D11DeviceContext* GetD3d11DevCon() const { return mD3d11DevCon; }
	Camera* GetCamera() const { return mCamera; }
	Player* GetPlayer() const { return mPlayer; }
	ID3D11RasterizerState* GetCWcullMode() const { return mCWcullMode; }
	ID3D11RasterizerState* GetCCWcullMode() const { return mCCWcullMode; }
	ID3D11RasterizerState* GetNoCullMode() const { return mNoCullMode; }
	ID3D11RasterizerState* GetWireFrameCWMode() const { return mWireFrameCWMode; }
	void SetPercent(int percent) { mPercent = percent; }
	void AddPercent(int percent) { mPercent += percent; }
	ID3D11InputLayout* GetBaseInputLayout() const { return mBaseVertLayout; }
	ID3D11VertexShader* GetBaseVertShader() const { return mBaseVertShader; }
	ID3D11PixelShader* GetBasePixelShader() const { return mBasePixelShader; }
	ID3D11VertexShader* GetAlphaVertShader() const { return mAlphaVertShader; }
	ID3D11PixelShader* GetAlphaPixelShader() const { return mAlphaPixelShader; }
	ID3D11Buffer** GetSquareVertBuffer() { return &mD2dVertBuffer; }
	ID3D11Buffer** GetSquareIdxBuffer() { return &mD2dIndexBuffer; }
	ID3D11BlendState* GetTransparencyBlendState() const { return mTransparencyBlendState; }
	ID3D11SamplerState** GetBaseSamplerState() { return &mBaseSamplerState; }
	std::vector<Object*> const& GetObjects() const { return mObjects; }
	MonsterManager* GetMonsterManager() const { return mMonsterManager; }
	InputManager* GetInputManager() const { return mInputManager; }
	SoundManager* GetSoundManager() const { return mSoundManager; }

private:
	HRESULT InitD2D_D3D101_DWrite(IDXGIAdapter1* adapter);
	void InitD2DScreenTexture();
	void ImportAllModelThread();
	void DrawLoading(ID3D11ShaderResourceView** loadingTex);
	void DrawTitle(ID3D11ShaderResourceView** titleTex);
	void InitBaseObject();
	void InitLastBaseObject();

private:
	// Drawing Text
	ID3D11BlendState*			mTransparencyBlendState		= nullptr;
	ID3D11Buffer*				mBaseCB						= nullptr;
	ID3D11SamplerState*			mBaseSamplerState			= nullptr;
	ID3D10Device1*				mD3d101Device				= nullptr;
	IDXGIKeyedMutex*			mKeyedMutex11				= nullptr;
	IDXGIKeyedMutex*			mKeyedMutex10				= nullptr;
	ID2D1RenderTarget*			mD2dRenderTarget			= nullptr;
	ID2D1SolidColorBrush*		mBrush						= nullptr;
	ID3D11Texture2D*			mBackbuffer11				= nullptr;
	ID3D11Texture2D*			mSharedTex11				= nullptr;
	ID3D11Buffer*				mD2dVertBuffer				= nullptr;
	ID3D11Buffer*				mD2dIndexBuffer				= nullptr;
	ID3D11ShaderResourceView*	mD2dTexture					= nullptr;
	IDWriteFactory*				mDWriteFactory				= nullptr;
	IDWriteTextFormat*			mTextFormat					= nullptr;

	// Core
	IDXGISwapChain*				mSwapChain					= nullptr;
	ID3D11Device*				mD3d11Device				= nullptr;
	ID3D11DeviceContext*		mD3d11DevCon				= nullptr;
	ID3D11RenderTargetView*		mRenderTargetView			= nullptr;
	ID3D11DepthStencilView*		mDepthStencilView			= nullptr;
	ID3D11RasterizerState*		mCCWcullMode				= nullptr;
	ID3D11RasterizerState*		mCWcullMode					= nullptr;
	ID3D11RasterizerState*		mNoCullMode					= nullptr;
	ID3D11RasterizerState*		mWireFrameCWMode			= nullptr;

	// Shaders
	ID3D11InputLayout*			mBaseVertLayout				= nullptr;
	ID3D11VertexShader*			mBaseVertShader				= nullptr;
	ID3D11PixelShader*			mBasePixelShader			= nullptr;
	ID3D11VertexShader*			mTextVertShader				= nullptr;
	ID3D11PixelShader*			mTextPixelShader			= nullptr;
	ID3D11VertexShader*			mAlphaVertShader			= nullptr;
	ID3D11PixelShader*			mAlphaPixelShader			= nullptr;

	// Objects
	Skymap*						mSkymap;
	Camera*						mCamera;
	Player*						mPlayer;
	std::vector<Object*>		mObjects;
	MonsterManager*				mMonsterManager;
	InputManager*				mInputManager;
	SoundManager*				mSoundManager;

	// Temp
	int							mPercent;
	bool						mbIsEnd;
};

