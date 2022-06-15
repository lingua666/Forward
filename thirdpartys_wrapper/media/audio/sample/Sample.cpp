// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <libFrameWork_Rtsp/RtpPacket.h>
#include <libThirdWrap_FFmpeg/FFResampler.h>
#include <libThirdWrap_FFmpeg/AAC_Encoder.h>
#include <libThirdWrap_FFmpeg/PSPacket.h>
#include <libThirdWrap_Audio/ResamplerConvert.h>
#include <libThirdWrap_Audio/CaptureAudioWindows.h>
#include <libThirdWrap_Audio/RenderAudioWindows.h>

FILE* g_fp = fopen("D:\\audio(8000_1).pcm", "wb");


#ifdef __cplusplus
extern "C"
{
#endif

	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libswresample/swresample.h>
	#include <libavutil/opt.h>

	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>

#ifdef __cplusplus
};
#endif

_ffmpeg_::FFResampler g_FFResampler;
bool				g_isInit = false;
_ffmpeg_::AAC_Encoder	g_AAC;

FILE* g_fp_in = fopen("D:\\audio(8000_1)_.pcm", "rb");
FILE* g_fp_out = fopen("D:\\audio_out.aac", "wb");


FILE* g_fp_pcm = fopen("D:\\audio_dst_20210115.pcm", "wb");

void HandleCaptureAudio(const _audio_::Device_INFO_sptr& sptr,
	BYTE* c_szData, UInt32 uSize)
{
	if (g_fp != NULL)
	{
		/*size_t outLen = 0;
		int16_t* outData = ResamplerConvert((int16_t*)c_szData, uSize / 2, 48000, 16000, 2, outLen);

		fwrite(c_szData, 1, uSize, g_fp);
		//fwrite((char*)outData, outLen, 1, g_fp);
		fflush(g_fp);*/
		if (!g_isInit)
		{
			g_FFResampler.Init(sptr->Format.nSamplesPerSec, sptr->Format.nChannels, 8000, 1);
			g_isInit = true;
		}

		_string_type s = g_FFResampler.Push((char*)c_szData, uSize);
		if (s.size() > 0)
		{
			g_AAC.Encoder(s.c_str(), s.size(), true);
			fwrite(s.c_str(), 1, s.size(), g_fp_pcm);
			fflush(g_fp_pcm);
		}

	}
}

void AACEncoderTest(void)
{
	int ret;
	AVCodec *audio_codec;
	AVCodecContext *c;
	AVFrame *frame;
	AVPacket pkt = { 0 }; // data and size must be 0;
	int got_output;

	/* Initialize libavcodec, and register all codecs and formats. */
	av_register_all();
	avcodec_register_all();
	//avdevice_register_all();

	audio_codec = avcodec_find_encoder(AV_CODEC_ID_AAC);
	c = avcodec_alloc_context3(audio_codec);
	//    c->strict_std_compliance =FF_COMPLIANCE_EXPERIMENTAL;   
	c->codec_id = AV_CODEC_ID_AAC;
	c->sample_fmt = AV_SAMPLE_FMT_FLTP;// AV_SAMPLE_FMT_S16;
	c->sample_rate = 16000;
	c->channels = 2;
	c->channel_layout = AV_CH_LAYOUT_STEREO;
	c->bit_rate = 64000; //16000

	/* Some formats want stream headers to be separate. */
	/*if (c->oformat->flags & AVFMT_GLOBALHEADER)
		c->flags |= CODEC_FLAG_GLOBAL_HEADER;*/

	/* open the codec */
	ret = avcodec_open2(c, audio_codec, NULL);
	if (ret < 0) {
		printf("Could not open video codec: %d\n", ret);
		exit(1);
	}

	/* allocate and init a re-usable frame */
	frame = av_frame_alloc();

	if (!frame) {
		fprintf(stderr, "Could not allocate video frame\n");
		exit(1);
	}

	frame->nb_samples = c->frame_size;
	frame->format = c->sample_fmt;
	frame->channels = c->channels;
	frame->channel_layout = c->channel_layout;
	ret = av_frame_get_buffer(frame, 0);
	if (ret < 0) {
		fprintf(stderr, "Could not allocate an audio frame.\n");
		exit(1);
	}
	printf("----nb_samples= %d, linesize= %d\n", frame->nb_samples, frame->linesize[0]);


	SwrContext *swr = swr_alloc();
	av_opt_set_int(swr, "in_channel_layout", AV_CH_LAYOUT_STEREO, 0);
	av_opt_set_int(swr, "out_channel_layout", AV_CH_LAYOUT_STEREO, 0);
	av_opt_set_int(swr, "in_sample_rate", 48000, 0);
	av_opt_set_int(swr, "out_sample_rate", 16000, 0);
	av_opt_set_sample_fmt(swr, "in_sample_fmt", AV_SAMPLE_FMT_S16, 0);
	av_opt_set_sample_fmt(swr, "out_sample_fmt", AV_SAMPLE_FMT_FLTP, 0);
	swr_init(swr);

	uint8_t *outs[2] = { 0 };
	int len = 4096;

	outs[0] = (uint8_t *)malloc(len);//len 为4096
	outs[1] = (uint8_t *)malloc(len);


	int size = av_samples_get_buffer_size(NULL, c->channels, c->frame_size, c->sample_fmt, 1);
	uint8_t* frame_buf = (uint8_t *)av_malloc(size);
	avcodec_fill_audio_frame(frame, c->channels, c->sample_fmt, (const uint8_t*)frame_buf, size, 1);

	av_init_packet(&pkt);

	printf("frame->nb_samples = %d\n",frame->nb_samples);

	while (1)
	{
		//memset(frame->data[0], 0, frame->linesize[0]);
		ret = fread(frame_buf, 1, size, g_fp_in);
		if (ret <= 0)
		{
			printf("read over !\n");
			break;
		}

		int count = swr_convert(swr, outs, len * 4, (const uint8_t **)&frame_buf, len / 4);//len 为4096
		frame->data[0] = (uint8_t*)outs[0];//audioFrame 是VFrame
		frame->data[1] = (uint8_t*)outs[1];

		//frame->pts = i * 100;
		got_output = 0;

		ret = avcodec_encode_audio2(c, &pkt, frame, &got_output);
		if (ret < 0) {
			printf( "Error encoding audio frame: %d\n", ret);
			break;
		}

		if (got_output > 0)
		{
			//printf("pkt.size = %d\n",pkt.size);
			fwrite(pkt.data, pkt.size, 1, g_fp_out);
			fflush(g_fp_out);
			av_free_packet(&pkt);
		}
	}

	avcodec_close(c);
	av_free(c);
	av_frame_free(&frame);

	return ;

}


int flush_encoder(AVFormatContext *fmt_ctx, unsigned int stream_index) {
	int ret;
	int got_frame;
	AVPacket enc_pkt;
	if (!(fmt_ctx->streams[stream_index]->codec->codec->capabilities &
		AV_CODEC_CAP_DELAY))
		return 0;
	while (1) {
		enc_pkt.data = NULL;
		enc_pkt.size = 0;
		av_init_packet(&enc_pkt);
		ret = avcodec_encode_audio2(fmt_ctx->streams[stream_index]->codec, &enc_pkt,
			NULL, &got_frame);
		av_frame_free(NULL);
		if (ret < 0)
			break;
		if (!got_frame) {
			ret = 0;
			break;
		}
		printf("Flush Encoder: Succeed to encode 1 frame!\tsize:%5d\n", enc_pkt.size);
		/* mux encoded frame */
		ret = av_write_frame(fmt_ctx, &enc_pkt);
		if (ret < 0)
			break;
	}
	return ret;
}

int AACEncoderTest_1(void)
{
	AVFormatContext* pFormatCtx;
	AVOutputFormat* fmt;
	AVStream* audio_st;
	AVCodecContext* pCodecCtx;
	AVCodec* pCodec;

	uint8_t* frame_buf;
	AVFrame* pFrame;
	AVPacket pkt;
	SwrContext *swr;

	int got_frame = 0;
	int ret = 0;
	int size = 0;

	FILE *in_file = NULL;	                        //Raw PCM data
	int framenum = 1000;                          //Audio frame number
	const char* out_file = "D:\\audio_out.aac";          //Output URL
	int i;
	
	in_file = fopen("D:\\audio(8000_2_xx).pcm", "rb");// fopen("D:\\audio(8000_1)_.pcm", "rb");

	av_register_all();

	//Method 1.
	pFormatCtx = avformat_alloc_context();
	fmt = av_guess_format(NULL, out_file, NULL);
	pFormatCtx->oformat = fmt;


	//Method 2.
	//avformat_alloc_output_context2(&pFormatCtx, NULL, NULL, out_file);
	//fmt = pFormatCtx->oformat;

	//Open output URL
	if (avio_open(&pFormatCtx->pb, out_file, AVIO_FLAG_READ_WRITE) < 0) 
	{
		printf("Failed to open output file!\n");
		return -1;
	}

	audio_st = avformat_new_stream(pFormatCtx, 0);
	if (audio_st == NULL) {
		return -1;
	}
	pCodec = avcodec_find_encoder(fmt->audio_codec);
	if (!pCodec) {
		printf("Can not find encoder!\n");
		return -1;
	}

	pCodecCtx = audio_st->codec;
	pCodecCtx->codec_id = fmt->audio_codec;
	pCodecCtx->codec_type = AVMEDIA_TYPE_AUDIO;
	pCodecCtx->sample_fmt = pCodec->sample_fmts[0];
	pCodecCtx->sample_rate = 8000;
	pCodecCtx->channel_layout = AV_CH_LAYOUT_STEREO;
	pCodecCtx->channels = av_get_channel_layout_nb_channels(pCodecCtx->channel_layout);
	pCodecCtx->bit_rate = 64000;
	pCodecCtx->profile = FF_PROFILE_AAC_MAIN;
	pCodecCtx->strict_std_compliance = FF_COMPLIANCE_EXPERIMENTAL;
	//Show some information
	av_dump_format(pFormatCtx, 0, out_file, 1);



	swr = swr_alloc();
	av_opt_set_int(swr, "in_channel_layout", AV_CH_LAYOUT_STEREO, 0);
	av_opt_set_int(swr, "out_channel_layout", AV_CH_LAYOUT_STEREO, 0);
	av_opt_set_int(swr, "in_sample_rate", 8000, 0);
	av_opt_set_int(swr, "out_sample_rate", 8000, 0);
	av_opt_set_sample_fmt(swr, "in_sample_fmt", AV_SAMPLE_FMT_S16, 0);
	av_opt_set_sample_fmt(swr, "out_sample_fmt", AV_SAMPLE_FMT_FLTP, 0);
	swr_init(swr);
	uint8_t *outs[2];
	int len = 4096;

	outs[0] = (uint8_t *)malloc(len);//len 为4096
	outs[1] = (uint8_t *)malloc(len);

	if ((ret = avcodec_open2(pCodecCtx, pCodec, NULL)) < 0) 
	{
		printf("Failed to open encoder!\n");
		return -1;
	}
	pFrame = av_frame_alloc();
	pFrame->nb_samples = pCodecCtx->frame_size;
	pFrame->format = pCodec->sample_fmts[0];

	size = av_samples_get_buffer_size(NULL, pCodecCtx->channels, pCodecCtx->frame_size, pCodecCtx->sample_fmt, 1);
	frame_buf = (uint8_t *)av_malloc(size);
	avcodec_fill_audio_frame(pFrame, pCodecCtx->channels, pCodecCtx->sample_fmt, (const uint8_t*)frame_buf, size, 1);

	//Write Header
	avformat_write_header(pFormatCtx, NULL);

	av_new_packet(&pkt, size);

	for (i = 0; i < framenum; i++) {
		//Read PCM
		if (fread(frame_buf, 1, size, in_file) <= 0) {
			printf("Failed to read raw data! \n");
			return -1;
		}
		else if (feof(in_file)) {
			break;
		}
		int count = swr_convert(swr, outs, len * 4, (const uint8_t **)&frame_buf, len / 4);//len 为4096
		pFrame->data[0] = (uint8_t*)outs[0];//audioFrame 是VFrame
		pFrame->data[1] = (uint8_t*)outs[1];


		//pFrame->data[0] = frame_buf;  //PCM Data
		pFrame->pts = i * 100;
		got_frame = 0;
		//Encode
		ret = avcodec_encode_audio2(pCodecCtx, &pkt, pFrame, &got_frame);
		if (ret < 0) {
			printf("Failed to encode!\n");
			return -1;
		}
		if (got_frame == 1) {
			printf("Succeed to encode 1 frame! \tsize:%5d\n", pkt.size);
			pkt.stream_index = audio_st->index;
			ret = av_write_frame(pFormatCtx, &pkt);
			av_free_packet(&pkt);
		}
	}

	//Flush Encoder
	ret = flush_encoder(pFormatCtx, 0);
	if (ret < 0) {
		printf("Flushing encoder failed\n");
		return -1;
	}

	//Write Trailer
	av_write_trailer(pFormatCtx);

	//Clean
	if (audio_st) {
		avcodec_close(audio_st->codec);
		av_free(pFrame);
		av_free(frame_buf);
	}
	avio_close(pFormatCtx->pb);
	avformat_free_context(pFormatCtx);

	fclose(in_file);

	return 0;
}


void short2float(short* in, void* out, int len) 
{
	register int i;
	for (i = 0; i < len; i++)
		((float*)out)[i] = ((float)(in[i])) / 32767.0;
}

int AACEncoderTest_2(void)
{
	av_register_all();

	AVCodec* mAVCodec = avcodec_find_encoder(AV_CODEC_ID_AAC);//查找AAC编码器
	if (!mAVCodec) {
		printf("encoder AV_CODEC_ID_AAC not found\r\n");
		return -1;
	}
	AVCodecContext * mAVCodecContext = avcodec_alloc_context3(mAVCodec);
	if (mAVCodecContext != NULL) 
	{
		mAVCodecContext->codec_id = AV_CODEC_ID_AAC;
		mAVCodecContext->codec_type = AVMEDIA_TYPE_AUDIO;
		mAVCodecContext->bit_rate = 64000;
		mAVCodecContext->sample_fmt = AV_SAMPLE_FMT_FLTP;
		mAVCodecContext->sample_rate = 8000;
		mAVCodecContext->channel_layout = AV_CH_LAYOUT_STEREO;
		mAVCodecContext->channels = av_get_channel_layout_nb_channels(mAVCodecContext->channel_layout);
	}
	else 
	{
		printf("avcodec_alloc_context3 fail\r\n");
		return -1;
	}

	if (avcodec_open2(mAVCodecContext, mAVCodec, NULL) < 0) 
	{
		printf("aac avcodec open fail\r\n");
		av_free(mAVCodecContext);
		mAVCodecContext = NULL;
		return -1;
	}

	AVFrame *mAVFrame = av_frame_alloc();
	if (!mAVFrame) 
	{
		avcodec_close(mAVCodecContext);
		av_free(mAVCodecContext);
		mAVCodecContext = NULL;
		printf("av_frame_alloc fail\r\n");
		return -1;
	}

	mAVFrame->nb_samples = mAVCodecContext->frame_size;
	mAVFrame->format = mAVCodecContext->sample_fmt;
	mAVFrame->channel_layout = mAVCodecContext->channel_layout;

	int mBufferSize = av_samples_get_buffer_size(NULL, mAVCodecContext->channels, mAVCodecContext->frame_size, mAVCodecContext->sample_fmt, 0);
	if (mBufferSize < 0) 
	{
		printf("av_samples_get_buffer_size fail\r\n");
		av_frame_free(&mAVFrame);
		mAVFrame = NULL;
		avcodec_close(mAVCodecContext);
		av_free(mAVCodecContext);
		mAVCodecContext = NULL;
		printf("mBufferSize fail\r\n");
		return -1;
	}
	uint8_t * mEncoderData = (uint8_t *)av_malloc(mBufferSize);

	if (!mEncoderData)
	{
		printf("av_malloc fail\r\n");
		av_frame_free(&mAVFrame);
		mAVFrame = NULL;
		avcodec_close(mAVCodecContext);
		av_free(mAVCodecContext);
		mAVCodecContext = NULL;
		printf("mEncoderData fail\r\n");
		return -1;
	}

	avcodec_fill_audio_frame(mAVFrame, mAVCodecContext->channels, mAVCodecContext->sample_fmt, (const uint8_t*)mEncoderData, mBufferSize, 0);

	int encode_ret = -1;
	int got_packet_ptr = 0;
	AVPacket pkt;
	av_init_packet(&pkt);
	pkt.data = NULL;
	pkt.size = 0;

	FILE* in_file = fopen("D:\\audio(8000_2_xx).pcm", "rb");// fopen("D:\\audio(8000_1)_.pcm", "rb");
	FILE* Out_file = fopen("D:\\audio_out.aac", "wb");//Output URL "D:\\fopen("D:\\audio(8000_2_xx).pcm", "rb");// fopen("D:\\audio(8000_1)_.pcm", "rb");";

	char pIn[1024 * 900] = { 0 };
	int frameSize = 0;

	while (true)
	{
		frameSize = fread(pIn, 1, 1024 * 2 * 2, in_file) / sizeof(short);
		if (frameSize == 0)
			break;

		printf("start to encode_pcm_data..., frameSize is:%d\r\m", frameSize);
		if (mAVCodecContext && mAVFrame)
		{

			short2float((int16_t *)pIn, mEncoderData, frameSize);
			mAVFrame->data[0] = mEncoderData;
			mAVFrame->pts = 0;
			//音频编码
			encode_ret = avcodec_encode_audio2(mAVCodecContext, &pkt, mAVFrame, &got_packet_ptr);
			printf("encode_pcm_data ----encode_ret:%d,got_packet_ptr:%d\r\n", encode_ret, got_packet_ptr);
			if (encode_ret < 0) {
				printf("Failed to encode!\n");
				return encode_ret;
			}

			printf("encode_pcm_data ----size =%d\r\n", pkt.size);
			void *adts;
			if (pkt.size > 0)
			{
				fwrite(pkt.data, 1, pkt.size, Out_file);
			}
			av_free_packet(&pkt);
		}
	}

	return encode_ret;
}


static int get_format_from_sample_fmt(const char **fmt,
	enum AVSampleFormat sample_fmt)
{
	int i;
	struct sample_fmt_entry {
		enum AVSampleFormat sample_fmt; const char *fmt_be, *fmt_le;
	} sample_fmt_entries[] = {
		{ AV_SAMPLE_FMT_U8,  "u8",    "u8" },
		{ AV_SAMPLE_FMT_S16, "s16be", "s16le" },
		{ AV_SAMPLE_FMT_S32, "s32be", "s32le" },
		{ AV_SAMPLE_FMT_FLT, "f32be", "f32le" },
		{ AV_SAMPLE_FMT_DBL, "f64be", "f64le" },
	};
	*fmt = NULL;

	for (i = 0; i < FF_ARRAY_ELEMS(sample_fmt_entries); i++) {
		struct sample_fmt_entry *entry = &sample_fmt_entries[i];
		if (sample_fmt == entry->sample_fmt) {
			*fmt = AV_NE(entry->fmt_be, entry->fmt_le);
			return 0;
		}
	}

	fprintf(stderr,
		"Sample format %s not supported as output format\n",
		av_get_sample_fmt_name(sample_fmt));
	return AVERROR(EINVAL);
}

/**
* Fill dst buffer with nb_samples, generated starting from t.
*/
static void fill_samples(double *dst, int nb_samples, int nb_channels, int sample_rate, double *t)
{
	int i, j;
	double tincr = 1.0 / sample_rate, *dstp = dst;
	const double c = 2 * M_PI * 440.0;

	/* generate sin tone with 440Hz frequency and duplicated channels */
	for (i = 0; i < nb_samples; i++) {
		*dstp = sin(c * *t);
		for (j = 1; j < nb_channels; j++)
			dstp[j] = dstp[0];
		dstp += nb_channels;
		*t += tincr;
	}
}

void Audio_swr_convert_fmts16_Test(void)
{
	const char *dst_filename = "D:\\2.pcm";

	FILE *dst_file = fopen(dst_filename, "wb");
	if (!dst_file) 
	{
		fprintf(stderr, "Could not open destination file %s\n", dst_filename);
		exit(1);
	}

	char szData[1024 * 900] = { 0 };
	//char szData[4096] = { 0 };
	int src_rate = 48000, dst_rate = 8000;
	int src_nb_channels = 2, dst_nb_channels = 1;
	int src_nb_samples = 1024;

	while (true)
	{
		_ffmpeg_::FFResampler FFResampler;
		int iRet = FFResampler.Init(src_rate, src_nb_channels, dst_rate, dst_nb_channels);
		if (iRet != -1)
		{
			FILE* fp_read = fopen("D:\\audio(48000_2) - 副本.pcm", "rb");
			do {
				/* generate synthetic audio */
				//fill_samples((double *)src_data[0], src_nb_samples, src_nb_channels, src_rate, &t);

				int iLen = fread(szData, 1, sizeof(szData), fp_read);
				if (iLen == 0)
					break;

				//_string_type s = _ffmpeg_::Audio_swr_convert_fmts16(src_rate, src_nb_channels, dst_rate, dst_nb_channels, szData, iLen);

				_string_type s = FFResampler.Push(szData, iLen);

				fwrite(s.c_str(), 1, s.size(), dst_file);
				fflush(dst_file);

			} while (true);

			fclose(fp_read);
			printf("11111\r\n");
		}
	}
}

int _ttmain()
{
	int64_t src_ch_layout = AV_CH_LAYOUT_STEREO, dst_ch_layout = AV_CH_LAYOUT_STEREO;
	int src_rate = 48000, dst_rate = 8000;
	uint8_t **src_data = NULL, **dst_data = NULL;
	int src_nb_channels = 0, dst_nb_channels = 0;
	int src_linesize, dst_linesize;
	int src_nb_samples = 1024, dst_nb_samples, max_dst_nb_samples;
	enum AVSampleFormat src_sample_fmt = AV_SAMPLE_FMT_S16, dst_sample_fmt = AV_SAMPLE_FMT_S16;
	const char *dst_filename = NULL;
	FILE *dst_file;
	int dst_bufsize;
	const char *fmt;
	struct SwrContext *swr_ctx;
	double t;
	int ret;

	dst_filename = "D:\\1.pcm";

	dst_file = fopen(dst_filename, "wb");
	if (!dst_file) {
		fprintf(stderr, "Could not open destination file %s\n", dst_filename);
		exit(1);
	}

	/* create resampler context */
	swr_ctx = swr_alloc();
	if (!swr_ctx) {
		fprintf(stderr, "Could not allocate resampler context\n");
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
	if ((ret = swr_init(swr_ctx)) < 0) {
		fprintf(stderr, "Failed to initialize the resampling context\n");
		goto end;
	}

	/* allocate source and destination samples buffers */

	src_nb_channels = av_get_channel_layout_nb_channels(src_ch_layout);
	ret = av_samples_alloc_array_and_samples(&src_data, &src_linesize, src_nb_channels,
		src_nb_samples, src_sample_fmt, 0);
	if (ret < 0) {
		fprintf(stderr, "Could not allocate source samples\n");
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
	if (ret < 0) {
		fprintf(stderr, "Could not allocate destination samples\n");
		goto end;
	}

	//FILE* fp_read = fopen("D:\\audio(16000_2)_.pcm","rb");
	FILE* fp_read = fopen("D:\\record(48000,2)_1.pcm", "rb");

	t = 0;
	do {
		/* generate synthetic audio */
		//fill_samples((double *)src_data[0], src_nb_samples, src_nb_channels, src_rate, &t);

		int iLen = fread(src_data[0], 1, src_nb_samples * src_nb_channels * 2, fp_read);
		if (iLen == 0)
			break;

		/* compute destination number of samples */
		dst_nb_samples = av_rescale_rnd(swr_get_delay(swr_ctx, src_rate) +
			src_nb_samples, dst_rate, src_rate, AV_ROUND_UP);
		if (dst_nb_samples > max_dst_nb_samples) {
			av_freep(&dst_data[0]);
			ret = av_samples_alloc(dst_data, &dst_linesize, dst_nb_channels,
				dst_nb_samples, dst_sample_fmt, 1);
			if (ret < 0)
				break;
			max_dst_nb_samples = dst_nb_samples;
		}

		/* convert to destination format */
		ret = swr_convert(swr_ctx, dst_data, dst_nb_samples, (const uint8_t **)src_data, src_nb_samples);
		if (ret < 0) {
			fprintf(stderr, "Error while converting\n");
			goto end;
		}
		dst_bufsize = av_samples_get_buffer_size(&dst_linesize, dst_nb_channels,
			ret, dst_sample_fmt, 1);
		if (dst_bufsize < 0) {
			fprintf(stderr, "Could not get sample buffer size\n");
			goto end;
		}
		printf("t:%f in:%d out:%d\n", t, src_nb_samples, ret);
		fwrite(dst_data[0], 1, dst_bufsize, dst_file);
	} while (t < 10);

	if ((ret = get_format_from_sample_fmt(&fmt, dst_sample_fmt)) < 0)
		goto end;
	printf("Resampling succeeded. Play the output file with the command:\n"
		"ffplay -f %s -channel_layout  -channels %d -ar %d %s\n",
		fmt, dst_ch_layout, dst_nb_channels, dst_rate, dst_filename);

end:
	fclose(dst_file);

	if (src_data)
		av_freep(&src_data[0]);
	av_freep(&src_data);

	if (dst_data)
		av_freep(&dst_data[0]);
	av_freep(&dst_data);

	swr_free(&swr_ctx);
	return ret < 0;
}

/* PCM转AAC */
int tsmain()
{
	/* ADTS头 */
	char *padts = (char *)malloc(sizeof(char) * 7);
	char *mp3padts = (char *)malloc(sizeof(char) * 4);
	int profile = 2;    //AAC LC  
	int freqIdx = 4;  //44.1KHz  
	int chanCfg = 2;  //MPEG-4 Audio Channel Configuration. 1 Channel front-center,channel_layout.h  
	padts[0] = (char)0xFF; // 11111111     = syncword  
	padts[1] = (char)0xF1; // 1111 1 00 1  = syncword MPEG-2 Layer CRC  
	padts[2] = (char)(((profile - 1) << 6) + (freqIdx << 2) + (chanCfg >> 2));
	padts[6] = (char)0xFC;

	/*mp3padts[0] = (char)0xFF;
	mp3padts[1] = (char)0xFB;
	mp3padts[2] = (char)0x90;
	mp3padts[3] = (char)0x04;*/
	SwrContext *swr_ctx = NULL;
	AVCodecContext *pCodecCtx = NULL;
	AVCodec *pCodec = NULL;
	AVFrame *pFrame;
	AVPacket pkt;
	AVCodecID codec_id = AV_CODEC_ID_AAC;

	FILE *fp_in;
	FILE *fp_out;
	char filename_in[] = "D:\\audio(8000_1)_.pcm";
	char filename_out[] = "D:\\audio.aac";

	uint8_t **convert_data;         //存储转换后的数据，再编码AAC  
	int i, ret, got_output;
	uint8_t* frame_buf;
	int size = 0;
	int y_size;
	int framecnt = 0;
	int framenum = 100000;

	avcodec_register_all();

	pCodec = avcodec_find_encoder(codec_id);
	if (!pCodec) {
		printf("Codec not found\n");
		return -1;
	}

	pCodecCtx = avcodec_alloc_context3(pCodec);
	if (!pCodecCtx) {
		printf("Could not allocate video codec context\n");
		return -1;
	}

	pCodecCtx->codec_id = codec_id;
	pCodecCtx->codec_type = AVMEDIA_TYPE_AUDIO;
	pCodecCtx->sample_fmt = pCodec->sample_fmts[0];
	pCodecCtx->sample_rate = 8000;
	pCodecCtx->channel_layout = AV_CH_LAYOUT_STEREO;
	pCodecCtx->channels = av_get_channel_layout_nb_channels(pCodecCtx->channel_layout);
	//pCodecCtx->frame_size = 1024;


	if ((ret = avcodec_open2(pCodecCtx, pCodec, NULL)) < 0) {
		printf("avcodec_open2 error ----> %d\r\n", ret);

		printf("Could not open codec\n");
		return -1;
	}
	pCodecCtx->frame_size/* = 1024*/;
	pCodecCtx->bit_rate = 128000;
	pFrame = av_frame_alloc();
	pFrame->nb_samples = pCodecCtx->frame_size;   //1024  
	pFrame->format = pCodecCtx->sample_fmt;
	pFrame->channels = 2;

	/* 由AV_SAMPLE_FMT_S16转为AV_SAMPLE_FMT_FLTP */
	swr_ctx = swr_alloc_set_opts(
		NULL,
		av_get_default_channel_layout(pCodecCtx->channels),
		pCodecCtx->sample_fmt,
		pCodecCtx->sample_rate,
		av_get_default_channel_layout(pCodecCtx->channels),
		AV_SAMPLE_FMT_S16,                      //PCM源文件的采样格式  
		/*pCodecCtx->sample_rate*/8000,
		0, NULL);

	swr_init(swr_ctx);
	/* 分配空间 */
	convert_data = (uint8_t**)calloc(pCodecCtx->channels,
		sizeof(*convert_data));
	av_samples_alloc(convert_data, NULL,
		pCodecCtx->channels, pCodecCtx->frame_size,
		pCodecCtx->sample_fmt, 0);

	size = av_samples_get_buffer_size(NULL, pCodecCtx->channels, pCodecCtx->frame_size, pCodecCtx->sample_fmt, 0);

	frame_buf = (uint8_t *)av_malloc(size);
	/* 此时data[0],data[1]分别指向frame_buf数组起始、中间地址 */
	ret = avcodec_fill_audio_frame(pFrame, pCodecCtx->channels, pCodecCtx->sample_fmt, (const uint8_t*)frame_buf, size, 0);

	if (ret < 0)
	{
		printf("avcodec_fill_audio_frame error\r\n");
		return 0;
	}

	//Input raw data  
	fp_in = fopen(filename_in, "rb");
	if (!fp_in) {
		printf("Could not open %s\n", filename_in);
		return -1;
	}

	//Output bitstream  
	fp_out = fopen(filename_out, "wb");
	if (!fp_out) {
		printf("Could not open %s\n", filename_out);
		return -1;
	}

	//Encode  
	for (i = 0; i < framenum; i++) {
		av_init_packet(&pkt);
		pkt.data = NULL;
		pkt.size = 0;
		//Read raw data  
		if (fread(frame_buf, 1, 4096, fp_in) <= 0) {
			printf("Failed to read raw data! \n");
			return -1;
		}
		else if (feof(fp_in)) {
			break;
		}
		/* 转换数据，令各自声道的音频数据存储在不同的数组（分别由不同指针指向）*/
		pCodecCtx->sample_fmt;
		swr_convert(swr_ctx, convert_data, pCodecCtx->frame_size,
			(const uint8_t**)pFrame->data, pCodecCtx->frame_size);

		/* 将转换后的数据复制给pFrame */
		int length = pCodecCtx->frame_size * av_get_bytes_per_sample(pCodecCtx->sample_fmt);

		memcpy(pFrame->data[0], convert_data[0], length);
		memcpy(pFrame->data[1], convert_data[1], length);

		pFrame->pts = i * 100;

		ret = avcodec_encode_audio2(pCodecCtx, &pkt, pFrame, &got_output);

		if (ret < 0) {
			printf("error encoding\r\n");
			return -1;
		}

		if (pkt.data == NULL)
		{
			av_free_packet(&pkt);
			continue;
		}

		if (got_output) {
			framecnt++;

			padts[3] = (char)(((chanCfg & 3) << 6) + ((7 + pkt.size) >> 11));
			padts[4] = (char)(((7 + pkt.size) & 0x7FF) >> 3);
			padts[5] = (char)((((7 + pkt.size) & 7) << 5) + 0x1F);
			fwrite(padts, 7, 1, fp_out);
			fwrite(pkt.data, 1, pkt.size, fp_out);

			av_free_packet(&pkt);
		}
	}
	//Flush Encoder  
	for (got_output = 1; got_output; i++) {
		ret = avcodec_encode_audio2(pCodecCtx, &pkt, NULL, &got_output);
		if (ret < 0) {
			printf("Error encoding frame\n");
			return -1;
		}
		if (got_output) {

			padts[3] = (char)(((chanCfg & 3) << 6) + ((7 + pkt.size) >> 11));
			padts[4] = (char)(((7 + pkt.size) & 0x7FF) >> 3);
			padts[5] = (char)((((7 + pkt.size) & 7) << 5) + 0x1F);
			// 
			fwrite(padts, 7, 1, fp_out);
			//			fwrite(mp3padts, 4, 1, fp_out);
			fwrite(pkt.data, 1, pkt.size, fp_out);
			av_free_packet(&pkt);
		}
	}

	fclose(fp_out);
	avcodec_close(pCodecCtx);
	av_free(pCodecCtx);
	av_freep(&pFrame->data[0]);
	av_frame_free(&pFrame);

	av_freep(&convert_data[0]);
	free(convert_data);
	return 0;
}

/* PCM转AAC */
int main_()
{
	/* ADTS头 */
	char *padts = (char *)malloc(sizeof(char) * 7);
	char *mp3padts = (char *)malloc(sizeof(char) * 4);
	int profile = 2;    //AAC LC  
	int freqIdx = 8;	//44.1KHz  
	int chanCfg = 2;	//MPEG-4 Audio Channel Configuration. 1 Channel front-center,channel_layout.h  
	padts[0] = (char)0xFF; // 11111111     = syncword  
	padts[1] = (char)0xF1; // 1111 1 00 1  = syncword MPEG-2 Layer CRC  
	padts[2] = (char)(((profile - 1) << 6) + (freqIdx << 2) + (chanCfg >> 2));
	padts[6] = (char)0xFC;

	/*mp3padts[0] = (char)0xFF;
	mp3padts[1] = (char)0xFB;
	mp3padts[2] = (char)0x90;
	mp3padts[3] = (char)0x04;*/
	SwrContext *swr_ctx = NULL;
	AVCodecContext *pCodecCtx = NULL;
	AVCodec *pCodec = NULL;
	AVFrame *pFrame;
	AVPacket pkt;
	AVCodecID codec_id = AV_CODEC_ID_AAC;

	FILE *fp_in;
	FILE *fp_out;
	char filename_in[] = "D:\\audio(16000_2)_.pcm";//"D:\\audio(8000_2_xx).pcm";//  "D:\\record(44100,2).pcm";// "D:\\audio(8000_2_xx).pcm";
	char filename_out[] = "D:\\audio_out.aac";

	uint8_t **convert_data;         //存储转换后的数据，再编码AAC  
	int i, ret, got_output;
	uint8_t* frame_buf;
	int size = 0;
	int y_size;
	int framecnt = 0;
	int framenum = 100000;


	avcodec_register_all();

	pCodec = avcodec_find_encoder(codec_id);
	if (!pCodec) {
		printf("Codec not found\n");
		return -1;
	}

	pCodecCtx = avcodec_alloc_context3(pCodec);
	if (!pCodecCtx) {
		printf("Could not allocate video codec context\n");
		return -1;
	}

	pCodecCtx->codec_id = codec_id;
	pCodecCtx->codec_type = AVMEDIA_TYPE_AUDIO;
	pCodecCtx->sample_fmt = pCodec->sample_fmts[0];
	pCodecCtx->sample_rate = 16000;
	pCodecCtx->channel_layout = AV_CH_LAYOUT_STEREO;
	pCodecCtx->channels = av_get_channel_layout_nb_channels(pCodecCtx->channel_layout);
	//pCodecCtx->frame_size = 1024;


	if ((ret = avcodec_open2(pCodecCtx, pCodec, NULL)) < 0) {
		printf("avcodec_open2 error ----> %d\r\n", ret);

		printf("Could not open codec\n");
		return -1;
	}
	pCodecCtx->frame_size/* = 1024*/;
	pCodecCtx->bit_rate = 64000;
	pFrame = av_frame_alloc();
	pFrame->nb_samples = pCodecCtx->frame_size;   //1024  
	pFrame->format = pCodecCtx->sample_fmt;
	pFrame->channels = 2;

	/* 由AV_SAMPLE_FMT_S16转为AV_SAMPLE_FMT_FLTP */
	swr_ctx = swr_alloc_set_opts(
		NULL,
		av_get_default_channel_layout(pCodecCtx->channels),
		pCodecCtx->sample_fmt,
		pCodecCtx->sample_rate,
		av_get_default_channel_layout(pCodecCtx->channels),
		AV_SAMPLE_FMT_S16,                      //PCM源文件的采样格式  
		/*pCodecCtx->sample_rate*/16000,
		0, NULL);

	swr_init(swr_ctx);
	/* 分配空间 */
	convert_data = (uint8_t**)calloc(pCodecCtx->channels,
		sizeof(*convert_data));
	av_samples_alloc(convert_data, NULL,
		pCodecCtx->channels, pCodecCtx->frame_size,
		pCodecCtx->sample_fmt, 0);

	size = av_samples_get_buffer_size(NULL, pCodecCtx->channels, pCodecCtx->frame_size, pCodecCtx->sample_fmt, 0);

	frame_buf = (uint8_t *)av_malloc(size);
	/* 此时data[0],data[1]分别指向frame_buf数组起始、中间地址 */
	ret = avcodec_fill_audio_frame(pFrame, pCodecCtx->channels, pCodecCtx->sample_fmt, (const uint8_t*)frame_buf, size, 0);

	if (ret < 0)
	{
		printf("avcodec_fill_audio_frame error!\r\n");
		return 0;
	}

	//Input raw data  
	fp_in = fopen(filename_in, "rb");
	if (!fp_in) {
		printf("Could not open %s\n", filename_in);
		return -1;
	}

	//Output bitstream  
	fp_out = fopen(filename_out, "wb");
	if (!fp_out) {
		printf("Could not open %s\n", filename_out);
		return -1;
	}

	//Encode  
	for (i = 0; i < framenum; i++) {
		av_init_packet(&pkt);
		pkt.data = NULL;
		pkt.size = 0;
		//Read raw data  
		if (fread(frame_buf, 1, 4096, fp_in) <= 0) {
			printf("Failed to read raw data! \n");
			return -1;
		}
		else if (feof(fp_in)) {
			break;
		}
		/* 转换数据，令各自声道的音频数据存储在不同的数组（分别由不同指针指向）*/
		pCodecCtx->sample_fmt;
		swr_convert(swr_ctx, convert_data, pCodecCtx->frame_size,
			(const uint8_t**)pFrame->data, pCodecCtx->frame_size);

		/* 将转换后的数据复制给pFrame */
		int length = pCodecCtx->frame_size * av_get_bytes_per_sample(pCodecCtx->sample_fmt);

		memcpy(pFrame->data[0], convert_data[0], length);
		memcpy(pFrame->data[1], convert_data[1], length);

		pFrame->pts = i * 100;

		ret = avcodec_encode_audio2(pCodecCtx, &pkt, pFrame, &got_output);

		if (ret < 0) {
			printf("error encoding\r\n");
			return -1;
		}

		if (pkt.data == NULL)
		{
			av_free_packet(&pkt);
			continue;
		}

		if (got_output) {
			framecnt++;

			padts[3] = (char)(((chanCfg & 3) << 6) + ((7 + pkt.size) >> 11));
			padts[4] = (char)(((7 + pkt.size) & 0x7FF) >> 3);
			padts[5] = (char)((((7 + pkt.size) & 7) << 5) + 0x1F);
			fwrite(padts, 7, 1, fp_out);
			fwrite(pkt.data, 1, pkt.size, fp_out);

			av_free_packet(&pkt);
		}
	}
	//Flush Encoder  
	for (got_output = 1; got_output; i++) {
		ret = avcodec_encode_audio2(pCodecCtx, &pkt, NULL, &got_output);
		if (ret < 0) {
			printf("Error encoding frame\n");
			return -1;
		}
		if (got_output) {

			padts[3] = (char)(((chanCfg & 3) << 6) + ((7 + pkt.size) >> 11));
			padts[4] = (char)(((7 + pkt.size) & 0x7FF) >> 3);
			padts[5] = (char)((((7 + pkt.size) & 7) << 5) + 0x1F);
			// 
			fwrite(padts, 7, 1, fp_out);
			//			fwrite(mp3padts, 4, 1, fp_out);
			fwrite(pkt.data, 1, pkt.size, fp_out);
			av_free_packet(&pkt);
		}
	}

	fclose(fp_out);
	avcodec_close(pCodecCtx);
	av_free(pCodecCtx);
	av_freep(&pFrame->data[0]);
	av_frame_free(&pFrame);

	av_freep(&convert_data[0]);
	free(convert_data);
	return 0;
}



#define EXIT_ON_ERROR(hres, errstring) \
            if(FAILED(hres)) { printf("Exit error: %x. %s\n", hres,errstring);goto Exit;}


#define SAFE_RELEASE(punk) \
            if((NULL != punk)) \
            { (punk)->Release(); (punk)=NULL; }

int Render_main()
{
	IAudioClient *      _AudioClient = NULL;
	IAudioRenderClient *_RenderClient = NULL;

	IMMDevice * _Device = NULL;
	IMMDeviceEnumerator * _DeviceEnumerator = NULL;


	BYTE* pData = NULL; //渲染缓冲区
	HANDLE _AudioSamplesReadyEvent = NULL;


	DWORD flags = 0;
	UINT32 numFramesAvailable = 0;
	UINT32 numFramesPadding = 0;
	UINT32 bufferFrameCount = 0;
	WAVEFORMATEX * _MixFormat = NULL;
	HRESULT hr;

	//打开声音文件
	FILE* file = fopen("D:\\audio(48000_2) - 副本.pcm", "rb");
	if (NULL == file)
	{
		printf("open pcm file failed.\n");
		goto Exit;
	}

	//初始化Com库
	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	EXIT_ON_ERROR(hr, "Com init failed")

		//创建Com对象IMMDeviceEnumerator
		hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, IID_PPV_ARGS(&_DeviceEnumerator));
	EXIT_ON_ERROR(hr, "Create IMMDeviceEnumerator object failed")

		//获取声音播放设备对象IMMDevice
		hr = _DeviceEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &_Device);
	EXIT_ON_ERROR(hr, "Create IMMDevice object failed")

		//创建Com对象IAudioClient
		hr = _Device->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, reinterpret_cast<void **>(&_AudioClient));
	EXIT_ON_ERROR(hr, "Create IAudioClient object failed")

		hr = _AudioClient->GetMixFormat(&_MixFormat);
	EXIT_ON_ERROR(hr, "retrieve audio device mixformat failed")

		//调整采样设备的采样深度到16位
		//https://msdn.microsoft.com/en-us/library/windows/desktop/dd390970(v=vs.85).aspx
		//https://msdn.microsoft.com/en-us/library/windows/desktop/dd390971(v=vs.85).aspx
		/*if (WAVE_FORMAT_EXTENSIBLE == _MixFormat->wFormatTag)
		{
		PWAVEFORMATEXTENSIBLE pEx = reinterpret_cast<PWAVEFORMATEXTENSIBLE>(_MixFormat);
		if (IsEqualGUID(KSDATAFORMAT_SUBTYPE_IEEE_FLOAT, pEx->SubFormat))
		{
		pEx->SubFormat = KSDATAFORMAT_SUBTYPE_PCM;
		pEx->Samples.wValidBitsPerSample = 16;
		_MixFormat->wBitsPerSample = 16;
		_MixFormat->nBlockAlign = _MixFormat->nChannels * _MixFormat->wBitsPerSample / 8;
		_MixFormat->nAvgBytesPerSec = _MixFormat->nBlockAlign * _MixFormat->nSamplesPerSec;
		}
		}
		else if (WAVE_FORMAT_IEEE_FLOAT == _MixFormat->wFormatTag)
		{*/
		_MixFormat->cbSize = 0;
	_MixFormat->wFormatTag = WAVE_FORMAT_PCM;
	_MixFormat->wBitsPerSample = 16;
	_MixFormat->nBlockAlign = _MixFormat->nChannels*_MixFormat->wBitsPerSample / 8;
	_MixFormat->nAvgBytesPerSec = _MixFormat->nBlockAlign*_MixFormat->nSamplesPerSec;
	//}


	//初始化音频引擎
	hr = _AudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_EVENTCALLBACK, 0, 0, _MixFormat, NULL);
	EXIT_ON_ERROR(hr, "Initialize audio engine failed")


		_AudioSamplesReadyEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (_AudioSamplesReadyEvent == NULL)
	{
		printf("Unable to create samples ready event");
		return false;
	}
	hr = _AudioClient->SetEventHandle(_AudioSamplesReadyEvent);
	EXIT_ON_ERROR(hr, "Unable to set ready event")


		hr = _AudioClient->GetService(IID_PPV_ARGS(&_RenderClient));
	EXIT_ON_ERROR(hr, "Unable to get new render client")


		//获取实际分配的缓冲区
		hr = _AudioClient->GetBufferSize(&bufferFrameCount);
	EXIT_ON_ERROR(hr, "Get the actual size of the allocated buffer failed")


		//获取初始操作的整个缓冲区
		//   hr = _RenderClient->GetBuffer(bufferFrameCount, &pData);
		EXIT_ON_ERROR(hr, "retrieve render audio buffer frame count failed")


		/////////////////////////////////////////////////////////////////////////

		IAudioClock* clock = NULL;

	UINT32 bufferLength = 0;
	hr = _AudioClient->GetBufferSize(&bufferLength);
	if (FAILED(hr))
	{
		LOG_Print_Error(libThirdWrap_Audio, "RenderAudioWindows::DoThread() GetBufferSize() Failed!!");
		goto Exit;
	}

	// Get maximum latency for the current stream (will not change for the lifetime of the IAudioClient object).
	//
	REFERENCE_TIME latency = 0;
	_AudioClient->GetStreamLatency(&latency);
	printf("[REND] max stream latency   : %u (%3.2f ms)\r\n",
		(DWORD)latency, (double)(latency / 10000.0));

	// Get the length of the periodic interval separating successive processing passes by
	// the audio engine on the data in the endpoint buffer.
	//
	REFERENCE_TIME devPeriod = 0;
	REFERENCE_TIME devPeriodMin = 0;
	_AudioClient->GetDevicePeriod(&devPeriod, &devPeriodMin);
	printf("[REND] device period        : %u (%3.2f ms)\r\n", (DWORD)devPeriod, (double)(devPeriod / 10000.0));

	UINT64 writtenSamples = 0;
	LONGLONG	playAcc = 0;
	int32_t time = 0;

	UINT32 _recBlockSize = _MixFormat->nSamplesPerSec / 100;

	// Derive initial rendering delay.
	// Example: 10*(960/480) + 15 = 20 + 15 = 35ms
	//
	int playout_delay = 10 * (bufferLength / _recBlockSize) +
		(int)((latency + devPeriod) / 10000);

	writtenSamples = 0;
	printf("[REND] initial delay        : %u", playout_delay);

	double endpointBufferSizeMS = 10.0 * ((double)bufferLength / (double)_recBlockSize);
	printf("[REND] endpointBufferSizeMS : %3.2f", endpointBufferSizeMS);


	// Before starting the stream, fill the rendering buffer with silence.
	//
	hr = _RenderClient->GetBuffer(bufferLength, &pData);
	if (FAILED(hr))
	{
		LOG_Print_Error(libThirdWrap_Audio, "RenderAudioWindows::DoThread() RenderClient GetBuffer() Failed!!");
		goto Exit;
	}

	hr = _RenderClient->ReleaseBuffer(bufferLength, AUDCLNT_BUFFERFLAGS_SILENT);
	if (FAILED(hr))
	{
		LOG_Print_Error(libThirdWrap_Audio, "RenderAudioWindows::DoThread() RenderClient ReleaseBuffer() Failed!!");
		goto Exit;
	}

	writtenSamples += bufferLength;

	hr = _AudioClient->GetService(__uuidof(IAudioClock), (void**)&clock);

	if (FAILED(hr))
	{
		LOG_Print_Error(libThirdWrap_Audio, "RenderAudioWindows::DoThread() failed to get IAudioClock interface from the IAudioClient!!");
		goto Exit;
	}

	/////////////////////////////////////////////////////////////////////////


	hr = _AudioClient->Start();
	EXIT_ON_ERROR(hr, "play audio failed")


		while (AUDCLNT_BUFFERFLAGS_SILENT != flags)
		{
			WaitForSingleObject(_AudioSamplesReadyEvent, INFINITE);


			//获取渲染缓冲区还未播放完的数据帧数
			_AudioClient->GetCurrentPadding(&numFramesPadding);


			if (bufferFrameCount == numFramesPadding) continue;

			//获取空余的缓冲区长度，缓冲区的最大长度是可以设置的
			numFramesAvailable = bufferFrameCount - numFramesPadding;


			//写入数据
			hr = _RenderClient->GetBuffer(numFramesAvailable, &pData);


			//加载数据到pData
			size_t nReadsize = fread_s(pData, numFramesAvailable*_MixFormat->nBlockAlign, sizeof(char), numFramesAvailable*_MixFormat->nBlockAlign, file);
			if (nReadsize == 0)
				break;
			//printf("%s\n", pData);
			printf("%d\n", nReadsize);
			_RenderClient->ReleaseBuffer(numFramesAvailable, flags);
		}


Exit:
	SAFE_RELEASE(_DeviceEnumerator)
		SAFE_RELEASE(_Device)
		SAFE_RELEASE(_DeviceEnumerator)
		SAFE_RELEASE(_DeviceEnumerator)
		SAFE_RELEASE(_DeviceEnumerator)
		CoUninitialize();
	return 0;
}

FILE* g_out_file = fopen("D:\\audio_out202101191723.aac", "wb"); ;          //Output URL
FILE* g_in_file = fopen("D:\\audio_dst_.pcm", "rb");// fopen("D:\\audio(8000_1)_ - 副本.pcm", "rb");// fopen("D:\\audio(8000_2_xx).pcm", "rb");//fopen("D:\\audio(16000_2)_.pcm", "rb"); // fopen("D:\\audio(8000_1)_.pcm", "rb");

void RenderTest(void)
{
	_audio_::RenderAudioWindows		RenderDevice;
	RenderDevice.Init();

	while (true)
	{
		_audio_::Device_INFO_sptr sptr = RenderDevice.GetDefaultDevice();

		tWAVEFORMATEX	Format = { 0 };
		// Set wave format
		Format.wFormatTag = WAVE_FORMAT_PCM;
		Format.wBitsPerSample = 16;
		Format.cbSize = 0;

		const int freqs[7] = { 8000, 32000, 96000, 16000, 24000, 44100, 48000 };
		uint16_t _recChannelsPrioList[2] = { 2, 1 };

		// Iterate over frequencies and channels, in order of priority
		for (unsigned int freq = 0; freq < sizeof(freqs) / sizeof(freqs[0]); freq++)
		{
			for (unsigned int chan = 0; chan < sizeof(_recChannelsPrioList) / sizeof(_recChannelsPrioList[0]); chan++)
			{
				Format.nChannels = _recChannelsPrioList[chan];
				Format.nSamplesPerSec = freqs[freq];
				Format.nBlockAlign = Format.nChannels * Format.wBitsPerSample / 8;
				Format.nAvgBytesPerSec = Format.nSamplesPerSec * Format.nBlockAlign;
				// If the method succeeds and the audio endpoint device supports the specified stream format,
				// it returns S_OK. If the method succeeds and provides a closest match to the specified format,
				// it returns S_FALSE.

				if (RenderDevice.IsFormatSupported(sptr, Format))
				{
					break;
				}
			}
		}

		if (sptr)
		{
			RenderDevice.Start(sptr, Format, _audio_::RenderAudioWindows::HFNRender());
		}

		FILE* fp = fopen("D:\\audio(48000_2) - 副本.pcm", "rb");
		if (fp != NULL)
		{
			while (true)
			{
				char szData[1024] = { 0 };
				int iLen = fread(szData, 1, sizeof(szData), fp);
				if (iLen <= 0)
					break;
				RenderDevice.Play(sptr, szData, iLen);
			};

			fclose(fp);
		}
		Sleep(1000);
		RenderDevice.Stop();
	}

	RenderDevice.Release();

	std::cin.get();
	return ;
}

void HandleEncoder(const UInt8* pData, UInt32 uSize)
{
	fwrite(pData, 1, uSize, g_out_file);
	fflush(g_out_file);
}

void FFMpegAACEncoder(void)
{
	_ffmpeg_::AAC_Encoder	AAC;
	AAC.Init(8000, 1, 16000, HandleEncoder);

	while (true)
	{
		char szData[100] = { 0 };
		int iLen = fread(szData, 1, sizeof(szData), g_in_file);
		if (iLen <= 0)
			break;

		AAC.Encoder(szData, iLen, true);
	}
}

void SpeekTest(void)
{
	g_AAC.Init(8000, 1, 16000, HandleEncoder);

	_audio_::CaptureAudioWindows	Device;

	while (true)
	{
		Device.Init();

		_audio_::Device_INFO_List	List;
		Device.Enumerate(&List);

		_audio_::Device_INFO_sptr sptr;
		for (_audio_::Device_INFO_List::iterator iter = List.begin(); iter != List.end(); iter ++)
		{
			sptr = *iter;
		}

		tWAVEFORMATEX	Fromat = { 0 };
		// Set wave format
		Fromat.wFormatTag = WAVE_FORMAT_PCM;
		Fromat.wBitsPerSample = 16;
		Fromat.cbSize = 0;

		const int freqs[7] = { 8000, 32000, 96000, 16000, 24000, 44100, 48000 };
		uint16_t _recChannelsPrioList[2] = { 1, 2 };

		// Iterate over frequencies and channels, in order of priority
		for (unsigned int freq = 0; freq < sizeof(freqs) / sizeof(freqs[0]); freq++)
		{
			for (unsigned int chan = 0; chan < sizeof(_recChannelsPrioList) / sizeof(_recChannelsPrioList[0]); chan++)
			{
				Fromat.nChannels = _recChannelsPrioList[chan];
				Fromat.nSamplesPerSec = freqs[freq];
				Fromat.nBlockAlign = Fromat.nChannels * Fromat.wBitsPerSample / 8;
				Fromat.nAvgBytesPerSec = Fromat.nSamplesPerSec * Fromat.nBlockAlign;
				// If the method succeeds and the audio endpoint device supports the specified stream format,
				// it returns S_OK. If the method succeeds and provides a closest match to the specified format,
				// it returns S_FALSE.

				if (Device.IsFormatSupported(sptr, Fromat))
				{
					Device.Start(sptr, Fromat, HandleCaptureAudio);

					Sleep(10000000);
					printf("stop\r\n");
					Device.Stop();
				}
			}
		}

		Device.Release();
	}


	while (true)
	{
		Sleep(1000);
	}
}


_string_type GetProgramStreamInfoDesc(const psm_header_t* pHead)
{
	return _string_type((char*)&pHead->program_stream_info_length + sizeof(pHead->program_stream_info_length),
		Conversion16ToByteOrder((*(UInt16*)pHead->program_stream_info_length), EN_BOT_BIG));
}

void GetElementaryStreamInfo(const psm_header_t* pHead)
{
	struct tagElementaryStreamInfo
	{
		unsigned char stream_type;
		unsigned char elementary_stream_id;
		_string_type  desc;
	};
	
	char* pstream_map_length = (char*)&pHead->program_stream_info_length + sizeof(pHead->program_stream_info_length) + Conversion16ToByteOrder((*(UInt16*)pHead->program_stream_info_length), EN_BOT_BIG);
	char* pStart = pstream_map_length + 2;
	UInt16 uMapLength = Conversion16ToByteOrder((*(UInt16*)pstream_map_length), EN_BOT_BIG);

	if (uMapLength > 0)
	{
		UInt16 uOffset = 0;
		while (uOffset < uMapLength)
		{
			tagElementaryStreamInfo Info;
			Info.stream_type = pStart[uOffset];
			Info.elementary_stream_id = pStart[uOffset + 1];
			UInt16 uStrLen = Conversion16ToByteOrder((*(UInt16*)&pStart[uOffset + 2]), EN_BOT_BIG);
			Info.desc = _string_type(&pStart[uOffset + 4], uStrLen);
			uOffset += 4 + uStrLen;
		};
	}
}

void GetPES(const pes_header_t* pPES)
{

}


void HandlePayLoadData(PSStatus Type, const char* c_szData, Int32 uSize)
{
	static FILE* g_fp = fopen("H:\\TestDir\\PS片\\out\\h264.bin", "wb");
	fwrite(c_szData, 1, uSize, g_fp);
	fflush(g_fp);

	//test
	char c_tmData[1024 * 80] = { 0 };
	memcpy(c_tmData, c_szData, uSize);
	int i = 0;
	//test
}

void PSStreamTest(void)
{
	PSPacket Ps;
	Ps.Init(HandlePayLoadData);


	FILE* fp_w = fopen("H:\\TestDir\\PS片\\out\\ps_full.bin", "wb");

	for (int i = 1; i < 108; i++)
	{	
		char szPath[1024] = { 0 };
		sprintf(szPath, "H:\\TestDir\\PS片\\%d.bin", i);
		FILE* fp = fopen(szPath, "rb");
		if (fp != NULL)
		{
			char szData[1024 * 10] = { 0 };
			int iLen = fread(szData, 1, sizeof(szData), fp);
			fclose(fp);

			fwrite(&szData[sizeof(tagRTP_HEAD)], 1, iLen - sizeof(tagRTP_HEAD), fp_w);
		}
	}

	fclose(fp_w);

	for (int i = 1; i < 108; i++)
	{
		printf("i:%d\r\n", i);
		if (i == 94)
		{
			int n = 0;
		}

		char szPath[1024] = { 0 };
		sprintf(szPath, "H:\\TestDir\\PS片\\%d.bin", i);
		FILE* fp = fopen(szPath, "rb");
		if (fp != NULL)
		{
			char szData[1024 * 10] = { 0 };
			int iLen = fread(szData, 1, sizeof(szData), fp);
			fclose(fp);

			int iOffset = 0;

			char szData1[1024 * 10] = { 0 };
			memcpy(szData1, &szData[sizeof(tagRTP_HEAD)], iLen - sizeof(tagRTP_HEAD));

			iOffset += sizeof(tagRTP_HEAD);

			//Ps.Parse((const char*)&szData[iOffset], iLen - iOffset);
			Ps.ParseEx((const char*)&szData[iOffset], iLen - iOffset);

			/*pes_header_t* pPesHead = NULL;

			//ps header
			ps_header_t* pPSHead = (ps_header_t*)(&szData[iOffset]);
			if (is_ps_header(pPSHead))
			{
			iOffset += sizeof(ps_header_t) + pPSHead->pack_stuffing_length;
			sh_header_t* pSHead = (sh_header_t*)&szData[iOffset];
			if (is_sh_header(pSHead))
			{//PS system header
			iOffset += sizeof(pSHead->system_header_start_code) + sizeof(pSHead->header_length) + Conversion16ToByteOrder((*(UInt16*)pSHead->header_length), EN_BOT_BIG);
			psm_header_t* pSMHead = (psm_header_t*)&szData[iOffset];
			if (is_psm_header(pSMHead))
			{//program_stream_map

			//test
			GetProgramStreamInfoDesc(pSMHead);
			GetElementaryStreamInfo(pSMHead);
			//test

			iOffset += sizeof(pSMHead->promgram_stream_map_start_code) + sizeof(pSMHead->program_stream_map_length) + Conversion16ToByteOrder((*(UInt16*)pSMHead->program_stream_map_length), EN_BOT_BIG);
			pPesHead = (pes_header_t*)&szData[iOffset];
			goto gt_PES;
			}
			}
			else
			{
			pPesHead = (pes_header_t*)&szData[iOffset];
			gt_PES:
			pPesHead->PES_packet_length = Conversion16ToByteOrder(pPesHead->PES_packet_length, EN_BOT_BIG);
			optional_pes_header* pOptional = (optional_pes_header*)((char*)&pPesHead->PES_packet_length + sizeof(pPesHead->PES_packet_length));

			unsigned __int64 ll = get_pts(pOptional);

			switch (pes_type(pPesHead))
			{
			case PSStatus::ps_pes_video://视频
			{
			char* pp = (char*)&pOptional->PES_header_data_length + sizeof(pOptional->PES_header_data_length);
			iOffset += sizeof(pes_header_t) + sizeof(optional_pes_header) + pOptional->PES_header_data_length;

			//负载长度
			int iPayLoad = pPesHead->PES_packet_length - sizeof(optional_pes_header) - pOptional->PES_header_data_length;

			int i = 0;
			}
			break;
			case PSStatus::ps_pes_audio://音频
			{

			}
			break;
			case PSStatus::ps_pes_private://私有流
			{

			}
			break;
			default:

			break;
			}
			}
			}*/
		}
	}
}

#include <libThirdWrap_Audio/RenderAudioWindows.h>
#include <libThirdWrap_FFmpeg/DecoderManager.h>
//FILE* g_fp = fopen("D:\\LowTerminal\\resource\\test.pcm", "wb");

_audio_::RenderAudioWindows		g_RenderDevice;
_audio_::Device_INFO_sptr		g_Device_sptr;

void Decode(const _string_type& sData, int Width, int Height)
{
	/*if (g_fp != NULL)
	{
	fwrite((void*)sData.c_str(), 1, sData.size(), g_fp);
	fflush(g_fp);
	}*/

	_string_type s = g_FFResampler.Push((char*)sData.c_str(), sData.size());
	g_RenderDevice.Play(g_Device_sptr, s.c_str(), s.size());
}

void Lib_MP3(void)
{
	g_RenderDevice.Init();

	g_Device_sptr = g_RenderDevice.GetDefaultDevice();

	tWAVEFORMATEX	Format = { 0 };
	// Set wave format
	Format.wFormatTag = WAVE_FORMAT_PCM;
	Format.wBitsPerSample = 16;
	Format.cbSize = 0;

	const int freqs[7] = { 8000, 32000, 96000, 16000, 24000, 44100, 48000 };
	uint16_t _recChannelsPrioList[2] = { 1, 2 };

	// Iterate over frequencies and channels, in order of priority
	for (unsigned int freq = 0; freq < sizeof(freqs) / sizeof(freqs[0]); freq++)
	{
		for (unsigned int chan = 0; chan < sizeof(_recChannelsPrioList) / sizeof(_recChannelsPrioList[0]); chan++)
		{
			Format.nChannels = _recChannelsPrioList[chan];
			Format.nSamplesPerSec = freqs[freq];
			Format.nBlockAlign = Format.nChannels * Format.wBitsPerSample / 8;
			Format.nAvgBytesPerSec = Format.nSamplesPerSec * Format.nBlockAlign;
			// If the method succeeds and the audio endpoint device supports the specified stream format,
			// it returns S_OK. If the method succeeds and provides a closest match to the specified format,
			// it returns S_FALSE.

			if (g_RenderDevice.IsFormatSupported(g_Device_sptr, Format))
			{
				break;
			}
		}
	}

	g_FFResampler.Init(44100, 1, 48000, 2);

	//test
	while (true)
	{
		if (g_Device_sptr)
		{
			g_RenderDevice.Start(g_Device_sptr, Format, _audio_::RenderAudioWindows::HFNRender());
		}

		/*AVFormatContext* format = avformat_alloc_context();
		if (avformat_open_input(&format, "D:\\24ae4084-120b-405e-a9b1-625a4999479e.mp3", NULL, NULL) != 0)
		{
		return NULL;
		}*/

		/*AVFormatContext* format = avformat_alloc_context();
		avformat_open_input_test(&format, "D:\\24ae4084-120b-405e-a9b1-625a4999479e.mp3", NULL, NULL);*/

		_ffmpeg_::HDecoder hDec = NULL;
		int iRead = 0;
		int Count = 0xFFFF;// 1024 * 20;// 1024 * 30000;
		char* szBuffer = new char[Count];
		//FILE* fp = fopen("D:\\LowTerminal\\resource\\48654653-e504-4c98-9517-e7a42c9513e2.mp3", "rb");
		FILE*  fp = fopen("D:\\LowTerminal\\resource\\24ae4084-120b-405e-a9b1-625a4999479e.mp3", "rb");
		//FILE*  fp = fopen("D:\\LowTerminal\\resource\\Speek_out(32000).aac", "rb");
		if (fp != NULL)
		{
			do
			{
				int iRet = fread(szBuffer, 1, Count, fp);
				if (iRet > 0)
				{
					iRead += iRet;
					hDec = Singleton<_ffmpeg_::DecoderManager>::instance()->GetDecoder(szBuffer, iRet,
						function20_bind_Ex(Decode));
					if (hDec != NULL)
						break;
				}
				else
					break;
			} while (true);
		}

		if (hDec != NULL)
		{
			//test
			_ffmpeg_::IDecoder::tagDecoder_INFO* pInfo = Singleton<_ffmpeg_::DecoderManager>::instance()->GetDecoderInfo(hDec);
			if (pInfo != NULL)
			{
				printf("uFormat:%d, uChannels:%d, uSampleRate:%d, uBitRate:%d\r\n",
					((_ffmpeg_::IDecoder::tagAudio_INFO*)pInfo->pInfo)->uFormat,
					((_ffmpeg_::IDecoder::tagAudio_INFO*)pInfo->pInfo)->uChannels,
					((_ffmpeg_::IDecoder::tagAudio_INFO*)pInfo->pInfo)->uSampleRate,
					((_ffmpeg_::IDecoder::tagAudio_INFO*)pInfo->pInfo)->uBitRate);
			}
			//test

			fseek(fp, 0, SEEK_SET);
			do
			{
				int iRet = fread(szBuffer, 1, Count, fp);
				if (iRet > 0)
				{
					iRead += iRet;
					int iSize = Singleton<_ffmpeg_::DecoderManager>::instance()->Decoder(hDec,
						(unsigned char*)szBuffer, iRet);
				}
				else
					break;
			} while (true);
		}

		if (fp != NULL)
		{
			fclose(fp);
			fp = NULL;
		}

		if (g_fp != NULL)
		{
			fclose(g_fp);
			g_fp = NULL;
		}

		if (hDec != NULL)
		{
			Singleton<_ffmpeg_::DecoderManager>::instance()->Close(hDec);
			hDec = NULL;
		}

		if (szBuffer != NULL)
		{
			delete szBuffer;
			szBuffer = NULL;
		}

		g_RenderDevice.Stop();
	}

	while (true)
	{
		Sleep(1000);
	};
	//test
}

void Lib_AAC1(void)
{
	g_RenderDevice.Init();

	g_Device_sptr = g_RenderDevice.GetDefaultDevice();

	tWAVEFORMATEX	Format = { 0 };
	// Set wave format
	Format.wFormatTag = WAVE_FORMAT_PCM;
	Format.wBitsPerSample = 16;
	Format.cbSize = 0;

	const int freqs[7] = { 8000, 32000, 96000, 16000, 24000, 44100, 48000 };
	uint16_t _recChannelsPrioList[2] = { 1, 2 };

	// Iterate over frequencies and channels, in order of priority
	for (unsigned int freq = 0; freq < sizeof(freqs) / sizeof(freqs[0]); freq++)
	{
		for (unsigned int chan = 0; chan < sizeof(_recChannelsPrioList) / sizeof(_recChannelsPrioList[0]); chan++)
		{
			Format.nChannels = _recChannelsPrioList[chan];
			Format.nSamplesPerSec = freqs[freq];
			Format.nBlockAlign = Format.nChannels * Format.wBitsPerSample / 8;
			Format.nAvgBytesPerSec = Format.nSamplesPerSec * Format.nBlockAlign;
			// If the method succeeds and the audio endpoint device supports the specified stream format,
			// it returns S_OK. If the method succeeds and provides a closest match to the specified format,
			// it returns S_FALSE.

			if (g_RenderDevice.IsFormatSupported(g_Device_sptr, Format))
			{
				break;
			}
		}
	}

	g_FFResampler.Init(8000, 1, 48000, 2);

	//test
	while (true)
	{
		if (g_Device_sptr)
		{
			g_RenderDevice.Start(g_Device_sptr, Format, _audio_::RenderAudioWindows::HFNRender());
		}

		/*AVFormatContext* format = avformat_alloc_context();
		if (avformat_open_input(&format, "D:\\24ae4084-120b-405e-a9b1-625a4999479e.mp3", NULL, NULL) != 0)
		{
		return NULL;
		}*/

		/*AVFormatContext* format = avformat_alloc_context();
		avformat_open_input_test(&format, "D:\\24ae4084-120b-405e-a9b1-625a4999479e.mp3", NULL, NULL);*/

		_ffmpeg_::HDecoder hDec = NULL;
		int iRead = 0;
		int Count = 0xFFFF;// 1024 * 20;// 1024 * 30000;
		char* szBuffer = new char[Count];
		//FILE*  fp = fopen("D:\\ActiveX_audio_out——.aac", "rb");
		FILE*  fp = fopen("D:\\LowTerminal\\resource\\Speek_out(32000).aac", "rb");
		if (fp != NULL)
		{
			do
			{
				int iRet = fread(szBuffer, 1, Count, fp);
				if (iRet > 0)
				{
					iRead += iRet;
					hDec = Singleton<_ffmpeg_::DecoderManager>::instance()->GetDecoder(szBuffer, iRet,
						function20_bind_Ex(Decode));
					if (hDec != NULL)
						break;
				}
				else
					break;
			} while (true);
		}

		if (hDec != NULL)
		{
			//test
			_ffmpeg_::IDecoder::tagDecoder_INFO* pInfo = Singleton<_ffmpeg_::DecoderManager>::instance()->GetDecoderInfo(hDec);
			if (pInfo != NULL)
			{
				printf("uFormat:%d, uChannels:%d, uSampleRate:%d, uBitRate:%d\r\n",
					((_ffmpeg_::IDecoder::tagAudio_INFO*)pInfo->pInfo)->uFormat,
					((_ffmpeg_::IDecoder::tagAudio_INFO*)pInfo->pInfo)->uChannels,
					((_ffmpeg_::IDecoder::tagAudio_INFO*)pInfo->pInfo)->uSampleRate,
					((_ffmpeg_::IDecoder::tagAudio_INFO*)pInfo->pInfo)->uBitRate);
			}
			//test

			fseek(fp, 0, SEEK_SET);
			do
			{
				int iRet = fread(szBuffer, 1, Count, fp);
				if (iRet > 0)
				{
					iRead += iRet;
					int iSize = Singleton<_ffmpeg_::DecoderManager>::instance()->Decoder(hDec,
						(unsigned char*)szBuffer, iRet);
				}
				else
					break;
			} while (true);
		}

		if (fp != NULL)
		{
			fclose(fp);
			fp = NULL;
		}

		if (g_fp != NULL)
		{
			fclose(g_fp);
			g_fp = NULL;
		}

		if (hDec != NULL)
		{
			Singleton<_ffmpeg_::DecoderManager>::instance()->Close(hDec);
			hDec = NULL;
		}

		if (szBuffer != NULL)
		{
			delete szBuffer;
			szBuffer = NULL;
		}

		g_RenderDevice.Stop();
	}

	while (true)
	{
		Sleep(1000);
	};
	//test
}

int main(int argc, char* argv[])
{
	//test
	//Lib_MP3();
	//Lib_AAC1();
	//test

	PSStreamTest();

	//Audio_swr_convert_fmts16_Test();

	//tsmain();
	//_ttmain();

	//AACEncoderTest();
	//AACEncoderTest_1();
	//AACEncoderTest_2();
	//main_();

	//while(true)
		//FFMpegAACEncoder();

	//while (true) Sleep(1000);

	//Render_main();
	//SpeekTest();
	//RenderTest();
	

	_audio_::CaptureAudioWindows	CaptureDevice;
	_audio_::RenderAudioWindows		RenderDevice;

	CaptureDevice.Init();
	RenderDevice.Init();

	while (true)
	{
		_audio_::Device_INFO_List	CaptrueList;
		_audio_::Device_INFO_List	RenderList;
		_audio_::Device_INFO_List::iterator iter;
		uint32_t Volume;

		_audio_::Device_INFO_sptr Captrue_sptr = CaptureDevice.GetDefaultDevice();
		printf("Captrue Device Name:%s\r\n", Captrue_sptr->sName.c_str());

		_audio_::Device_INFO_sptr Render_sptr = RenderDevice.GetDefaultDevice();
		printf("Render Device Name:%s\r\n", Render_sptr->sName.c_str());

		CaptureDevice.Enumerate(&CaptrueList);

		for (iter = CaptrueList.begin(); iter != CaptrueList.end(); iter++)
		{
			CaptureDevice.GetVolume(*iter, Volume);
			printf("Captrue Device Name:%s, Volume:%d\r\n", (*iter)->sName.c_str(), Volume);
		}

		RenderDevice.Enumerate(&RenderList);

		for (iter = RenderList.begin(); iter != RenderList.end(); iter++)
		{
			RenderDevice.GetVolume(*iter, Volume);
			RenderDevice.SetVolume(*iter, 255);
			printf("Render Device Name:%s, Volume:%d\r\n", (*iter)->sName.c_str(), Volume);
		}
	}

	std::cin.get();
	return 0;
}
