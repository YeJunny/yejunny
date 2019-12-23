#include "Axis.h"
#include "Bullet.h"
#include "Camera.h"
#include "Ground.h"
#include "Gun.h"
#include "House.h"
#include "MainLoop.h"
#include "Player.h"
#include "Skybox.h"

MainLoop::MainLoop()
{
}

MainLoop::~MainLoop()
{
	mHouse.reset();
	//mAxis.reset();
	//mSkybox.reset();
	mGround.reset();
	mCamera.reset();
	mPlayer.reset();
}

void MainLoop::Init()
{
	// Init Player
	mPlayer = std::make_unique<Player>();
	mPlayer->Init(mD3DDevice, mhWnd, mProjectionMat, mInput, mTimer);
	
	// Init Camera
	mCamera = std::make_unique<Camera>();
	mCamera->Init(mD3DDevice);

	// Init Object
	/*mAxis = std::make_unique<Axis>();
	mAxis->Init(mD3DDevice, L"Shader\\Axis.fx", mProjectionMat, mTimer);*/

	mHouse = std::make_unique<House>();
	mHouse->Init(mD3DDevice, mhWnd, L"Shader\\House.fx", "Fbx\\Castle.fbx", L"Fbx\\Castle_Interior_Texture.jpg", mProjectionMat, mTimer);

	mSkybox = std::make_unique<Skybox>();
	mSkybox->Init(mD3DDevice, mhWnd, L"Shader\\Box.fx", "Fbx\\skybox.fbx", L"Fbx\\skyboxGood.png", mProjectionMat, mTimer);

	/*mGround = std::make_unique<Ground>();
	mGround->Init(mD3DDevice, mhWnd, L"Shader\\Ground.fx", "Fbx\\sphere3k.fbx", L"Fbx\\wood.png", mProjectionMat, mTimer);*/
}

void MainLoop::Update()
{
	// Player update
	mPlayer->Update(mViewMat);

	// Camera update
	mCamera->UpdateLocation(mPlayer->GetPosition(), mPlayer->GetRotation());
	mViewMat = mCamera->GetCameraViewMatrix();

	// Object update
	//mAxis->Update(mViewMat);
	mHouse->Update(mViewMat);
	//mGround->Update(mViewMat);
	mSkybox->Update(mViewMat);
}

void MainLoop::Render()
{
	float backgroundColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	mD3DContext->ClearRenderTargetView(mRenderTargetView.Get(), backgroundColor);
	mD3DContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	

	// Player(Gun, Bullet, etc...) rendering
	mPlayer->Render();
	//mAxis->Render();


	XMFLOAT3 playerPos = mPlayer->GetPosition();

	// Object rendering
	mHouse->Render(playerPos);
	//mGround->Render(playerPos);
	mSkybox->Render(playerPos);
	

	mSwapChain->Present(0, 0);
}