
#ifndef __RTPPACKET_20171226154204_3344199361_H__
#define __RTPPACKET_20171226154204_3344199361_H__

#include "IVideoInfo.h"

#define MAX_RTP_PKT_LENGTH  (1440 - 12)	// 最大RTP包长度

#pragma pack(push, 1)

//RTP的头部信息（windows/linux小端）：

#if defined( LITTLE_ENDIAN_DEF )
	struct tagRTP_HEAD
	{
		unsigned char	CC	: 4;	//CSRC计数器, 指示CSRC 标识符的个数
		unsigned char	X	: 1;	//扩展标志，占1位，如果X=1，
									//则在RTP报头后跟有一个扩展报头
		unsigned char	P	: 1;	//填充标志，占1位，如果P=1，
									//则在该报文的尾部填充一个或多个额外的八位组，
									//它们不是有效载荷的一部分
		unsigned char	Ver	: 2;	//RTP协议的版本号

		unsigned char	PT	: 7;	//有效载荷类型，占7位，用于说明RTP报文中有效载荷的类型
									//参考RTP头有效载荷类型列表
									//RFC2250 建议96 表示PS 封装，建议97 为MPEG-4，建议98 为H264
		unsigned char	M	: 1;	//标记，占1位，不同的有效载荷有不同的含义，
									//对于视频，标记一帧的结束；
									//对于音频，标记会话的开始
		UInt16	SequenceNumber; //16bits
		UInt32	Timestamp;      //32
		UInt32	SSRC;           //32
		//UInt32	CSRC;       //32
	};

	//RTCP的头部信息：
	typedef struct tagRTCP_HEAD
	{
		unsigned char    PCount     : 5;
		unsigned char    Padding    : 1;
		unsigned char    Version	: 2;
		unsigned char    Ptype;            //8bits
		UInt16           Length;           //16bits
	}RTCP_HEAD,*pRTCP_HEAD;
#endif

#if defined( BIG_ENDIAN_DEF )
	struct tagRTP_HEAD
	{
		unsigned char	Ver	: 2;	//RTP协议的版本号
		unsigned char	P	: 1;	//填充标志，占1位，如果P=1，
									//则在该报文的尾部填充一个或多个额外的八位组，
									//它们不是有效载荷的一部分
		unsigned char	X	: 1;	//扩展标志，占1位，如果X=1，
									//则在RTP报头后跟有一个扩展报头
		unsigned char	CC	: 4;	//CSRC计数器, 指示CSRC 标识符的个数

		unsigned char	M	: 1;	//标记，占1位，不同的有效载荷有不同的含义，
									//对于视频，标记一帧的结束；
									//对于音频，标记会话的开始
		unsigned char	PT	: 7;	//有效载荷类型，占7位，用于说明RTP报文中有效载荷的类型
									//参考RTP头有效载荷类型列表
									//RFC2250 建议96 表示PS 封装，建议97 为MPEG-4，建议98 为H264
		UInt16	SequenceNumber;     //16bits
		UInt32	Timestamp;      //32
		UInt32	SSRC;           //32
		//UInt32	CSRC;           //32
	};

	//RTCP的头部信息：
	typedef struct tagRTCP_HEAD
	{
		unsigned char    Version	: 2;
		unsigned char    Padding    : 1;
		unsigned char    PCount     : 5;
		unsigned char    Ptype;            //8bits
		UInt16           Length;           //16bits
	}RTCP_HEAD,*pRTCP_HEAD;
#endif

#pragma pack(pop)

class RtpPacket
{
public:
	enum {
		RTCP_SR   = 200,
		RTCP_RR   = 201,
		RTCP_SDES = 202,
		RTCP_BYE  = 203,
		RTCP_APP  = 204
	};

	static MemPool_type* GetPoolInstance( void )
	{
		//static MemPool_type s_Pool( 5120 + 512 );
		static MemPool_type s_Pool( MAX_RTP_PKT_LENGTH + sizeof(tagRTP_HEAD) + sizeof(tagPKBuf) + 5 );
		return &s_Pool;
	}

	static PKBuf_ptr Alloc( void )
	{
		return PKBuf_ptr( GetPoolInstance()->AllocObj<tagPKBuf>(),
			function20_bind_Ex(&MemPool_type::FreeObj<tagPKBuf>,
			GetPoolInstance()) );
	}

public:
	RtpPacket( void );

	~RtpPacket( void );

	void Init( UInt32 uSSRC, UInt8 uFrameRate,
		UInt32 uFrequency = 90000, UInt8 uVideoType = IVideoInfo::ENUM_ENC_H264  );

	void Release( void );

	int Parse( char* c_szData, UInt32 uLen, tagVideoPlayload_INFO* pPlayloadInfo );

	PKBuf_ptr	GetPacket_OneNaul( UInt8 uType, const char* c_szData, UInt32 uLen );

	PKBuf_ptr	GetPacket_FUs( UInt8 uType, bool isMark,
		const char* c_szData, UInt32 uLen );

	_string_type GetVideoHeadBuffer( void );

	tagMediaVideoINFO* GetStreamInfo( void )
	{
		return _pIVideoInfo != NULL ? _pIVideoInfo->GetStreamInfo() : NULL; 
	}

	UInt16	GetHeadSeq( void ) const
	{
		return _Head.SequenceNumber;
	}

	tagRTP_HEAD*	GetHeadInfo( void )
	{
		return &_Head;
	}

	bool is_I_Frame( UInt8 uHeadType, const char* Nalu_Header )
	{
		return _pIVideoInfo != NULL ? _pIVideoInfo->is_I_Frame(uHeadType, Nalu_Header) : false;
	}
	 
private:
	int isVaild( const char* c_szData, UInt32 uLen );

private:
	tagRTP_HEAD	_Head;
	IVideoInfo*	_pIVideoInfo;
	UInt32	_uSSRC;
	UInt16	_uSeq;
	UInt32	_Timestamp;
	UInt32 _uStep;
	void*	_pNalu_Header;
};

#endif
