//  *******************************************************************************************************************
//  DirectSoundAudioBuffer   version:  1.0   Ankur Sheel  date: 2014/12/05
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef __DIRECTSOUNDAUDIOBUFFER_H__
#define __DIRECTSOUNDAUDIOBUFFER_H__

#include <dsound.h>
#include "AudioBuffer.h"

namespace Sound
{
	class ISoundResHandle;
}

namespace Sound
{
	class cDirectSoundAudioBuffer
		: public cAudioBuffer
	{
	public:
		cDirectSoundAudioBuffer(LPDIRECTSOUNDBUFFER pSound, shared_ptr<ISoundResHandle> pResource);

	private:
		bool VPlay() OVERRIDE;
		bool VStop() OVERRIDE;
		bool VResume() OVERRIDE;
		bool VSetPaused(bool paused) OVERRIDE;
		bool VPause() OVERRIDE;
		bool VIsPlaying() OVERRIDE;
		bool VRestore() OVERRIDE;
		void VSetVolume(uint16 volume) OVERRIDE;
		float VGetProgress() OVERRIDE;
		LPDIRECTSOUNDBUFFER GetSoundBuffer();
		bool RestoreBuffer();
		bool FillBufferWithSound();

	private:
		LPDIRECTSOUNDBUFFER m_pSoundBuffer;
	};
}  // namespace Sound
#endif  // __DIRECTSOUNDAUDIOBUFFER_H__
