//  *******************************************************************************************************************
//  DirectSoundAudio   version:  1.0   Ankur Sheel  date: 2013/01/03
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef DirectSoundAudio_h__
#define DirectSoundAudio_h__

#include <dsound.h>
#include "Audio.h"

namespace Sound
{
  class ISoundResHandle;
}

namespace Sound
{
  class cDirectSoundAudio : public cAudio
  {
  public:
    cDirectSoundAudio();
    ~cDirectSoundAudio();
    bool VInitialize(const HWND& windowHandle);
    void VCleanup();
    IAudioBuffer* VInitializeAudioBuffer(shared_ptr<ISoundResHandle> pSoundResource);
    void VReleaseAudioBuffer(IAudioBuffer* pAudioBuffer);

  private:
    bool SetPrimaryBuffer(uint16 primaryChannels, uint32 primaryFreq, uint16 primaryBitRate);

  protected:
    LPDIRECTSOUND m_pDS;
  };
}  // namespace Sound
#endif  // DirectSoundAudio_h__
