#include "stdafx.h"
#include "Audio.h"
#include "AudioBuffer.hxx"
#include "DirectSoundAudio.h"

using namespace Sound;
using namespace Base;

//  *******************************************************************************************************************
cAudio::cAudio()
	: m_IsPaused(false)
	, m_Initialized(false)
{
}

//  *******************************************************************************************************************
cAudio::~cAudio()
{
	VCleanup();
}

//  *******************************************************************************************************************
void cAudio::ResumeAllSounds()
{
	for(auto iter = m_ActiveSoundList.begin(); iter != m_ActiveSoundList.end(); iter++)
	{
		IAudioBuffer * pAudioBuffer = (*iter);
		pAudioBuffer->VResume();
	}
	m_IsPaused = false;
}

//  *******************************************************************************************************************
void cAudio::PauseAllSounds()
{
	for(auto iter = m_ActiveSoundList.begin(); iter != m_ActiveSoundList.end(); iter++)
	{
		IAudioBuffer * pAudioBuffer = (*iter);
		pAudioBuffer->VPause();
	}
	m_IsPaused = true;
}

//  *******************************************************************************************************************
void cAudio::StopAllSounds()
{
	for(auto iter = m_ActiveSoundList.begin(); iter != m_ActiveSoundList.end(); iter++)
	{
		IAudioBuffer * pAudioBuffer = (*iter);
		pAudioBuffer->VStop();
	}
	m_IsPaused = true;
}

//  *******************************************************************************************************************
void cAudio::VCleanup()
{
	while(!m_ActiveSoundList.empty())
	{
		m_ActiveSoundList.pop_front();
	}
}

//  *******************************************************************************************************************
void IAudio::CreateAsService()
{
	cServiceLocator::GetInstance()->Register<IAudio>(shared_ptr<IAudio>(DEBUG_NEW cDirectSoundAudio()));
}
