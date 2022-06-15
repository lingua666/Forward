
#ifndef __RTSPSERVER_20171226154204_3344199361_H__
#define __RTSPSERVER_20171226154204_3344199361_H__

#include "RtspSession.h"
#include "MediaVideoStream.h"

class RtspServer
{
public:
	typedef RtspSession::self_sptr	RtspSession_sptr;

	typedef function20_handle<void (const _string_type&,
		const _string_type&, const RtspSession_sptr&) >										HFNRtspSessionAccept;
	typedef function20_handle<void (const _string_type&, const RtspSession_sptr&) >			HFNRtspSessionDestroy;
	typedef function20_handle<void (const RtspSession_sptr&, const _string_type&, UInt8) >	HFNRtspSessionOPStatus;
	typedef MediaVideoStream::HFNMediaVideoIdle												HFNMediaVideoIdle;
	

	typedef MediaVideoStream::HFNReady							HFNReady;
	typedef MediaVideoStream::HFNRecvData						HFNRecvData;
	typedef MediaVideoStream::HFNStreamProcessEvent				HFNStreamProcessEvent;
	typedef MediaVideoStream::HFNClose							HFNClose;

	typedef Container_Map_type<_string_type, RtspSession_sptr>				RtspSession_Map;
	typedef Container_Map_type<_string_type, MediaVideoStream::self_sptr>	MediaStream_Map;
	typedef _server_::_tcp_::StreamServer::Stream_HAccept					Stream_HAccept;
	typedef _io_net_::StreamBuf_ptr	StreamBuf_ptr;

	typedef	RtspPacket::tagVideo_INFO	tagVideo_INFO;

	struct tagRtspSvrInfo
	{
		_string_type sUser;
		_string_type sPassword;
		_string_type sHost;
		_string_type sPort;
	};

public:
	RtspServer( void );

	~RtspServer( void );

	int Init( const _string_type& sUser, const _string_type& sPassword,
		const _string_type& sHost, const _string_type& sPort );

	void Release( void );

	void SetSessionEvent( const HFNRtspSessionAccept& hAccept,
						const HFNRtspSessionDestroy& hDestroy,
						const HFNRtspSessionOPStatus& hStatus );

	int Start( UInt16 u16Port, UInt8 uIOThreadNum = 0, UInt8 uProcThreadNum = 0 );

	int Stop( void );

	void Close( const _string_type& sSessionUid );

	RtspServer::HFNStreamProcessEvent	AddMediaStream( const _string_type& sUrl,
		const HFNClose& hClose, UInt8 uVideoType = IVideoInfo::ENUM_ENC_H264,
		const HFNMediaVideoIdle& hIdle = HFNMediaVideoIdle() );

	void	RemoveMediaStream( const _string_type& sUrl );

protected:

	int	Close( NETHANDLE Node );

	RtspServer::RtspSession_sptr 	FindSession( const _string_type& sSessionUid );

	void	HandleToken( NETHANDLE Node, const StreamBuf_ptr& Buf_sptr );

	int	HandleSessionOPStatusEvent( const RtspSession_sptr& sptr, const _string_type& sSessionUid,
		const _string_type& sUrl, UInt8 uStatus );

	void	HandleMediaStreamReadyEvent( const _string_type& sUrl,
		const MediaVideoStream::self_sptr& Stream_sptr );

	void	HandleAccept( const char* c_szIP, UInt16 u16Port,
		NETHANDLE Node );

	void	HandleDestroy( const _string_type& sSessionUid,
		const RtspSession_sptr& sptr,
		const _string_type& sUrl,
		NETHANDLE Node );

private:
	HFNRtspSessionAccept	_hAccept;
	HFNRtspSessionDestroy	_hDestroy;
	HFNRtspSessionOPStatus	_hStatus;
	MediaStream_Map	_MediaMap;
	CLock			_MediaLock;
	RtspSession_Map	_RtspMap;
	CLock			_Lock;
	MemPool_type	_Pool;
	tagRtspSvrInfo	_RtspSvrInfo;
};

#endif

