#include "stdafx.h"
#include "SoundProcess.h"
#include "Audio.hxx"
#include "SoundResource.h"
#include "AudioBuffer.hxx"
#include "ResourceManager.hxx"

using namespace Sound;
using namespace Base;
using namespace Utilities;
using namespace std;

const cHashedString cSoundProcess::m_Type = cHashedString("soundprocess");

//  *******************************************************************************************************************
cSoundProcess::cSoundProcess(const cString& type, shared_ptr<ISoundResHandle> pSoundResource, int volume, bool looping)
	: m_pSoundHandle(pSoundResource)
	, m_Volume(volume)
	, m_Looping(looping)
{
}

//  *******************************************************************************************************************
cSoundProcess::~cSoundProcess()
{
	if (m_pAudioBuffer)
	{
		shared_ptr<IAudio> pAudioService = MakeStrongPtr(cServiceLocator::GetInstance()->GetService<IAudio>());
		if (pAudioService != nullptr)
		{
			pAudioService->VReleaseAudioBuffer(m_pAudioBuffer.get());
		}
	}
}
//  *******************************************************************************************************************
void cSoundProcess::VInitialize()
{
	cProcess::VInitialize();
	if (m_pSoundHandle == NULL)
	{
		return;
	}
	m_pSoundHandle->VInitialize();
	IAudioBuffer * pAudioBuffer = nullptr;
	shared_ptr<IAudio> pAudioService = MakeStrongPtr(cServiceLocator::GetInstance()->GetService<IAudio>());
	if (pAudioService != nullptr)
	{
		pAudioBuffer = pAudioService->VInitializeAudioBuffer(m_pSoundHandle);
	}
	if (pAudioBuffer == nullptr)
	{
		VKill();
		return;
	}
	m_pAudioBuffer.reset(pAudioBuffer);
	m_pAudioBuffer->VSetVolume(m_Volume);
	Play();
}

//  *******************************************************************************************************************
void cSoundProcess::VUpdate(const float DeltaTime)
{
	cProcess::VUpdate(DeltaTime);
	if (!IsPlaying())
	{
		if (m_Looping)
		{
			Play();
		}
		else
		{
			VKill();
		}
	}
}

//  *******************************************************************************************************************
void cSoundProcess::VKill()
{
	if (IsPlaying())
	{
		Stop();
	}
	cProcess::VKill();
}

//  *******************************************************************************************************************
void cSoundProcess::VSetVolume(int volume)
{
	m_Volume = volume;
	if (m_pAudioBuffer == NULL)
	{
		return;
	}
	m_pAudioBuffer->VSetVolume(m_Volume);
}

//  *******************************************************************************************************************
void cSoundProcess::Play()
{
	if (m_pAudioBuffer == NULL)
	{
		return;
	}
	m_pAudioBuffer->VPlay();
}

//  *******************************************************************************************************************
void cSoundProcess::Stop()
{
	if (m_pAudioBuffer)
	{
		m_pAudioBuffer->VStop();
	}
}

//  *******************************************************************************************************************
bool cSoundProcess::IsPlaying()
{
	if (m_pSoundHandle == NULL || m_pAudioBuffer == NULL)
	{
		return false;
	}
	return m_pAudioBuffer->VIsPlaying();
}

//  *******************************************************************************************************************
void cSoundProcess::VSetPaused(const bool Paused)
{
	if (m_pAudioBuffer)
	{
		m_pAudioBuffer->VSetPaused(Paused);
	}
	cProcess::VSetPaused(Paused);
}

//  *******************************************************************************************************************
void cSoundProcess::VSetActive(const bool active)
{
	if (m_Active != active)
	{
		cProcess::VSetActive(active);
		if (m_pAudioBuffer)
		{
			if (m_Active)
			{
				m_pAudioBuffer->VResume();
			}
			else
			{
				m_pAudioBuffer->VPause();
			}
		}
	}
}

//  *******************************************************************************************************************
shared_ptr<ISoundProcess> ISoundProcess::CreateSoundProcess(const cString& type, const cString& soundFile, int volume, bool looping)
{
	unique_ptr<cSoundResource> pResource(DEBUG_NEW cSoundResource(soundFile));
	shared_ptr<ISoundResHandle> pHandle = static_pointer_cast<ISoundResHandle>(IResourceManager::GetInstance()->VGetResourceCache()->GetHandle(*pResource));
	return shared_ptr<ISoundProcess>(DEBUG_NEW cSoundProcess(type, pHandle, volume, looping));
}
