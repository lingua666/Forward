#ifndef  __FLV_PARSE_20170111090004_1444199361_H__
#define  __FLV_PARSE_20170111090004_1444199361_H__

#include "IDecoder.h"

namespace _ffmpeg_{

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

	typedef function20_handle<void (const unsigned char*, UInt32)>	HFNData;

public:
	FlvParse( void );

	~FlvParse( void );

	int Init( const HFNData& hData );

	void Release( void );
	
	int Parse( const char* c_szData, UInt32 uSize );

	int ParseAvcVideo( const TAG_HEADER& TagHead, const unsigned char* c_szData, UInt32 uSize );

private:
	HFNData	_hData;
	UInt16	_uOffset;
	UInt16	_uSize;
	char*	_pData;
	int		_iStatus;
	TAG_HEADER	_Tag;
};

} //_ffmpeg_

#endif