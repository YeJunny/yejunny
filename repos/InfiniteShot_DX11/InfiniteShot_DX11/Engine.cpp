#include "Engine.h"

bool Engine::InitializeWindow(HINSTANCE hInstance, int showWnd, bool bIsWindowed, const TCHAR* titleName, const TCHAR* className, INT width, INT height)
{
	if (!WindowManager::InitializeWindow(hInstance, showWnd, bIsWindowed, titleName, className, width, height))
	{
		ErrorLogger::Log(E_FAIL, "Initialize error!");
		exit(EXIT_FAILURE);
	}

	return true;
}

bool Engine::ProcessMessage()
{
	return WindowManager::ProcessMessage();
}

bool Engine::InitializeDirect3d11App(HINSTANCE hInstance)
{
	HRESULT hr;

	//Describe backbuffer
	DXGI_MODE_DESC backbufferDesc;

	ZeroMemory(&backbufferDesc, sizeof(DXGI_MODE_DESC));

	backbufferDesc.Width = mWidth;
	backbufferDesc.Height = mHeight;
	backbufferDesc.RefreshRate.Numerator = 60;
	backbufferDesc.RefreshRate.Denominator = 1;
	backbufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
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
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


	// Creating device & swap chain
	hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		null,
		null,
		null,
		null,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&mSwapChain,
		&mD3d11Device,
		nullptr,
		&mD3d11DevCon
	);

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Direct 3D Create Device & Swap Chain - Failed");
		exit(EXIT_FAILURE);
	}


	// Creating back buffer
	ID3D11Texture2D* backBuffer;

	hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Direct 3D Get Buffer - Failed");
		exit(EXIT_FAILURE);
	}


	// Creating render target
	hr = mD3d11Device->CreateRenderTargetView(backBuffer, nullptr, &mRenderTargetView);

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Direct 3D Create Render Target - Failed");
		exit(EXIT_FAILURE);
	}

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

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Direct 3D Create Depth Stencil Buffer - Failed");
		exit(EXIT_FAILURE);
	}

	hr = mD3d11Device->CreateDepthStencilView(depthStencilBuffer, nullptr, &mDepthStencilView);

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Direct 3D Create Depth Stencil View - Failed");
		exit(EXIT_FAILURE);
	}

	depthStencilBuffer->Release();

	// Binding render target & depth stencil
	mD3d11DevCon->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	return true;
}

void Engine::ReleaseObject() const
{
	// Release the objects
	for (int i = 0; i < mObjects.size(); ++i)
	{
		mObjects[i]->CleanUp();
		delete mObjects[i];
	}
	mObjects.empty();

	// Release the COM Ojbects
	mSwapChain->Release();
	mDepthStencilView->Release();
	mRenderTargetView->Release();
	mD3d11DevCon->Release();
	mD3d11Device->Release();
}

bool Engine::InitScene()
{
	// Create objects
	Object* triangle = new Object();
	mObjects.push_back(triangle);
	for (int i = 0; i < mObjects.size(); ++i)
	{
		if (!mObjects[i]->Initalize(L"Shaders\\Effects.fx", mD3d11Device, mD3d11DevCon))
		{
			ErrorLogger::Log("Init Scene Object Initialize - Failed");
			exit(EXIT_FAILURE);
		}
	}

	// Set primitive topology
	mD3d11DevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(viewport));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = mWidth;
	viewport.Height = mHeight;
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;

	// Set the viewport
	mD3d11DevCon->RSSetViewports(1, &viewport);

	return true;
}

void Engine::UpdateScene()
{
	for (int i = 0; i < mObjects.size(); ++i)
	{
		mObjects[i]->Update();
	}
}

void Engine::DrawScene()
{
	// Clear backbuffer
	float bgColor[] = { 0, 0, 0, 0.0f };
	mD3d11DevCon->ClearRenderTargetView(mRenderTargetView, bgColor);
	mD3d11DevCon->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Draw the triangle
	for (int i = 0; i < mObjects.size(); ++i)
	{
		mObjects[i]->Draw();
	}
	
	// Present the back buffer to the screen
	mSwapChain->Present(0, 0);
}
