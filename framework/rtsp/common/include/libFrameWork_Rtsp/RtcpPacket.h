
#ifndef __RTCPPACKET_20171226154204_3344199361_H__
#define __RTCPPACKET_20171226154204_3344199361_H__

#include "External.h"

#pragma pack(push, 1)
/*
* RTCP common header word
*/
#if defined( LITTLE_ENDIAN_DEF )

	typedef struct {
		unsigned short count:5;      /* varies by packet type */
		unsigned short p:1;          /* padding flag */
		unsigned short version:2;    /* protocol version */
		unsigned short pt:8;         /* RTCP packet type */
		UInt16 length;            /* pkt len in words, w/o this word */
	} rtcp_common_t;

#endif

#if defined( BIG_ENDIAN_DEF )
	
	typedef struct {
		unsigned short version:2;    /* protocol version */
		unsigned short p:1;          /* padding flag */
		unsigned short count:5;      /* varies by packet type */
		unsigned short pt:8;         /* RTCP packet type */
		UInt16 length;            /* pkt len in words, w/o this word */
	} rtcp_common_t;

#endif

/*
* Big-endian mask for version, padding bit and packet type pair
*/
#define RTCP_VALID_MASK (0xc000 | 0x2000 | 0xfe)
#define RTCP_VALID_VALUE ((RTP_VERSION << 14) | RTCP_SR)
#define RTCP_CNAME_TEXT	"TRANWIT RTCP V1.0"		//长度不能超过40个

/*
* Reception report block
*/
typedef struct {
	UInt32 ssrc;              /* data source being reported */
	unsigned int fraction:8;   /* fraction lost since last SR/RR */
	int lost:24;               /* cumul. no. pkts lost (signed!) */
	UInt32 last_seq;          /* extended last seq. no. received */
	UInt32 jitter;            /* interarrival jitter */
	UInt32 lsr;               /* last SR packet from this source */
	UInt32 dlsr;              /* delay since last SR packet */
} rtcp_rr_t;

/*
* SDES item
*/
typedef struct {
	UInt8 type;               /* type of item (rtcp_sdes_type_t) */
	UInt8 length;             /* length of item (in octets) */
	char data[1];              /* text, not null-terminated */
} rtcp_sdes_item_t;

/*
* One RTCP packet
*/
typedef struct {
	rtcp_common_t common;      /* common header */
	union {
		/* sender report (SR) */
		struct {
			UInt32 ssrc;      /* sender generating this report */
			UInt32 ntp_sec;   /* NTP timestamp */
			UInt32 ntp_frac;
			UInt32 rtp_ts;    /* RTP timestamp */
			UInt32 psent;     /* packets sent */
			UInt32 osent;     /* octets sent */
			rtcp_rr_t rr[1];   /* variable-length list */
		} sr;

		/* reception report (RR) */
		struct {
			UInt32 ssrc;      /* receiver generating this report */
			rtcp_rr_t rr[1];   /* variable-length list */
		} rr;

		/* source description (SDES) */
		struct rtcp_sdes {
			UInt32 src;       /* first SSRC/CSRC */
			rtcp_sdes_item_t item[1]; /* list of SDES items */
		} sdes;

		/* BYE */
		struct {
			UInt32 src[1];    /* list of sources */
			/* can't express trailing text for reason */
		} bye;
	} r;
} rtcp_t;

#pragma pack(pop)

class RtcpPacket
{
public:
	static MemPool_type* GetPoolInstance( void )
	{
		static MemPool_type s_Pool( 512 );
		return &s_Pool;
	}

	static PKBuf_ptr Alloc( void )
	{
		return PKBuf_ptr( GetPoolInstance()->AllocObj<tagPKBuf>(),
					function20_bind_Ex(&MemPool_type::FreeObj<tagPKBuf>,
					GetPoolInstance()) );
	}

	static UInt32 GetSenderSSRC( const char* c_szData, UInt16 uLen );

public:
	typedef enum {
		RTCP_SR		= 200,
		RTCP_RR		= 201,
		RTCP_SDES	= 202,
		RTCP_BYE	= 203,
		RTCP_APP	= 204
	} rtcp_type_t;

	typedef enum {
		RTCP_SDES_END	= 0,
		RTCP_SDES_CNAME	= 1,
		RTCP_SDES_NAME	= 2,
		RTCP_SDES_EMAIL	= 3,
		RTCP_SDES_PHONE	= 4,
		RTCP_SDES_LOC	= 5,
		RTCP_SDES_TOOL	= 6,
		RTCP_SDES_NOTE	= 7,
		RTCP_SDES_PRIV	= 8
	} rtcp_sdes_type_t;

public:
	RtcpPacket( void );

	~RtcpPacket( void );

	void Init( UInt32 uRtpSSRC );

	void Release( void );

	PKBuf_ptr GetRRReport_Udp( UInt32 uSSRC, UInt32 uLost,
		UInt16 uNoCycle, UInt16 uMaxNo, UInt32 uJitter );

	PKBuf_ptr GetRRReport_Tcp( UInt32 uSSRC, UInt32 uLost,
		UInt16 uNoCycle, UInt16 uMaxNo, UInt32 uJitter );

	bool isInit( void )
	{
		return _isInit;
	}

private:
	UInt32	_uRtpSSRC;
	bool	_isInit;
};


#endif
