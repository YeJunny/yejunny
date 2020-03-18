#include "../Settings/Global.h"
#include "Sound.h"
#include <Mmreg.h>

Sound::Sound()
{
}


Sound::Sound(const Sound& other)
{
}


Sound::~Sound()
{
}

bool Sound::Initialize(const char* soundFile)
{
	// ���� ����� �⺻ ���� ���۸� �ʱ�ȭ�մϴ�.
	if (!InitializeDirectSound())
	{
		return false;
	}

	// ���̺� ����� ������ 2 �� ���ۿ��ε��Ѵ�.
	if (!LoadWaveFile(soundFile, &m_secondaryBuffer1, &m_secondary3DBuffer1))
	{
		return false;
	}

	// �ε� �� ���̺� ������ ����մϴ�.
	/*if (!PlayWaveFile())
	{
		return false;
	}*/

	return true;
}


void Sound::Shutdown()
{
	// 2 �� ���۸� �����Ѵ�.
	ShutdownWaveFile(&m_secondaryBuffer1, &m_secondary3DBuffer1);

	// Direct Sound API�� �˴ٿ��Ѵ�.
	ShutdownDirectSound();
}


bool Sound::InitializeDirectSound()
{
	// �⺻ ���� ��ġ�� ���� ���� ���� �������̽� �����͸� �ʱ�ȭ�մϴ�.
	if (FAILED(DirectSoundCreate8(NULL, &m_DirectSound, NULL)))
	{
		return false;
	}

	// ���� ������ �켱 ������ �����Ͽ� �⺻ ���� ������ ������ ������ �� �ֽ��ϴ�.
	if (FAILED(m_DirectSound->SetCooperativeLevel(Global::GetHwnd(), DSSCL_PRIORITY)))
	{
		return false;
	}

	// �⺻ ���� ������ �����մϴ�.
	DSBUFFERDESC bufferDesc;
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// �⺻ ���� ��ġ���� �⺻ ���� ���۸� �����մϴ�.
	if (FAILED(m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer, NULL)))
	{
		return false;
	}

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
	{
		return false;
	}

	// ������ �������̽��� ����ϴ�.
	if (FAILED(m_primaryBuffer->QueryInterface(IID_IDirectSound3DListener8, (LPVOID*)&m_listener)))
	{
		return false;
	}

	// �������� �ʱ� ��ġ�� ����� �߰��� �����մϴ�.
	m_listener->SetPosition(0.0f, 0.0f, 0.0f, DS3D_IMMEDIATE);

	return true;
}


void Sound::ShutdownDirectSound()
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


bool Sound::LoadWaveFile(const char* filename, IDirectSoundBuffer8** secondaryBuffer, IDirectSound3DBuffer8** secondary3DBuffer)
{
	// ���̺� ������ ���̳ʸ� ���� ���ϴ�.
	FILE* filePtr = nullptr;
	int error = fopen_s(&filePtr, filename, "rb");
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
	if ((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') ||
		(waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
	{
		return false;
	}

	// ���������� wave ���� üũ �Ѵ�
	if ((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
		(waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
	{
		return false;
	}

	// fmt ���� chunk id üũ�Ѵ�.
	if ((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
		(waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
	{
		return false;
	}

	// ����� ������ WAVE_FORMAT_PCM���� üũ�Ѵ�
	if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
	{
		return false;
	}

	// ���̺� ������ ��� �������� ���׷������� üũ�Ѵ�.
	if (waveFileHeader.numChannels != 1)
	{
		return false;
	}

	// ���̺� ������ 44.1 KHz�� ���� �ӵ��� ���ڵ��Ǿ����� Ȯ���մϴ�.
	if (waveFileHeader.sampleRate != 44100)
	{
		return false;
	}

	// ���̺� ������ 16 ��Ʈ �������� ���ڵ��Ǿ����� Ȯ���մϴ�.
	if (waveFileHeader.bitsPerSample != 16)
	{
		return false;
	}

	// ��� �������� chunk�� Ȯ���Ѵ�.
	if ((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
		(waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
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
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Ư�� ���� �������� �ӽ� ���� ���۸� ����ϴ�.
	IDirectSoundBuffer* tempBuffer = nullptr;
	if (FAILED(m_DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL)))
	{
		return false;
	}

	// ���̷�Ʈ ���� �������̽��� ���� ���� ������ �׽�Ʈ�ϰ� ���� ���۸� ����ϴ�.
	if (FAILED(tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer)))
	{
		return false;
	}

	// �ӽ� ���۸� �����Ѵ�.
	tempBuffer->Release();
	tempBuffer = 0;

	// ������ ûũ ����� ������ �����ϴ� ���̺� �������� ���� �κ����� �̵��մϴ�.
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	// ���̺� ���� �����͸� ������ �ӽ� ���۸� ����ϴ�.
	unsigned char* waveData = new unsigned char[waveFileHeader.dataSize];
	if (!waveData)
	{
		return false;
	}

	// ���̺� ���� �����͸� ���� ���� �� ���۷� �о���Դϴ�.
	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if (count != waveFileHeader.dataSize)
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
	if (FAILED((*secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0)))
	{
		return false;
	}

	// ���̺� �����͸� ���ۿ� �����մϴ�.
	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	// �����Ͱ� ������ �� ���� ������ ����� �����մϴ�.
	if (FAILED((*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0)))
	{
		return false;
	}

	// ���� ���ۿ� ���� �� ���̺� �����͸� �����մϴ�.
	delete[] waveData;
	waveData = 0;

	// 2 �� ���� ���ۿ� 3D �������̽� ���� ����.
	if (FAILED((*secondaryBuffer)->QueryInterface(IID_IDirectSound3DBuffer8, (void**)&*secondary3DBuffer)))
	{
		return false;
	}

	return true;
}


void Sound::ShutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer, IDirectSound3DBuffer8** secondary3DBuffer)
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


bool Sound::PlayWaveFile()
{
	// ���� ������ ���� �κп� ��ġ�� �����մϴ�.
	if (FAILED(m_secondaryBuffer1->SetCurrentPosition(0)))
	{
		return false;
	}

	// ������ ������ 100 %�� �����մϴ�.
	if (FAILED(m_secondaryBuffer1->SetVolume(DSBVOLUME_MAX)))
	{
		return false;
	}

	// ������ 3D ��ġ�� �����մϴ�.
	XMFLOAT3& playerPos = Global::GetPlayerPos();
	if (FAILED(m_secondary3DBuffer1->
		SetPosition(playerPos.x, playerPos.y, playerPos.z, DS3D_DEFERRED)))
	{
		return false;
	}

	// 2 �� ���� ������ ������ ����մϴ�.
	if (FAILED(m_secondaryBuffer1->Play(0, 0, 0)))
	{
		return false;
	}

	return true;
}