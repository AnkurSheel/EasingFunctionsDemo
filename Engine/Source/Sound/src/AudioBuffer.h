//  *******************************************************************************************************************
//  AudioBuffer   version:  1.0   Ankur Sheel  date: 2013/01/03
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef AudioBuffer_h__
#define AudioBuffer_h__

#include "AudioBuffer.hxx"

namespace Sound
{
  class ISoundResHandle;
}

namespace Sound
{
  class cAudioBuffer : public IAudioBuffer
  {
  public:
    ~cAudioBuffer();

  protected:
    explicit cAudioBuffer(shared_ptr<ISoundResHandle> pResource);

  protected:
    shared_ptr<ISoundResHandle> m_pResource;
    uint32 m_Volume;
    bool m_Paused;
  };
}  // namespace Sound
#endif  // AudioBuffer_h__
