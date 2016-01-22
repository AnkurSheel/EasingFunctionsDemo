#include "stdafx.h"
#include "SoundResource.h"
#include "Vorbis\codec.h"
#include "Vorbis\vorbisfile.h"

using namespace Utilities;
using namespace Base;
using namespace Sound;

//  *******************************************************************************************************************
cSoundResource::cSoundResource(const Base::cString & strName)
	: cResource(strName)
{
}

//  *******************************************************************************************************************
cSoundResource::~cSoundResource()
{
}

//  *******************************************************************************************************************
IResHandle * cSoundResource::VCreateHandle(const char * pBuffer, unsigned int uiSize, IResCache * pResCache)
{
	return DEBUG_NEW cSoundResHandle(*this, (unsigned char *)pBuffer, uiSize, pResCache);
}

//  *******************************************************************************************************************
ISoundResHandle::ISoundResHandle(Utilities::cResource & resource, unsigned char * pBuffer, unsigned int uiSize, IResCache * pResCache)
	: cResHandle(resource, (char *)pBuffer, uiSize, pResCache)
{
}

//  *******************************************************************************************************************
cSoundResHandle::cSoundResHandle(Utilities::cResource & resource, unsigned char * pBuffer, unsigned int uiSize, IResCache * pResCache)
	: ISoundResHandle(resource, pBuffer, uiSize, pResCache)
	, m_Initialized(false)
	, m_pPCMBuffer(NULL)
	, m_PCMBufferSize(0)
	, m_LengthMS(0)
	, m_SoundFileName(resource.VGetFileName())
{
}

//  *******************************************************************************************************************
cSoundResHandle::~cSoundResHandle()
{
	SafeDeleteArray(&m_pPCMBuffer);
}

//  *******************************************************************************************************************
bool cSoundResHandle::VInitialize()
{
	if (!m_Initialized)
	{
		cString extension = cStringUtilities::GetExtensionFromFileName(m_SoundFileName);
		if (extension.CompareInsensitive("wav") == 0)
		{
			ParseWave(m_pBuffer, m_iBufferSize);
		}
		else if (extension.CompareInsensitive("ogg") == 0)
		{
			ParseOgg(m_pBuffer, m_iBufferSize);
		}
		else
		{
			SP_LOG_CUSTOM(AssertLevels::LEVEL_ERROR, 1, "We do not support " + extension + " at this time");
		}
		m_Initialized = true;
	}
	return true;
}

//  *******************************************************************************************************************
int cSoundResHandle::VGetPCMBufferSize() const
{
	return m_PCMBufferSize;
}

//  *******************************************************************************************************************
char const * Sound::cSoundResHandle::VGetPCMBuffer() const
{
	return m_pPCMBuffer;
}

//  *******************************************************************************************************************
bool cSoundResHandle::ParseWave(const char * const pWavStream,
	const unsigned int uiBufferLength)
{
	UINT64 uiFile = 0;
	UINT64 uiFileEnd = 0;
	UINT64 uiLength = 0;
	UINT64 uiType = 0;
	UINT64 uiPos = 0;

	// mmioFOURCC -- converts four chars into a 4 byte integer code.
	// The first 4 bytes of a valid .wav file is 'R','I','F','F'
	uiType = *((UINT64  *)(pWavStream + uiPos));
	uiPos += sizeof(UINT64);
	if (uiType != mmioFOURCC('R', 'I', 'F', 'F'))
	{
		return false;
	}
	uiLength = *((UINT64 *)(pWavStream + uiPos));
	uiPos += sizeof(UINT64);
	uiType = *((UINT64 *)(pWavStream + uiPos));
	uiPos +=sizeof(UINT64);
	// 'W','A','V','E' for a legal .wav file
	if (uiType != mmioFOURCC('W', 'A', 'V', 'E'))
	{
		// not a WAV
		return false;
	}
	uiFileEnd = uiLength - 4;
	memset(&m_WaveFormatEx, 0, sizeof(WAVEFORMATEX));
	while(uiFile < uiFileEnd)
	{
		uiType = *((UINT64 *)(pWavStream + uiPos));
		uiPos += sizeof(UINT64);
		uiFile += sizeof(UINT64);
		uiLength= *((UINT64 *)(pWavStream + uiPos));
		uiPos +=sizeof(UINT64);
		uiFile += sizeof(UINT64);
		switch(uiType)
		{
		case mmioFOURCC('f', 'a', 'c', 't'):
			SP_ASSERT_ERROR(false).SetCustomMessage("This wav file is compressed. We don't handle compressed wav at this time");
			break;

		case mmioFOURCC('f', 'm', 't', ' '):
			memcpy(&m_WaveFormatEx, pWavStream + uiPos, uiLength);
			uiPos += uiLength;
			m_WaveFormatEx.cbSize = uiLength;
			break;

		case mmioFOURCC('d', 'a', 't', 'a'):
			m_pPCMBuffer = DEBUG_NEW char[uiLength];
			m_PCMBufferSize = uiLength;
			memcpy(m_pPCMBuffer, pWavStream + uiPos, uiLength);
			uiPos +=uiLength;
			break;
		}
		uiFile += uiLength;
		// If both blocks have been seen, we can return true.
		if ( (m_pPCMBuffer != 0 ) && (m_PCMBufferSize != 0) )
		{
			m_LengthMS = (m_PCMBufferSize * 1000) / m_WaveFormatEx.nAvgBytesPerSec;
			return true;
		}
		// Increment the pointer past the block we just read and make sure the pointer is word aligned.
		if (uiLength & 1)
		{
			++uiPos;
			++uiFile;
		}
	}
	SP_ASSERT_ERROR(false).SetCustomMessage("This wav file didnt contain all the right pieces");
	return false;
}

struct OggMemoryFile
{
	unsigned char * pData;  /// Pointer to the data in memory
	unsigned int uiDataSize;  /// Size of the data
	unsigned int uiDataRead;  /// Bytes read so far

	OggMemoryFile()
		: pData(NULL)
		, uiDataSize(0)
		, uiDataRead(0)
	{
	}
};

unsigned int VorbisRead(void * data_ptr, unsigned int uiByteSize, unsigned int uiSizeToRead, void * data_src);
int VorbisSeek(void * data_src, ogg_int64_t offset, int origin);
int VorbisClose(void *src);
long VorbisTell(void * data_src);

//  *******************************************************************************************************************
bool cSoundResHandle::ParseOgg(const char * const pOggStream, const unsigned int uiBufferLength)
{
	OggVorbis_File vf;
	ov_callbacks oggCallbacks;

	OggMemoryFile * pVorbisMemoryFile = DEBUG_NEW OggMemoryFile;
	pVorbisMemoryFile->pData = (unsigned char *)(const_cast<char *>(pOggStream));
	pVorbisMemoryFile->uiDataSize = uiBufferLength;

	oggCallbacks.read_func = VorbisRead;
	oggCallbacks.close_func = VorbisClose;
	oggCallbacks.seek_func = VorbisSeek;
	oggCallbacks.tell_func = VorbisTell;

	int ov_ret = ov_open_callbacks(pVorbisMemoryFile, &vf, NULL, 0, oggCallbacks);
	SP_ASSERT_ERROR(ov_ret >= 0)(ov_ret).SetCustomMessage("Error while setting vorbis callbacks");

	vorbis_info * pvi = ov_info(&vf, -1);

	memset(&m_WaveFormatEx, 0, sizeof(WAVEFORMATEX));
	m_WaveFormatEx.cbSize = sizeof(m_WaveFormatEx);
	m_WaveFormatEx.nChannels = pvi->channels;
	m_WaveFormatEx.wBitsPerSample = 16;  // ogg vorbis is always 16 bit
	m_WaveFormatEx.nSamplesPerSec = pvi->rate;
	m_WaveFormatEx.nAvgBytesPerSec = m_WaveFormatEx.nSamplesPerSec * m_WaveFormatEx.nChannels * 2;
	m_WaveFormatEx.nBlockAlign = 2 * m_WaveFormatEx.nChannels;
	m_WaveFormatEx.wFormatTag = 1;

	UINT64 ulSize = 4096 * 16;
	UINT64 ulPos = 0;
	int iSec = 0;
	int iRet = 1;

	UINT64 ulBytes = (UINT64)ov_pcm_total(&vf, -1);
	ulBytes *= 2 * pvi->channels;
	m_pPCMBuffer = DEBUG_NEW char[ulBytes];
	m_PCMBufferSize = ulBytes;

	while(iRet && ulPos < ulBytes)
	{
		iRet = ov_read(&vf, m_pPCMBuffer + ulPos, ulSize, 0, 2, 1, &iSec);
		ulPos += iRet;
		if (ulBytes - ulPos > ulSize)
		{
			ulSize = ulBytes - ulPos;
		}
	}

	m_LengthMS = 1000.0f * ov_time_total(&vf, -1);

	ov_clear(&vf);

	SafeDelete(&pVorbisMemoryFile);
	return true;
}

//  *******************************************************************************************************************
WAVEFORMATEX const * cSoundResHandle::GetFormat() const
{
	return &m_WaveFormatEx;
}

//  *******************************************************************************************************************
unsigned int VorbisRead(void * data_ptr, unsigned int uiByteSize, unsigned int uiSizeToRead, void * data_src)
{
	OggMemoryFile *pVorbisData = static_cast<OggMemoryFile *>(data_src);
	if (pVorbisData == nullptr)
	{
		return -1;
	}

	unsigned int actualSizeToRead;
	unsigned int spaceToEOF = pVorbisData->uiDataSize - pVorbisData->uiDataRead;
	if ((uiSizeToRead * uiByteSize) < spaceToEOF)
	{
		actualSizeToRead = (uiSizeToRead * uiByteSize);
	}
	else
	{
		actualSizeToRead = spaceToEOF;
	}

	if (actualSizeToRead)
	{
		memcpy(data_ptr, (char*)pVorbisData->pData + pVorbisData->uiDataRead, actualSizeToRead);
		pVorbisData->uiDataRead += actualSizeToRead;
	}

	return actualSizeToRead;
}

//  *******************************************************************************************************************
int VorbisSeek(void * data_src, ogg_int64_t offset, int origin)
{
	OggMemoryFile *pVorbisData = static_cast<OggMemoryFile *>(data_src);
	if (NULL == pVorbisData)
	{
		return -1;
	}

	switch (origin)
	{
	case SEEK_SET:
		{
			ogg_int64_t actualOffset;
			actualOffset = (pVorbisData->uiDataSize >= offset) ? offset : pVorbisData->uiDataSize;
			pVorbisData->uiDataRead = static_cast<size_t>(actualOffset);
			break;
		}

	case SEEK_CUR:
		{
			unsigned int spaceToEOF = pVorbisData->uiDataSize - pVorbisData->uiDataRead;

			ogg_int64_t actualOffset;
			actualOffset = (offset < spaceToEOF) ? offset : spaceToEOF;

			pVorbisData->uiDataRead += static_cast<LONG>(actualOffset);
			break;
		}

	case SEEK_END:
		pVorbisData->uiDataRead = pVorbisData->uiDataSize+1;
		break;

	default:
		SP_ASSERT_ERROR("Bad parameter for 'origin', requires same as fseek.")(origin);
		break;
	};

	return 0;
}

//  *******************************************************************************************************************
int VorbisClose(void *src)
{
	return 0;
}

//  *******************************************************************************************************************
long VorbisTell(void * data_src)
{
	OggMemoryFile *pVorbisData = static_cast<OggMemoryFile *>(data_src);
	if (NULL == pVorbisData)
	{
		return -1L;
	}

	return static_cast<long>(pVorbisData->uiDataRead);
}
