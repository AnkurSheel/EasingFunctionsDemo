#include "stdafx.h"
#include "DirectSoundAudioBuffer.h"
#include "AudioBuffer.h"
#include "SoundResource.h"
#include <DxErr.h>

#pragma comment(lib, "dxerr.lib")
using namespace Sound;
using namespace Utilities;
using namespace Base;

//  *******************************************************************************************************************
cDirectSoundAudioBuffer::cDirectSoundAudioBuffer(LPDIRECTSOUNDBUFFER pSound, shared_ptr<ISoundResHandle> pResource)
  : cAudioBuffer(pResource)
{
  m_pSoundBuffer = pSound;
  FillBufferWithSound();
}

//  *******************************************************************************************************************
bool cDirectSoundAudioBuffer::VPlay()
{
  VStop();

  LPDIRECTSOUNDBUFFER pDSBuffer = GetSoundBuffer();
  if (pDSBuffer == NULL)
  {
    return false;
  }

  if (S_OK == pDSBuffer->Play(0, 0, 0))
  {
    m_Paused = false;
  }
  return m_Paused;
}

//  *******************************************************************************************************************
bool cDirectSoundAudioBuffer::VStop()
{
  LPDIRECTSOUNDBUFFER pDSBuffer = GetSoundBuffer();
  if (pDSBuffer == NULL)
  {
    return false;
  }
  m_Paused = true;
  pDSBuffer->Stop();
  return true;
}

//  *******************************************************************************************************************
bool cDirectSoundAudioBuffer::VResume()
{
  m_Paused = false;
  return VPlay();
}

//  *******************************************************************************************************************
bool cDirectSoundAudioBuffer::VSetPaused(bool paused)
{
  if (paused)
  {
    VPause();
  }
  else
  {
    VResume();
  }
  return true;
}

//  *******************************************************************************************************************
bool cDirectSoundAudioBuffer::VPause()
{
  return VStop();
}

//  *******************************************************************************************************************
bool cDirectSoundAudioBuffer::VIsPlaying()
{
  LPDIRECTSOUNDBUFFER pDSBuffer = GetSoundBuffer();
  if (pDSBuffer == NULL)
  {
    return false;
  }
  unsigned long ulStatus = 0L;
  pDSBuffer->GetStatus(&ulStatus);
  return ((ulStatus & DSBSTATUS_PLAYING) != 0);
}

//  *******************************************************************************************************************
bool cDirectSoundAudioBuffer::VRestore()
{
  if (!RestoreBuffer())
    return false;

  return FillBufferWithSound();
}

//  *******************************************************************************************************************
void cDirectSoundAudioBuffer::VSetVolume(uint16 volume)
{
  SP_ASSERT(volume <= 100)(volume).SetCustomMessage("Volume must be between 0 and 100");
  Clamp<uint16>(volume, 0, 100);

  LPDIRECTSOUNDBUFFER pDSBuffer = GetSoundBuffer();
  SP_ASSERT(pDSBuffer != NULL).SetCustomMessage("Could not get Sound Buffer");

  if (pDSBuffer)
  {
    return;
  }

  float cCoeff = static_cast<float>(volume) / 100.0f;
  float logarithmicProportion = cCoeff > 0.1f ? 1 + log10(cCoeff) : 0;
  float range = (DSBVOLUME_MAX - DSBVOLUME_MIN);
  float adjustedVolume = (range * logarithmicProportion) + DSBVOLUME_MIN;
  m_Volume = static_cast<unsigned int>(adjustedVolume);

  pDSBuffer->SetVolume(m_Volume);
}

//  *******************************************************************************************************************
float cDirectSoundAudioBuffer::VGetProgress()
{
  LPDIRECTSOUNDBUFFER pDSBuffer = GetSoundBuffer();
  if (pDSBuffer == NULL)
  {
    return 0.0f;
  }
  unsigned long progress = 0;
  pDSBuffer->GetCurrentPosition(&progress, NULL);
  float length = static_cast<float>(m_pResource->VGetPCMBufferSize());
  return (progress / length);
}

//  *******************************************************************************************************************
LPDIRECTSOUNDBUFFER cDirectSoundAudioBuffer::GetSoundBuffer()
{
  if (!VRestore())
  {
    return NULL;
  }
  return m_pSoundBuffer;
}

//  *******************************************************************************************************************
bool cDirectSoundAudioBuffer::RestoreBuffer()
{
  if (m_pSoundBuffer == NULL)
  {
    return false;
  }

  unsigned long ulStatus = 0L;
  m_pSoundBuffer->GetStatus(&ulStatus);
  int iCount = 0;
  if (ulStatus && DSBSTATUS_BUFFERLOST)
  {
    HRESULT result;
    do
    {
      result = m_pSoundBuffer->Restore();
      if (result == DSERR_BUFFERLOST)
      {
        Sleep(10);
      }
    }
    // cppcheck-suppress unreadVariable
    while ((result = m_pSoundBuffer->Restore()) == DSERR_BUFFERLOST && ++iCount < 100);  // NOLINT(whitespace/empty_loop_body)
  }
  return (iCount < 100);
}

//  *******************************************************************************************************************
bool cDirectSoundAudioBuffer::FillBufferWithSound()
{
  if (m_pSoundBuffer == NULL)
  {
    return false;
  }
  HRESULT result;
  if (!RestoreBuffer())
  {
    return false;
  }
  void* pDSLockedBuffer = NULL;
  unsigned long lockedBufferSize = 0L;

  uint32 pcmBufferSize = m_pResource->VGetPCMBufferSize();
  result = m_pSoundBuffer->Lock(0, pcmBufferSize, &pDSLockedBuffer, &lockedBufferSize, NULL, NULL, NULL);
  if (FAILED(result))
  {
    SP_ASSERT_ERROR(false)
    (DXGetErrorString(result))(DXGetErrorDescription(result)).SetCustomMessage("Could not lock sound buffer");
    return false;
  }

  shared_ptr<cSoundResHandle> pHandle = dynamic_pointer_cast<cSoundResHandle>(m_pResource);

  if (pcmBufferSize == 0)
  {
    FillMemory((uint8*)pDSLockedBuffer, lockedBufferSize, (uint8)pHandle->GetFormat()->wBitsPerSample == 8 ? 128 : 0);
  }
  else
  {
    CopyMemory(pDSLockedBuffer, m_pResource->VGetPCMBuffer(), pcmBufferSize);
    if (pcmBufferSize < (lockedBufferSize))
    {
      FillMemory((uint8*)pDSLockedBuffer + pcmBufferSize, lockedBufferSize - pcmBufferSize,
                 (uint8)pHandle->GetFormat()->wBitsPerSample == 8 ? 128 : 0);
    }
  }
  m_pSoundBuffer->Unlock(pDSLockedBuffer, lockedBufferSize, NULL, 0);
  return true;
}
