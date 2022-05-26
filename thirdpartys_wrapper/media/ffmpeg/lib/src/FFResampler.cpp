
#include <libThirdWrap_FFmpeg/FFResampler.h>

namespace _ffmpeg_{

	FFResampler::FFResampler(void)
		: _swr_ctx(NULL)
		, _src_data(NULL)
		, _dst_data(NULL)
		, _src_nb_samples(1024)
		, _dst_nb_samples(0)
		, _max_dst_nb_samples(0)
		, _src_nb_channels(0)
		, _dst_nb_channels(0)
		, _src_sample_fmt(AV_SAMPLE_FMT_S16)
		, _dst_sample_fmt(AV_SAMPLE_FMT_S16)
		, _src_linesize(0)
		, _dst_linesize(0)
		, _iFrameSize(0)
	{

	}

	FFResampler::~FFResampler(void)
	{
		Release();
	}

	int FFResampler::Init(int SrcRate, UInt8 uSrcChannels, int DstRate, UInt8 uDstChannels)
	{
		int64_t src_ch_layout = uSrcChannels == 1 ? AV_CH_LAYOUT_MONO : AV_CH_LAYOUT_STEREO;
		int64_t dst_ch_layout = uDstChannels == 1 ? AV_CH_LAYOUT_MONO : AV_CH_LAYOUT_STEREO;
		int src_rate = SrcRate, dst_rate = DstRate;
		const char *fmt = NULL;
		int ret = 0;

		/* create resampler context */
		_swr_ctx = swr_alloc();
		if (!_swr_ctx)
		{
			LOG_Print_Error(libThirdWrap_FFmpeg, "Audio_swr_convert_fmts16() Could not allocate resampler context!");
			ret = AVERROR(ENOMEM);
			return ret;
		}

		/* set options */
		av_opt_set_int(_swr_ctx, "in_channel_layout", src_ch_layout, 0);
		av_opt_set_int(_swr_ctx, "in_sample_rate", src_rate, 0);
		av_opt_set_sample_fmt(_swr_ctx, "in_sample_fmt", _src_sample_fmt, 0);

		av_opt_set_int(_swr_ctx, "out_channel_layout", dst_ch_layout, 0);
		av_opt_set_int(_swr_ctx, "out_sample_rate", dst_rate, 0);
		av_opt_set_sample_fmt(_swr_ctx, "out_sample_fmt", _dst_sample_fmt, 0);

		/* initialize the resampling context */
		if ((ret = swr_init(_swr_ctx)) < 0)
		{
			LOG_Print_Error(libThirdWrap_FFmpeg, "Audio_swr_convert_fmts16() Failed to initialize the resampling context!");
			swr_free(&_swr_ctx);
			_swr_ctx = NULL;
			return -1;
		}

		/* allocate source and destination samples buffers */

		_src_nb_channels = av_get_channel_layout_nb_channels(src_ch_layout);
		ret = av_samples_alloc_array_and_samples(&_src_data, &_src_linesize, _src_nb_channels,
			_src_nb_samples, _src_sample_fmt, 0);
		if (ret < 0)
		{
			LOG_Print_Error(libThirdWrap_FFmpeg, "Audio_swr_convert_fmts16() Could not allocate source samples!");
			swr_free(&_swr_ctx);
			_swr_ctx = NULL;
			return -1;
		}

		/* compute the number of converted samples: buffering is avoided
		* ensuring that the output buffer will contain at least all the
		* converted input samples */
		_max_dst_nb_samples = _dst_nb_samples =
			av_rescale_rnd(_src_nb_samples, dst_rate, src_rate, AV_ROUND_UP);

		/* buffer is going to be directly written to a rawaudio file, no alignment */
		_dst_nb_channels = av_get_channel_layout_nb_channels(dst_ch_layout);
		ret = av_samples_alloc_array_and_samples(&_dst_data, &_dst_linesize, _dst_nb_channels,
			_dst_nb_samples, _dst_sample_fmt, 0);
		if (ret < 0)
		{
			LOG_Print_Error(libThirdWrap_FFmpeg, "Audio_swr_convert_fmts16() Could not allocate destination samples!");
			swr_free(&_swr_ctx);
			_swr_ctx = NULL;
			return -1;
		}

		/* compute destination number of samples */
		_dst_nb_samples = av_rescale_rnd(swr_get_delay(_swr_ctx, src_rate) +
			_src_nb_samples, dst_rate, src_rate, AV_ROUND_UP);
		if (_dst_nb_samples > _max_dst_nb_samples)
		{
			av_freep(&_dst_data[0]);
			ret = av_samples_alloc(_dst_data, &_dst_linesize, _dst_nb_channels,
				_dst_nb_samples, _dst_sample_fmt, 1);
			if (ret < 0)
			{
				swr_free(&_swr_ctx);
				_swr_ctx = NULL;
				return -1;
			}

			_max_dst_nb_samples = _dst_nb_samples;
		}

		_iFrameSize = _src_nb_samples * uSrcChannels * 2;

		//�洢4֡
		_sBuffer.reserve(_iFrameSize * 4);

		return 1;
	}

	void FFResampler::Release(void)
	{
		if (_src_data != NULL)
		{
			av_freep(&_src_data[0]);
			av_freep(&_src_data);
			_src_data = NULL;
		}

		if (_dst_data != NULL)
		{
			av_freep(&_dst_data[0]);
			av_freep(&_dst_data);
			_dst_data = NULL;
		}

		if (_swr_ctx != NULL)
		{
			swr_free(&_swr_ctx);
			_swr_ctx = NULL;
		}
	}

	_string_type FFResampler::Push(const char* c_szData, UInt32 uSize)
	{
		int iOffset = 0, iLen = 0, dst_bufsize = 0;
		_sRetData.update_size(0);

		_sBuffer.append(c_szData, uSize);
		if (_sBuffer.size() < _iFrameSize)
		{
			return _string_type();
		}

		do {

			iLen = _sBuffer.size() - iOffset > _iFrameSize ? _iFrameSize : _sBuffer.size() - iOffset;
			memcpy(_src_data[0], &_sBuffer[iOffset], iLen);

			// convert to destination format
			int ret = swr_convert(_swr_ctx, _dst_data, _dst_nb_samples, (const uint8_t **)_src_data, _src_nb_samples);
			if (ret < 0)
			{
				LOG_Print_Error(libThirdWrap_FFmpeg, "Audio_swr_convert_fmts16() Error while converting!");
				goto gt_end;
			}

			dst_bufsize = av_samples_get_buffer_size(&_dst_linesize, _dst_nb_channels,
				ret, _dst_sample_fmt, 1);

			if (dst_bufsize < 0)
			{
				LOG_Print_Error(libThirdWrap_FFmpeg, "Audio_swr_convert_fmts16() Could not get sample buffer size!");
				goto gt_end;
			}

			_sRetData.append((char*)_dst_data[0], dst_bufsize);
		gt_end:
			iOffset += iLen;

		} while (_sBuffer.size() - iOffset >= _iFrameSize);

		std::memmove(&_sBuffer[0], &_sBuffer[iOffset], _sBuffer.size() - iOffset);
		_sBuffer.update_size(_sBuffer.size() - iOffset);

		return _sRetData;
	}

	_string_type Audio_swr_convert_fmts16(int SrcRate, UInt8 uSrcChannels, int DstRate, UInt8 uDstChannels,
		const char* c_szData, UInt64 uSize)
	{
		_string_type sData;
		int64_t src_ch_layout = uSrcChannels == 1 ? AV_CH_LAYOUT_MONO : AV_CH_LAYOUT_STEREO;
		int64_t dst_ch_layout = uDstChannels == 1 ? AV_CH_LAYOUT_MONO : AV_CH_LAYOUT_STEREO;
		int src_rate = SrcRate, dst_rate = DstRate;
		uint8_t **src_data = NULL, **dst_data = NULL;
		int src_nb_channels = 0, dst_nb_channels = 0;
		int src_linesize = 0, dst_linesize = 0;
		int src_nb_samples = 1024, dst_nb_samples, max_dst_nb_samples;
		enum AVSampleFormat src_sample_fmt = AV_SAMPLE_FMT_S16, dst_sample_fmt = /*AV_SAMPLE_FMT_FLTP*/AV_SAMPLE_FMT_S16;
		int dst_bufsize = 0;
		const char *fmt = NULL;
		struct SwrContext *swr_ctx = NULL;
		int ret = 0;

		/* create resampler context */
		swr_ctx = swr_alloc();
		if (!swr_ctx)
		{
			LOG_Print_Error(libThirdWrap_FFmpeg, "Audio_swr_convert_fmts16() Could not allocate resampler context!");
			ret = AVERROR(ENOMEM);
			goto end;
		}

		/* set options */
		av_opt_set_int(swr_ctx, "in_channel_layout", src_ch_layout, 0);
		av_opt_set_int(swr_ctx, "in_sample_rate", src_rate, 0);
		av_opt_set_sample_fmt(swr_ctx, "in_sample_fmt", src_sample_fmt, 0);

		av_opt_set_int(swr_ctx, "out_channel_layout", dst_ch_layout, 0);
		av_opt_set_int(swr_ctx, "out_sample_rate", dst_rate, 0);
		av_opt_set_sample_fmt(swr_ctx, "out_sample_fmt", dst_sample_fmt, 0);

		/* initialize the resampling context */
		if ((ret = swr_init(swr_ctx)) < 0)
		{
			LOG_Print_Error(libThirdWrap_FFmpeg, "Audio_swr_convert_fmts16() Failed to initialize the resampling context!")
			goto end;
		}

		/* allocate source and destination samples buffers */

		src_nb_channels = av_get_channel_layout_nb_channels(src_ch_layout);
		ret = av_samples_alloc_array_and_samples(&src_data, &src_linesize, src_nb_channels,
			src_nb_samples, src_sample_fmt, 0);
		if (ret < 0)
		{
			LOG_Print_Error(libThirdWrap_FFmpeg, "Audio_swr_convert_fmts16() Could not allocate source samples!")
				goto end;
		}

		/* compute the number of converted samples: buffering is avoided
		* ensuring that the output buffer will contain at least all the
		* converted input samples */
		max_dst_nb_samples = dst_nb_samples =
			av_rescale_rnd(src_nb_samples, dst_rate, src_rate, AV_ROUND_UP);

		/* buffer is going to be directly written to a rawaudio file, no alignment */
		dst_nb_channels = av_get_channel_layout_nb_channels(dst_ch_layout);
		ret = av_samples_alloc_array_and_samples(&dst_data, &dst_linesize, dst_nb_channels,
			dst_nb_samples, dst_sample_fmt, 0);
		if (ret < 0)
		{
			LOG_Print_Error(libThirdWrap_FFmpeg, "Audio_swr_convert_fmts16() Could not allocate destination samples!")
				goto end;
		}

		/* compute destination number of samples */
		dst_nb_samples = av_rescale_rnd(swr_get_delay(swr_ctx, src_rate) +
			src_nb_samples, dst_rate, src_rate, AV_ROUND_UP);
		if (dst_nb_samples > max_dst_nb_samples) {
			av_freep(&dst_data[0]);
			ret = av_samples_alloc(dst_data, &dst_linesize, dst_nb_channels,
				dst_nb_samples, dst_sample_fmt, 1);
			if (ret < 0)
			{
				goto end;
			}
			max_dst_nb_samples = dst_nb_samples;
		}

		int iOffset = 0, iLen = 0;

		do {

			iLen = uSize - iOffset > src_nb_samples * uSrcChannels * 2 ? src_nb_samples * uSrcChannels * 2 : uSize - iOffset;
			memcpy(src_data[0], &c_szData[iOffset], iLen);

			iOffset += iLen;
			

			/* convert to destination format */
			ret = swr_convert(swr_ctx, dst_data, dst_nb_samples, (const uint8_t **)src_data, src_nb_samples);
			if (ret < 0)
			{
				LOG_Print_Error(libThirdWrap_FFmpeg, "Audio_swr_convert_fmts16() Error while converting!")
					goto end;
			}
			dst_bufsize = av_samples_get_buffer_size(&dst_linesize, dst_nb_channels,
				ret, dst_sample_fmt, 1);
			if (dst_bufsize < 0)
			{
				LOG_Print_Error(libThirdWrap_FFmpeg, "Audio_swr_convert_fmts16() Could not get sample buffer size!")
					goto end;
			}

			sData.append((char*)dst_data[0], dst_bufsize);

		} while (iOffset != uSize);

		printf("Resampling succeeded. Play the output file with the command:\n"
			"ffplay -f %s -channel_layout  -channels %d -ar %d\n",
			fmt, dst_ch_layout, dst_nb_channels, dst_rate);

	end:

		if (src_data)
			av_freep(&src_data[0]);
		av_freep(&src_data);

		if (dst_data)
			av_freep(&dst_data[0]);
		av_freep(&dst_data);

		swr_free(&swr_ctx);
		return sData;
	}

} //namespace _ffmpeg_

