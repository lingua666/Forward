
#ifndef __AUDIO_SWR_CONVERT_20170111090004_1444199361_H__
#define __AUDIO_SWR_CONVERT_20170111090004_1444199361_H__

#include "External.h"
#include "IDecoder.h"

namespace _ffmpeg_{

	class FFResampler
	{
	public:
		FFResampler(void);

		~FFResampler(void);

		int Init(int SrcRate, UInt8 uSrcChannels, int DstRate, UInt8 uDstChannels);

		void Release(void);

		_string_type Push(const char* c_szData, UInt32 uSize);

	private:
		struct SwrContext* _swr_ctx;
		AVSampleFormat _src_sample_fmt;
		AVSampleFormat _dst_sample_fmt;
		_string_type	_sBuffer;
		_string_type	_sRetData;
		uint8_t**	_src_data;
		uint8_t**	_dst_data;
		int _src_nb_samples;
		int _dst_nb_samples;
		int _max_dst_nb_samples;
		int _src_nb_channels;
		int _dst_nb_channels;
		int _src_linesize;
		int _dst_linesize;
		int	_iFrameSize;
	};

	_string_type Audio_swr_convert_fmts16(int SrcRate, UInt8 uSrcChannels, int DstRate, UInt8 uDstChannels,
		const char* c_szData, UInt64 uSize);

} //namespace _ffmpeg_

#endif//__DECODERFACTORY_20170111090004_1444199361_H__
