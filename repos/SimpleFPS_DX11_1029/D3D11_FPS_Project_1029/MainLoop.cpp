#include "Bullet.h"
#include "Camera.h"
#include "Ground.h"
#include "Gun.h"
#include "MainLoop.h"
#include "Player.h"
#include "Skybox.h"

MainLoop::MainLoop()
{
}

MainLoop::~MainLoop()
{
	mSkybox.reset();
	mGround.reset();
	mCamera.reset();
	mPlayer.reset();
}

void MainLoop::Init()
{
	// Init Player
	mPlayer = std::make_unique<Player>();
	mPlayer->Init(mD3DDevice, mhWnd, mViewMat, mProjectionMat, mInput, mTimer);
	
	// Init Camera
	mCamera = std::make_unique<Camera>();
	mCamera->Init(mD3DDevice, mViewMat);

	// Init Object
	mSkybox = std::make_unique<Skybox>();
	mSkybox->Init(mD3DDevice, mhWnd, L"Shader\\Box.fx", L"Fbx\\skybox.png", mProjectionMat, mTimer);

	mGround = std::make_unique<Ground>();
	mGround->Init(mD3DDevice, mhWnd, L"Shader\\Box.fx", L"Fbx\\book-texture.png", mProjectionMat, mTimer);
}

void MainLoop::Update()
{
	// Player update
	mPlayer->Update(mViewMat);
	XMFLOAT3 playerPos = mPlayer->GetPosition();
	XMFLOAT3 playerRot = mPlayer->GetRotation();

	// Camera update
	mCamera->UpdateLocation(playerPos, playerRot);
	mViewMat = mCamera->GetViewMatrix();

	// Object update
	mSkybox->Update(mViewMat);
	mGround->Update(mViewMat);
}

void MainLoop::Render()
{
	float backgroundColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	mD3DContext->ClearRenderTargetView(mRenderTargetView.Get(), backgroundColor);
	mD3DContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	
	mGround->Render();
	mSkybox->Render();
	mPlayer->Render();
	
	mSwapChain->Present(0, 0);
}