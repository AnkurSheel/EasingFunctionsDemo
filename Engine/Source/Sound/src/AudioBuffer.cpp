#include "stdafx.h"
#include "AudioBuffer.h"

using namespace Sound;

//  *******************************************************************************************************************
cAudioBuffer::cAudioBuffer(shared_ptr<ISoundResHandle> pResource)
: m_pResource(pResource)
, m_Paused(false)
, m_Volume(0)
{
}

//  *******************************************************************************************************************
cAudioBuffer::~cAudioBuffer()
{
}
