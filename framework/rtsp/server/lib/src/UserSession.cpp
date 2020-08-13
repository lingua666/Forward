
#include <libFrameWork_RtspSvr/UserSession.h>

UserSession::UserSession( void )
	: _hRtp( -1 )
	, _hRtcp( -1 )
{

}

UserSession::~UserSession( void )
{
	Release();
}

int UserSession::Init( void )
{
	_sSessionUid = Timestamp_type().DateMicroString();
	return 1;
}

void UserSession::Release( void )
{
	Close();
}

int UserSession::Open( NETHANDLE hRtp, sockaddr_in* DestRtp,
	NETHANDLE hRtcp, sockaddr_in* DestRtcp, UInt8 uFrameRate,
	UInt8 uVideoType )
{
	memcpy(&_DestRtp, DestRtp, sizeof(sockaddr_in));
	memcpy(&_DestRtcp, DestRtcp, sizeof(sockaddr_in));
	_hRtp = hRtp;
	_hRtcp = hRtcp;
	_uFrameRate = uFrameRate;
	_RtpPacket.Init(Timestamp_type().utcTime(), _uFrameRate, 90000, uVideoType);
	return 1;
}

int UserSession::Close( void )
{
	if( _hRtp != -1 )
	{
		GetUDPStreamInstance()->Close(_hRtp);
		_hRtp = -1;
	}

	if( _hRtcp != -1 )
	{
		GetUDPStreamInstance()->Close(_hRtcp);
		_hRtcp = -1;
	}

	return 1;
}

_string_type UserSession::GetSessionUid( void ) const
{
	return _sSessionUid;
}

void UserSession::MediaStreamProcess( NETHANDLE Node, const StreamBuf_ptr& Buf_ptr )
{
	/*if(Buf_ptr->payload >= REVBUF_SIZE)
	{//H264
		sptr = _RtpPacket.GetPacket(96, false, Buf_ptr->data, Buf_ptr->payload);
	}
	else
	{//H264
		sptr = _RtpPacket.GetPacket(96, false, Buf_ptr->data, Buf_ptr->payload);
	}

	if( sptr )
	{
		GetUDPStreamInstance()->Send(_hRtp, &_DestRtp, sptr->_Data, sptr->_uPayload);
	}*/

	UInt32 uLen = 0, iOffset = 0, RealLen = 0;
	bool isFUs = true, isBegin = true;
	if( Buf_ptr->payload < MAX_RTP_PKT_LENGTH )
	{
		isFUs = false;
	}

	do 
	{
		PKBuf_ptr sptr;
		uLen = Buf_ptr->payload - iOffset;
		if(  uLen > MAX_RTP_PKT_LENGTH )
		{
			RealLen = iOffset == 0 ? MAX_RTP_PKT_LENGTH - 1 : MAX_RTP_PKT_LENGTH - 2;
			sptr = _RtpPacket.GetPacket_FUs(96, false, &(Buf_ptr->data[iOffset]), RealLen);
			iOffset += RealLen;
		}
		else
		{
			if( isFUs )
				sptr = _RtpPacket.GetPacket_FUs(96, true, &(Buf_ptr->data[iOffset]), uLen);
			else
				sptr = _RtpPacket.GetPacket_OneNaul(96, &(Buf_ptr->data[iOffset]), uLen);

			iOffset += uLen;
		}

		if( sptr )
		{
			GetUDPStreamInstance()->Send(_hRtp, &_DestRtp, sptr->_Data, sptr->_uPayload);
		}
	} while ( Buf_ptr->payload - iOffset > 0 );
}


void UserSession::HandleRtpRecv( NETHANDLE Node, const StreamBuf_ptr& Buf_ptr )
{

}

void UserSession::HandleRtcpRecv( NETHANDLE Node, const StreamBuf_ptr& Buf_ptr )
{

}
