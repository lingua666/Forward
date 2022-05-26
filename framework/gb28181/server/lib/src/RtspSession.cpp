
#include <libFrameWork_RtspSvr/RtspSession.h>

RtspSession::RtspSession( void )
	: _hNet( -1 )
	, _hRtcp( -1 )
	, _Pool( sizeof(tagUserSession_INFO) )
{
	_RtpInfo.uRtpPort = 20000;
	_RtpInfo.uSeq = 0;
	_sSessionID = Timestamp_type().DateMicroString();
	_uSsrc = Timestamp_type().utcTime();
}

RtspSession::~RtspSession( void )
{
	Release();
}

int RtspSession::Init( int iStreamType, const _string_type& sUser, const _string_type& sPassword,
	const _string_type& sHost, const _string_type& sPort, UInt8 uVideoType )
{
	_uVideoType = uVideoType;
	_Rtsp.SetUserInfo(iStreamType, sUser, sPassword, sHost, sPort);
	return 1;
}

void RtspSession::Release( void )
{
	Close();
	_UserMap.clear();
	_UserList.clear();
}

int RtspSession::Open( NETHANDLE Node, const HFNRtspSessionOPStatus& hStatus, UInt8 uFrameRate )
{
	_uFrameRate = uFrameRate;
	_hNet = Node;
	_hStatusEvent = hStatus;
	return GetStreamSvrInstance()->SetRecvHandle(Node, function20_bind_Ex(&RtspSession::HandleRecv, shared_from_this()));
}

int RtspSession::Close( void )
{
	NETHANDLE Handle = _hNet;
	_hNet = -1;
	return GetStreamSvrInstance()->Close(Handle);
}

const RtspSession::HFNMediaStreamData& RtspSession::GetMediaStreamProcessEvent( void )
{
	return function20_bind_Ex(&RtspSession::HandleStreamRecv, shared_from_this());
}

_string_type	RtspSession::GetSessionID( void ) const
{
	return _sSessionID;
}

void RtspSession::HandleRecv( NETHANDLE Node, const StreamBuf_ptr& Buf_ptr )
{
	char* pEnd = strstr(Buf_ptr->data, " ");
	if( pEnd == NULL )
		return ;

	_string_type sMethod = _string_type(Buf_ptr->data, pEnd - Buf_ptr->data);
	if( sMethod == "" )//没有解析到
		return ;

	char* pStart = strstr(Buf_ptr->data, "rtsp://");
	pEnd = strstr(pStart + strlen("rtsp://"), " ");
	_string_type sUrl = _string_type(pStart, pEnd - pStart);

	pStart = strstr(pStart + strlen("rtsp://"), "/") + 1;
	_string_type sChildUrl = _string_type(pStart, pEnd - pStart);


	_string_type sSeq = RtspPacket::get_field("CSeq", Buf_ptr->data, Buf_ptr->payload, pEnd - Buf_ptr->data);

	if(sMethod == "DESCRIBE")
	{

	}
	else if(sMethod == "SETUP")
	{
		NETHANDLE hRtp = -1;
		NETHANDLE hRtcp = -1;
		_string_type sClientInfo;
		pStart = pEnd;

		if(strstr(pStart,"RTP/AVP;") != NULL || strstr(pStart,"RTP/AVP/UDP") != NULL)
		{
			SetRtspUserInfo(sUrl, RtspPacket::ENUM_UDP_TYPE);
			pStart = strstr(pStart, "client_port=");
			pEnd = strstr(pStart + strlen("client_port="), "\r\n");
			const char* pEnd1 = strstr(pStart + strlen("client_port="), ";");

			if( pEnd < pEnd1 )
			{
				if( pEnd != NULL )
					sClientInfo = _string_type(pStart, pEnd - pStart);
				else
					sClientInfo = _string_type(pStart, pEnd1 - pStart);
			}
			else
			{
				if( pEnd1 != NULL )
					sClientInfo = _string_type(pStart, pEnd1 - pStart);
				else
					sClientInfo = _string_type(pStart, pEnd - pStart);
			}

			do 
			{
				_RtpInfo.uRtpPort ++;
				hRtp = GetUDPStreamInstance()->Open(_RtpInfo.uRtpPort);
				if( hRtp != -1 )
				{
					_RtpInfo.uRtpPort ++;
					hRtcp = GetUDPStreamInstance()->Open(_RtpInfo.uRtpPort);
					if( hRtcp == -1 )
					{
						GetUDPStreamInstance()->Close(hRtp);
						continue;
					}
					break;
				}

			} while (true);

			int iLen = strlen("client_port=");
			int iPos = sClientInfo.find("-", iLen);
			_string_type sRtpPort = sClientInfo.substr(iLen, iPos - iLen);
			_string_type sRtcpPort = sClientInfo.substr(iPos + 1);

			struct sockaddr_in DestRtp = {0};
			DestRtp.sin_port			= htons( _string_type::StringToInt(sRtpPort) );
			DestRtp.sin_addr.s_addr	= inet_addr( GetIPv4ToString(Node).c_str() );
			DestRtp.sin_family			= AF_INET;

			struct sockaddr_in DestRtcp = {0};
			DestRtcp.sin_port			= htons( _string_type::StringToInt(sRtcpPort) );
			DestRtcp.sin_addr.s_addr	= inet_addr( GetIPv4ToString(Node).c_str() );
			DestRtcp.sin_family			= AF_INET;

			tagUserSession_INFO* p = _Pool.AllocObj<tagUserSession_INFO>();
			p->User.Init();
			p->uStatus = enUserSession_Status::EN_INIT_STATUS;

			tagUserSession_INFO_sptr	sptr(p, 
				function20_bind_Ex(&MemPool_type::FreeObj<tagUserSession_INFO>, &_Pool));

			_Lock.Lock();
			_UserMap.insert(std::make_pair(p->User.GetSessionUid(), sptr));
			_UserList.push_back(sptr);
			_Lock.UnLock();

			p->User.Open(hRtp, &DestRtp, hRtcp, &DestRtcp, _uFrameRate, _uVideoType);
			_string_type s = _Rtsp.Setup_Ack(p->User.GetSessionUid().c_str(), 60, _uSsrc, sClientInfo.c_str(),
				_RtpInfo.uRtpPort - 1, _RtpInfo.uRtpPort, sSeq.c_str());

			Send(s);
		}
		else
		{
			SetRtspUserInfo(sUrl, RtspPacket::ENUM_TCP_TYPE);
		}
	}
	else if(sMethod == "PLAY")
	{
		_string_type sSession = RtspPacket::get_field("Session", Buf_ptr->data, Buf_ptr->payload,
			pEnd - Buf_ptr->data).strim();

		tagUserSession_INFO_sptr sptr;
		UserSession_Map::iterator iter;
		_Lock.Lock();
		iter = _UserMap.find(sSession);
		if(iter != _UserMap.end())
		{
			sptr = iter->second;
		}
		_Lock.UnLock();

		if(sptr)
		{
			sptr->uStatus = enUserSession_Status::EN_RUN_STATUS;
			_string_type s = _Rtsp.Play_Ack(sUrl.c_str(), sSession.c_str(), sSeq.c_str(),
				_string_type::NumberToStr(_RtpInfo.uSeq).c_str(),
				_string_type::NumberToStr(Timestamp_type().utcTime()).c_str());
			_RtpInfo.uSeq ++;
			Send(s);
			_hStatusEvent(shared_from_this(), GetSessionID(), sChildUrl, enRTSPSESSION_OP_STATUS::EN_OP_PLAY_STATUS);
		}
	}
	else if(sMethod == "TEARDOWN")
	{
		_string_type sSession = RtspPacket::get_field("Session", Buf_ptr->data, Buf_ptr->payload,
			pEnd - Buf_ptr->data).strim();

		tagUserSession_INFO_sptr sptr;
		UserSession_Map::iterator iter;
		_Lock.Lock();
		iter = _UserMap.find(sSession);
		if(iter != _UserMap.end())
		{
			sptr = iter->second;
			_UserMap.erase(iter);
		}
		_Lock.UnLock();

		if( sptr )
		{
			sptr->uStatus = enUserSession_Status::EN_STOP_STATUS;
			_string_type s = _Rtsp.Teardown_Ack(sSession.c_str(), sSeq.c_str());
			Send(s);
		}
	}
	else if(sMethod == "OPTIONS")
	{
		_string_type sSession = RtspPacket::get_field("Session", Buf_ptr->data, Buf_ptr->payload,
			pEnd - Buf_ptr->data).strim();

		_string_type s = _Rtsp.Option_Ack(sSeq.c_str());
		Send(s);
	}
}

int RtspSession::Send( const _string_type& sData )
{
	return GetStreamSvrInstance()->Send(_hNet, sData.c_str(), sData.size());
}

int RtspSession::SetRtspUserInfo( const _string_type& sUrl, RtspPacket::enRtspStreamType Type )
{
	int iStart = sUrl.find("rtsp://");
	if( iStart == -1 )
		return -1;

	_string_type sUserPwd;
	_string_type sUser;
	_string_type sPassword;
	iStart = iStart + strlen("rtsp://");
	int iEnd = sUrl.find('@', iStart);
	if( iEnd != -1 )
	{//标准格式的用户名密码
		sUserPwd = sUrl.substr(iStart, iEnd - iStart);
		iStart = iEnd;
		iStart += 1;
	}

	iEnd = sUrl.find('/', iStart);
	_string_type sHost = sUrl.substr(iStart, iEnd - iStart);

	if( sUserPwd != "" )
	{
		iStart = sUserPwd.find(":");
		sUser = sUserPwd.substr(0, iStart);
		sPassword = sUserPwd.substr(iStart + 1);
	}

	iStart = sHost.find(":");
	_string_type sIP = sHost.substr(0, iStart);
	_string_type sPort = sHost.substr(iStart + 1);
	_Rtsp.SetUserInfo(Type, sUser, sPassword, sIP, sPort);
	return 1;
}

void RtspSession::HandleStreamRecv( NETHANDLE hNet, const StreamBuf_ptr& Buf_sptr )
{
	for( UserSession_List::iterator iter = _UserList.begin(); iter != _UserList.end(); )
	{
		if( (*iter) )
		{
			switch((*iter)->uStatus)
			{
			case enUserSession_Status::EN_STOP_STATUS:
				_Lock.Lock();
				iter = _UserList.erase(iter);
				_Lock.UnLock();
				break;
			case enUserSession_Status::EN_RUN_STATUS:
				(*iter)->User.MediaStreamProcess(hNet, Buf_sptr);
				_Lock.Lock();
				++ iter;
				_Lock.UnLock();
				break;
			}
		}
	}
}
