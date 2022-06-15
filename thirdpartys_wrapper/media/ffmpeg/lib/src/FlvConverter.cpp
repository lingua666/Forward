
#include <libThirdWrap_FFmpeg/FlvConverter.h>

namespace _ffmpeg_{

	FlvConverter::FlvConverter( void )
	{
		_bWriteAVCSeqHeader = 0;
		_nPrevTagSize = 0;
		_nStreamID = 0;
		_bHaveAudio = false;
		_bHaveAudio = false;

		_pAudioSpecificConfig = NULL;
		_nAudioConfigSize = 0;
		_aacProfile = 0;
		_sampleRateIndex = 0;
		_channelConfig = 0;
		_bWriteAACSeqHeader = 0;
	}

	FlvConverter::~FlvConverter( void )
	{

	}

	int FlvConverter::Init(const _ff_string& SPS, const _ff_string& PPS, int bHaveAudio, int bHaveVideo)
	{
		_nPrevTagSize = 0xC3;// 0;

		_bHaveAudio = bHaveAudio;
		_bHaveVideo = bHaveVideo;
		_SPS = SPS;
		_PPS = PPS;

		_FlvHeader.append("FLV");
		_FlvHeader.append(1, 0x01);
		if (_bHaveVideo != 0)
			_FlvHeader.append(1, 0x01);

		if (_bHaveAudio != 0)
			_FlvHeader.append(1, 0x04);

		UInt32 uSize = 9;
		uSize = Conversion32ToByteOrder(uSize, enByteOrderType::EN_BOT_BIG);
		_FlvHeader.append((char*)&uSize, sizeof(UInt32));
		return 1;
	}

	void FlvConverter::Release(void)
	{

	}

	_ff_string FlvConverter::GetFlvHeader(void)
	{
		return _FlvHeader;
	}

	_ff_string_big FlvConverter::Close(void)
	{		
		if (_bHaveVideo!=0)
			return WriteH264EndofSeq();

		return "";
	}

	_ff_string_big FlvConverter::ConvertH264To(char *pNalu, int nNaluSize,
		unsigned int nTimeStamp, bool isKeyFrame)
	{
		_ff_string_big	sData;

		//_nVideoTimeStamp = nTimeStamp;

		if (pNalu == NULL || nNaluSize <= 4)
			return 0;

		if (isKeyFrame)
		{
			sData = ConvertH264KeyFrameTo(nTimeStamp);
		}

		int nNaluType = pNalu[4] & 0x1F;

		u4 prev_u4(_nPrevTagSize);
		sData.append((char*)prev_u4._u, sizeof(prev_u4._u));
		sData.append(1, 0x09);

		int nDataSize;
		nDataSize = 1 + 1 + 3 + 4 + (nNaluSize);
		u3 datasize_u3(nDataSize);
		sData.append((char*)datasize_u3._u, sizeof(datasize_u3._u));

		u3 tt_u3(nTimeStamp);
		sData.append((char*)tt_u3._u, sizeof(tt_u3._u));
		sData.append(1, unsigned char(nTimeStamp >> 24));

		u3 sid(_nStreamID);
		sData.append((char*)sid._u, sizeof(sid._u));
		if(isKeyFrame)
			sData.append(1, 0x17);
		else
			sData.append(1, 0x27);

		sData.append(1, 0x01);

		u3 com_time_u3(0);
		sData.append((char*)com_time_u3._u, sizeof(com_time_u3._u));

		u4 nalusize_u4(nNaluSize);
		sData.append((char*)nalusize_u4._u, sizeof(nalusize_u4._u));

		sData.append((char *)pNalu, nNaluSize);

		_nPrevTagSize = 11 + nDataSize;
		return sData;
	}

	_ff_string_big FlvConverter::ConvertH264KeyFrameTo(UInt32 uTimeStamp)
	{
		_ff_string_big sData;
		u4 prev_u4(_nPrevTagSize);
		sData.append((char*)prev_u4._u, sizeof(prev_u4._u));
		sData.append(1, 0x09);

		int nDataSize = 1 + 1 + 3 + 6 + 2 + _SPS.size() + 1 + 2 + _PPS.size();
		u3 datasize_u3(nDataSize);
		sData.append((char*)datasize_u3._u, sizeof(datasize_u3._u));

		u3 tt_u3(uTimeStamp);
		sData.append((char*)tt_u3._u, sizeof(tt_u3._u));

		unsigned char cTTex = uTimeStamp >> 24;
		sData.append(1, cTTex);

		u3 sid_u3(_nStreamID);
		sData.append((char*)sid_u3._u, sizeof(sid_u3._u));

		sData.append(1, 0x17);

		unsigned char cAVCPacketType = 0; /* seq header */
		sData.append(1, cAVCPacketType);

		u3 CompositionTime_u3(0);
		sData.append((char*)CompositionTime_u3._u, sizeof(CompositionTime_u3._u));

		sData.append(1, 0x01);

		sData.append(1, _SPS[1]);
		sData.append(1, _SPS[2]);
		sData.append(1, _SPS[3]);

		sData.append(1, 0xFF);
		sData.append(1, 0xE1);

		u2 spssize_u2(_SPS.size());
		sData.append((char*)spssize_u2._u, sizeof(spssize_u2._u));

		sData.append((char*)&_SPS[0], _SPS.size());

		sData.append(1, 0x01);

		u2 ppssize_u2(_PPS.size());
		sData.append((char*)ppssize_u2._u, sizeof(ppssize_u2._u));
		sData.append((char*)&_PPS[0], _PPS.size());

		_nPrevTagSize = 11 + nDataSize;

		return sData;
	}	

	_ff_string_big FlvConverter::WriteH264EndofSeq(UInt32 uTimeStamp)
	{
		_ff_string_big sData;
		u4 prev_u4(_nPrevTagSize);
		sData.append((char*)prev_u4._u, sizeof(prev_u4._u));

		sData.append(1, 0x09);
		
		int nDataSize;
		nDataSize = 1 + 1 + 3;
		u3 datasize_u3(nDataSize);
		sData.append((char*)datasize_u3._u, sizeof(datasize_u3._u));

		u3 tt_u3(uTimeStamp);
		sData.append((char*)tt_u3._u, sizeof(tt_u3._u));
		sData.append(1, unsigned char(uTimeStamp >> 24));

		u3 sid(_nStreamID);
		sData.append((char*)sid._u, sizeof(sid._u));
		
		sData.append(1, 0x27);
		sData.append(1, 0x02);

		u3 com_time_u3(0);
		sData.append((char*)com_time_u3._u, sizeof(com_time_u3._u));
		return sData;
	}

	/*int FlvConverter::ConvertAAC(char *pAAC, int nAACFrameSize, unsigned int nTimeStamp)
	{
		if (pAAC == NULL || nAACFrameSize <= 7)
			return 0;

		if (_pAudioSpecificConfig == NULL)
		{
			_pAudioSpecificConfig = new unsigned char[2];
			_nAudioConfigSize = 2;

			unsigned char *p = (unsigned char *)pAAC;
			_aacProfile = (p[2] >> 6) + 1;
			_sampleRateIndex = (p[2] >> 2) & 0x0f;
			_channelConfig = ((p[2] & 0x01) << 2) + (p[3]>>6);

			_pAudioSpecificConfig[0] = (_aacProfile << 3) + (_sampleRateIndex>>1);
			_pAudioSpecificConfig[1] = ((_sampleRateIndex&0x01)<<7) + (_channelConfig<<3);
		}
		if (_pAudioSpecificConfig != NULL & _bWriteAACSeqHeader == 0)
		{
			WriteAACHeader(nTimeStamp);
			_bWriteAACSeqHeader = 1;
		}
		if (_bWriteAACSeqHeader == 0)
			return 1;

		WriteAACFrame(pAAC, nAACFrameSize, nTimeStamp);

		return 1;
	}

	void FlvConverter::WriteAACHeader(unsigned int nTimeStamp)
	{
		u4 prev_u4(_nPrevTagSize);
		_fileOut.write((char *)prev_u4._u, 4);

		char cTagType = 0x08;
		_fileOut.write(&cTagType, 1);
		int nDataSize = 1 + 1 + 2;

		u3 datasize_u3(nDataSize);
		_fileOut.write((char *)datasize_u3._u, 3);

		u3 tt_u3(nTimeStamp);
		_fileOut.write((char *)tt_u3._u, 3);

		unsigned char cTTex = nTimeStamp >> 24;
		_fileOut.write((char *)&cTTex, 1);

		u3 sid_u3(_nStreamID);
		_fileOut.write((char *)sid_u3._u, 3);

		unsigned char cAudioParam = 0xAF;
		_fileOut.write((char *)&cAudioParam, 1);
		unsigned char cAACPacketType = 0; // seq header
		_fileOut.write((char *)&cAACPacketType, 1);

		_fileOut.write((char *)_pAudioSpecificConfig, 2);

		_nPrevTagSize = 11 + nDataSize;
	}

	void FlvConverter::WriteAACFrame(char *pFrame, int nFrameSize, unsigned int nTimeStamp)
	{
		u4 prev_u4(_nPrevTagSize);
		Write(prev_u4);

		Write(0x08);
		int nDataSize;
		nDataSize = 1 + 1 + (nFrameSize - 7);
		u3 datasize_u3(nDataSize);
		Write(datasize_u3);
		u3 tt_u3(nTimeStamp);
		Write(tt_u3);
		Write(unsigned char(nTimeStamp >> 24));

		u3 sid(_nStreamID);
		Write(sid);

		unsigned char cAudioParam = 0xAF;
		_fileOut.write((char *)&cAudioParam, 1);
		unsigned char cAACPacketType = 1; // AAC raw data
		_fileOut.write((char *)&cAACPacketType, 1);

		_fileOut.write((char *)pFrame + 7, nFrameSize - 7);

		_nPrevTagSize = 11 + nDataSize;
	}*/
}
