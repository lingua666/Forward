
#ifndef __IMEDIAINFO_20171226154204_3344199361_H__
#define __IMEDIAINFO_20171226154204_3344199361_H__

#include "bs.h"
#include "External.h"

struct tagVideoPlayload_INFO
{
	UInt8	uHeadType;		//ͷ����
	bool	isFull;			//�Ƿ�������
	char*	pData;			//ʵ����������
	UInt16	uDataLen;		//���ݳ���
};

struct tagMediaVideoINFO
{
	UInt8		uVideoType;	//����Ϊö�� enVideoEnc
	UInt32		uWidth;
	UInt32		uHeight;
	UInt32		uMax_Framerate;
};

class IVideoInfo
{
public:
	enum enVideoEnc
	{
		ENUM_ENC_H264,
		ENUM_ENC_H265,
		ENUM_ENC_COUNT
	};

	static MemPool_type*	GetMediaInfoPoolInstance( void );

public:
	virtual ~IVideoInfo( void ) { }

	virtual void Parse( char* c_szData, UInt32 uLen, tagVideoPlayload_INFO* pPlayloadInfo ) = 0;

	virtual bool isReady( void ) = 0;

	virtual _string_type Get_SDP_ProfileLevelID( void ) = 0;

	virtual _string_type Get_SDP_SpropParameterSets( void ) = 0;

	virtual tagMediaVideoINFO*	GetStreamInfo( void ) = 0;

	virtual	UInt8	GetVideoType( void ) const = 0;

	virtual _string_type GetHeadBuffer( void ) = 0;

	virtual bool is_I_Frame( UInt8 uHeadType, const char* Nalu_Header ) = 0;
};

#endif
