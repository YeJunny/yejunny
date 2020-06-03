#include "Camera.h"
#include "Skymap.h"
#include <DDSTextureLoader.h>

void Skymap::CreateSpere(int latLines, int longLines, Engine* engine)
{
    mEngine = engine;


    mNumSphereVertices = ((latLines - 2) * longLines) + 2;
    mNumSphereFaces = ((latLines - 3) * (longLines) * 2) + (longLines * 2);


    float sphereYaw = 0.0f;
    float spherePitch = 0.0f;


    std::vector<Vertex> vertices;
    vertices.reserve(mNumSphereVertices);

    DirectX::XMVECTOR currVertPos = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    vertices.push_back(Vertex(0.0f, 0.0f, 1.0f, 0, 0, 0, 0, 0));


    for (DWORD i = 0; i < latLines - 2; ++i)
    {
        spherePitch = (i + 1) * (3.14 / (latLines - 1));
        DirectX::XMMATRIX Rotationx = DirectX::XMMatrixRotationX(spherePitch);
        for (DWORD j = 0; j < longLines; ++j)
        {
            sphereYaw = j * (6.28 / (longLines));
            DirectX::XMMATRIX Rotationy = DirectX::XMMatrixRotationZ(sphereYaw);
            currVertPos = DirectX::XMVector3TransformNormal(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (Rotationx * Rotationy));
            currVertPos = DirectX::XMVector3Normalize(currVertPos);
            vertices.push_back(Vertex(DirectX::XMVectorGetX(currVertPos), DirectX::XMVectorGetY(currVertPos), DirectX::XMVectorGetZ(currVertPos), 0, 0, 0, 0, 0));
        }
    }

    vertices.push_back(Vertex(0.0f, 0.0f, -1.0f, 0, 0, 0, 0, 0));


    D3D11_BUFFER_DESC vertexBufferDesc;
    ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * mNumSphereVertices;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;


    D3D11_SUBRESOURCE_DATA vertexBufferData;
    ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
    vertexBufferData.pSysMem = &vertices[0];
    HRESULT hr = engine->GetD3d11Deivce()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &mSphereVertBuffer);
    AssertInitialization(hr, "Direct 3D Create Buffer - Failed");


    std::vector<DWORD> indices(mNumSphereFaces * 3);

    int k = 0;
    for (DWORD l = 0; l < longLines - 1; ++l)
    {
        indices[k] = 0;
        indices[k + 1] = l + 1;
        indices[k + 2] = l + 2;
        k += 3;
    }

    indices[k] = 0;
    indices[k + 1] = longLines;
    indices[k + 2] = 1;
    k += 3;

    for (DWORD i = 0; i < latLines - 3; ++i)
    {
        for (DWORD j = 0; j < longLines - 1; ++j)
        {
            indices[k] = i * longLines + j + 1;
            indices[k + 1] = i * longLines + j + 2;
            indices[k + 2] = (i + 1) * longLines + j + 1;

            indices[k + 3] = (i + 1) * longLines + j + 1;
            indices[k + 4] = i * longLines + j + 2;
            indices[k + 5] = (i + 1) * longLines + j + 2;

            k += 6; // next quad
        }

        indices[k] = (i * longLines) + longLines;
        indices[k + 1] = (i * longLines) + 1;
        indices[k + 2] = ((i + 1) * longLines) + longLines;

        indices[k + 3] = ((i + 1) * longLines) + longLines;
        indices[k + 4] = (i * longLines) + 1;
        indices[k + 5] = ((i + 1) * longLines) + 1;

        k += 6;
    }

    for (DWORD l = 0; l < longLines - 1; ++l)
    {
        indices[k] = mNumSphereVertices - 1;
        indices[k + 1] = (mNumSphereVertices - 1) - (l + 1);
        indices[k + 2] = (mNumSphereVertices - 1) - (l + 2);
        k += 3;
    }

    indices[k] = mNumSphereVertices - 1;
    indices[k + 1] = (mNumSphereVertices - 1) - longLines;
    indices[k + 2] = mNumSphereVertices - 2;


    D3D11_BUFFER_DESC indexBufferDesc;
    ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(DWORD) * mNumSphereFaces * 3;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = &indices[0];

    engine->GetD3d11Deivce()->CreateBuffer(&indexBufferDesc, &iinitData, &mSphereIndexBuffer);


    // Create shaders
    ID3DBlob* SKYMAP_VS_Buffer;
    ID3DBlob* SKYMAP_PS_Buffer;

    hr = Engine::CompileShader(L"Shaders\\SkymapEffects.fx", "SKYMAP_VS", "vs_5_0", &SKYMAP_VS_Buffer);
    AssertInitialization(hr, "Direct 3D Compile Shader - Failed");

    hr = Engine::CompileShader(L"Shaders\\SkymapEffects.fx", "SKYMAP_PS", "ps_5_0", &SKYMAP_PS_Buffer);
    AssertInitialization(hr, "Direct 3D Compile Shader - Failed");

    hr = engine->GetD3d11Deivce()->CreateVertexShader(SKYMAP_VS_Buffer->GetBufferPointer(), SKYMAP_VS_Buffer->GetBufferSize(), nullptr, &mSkymap_VS);
    AssertInitialization(hr, "Direct 3D Create Vertex Shader - Failed");

    hr = engine->GetD3d11Deivce()->CreatePixelShader(SKYMAP_PS_Buffer->GetBufferPointer(), SKYMAP_PS_Buffer->GetBufferSize(), nullptr, &mSkymap_PS);
    AssertInitialization(hr, "Direct 3D Create Vertex Shader - Failed");

    SKYMAP_VS_Buffer->Release();
    SKYMAP_PS_Buffer->Release();


    // Create cubemap texture
    ID3D11Texture2D* skymapTex;

    hr = DirectX::CreateDDSTextureFromFileEx(engine->GetD3d11Deivce(), L"Textures\\Skymap\\Skymap.dds", 0,
        D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, D3D11_RESOURCE_MISC_TEXTURECUBE,
        false, reinterpret_cast<ID3D11Resource**>(&skymapTex), nullptr);
    AssertInitialization(hr, "Direct 3D Create DDS Texture From File Ex - Failed");
    
    D3D11_TEXTURE2D_DESC skymapTexDesc;
    skymapTex->GetDesc(&skymapTexDesc);

    D3D11_SHADER_RESOURCE_VIEW_DESC skymapViewDesc;
    skymapViewDesc.Format = skymapTexDesc.Format;
    skymapViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    skymapViewDesc.TextureCube.MipLevels = skymapTexDesc.MipLevels;
    skymapViewDesc.TextureCube.MostDetailedMip = 0;

    hr = engine->GetD3d11Deivce()->CreateShaderResourceView(reinterpret_cast<ID3D11Resource*>(skymapTex), &skymapViewDesc, &mSkymapRV);
    AssertInitialization(hr, "Direct 3D Create Shader Resource View - Failed");

    skymapTex->Release();


    // Create depth stencil state
    D3D11_DEPTH_STENCIL_DESC dssDesc;
    ZeroMemory(&dssDesc, sizeof(dssDesc));
    dssDesc.DepthEnable = true;
    dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

    engine->GetD3d11Deivce()->CreateDepthStencilState(&dssDesc, &mDSLessEqual);


    // Create constant buffer
    D3D11_BUFFER_DESC cbDesc;
    ZeroMemory(&cbDesc, sizeof(cbDesc));
    cbDesc.Usage = D3D11_USAGE_DEFAULT;
    cbDesc.ByteWidth = sizeof(CBPerObject);
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = 0;
    cbDesc.MiscFlags = 0;

    hr = mEngine->GetD3d11Deivce()->CreateBuffer(&cbDesc, nullptr, &mCBPerObjectBuffer);
    AssertInitialization(hr, "Direct 3D Create Constant Buffer - Failed");


    // Describe the sampler
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.MipLODBias = 0.0f;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0.0f;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;


    // Create sampler state
    hr = mEngine->GetD3d11Deivce()->CreateSamplerState(&sampDesc, &mSamplerState);
    AssertInitialization(hr, "Direct 3D Create Sampler State - Failed");
}

void Skymap::Update(double deltaTime)
{
    mSphereWorld = DirectX::XMMatrixIdentity();

    DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(5.0f, 5.0f, 5.0f);
    XMVECTOR camPos = mEngine->GetCamera()->GetPosMat();
    XMMATRIX translation = DirectX::XMMatrixTranslation(XMVectorGetX(camPos), XMVectorGetY(camPos), XMVectorGetZ(camPos));

    mSphereWorld = scale * translation;
}

void Skymap::Draw()
{
    ID3D11DeviceContext* d3d11DevCon = mEngine->GetD3d11DevCon();

    UINT strides = sizeof(Vertex);
    UINT offset = 0;
    
    CBPerObject cbPerObj;
    XMMATRIX WVP = mSphereWorld * mEngine->GetCamera()->GetViewMat() * mEngine->GetCamera()->GetProjMat();
    cbPerObj.WVP = XMMatrixTranspose(WVP);
    cbPerObj.World = XMMatrixTranspose(mSphereWorld);

    d3d11DevCon->IASetIndexBuffer(mSphereIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    d3d11DevCon->IASetVertexBuffers(0, 1, &mSphereVertBuffer, &strides, &offset);
    
    d3d11DevCon->UpdateSubresource(mCBPerObjectBuffer, 0, nullptr, &cbPerObj, 0, 0);
    d3d11DevCon->VSSetConstantBuffers(0, 1, &mCBPerObjectBuffer);

    d3d11DevCon->PSSetShaderResources(0, 1, &mSkymapRV);
    d3d11DevCon->PSSetSamplers(0, 1, &mSamplerState);

    d3d11DevCon->VSSetShader(mSkymap_VS, nullptr, 0);
    d3d11DevCon->PSSetShader(mSkymap_PS, nullptr, 0);

    d3d11DevCon->OMSetBlendState(mEngine->GetTransparencyBlendState(), nullptr, 0xffffffff);
    d3d11DevCon->OMSetDepthStencilState(mDSLessEqual, 0);
    d3d11DevCon->RSSetState(mEngine->GetNoCullMode());

    d3d11DevCon->DrawIndexed(mNumSphereFaces * 3, 0, 0);

    d3d11DevCon->OMSetDepthStencilState(nullptr, 0);
}

void Skymap::CleanUp()
{
	mSphereIndexBuffer->Release();
	mSphereVertBuffer->Release();
	mSkymap_VS->Release();
	mSkymap_PS->Release();
	mSkymapRV->Release();
	mDSLessEqual->Release();
}
