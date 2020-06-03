#include "Object.h"
#include "Engine.h"
#include "Camera.h"
#include <WICTextureLoader.h>
#include <wincodec.h>
#include <wrl.h>
#include <ScreenGrab.h>

using namespace DirectX;

bool Object::Initalize(const WCHAR shaderFileName[])
{
	HRESULT hr;

	ID3DBlob* vsBuffer;
	ID3DBlob* psBuffer;

	// Compile shaders from shader file
	Engine::CompileShader(shaderFileName, "VS", "vs_5_0", &vsBuffer);
	Engine::CompileShader(shaderFileName, "PS", "ps_5_0", &psBuffer);
	

	// Create the shader objects
	hr = mD3d11Device->CreateVertexShader(
		vsBuffer->GetBufferPointer(),
		vsBuffer->GetBufferSize(),
		nullptr,
		&mVS
	);
	AssertInitialization(hr, "Direct 3D Create Vertex Shader - Failed");

	hr = mD3d11Device->CreatePixelShader(
		psBuffer->GetBufferPointer(),
		psBuffer->GetBufferSize(),
		nullptr,
		&mPS
	);
	AssertInitialization(hr, "Direct 3D Create Pixel Shader - Failed");


	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));

	mModelBufferParts = new ID3D11Buffer*[mModel.GetVertices().size()];

	// Create part of vertex buffer in a model
	for (int num = 0; num < mModel.GetVertices().size(); ++num)
	{
		vertexBufferDesc.ByteWidth = sizeof(Model_::Vertex) * mModel.GetVertices()[num].size();
		vertexBufferData.pSysMem = mModel.GetVertices()[num].data();

		hr = mD3d11Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &mModelBufferParts[num]);
		AssertInitialization(hr, "Direct 3D Create Vertex Buffer - Failed");
	}

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
	hr = mD3d11Device->CreateSamplerState(&sampDesc, &mObjectSamplerState);
	AssertInitialization(hr, "Direct 3D Create Sampler State - Failed");


	// Create the input layout
	if (mModel.GetHasAnim())
	{
		D3D11_INPUT_ELEMENT_DESC layouts[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT numElements = ARRAYSIZE(layouts);

		hr = mD3d11Device->CreateInputLayout(layouts, numElements, vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), &mVertLayout);
		AssertInitialization(hr, "Direct 3D Create Input Layout - Failed");
	}
	else
	{
		D3D11_INPUT_ELEMENT_DESC layouts[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		UINT numElements = ARRAYSIZE(layouts);

		hr = mD3d11Device->CreateInputLayout(layouts, numElements, vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), &mVertLayout);
		AssertInitialization(hr, "Direct 3D Create Input Layout - Failed");
	}

	// Set the input layout
	mD3d11DevCon->IASetInputLayout(mVertLayout);


	// Release buffers
	vsBuffer->Release();
	psBuffer->Release();


	// Create constant buffer
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(cbDesc));
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.ByteWidth = sizeof(CBPerObject);
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = 0;
	cbDesc.MiscFlags = 0;

	hr = mD3d11Device->CreateBuffer(&cbDesc, nullptr, &mCBPerObjectBuffer);
	AssertInitialization(hr, "Direct 3D Create Constant Buffer - Failed");


	// Set the light buffer
	ZeroMemory(&cbDesc, sizeof(cbDesc));

	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.ByteWidth = sizeof(CBPerFrame);
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = 0;
	cbDesc.MiscFlags = 0;

	hr = mD3d11Device->CreateBuffer(&cbDesc, nullptr, &mCBPerFrameBuffer);
	AssertInitialization(hr, "Direct 3D Create Buffer - Failed");

	if (mModel.GetHasAnim())
	{
		cbDesc.ByteWidth = sizeof(CBAnimMat);
		
		hr = mD3d11Device->CreateBuffer(&cbDesc, nullptr, &mAnimMatCB);
		AssertInitialization(hr, "Direct 3D Create Buffer = Failed");
	}

	
	return true;
}

void Object::Setup(ID3D11Device* d3d11Device, ID3D11DeviceContext* d3d11DevCon, Engine* engine)
{
	mD3d11Device = d3d11Device;
	mD3d11DevCon = d3d11DevCon;
	mEngine = engine;

	mWorld = XMMatrixIdentity();
}

HRESULT Object::ImportModel(const CHAR fbxFileName[])
{
	HRESULT hr = mModel.ImportModelFromFile(fbxFileName, mEngine);
	AssertInitialization(hr, "Model Import Model From File - Failed");

	return S_OK;
}

HRESULT Object::ImportTextures()
{
	mModelTextureParts = new ID3D11ShaderResourceView * [mModel.GetVertices().size()];
	for (int num = 0; num < mModel.GetVertices().size(); ++num)
	{
		HRESULT hr = CreateWICTextureFromFile(mD3d11Device, mD3d11DevCon, mTextureFileNames[num].c_str(), nullptr, &(mModelTextureParts[num]), 0);
		AssertInitialization(hr, "Direct 3D Loading Texture File - Failed");
	}

	mTextureFileNames.clear();

	return S_OK;
}

void Object::Update(double deltaTime)
{
	mCurrTimeAnim += deltaTime;

	if (mCurrTimeAnim >= mModel.GetTotalFramesAnim())
	{
		mCurrTimeAnim = 0;
	}
}

void Object::Draw()
{
	mD3d11DevCon->IASetInputLayout(mVertLayout);

	XMMATRIX WVP = mWorld * mEngine->GetCamera()->GetViewMat() * mEngine->GetCamera()->GetProjMat();
	CBPerObject cbPerObj;
	cbPerObj.World = XMMatrixTranspose(mWorld);
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	mD3d11DevCon->UpdateSubresource(mCBPerObjectBuffer, 0, nullptr, &cbPerObj, 0, 0);
	mD3d11DevCon->VSSetConstantBuffers(0, 1, &mCBPerObjectBuffer);

	Light light;
	CBPerFrame constBuffFrame;
	constBuffFrame.Light = light;
	mD3d11DevCon->UpdateSubresource(mCBPerFrameBuffer, 0, nullptr, &constBuffFrame, 0, 0);
	mD3d11DevCon->PSSetConstantBuffers(0, 1, &mCBPerFrameBuffer);


	if (mModel.GetHasAnim())
	{
		CBAnimMat cbAnimMat;
		for (int num = 0; num < mModel.GetAnimKeyFRamePerTime()[0].size(); ++num)
		{
			cbAnimMat.AnimMat[num] = mModel.GetAnimKeyFRamePerTime()[static_cast<int>(mCurrTimeAnim)][num];
		}

		mD3d11DevCon->UpdateSubresource(mAnimMatCB, 0, nullptr, &cbAnimMat, 0, 0);
		mD3d11DevCon->VSSetConstantBuffers(1, 1, &mAnimMatCB);
	}


	mD3d11DevCon->PSSetSamplers(0, 1, &mObjectSamplerState);
	mD3d11DevCon->RSSetState(mEngine->GetNoCullMode());

	mD3d11DevCon->VSSetShader(mVS, nullptr, 0);
	mD3d11DevCon->PSSetShader(mPS, nullptr, 0);

	UINT stride = sizeof(Model_::Vertex);
	UINT offset = 0; 
	
	for (int num = 0; num < mModel.GetVertices().size(); ++num)
	{
		mD3d11DevCon->IASetVertexBuffers(0, 1, &mModelBufferParts[num], &stride, &offset);
		mD3d11DevCon->PSSetShaderResources(0, 1, &mModelTextureParts[num]);
		mD3d11DevCon->Draw(mModel.GetVertices()[num].size(), 0);
	}
}

void Object::CleanUp()
{
	mCBPerObjectBuffer->Release();
	mVS->Release();
	mPS->Release();
	mVertLayout->Release();
	mCBPerFrameBuffer->Release();
	for (int i = 0; i < mModel.GetVertices().size(); ++i)
	{
		mModelTextureParts[i]->Release();
		mModelBufferParts[i]->Release();
	}
	delete[] mModelTextureParts;
	delete[] mModelBufferParts;
	mModel.ReleaseModel();
}

void* Object::operator new(size_t i)
{
	return _aligned_malloc(sizeof(Object), 16);
}

void Object::operator delete(void* p)
{
	_aligned_free(p);
}
