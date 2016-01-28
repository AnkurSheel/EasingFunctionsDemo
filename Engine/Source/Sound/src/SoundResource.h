//  *******************************************************************************************************************
//  SoundResource   version:  1.0   Ankur Sheel  date: 2012/12/29
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#include "SoundResource.hxx"

#ifndef SOUNDRESOURCE_H__
#define SOUNDRESOURCE_H__

namespace Sound
{
  class cSoundResHandle : public ISoundResHandle
  {
  public:
    cSoundResHandle(Utilities::cResource& resource, unsigned char* pBuffer, unsigned int uiSize,
                    Utilities::IResCache* pResCache);
    ~cSoundResHandle();
    bool VInitialize();
    int VGetPCMBufferSize() const;
    WAVEFORMATEX const* GetFormat() const;
    char const* VGetPCMBuffer() const;

  private:
    bool ParseWave(const char* const pWavStream, const unsigned int uiBufferLength);
    bool ParseOgg(const char* const pOggStream, const unsigned int uiBufferLength);

  private:
    const Base::cString m_SoundFileName;  /// The name of the file resource.
    WAVEFORMATEX m_WaveFormatEx;          /// Description of the PCM format.
    char* m_pPCMBuffer;                   /// The destination PCM buffer of the sound.
    int m_PCMBufferSize;                  /// The length of the PCM buffer.
    int m_LengthMS;                       /// Length of the sound in milliseconds.
    bool m_Initialized;                   /// True if the sound has been initialized.
  };
}  // namespace Sound
#endif  // SOUNDRESOURCE_H__
