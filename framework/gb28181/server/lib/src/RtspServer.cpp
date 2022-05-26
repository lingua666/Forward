
#include <libFrameWork_RtspSvr/RtspServer.h>

RtspServer::RtspServer( void )
	: _Pool( __max(sizeof(RtspSession), sizeof(MediaVideoStream)) )
{

}

RtspServer::~RtspServer( void )
{

}

int RtspServer::Init( const _string_type& sUser, const _string_type& sPassword,
	const _string_type& sHost, const _string_type& sPort )
{
	_RtspSvrInfo.sUser = sUser;
	_RtspSvrInfo.sPassword = sUser;
	_RtspSvrInfo.sHost = sHost;
	_RtspSvrInfo.sPort = sPort;

	GetStreamSvrInstance()->Init();
	GetUDPStreamInstance()->Init();
	return 1;
}

void RtspServer::Release( void )
{
	GetStreamSvrInstance()->Release();
	GetUDPStreamInstance()->Release();
}

void RtspServer::SetSessionEvent( const HFNRtspSessionAccept& hAccept,
	const HFNRtspSessionDestroy& hDestroy,
	const HFNRtspSessionOPStatus& hStatus )
{
	_hAccept = hAccept;
	_hDestroy = hDestroy;
	_hStatus = hStatus;
}

int RtspServer::Start( UInt16 u16Port, UInt8 uIOThreadNum, UInt8 uProcThreadNum )
{
	if( !GetStreamSvrInstance()->Listen(u16Port, function20_bind_Ex(&RtspServer::HandleAccept, this)) )
		return -1;

	GetStreamSvrInstance()->Run(uIOThreadNum, uProcThreadNum, true);
	return 1;
}

int RtspServer::Stop( void )
{
	GetStreamSvrInstance()->Stop();
	return 1;
}

int	RtspServer::Close( NETHANDLE Node )
{
	return GetStreamSvrInstance()->Close(Node);
}

RtspServer::HFNStreamProcessEvent	RtspServer::AddMediaStream( const _string_type& sUrl,
	const HFNClose& hClose, UInt8 uVideoType, const HFNMediaVideoIdle& hIdle )
{
	MediaVideoStream::self_sptr sptr(_Pool.AllocObj<MediaVideoStream>(),
		function20_bind_Ex(&MemPool_type::FreeObj<MediaVideoStream>, &_Pool));
	sptr->Init(uVideoType);
	RtspServer::HFNStreamProcessEvent Event;
	MediaStream_Map::iterator iter;
	_MediaLock.Lock();
	iter = _MediaMap.find(sUrl);
	if( iter == _MediaMap.end() )
	{
		Event = sptr->Open(sUrl, function20_bind(&RtspServer::HandleMediaStreamReadyEvent, this, sUrl, _function_::_1),
			hClose, hIdle);

		_MediaMap.insert(std::make_pair(sUrl, sptr));
	}
	_MediaLock.UnLock();
	return Event;
}

void	RtspServer::RemoveMediaStream( const _string_type& sUrl )
{
	MediaStream_Map::iterator iter;
	_MediaLock.Lock();
	iter = _MediaMap.find(sUrl);
	if( iter != _MediaMap.end() )
	{
		_MediaMap.erase(iter);
	}
	_MediaLock.UnLock();
}

void	RtspServer::HandleMediaStreamReadyEvent( const _string_type& sUrl, 
	const MediaVideoStream::self_sptr& Stream_sptr )
{

}

void RtspServer::Close( const _string_type& sSessionUid )
{
	RtspSession_sptr sptr = FindSession(sSessionUid);
	if( sptr )
		sptr->Close();
}

RtspServer::RtspSession_sptr 	RtspServer::FindSession( const _string_type& sSessionUid )
{
	RtspSession_sptr	sptr;
	RtspSession_Map::iterator iter;
	_Lock.Lock();
	iter = _RtspMap.find(sSessionUid);
	if( iter != _RtspMap.end() )
	{
		sptr = iter->second;
	}
	_Lock.UnLock();

	return sptr;
}

void	RtspServer::HandleAccept( const char* c_szIP, UInt16 u16Port,
	NETHANDLE Node )
{
	GetStreamSvrInstance()->SetRecvHandle(Node, function20_bind_Ex(&RtspServer::HandleToken, this));
}

void	RtspServer::HandleToken( NETHANDLE Node, const StreamBuf_ptr& Buf_sptr )
{
	char* pEnd = strstr(Buf_sptr->data, " ");
	if( pEnd == NULL )
		return ;

	_string_type sMethod = _string_type(Buf_sptr->data, pEnd - Buf_sptr->data);
	if( sMethod == "" )//没有解析到
		return ;

	char* pStart = strstr(Buf_sptr->data, "rtsp://");
	pEnd = strstr(pStart + strlen("rtsp://"), " ");
	_string_type sUrl = _string_type(pStart, pEnd - pStart);

	pStart = strstr(pStart + strlen("rtsp://"), "/") + 1;
	_string_type sChildUrl = _string_type(pStart, pEnd - pStart);

	_string_type sSeq = RtspPacket::get_field("CSeq", Buf_sptr->data, Buf_sptr->payload, pEnd - Buf_sptr->data);

	if( sMethod  == "OPTIONS")
	{
		_string_type s = RtspPacket::Option_Ack(sSeq.c_str());
		GetStreamSvrInstance()->Send(Node, s.c_str(), s.size());
	}
	else if(sMethod == "DESCRIBE")
	{
		MediaVideoStream::self_sptr Media_sptr;
		MediaStream_Map::iterator iter;
		_MediaLock.Lock();
		iter = _MediaMap.find(sChildUrl);
		if( iter != _MediaMap.end() )
		{
			Media_sptr = iter->second;
		}
		_MediaLock.UnLock();

		if(Media_sptr && Media_sptr->isReady())
		{//资源存在
			RtspSession_sptr sprt(_Pool.AllocObj<RtspSession>(),
				function20_bind_Ex(&MemPool_type::FreeObj<RtspSession>, &_Pool));

			_Lock.Lock();
			_RtspMap.insert(std::make_pair(sprt->GetSessionID(), sprt));
			_Lock.UnLock();

			if( GetStreamSvrInstance()->SetDestroyHandle(Node, function20_bind(&RtspServer::HandleDestroy, this,
				sprt->GetSessionID(), sprt, sChildUrl, Node)) == 1 )
			{
				MediaVideoStream::tagMediaInfo* pInfo = Media_sptr->GetMediaInfo();
				tagVideo_INFO	VideoInfo;
				VideoInfo.uFrameRate = pInfo->uFramerate;
				VideoInfo.uWidth = pInfo->uWidth;
				VideoInfo.uHeight = pInfo->uHeight;

				VideoInfo.uFrequency = 90000;
				VideoInfo.uVideoType = pInfo->uVideoType;

				_string_type s = RtspPacket::Describe_Ack(sUrl, VideoInfo, sprt->GetSessionID().c_str(),
					"",  pInfo->sProfileLevelID.c_str(), pInfo->sSpropParameterSets.c_str(), 
					sSeq.c_str(), RtspPacket::ENUM_UDP_TYPE);

				GetStreamSvrInstance()->Send(Node, s.c_str(), s.size());

				sprt->Init(0,"","","","",pInfo->uVideoType);
				sprt->Open(Node, function20_bind_Ex(&RtspServer::HandleSessionOPStatusEvent, this),
					VideoInfo.uFrameRate);
				if( _hAccept )
				{
					_hAccept(sprt->GetSessionID(), sChildUrl, sprt);
				}
			}
			else
			{
				_Lock.Lock();
				_RtspMap.erase(sprt->GetSessionID());
				_Lock.UnLock();
			}
		}
	}
}

int	RtspServer::HandleSessionOPStatusEvent( const RtspSession_sptr& sptr, const _string_type& sSessionUid,
	const _string_type& sUrl, UInt8 uStatus )
{
	MediaVideoStream::self_sptr Media_sptr;
	MediaStream_Map::iterator iter;
	_Lock.Lock();
	iter = _MediaMap.find(sUrl);
	if( iter != _MediaMap.end() )
	{
		Media_sptr = iter->second;
		_Lock.UnLock();
	}
	else
	{
		_Lock.UnLock();
		return -1;
	}

	if( Media_sptr )
	{
		switch(uStatus)
		{
		case RtspSession::enRTSPSESSION_OP_STATUS::EN_OP_PLAY_STATUS:
			Media_sptr->Register(sSessionUid, sptr);
			break;
		}
	}
	else //媒体流已经不存在，关闭rtsp 会话连接
	{
		sptr->Close();
	}

	if( _hStatus )
	{
		_hStatus(sptr, sSessionUid, uStatus);
	}

	return 1;
}

void	RtspServer::HandleDestroy( const _string_type& sSessionUid,
	const RtspSession_sptr& sptr,
	const _string_type& sUrl,
	NETHANDLE Node )
{
	MediaVideoStream::self_sptr Media_sptr;
	MediaStream_Map::iterator iter;

	_Lock.Lock();
	_RtspMap.erase(sSessionUid);
	iter = _MediaMap.find(sUrl);
	if( iter != _MediaMap.end() )
	{
		Media_sptr = iter->second;
	}
	_Lock.UnLock();

	if( Media_sptr )
	{
		Media_sptr->unRegister(sSessionUid);
	}

	if( _hDestroy )
	{
		_hDestroy(sSessionUid, sptr);
	}
}

