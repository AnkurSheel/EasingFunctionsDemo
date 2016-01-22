#include "stdafx.h"
#include "DirectSoundAudio.h"
#include <DxErr.h>
#include "SoundResource.h"
#include "AudioBuffer.hxx"
#include "DirectSoundAudioBuffer.h"

using namespace Sound;
using namespace Utilities;
using namespace Base;

//  *******************************************************************************************************************
cDirectSoundAudio::cDirectSoundAudio()
	: m_pDS(nullptr)
{
}

//  *******************************************************************************************************************
cDirectSoundAudio::~cDirectSoundAudio()
{
}

//  *******************************************************************************************************************
bool cDirectSoundAudio::VInitialize(const HWND& windowHandle)
{
	SP_ASSERT(!m_Initialized).SetCustomMessage("DirectSound Audio already initialized");

	if (m_Initialized)
	{
		return true;
	}

	HRESULT result = DirectSoundCreate(NULL, &m_pDS, NULL);
	if (FAILED(result))
	{
		SP_ASSERT_ERROR(false)(result)(DXGetErrorString(result))(DXGetErrorDescription(result)).SetCustomMessage("Could not create direct sound");
		return false;
	}

	result = m_pDS->SetCooperativeLevel(windowHandle, DSSCL_PRIORITY);
	if (FAILED(result))
	{
		SP_ASSERT_ERROR(false)(result)(DXGetErrorString(result))(DXGetErrorDescription(result)).SetCustomMessage("Could not set direct sound cooperative level");
		return false;
	}

	result = SetPrimaryBuffer(2, 44100, 16);
	if (FAILED(result))
	{
		SP_ASSERT_ERROR(false)(result)(DXGetErrorString(result))(DXGetErrorDescription(result)).SetCustomMessage("Could not set primary buffer format");
		return false;
	}

	m_Initialized = true;
	m_ActiveSoundList.clear();

	return true;
}

//  *******************************************************************************************************************
bool cDirectSoundAudio::SetPrimaryBuffer(uint16 primaryChannels, uint32 primaryFreq, uint16 primaryBitRate)
{
	DSBUFFERDESC bufferDesc = {};
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER;

	HRESULT result;
	LPDIRECTSOUNDBUFFER pDSBuffer;
	result = m_pDS->CreateSoundBuffer(&bufferDesc, &pDSBuffer, NULL);
	if (FAILED(result))
	{
		SP_ASSERT_ERROR(false)(result)(DXGetErrorString(result))(DXGetErrorDescription(result)).SetCustomMessage("Could not create sound buffer");
		return false;
	}

	WAVEFORMATEX waveFormat = {};
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nChannels = primaryChannels;
	waveFormat.nSamplesPerSec = primaryFreq;
	waveFormat.wBitsPerSample = primaryBitRate;
	waveFormat.nBlockAlign = waveFormat.nChannels * waveFormat.wBitsPerSample / 8;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;

	result = pDSBuffer->SetFormat(&waveFormat);
	if (FAILED(result))
	{
		SP_ASSERT_ERROR(false)(result)(DXGetErrorString(result))(DXGetErrorDescription(result)).SetCustomMessage("Could not set format for sound buffer");
		return false;
	}

	SafeRelease(&pDSBuffer);
	return true;
}

//  *******************************************************************************************************************
void cDirectSoundAudio::VCleanup()
{
	cAudio::VCleanup();
	SafeRelease(&m_pDS);
	m_Initialized = false;
}

//  *******************************************************************************************************************
IAudioBuffer * cDirectSoundAudio::VInitializeAudioBuffer(shared_ptr<ISoundResHandle> pSoundResource)
{
	if (m_pDS == NULL)
	{
		return NULL;
	}

	DSBUFFERDESC bufferDesc = {};
	ZeroMemory(&bufferDesc, sizeof(DSBUFFERDESC));
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = pSoundResource->VGetPCMBufferSize();
	shared_ptr<cSoundResHandle> pHandle = dynamic_pointer_cast<cSoundResHandle>(pSoundResource);
	bufferDesc.lpwfxFormat = const_cast<WAVEFORMATEX *>(pHandle->GetFormat());

	HRESULT result;
	LPDIRECTSOUNDBUFFER pDSBuffer;

	result = m_pDS->CreateSoundBuffer(&bufferDesc, &pDSBuffer, NULL);
	if (FAILED(result))
	{
		SP_ASSERT_ERROR(false)(result)(DXGetErrorString(result))(DXGetErrorDescription(result)).SetCustomMessage("Could not create sound buffer");
		return NULL;
	}

	IAudioBuffer * pAudioBuffer = static_cast<IAudioBuffer *>(DEBUG_NEW cDirectSoundAudioBuffer(pDSBuffer, pSoundResource));
	m_ActiveSoundList.push_front(pAudioBuffer);
	return pAudioBuffer;
}

//  *******************************************************************************************************************
void cDirectSoundAudio::VReleaseAudioBuffer(IAudioBuffer * pAudioBuffer)
{
	pAudioBuffer->VStop();
	m_ActiveSoundList.remove(pAudioBuffer);
}
