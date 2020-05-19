#include "Global.h"
#include "../Hardware/Input.h"
#include "./Time/Time.h"


/* Static variables */

ComPtr<ID3D11Device> Global::Device;
ComPtr<ID3D11DeviceContext> Global::Context;
ComPtr<IDXGISwapChain> Global::SwapChain;
ComPtr<ID3D11Texture2D> Global::DepthStencil;
ComPtr<ID3D11RenderTargetView> Global::RenderTargetView;
ComPtr<ID3D11DepthStencilView> Global::DepthStencilView;

HWND Global::Hwnd;
HINSTANCE Global::Hinst;

FLOAT Global::Width;
FLOAT Global::Height;

XMMATRIX Global::ViewMat;
XMMATRIX Global::ProjectionMat;

DWORD Global::Frame;

std::unique_ptr<Input> Global::InputManager;
std::unique_ptr<Time> Global::TimeManager;

XMFLOAT3 Global::PlayerPos;
XMFLOAT3 Global::PlayerRot;



void Global::CreateGlobal()
{
	HRESULT hr;

	RECT rt;
	GetClientRect(Hwnd, &rt);
	Width = static_cast<FLOAT>(rt.right - rt.left);
	Height = static_cast<FLOAT>(rt.bottom - rt.top);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};


	// Create Device, Swap Chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.BufferDesc.Width = static_cast<UINT>(Width);
	swapChainDesc.BufferDesc.Height = static_cast<UINT>(Height);
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = Hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = true;

	hr = D3D11CreateDeviceAndSwapChain(
		nullptr, 
		D3D_DRIVER_TYPE_HARDWARE, 
		NULL, 
		0, 
		featureLevels, 
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc, 
		SwapChain.GetAddressOf(), 
		Device.GetAddressOf(), 
		nullptr, 
		Context.GetAddressOf()
	);
	if (FAILED(hr))
	{
		assert(hr == S_OK);
	}


	// Create BackBuffer
	ID3D11Texture2D* pBackBuffer = nullptr;

	hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
	{
		assert(hr == S_OK);
	}

	hr = Device->CreateRenderTargetView(pBackBuffer, nullptr, RenderTargetView.GetAddressOf());
	if (FAILED(hr))
	{
		assert(hr == S_OK);
	}


	// Create Depth Stencil View
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = static_cast<UINT>(Width);
	descDepth.Height = static_cast<UINT>(Height);
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	hr = Device->CreateTexture2D(&descDepth, NULL, DepthStencil.GetAddressOf());
	if (FAILED(hr))
	{
		assert(hr == S_OK);
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = Device->CreateDepthStencilView(DepthStencil.Get(), &descDSV, DepthStencilView.GetAddressOf());
	if (FAILED(hr))
	{
		assert(hr == S_OK);
	}

	Context->OMSetRenderTargets(1, RenderTargetView.GetAddressOf(), DepthStencilView.Get());


	// Create Veiwport
	D3D11_VIEWPORT vp;
	vp.Width = Width;
	vp.Height = Height;
	vp.MaxDepth = 1.0f;
	vp.MinDepth = 0.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	Context->RSSetViewports(1, &vp);


	//CoInitialize(nullptr);


	// Create Projection Matrix
	ProjectionMat = XMMatrixPerspectiveFovLH(XM_PIDIV4, Width / static_cast<int>(Height), 0.1f, 1000.0f);


	// Other Init
	InputManager = std::make_unique<Input>();
	if (!InputManager->Initialize(static_cast<int>(Width), static_cast<int>(Height)))
	{
		assert(false);
	}

	TimeManager = std::make_unique<Time>();

	PlayerPos = { 0.0f, 4.5f, 0.0f };
	PlayerRot = { 0.0f, 0.0f, 0.0f };

}

void Global::DestroyGlobal()
{
	InputManager->Shutdown();

	InputManager.reset();
	TimeManager.reset();

	DepthStencil.Reset();
	RenderTargetView.Reset();
	DepthStencilView.Reset();

	Context.Reset();
	SwapChain.Reset();
	Device.Reset();
}

HRESULT Global::CompileShaderFromFile(const WCHAR* szFileName, const LPCSTR szEntryPoint, const LPCSTR szShaderModel, ID3DBlob** ppBlobout)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined _DEBUG
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DCompileFromFile(szFileName, NULL, NULL,
		szEntryPoint, szShaderModel, dwShaderFlags,
		0, ppBlobout, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
		{
			char* errorString = (char*)pErrorBlob->GetBufferPointer();
			OutputDebugStringA("===========Shader Error============\n");
			OutputDebugString(szFileName);
			OutputDebugStringA("\n=====================================\n");
			OutputDebugStringA(errorString);
			OutputDebugStringA("=====================================\n");
		}
		if (pErrorBlob) pErrorBlob->Release();
		assert(false);
		return hr;
	}

	if (pErrorBlob)
	{
		pErrorBlob->Release();
	}

	return S_OK;
}


// Getter
ComPtr<ID3D11Device> Global::GetDevice()
{
	return Device;
}
ComPtr<ID3D11DeviceContext> Global::GetContext()
{
	return Context;
}
ComPtr<IDXGISwapChain> Global::GetSwapChain()
{
	return SwapChain;
}
ComPtr<ID3D11RenderTargetView> Global::GetRenderTargetView()
{
	return RenderTargetView;
}
ComPtr<ID3D11DepthStencilView> Global::GetDepthStencilView()
{
	return DepthStencilView;
}
HWND& Global::GetHwnd()
{
	return Hwnd;
}
HINSTANCE& Global::GetHinst()
{
	return Hinst;
}
FLOAT& Global::GetWidth()
{
	return Width;
}
FLOAT& Global::GetHeight()
{
	return Height;
}
XMMATRIX& Global::GetViewMat()
{
	return ViewMat;
}
XMMATRIX& Global::GetProjectionMat()
{
	return ProjectionMat;
}
DWORD& Global::GetFrame()
{
	return Frame;
}
std::unique_ptr<Input>& Global::GetInput()
{
	return InputManager;
}
std::unique_ptr<Time>& Global::GetTime()
{
	return TimeManager;
}
XMFLOAT3& Global::GetPlayerPos()
{
	return PlayerPos;
}
XMFLOAT3& Global::GetPlayerRot()
{
	return PlayerRot;
}

// Setter
void Global::SetHwnd(HWND hWnd)
{
	Hwnd = hWnd;
}
void Global::SetHinst(HINSTANCE hInst)
{
	Hinst = hInst;
}
void Global::SetViewMat(XMMATRIX viewMat)
{
	ViewMat = viewMat;
}
void Global::SetFrame(DWORD frame)
{
	Frame = frame;
}
void Global::SetPlayerPos(XMFLOAT3 playerPos)
{
	PlayerPos = playerPos;
}
void Global::SetPlayerRot(XMFLOAT3 playerRot)
{
	PlayerRot = playerRot;
}