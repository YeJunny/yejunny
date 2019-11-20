#include "D3DSetting.h"
#include "Input.h"
#include "Timer.h"
using namespace DirectX;

D3DSetting::D3DSetting()
	: mFeatureLevel()
	, mSwapChain(nullptr)
	, mpD3DDevice(nullptr)
	, mpD3DContext(nullptr)
	, mpRenderTargetView(nullptr)
	, mpDepthStencil(nullptr)
	, mpDepthStencilView(nullptr)
	, mWorld()
	, mView()
	, mProjection()
	, mWidth(0.0f)
	, mHeight(0.0f)
{
	mInput = std::make_unique<Input>();
	mTimer = std::make_unique<Timer>();
}

D3DSetting::~D3DSetting()
{
	mInput->Shutdown();

	mInput.reset();
	mTimer.reset();

	mpD3DContext.Reset();
	mSwapChain.Reset();
	mpD3DDevice.Reset();
}

bool D3DSetting::InitD3D_win(HINSTANCE hInst, HWND hWnd)
{
	mhWnd = hWnd;

	// CREATE DEVICE & SWAP CHAIN
	RECT rt;
	GetClientRect(hWnd, &rt);
	mWidth = static_cast<FLOAT>(rt.right - rt.left);
	mHeight = static_cast<FLOAT>(rt.bottom - rt.top);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.BufferDesc.Width = static_cast<UINT>(mWidth);
	swapChainDesc.BufferDesc.Height = static_cast<UINT>(mHeight);
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = true;

	if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &mSwapChain, &mpD3DDevice, mFeatureLevel, &mpD3DContext)))
	{
		return false;
	}


	// SET RENDER TARGET VIEW
	ID3D11Texture2D* pBackBuffer = nullptr;

	if (FAILED(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
	{
		return false;
	}

	if (FAILED(mpD3DDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mpRenderTargetView)))
	{
		return false;
	}


	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = static_cast<UINT>(mWidth);
	descDepth.Height = static_cast<UINT>(mHeight);
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	if (FAILED(mpD3DDevice->CreateTexture2D(&descDepth, NULL, &mpDepthStencil)))
	{
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	if (FAILED(mpD3DDevice->CreateDepthStencilView(mpDepthStencil, &descDSV, &mpDepthStencilView)))
	{
		return false;
	}

	mpD3DContext->OMSetRenderTargets(1, &mpRenderTargetView, mpDepthStencilView);


	// SET VIEW PORT
	D3D11_VIEWPORT vp;
	vp.Width = mWidth;
	vp.Height = mHeight;
	vp.MaxDepth = 1.0f;
	vp.MinDepth = 0.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	mpD3DContext->RSSetViewports(1, &vp);


	// SET MATRIX
	mProjection = XMMatrixPerspectiveFovLH(XM_PIDIV2, mWidth / static_cast<int>(mHeight), 0.01f, 100.0f);


	// Other Init
	if (!mInput->Initialize(hInst, hWnd, static_cast<int>(mWidth), static_cast<int>(mHeight)))
	{
		return false;
	}

	Init();

	return true;
}

void D3DSetting::Update_win()
{
	mInput->Frame();
	mTimer->Update();
	Update();
}

void D3DSetting::Render_win()
{
	Render();
}

void D3DSetting::Release_win()
{
	Release();
}