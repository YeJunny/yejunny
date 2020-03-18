#include "../Objects/Player/Bullet.h"
#include "../Objects/Camera/Camera.h"
#include "../Objects/Enemy/Enemy.h"
#include "../Objects/Player/Gun.h"
#include "../Objects/Castle/Castle.h"
#include "../Hardware/Input.h"
#include "MainLoopManager.h"
#include "../Objects/Player/Player.h"
#include "../Objects/Others/Skybox.h"
using namespace std;

MainLoopManager::MainLoopManager()
{
}

MainLoopManager::~MainLoopManager()
{
	mHouse.reset();
	mSkybox.reset();
	mCamera.reset();
	mPlayer.reset();
	mEnemy.reset();
}

void MainLoopManager::Init()
{
	/* Init Player */

	mPlayer = make_unique<Player>();
	mPlayer->Init();


	/* Init Camera */

	mCamera = make_unique<Camera>();
	mCamera->Init();


	/* Init Objects */

	{
		mHouse = make_unique<Castle>();
		WCHAR textureFiles[][TEXTURE_LEN] =
		{
			L"Fbx\\Castle_Exterior_Texture.jpg",
			L"Fbx\\Castle_TowersDoors_Windows_Texture.jpg",
			L"Fbx\\Castle_Ground_Fountain_Texture.jpg",
			L"Fbx\\Castle_Interior_Texture.jpg",
		};
		mHouse->Init(L"Shader\\House.fx", "Fbx\\Castle.fbx", textureFiles);
	}
	{
		mSkybox = make_unique<Skybox>();
		WCHAR textureFiles[][TEXTURE_LEN] = { L"Fbx\\skyboxGood.png" };
		mSkybox->Init(L"Shader\\Box.fx", "Fbx\\skybox.fbx", textureFiles);
	}
	{
		mEnemy = make_unique<Enemy>();
		WCHAR textureFiles[][TEXTURE_LEN] = { L"Fbx\\Enemy.jpg" };
		mEnemy->Init(L"Shader\\Enemy.fx", "Fbx\\Enemy.fbx", textureFiles);
	}
}

void MainLoopManager::Update()
{
	unique_ptr<Time>& timeManager = Global::GetTime();
	timeManager->Update();
	ULONGLONG diffTick = timeManager->GetDiffTick();


	/* Calculate FPS */

	DWORD& frame = Global::GetFrame();
	static ULONGLONG cumulativeTime = 0;
	cumulativeTime += diffTick;
	if (cumulativeTime >= 1000)
	{
		WCHAR str[100];
		wsprintf(str, L"FPS: %d\n", frame);
		frame = 0;
		cumulativeTime = 0;
		OutputDebugStringW(str);
	}

	Sleep(10);
	std::unique_ptr<Input>& inputManager = Global::GetInput();
	inputManager->Frame();

	while (diffTick > 0)
	{
		ULONGLONG deltaTime = min(diffTick, 1.0f / 60 * 1000);
		diffTick -= deltaTime;


		/* Player update */

		mPlayer->Update();


		/* Camera update */

		mCamera->UpdateLocation();


		/* Objects update */

		mHouse->Update();
		mSkybox->Update();
		mEnemy->Update();
	}
}

void MainLoopManager::Render()
{
	ComPtr<ID3D11DeviceContext> Context = Global::GetContext();

	float backgroundColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	Context->ClearRenderTargetView(Global::GetRenderTargetView().Get(), backgroundColor);
	Context->ClearDepthStencilView(Global::GetDepthStencilView().Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);


	/* Player(Gun, Bullet, etc...) Rendering */

	mPlayer->Render();


	/* Objects Rendering */

	mHouse->Render();
	mSkybox->Render();
	mEnemy->Render();


	/* Draw */

	Global::GetSwapChain()->Present(0, 0);


	++Global::GetFrame();
}