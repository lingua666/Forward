
#ifndef __USERSESSION_20171226154204_3344199361_H__
#define __USERSESSION_20171226154204_3344199361_H__

#include "External.h"

class UserSession
{
public:
	UserSession( void );

	~UserSession( void );

	int Init( void );

	void Release( void );

	int Open( NETHANDLE hRtp, sockaddr_in* DestRtp,
		NETHANDLE hRtcp, sockaddr_in* DestRtcp, UInt8 uFrameRate,
		UInt8 uVideoType );

	int Close( void );

	_string_type GetSessionUid( void ) const;

	void MediaStreamProcess( NETHANDLE Node, const StreamBuf_ptr& Buf_ptr );

protected:

	void HandleRtpRecv( NETHANDLE Node, const StreamBuf_ptr& Buf_ptr );

	void HandleRtcpRecv( NETHANDLE Node, const StreamBuf_ptr& Buf_ptr );

private:
	_string_type	_sSessionUid;
	NETHANDLE _hRtp;
	sockaddr_in _DestRtp;
	NETHANDLE _hRtcp;
	sockaddr_in _DestRtcp;
	RtpPacket	_RtpPacket;
	UInt8		_uFrameRate;
};

#endif
