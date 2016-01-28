//  *******************************************************************************************************************
//  SoundProcess   version:  1.0   Ankur Sheel  date: 2013/01/08
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef SoundProcess_h__
#define SoundProcess_h__

#include "SoundProcess.hxx"

namespace Sound
{
  class cSoundResHandle;
  class IAudioBuffer;
}
namespace Sound
{
  class cSoundProcess : public ISoundProcess
  {
  public:
    cSoundProcess(const Base::cString& type, shared_ptr<ISoundResHandle> pSoundResource, int volume, bool looping);
    ~cSoundProcess();
    void VInitialize() OVERRIDE;
    void VUpdate(const float DeltaTime) OVERRIDE;
    void VKill() OVERRIDE;
    void VSetPaused(const bool Paused) OVERRIDE;
    void VSetActive(const bool active) OVERRIDE;
    void VSetVolume(int volume) OVERRIDE;
    void Play();
    void Stop();
    bool IsPlaying();
    const Base::cHashedString& VGetType() const OVERRIDE { return m_Type; }
  private:
    static const Base::cHashedString m_Type;

  private:
    shared_ptr<ISoundResHandle> m_pSoundHandle;
    shared_ptr<IAudioBuffer> m_pAudioBuffer;
    int m_Volume;
    bool m_Looping;
  };
}  // namespace Sound
#endif  // SoundProcess_h__
