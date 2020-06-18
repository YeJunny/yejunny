#include "Engine.h"
#include "Camera.h"
#include "InputManager.h"
#include "Monster.h"
#include "MonsterManager.h"
#include "Player.h"
#include "Skymap.h"
#include "SoundManager.h"
#include "Utilities.h"
#include <ScreenGrab.h>
#include <thread>
#include <wincodec.h>
#include <WICTextureLoader.h>
#include <wrl.h>
using namespace DirectX;

HRESULT Engine::InitializeWindow(HINSTANCE hInstance, int showWnd, bool bIsWindowed, const TCHAR* titleName, const TCHAR* className, INT width, INT height)
{
	HRESULT hr = WindowManager::InitializeWindow(hInstance, showWnd, bIsWindowed, titleName, className, width, height);
	AssertInitialization(hr, "Initialize error!");
	
	return S_OK;
}

bool Engine::ProcessMessage()
{
	return WindowManager::ProcessMessage();
}

HRESULT Engine::InitializeDirect3d11App(HINSTANCE hInstance)
{
	HRESULT hr;
	
	//Describe backbuffer
	DXGI_MODE_DESC backbufferDesc;

	ZeroMemory(&backbufferDesc, sizeof(DXGI_MODE_DESC));

	backbufferDesc.Width = mWidth;
	backbufferDesc.Height = mHeight;
	backbufferDesc.RefreshRate.Numerator = 60;
	backbufferDesc.RefreshRate.Denominator = 1;
	backbufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	backbufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	backbufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;


	// Describe our swap chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferDesc = backbufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = mHwnd;
	swapChainDesc.Windowed = false;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Create DXGI Factory to enumerate adapters
	IDXGIFactory1* DXGIFactory;

	hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&DXGIFactory);
	AssertInitialization(hr, "Direct 3D Create DXGI Factory - Failed");

	// Use the first adapter
	IDXGIAdapter1* adapter;

	hr = DXGIFactory->EnumAdapters1(1, &adapter);
	AssertInitialization(hr, "Direct 3D EnumAdapters1 - Failed");

	DXGI_ADAPTER_DESC1 adapterDescription;
	adapter->GetDesc1(&adapterDescription);
	OutputDebugStringA("==========================\n");
	OutputDebugStringW(adapterDescription.Description);
	OutputDebugStringA("\n==========================\n");

	DXGIFactory->Release();


	// Creating direct3d 11 device & swap chain
	hr = D3D11CreateDeviceAndSwapChain(
		adapter,
		D3D_DRIVER_TYPE_UNKNOWN,
		null,
		D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		null,
		null,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&mSwapChain,
		&mD3d11Device,
		nullptr,
		&mD3d11DevCon
	);
	AssertInitialization(hr, "Direct 3D Create Device & Swap Chain - Failed");

	// Initalizer direct2d, direct10.1, directwrite
	InitD2D_D3D101_DWrite(adapter);

	adapter->Release();

	// Creating back buffer
	ID3D11Texture2D* backBuffer;

	hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	AssertInitialization(hr, "Direct 3D Get Buffer - Failed");

	// Creating render target
	hr = mD3d11Device->CreateRenderTargetView(backBuffer, nullptr, &mRenderTargetView);
	AssertInitialization(hr, "Direct 3D Create Render Target - Failed");

	backBuffer->Release();

	// Describe depth stencil buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = mWidth;
	depthStencilDesc.Height = mHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	// Create depth stencil
	ID3D11Texture2D* depthStencilBuffer;

	hr = mD3d11Device->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer);
	AssertInitialization(hr, "Direct 3D Create Depth Stencil Buffer - Failed");

	hr = mD3d11Device->CreateDepthStencilView(depthStencilBuffer, nullptr, &mDepthStencilView);
	AssertInitialization(hr, "Direct 3D Create Depth Stencil View - Failed");

	depthStencilBuffer->Release();

	return S_OK;
}

void Engine::DrawLoading(ID3D11ShaderResourceView** loadingTex)
{
	float bgColor[] = { 0, 0, 0, 0.0f };
	mD3d11DevCon->ClearRenderTargetView(mRenderTargetView, bgColor);
	mD3d11DevCon->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	CBPerObject cbPerObject;
	cbPerObject.World = XMMatrixTranspose(XMMatrixScaling(0.5f, 0.1f, 1.0f));
	cbPerObject.WVP = XMMatrixTranspose(XMMatrixScaling(0.5f, 0.1f, 1.0f));

	UINT strides = sizeof(Vertex);
	UINT offset = 0;

	mD3d11DevCon->IASetVertexBuffers(0, 1, GetSquareVertBuffer(), &strides, &offset);
	mD3d11DevCon->IASetIndexBuffer(*GetSquareIdxBuffer(), DXGI_FORMAT_R32_UINT, 0);
	mD3d11DevCon->IASetInputLayout(mBaseVertLayout);

	mD3d11DevCon->PSSetShaderResources(0, 1, loadingTex);

	mD3d11DevCon->VSSetShader(mBaseVertShader, nullptr, 0);
	mD3d11DevCon->PSSetShader(mBasePixelShader, nullptr, 0);

	mD3d11DevCon->UpdateSubresource(mBaseCB, 0, nullptr, &cbPerObject, 0, 0);
	mD3d11DevCon->VSSetConstantBuffers(0, 1, &mBaseCB);

	mD3d11DevCon->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	mD3d11DevCon->DrawIndexed(6, 0, 0);

	if (!mObjects.empty())
	{
		RenderText(L"Loding Model Resources Name : " + StringConverter::StringToWide(mObjects.back()->GetName()) + L" ... ", mPercent);
	}

	mSwapChain->Present(0, 0);
}

HRESULT Engine::InitD2D_D3D101_DWrite(IDXGIAdapter1* adapter)
{
	// Create direct 10.1 device
	HRESULT hr = D3D10CreateDevice1(
		adapter,
		D3D10_DRIVER_TYPE_HARDWARE,
		null,
		D3D10_CREATE_DEVICE_DEBUG | D3D10_CREATE_DEVICE_BGRA_SUPPORT,
		D3D10_FEATURE_LEVEL_9_3,
		D3D10_1_SDK_VERSION,
		&mD3d101Device
	);
	AssertInitialization(hr, "Direct 3D Create Device 10.1 - Failed");

	// Create shared texture that direct 3d 10.1 will render on
	D3D11_TEXTURE2D_DESC sharedTexDesc;
	ZeroMemory(&sharedTexDesc, sizeof(sharedTexDesc));

	sharedTexDesc.Width = mWidth;
	sharedTexDesc.Height = mHeight;
	sharedTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sharedTexDesc.MipLevels = 1;
	sharedTexDesc.ArraySize = 1;
	sharedTexDesc.SampleDesc.Count = 1;
	sharedTexDesc.Usage = D3D11_USAGE_DEFAULT;
	sharedTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	sharedTexDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX;
	
	hr = mD3d11Device->CreateTexture2D(&sharedTexDesc, nullptr, &mSharedTex11);
	AssertInitialization(hr, "Direct 3D Create Texture 2D - Failed");

	// Get the keyed mutex for the shared texture (for D3D11)
	hr = mSharedTex11->QueryInterface(__uuidof(IDXGIKeyedMutex), (void**)&mKeyedMutex11);
	AssertInitialization(hr, "Direct 3D Texture2D Query Interface - Failed");

	// Get the shared handle needed to open the ahared texture in D3D10.1
	IDXGIResource* sharedResource10;
	HANDLE sharedHandle10;

	hr = mSharedTex11->QueryInterface(__uuidof(IDXGIResource), (void**)&sharedResource10);
	AssertInitialization(hr, "Direct 3D Texture2D Query Interface = Failed");

	hr = sharedResource10->GetSharedHandle(&sharedHandle10);
	AssertInitialization(hr, "Direct 3D IDXGIResource Query Interface = Failed");

	sharedResource10->Release();

	// Open the surface for the shared texture in d3d10.1
	IDXGISurface1* sharedSurface10;

	hr = mD3d101Device->OpenSharedResource(sharedHandle10, __uuidof(IDXGISurface1), (void**)&sharedSurface10);
	AssertInitialization(hr, "Direct 3D 10.1 Open Shared Resource - Failed");

	hr = sharedSurface10->QueryInterface(__uuidof(IDXGIKeyedMutex), (void**)&mKeyedMutex10);
	AssertInitialization(hr, "Direct 3D 10.1 IDXGISurface1 Query Interface - Failed");

	// Create d2d factory
	ID2D1Factory* D2DFactory;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), (void**)&D2DFactory);
	AssertInitialization(hr, "Direct 2D 1 Create Factory - Failed");

	D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties;

	ZeroMemory(&renderTargetProperties, sizeof(renderTargetProperties));

	renderTargetProperties.type = D2D1_RENDER_TARGET_TYPE_HARDWARE;
	renderTargetProperties.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED);

	hr = D2DFactory->CreateDxgiSurfaceRenderTarget(sharedSurface10, &renderTargetProperties, &mD2dRenderTarget);
	AssertInitialization(hr, "Direct 2D 1 Create Dxgi Surface Render Target - Failed");

	sharedSurface10->Release();
	D2DFactory->Release();


	// Create a solid color brush to draw something with 
	hr = mD2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 0.0f, 1.0f), &mBrush);
	AssertInitialization(hr, "Direct 2D 1 Create Solid Color Brush - Failed");


	// Direct write
	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&mDWriteFactory)
	);
	AssertInitialization(hr, "Create Direct Write - Failed");
	
	hr = mDWriteFactory->CreateTextFormat(
		L"Script",
		nullptr,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		30.0f,
		L"en-us",
		&mTextFormat
	);
	AssertInitialization(hr, "Direct Write Create Text Format - Failed");

	hr = mTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	AssertInitialization(hr, "Direct Write Set Text Alignment - Failed");

	hr = mTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	AssertInitialization(hr, "Direct Write Set Paragraph Alignment - Failed");

	// Kepping D3D 10.1 Debug Output Quiet
	mD3d101Device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);

	return S_OK;
}

void Engine::InitD2DScreenTexture()
{
	// Create the vertex buffer
	Vertex v[] =
	{
		// Front Face
		Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f,-1.0f, -1.0f, -1.0f),
		Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f,-1.0f,  1.0f, -1.0f),
		Vertex(1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f,  1.0f, -1.0f),
		Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f),
	};

	DWORD indices[] =
	{
		// Front Face
		0, 1, 2,
		0, 2, 3,
	};

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * 2 * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData;
	ZeroMemory(&indexBufferData, sizeof(indexBufferData));

	indexBufferData.pSysMem = indices;

	HRESULT hr = mD3d11Device->CreateBuffer(&indexBufferDesc, &indexBufferData, &mD2dIndexBuffer);
	AssertInitialization(hr, "Direct 3D 11 Create Buffer - Failed");

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * 4;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	vertexBufferData.pSysMem = v;
	hr = mD3d11Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &mD2dVertBuffer);
	AssertInitialization(hr, "Direct 3D 11 Create Buffer - Failed");

	// Create a shader resource view from the texture D2D will render to,
	// So we can use it to texutre a square which overlays our scene
	hr = mD3d11Device->CreateShaderResourceView(mSharedTex11, nullptr, &mD2dTexture);
	AssertInitialization(hr, "Direct 3D 11 Create Shader Resource View - failed");
}

HRESULT Engine::InitScene()
{
	InitD2DScreenTexture();

	ID3DBlob* vsBuffer;
	ID3DBlob* psBuffer;
	ID3DBlob* d2dVSBuffer;
	ID3DBlob* d2dPSBuffer;
	ID3DBlob* alphaVSBuffer;
	ID3DBlob* alphaPSBuffer;

	// Compile shaders from shader file
	HRESULT hr = Engine::CompileShader(L"Shaders\\BaseEffects.fx", "VS", "vs_5_0", &vsBuffer);
	AssertInitialization(hr, "Direct 3D CompileShader - Failed");
	hr = Engine::CompileShader(L"Shaders\\BaseEffects.fx", "PS", "ps_5_0", &psBuffer);
	AssertInitialization(hr, "Direct 3D CompileShader - Failed");
	hr = Engine::CompileShader(L"Shaders\\TextEffects.fx", "D2D_VS", "vs_5_0", &d2dVSBuffer);
	AssertInitialization(hr, "Direct 3D CompileShader - Failed");
	hr = Engine::CompileShader(L"Shaders\\TextEffects.fx", "D2D_PS", "ps_5_0", &d2dPSBuffer);
	AssertInitialization(hr, "Direct 3D CompileShader - Failed");
	hr = Engine::CompileShader(L"Shaders\\AlphaEffects.fx", "VS", "vs_5_0", &alphaVSBuffer);
	AssertInitialization(hr, "Direct 3D CompileShader - Failed");
	hr = Engine::CompileShader(L"Shaders\\AlphaEffects.fx", "PS", "ps_5_0", &alphaPSBuffer);
	AssertInitialization(hr, "Direct 3D CompileShader - Failed");

	// Create the shader objects
	hr = mD3d11Device->CreateVertexShader(
		vsBuffer->GetBufferPointer(),
		vsBuffer->GetBufferSize(),
		nullptr,
		&mBaseVertShader
	);
	AssertInitialization(hr, "Direct 3D Create Vertex Shader - Failed");

	hr = mD3d11Device->CreatePixelShader(
		psBuffer->GetBufferPointer(),
		psBuffer->GetBufferSize(),
		nullptr,
		&mBasePixelShader
	);
	AssertInitialization(hr, "Direct 3D Create Pixel Shader - Failed");

	hr = mD3d11Device->CreateVertexShader(
		d2dVSBuffer->GetBufferPointer(),
		d2dVSBuffer->GetBufferSize(),
		nullptr,
		&mTextVertShader
	);
	AssertInitialization(hr, "Direct 3D Create Vertex Shader - Failed");

	hr = mD3d11Device->CreatePixelShader(
		d2dPSBuffer->GetBufferPointer(),
		d2dPSBuffer->GetBufferSize(),
		nullptr,
		&mTextPixelShader
	);
	AssertInitialization(hr, "Direct 3D Create Pixel Shader - Failed");

	hr = mD3d11Device->CreateVertexShader(
		alphaVSBuffer->GetBufferPointer(),
		alphaVSBuffer->GetBufferSize(),
		nullptr,
		&mAlphaVertShader
	);
	AssertInitialization(hr, "Direct 3D Create Vertex Shader - Failed");

	hr = mD3d11Device->CreatePixelShader(
		alphaPSBuffer->GetBufferPointer(),
		alphaPSBuffer->GetBufferSize(),
		nullptr,
		&mAlphaPixelShader
	);
	AssertInitialization(hr, "Direct 3D Create Pixel Shader - Failed");


	// Create the input layout
	D3D11_INPUT_ELEMENT_DESC layouts[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	UINT numElements = ARRAYSIZE(layouts);

	hr = mD3d11Device->CreateInputLayout(layouts, numElements, vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), &mBaseVertLayout);
	AssertInitialization(hr, "Direct 3D Create Input Layout - Failed");


	// Set the input layout
	mD3d11DevCon->IASetInputLayout(mBaseVertLayout);


	vsBuffer->Release();
	psBuffer->Release();
	d2dVSBuffer->Release();
	d2dPSBuffer->Release();
	alphaVSBuffer->Release();
	alphaPSBuffer->Release();


	// Set primitive topology
	mD3d11DevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(viewport));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<FLOAT>(mWidth);
	viewport.Height = static_cast<FLOAT>(mHeight);
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;

	// Set the viewport
	mD3d11DevCon->RSSetViewports(1, &viewport);


	// Create blender state
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(rtbd));

	rtbd.BlendEnable = true;
	rtbd.SrcBlend = D3D11_BLEND_SRC_COLOR;
	rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	rtbd.BlendOp = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = rtbd;

	hr = mD3d11Device->CreateBlendState(&blendDesc, &mTransparencyBlendState);
	AssertInitialization(hr, "Direct 3D 11 Create Blend State - Failed");

	// Create constant buffer
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(cbDesc));
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.ByteWidth = sizeof(CBPerObject);
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = 0;
	cbDesc.MiscFlags = 0;

	hr = mD3d11Device->CreateBuffer(&cbDesc, nullptr, &mBaseCB);
	AssertInitialization(hr, "Direct 3D Create Constant Buffer - Failed");

	// Describe the sampler
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0.0f;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create sampler state
	hr = mD3d11Device->CreateSamplerState(&sampDesc, &mBaseSamplerState);
	AssertInitialization(hr, "Direct 3D Create Sampler State - Failed");

	// Create CCW & CW & No Culling
	D3D11_RASTERIZER_DESC cmDesc;
	ZeroMemory(&cmDesc, sizeof(cmDesc));

	cmDesc.FillMode = D3D11_FILL_SOLID;
	cmDesc.CullMode = D3D11_CULL_BACK;

	cmDesc.FrontCounterClockwise = true;

	hr = mD3d11Device->CreateRasterizerState(&cmDesc, &mCCWcullMode);
	AssertInitialization(hr, "Direct 3D Create Rasterizer State - Failed");

	cmDesc.FrontCounterClockwise = false;
	hr = mD3d11Device->CreateRasterizerState(&cmDesc, &mCWcullMode);
	AssertInitialization(hr, "Direct 3D CreateRasterizer State - Failed");

	cmDesc.FillMode = D3D11_FILL_WIREFRAME;
	hr = mD3d11Device->CreateRasterizerState(&cmDesc, &mWireFrameCWMode);
	AssertInitialization(hr, "Direct 3D CreateRasterizer State - Failed");

	D3D11_RASTERIZER_DESC cullDesc;
	ZeroMemory(&cullDesc, sizeof(cullDesc));
	cullDesc.FillMode = D3D11_FILL_SOLID;
	cullDesc.CullMode = D3D11_CULL_NONE;

	hr = mD3d11Device->CreateRasterizerState(&cullDesc, &mNoCullMode);
	AssertInitialization(hr, "Direct 3D Create Rasterizer state- Failed");


	return S_OK;
}

void Engine::InitBaseObject()
{
	mSoundManager = new SoundManager();
	mSoundManager->Init(this);


	mPlayer = new Player();
	mPlayer->Init(this);


	mInputManager = new InputManager();
	mInputManager->Init(mInstance, mHwnd, this);


	mCamera = new Camera();
	mCamera->Init(this);


	mSkymap = new Skymap();
	mSkymap->CreateSpere(10, 10, this);
}

void Engine::InitLastBaseObject()
{
	// Monster Manager
	mMonsterManager = new MonsterManager();
	mMonsterManager->Init(this);
}

void Engine::DrawTitle(ID3D11ShaderResourceView** titleTex)
{
	float bgColor[] = { 0, 0, 0, 0.0f };
	mD3d11DevCon->ClearRenderTargetView(mRenderTargetView, bgColor);
	mD3d11DevCon->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	CBPerObject cbPerObject;
	cbPerObject.World = XMMatrixTranspose(XMMatrixIdentity());
	cbPerObject.WVP = XMMatrixTranspose(XMMatrixIdentity());

	UINT strides = sizeof(Vertex);
	UINT offset = 0;

	mD3d11DevCon->IASetVertexBuffers(0, 1, GetSquareVertBuffer(), &strides, &offset);
	mD3d11DevCon->IASetIndexBuffer(*GetSquareIdxBuffer(), DXGI_FORMAT_R32_UINT, 0);
	mD3d11DevCon->IASetInputLayout(mBaseVertLayout);

	mD3d11DevCon->PSSetShaderResources(0, 1, titleTex);

	mD3d11DevCon->VSSetShader(mBaseVertShader, nullptr, 0);
	mD3d11DevCon->PSSetShader(mBasePixelShader, nullptr, 0);

	mD3d11DevCon->UpdateSubresource(mBaseCB, 0, nullptr, &cbPerObject, 0, 0);
	mD3d11DevCon->VSSetConstantBuffers(0, 1, &mBaseCB);

	mD3d11DevCon->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	mD3d11DevCon->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	mD3d11DevCon->RSSetState(mCWcullMode);

	mD3d11DevCon->DrawIndexed(6, 0, 0);

	mSwapChain->Present(0, 0);
}

void Engine::ImportAllModelThread()
{
	json modelJson = Utilities::ParseJsonFile("Settings/Model_List.json");

	auto objects = modelJson.items();

	for (auto iter = objects.begin(); iter != objects.end(); ++iter)
	{
		std::string objName = (*iter).key();

		json objValue = (*iter).value();
		std::string type = objValue["Type"];
		std::string modelPath = objValue["ModelPath"];
		std::string shaderPath = objValue["ShaderPath"];
		std::vector<std::string> textures = objValue["Textures"].get<std::vector<std::string>>();

		if (type == "Monster")
		{
			Object* object = new Monster();
			mObjects.push_back(object);
			reinterpret_cast<Monster*>(object)->Init(objName, type, modelPath, textures, shaderPath, 10, this);
		}
		else
		{
			Object* object = new Object();
			mObjects.push_back(object);
			object->Init(objName, type, modelPath, textures, shaderPath, this);
		}

		textures.clear();
	}

	// End of thread
	mbIsEnd = true;
}

HRESULT Engine::LoadResource()
{
	InitBaseObject();


	// Main title
	ID3D11ShaderResourceView* titleTex;
	HRESULT hr = CreateWICTextureFromFile(mD3d11Device, L"Textures\\Title.png", nullptr, &titleTex, 0);
	AssertInitialization(hr, "Direct 3D Create WIC Texture From File - Failed");

	while (true)
	{
		mInputManager->Update();

		if (mInputManager->IsPressKey(DIK_SPACE))
		{
			break;
		}

		DrawTitle(&titleTex);
	}
	titleTex->Release();


	// Loading
	ID3D11ShaderResourceView* loadingTex;
	hr = CreateWICTextureFromFile(mD3d11Device, L"Textures\\Loading.png", nullptr, &loadingTex, 0);
	AssertInitialization(hr, "Direct 3D Create WIC Texture From File - Failed");
	DrawLoading(&loadingTex);


	// Import Model
	mbIsEnd = false;
	std::thread allModelThread(&Engine::ImportAllModelThread, this);


	// Draw loading
	int tempPercent = 0;
	while (!mbIsEnd)
	{
		if (tempPercent != mPercent)
		{
			tempPercent = mPercent;

			DrawLoading(&loadingTex);
		}
	}
	loadingTex->Release();

	allModelThread.join();


	InitLastBaseObject();

	return S_OK;
}

void Engine::UpdateScene()
{
	if (mPlayer->IsDead())
	{
		mPlayer->UpdateDeadImage(mFrameTime);
		return;
	}

	mInputManager->Update();

	mCamera->Update(mFrameTime);

	mSkymap->Update(mFrameTime);

	mPlayer->Update(mFrameTime);

	for (size_t i = 0; i < mObjects.size(); ++i)
	{
		mObjects[i]->Update(mFrameTime);
	}
}

void Engine::DrawScene()
{
	// Clear backbuffer
	float bgColor[] = { 0, 0, 0, 0.0f };
	mD3d11DevCon->ClearRenderTargetView(mRenderTargetView, bgColor);
	mD3d11DevCon->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Set the render target & depth stencil
	mD3d11DevCon->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	mD3d11DevCon->OMSetBlendState(nullptr, null, 0xffffffff);
	

	// Draw objects
	for (size_t i = 0; i < mObjects.size(); ++i)
	{
		mObjects[i]->Draw();
	}

	mSkymap->Draw();

	mPlayer->Draw();

	RenderPlayerInfo();

	// Present the back buffer to the screen
	mSwapChain->Present(0, 0);
}


void Engine::ReleaseObject()
{
	mSwapChain->SetFullscreenState(false, nullptr);
	PostMessage(mHwnd, WM_DESTROY, 0, 0);

	// Release the objects
	mCamera->CleanUp();
	mSkymap->CleanUp();
	mPlayer->CleanUp();
	mMonsterManager->CleanUp();
	mInputManager->CleanUp();
	mSoundManager->CleanUp();
	delete mCamera;
	delete mSkymap;
	delete mPlayer;
	delete mMonsterManager;
	delete mInputManager;
	delete mSoundManager;

	for (int i = mObjects.size() - 1; i >= 0; --i)
	{
		mObjects[i]->CleanUp();
		delete[] mObjects[i];
	}

	// Release the COM Ojbects
	mTransparencyBlendState->Release();
	mBaseCB->Release();
	mBaseSamplerState->Release();
	mD3d101Device->Release();
	mKeyedMutex10->Release();
	mKeyedMutex11->Release();
	mD2dRenderTarget->Release();
	mD2dTexture->Release();
	mBrush->Release();
	mSharedTex11->Release();
	mDWriteFactory->Release();
	mTextFormat->Release();
	mSwapChain->Release();
	mDepthStencilView->Release();
	mRenderTargetView->Release();
	mD3d11DevCon->Release();
	mD3d11Device->Release();
}

void Engine::RenderText(std::wstring text, int inInt)
{
	// Set shaders
	mD3d11DevCon->VSSetShader(mTextVertShader, nullptr, 0);
	mD3d11DevCon->PSSetShader(mTextPixelShader, nullptr, 0);

	// Release the d3d 11 device
	mKeyedMutex11->ReleaseSync(0);

	// Use d3d10.1 device
	mKeyedMutex10->AcquireSync(0, 5);

	// Draw d2d content
	mD2dRenderTarget->BeginDraw();

	// Clear d2d background
	mD2dRenderTarget->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f));

	// Create the string
	std::wostringstream outputString;
	outputString << text << inInt;
	std::wstring outputText = outputString.str();

	// Set the font color
	D2D1_COLOR_F fontColor = D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f);

	// Set the brush color d2d will use to draw with
	mBrush->SetColor(fontColor);

	// Create the d2d render area
	D2D1_RECT_F layoutRect = D2D1::RectF(0, 0, static_cast<FLOAT>(mWidth), static_cast<FLOAT>(mHeight));

	// Draw the text
	mD2dRenderTarget->DrawText(
		outputText.c_str(),
		wcslen(outputText.c_str()),
		mTextFormat,
		layoutRect,
		mBrush
	);

	mD2dRenderTarget->EndDraw();

	// Release the d3d10.1 device
	mKeyedMutex10->ReleaseSync(1);

	// Use the d3d11 device
	mKeyedMutex11->AcquireSync(1, 5);

#ifdef _DEBUG
	/*ID3D11Resource* d2dResource = nullptr;
	mD2dTexture->GetResource(&d2dResource);
	SaveWICTextureToFile(mD3d11DevCon, d2dResource,
		GUID_ContainerFormatBmp, L"Text.bmp");
	d2dResource->Release();*/
#endif

	// Set the blend state for d2d render target texture objects
	mD3d11DevCon->OMSetBlendState(mTransparencyBlendState, nullptr, 0xffffffff);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	XMMATRIX WVP = XMMatrixIdentity();
	CBPerObject cbText;
	cbText.WVP = XMMatrixTranspose(WVP);

	mD3d11DevCon->IASetVertexBuffers(0, 1, &mD2dVertBuffer, &stride, &offset);
	mD3d11DevCon->IASetIndexBuffer(mD2dIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	
	mD3d11DevCon->UpdateSubresource(mBaseCB, 0, nullptr, &cbText, 0, 0);
	mD3d11DevCon->VSSetConstantBuffers(0, 1, &mBaseCB);
	mD3d11DevCon->PSSetShaderResources(0, 1, &mD2dTexture);
	mD3d11DevCon->PSSetSamplers(0, 1, &mBaseSamplerState);

	mD3d11DevCon->RSSetState(mCWcullMode);

	mD3d11DevCon->DrawIndexed(6, 0, 0);
}

void Engine::RenderPlayerInfo()
{
	// Set shaders
	mD3d11DevCon->VSSetShader(mTextVertShader, nullptr, 0);
	mD3d11DevCon->PSSetShader(mTextPixelShader, nullptr, 0);

	// Release the d3d 11 device
	mKeyedMutex11->ReleaseSync(0);

	// Use d3d10.1 device
	mKeyedMutex10->AcquireSync(0, 5);

	// Draw d2d content
	mD2dRenderTarget->BeginDraw();

	// Clear d2d background
	mD2dRenderTarget->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f));

	// Create the string
	std::wostringstream outputString;
	outputString
		<< L"FPS : " << mFps << "\n"
		<< L"X : " << mPlayer->GetSpawnPos().x << " , Y : " << mPlayer->GetSpawnPos().y << " , Z : " << mPlayer->GetSpawnPos().z << "\n"
		<< L"HP : " << mPlayer->GetHP() << "\n"
		<< L"Score : " << mPlayer->GetScore() << "\n"
		<< L"Distance : " << mPlayer->GetPickedDist();
	std::wstring outputText = outputString.str();

	// Set the font color
	D2D1_COLOR_F fontColor = D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f);

	// Set the brush color d2d will use to draw with
	mBrush->SetColor(fontColor);

	// Create the d2d render area
	D2D1_RECT_F layoutRect = D2D1::RectF(0, 0, static_cast<FLOAT>(mWidth), static_cast<FLOAT>(mHeight));

	// Draw the text
	mD2dRenderTarget->DrawText(
		outputText.c_str(),
		wcslen(outputText.c_str()),
		mTextFormat,
		layoutRect,
		mBrush
	);

	mD2dRenderTarget->EndDraw();

	// Release the d3d10.1 device
	mKeyedMutex10->ReleaseSync(1);

	// Use the d3d11 device
	mKeyedMutex11->AcquireSync(1, 5);

#ifdef _DEBUG
	/*ID3D11Resource* d2dResource = nullptr;
	mD2dTexture->GetResource(&d2dResource);
	SaveWICTextureToFile(mD3d11DevCon, d2dResource,
		GUID_ContainerFormatBmp, L"Text.bmp");
	d2dResource->Release();*/
#endif

	// Set the blend state for d2d render target texture objects
	mD3d11DevCon->OMSetBlendState(mTransparencyBlendState, nullptr, 0xffffffff);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	XMMATRIX WVP = XMMatrixIdentity();
	CBPerObject cbText;
	cbText.WVP = XMMatrixTranspose(WVP);

	mD3d11DevCon->IASetVertexBuffers(0, 1, &mD2dVertBuffer, &stride, &offset);
	mD3d11DevCon->IASetIndexBuffer(mD2dIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	mD3d11DevCon->UpdateSubresource(mBaseCB, 0, nullptr, &cbText, 0, 0);
	mD3d11DevCon->VSSetConstantBuffers(0, 1, &mBaseCB);
	mD3d11DevCon->PSSetShaderResources(0, 1, &mD2dTexture);
	mD3d11DevCon->PSSetSamplers(0, 1, &mBaseSamplerState);

	mD3d11DevCon->RSSetState(mCWcullMode);

	mD3d11DevCon->DrawIndexed(6, 0, 0);
}

HRESULT Engine::CompileShader(LPCWSTR shaderFileName, LPCSTR entryPointName, LPCSTR shaderModelName, ID3DBlob** shaderBlob)
{
	HRESULT hr;

	UINT shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef _DEBUG
	shaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* errorBlob = nullptr;

	hr = D3DCompileFromFile(shaderFileName, nullptr, null, entryPointName, shaderModelName, shaderFlags, null, shaderBlob, &errorBlob);

	if (FAILED(hr))
	{
		if (errorBlob)
		{
			ErrorLogger::Log(hr, (char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
			exit(EXIT_FAILURE);
		}

		ErrorLogger::Log(hr, "Direct 3D Compile Shader - Failed");
		exit(EXIT_FAILURE);
	}

	if (errorBlob)
	{
		errorBlob->Release();
	}

	return hr;
}