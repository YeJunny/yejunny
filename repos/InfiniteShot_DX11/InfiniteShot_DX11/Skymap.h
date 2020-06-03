#pragma once

#include "Engine.h"

class Skymap
{
public:
	void CreateSpere(int latLines, int longLines, Engine* engine);
	void Update(double deltaTime);
	void Draw();
	void CleanUp();

private:
	// Buffers
	ID3D11Buffer*				mCBPerObjectBuffer;
	ID3D11Buffer*				mSphereIndexBuffer;
	ID3D11Buffer*				mSphereVertBuffer;

	// Shaders
	ID3D11VertexShader*			mSkymap_VS;
	ID3D11PixelShader*			mSkymap_PS;
	
	// Texture
	ID3D11ShaderResourceView*	mSkymapRV;

	// States
	ID3D11DepthStencilState*	mDSLessEqual;
	ID3D11SamplerState*			mSamplerState;

	// Sphere
	int							mNumSphereVertices;
	int							mNumSphereFaces;

	DirectX::XMMATRIX			mSphereWorld;

	// Core
	Engine*						mEngine;
};

