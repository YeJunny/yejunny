#include "Camera.h"
#include "Engine.h"
#include "SoundManager.h"
#include <DirectXMath.h>
#include <stdio.h>
#include <Mmreg.h>

bool SoundManager::Init(Engine* engine)
{
	mEngine = engine;

	bool bIsSuccess = false;

	// ���� ����� �⺻ ���� ���۸� �ʱ�ȭ�մϴ�.
	bIsSuccess = InitializeDirectSound();
	AssertIsSuccess(bIsSuccess, "Initialize Direct Sound - Failed");

	return bIsSuccess;
}

void SoundManager::CleanUp()
{
	// 2 �� ���۸� �����Ѵ�.
	for (auto iter = mSounds.begin(); iter != mSounds.end(); ++iter)
	{
		ShutdownWaveFile(&(*iter).second.SecondaryBuffer, &(*iter).second.Secondary3DBuffer);
	}

	mSounds.clear();

	// Direct Sound API�� �˴ٿ��Ѵ�.
	ShutdownDirectSound();
}


bool SoundManager::InitializeDirectSound()
{
	// �⺻ ���� ��ġ�� ���� ���� ���� �������̽� �����͸� �ʱ�ȭ�մϴ�.
	HRESULT hr = DirectSoundCreate8(NULL, &m_DirectSound, NULL);
	AssertInitialization(hr, "Direct Sound Create 8 - Failed");

	// ���� ������ �켱 ������ �����Ͽ� �⺻ ���� ������ ������ ������ �� �ֽ��ϴ�.
	hr = m_DirectSound->SetCooperativeLevel(mEngine->GetHanleer(), DSSCL_PRIORITY);
	AssertInitialization(hr, "Direct Sound Set Cooperative Level - Failed");


	// �⺻ ���� ������ �����մϴ�.
	DSBUFFERDESC bufferDesc;
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// �⺻ ���� ��ġ���� �⺻ ���� ���۸� �����մϴ�.
	hr = m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer, NULL);
	AssertInitialization(hr, "Direct Sound Create Sound Buffer - Failed");


	// �⺻ ���� ������ ������ �����մϴ�.
	// �� ��� 16 ��Ʈ ���׷��� (cd ����� ����)���� 44,100 ���� / �ʷ� ��ϵǴ� .WAV �����Դϴ�.
	WAVEFORMATEX waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// �⺻ ���۸� ������ ���� �������� �����մϴ�.
	if (FAILED(m_primaryBuffer->SetFormat(&waveFormat)))
		AssertInitialization(hr, "Direct Sound Create Sound Buffer - Failed");


	// ������ �������̽��� ����ϴ�.
	if (FAILED(m_primaryBuffer->QueryInterface(IID_IDirectSound3DListener8, (LPVOID*)&m_listener)))
		AssertInitialization(hr, "Direct Sound Create Sound Buffer - Failed");


	// �������� �ʱ� ��ġ�� ����� �߰��� �����մϴ�.
	m_listener->SetPosition(0.0f, 0.0f, 0.0f, DS3D_IMMEDIATE);

	return true;
}

void SoundManager::ShutdownDirectSound()
{
	// ������ �������̽��� �����մϴ�.
	if (m_listener)
	{
		m_listener->Release();
		m_listener = 0;
	}

	// �⺻ ���� ���� �����͸� �����ϴ�.
	if (m_primaryBuffer)
	{
		m_primaryBuffer->Release();
		m_primaryBuffer = 0;
	}

	// ���� ���� �������̽� �����͸� �����ϴ�.
	if (m_DirectSound)
	{
		m_DirectSound->Release();
		m_DirectSound = 0;
	}
}

bool SoundManager::AddSound(std::string name, const char* soundPath)
{
	Sound sound;


	// ���̺� ������ ���̳ʸ� ���� ���ϴ�.
	FILE* filePtr = nullptr;
	int error = fopen_s(&filePtr, soundPath, "rb");
	if (error != 0)
	{
		return false;
	}

	// ���̺� ���� ����� �д´�
	WaveHeaderType waveFileHeader;
	size_t count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// RIFF ���� chunk id üũ�Ѵ�.
	if ((waveFileHeader.ChunkId[0] != 'R') || (waveFileHeader.ChunkId[1] != 'I') ||
		(waveFileHeader.ChunkId[2] != 'F') || (waveFileHeader.ChunkId[3] != 'F'))
	{
		return false;
	}

	// ���������� wave ���� üũ �Ѵ�
	if ((waveFileHeader.Format[0] != 'W') || (waveFileHeader.Format[1] != 'A') ||
		(waveFileHeader.Format[2] != 'V') || (waveFileHeader.Format[3] != 'E'))
	{
		return false;
	}

	// fmt ���� chunk id üũ�Ѵ�.
	if ((waveFileHeader.SubChunkId[0] != 'f') || (waveFileHeader.SubChunkId[1] != 'm') ||
		(waveFileHeader.SubChunkId[2] != 't') || (waveFileHeader.SubChunkId[3] != ' '))
	{
		return false;
	}

	// ����� ������ WAVE_FORMAT_PCM���� üũ�Ѵ�
	if (waveFileHeader.AudioFormat != WAVE_FORMAT_PCM)
	{
		return false;
	}

	// ���̺� ������ ��� �������� ���׷������� üũ�Ѵ�.
	if (waveFileHeader.NumChannels != 1)
	{
		return false;
	}

	// ���̺� ������ 44.1 KHz�� ���� �ӵ��� ���ڵ��Ǿ����� Ȯ���մϴ�.
	if (waveFileHeader.SampleRate != 44100)
	{
		return false;
	}

	// ���̺� ������ 16 ��Ʈ �������� ���ڵ��Ǿ����� Ȯ���մϴ�.
	if (waveFileHeader.BitsPerSample != 16)
	{
		return false;
	}

	// ��� �������� chunk�� Ȯ���Ѵ�.
	if ((waveFileHeader.DataChunkId[0] != 'd') || (waveFileHeader.DataChunkId[1] != 'a') ||
		(waveFileHeader.DataChunkId[2] != 't') || (waveFileHeader.DataChunkId[3] != 'a'))
	{
		return false;
	}

	// �� ���̺� �����̷ε� �� ���� ������ ���̺� ������ �����մϴ�.
	WAVEFORMATEX waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 1;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// ���̺� �����̷ε� �� 2 �� ���� ������ ���� ������ �����մϴ�.
	DSBUFFERDESC bufferDesc;
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
	bufferDesc.dwBufferBytes = waveFileHeader.DataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Ư�� ���� �������� �ӽ� ���� ���۸� ����ϴ�.
	IDirectSoundBuffer* tempBuffer = nullptr;
	HRESULT hr = m_DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
	AssertInitialization(hr, "Direct Sound Create Sound Buffer - Failed");


	// ���̷�Ʈ ���� �������̽��� ���� ���� ������ �׽�Ʈ�ϰ� ���� ���۸� ����ϴ�.
	hr = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)(&sound.SecondaryBuffer));
	AssertInitialization(hr, "Direct Sound Query Interface - Failed");


	// �ӽ� ���۸� �����Ѵ�.
	tempBuffer->Release();
	tempBuffer = 0;

	// ������ ûũ ����� ������ �����ϴ� ���̺� �������� ���� �κ����� �̵��մϴ�.
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	// ���̺� ���� �����͸� ������ �ӽ� ���۸� ����ϴ�.
	unsigned char* waveData = new unsigned char[waveFileHeader.DataSize];
	if (!waveData)
	{
		return false;
	}

	// ���̺� ���� �����͸� ���� ���� �� ���۷� �о���Դϴ�.
	count = fread(waveData, 1, waveFileHeader.DataSize, filePtr);
	if (count != waveFileHeader.DataSize)
	{
		return false;
	}

	// �а� ���� ������ �ݴ´�.
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	// ���̺� �����͸� ���� ���� 2�� ���۸� ��޴ϴ�.
	unsigned char* bufferPtr = nullptr;
	unsigned long bufferSize = 0;
	hr = (sound.SecondaryBuffer)->Lock(0, waveFileHeader.DataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	AssertInitialization(hr, "Direct Sound Lock - Failed");


	// ���̺� �����͸� ���ۿ� �����մϴ�.
	memcpy(bufferPtr, waveData, waveFileHeader.DataSize);

	// �����Ͱ� ������ �� ���� ������ ����� �����մϴ�.
	hr = (sound.SecondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	AssertInitialization(hr, "Direct Sound Unlock - Failed");


	// ���� ���ۿ� ���� �� ���̺� �����͸� �����մϴ�.
	delete[] waveData;
	waveData = 0;

	// 2 �� ���� ���ۿ� 3D �������̽� ���� ����.
	hr = (sound.SecondaryBuffer)->QueryInterface(IID_IDirectSound3DBuffer8, (void**)(&sound.Secondary3DBuffer));
	AssertInitialization(hr, "Direct Sound Query Interface - Failed");



	mSounds[name] = sound;

	return true;
}

bool SoundManager::DeleteSound(std::string name)
{
	Sound sound = mSounds[name];

	ShutdownWaveFile(&sound.SecondaryBuffer, &sound.Secondary3DBuffer);

	mSounds.erase(name);

	return true;
}

void SoundManager::ShutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer, IDirectSound3DBuffer8** secondary3DBuffer)
{
	// 3D �������̽��� 2 �� ���� ���ۿ� �����ϴ�.
	if (*secondary3DBuffer)
	{
		(*secondary3DBuffer)->Release();
		*secondary3DBuffer = 0;
	}

	// 2 �� ���� ���۸� �����Ѵ�.
	if (*secondaryBuffer)
	{
		(*secondaryBuffer)->Release();
		*secondaryBuffer = 0;
	}
}

bool SoundManager::PlayWaveFile(std::string name)
{
	Sound sound = mSounds[name];


	// ���� ������ ���� �κп� ��ġ�� �����մϴ�.
	HRESULT hr = (sound.SecondaryBuffer)->SetCurrentPosition(0);
	AssertInitialization(hr, "Direct Sound Set Current Position - Failed");

	// ������ ������ 100 %�� �����մϴ�.
	hr = (sound.SecondaryBuffer)->SetVolume(DSBVOLUME_MAX);
	AssertInitialization(hr, "Direct Sound Set Current Position - Failed");


	// ������ 3D ��ġ�� �����մϴ�.
	const DirectX::XMVECTOR& tempPlayerPos = mEngine->GetCamera()->GetCamPos();
	
	DirectX::XMFLOAT3 playerPos;
	XMStoreFloat3(&playerPos, tempPlayerPos);

	hr = (sound.Secondary3DBuffer)->SetPosition(playerPos.x, playerPos.y, playerPos.z, DS3D_DEFERRED);
	AssertInitialization(hr, "Direct Sound Set Current Position - Failed");


	// 2 �� ���� ������ ������ ����մϴ�.
	hr = (sound.SecondaryBuffer)->Play(0, 0, 0);
	AssertInitialization(hr, "Direct Sound Set Current Position - Failed");


	return true;
}