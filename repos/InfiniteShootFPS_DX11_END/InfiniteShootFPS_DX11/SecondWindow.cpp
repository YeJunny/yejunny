#include "Engine.h"
#include "ErrorLogger.h"
#include "SecondWindow.h"

HRESULT SecondWindow::Init(HWND hWnd, Engine* engine)
{
    mEngine = engine;


    DXGI_MODE_DESC backBufferDesc;
    ZeroMemory(&backBufferDesc, sizeof(DXGI_MODE_DESC));

    backBufferDesc.Width = 300;
    backBufferDesc.Height = 600;
    backBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    backBufferDesc.RefreshRate.Numerator = 60;
    backBufferDesc.RefreshRate.Denominator = 1;
    backBufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    backBufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;


    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

    swapChainDesc.BufferDesc = backBufferDesc;
    swapChainDesc.Windowed = true;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Flags = 0;

    IDXGIFactory1* dxgiFactory;
    HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
    AssertInitialization(hr, "Second Window Create DXGI Factory1 - Failed");

    IDXGIAdapter* adapter;
    hr = dxgiFactory->EnumAdapters(1, &adapter);
    AssertInitialization(hr, "Second Window Enum Adapters - Failed");

    hr = D3D11CreateDeviceAndSwapChain(
        adapter,
        D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_UNKNOWN,
        NULL,
        D3D11_CREATE_DEVICE_DEBUG,
        NULL,
        NULL,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &mSwapChain,
        &mD3d11Device,
        nullptr,
        &mD3d11DevCon
    );
    AssertInitialization(hr, "Second Window D3D11 Create Device And Swap Chain - Failed");

    adapter->Release();
    dxgiFactory->Release();


    ID3D11Texture2D* backBuffer;
    
    hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
    AssertInitialization(hr, "Second Window Swap Chain Get Buffer - Failed");

    hr = mD3d11Device->CreateRenderTargetView(backBuffer, nullptr, &mRenderTargetView);
    AssertInitialization(hr, "Second Window Create Render Target View - Failed");

    backBuffer->Release();


    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(viewport));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = static_cast<FLOAT>(300);
    viewport.Height = static_cast<FLOAT>(600);
    viewport.MaxDepth = 1.0f;
    viewport.MinDepth = 0.0f;

    // Set the viewport
    mD3d11DevCon->RSSetViewports(1, &viewport);


    D3D11_RASTERIZER_DESC cmDesc;
    ZeroMemory(&cmDesc, sizeof(cmDesc));

    cmDesc.FillMode = D3D11_FILL_SOLID;
    cmDesc.CullMode = D3D11_CULL_BACK;
    cmDesc.FrontCounterClockwise = false;

    hr = mD3d11Device->CreateRasterizerState(&cmDesc, &mCWCullMode);
    AssertInitialization(hr, "Direct 3D Create Rasterizer State - Failed");


    D3D11_TEXTURE2D_DESC depthStencilDesc;
    depthStencilDesc.Width = 300;
    depthStencilDesc.Height = 600;
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


    InitBuffer();


    return S_OK;
}

HRESULT SecondWindow::InitBuffer()
{
    SimpleVertex vertices[] = {
        XMFLOAT3(0.0f, 0.5f, 1.0f), XMFLOAT3(1.0f, 0.0f, 1.0f),
        XMFLOAT3(-0.5f, 0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f),
        XMFLOAT3(0.5f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)
    };

    UINT indices[] = {
        1, 0, 2
    };

    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));

    bufferDesc.ByteWidth = sizeof(SimpleVertex) * 3;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA subresourceData;
    ZeroMemory(&subresourceData, sizeof(subresourceData));

    subresourceData.pSysMem = vertices;

    HRESULT hr = mD3d11Device->CreateBuffer(&bufferDesc, &subresourceData, &mVertBuffer);
    AssertInitialization(hr, "Second Window Create Buffer - Failed");

    bufferDesc.ByteWidth = sizeof(UINT) * 1;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    subresourceData.pSysMem = indices;

    hr = mD3d11Device->CreateBuffer(&bufferDesc, &subresourceData, &mIndexBuffer);
    AssertInitialization(hr, "Second Window Create Buffer - Failed");


    ID3DBlob* vsBlob;
    ID3DBlob* psBlob;

    hr = Engine::CompileShader(L"Shaders\\OnlyColorEffects.fx", "VS", "vs_5_0", &vsBlob);
    AssertInitialization(hr, "Second Window CompileShader - Failed");
    hr = Engine::CompileShader(L"Shaders\\OnlyColorEffects.fx", "PS", "ps_5_0", &psBlob);
    AssertInitialization(hr, "Second Window CompileShader - Failed");

    hr = mD3d11Device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &mVS);
    AssertInitialization(hr, "Second Window CreateVertexShader - Failed");
    hr = mD3d11Device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &mPS);
    AssertInitialization(hr, "Second Window CreatePixelShader - Failed");


    D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    hr = mD3d11Device->CreateInputLayout(layoutDesc, ARRAYSIZE(layoutDesc), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &mInputLayout);
    AssertInitialization(hr, "Second Window CreateInputLayout - Failed");


    vsBlob->Release();
    psBlob->Release();
    

    return S_OK;
}

void SecondWindow::Update()
{
}

void SecondWindow::Draw()
{
    FLOAT color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    mD3d11DevCon->ClearRenderTargetView(mRenderTargetView, color);
    mD3d11DevCon->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    mD3d11DevCon->OMSetBlendState(nullptr, null, 0xffffffff);
    mD3d11DevCon->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

    UINT strides = sizeof(SimpleVertex);
    UINT offset = 0;

    mD3d11DevCon->IASetInputLayout(mInputLayout);
    mD3d11DevCon->IASetVertexBuffers(0, 1, &mVertBuffer, &strides, &offset);
    mD3d11DevCon->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    mD3d11DevCon->VSSetShader(mVS, nullptr, 0);
    mD3d11DevCon->PSSetShader(mPS, nullptr, 0);


    //mD3d11DevCon->OMSetBlendState();
    mD3d11DevCon->RSSetState(mCWCullMode);

    mD3d11DevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    mD3d11DevCon->DrawIndexed(3, 0, 0);

    mSwapChain->Present(0, 0);
}
