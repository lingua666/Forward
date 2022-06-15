#ifndef  __FLV_CONVERTER_20170111090004_1444199361_H__
#define  __FLV_CONVERTER_20170111090004_1444199361_H__

#include "External.h"

namespace _ffmpeg_{

	class u4
	{
	public:
		u4(unsigned int i) { _u[0] = i >> 24; _u[1] = (i >> 16) & 0xff; _u[2] = (i >> 8) & 0xff; _u[3] = i & 0xff; }

	public:
		unsigned char _u[4];
	};

	class u3
	{
	public:
		u3(unsigned int i) { _u[0] = i >> 16; _u[1] = (i >> 8) & 0xff; _u[2] = i & 0xff; }

	public:
		unsigned char _u[3];
	};

	class u2
	{
	public:
		u2(unsigned int i) { _u[0] = i >> 8; _u[1] = i & 0xff; }

	public:
		unsigned char _u[2];
	};

	class FlvConverter
	{
	public:
		FlvConverter( void );

		virtual ~FlvConverter( void );

		int Init(const _ff_string& SPS, const _ff_string& PPS, int bHaveAudio = 0, int bHaveVideo = 1);

		void Release(void);

		_ff_string GetFlvHeader(void);

		_ff_string_big Close(void);

		_ff_string_big ConvertH264To(char *pNalu, int nNaluSize, unsigned int nTimeStamp, bool isKeyFrame);

		//int ConvertAAC(char *pAAC, int nAACFrameSize, unsigned int nTimeStamp);

	private:
		// h.264
		_ff_string_big ConvertH264KeyFrameTo(UInt32 uTimeStamp);


		_ff_string_big WriteH264EndofSeq(UInt32 uTimeStamp = 0);

		// aac
		/*void WriteAACHeader(unsigned int nTimeStamp);
		void WriteAACFrame(char *pFrame, int nFrameSize, unsigned int nTimeStamp);

		void Write(unsigned char u) { _fileOut.write((char *)&u, 1); }
		void Write(u4 u) { _fileOut.write((char *)u._u, 4); }
		void Write(u3 u) { _fileOut.write((char *)u._u, 3); }
		void Write(u2 u) { _fileOut.write((char *)u._u, 2); }*/

	private:
		_ff_string _FlvHeader;
		_ff_string _SPS;
		_ff_string _PPS;
		int _bWriteAVCSeqHeader;
		int _nPrevTagSize;
		int _nStreamID;
		UInt32 _uVideoTimeStamp;

		unsigned char *_pAudioSpecificConfig;
		int _nAudioConfigSize;
		int _aacProfile;
		int _sampleRateIndex;
		int _channelConfig;
		int _bWriteAACSeqHeader;

	private:
		int _bHaveAudio, _bHaveVideo;

	};

}

#endif // CONVERTER_H
