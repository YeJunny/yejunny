#include "Camera.h"
#include "InputManager.h"
#include "Monster.h"
#include "MonsterManager.h"
#include "Player.h"
#include "SoundManager.h"
#include "Utilities.h"
#include <WICTextureLoader.h>

void Player::Shoot()
{
	if (mAccTime >= BOUND_TIME)
	{
		float tempDist;
		float closestDist = FLT_MAX;
		int hitIndex;

		const int pointX = mEngine->GetClientWidth() / 2;
		const int pointY = mEngine->GetClientHeight() / 2;

		XMVECTOR prwsPos, prwsDir;

		ConvertMouseToRay(pointX, pointY, &prwsPos, &prwsDir);

		XMFLOAT3 pos, dir;
		XMStoreFloat3(&pos, prwsPos);
		XMStoreFloat3(&dir, prwsDir);

		std::vector<Monster*> monsters = mEngine->GetMonsterManager()->GetMonsters();

		for (auto iter = monsters.begin(); iter != monsters.end(); ++iter)
		{
			for (int index = 0; index < (*iter)->GetNumMonster(); ++index)
			{
				if ((*iter)->GetMonHP(index) > 0)
				{
					tempDist = (*iter)->Picked(index, prwsPos, prwsDir);
					if (tempDist < closestDist)
					{
						closestDist = tempDist;
						hitIndex = index;
					}
				}
			}

			if (closestDist < FLT_MAX)
			{
				(*iter)->SubHP(hitIndex, 1);

				mPickedDist = closestDist;
				
				if ((*iter)->IsDeadMonster(hitIndex))
				{
					mScore += 10;
				}

				break;
			}
		}

		mAccTime = 0;
		mbIsShoot = true;
		mEngine->GetSoundManager()->PlayWaveFile(mWeaponName);
	}
}

void Player::ChangeWeapon()
{
	
}

void Player::Init(Engine* engine)
{
	mEngine = engine;

	
	json playerJson = Utilities::ParseJsonFile("Settings/Player.json");

	std::vector<float> spawnPos = playerJson["SpawnPosition"].get<std::vector<float>>();
	std::string damageImagePath = playerJson["DamageImagePath"];
	std::string deadImagePath = playerJson["DeadImagePath"];

	mWeaponName = playerJson["Weapon"];

	mSpawnPos.x = spawnPos[0];
	mSpawnPos.y = spawnPos[1];
	mSpawnPos.z = spawnPos[2];

	InitWeapon();
	InitDamageImage(damageImagePath);
	InitDeadImage(deadImagePath);
}

void Player::Update(double deltaTime)
{
	XMVECTOR camRight = mEngine->GetCamera()->GetCamRight();
	XMVECTOR camForward = mEngine->GetCamera()->GetCamForward();
	XMVECTOR camPos = mEngine->GetCamera()->GetCamPos();

	float camPitch = mEngine->GetCamera()->GetCamPitch();
	float camYaw = mEngine->GetCamera()->GetCamYaw();

	XMStoreFloat3(&mSpawnPos, mEngine->GetCamera()->GetCamPos());


	// Weapon
	XMVECTOR upVec = XMVectorSet(0.0f, -mWeaponPos.y, 0.0f, 0.0f);
	XMVECTOR rightVec = mWeaponPos.x * camRight;
	XMVECTOR forwardVec = mWeaponPos.z * camForward;

	XMVECTOR weaponPos = camPos + upVec + rightVec + forwardVec;
	XMMATRIX weaponRot = XMMatrixRotationRollPitchYaw(camPitch - XM_PI, camYaw, 0);
	XMMATRIX weaponScale = XMMatrixScaling(mWeaponScale, mWeaponScale, mWeaponScale);

	mWorldWeapon = weaponScale * weaponRot * XMMatrixTranslationFromVector(weaponPos);

	mWeapon->SetMatWorld(mWorldWeapon);


	// Muzzle Flash
	upVec = XMVectorSet(0.0f, -mMuzzleflashPos.y, 0.0f, 0.0f);
	rightVec = mMuzzleflashPos.x * camRight;
	forwardVec = mMuzzleflashPos.z * camForward;

	XMVECTOR muzzleFlashPos = camPos + upVec + rightVec + forwardVec;
	XMMATRIX muzzleFlashRot = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);
	XMMATRIX muzzleFlashScale = XMMatrixScaling(mMuzzleflashScale, mMuzzleflashScale, mMuzzleflashScale);

	mWorldMuzzleflash = muzzleFlashScale * XMMatrixTranslationFromVector(muzzleFlashPos);


	if (!mbIsAttacked)
	{
		std::vector<Monster*> monsters = mEngine->GetMonsterManager()->GetMonsters();
		for (auto iter = monsters.begin(); iter != monsters.end(); ++iter)
		{
			float attackTime = (*iter)->GetAttackTimeAnim();

			for (int i = 0; i < (*iter)->GetNumMonster(); ++i)
			{
				if ((*iter)->GetPlayerDist(i) <= 2.0f && (*iter)->GetCurrTimeAnim() >= attackTime)
				{
					--mHP;
					mbIsAttacked = true;
					mAlpha = 1.0f;
				}
			}
		}
	}
	if (mbIsAttacked)
	{
		mAlpha -= deltaTime;

		if (mAlpha <= 0.0f)
		{
			mbIsAttacked = false;
		}
	}


	if (mHP <= 0)
	{
		mbIsDead = true;
		mAlpha = 0.0f;
	}


	mAccTime += deltaTime;
}

void Player::Draw()
{
	mWeapon->Draw();

	if (mbIsDead)
	{
		DrawDeadImage();
	}
	else
	{
		DrawDamageImage();
	}
	
	DrawAim();

	DrawMuzzleflash();
}

void Player::CleanUp()
{
	mWeapon->CleanUp();
	mTexAim->Release();
	mCBAim->Release();
	mTexMuzzleflash->Release();
	mCBMuzzleflash->Release();
	mWeapon->CleanUp();
	mBloodTex->Release();
	mGameoverTex->Release();
	mAlphaCB->Release();
	delete mWeapon;
}

void Player::UpdateDeadImage(double deltaTime)
{
	InputManager* inputManager = mEngine->GetInputManager();

	inputManager->Update();
	if (inputManager->IsPressKey(DIK_R))
	{
		std::vector<Monster*> monsters = mEngine->GetMonsterManager()->GetMonsters();
		for (auto iter = monsters.begin(); iter != monsters.end(); ++iter)
		{
			(*iter)->SpawnMonsters();
		}

		mEngine->GetCamera()->ResetCamera();

		mbIsDead = false;
		mbIsAttacked = false;
		mHP = 50;
		mScore = 0;

		return;
	}

	mAlpha += deltaTime;
}

void Player::ConvertMouseToRay(int mouseX, int mouseY, XMVECTOR* pos, XMVECTOR* dir)
{
	XMVECTOR pickRayInViewSpaceDir = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR pickRayInViewSpacePos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	float PRVecX, PRVecY, PRVecZ;

	XMFLOAT4X4 projection;
	XMStoreFloat4x4(&projection, mEngine->GetCamera()->GetProjMat());

	//Transform 2D pick position on screen space to 3D ray in View space
	PRVecX = (((2.0f * mouseX) / mEngine->GetClientWidth()) - 1) / projection._11;
	PRVecY = -(((2.0f * mouseY) / mEngine->GetClientHeight()) - 1) / projection._22;
	PRVecZ = 1.0f;    //View space's Z direction ranges from 0 to 1, so we set 1 since the ray goes "into" the screen

	pickRayInViewSpaceDir = XMVectorSet(PRVecX, PRVecY, PRVecZ, 0.0f);

	//Uncomment this line if you want to use the center of the screen (client area)
	//to be the point that creates the picking ray (eg. first person shooter)
	//pickRayInViewSpaceDir = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// Transform 3D Ray from View space to 3D ray in World space
	XMMATRIX pickRayToWorldSpaceMatrix;
	XMVECTOR matInvDeter;    //We don't use this, but the xna matrix inverse function requires the first parameter to not be null

	pickRayToWorldSpaceMatrix = XMMatrixInverse(&matInvDeter, mEngine->GetCamera()->GetViewMat());    //Inverse of View Space matrix is World space matrix

	*pos = XMVector3TransformCoord(pickRayInViewSpacePos, pickRayToWorldSpaceMatrix);
	*dir = XMVector3TransformNormal(pickRayInViewSpaceDir, pickRayToWorldSpaceMatrix);
}

void Player::InitWeapon()
{
	std::string weaponPath = "Settings/Object/Weapon/" + mWeaponName + ".json";
	json modelJson = Utilities::ParseJsonFile(weaponPath.c_str());


	// Weapon
	auto weapon = modelJson["Weapon"];
	std::vector<float> weaponPos = weapon["Position"].get<std::vector<float>>();
	mWeaponScale = weapon["Scale"];
	std::string type = "Weapon";
	std::string shaderPath = weapon["ShaderPath"];
	std::string modelPath = weapon["ModelPath"];
	std::string soundShootPath = weapon["SoundPath"]["Shoot"];
	std::vector<std::string> texturesPath = weapon["TexturesPath"].get<std::vector<std::string>>();

	mWeaponPos.x = weaponPos[0];
	mWeaponPos.y = weaponPos[1];
	mWeaponPos.z = weaponPos[2];

	bool bIsSucess = mEngine->GetSoundManager()->AddSound(mWeaponName, soundShootPath.c_str());
	AssertIsSuccess(bIsSucess, "Direct Sound Init - Failed");

	mWeapon = new Object();
	mWeapon->Init(mWeaponName, type, modelPath, texturesPath, shaderPath, mEngine);
	texturesPath.clear();


	// Muzzle flash
	auto muzzleflash = modelJson["Muzzleflash"];
	std::vector<float> muzzleflashPos = muzzleflash["Position"].get<std::vector<float>>();
	mMuzzleflashScale = muzzleflash["Scale"];
	texturesPath = muzzleflash["TexturesPath"].get< std::vector<std::string>>();

	mMuzzleflashPos.x = muzzleflashPos[0];
	mMuzzleflashPos.y = muzzleflashPos[1];
	mMuzzleflashPos.z = muzzleflashPos[2];

	for (int i = 0; i < texturesPath.size(); ++i)
	{
		HRESULT hr = CreateWICTextureFromFile(mEngine->GetD3d11Deivce(), StringConverter::StringToWide(texturesPath[i]).c_str(), nullptr, &mTexMuzzleflash, 0);
		AssertInitialization(hr, "Direct 3D Create WIC Texture From File - Failed");
	}
	texturesPath.clear();

	D3D11_BUFFER_DESC bdDesc;
	ZeroMemory(&bdDesc, sizeof(bdDesc));
	bdDesc.ByteWidth = sizeof(CBPerObject);
	bdDesc.Usage = D3D11_USAGE_DEFAULT;
	bdDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bdDesc.CPUAccessFlags = 0;
	bdDesc.MiscFlags = 0;

	HRESULT hr = mEngine->GetD3d11Deivce()->CreateBuffer(&bdDesc, nullptr, &mCBMuzzleflash);
	AssertInitialization(hr, "Direct 3D Create Buffer - Failed");

	
	// Aim
	auto aim = modelJson["Aim"];
	texturesPath = aim["TexturesPath"].get< std::vector<std::string>>();

	for (int i = 0; i < texturesPath.size(); ++i)
	{
		hr = CreateWICTextureFromFile(mEngine->GetD3d11Deivce(), StringConverter::StringToWide(texturesPath[i]).c_str(), nullptr, &mTexAim, 0);
		AssertInitialization(hr, "Direct 3D Create WIC Texture From File - Failed");
	}
	texturesPath.clear();

	mWorldAim = XMMatrixScaling(0.1f, 0.1f, 0.1f);

	hr = mEngine->GetD3d11Deivce()->CreateBuffer(&bdDesc, nullptr, &mCBAim);
	AssertInitialization(hr, "Direct 3D Create Buffer - Failed");
}

void Player::InitDamageImage(std::string damageImagePath)
{
	HRESULT hr = CreateWICTextureFromFile(mEngine->GetD3d11Deivce(), StringConverter::StringToWide(damageImagePath).c_str(), nullptr, &mBloodTex, 0);
	AssertInitialization(hr, "Direct 3D Loading Texture File - Failed");

	D3D11_BUFFER_DESC bdDesc;
	ZeroMemory(&bdDesc, sizeof(bdDesc));
	bdDesc.ByteWidth = sizeof(CBPerObject);
	bdDesc.Usage = D3D11_USAGE_DEFAULT;
	bdDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bdDesc.CPUAccessFlags = 0;
	bdDesc.MiscFlags = 0;

	hr = mEngine->GetD3d11Deivce()->CreateBuffer(&bdDesc, nullptr, &mAlphaCB);
	AssertInitialization(hr, "Direct 3D Create Buffer - Failed");
}

void Player::InitDeadImage(std::string deadImagePath)
{
	HRESULT hr = CreateWICTextureFromFile(mEngine->GetD3d11Deivce(), StringConverter::StringToWide(deadImagePath).c_str(), nullptr, &mGameoverTex, 0);
	AssertInitialization(hr, "Direct 3D Loading Texture File - Failed");
}

void Player::DrawAim() const
{
	ID3D11DeviceContext* d3d11DevCon = mEngine->GetD3d11DevCon();

	UINT strides = sizeof(Vertex);
	UINT offset = 0;

	CBPerObject cbPerObject;
	cbPerObject.World = XMMatrixTranspose(mWorldAim);
	cbPerObject.WVP = XMMatrixTranspose(mWorldAim);

	d3d11DevCon->OMSetBlendState(mEngine->GetTransparencyBlendState(), nullptr, 0xffffffff);

	d3d11DevCon->IASetInputLayout(mEngine->GetBaseInputLayout());
	d3d11DevCon->IASetVertexBuffers(0, 1, mEngine->GetSquareVertBuffer(), &strides, &offset);
	d3d11DevCon->IASetIndexBuffer(*(mEngine->GetSquareIdxBuffer()), DXGI_FORMAT_R32_UINT, 0);

	d3d11DevCon->VSSetShader(mEngine->GetBaseVertShader(), nullptr, 0);
	d3d11DevCon->PSSetShader(mEngine->GetBasePixelShader(), nullptr, 0);

	d3d11DevCon->UpdateSubresource(mCBAim, 0, nullptr, &cbPerObject, 0, 0);
	d3d11DevCon->VSSetConstantBuffers(0, 1, &mCBAim);

	d3d11DevCon->PSSetShaderResources(0, 1, &mTexAim);
	d3d11DevCon->PSSetSamplers(0, 1, mEngine->GetBaseSamplerState());

	d3d11DevCon->RSSetState(mEngine->GetCWcullMode());

	d3d11DevCon->DrawIndexed(6, 0, 0);
}

void Player::DrawDamageImage() const
{
	if (mbIsAttacked)
	{
		ID3D11DeviceContext* d3d11DevCon = mEngine->GetD3d11DevCon();

		UINT strides = sizeof(Vertex);
		UINT offset = 0;

		CBAlpha cbAlpha;
		cbAlpha.Alpha = mAlpha;

		d3d11DevCon->VSSetShader(mEngine->GetAlphaVertShader(), nullptr, 0);
		d3d11DevCon->PSSetShader(mEngine->GetAlphaPixelShader(), nullptr, 0);

		d3d11DevCon->IASetInputLayout(mEngine->GetBaseInputLayout());
		d3d11DevCon->IASetVertexBuffers(0, 1, mEngine->GetSquareVertBuffer(), &strides, &offset);
		d3d11DevCon->IASetIndexBuffer(*(mEngine->GetSquareIdxBuffer()), DXGI_FORMAT_R32_UINT, 0);

		d3d11DevCon->UpdateSubresource(mAlphaCB, 0, nullptr, &cbAlpha, 0, 0);
		d3d11DevCon->PSSetConstantBuffers(0, 1, &mAlphaCB);

		d3d11DevCon->PSSetShaderResources(0, 1, &mBloodTex);
		d3d11DevCon->PSSetSamplers(0, 1, mEngine->GetBaseSamplerState());

		d3d11DevCon->OMSetBlendState(mEngine->GetTransparencyBlendState(), nullptr, 0xffffffff);
		d3d11DevCon->RSSetState(mEngine->GetCWcullMode());

		d3d11DevCon->DrawIndexed(6, 0, 0);
	}
}

void Player::DrawMuzzleflash()
{
	if (mAccTime <= BOUND_TIME && mbIsShoot)
	{
		ID3D11DeviceContext* d3d11DevCon = mEngine->GetD3d11DevCon();

		UINT strides = sizeof(Vertex);
		UINT offset = 0;

		CBPerObject cbPerObject;
		cbPerObject.World = XMMatrixTranspose(mWorldMuzzleflash);
		cbPerObject.WVP = XMMatrixTranspose(mWorldMuzzleflash);

		d3d11DevCon->OMSetBlendState(mEngine->GetTransparencyBlendState(), nullptr, 0xffffffff);

		d3d11DevCon->IASetInputLayout(mEngine->GetBaseInputLayout());
		d3d11DevCon->IASetVertexBuffers(0, 1, mEngine->GetSquareVertBuffer(), &strides, &offset);
		d3d11DevCon->IASetIndexBuffer(*(mEngine->GetSquareIdxBuffer()), DXGI_FORMAT_R32_UINT, 0);

		d3d11DevCon->VSSetShader(mEngine->GetBaseVertShader(), nullptr, 0);
		d3d11DevCon->PSSetShader(mEngine->GetBasePixelShader(), nullptr, 0);

		d3d11DevCon->UpdateSubresource(mCBMuzzleflash, 0, nullptr, &cbPerObject, 0, 0);
		d3d11DevCon->VSSetConstantBuffers(0, 1, &mCBMuzzleflash);

		d3d11DevCon->PSSetShaderResources(0, 1, &mTexMuzzleflash);
		d3d11DevCon->PSSetSamplers(0, 1, mEngine->GetBaseSamplerState());

		d3d11DevCon->RSSetState(mEngine->GetNoCullMode());

		d3d11DevCon->DrawIndexed(6, 0, 0);
	}
	else
	{
		mbIsShoot = false;
	}
}

void Player::DrawDeadImage() const
{
	ID3D11DeviceContext* d3d11DevCon = mEngine->GetD3d11DevCon();

	UINT strides = sizeof(Vertex);
	UINT offset = 0;

	CBAlpha cbAlpha;
	cbAlpha.Alpha = mAlpha;

	d3d11DevCon->OMSetBlendState(mEngine->GetTransparencyBlendState(), nullptr, 0xffffffff);

	d3d11DevCon->IASetInputLayout(mEngine->GetBaseInputLayout());
	d3d11DevCon->IASetVertexBuffers(0, 1, mEngine->GetSquareVertBuffer(), &strides, &offset);
	d3d11DevCon->IASetIndexBuffer(*(mEngine->GetSquareIdxBuffer()), DXGI_FORMAT_R32_UINT, 0);

	d3d11DevCon->VSSetShader(mEngine->GetAlphaVertShader(), nullptr, 0);
	d3d11DevCon->PSSetShader(mEngine->GetAlphaPixelShader(), nullptr, 0);

	d3d11DevCon->UpdateSubresource(mAlphaCB, 0, nullptr, &cbAlpha, 0, 0);
	d3d11DevCon->PSSetConstantBuffers(0, 1, &mAlphaCB);

	d3d11DevCon->PSSetShaderResources(0, 1, &mGameoverTex);
	d3d11DevCon->PSSetSamplers(0, 1, mEngine->GetBaseSamplerState());

	d3d11DevCon->RSSetState(mEngine->GetCWcullMode());

	d3d11DevCon->DrawIndexed(6, 0, 0);
}
