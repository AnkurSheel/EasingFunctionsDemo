//  *******************************************************************************************************************
//  Audio   version:  1.0   Ankur Sheel  date: 2013/01/03
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef Audio_h__
#define Audio_h__

#include "Audio.hxx"

namespace Sound
{
	class IAudioBuffer;
}

namespace Sound
{
	class cAudio
		: public IAudio
	{
	private:
		typedef std::list<IAudioBuffer *> AudioBufferList;

	protected:
		cAudio();
		virtual ~cAudio();
		virtual void VCleanup();

	private:
		void StopAllSounds();
		void PauseAllSounds();
		void ResumeAllSounds();

	protected:
		AudioBufferList m_ActiveSoundList;
		bool m_IsPaused;
		bool m_Initialized;
	};
}  // namespace Sound
#endif  // Audio_h__
