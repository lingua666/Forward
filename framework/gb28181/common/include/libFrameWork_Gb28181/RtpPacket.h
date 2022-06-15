
#ifndef __RTPPACKET_20171226154204_3344199361_H__
#define __RTPPACKET_20171226154204_3344199361_H__

#include "IVideoInfo.h"

#define MAX_RTP_PKT_LENGTH  (1440 - 12)	// ���RTP������

#pragma pack(push, 1)

//RTP��ͷ����Ϣ��windows/linuxС�ˣ���

#if defined( LITTLE_ENDIAN_DEF )
	struct tagRTP_HEAD
	{
		unsigned char	CC	: 4;	//CSRC������, ָʾCSRC ��ʶ���ĸ���
		unsigned char	X	: 1;	//��չ��־��ռ1λ�����X=1��
									//����RTP��ͷ�����һ����չ��ͷ
		unsigned char	P	: 1;	//����־��ռ1λ�����P=1��
									//���ڸñ��ĵ�β�����һ����������İ�λ�飬
									//���ǲ�����Ч�غɵ�һ����
		unsigned char	Ver	: 2;	//RTPЭ��İ汾��

		unsigned char	PT	: 7;	//��Ч�غ����ͣ�ռ7λ������˵��RTP��������Ч�غɵ�����
									//�ο�RTPͷ��Ч�غ������б�
									//RFC2250 ����96 ��ʾPS ��װ������97 ΪMPEG-4������98 ΪH264
		unsigned char	M	: 1;	//��ǣ�ռ1λ����ͬ����Ч�غ��в�ͬ�ĺ��壬
									//������Ƶ�����һ֡�Ľ�����
									//������Ƶ����ǻỰ�Ŀ�ʼ
		UInt16	SequenceNumber; //16bits
		UInt32	Timestamp;      //32
		UInt32	SSRC;           //32
		//UInt32	CSRC;       //32
	};

	//RTCP��ͷ����Ϣ��
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
		unsigned char	Ver	: 2;	//RTPЭ��İ汾��
		unsigned char	P	: 1;	//����־��ռ1λ�����P=1��
									//���ڸñ��ĵ�β�����һ����������İ�λ�飬
									//���ǲ�����Ч�غɵ�һ����
		unsigned char	X	: 1;	//��չ��־��ռ1λ�����X=1��
									//����RTP��ͷ�����һ����չ��ͷ
		unsigned char	CC	: 4;	//CSRC������, ָʾCSRC ��ʶ���ĸ���

		unsigned char	M	: 1;	//��ǣ�ռ1λ����ͬ����Ч�غ��в�ͬ�ĺ��壬
									//������Ƶ�����һ֡�Ľ�����
									//������Ƶ����ǻỰ�Ŀ�ʼ
		unsigned char	PT	: 7;	//��Ч�غ����ͣ�ռ7λ������˵��RTP��������Ч�غɵ�����
									//�ο�RTPͷ��Ч�غ������б�
									//RFC2250 ����96 ��ʾPS ��װ������97 ΪMPEG-4������98 ΪH264
		UInt16	SequenceNumber;     //16bits
		UInt32	Timestamp;      //32
		UInt32	SSRC;           //32
		//UInt32	CSRC;           //32
	};

	//RTCP��ͷ����Ϣ��
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
