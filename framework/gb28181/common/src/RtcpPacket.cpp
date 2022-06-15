
#include <libFrameWork_Rtsp/RtcpPacket.h>

RtcpPacket::RtcpPacket( void )
	: _isInit( false )
{

}

RtcpPacket::~RtcpPacket( void )
{

}

void RtcpPacket::Init( UInt32 uRtpSSRC )
{
	_uRtpSSRC = uRtpSSRC;
	_isInit = true;
}

void RtcpPacket::Release( void )
{
	_isInit = false;
}

PKBuf_ptr RtcpPacket::GetRRReport_Udp( UInt32 uSSRC, UInt32 uLost,
	UInt16 uNoCycle, UInt16 uMaxNo, UInt32 uJitter )
{
	rtcp_t	rtcp_rr;
	rtcp_rr.common.version = RTP_VERSION;
	rtcp_rr.common.p = 0;
	rtcp_rr.common.count = 1;
	rtcp_rr.common.pt = RTCP_RR;
	rtcp_rr.r.rr.rr[0].fraction = 0;

#if defined( LITTLE_ENDIAN_DEF )
	rtcp_rr.common.length = BigEndian16_UInt16(7);	//实际长度为7 * sizeof(UInt32)
	rtcp_rr.r.rr.ssrc = BigEndian32_UInt32(_uRtpSSRC);
	rtcp_rr.r.rr.rr[0].ssrc = BigEndian32_UInt32(uSSRC);
	rtcp_rr.r.rr.rr[0].lost = BigEndian24_UInt8((const UInt8*)&uLost);
	rtcp_rr.r.rr.rr[0].last_seq = BigEndian32_UInt32(((UInt32)uNoCycle) << 16 | uMaxNo);
	//计算方式参考
	//1:https://wenku.baidu.com/view/6211711b2379168884868762caaedd3383c4b59b.html
	//2:https://wenku.baidu.com/view/749d6b9e376baf1ffc4fadfd.html
	rtcp_rr.r.rr.rr[0].jitter = BigEndian32_UInt32(uJitter);	
	rtcp_rr.r.rr.rr[0].lsr = BigEndian32_UInt32(0);
	rtcp_rr.r.rr.rr[0].dlsr = BigEndian32_UInt32(0);
#endif

#if defined( BIG_ENDIAN_DEF )
	rtcp_rr.common.length = 7;	//实际长度为7 * sizeof(UInt32)
	rtcp_rr.r.rr.ssrc = _uRtpSSRC;
	rtcp_rr.r.rr.rr[0].ssrc = uSSRC;

	rtcp_rr.r.rr.rr[0].lost = uLost;
	rtcp_rr.r.rr.rr[0].last_seq = ((UInt32)uNoCycle) << 16 | uMaxNo;
	//计算方式参考
	//1:https://wenku.baidu.com/view/6211711b2379168884868762caaedd3383c4b59b.html
	//2:https://wenku.baidu.com/view/749d6b9e376baf1ffc4fadfd.html
	rtcp_rr.r.rr.rr[0].jitter = uJitter;	
	rtcp_rr.r.rr.rr[0].lsr = 0;
	rtcp_rr.r.rr.rr[0].dlsr = 0;
#endif

	rtcp_t	rtcp_sdes;
	rtcp_sdes.common.version = RTP_VERSION;
	rtcp_sdes.common.p = 0;
	rtcp_sdes.common.count = 1;
	rtcp_sdes.common.pt = RTCP_SDES;

#if defined( LITTLE_ENDIAN_DEF )
	//int iSize = 4 + 1 + 1 + strlen(RTCP_CNAME_TEXT);
	UInt32 uLen = sizeof(rtcp_sdes.r.sdes) + strlen(RTCP_CNAME_TEXT);
	rtcp_sdes.common.length = uLen % 4 == 0 ?
		BigEndian16_UInt16(uLen) / 4 : BigEndian16_UInt16(uLen + 4 - uLen % 4) / 4;	//实际长度为5 * sizeof(UInt32)
	rtcp_sdes.r.sdes.src = BigEndian32_UInt32(_uRtpSSRC);
#endif

#if defined( BIG_ENDIAN_DEF )
	//int iSize = 4 + 1 + 1 + strlen(RTCP_CNAME_TEXT);
	UInt32 uLen = sizeof(rtcp_sdes.r.sdes) + strlen(RTCP_CNAME_TEXT);
	rtcp_sdes.common.length = uLen % 4 == 0 ? uLen / 4 : (uLen + 4 - uLen % 4) / 4;	//实际长度为5 * sizeof(UInt32)
	rtcp_sdes.r.sdes.src = _uRtpSSRC;
#endif

	rtcp_sdes.r.sdes.item[0].type = RTCP_SDES_CNAME;
	rtcp_sdes.r.sdes.item[0].length = std::strlen(RTCP_CNAME_TEXT);
	memset(&(rtcp_sdes.r.sdes.item[0].data[rtcp_sdes.r.sdes.item[0].length]), 0, 2);
	strcpy(rtcp_sdes.r.sdes.item[0].data, RTCP_CNAME_TEXT);

	PKBuf_ptr sptr = Alloc();
	sptr->_uPayload = sizeof(rtcp_common_t) + sizeof(rtcp_rr.r.rr);
	memcpy((char*)sptr->_Data, (char*)&rtcp_rr, sptr->_uPayload);

	//UInt32 uLen = sizeof(rtcp_common_t) + sizeof(rtcp_sdes.r.sdes) + rtcp_sdes.r.sdes.item[0].length;
	memcpy((char*)&sptr->_Data[sptr->_uPayload], (char*)&rtcp_sdes, sizeof(rtcp_common_t) + uLen);
	sptr->_uPayload += sizeof(rtcp_common_t) + uLen;
	if( uLen % 4 != 0 )
	{
		memset((char*)&sptr->_Data[sptr->_uPayload], 0, 4 - uLen % 4);
		sptr->_uPayload += 4 - uLen % 4;
	}

	return sptr;
}

PKBuf_ptr RtcpPacket::GetRRReport_Tcp( UInt32 uSSRC, UInt32 uLost,
	UInt16 uNoCycle, UInt16 uMaxNo, UInt32 uJitter )
{
	rtcp_t	rtcp_rr;
	rtcp_rr.common.version = RTP_VERSION;
	rtcp_rr.common.p = 0;
	rtcp_rr.common.count = 1;
	rtcp_rr.common.pt = RTCP_RR;
	rtcp_rr.r.rr.rr[0].fraction = 0;

#if defined( LITTLE_ENDIAN_DEF )
	rtcp_rr.common.length = BigEndian16_UInt16(7);	//实际长度为7 * sizeof(UInt32)
	rtcp_rr.r.rr.ssrc = BigEndian32_UInt32(_uRtpSSRC);
	rtcp_rr.r.rr.rr[0].ssrc = BigEndian32_UInt32(uSSRC);
	rtcp_rr.r.rr.rr[0].lost = BigEndian24_UInt8((const UInt8*)&uLost);
	rtcp_rr.r.rr.rr[0].last_seq = BigEndian32_UInt32(((UInt32)uNoCycle) << 16 | uMaxNo);
	//计算方式参考
	//1:https://wenku.baidu.com/view/6211711b2379168884868762caaedd3383c4b59b.html
	//2:https://wenku.baidu.com/view/749d6b9e376baf1ffc4fadfd.html
	rtcp_rr.r.rr.rr[0].jitter = BigEndian32_UInt32(uJitter);	
	rtcp_rr.r.rr.rr[0].lsr = BigEndian32_UInt32(0);
	rtcp_rr.r.rr.rr[0].dlsr = BigEndian32_UInt32(0);
#endif

#if defined( BIG_ENDIAN_DEF )
	rtcp_rr.common.length = 7;	//实际长度为7 * sizeof(UInt32)
	rtcp_rr.r.rr.ssrc = _uRtpSSRC;
	rtcp_rr.r.rr.rr[0].ssrc = uSSRC;

	rtcp_rr.r.rr.rr[0].lost = uLost;
	rtcp_rr.r.rr.rr[0].last_seq = ((UInt32)uNoCycle) << 16 | uMaxNo;
	//计算方式参考
	//1:https://wenku.baidu.com/view/6211711b2379168884868762caaedd3383c4b59b.html
	//2:https://wenku.baidu.com/view/749d6b9e376baf1ffc4fadfd.html
	rtcp_rr.r.rr.rr[0].jitter = uJitter;	
	rtcp_rr.r.rr.rr[0].lsr = 0;
	rtcp_rr.r.rr.rr[0].dlsr = 0;
#endif

	rtcp_t	rtcp_sdes;
	rtcp_sdes.common.version = RTP_VERSION;
	rtcp_sdes.common.p = 0;
	rtcp_sdes.common.count = 1;
	rtcp_sdes.common.pt = RTCP_SDES;

#if defined( LITTLE_ENDIAN_DEF )
	//int iSize = 4 + 1 + 1 + strlen(RTCP_CNAME_TEXT);
	UInt32 uLen = sizeof(rtcp_sdes.r.sdes) + strlen(RTCP_CNAME_TEXT);
	rtcp_sdes.common.length = uLen % 4 == 0 ?
		BigEndian16_UInt16(uLen) / 4 : BigEndian16_UInt16(uLen + 4 - uLen % 4) / 4;	//实际长度为5 * sizeof(UInt32)
	rtcp_sdes.r.sdes.src = BigEndian32_UInt32(_uRtpSSRC);
#endif

#if defined( BIG_ENDIAN_DEF )
	//int iSize = 4 + 1 + 1 + strlen(RTCP_CNAME_TEXT);
	UInt32 uLen = sizeof(rtcp_sdes.r.sdes) + strlen(RTCP_CNAME_TEXT);
	rtcp_sdes.common.length = uLen % 4 == 0 ? uLen / 4 : (uLen + 4 - uLen % 4) / 4;	//实际长度为5 * sizeof(UInt32)
	rtcp_sdes.r.sdes.src = _uRtpSSRC;
#endif

	rtcp_sdes.r.sdes.item[0].type = RTCP_SDES_CNAME;
	rtcp_sdes.r.sdes.item[0].length = std::strlen(RTCP_CNAME_TEXT);
	memset(&(rtcp_sdes.r.sdes.item[0].data[rtcp_sdes.r.sdes.item[0].length]), 0, 2);
	strcpy(rtcp_sdes.r.sdes.item[0].data, RTCP_CNAME_TEXT);

	struct tagINTERLEAVED
	{
		UInt8	uMagic;
		UInt8	uChannel;
		UInt16	uLength;
	};

	tagINTERLEAVED tagInter = {0};
	tagInter.uMagic = 0x24;
	tagInter.uChannel = 0x01;
	
	PKBuf_ptr sptr = Alloc();
	sptr->_uPayload = sizeof(rtcp_common_t) + sizeof(rtcp_rr.r.rr);
	memcpy((char*)&(sptr->_Data[sizeof(tagINTERLEAVED)]), (char*)&rtcp_rr, sptr->_uPayload);

	//UInt32 uLen = sizeof(rtcp_common_t) + sizeof(rtcp_sdes.r.sdes) + rtcp_sdes.r.sdes.item[0].length;
	memcpy((char*)&sptr->_Data[sptr->_uPayload + sizeof(tagINTERLEAVED)], (char*)&rtcp_sdes, sizeof(rtcp_common_t) + uLen);
	sptr->_uPayload += sizeof(rtcp_common_t) + uLen;
	if( uLen % 4 != 0 )
	{
		memset((char*)&sptr->_Data[sptr->_uPayload], 0, 4 - uLen % 4);
		sptr->_uPayload += 4 - uLen % 4;
	}

	tagInter.uLength = BigEndian16_UInt16(sptr->_uPayload);
	memcpy((char*)sptr->_Data, (char*)&tagInter, sizeof(tagINTERLEAVED));
	sptr->_uPayload += sizeof(tagINTERLEAVED);

	return sptr;
}

UInt32 RtcpPacket::GetSenderSSRC( const char* c_szData, UInt16 uLen )
{
	if( uLen < sizeof(rtcp_common_t) + sizeof(UInt32) )
		return -1;

	rtcp_t rtcp = {0};
	memcpy(&rtcp, c_szData, uLen);
	return rtcp.r.sr.ssrc;
}