#include "Object.h"
#include "Engine.h"
#include "Camera.h"
#include <WICTextureLoader.h>

using namespace DirectX;

bool Object::Initalize(const WCHAR shaderFileName[])
{
	HRESULT hr;

	ID3DBlob* vsBuffer;
	ID3DBlob* psBuffer;
	ID3DBlob* d2dPSBuffer;

	// Compile shaders from shader file
	CompileShader(shaderFileName, "VS", "vs_5_0", &vsBuffer);
	CompileShader(shaderFileName, "PS", "ps_5_0", &psBuffer);
	CompileShader(shaderFileName, "D2D_PS", "ps_5_0", &d2dPSBuffer);
	

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

	hr = mD3d11Device->CreatePixelShader(
		d2dPSBuffer->GetBufferPointer(),
		d2dPSBuffer->GetBufferSize(),
		nullptr,
		&mD2d_PS
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
	sampDesc.MipLODBias = 3.0f;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0.0f;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create sampler state
	hr = mD3d11Device->CreateSamplerState(&sampDesc, &mObjectSamplerState);
	AssertInitialization(hr, "Direct 3D Create Sampler State - Failed");


	// Create the input layout
	D3D11_INPUT_ELEMENT_DESC layouts[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	UINT numElements = ARRAYSIZE(layouts);

	hr = mD3d11Device->CreateInputLayout(layouts, numElements, vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), &mVertLayout);
	AssertInitialization(hr, "Direct 3D Create Input Layout - Failed");

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

	
	return true;
}

void Object::Setup(ID3D11Device* d3d11Device, ID3D11DeviceContext* d3d11DevCon, Engine* engine)
{
	mD3d11Device = d3d11Device;
	mD3d11DevCon = d3d11DevCon;
	mEngine = engine;
}

HRESULT Object::ImportModel(const CHAR fbxFileName[], const std::vector<std::wstring> textureFileNames)
{
	HRESULT hr = mModel.ImportModelFromFile(fbxFileName);
	AssertInitialization(hr, "Model Import Model From File - Failed");

	// Loading the Texture from a File
	mModelTextureParts = new ID3D11ShaderResourceView*[mModel.GetVertices().size()];
	for (int num = 0; num < mModel.GetVertices().size(); ++num)
	{
		hr = CreateWICTextureFromFile(mD3d11Device, textureFileNames[num].c_str(), nullptr, &(mModelTextureParts[num]), 0);
		AssertInitialization(hr, "Direct 3D Loading Texture File - Failed");
	}

	return S_OK;
}

void Object::Update(double deltaTime)
{

}

void Object::Draw()
{
	Light light;
	CBPerFrame constBuffFrame;
	constBuffFrame.Light = light;
	mD3d11DevCon->UpdateSubresource(mCBPerFrameBuffer, 0, nullptr, &constBuffFrame, 0, 0);
	mD3d11DevCon->PSSetConstantBuffers(0, 1, &mCBPerFrameBuffer);


	// Set vertex & pixel shader
	mD3d11DevCon->VSSetShader(mVS, nullptr, 0);
	mD3d11DevCon->PSSetShader(mPS, nullptr, 0);

	UINT stride = sizeof(Model_::Vertex);
	UINT offset = 0;


	// Set the WVP matrix and send it to the constant buffer in effect file
	XMMATRIX WVP = DirectX::XMMatrixIdentity() * mEngine->GetCamera()->GetViewMat() * mEngine->GetCamera()->GetProjMat();
	CBPerObject cbPerObj;
	cbPerObj.World = XMMatrixTranspose(DirectX::XMMatrixIdentity());
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	mD3d11DevCon->UpdateSubresource(mCBPerObjectBuffer, 0, nullptr, &cbPerObj, 0, 0);
	mD3d11DevCon->VSSetConstantBuffers(0, 1, &mCBPerObjectBuffer);
	mD3d11DevCon->PSSetSamplers(0, 1, &mObjectSamplerState);


	// Draw the first cube
	mD3d11DevCon->RSSetState(mEngine->GetCWcullMode());

	for (int num = 0; num < mModel.GetVertices().size(); ++num)
	{
		mD3d11DevCon->IASetVertexBuffers(0, 1, &mModelBufferParts[num], &stride, &offset);
		mD3d11DevCon->PSSetShaderResources(0, 1, &mModelTextureParts[num]);
		mD3d11DevCon->Draw(mModel.GetVertices()[num].size(), 0);
	}

	mEngine->RenderText(L"FPS : ", static_cast<int>(mEngine->GetFps()));
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

HRESULT Object::CompileShader(LPCWSTR shaderFileName, LPCSTR entryPointName, LPCSTR shaderModelName, ID3DBlob** shaderBlob)
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