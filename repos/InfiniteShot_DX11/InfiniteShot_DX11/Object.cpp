#include "Object.h"
#include <WICTextureLoader.h>

using namespace DirectX;

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

bool Object::Initalize(const WCHAR shaderFileName[], ID3D11Device* d3d11Device, ID3D11DeviceContext* d3d11DevCon)
{
	HRESULT hr;

	ID3DBlob* vsBuffer;
	ID3DBlob* psBuffer;

	// Compile shaders from shader file
	CompileShader(shaderFileName, "VS", "vs_5_0", &vsBuffer);
	CompileShader(shaderFileName, "PS", "ps_5_0", &psBuffer);

	mD3d11Device = d3d11Device;

	// Create the shader objects
	hr = mD3d11Device->CreateVertexShader(
		vsBuffer->GetBufferPointer(),
		vsBuffer->GetBufferSize(),
		nullptr,
		&mVS
	);

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Direct 3D Create Vertex Shader - Failed");
		exit(EXIT_FAILURE);
	}

	hr = mD3d11Device->CreatePixelShader(
		psBuffer->GetBufferPointer(),
		psBuffer->GetBufferSize(),
		nullptr,
		&mPS
	);

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Direct 3D Create Pixel Shader - Failed");
		exit(EXIT_FAILURE);
	}

	mD3d11DevCon = d3d11DevCon;

	// Set vertex & pixel shader
	mD3d11DevCon->VSSetShader(mVS, nullptr, 0);
	mD3d11DevCon->PSSetShader(mPS, nullptr, 0);

	// Create the vertex buffer
	Vertex v[] =
	{
		// Front Face
		Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f),
		Vertex(1.0f,  1.0f, -1.0f, 1.0f, 0.0f),
		Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f),

		// Back Face (CCW)
		Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f),
		Vertex(1.0f, -1.0f, 1.0f, 0.0f, 1.0f),
		Vertex(1.0f,  1.0f, 1.0f, 0.0f, 0.0f),
		Vertex(-1.0f,  1.0f, 1.0f, 1.0f, 0.0f),

		// Top Face
		Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f),
		Vertex(-1.0f, 1.0f,  1.0f, 0.0f, 0.0f),
		Vertex(1.0f, 1.0f,  1.0f, 1.0f, 0.0f),
		Vertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f),

		// Bottom Face (CCW)
		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f),
		Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		Vertex(1.0f, -1.0f,  1.0f, 0.0f, 0.0f),
		Vertex(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f),

		// Left Face
		Vertex(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f),
		Vertex(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f),
		Vertex(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f),
		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f),

		// Right Face
		Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		Vertex(1.0f,  1.0f, -1.0f, 0.0f, 0.0f),
		Vertex(1.0f,  1.0f,  1.0f, 1.0f, 0.0f),
		Vertex(1.0f, -1.0f,  1.0f, 1.0f, 1.0f),
	};

	// Create index buffer
	DWORD indices[] =
	{
		// Front Face
		0,  1,  2,
		0,  2,  3,

		// Back Face
		4,  5,  6,
		4,  6,  7,

		// Top Face
		8,  9, 10,
		8, 10, 11,

		// Bottom Face
		12, 13, 14,
		12, 14, 15,

		// Left Face
		16, 17, 18,
		16, 18, 19,

		// Right Face
		20, 21, 22,
		20, 22, 23
	};

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * 12 * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData;
	ZeroMemory(&indexBufferData, sizeof(indexBufferData));
	indexBufferData.pSysMem = indices;
	mD3d11Device->CreateBuffer(&indexBufferDesc, &indexBufferData, &mTriIndexBuffer);

	// Set the index buffer
	mD3d11DevCon->IASetIndexBuffer(mTriIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * 24;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = v;

	hr = mD3d11Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &mTrianglesVertBuffer);
	
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Direct 3D Create Vertex Buffer - Failed");
		exit(EXIT_FAILURE);
	}

	// Loading the Texture from a File
	hr = CreateWICTextureFromFile(mD3d11Device, L"Textures\\braynzar.jpg", nullptr, &mCubesTexture, 0);

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Direct 3D Loading Texture File - Failed");
		exit(EXIT_FAILURE);
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
	hr = mD3d11Device->CreateSamplerState(&sampDesc, &mCubesSamplerState);

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Direct 3D Create Sampler State - Failed");
		exit(EXIT_FAILURE);
	}

	// Set the vertex buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	mD3d11DevCon->IASetVertexBuffers(0, 1, &mTrianglesVertBuffer, &stride, &offset);

	// Create the input layout
	D3D11_INPUT_ELEMENT_DESC layouts[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	UINT numElements = ARRAYSIZE(layouts);
	hr = mD3d11Device->CreateInputLayout(layouts, numElements, vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), &mVertLayout);

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Direct 3D Create Input Layout - Failed");
		exit(EXIT_FAILURE);
	}

	// Set the input layout
	mD3d11DevCon->IASetInputLayout(mVertLayout);

	// Release buffers
	vsBuffer->Release();
	psBuffer->Release();

	// Create constant buffer
	XMVECTOR camTarget;
	XMVECTOR camUp;

	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(cbDesc));
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.ByteWidth = sizeof(cbPerObject);
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = 0;
	cbDesc.MiscFlags = 0;

	hr = mD3d11Device->CreateBuffer(&cbDesc, nullptr, &mCBPerObjectBuffer);

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Direct 3D Create Constant Buffer - Failed");
		exit(EXIT_FAILURE);
	}

	mCamPosition = XMVectorSet(0.0f, 3.0f, -9.0f, 0.0f);
	camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	mCamView = XMMatrixLookAtLH(mCamPosition, camTarget, camUp);
	mCamProjection = XMMatrixPerspectiveFovLH(0.4f * XM_PI, 800 / 600.0f, 0.1f, 1000.0f);

	// Create blender state
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(rtbd));

	rtbd.BlendEnable = true;
	rtbd.SrcBlend = D3D11_BLEND_SRC_COLOR;
	rtbd.DestBlend = D3D11_BLEND_DEST_COLOR;
	rtbd.BlendOp = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = rtbd;

	hr = mD3d11Device->CreateBlendState(&blendDesc, &mTransparency);

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Direct 3D Create Blend State - Failed");
		exit(EXIT_FAILURE);
	}

	// Create CCW & CW Culling
	D3D11_RASTERIZER_DESC cmDesc;
	ZeroMemory(&cmDesc, sizeof(cmDesc));

	cmDesc.FillMode = D3D11_FILL_SOLID;
	cmDesc.CullMode = D3D11_CULL_BACK;

	cmDesc.FrontCounterClockwise = true;
	hr = mD3d11Device->CreateRasterizerState(&cmDesc, &mCCWcullMode);

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Direct 3D Create Rasterizer State - Failed");
		exit(EXIT_FAILURE);
	}

	cmDesc.FrontCounterClockwise = false;
	hr = mD3d11Device->CreateRasterizerState(&cmDesc, &mCWcullMode);

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Direct 3D CreateRasterizer State - Failed");
		exit(EXIT_FAILURE);
	}

	return true;
}

void Object::Update()
{
	// Keep the cubes rotating
	static float rot = 0;
	if (rot > XM_2PI)
	{
		rot = 0;
	}
	rot += 0.0005f;

	// Reset cube1World1
	mCube1World = XMMatrixIdentity();

	// Define cube1's world space matrix
	XMVECTOR rotAxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX Rotation = XMMatrixRotationAxis(rotAxis, rot);
	XMMATRIX Translation = XMMatrixTranslation(0.0f, 0.0f, 4.0f);

	// Set cube1's world space using the transformations
	mCube1World =  Translation * Rotation;

	// Reset cube2World
	mCube2World = XMMatrixIdentity();

	// Define cube2's world space matrics
	rotAxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	Rotation = XMMatrixRotationAxis(rotAxis, -rot);
	XMMATRIX Scaling = XMMatrixScaling(1.3f, 1.3f, 1.3f);

	// Set cube2's world space matrix
	mCube2World =  Rotation * Scaling;
}

void Object::Draw()
{
	// "fine=tune" the blending equation
	float blendFactor[] = { 0.1f, 0.1f, 0.1f, 0.5f };

	// Set the default blend state (no blending) for opaque objects
	mD3d11DevCon->OMSetBlendState(nullptr, nullptr, 0xffffffff);

	// Render opaque objects

	// Set the blend state for transparent objects
	mD3d11DevCon->OMSetBlendState(mTransparency, blendFactor, 0xffffffff);

	//*** Transparency Depth Orderings ***//
	// Find which transparent object is further from the camera
	// So we can render the objects in depth order to the render target

	// Find distance from first cube to camera
	XMVECTOR cubePos = XMVectorZero();

	cubePos = XMVector3TransformCoord(cubePos, mCube1World);

	float distX = XMVectorGetX(cubePos) - XMVectorGetX(mCamPosition);
	float distY = XMVectorGetY(cubePos) - XMVectorGetY(mCamPosition);
	float distZ = XMVectorGetZ(cubePos) - XMVectorGetZ(mCamPosition);

	float cube1Dist = distX * distX + distY * distY + distZ * distZ;

	// Find distance from second cube to camera
	cubePos = XMVectorZero();
	cubePos = XMVector3TransformCoord(cubePos, mCube2World);

	distX = XMVectorGetX(cubePos) - XMVectorGetX(mCamPosition);
	distY = XMVectorGetY(cubePos) - XMVectorGetY(mCamPosition);
	distZ = XMVectorGetZ(cubePos) - XMVectorGetZ(mCamPosition);

	float cube2Dist = distX * distX + distY * distY + distZ * distZ;

	// If the first cubes distance is less than the second cube
	if (cube1Dist < cube2Dist)
	{
		// Switch the order in which the cubes are drawn
		XMMATRIX tempMatrix = mCube1World;
		mCube1World = mCube2World;
		mCube2World = tempMatrix;
	}

	// Set the WVP matrix and send it to the constant buffer in effect file
	XMMATRIX WVP = mCube1World * mCamView * mCamProjection;
	cbPerObject cbPerObj;
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	mD3d11DevCon->UpdateSubresource(mCBPerObjectBuffer, 0, nullptr, &cbPerObj, 0, 0);
	mD3d11DevCon->VSSetConstantBuffers(0, 1, &mCBPerObjectBuffer);
	mD3d11DevCon->PSSetShaderResources(0, 1, &mCubesTexture);
	mD3d11DevCon->PSSetSamplers(0, 1, &mCubesSamplerState);

	// Counter clockwise culling first becuase we need the back side of
	// the cube to be rendered first, so the front side can blend with it
	mD3d11DevCon->RSSetState(mCCWcullMode);

	// Draw the first cube
	mD3d11DevCon->DrawIndexed(36, 0, 0);

	mD3d11DevCon->RSSetState(mCWcullMode);
	mD3d11DevCon->DrawIndexed(36, 0, 0);

	WVP = mCube2World * mCamView * mCamProjection;
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	mD3d11DevCon->UpdateSubresource(mCBPerObjectBuffer, 0, nullptr, &cbPerObj, 0, 0);
	mD3d11DevCon->VSSetConstantBuffers(0, 1, &mCBPerObjectBuffer);
	mD3d11DevCon->PSSetShaderResources(0, 1, &mCubesTexture);
	mD3d11DevCon->PSSetSamplers(0, 1, &mCubesSamplerState);

	mD3d11DevCon->RSSetState(mCCWcullMode);

	// Draw the second cube
	mD3d11DevCon->DrawIndexed(36, 0, 0);

	mD3d11DevCon->RSSetState(mCWcullMode);
	mD3d11DevCon->DrawIndexed(36, 0, 0);
}

void Object::CleanUp() const
{
	mTransparency->Release();
	mCCWcullMode->Release();
	mCWcullMode->Release();
	mCBPerObjectBuffer->Release();
	mTrianglesVertBuffer->Release();
	mTriIndexBuffer->Release();
	mVS->Release();
	mPS->Release();
	mVertLayout->Release();
}

