//  *******************************************************************************************************************
//  Audio   version:  1.0   Ankur Sheel  date: 2013/01/03
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef Audio_hxx__
#define Audio_hxx__

#include "SoundDefines.h"

namespace Sound
{
  class IAudioBuffer;
  class ISoundResHandle;
}

namespace Sound
{
  class IAudio
  {
  public:
    virtual ~IAudio() {}
    virtual bool VInitialize(const HWND& windowHandle) = 0;
    virtual IAudioBuffer* VInitializeAudioBuffer(shared_ptr<ISoundResHandle> pSoundResource) = 0;
    virtual void VReleaseAudioBuffer(IAudioBuffer* pAudioBuffer) = 0;
    SOUND_API static void CreateAsService();
  };
}  // namespace Sound
#endif  // Audio_hxx__
