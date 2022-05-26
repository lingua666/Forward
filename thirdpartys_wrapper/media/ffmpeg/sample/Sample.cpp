// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <libThirdWrap_FFmpeg/H264_Decoder.h>
#include <libThirdWrap_D3D/Shader/D3DManager.h>
#include <libThirdWrap_FFmpeg/DecoderManager.h>
#include <dllThirdWrap_FFmpeg/Exports.h>
#include <libThirdWrap_FFmpeg/Audio_swr_convert.h>
#include <libThirdWrap_FFmpeg/PSPacket.h>


///////////////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <dllThirdWrap_FFmpeg/Exports.h>

//test
FILE* g_fp = fopen("d:\\test.h264", "wb");
//test

//************ Constants
#define TAG_TYPE_AUDIO 8
#define TAG_TYPE_VIDEO 9
#define TAG_TYPE_SCRIPT 18

//*********** TYPEDEFs
typedef unsigned char ui_24[3];
typedef unsigned char byte;
typedef unsigned int uint; 

const UInt8 g_uH264Head[4] = {0x00, 0x00, 0x00, 0x01};

#pragma pack(push, 1)

typedef struct 
{
	byte Signature[3];
	byte Version;
	byte Flags;		//流信息 倒数第1bit是1表示有视频，倒数第3bit是1表示有音频
	UInt32 DataOffset;
} FLV_HEADER;

typedef struct
{
	UInt32 TagType : 8;
	UInt32 DataSize : 24;
	UInt32 Timestamp;
	UInt8 StreamID[3];
} TAG_HEADER;

typedef struct 
{
	UInt8 FrameType : 4;	//帧类型
	UInt8 CodecID : 4;		//编码类型
	UInt8 AvcType;			//AVC类型
	UInt8 Reserved[3];		//保留
}AVC_HEADER;

typedef struct 
{
	UInt8	PPSNum;
	UInt16	PPSSize;
	char	Data[1];
}AVC_PPS_INFO;

typedef struct
{
	UInt8	Reserved[6];
	UInt16	Length;
	char	Data[1];
}AVC_HEAD_INFO;

typedef struct
{
	UInt32	Length;
	char	Data[1];
}AVC_DATA_INFO;

#pragma pack(pop)

enum FLV_CODE_ID_TYPE
{
	FLV_CODE_ID_JPEG = 1,
	FLV_CODE_ID_H263,
	FLV_CODE_ID_SCREEN_VIDEO,
	FLV_CODE_ID_VP6,
	FLV_CODE_ID_VP6_WAC,
	FLV_CODE_ID_SCREEN_VIDEO2,
	FLV_CODE_ID_AVC
};

class FlvParse
{
public:
	enum enSTATUS
	{
		STATUS_HEADER,
		STATUS_AVC_HEAD,
		STATUS_AVC_DATA
	};

public:
	FlvParse( void )
		: _uOffset( 0 )
		, _uSize( 0 )
		, _pData( NULL )
	{
		_pData = new char[0xFFFF];
		memset(_pData, 0, 0xFFFF);
		memset(&_Tag, 0, sizeof(_Tag));
		_iStatus = STATUS_HEADER;
	}

	~FlvParse( void )
	{
		Release();
	}

	int Init( )
	{

	}

	void Release( void )
	{
		if( _pData != NULL )
		{
			delete _pData;
			_pData = NULL;
		}
	}
	
	int Parse( const char* c_szData, UInt16 uSize )
	{
		int iRemain = 0;
		char* pData = (char*)c_szData;

gt_start:
		switch (_iStatus)
		{
		case STATUS_HEADER:
			{
				if( _Tag.DataSize == 0 )
				{//头部信息不完整
					if( _uSize + uSize >= sizeof(FLV_HEADER) )
					{//头部信息完整
						FLV_HEADER* pHead = NULL;
						if( _uSize == 0 )
						{
							pHead = (FLV_HEADER*)c_szData;
							_Tag.DataSize = BigEndian32_UInt32(pHead->DataOffset);
							iRemain = uSize - _Tag.DataSize ;
							if( iRemain > 0 )
								pData = (char*)&c_szData[_Tag.DataSize];
						}
						else
						{
							memcpy(&_pData[_uSize], c_szData, uSize);
							_uSize += uSize;
							pHead = (FLV_HEADER*)_pData;

							_Tag.DataSize = BigEndian32_UInt32(pHead->DataOffset);
							iRemain = _uSize - _Tag.DataSize ;
							if( iRemain > 0 )
								pData = &_pData[_Tag.DataSize];
						}

						if( iRemain >= 0 )
						{//包含完整的头部信息
							_iStatus = STATUS_AVC_HEAD;
							_uSize = _Tag.DataSize = 0;
							if( iRemain > 0 )
							{//拷贝剩余的数据做为下一阶段的数据
								memcpy(_pData, pData, iRemain);
								uSize = iRemain;
								goto gt_start;
							}
						}
					}
					else
					{//头部信息不完整
						memcpy(&_pData[_uSize], c_szData, uSize);
						_uSize += uSize;
					}
				}
				else
				{//头部信息完整
					memcpy(&_pData[_uSize], c_szData, uSize);
					_uSize += uSize;

					if( _uSize >= _Tag.DataSize )
					{
						_iStatus = STATUS_AVC_HEAD;
						iRemain = _uSize - _Tag.DataSize ;
						_uSize = _Tag.DataSize = 0;
						if( iRemain > 0 )
						{
							memcpy(_pData, &_pData[_Tag.DataSize], iRemain);
							uSize = iRemain;
							goto gt_start;
						}
					}
				}
			}
			break;
		case STATUS_AVC_HEAD:
			{
				memcpy(&_pData[_uSize], pData, uSize);
				_uSize += uSize;

				if( _Tag.DataSize == 0 )
				{
					if( _uSize + uSize >= sizeof(TAG_HEADER) + 4 )
					{
						TAG_HEADER* pHead = (TAG_HEADER*)&_pData[4];
						_Tag.TagType = pHead->TagType;
						_Tag.DataSize = BigEndian32_UInt32(pHead->DataSize << 8);
						_Tag.Timestamp = pHead->Timestamp;
						memcpy(_Tag.StreamID, pHead->StreamID, sizeof(pHead->StreamID));
					}

					int iLen = 4 + sizeof(TAG_HEADER) + _Tag.DataSize;
					if( _uSize >= iLen )
					{//完整数据
						iRemain = _uSize - iLen ;

						//获取头部实际长度
						ParseAvcVideo(_Tag, &_pData[sizeof(TAG_HEADER) + 4], _Tag.DataSize);
						_iStatus = STATUS_AVC_DATA;
						_uSize = _Tag.DataSize = 0;
						if( iRemain > 0 )
						{
							pData =  &_pData[iLen];
							uSize = iRemain;
							goto gt_start;
						}
					}
				}
				else
				{
					int iLen = sizeof(TAG_HEADER) + 4 + _Tag.DataSize;
					if( _uSize >= iLen )
					{//完整数据
						iRemain = _uSize - iLen ;
						ParseAvcVideo(_Tag, &_pData[sizeof(TAG_HEADER) + 4], _Tag.DataSize);
						_iStatus = STATUS_AVC_DATA;
						_uSize = _Tag.DataSize = 0;
						if( iRemain > 0 )
						{
							pData =  &_pData[iLen];
							uSize = iRemain;
							goto gt_start;
						}
					}
				}
			}
			break;
		case STATUS_AVC_DATA:
			{
				memcpy(&_pData[_uSize], pData, uSize);
				_uSize += uSize;

				if( _Tag.DataSize == 0 )
				{
					if( _uSize >= sizeof(TAG_HEADER) + 4 )
					{
						TAG_HEADER* pHead = (TAG_HEADER*)&_pData[4];
						_Tag.TagType = pHead->TagType;
						_Tag.DataSize = BigEndian32_UInt32(pHead->DataSize << 8);
						_Tag.Timestamp = pHead->Timestamp;
						memcpy(_Tag.StreamID, pHead->StreamID, sizeof(pHead->StreamID));
					}

					int iLen = sizeof(TAG_HEADER) + 4 + _Tag.DataSize;
					if( _uSize >= iLen )
					{//完整数据
						iRemain = _uSize - iLen ;
						ParseAvcVideo(_Tag, &_pData[sizeof(TAG_HEADER) + 4], _Tag.DataSize);

						_uSize = _Tag.DataSize = 0;
						if( iRemain > 0 )
						{
							pData =  &_pData[iLen];
							uSize = iRemain;
							goto gt_start;
						}
					}
				}
				else
				{
					int iLen = sizeof(TAG_HEADER) + 4 + _Tag.DataSize;
					if( _uSize >= iLen )
					{//完整数据
						iRemain = _uSize - iLen ;
						ParseAvcVideo(_Tag, &_pData[sizeof(TAG_HEADER) + 4], _Tag.DataSize);

						_uSize = _Tag.DataSize = 0;
						if( iRemain > 0 )
						{
							pData =  &_pData[iLen];
							uSize = iRemain;
							goto gt_start;
						}
					}
				}
			}
			break;
		}

		return 1;
	}

	int ParseAvcVideo( const TAG_HEADER& TagHead, const char* c_szData, UInt32 uSize )
	{
		switch(TagHead.TagType)
		{
		case 0x8://音频数据

			break;
		case 0x9://视频数据
			{
				UInt8 uCodeID = c_szData[0] & 0x0F;
				switch( uCodeID )
				{
				case 7: //AVC1
					{
						AVC_HEADER* pHead = (AVC_HEADER*)c_szData;
						if( pHead->AvcType == 0 )
						{//携带pps信息
							//AVC_FIRST_PACKET* pPacket = (AVC_FIRST_PACKET*)&c_szData[sizeof(AVC_HEADER)];
							AVC_HEAD_INFO* pHead = (AVC_HEAD_INFO*)&c_szData[sizeof(AVC_HEADER)];
							int iHeadLen = BigEndian16_UInt16(pHead->Length);

							UInt32 uLen = sizeof(AVC_HEADER) + sizeof(AVC_HEAD_INFO) - 1 + iHeadLen + sizeof(AVC_PPS_INFO) - 1;
							if( uSize < uLen )
							{//数据不完整
								return uLen;
							}

							AVC_PPS_INFO* pPPS = (AVC_PPS_INFO*)(pHead->Data + iHeadLen);
							int iPPSLen = BigEndian16_UInt16(pPPS->PPSSize);

							uLen += iPPSLen;
							if( uSize < uLen  )
							{//数据不完整
								return uLen;
							}

							//获取AVC头部信息
							//回调数据
							fwrite(g_uH264Head, 1, sizeof(g_uH264Head), g_fp); //填充H264标准头
							fwrite(pHead->Data, 1, iHeadLen,  g_fp);
							fflush(g_fp);

							//获取AVC PPS信息
							//回调数据
							fwrite(g_uH264Head, 1, sizeof(g_uH264Head), g_fp); //填充H264标准头
							fwrite(pPPS->Data, 1, iPPSLen,  g_fp);
							fflush(g_fp); 
						}
						else
						{//
							int PendingSize = 0;
							while( PendingSize + 2 + 3 < TagHead.DataSize )
							{//可能存在多个nal，需全部读取  
								AVC_DATA_INFO* pData = (AVC_DATA_INFO*)&c_szData[sizeof(AVC_HEADER) + PendingSize];
								int iDataLen = BigEndian32_UInt32(pData->Length);

								//回调数据
								fwrite(g_uH264Head, 1, sizeof(g_uH264Head), g_fp); //填充H264标准头
								fwrite(pData->Data, 1, iDataLen,  g_fp);
								fflush(g_fp);

								PendingSize += iDataLen + sizeof(AVC_DATA_INFO) - 1;

								//test
								printf("iDataLen:%d\r\n", iDataLen);
								//test
							}
						}
					}
					break;
				case 4:
					{
						_string_type s;
						s.append( c_szData + 2, TagHead.DataSize - 2);
					}
					break;
				default:
					{
						
					}
				}
			}
			break;
		case 0x12://脚本数据

			break;
		}

		return -1;
	}


private:
	UInt16	_uOffset;
	UInt16	_uSize;
	char*	_pData;
	int		_iStatus;
	TAG_HEADER	_Tag;
};
///////////////////////////////////////////////////////////////////////////////////////////////









_d3d_::CD3DManager	g_D3DManager;
_d3d_::HIMAGE		g_Display;
_d3d_::HIMAGE		g_Display1;

FILE* g_wfile = NULL;

int g_inc = 0;

void Decode(const _string_type& sData, int Width, int Height)
{
	//printf("%d\r\n", g_inc ++);
	//fwrite((void*)sData.c_str(), 1, sData.size(), g_wfile);

	g_D3DManager.UpdateImage( g_Display, (BYTE*)sData.c_str() ) ;
	g_D3DManager.BeginRender () ;
	//g_D3DManager.DrawImage ( g_Display, 0.5f, GS_NONE ) ;
	//g_D3DManager.Draw(g_Display, 0, 0);
	g_D3DManager.Draw_H(g_Display, GetConsoleWindow());
	//g_D3DManager.DrawVariety(g_Display,0,0,1.0,50,1.0);
	/*RECT Rect = {0,0,100,100};
	g_D3DManager.DrawVariety(g_Display,0,0,Rect,1.0,50,0.5);*/
	/*g_D3DManager.DrawVariety(g_Display,0,100,Rect,1.0,50,0.5);
	g_D3DManager.DrawVariety(g_Display,0,200,Rect,1.0,50,0.5);*/
	g_D3DManager.EndRender (GetConsoleWindow()) ;
}

int DrawPic( void )
{
	_d3d_::CD3DManager	D3DManager;
	_d3d_::HIMAGE		Display;

	//test
	/*RECT WRect = {0};
	::GetWindowRect(GetConsoleWindow(), &WRect);
	long lWidth = WRect.right - WRect.left; //ImageInfo.lWidth
	long lHeight = WRect.bottom - WRect.top;//ImageInfo.lHeight
	//test

	if( D3DManager.CreateDevice(GetConsoleWindow(), lWidth, lHeight) != 1 )
	{
		return -1;
	}

	while(true)
	{
		Display = D3DManager.CreateImage("D:\\logo2_alpha(120_182).bmp");
		D3DManager.BeginRender (true) ;
		D3DManager.Draw(Display, 0, 0, 1.0);
		D3DManager.EndRender () ;

		D3DManager.DestroyImage(Display);
		Sleep(10);
	};*/

		{
		//test
		RECT WRect = {0};
		::GetWindowRect(GetConsoleWindow(), &WRect);
		long lWidth = WRect.right - WRect.left; //ImageInfo.lWidth
		long lHeight = WRect.bottom - WRect.top;//ImageInfo.lHeight
		//test

		if( g_D3DManager.CreateDevice(GetConsoleWindow(), lWidth, lHeight) != 1 )
		{
			return -1;
		}

		g_Display = g_D3DManager.CreateImage("D:\\logo2_alpha(120_182).bmp");
		g_Display1 = g_D3DManager.CreateImage("D:\\logo2.bmp");

		while( true )
		{
			g_D3DManager.BeginRender (true) ;
			g_D3DManager.Draw(g_Display1, 0, 0, 1.0);
			g_D3DManager.Draw(g_Display, 100, 0, 1.0);
			//g_D3DManager.Draw(g_Display, 50, 0, 0.5);
			//g_D3DManager.Draw(g_Display, 50, 0, 1.0);
			//g_D3DManager.Draw_H(g_Display, GetConsoleWindow());
			//g_D3DManager.DrawVariety(g_Display,0,0,1.0,50,1.0);
			/*RECT Rect = {0,0,100,100};
			g_D3DManager.DrawVariety(g_Display,0,0,Rect,1.0,50,0.5);
			g_D3DManager.DrawVariety(g_Display,0,100,Rect,1.0,50,0.5);*/
			g_D3DManager.DrawVariety(g_Display,0,0,1.0,50,1.0);
			g_D3DManager.EndRender () ;
			Sleep(10);
		};
	}
}

//test
#define PROBE_BUF_MAX 1 << 20
#define PROBE_BUF_MIN 2048

AVInputFormat *av_probe_input_format2(AVProbeData *pd, int is_opened, int *score_max)
{
	int score_ret;
	AVInputFormat *fmt = av_probe_input_format3(pd, is_opened, &score_ret);
	if (score_ret > *score_max) {
		*score_max = score_ret;
		return fmt;
	} else
		return NULL;
}

int ff_av_probe_input_buffer2(AVIOContext *pb,    
	AVInputFormat **fmt,    
	const char *filename,    
	void *logctx,    
	unsigned int offset,    
	unsigned int max_probe_size)    
{    
	AVProbeData pd = { filename ? filename : "" };
	uint8_t *buf = NULL;
	int ret = 0, probe_size, buf_offset = 0;
	int score = 0;
	int ret2;

	if (!max_probe_size)
		max_probe_size = PROBE_BUF_MAX;
	else if (max_probe_size < PROBE_BUF_MIN) {
		av_log(logctx, AV_LOG_ERROR,
			"Specified probe size value %u cannot be < %u\n", max_probe_size, PROBE_BUF_MIN);
		return AVERROR(EINVAL);
	}

	if (offset >= max_probe_size)
		return AVERROR(EINVAL);

	if (pb->av_class) {
		uint8_t *mime_type_opt = NULL;
		char *semi;
		//av_opt_get(pb, "mime_type", AV_OPT_SEARCH_CHILDREN, &mime_type_opt);
		pd.mime_type = (const char *)mime_type_opt;
		semi = pd.mime_type ? strchr((char*)pd.mime_type, ';') : NULL;
		if (semi) {
			*semi = '\0';
		}
	}
#if 0
	if (!*fmt && pb->av_class && av_opt_get(pb, "mime_type", AV_OPT_SEARCH_CHILDREN, &mime_type) >= 0 && mime_type) {
		if (!av_strcasecmp(mime_type, "audio/aacp")) {
			*fmt = av_find_input_format("aac");
		}
		av_freep(&mime_type);
	}
#endif

	for (probe_size = PROBE_BUF_MIN; probe_size <= max_probe_size && !*fmt;
		probe_size = FFMIN(probe_size << 1,
		FFMAX(max_probe_size, probe_size + 1))) {
			score = probe_size < max_probe_size ? AVPROBE_SCORE_RETRY : 0;

			/* Read probe data. */
			if ((ret = av_reallocp(&buf, probe_size + AVPROBE_PADDING_SIZE)) < 0)
				goto fail;
			if ((ret = avio_read(pb, buf + buf_offset,
				probe_size - buf_offset)) < 0) {
					/* Fail if error was not end of file, otherwise, lower score. */
					if (ret != AVERROR_EOF)
						goto fail;

					score = 0;
					ret   = 0;          /* error was end of file, nothing read */
			}
			buf_offset += ret;
			if (buf_offset < offset)
				continue;
			pd.buf_size = buf_offset - offset;
			pd.buf = &buf[offset];

			memset(pd.buf + pd.buf_size, 0, AVPROBE_PADDING_SIZE);

			/* Guess file format. */
			*fmt = av_probe_input_format2(&pd, 1, &score);
			if (*fmt) {
				/* This can only be true in the last iteration. */
				if (score <= AVPROBE_SCORE_RETRY) {
					av_log(logctx, AV_LOG_WARNING,
						"Format %s detected only with low score of %d, "
						"misdetection possible!\n", (*fmt)->name, score);
				} else
					av_log(logctx, AV_LOG_DEBUG,
					"Format %s probed with size=%d and score=%d\n",
					(*fmt)->name, probe_size, score);
#if 0
				FILE *f = fopen("probestat.tmp", "ab");
				fprintf(f, "probe_size:%d format:%s score:%d filename:%s\n", probe_size, (*fmt)->name, score, filename);
				fclose(f);
#endif
			}
	}

	if (!*fmt)
		ret = AVERROR_INVALIDDATA;

fail:
	/* Rewind. Reuse probe buffer to avoid seeking. */
	/*ret2 = ffio_rewind_with_probe_data(pb, &buf, buf_offset);
	if (ret >= 0)
		ret = ret2;*/

	av_freep(&pd.mime_type);
	return ret < 0 ? ret : score; 
}  

static int ff_init_input(AVFormatContext *s, const char *filename,  
	AVDictionary **options)  
{  
	int ret;
	AVProbeData pd = { filename, NULL, 0 };
	int score = AVPROBE_SCORE_RETRY;

	if (s->pb) {
		s->flags |= AVFMT_FLAG_CUSTOM_IO;
		if (!s->iformat)
			return av_probe_input_buffer2(s->pb, &s->iformat, filename,
			s, 0, s->format_probesize);
		else if (s->iformat->flags & AVFMT_NOFILE)
			av_log(s, AV_LOG_WARNING, "Custom AVIOContext makes no sense and "
			"will be ignored with AVFMT_NOFILE format.\n");
		return 0;
	}

	if ((s->iformat && s->iformat->flags & AVFMT_NOFILE) ||
		(!s->iformat && (s->iformat = av_probe_input_format2(&pd, 0, &score))))
		return score;

	if ((ret = s->io_open(s, &s->pb, filename, AVIO_FLAG_READ | s->avio_flags, options)) < 0)
		return ret;

	if (s->iformat)
		return 0;
	return ff_av_probe_input_buffer2(s->pb, &s->iformat, filename,
		s, 0, s->format_probesize);
}

int av_opt_set_dict2(void *obj, AVDictionary **options, int search_flags)
{
	AVDictionaryEntry *t = NULL;
	AVDictionary    *tmp = NULL;
	int ret = 0;

	if (!options)
		return 0;

	while ((t = av_dict_get(*options, "", t, AV_DICT_IGNORE_SUFFIX))) {
		ret = 0;//av_opt_set(obj, t->key, t->value, search_flags);
		if (ret == AVERROR_OPTION_NOT_FOUND)
			ret = av_dict_set(&tmp, t->key, t->value, 0);
		if (ret < 0) {
			av_log(obj, AV_LOG_ERROR, "Error setting option %s to value %s.\n", t->key, t->value);
			av_dict_free(&tmp);
			return ret;
		}
		ret = 0;
	}
	av_dict_free(options);
	*options = tmp;
	return ret;
}

int av_opt_set_dict(void *obj, AVDictionary **options)
{
	return av_opt_set_dict2(obj, options, 0);
}

int ff_avformat_open_input(AVFormatContext **ps, const char *filename,
	AVInputFormat *fmt, AVDictionary **options)
{
	AVFormatContext *s = *ps;
	int i, ret = 0;
	AVDictionary *tmp = NULL;

	if (!s && !(s = avformat_alloc_context()))
		return AVERROR(ENOMEM);
	if (!s->av_class) {
		av_log(NULL, AV_LOG_ERROR, "Input context has not been properly allocated by avformat_alloc_context() and is not NULL either\n");
		return AVERROR(EINVAL);
	}
	if (fmt)
		s->iformat = fmt;

	if (options)
		av_dict_copy(&tmp, *options, 0);

	if (s->pb) // must be before any goto fail
		s->flags |= AVFMT_FLAG_CUSTOM_IO;

	if ((ret = av_opt_set_dict(s, &tmp)) < 0)
		goto fail;

	if ((ret = ff_init_input(s, filename, &tmp)) < 0)
		goto fail;
	s->probe_score = ret;

fail:
	return -1;
}

/*******************************************************/
//test

void Lib_FFMpeg( void )
{
	while(true)
	{
		_ffmpeg_::DecoderManager	DecoderMan;
		_ffmpeg_::HDecoder hDec = NULL;

		const int iLen = 2048;// * 10;
		_ffmpeg_::H264Decoder	Dec;
		char* szData = new char[iLen];
		//char szData[iLen + 1] = {0};

		//FILE* fp = fopen("D:\\111_mo.h264","rb");
		//FILE* fp = fopen("d:\\海康样本_不带头.h264","rb");
		//FILE* fp = fopen("C:\\Users\\gzcw\\Desktop\\H264\\样本\\海康(onvif_rtsp)_test.264","rb");
		//FILE* fp = fopen("C:\\Users\\gzcw\\Desktop\\H264\\样本\\test(天地伟业).265","rb");
		FILE* fp = fopen("F:\\安装包\\装机必装\\编程软件\\辅助工具\\音视频相关\\样本\\H265\\test3.h265","rb");

		//FILE* fp = fopen("C:\\Users\\gzcw\\Desktop\\H264\\样本\\test(利凌).264","rb");
		//FILE* fp = fopen("C:\\Users\\gzcw\\Desktop\\test.264","rb");
		if( fp == NULL )
			goto gt_end;
		
		int iSize = 0;
		do
		{
			iSize = fread(szData, 1, iLen , fp);
			if( iSize <= 0 )
			{
				fclose(fp);
				goto gt_end;
			}

			hDec = DecoderMan.GetDecoder(szData, iSize, function20_bind_Ex(Decode));
			if( hDec != NULL )
				break;
		}while(true);

		//if( Dec.Init((unsigned char*)szData, iSize, function20_bind_Ex(Decode)) == -1 )
		//if( Dec.GetCodecContextToData((unsigned char*)szData, iSize, NULL) )
		/*if( Dec.Init(800,600, 25 , function20_bind_Ex(Decode)) == -1 )
		{
			fclose(fp);
			goto gt_end;
		}*/

		_ffmpeg_::IDecoder::tagDecoder_INFO* ImageInfo = DecoderMan.GetDecoderInfo(hDec);

		//test
		RECT WRect = {0};
		::GetWindowRect(GetConsoleWindow(), &WRect);
		long lWidth = WRect.right - WRect.left; //ImageInfo.lWidth
		long lHeight = WRect.bottom - WRect.top;//ImageInfo.lHeight
		//test

		HWND hcon = GetConsoleWindow();
		HDC hdcScreen = GetDC( NULL ); 
		HWND hwnd1 = WindowFromDC(hdcScreen);

		RECT WRect1 = {0};
		::GetWindowRect(hwnd1, &WRect1);
		long lWidth1 = WRect1.right - WRect1.left; //ImageInfo.lWidth
		long lHeight1 = WRect1.bottom - WRect1.top;//ImageInfo.lHeight

		HWND hwnd = GetConsoleWindow();
		if( g_D3DManager.CreateDevice(hwnd1, lWidth1, lHeight1) != 1 )
		//if( g_D3DManager.CreateDevice(hwnd, lWidth, lHeight) != 1 )
		{
			return ;
		}

		RECT Rect = {0};
		Rect.left = Rect.top = 0;
		Rect.right = ((_ffmpeg_::IDecoder::tagVideo_INFO*)ImageInfo->pInfo)-> uWidth;
		Rect.bottom = ((_ffmpeg_::IDecoder::tagVideo_INFO*)ImageInfo->pInfo)->uHeight;

		g_Display = g_D3DManager.CreateImage(((_ffmpeg_::IDecoder::tagVideo_INFO*)ImageInfo->pInfo)->uWidth,
			((_ffmpeg_::IDecoder::tagVideo_INFO*)ImageInfo->pInfo)->uHeight, _d3d_::CS_YV12);

		//g_wfile = fopen("D:\\YV12.txt", "wb+");

		fseek(fp, 0, SEEK_SET);

		do
		{
			iSize = fread(szData, 1, 1024, fp);
			if( iSize > 0 )
				DecoderMan.Decoder(hDec, (unsigned char*)szData, iSize);
		} while ( iSize  > 0 );

		printf("finish\r\n");
		if( g_wfile != NULL )
			fclose(g_wfile);

		if( fp != NULL )
			fclose(fp);

		DecoderMan.Destroy(hDec);
		g_D3DManager.DestroyImage(g_Display);
		g_D3DManager.Destroy();
		delete szData;
	}

gt_end:
	g_D3DManager.Destroy();
}

void Dll_FFMpeg1( void )
{
	while(true)
	{
		MDecoder_Video_Init();

		const int iLen = 2048;// * 10;
		char* szData = new char[iLen];

		//FILE* fp = fopen("D:\\111_mo.h264","rb");
		//FILE* fp = fopen("d:\\海康样本_不带头.h264","rb");
		//FILE* fp = fopen("D:\\安装包\\装机必装\\编程软件\\辅助工具\\音视频相关\\样本\\H264\\海康(onvif_rtsp)_test.264","rb");
		//FILE* fp = fopen("F:\\安装包\\装机必装\\编程软件\\辅助工具\\音视频相关\\样本\\H265\\test3.h265","rb");
		//FILE* fp = fopen("C:\\Users\\Tranwit\\Desktop\\桌面\\H264\\样本\\test(天地伟业).265","rb");
		//FILE* fp = fopen("C:\\Users\\gzcw\\Desktop\\H264\\样本\\test(利凌).264","rb");
		//FILE* fp = fopen("C:\\Users\\gzcw\\Desktop\\test.264","rb");
		//FILE* fp = fopen("G:\\RecrodDir\\HK_123456789_0\\HK_123456789_1\\0_20170420160347043.record","rb");
		//FILE* fp = fopen("D:\\rtsp_client.h264","rb");
		//FILE* fp = fopen("C:\\Users\\Tranwit\\Downloads\\test3.h265","rb");
		//FILE* fp = fopen("D:\RtspClient.h264","rb");
		FILE* fp = fopen("F:\\ProjectCode\\build\\x64\\NetControl\\bin\\Debug\\128x128.264","rb");
		if( fp == NULL )
			goto gt_end;

		THandle hDec = NULL;
		int iSize = 0;

		//hDec = MDecoder_Get(1920, 1080, 25, EN_TYPE_H264);
		/*hDec = MDecoder_Get(1280, 720, 25, EN_TYPE_H265);
		//hDec = MDecoder_Get(1024, 768, 25, EN_TYPE_H264);
		if( hDec != NULL )
		{
			MDecoder_Set(hDec, GetConsoleWindow());
			//MDecoder_SetSpeed(hDec, 5);
		}*/

		do 
		{
			iSize = fread(szData, 1, iLen, fp);
			hDec = MDecoder_Parse(szData, iSize);
			if( hDec != NULL )
			{
				MDecoder_Set(hDec, GetConsoleWindow());
				break;
			}
		} while ( iSize  > 0 );

		fseek(fp, 0, SEEK_SET);

		do
		{
			iSize = fread(szData, 1, 1024, fp);
			if( iSize > 0 )
				MDecoder_Decode(hDec, szData, iSize);
		} while ( iSize  > 0 );

		printf("finish\r\n");
		if( g_wfile != NULL )	
			fclose(g_wfile);

		if( fp != NULL )
			fclose(fp);

		MDecoder_Close(hDec);
		MDecoder_Video_Free();
		delete szData;
	}

gt_end:
	MDecoder_Video_Free();
}

void Dll_FFMpeg( void )
{
	while(true)
	{
		MDecoder_Video_Init();

		const int iLen = 2048;// * 10;
		char* szData = new char[iLen];

		//FILE* fp = fopen("D:\\111_mo.h264","rb");
		//FILE* fp = fopen("d:\\海康样本_不带头.h264","rb");
		//FILE* fp = fopen("C:\\Users\\gzcw\\Desktop\\H264\\样本\\海康(onvif_rtsp)_test.264","rb");
		//FILE* fp = fopen("C:\\Users\\Tranwit\\Desktop\\桌面\\H264\\样本\\test(天地伟业).265","rb");
		//FILE* fp = fopen("C:\\Users\\gzcw\\Desktop\\H264\\样本\\test(利凌).264","rb");
		//FILE* fp = fopen("C:\\Users\\gzcw\\Desktop\\test.264","rb");
		//FILE* fp = fopen("G:\\RecrodDir\\HK_123456789_0\\HK_123456789_1\\0_20170420160347043.record","rb");
		//FILE* fp = fopen("C:\\Users\\Tranwit\\Downloads\\test3.h265","rb");
		FILE* fp = fopen("F:\\安装包\\装机必装\\编程软件\\辅助工具\\音视频相关\\样本\\H264\\海康(onvif_rtsp)_test.264","rb");
		if( fp == NULL )
			goto gt_end;
		
		THandle hDec;
		int iSize = 0;

		do
		{
			iSize = fread(szData, 1, iLen , fp);
			if( iSize <= 0 )
			{
				fclose(fp);
				goto gt_end;
			}

			hDec = MDecoder_Parse(szData, iSize);
			if( hDec != NULL )
			{
				MDecoder_Set(hDec, GetConsoleWindow());
				break;
			}
		}while(true);

		do
		{
			MDecoder_Decode(hDec, szData, iSize);
			iSize = fread(szData, 1, 1024, fp);
		} while ( iSize  > 0 );

		printf("finish\r\n");
		if( g_wfile != NULL )	
			fclose(g_wfile);

		if( fp != NULL )
			fclose(fp);

		MDecoder_Close(hDec);
		MDecoder_Video_Free();
		delete szData;
	}

gt_end:
	MDecoder_Video_Free();
}

void Dll_FLV( void )
{
	while(true)
	{
		MDecoder_Video_Init();

		const int iLen = 2048;// * 10;
		char* szData = new char[iLen];

		FILE* fp = fopen("C:\\Users\\Tranwit\\Downloads\\simplest_ffmpeg_mem_handler-master\\simplest_ffmpeg_mem_handler-master\\simplest_ffmpeg_mem_player\\receive__.flv","rb");
		if( fp == NULL )
			goto gt_end;

		THandle hDec;
		int iSize = 0;

		do
		{
			iSize = fread(szData, 1, iLen , fp);
			if( iSize <= 0 )
			{
				fclose(fp);
				goto gt_end;
			}

			hDec = MDecoder_Parse(szData, iSize);
			if( hDec != NULL )
			{
				MDecoder_Set(hDec, GetConsoleWindow());
				break;
			}
		}while(true);

		fseek(fp, 0, SEEK_SET);
		do
		{
			iSize = fread(szData, 1, 1024, fp);
			if( iSize > 0 )
				MDecoder_Decode(hDec, szData, iSize);
		} while ( iSize  > 0 );

		printf("finish\r\n");
		if( g_wfile != NULL )	
			fclose(g_wfile);

		if( fp != NULL )
			fclose(fp);

		MDecoder_Close(hDec);
		MDecoder_Video_Free();
		delete szData;
	}

gt_end:
	MDecoder_Video_Free();
}

void Lib_FLV( void )
{
	while(true)
	{
		_ffmpeg_::DecoderManager	DecoderMan;
		_ffmpeg_::HDecoder hDec = NULL;

		const int iLen = 2048 * 10;
		_ffmpeg_::H264Decoder	Dec;
		char* szData = new char[iLen];
		//char szData[iLen + 1] = {0};

		FILE* fp = fopen("C:\\Users\\Tranwit\\Downloads\\simplest_ffmpeg_mem_handler-master\\simplest_ffmpeg_mem_handler-master\\simplest_ffmpeg_mem_player\\receive.flv","rb");
		if( fp == NULL )
			goto gt_end;
		
		int iSize = 0;
		do
		{
			iSize = fread(szData, 1, iLen , fp);
			if( iSize <= 0 )
			{
				fclose(fp);
				goto gt_end;
			}

			hDec = DecoderMan.GetDecoder(szData, iSize, function20_bind_Ex(Decode));
			if( hDec != NULL )
				break;
		}while(true);

		//if( Dec.Init((unsigned char*)szData, iSize, function20_bind_Ex(Decode)) == -1 )
		//if( Dec.GetCodecContextToData((unsigned char*)szData, iSize, NULL) )
		/*if( Dec.Init(800,600, 25 , function20_bind_Ex(Decode)) == -1 )
		{
			fclose(fp);
			goto gt_end;
		}*/

		_ffmpeg_::IDecoder::tagDecoder_INFO* ImageInfo = DecoderMan.GetDecoderInfo(hDec);

		//test
		RECT WRect = {0};
		::GetWindowRect(GetConsoleWindow(), &WRect);
		long lWidth = WRect.right - WRect.left; //ImageInfo.lWidth
		long lHeight = WRect.bottom - WRect.top;//ImageInfo.lHeight
		//test

		HWND hcon = GetConsoleWindow();
		HDC hdcScreen = GetDC( NULL );
		HWND hwnd1 = WindowFromDC(hdcScreen);

		RECT WRect1 = {0};
		::GetWindowRect(hwnd1, &WRect1);
		long lWidth1 = WRect1.right - WRect1.left; //ImageInfo.lWidth
		long lHeight1 = WRect1.bottom - WRect1.top;//ImageInfo.lHeight

		HWND hwnd = GetConsoleWindow();
		if( g_D3DManager.CreateDevice(hwnd1, lWidth1, lHeight1) != 1 )
		//if( g_D3DManager.CreateDevice(hwnd, lWidth, lHeight) != 1 )
		{
			return ;
		}

		RECT Rect = {0};
		Rect.left = Rect.top = 0;
		Rect.right = ((_ffmpeg_::IDecoder::tagVideo_INFO*)ImageInfo->pInfo)->uWidth;
		Rect.bottom = ((_ffmpeg_::IDecoder::tagVideo_INFO*)ImageInfo->pInfo)->uHeight;

		g_Display = g_D3DManager.CreateImage(((_ffmpeg_::IDecoder::tagVideo_INFO*)ImageInfo->pInfo)->uWidth,
			((_ffmpeg_::IDecoder::tagVideo_INFO*)ImageInfo->pInfo)->uHeight, _d3d_::CS_YV12);

		//g_wfile = fopen("D:\\YV12.txt", "wb+");

		fseek(fp, 0, SEEK_SET);

		do
		{
			iSize = fread(szData, 1, 1024, fp);
			DecoderMan.Decoder(hDec, (unsigned char*)szData, iSize);
		} while ( iSize  > 0 );

		printf("finish\r\n");
		if( g_wfile != NULL )
			fclose(g_wfile);

		if( fp != NULL )
			fclose(fp);

		DecoderMan.Destroy(hDec);
		g_D3DManager.DestroyImage(g_Display);
		g_D3DManager.Destroy();
		delete szData;
	}

gt_end:
	g_D3DManager.Destroy();
}

#include <immintrin.h>

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
	int src_rate = 48000, dst_rate = 8000;
	int src_nb_channels = 2, dst_nb_channels = 2;
	int src_nb_samples = 1024;

	FILE* fp_read = fopen("D:\\record(48000,2)_1.pcm", "rb");
	do {
		/* generate synthetic audio */
		//fill_samples((double *)src_data[0], src_nb_samples, src_nb_channels, src_rate, &t);

		int iLen = fread(szData, 1, sizeof(szData), fp_read);
		if (iLen == 0)
			break;


		_string_type s = _ffmpeg_::Audio_swr_convert_fmts16(src_rate, src_nb_channels, dst_rate, dst_nb_channels, szData, iLen);

		fwrite(s.c_str(), 1, s.size(), dst_file);
		fflush(dst_file);

	} while (true);

}

int main(int argc, char* argv[])
{
	__m256i in1;

	//Lib_FFMpeg();
	//Dll_FFMpeg();
	Dll_FFMpeg1();
	//Dll_FLV();
	//Lib_FLV();

	//DrawPic();

	/*while (true)
	{
		uint8_t* buf = (uint8_t*)av_mallocz(sizeof(uint8_t)* 1024);  
		av_free(buf);
		Sleep(5);
	}*/

	Audio_swr_convert_fmts16_Test();

	std::cin.get();
	return 0;
}