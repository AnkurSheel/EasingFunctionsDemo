//  *******************************************************************************************************************
//  SoundProcess   version:  1.0   Ankur Sheel  date: 2013/01/08
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef SoundProcess_hxx__
#define SoundProcess_hxx__

#include "Process.h"
#include "SoundDefines.h"

namespace Sound
{
  class ISoundResHandle;
  class IAudioBuffer;
}
namespace Sound
{
  class ISoundProcess : public Utilities::cProcess
  {
  public:
    virtual ~ISoundProcess() {}
    SOUND_API static shared_ptr<ISoundProcess> CreateSoundProcess(const Base::cString& type,
                                                                  const Base::cString& soundFile, int volume,
                                                                  bool looping);
    SOUND_API virtual void VSetVolume(int volume) = 0;
  };
}  // namespace Sound
#endif  // SoundProcess_hxx__
