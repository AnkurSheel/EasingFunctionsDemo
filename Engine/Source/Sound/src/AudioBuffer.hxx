//  *******************************************************************************************************************
//  AudioBuffer   version:  1.0   Ankur Sheel  date: 2014/12/05
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef __AUDIOBUFFER_HXX__
#define __AUDIOBUFFER_HXX__

namespace Sound
{
  class IAudioBuffer
  {
  public:
    virtual ~IAudioBuffer() {}
    virtual bool VPlay() = 0;
    virtual bool VStop() = 0;
    virtual bool VResume() = 0;
    virtual bool VPause() = 0;
    virtual bool VSetPaused(bool paused) = 0;
    virtual bool VIsPlaying() = 0;
    virtual bool VRestore() = 0;
    virtual void VSetVolume(uint16 volume) = 0;
    virtual float VGetProgress() = 0;
  };
}  // namespace Sound
#endif  // __AUDIOBUFFER_HXX__
