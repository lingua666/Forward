
#include <libFrameWork_RtspSvr/MediaVideoStream.h>
#include <libFrameWork_Rtsp/H264Info.h>
#include <libFrameWork_Rtsp/H265Info.h>

MediaVideoStream::MediaVideoStream( void )
	: _isReady( false )
	, _Pool( sizeof(tagRtspSession_INFO) )
	, _pIVideoInfo( NULL )
{

}

MediaVideoStream::~MediaVideoStream( void )
{

}

int MediaVideoStream::Init( UInt8 uVideoType )
{
	_uVideoType = uVideoType;
	_Info.uVideoType = uVideoType;
	switch(_uVideoType)
	{
	case IVideoInfo::ENUM_ENC_H264:
		{
			_pIVideoInfo = IVideoInfo::GetMediaInfoPoolInstance()->AllocObj<H264Info>();
		}
		break;
	case IVideoInfo::ENUM_ENC_H265:
		{
			_pIVideoInfo = IVideoInfo::GetMediaInfoPoolInstance()->AllocObj<H265Info>();
		}
		break;
	}
	return 1;
}

void MediaVideoStream::Release( void )
{
	if( _pIVideoInfo != NULL )
	{
		IVideoInfo::GetMediaInfoPoolInstance()->FreeObj(_pIVideoInfo);
		_pIVideoInfo = NULL;
	}
	_Map.clear();
	_List.clear();
}

MediaVideoStream::HFNStreamProcessEvent MediaVideoStream::Open( const _string_type& sUrl, 
	const HFNReady& hReady,
	const HFNClose& hClose,
	const HFNMediaVideoIdle& hIdle )
{
	_hReady = hReady;
	_hClose = hClose;
	_hIdle = hIdle;
	_sUrl = sUrl;
	return function20_bind_Ex(&MediaVideoStream::HandleStreamProcessEvent, shared_from_this());
}

void MediaVideoStream::Close( void )
{
	if( _hClose )
		_hClose();
}

MediaVideoStream::tagMediaInfo* MediaVideoStream::GetMediaInfo( void )
{
	return &_Info;
}

int MediaVideoStream::Register( const _string_type& sSessionUid, const RtspSession_sptr& sptr)
{
	int iRet = -1;
	tagRtspSession_INFO_sptr	Info_sptr(_Pool.AllocObj<tagRtspSession_INFO>(),
		function20_bind_Ex(&MemPool_type::FreeObj<tagRtspSession_INFO>, &_Pool));

	RtspSession_Map::iterator iter;
	_Lock.Lock();
	iter = _Map.find(sSessionUid);
	if( iter == _Map.end() )
	{
		Info_sptr->uStatus = enRtspSessiontatus::EN_RUN_STATUS;
		Info_sptr->RtspSession = sptr;
		_Map.insert(std::make_pair(sSessionUid, Info_sptr));
		_List.push_back(Info_sptr);
		iRet = 1;
	}
	_uUserCount ++;
	_Lock.UnLock();

	return iRet;
}

void MediaVideoStream::unRegister( const _string_type& sSessionUid )
{
	tagRtspSession_INFO_sptr sptr;
	RtspSession_Map::iterator iter;
	_Lock.Lock();
	iter = _Map.find(sSessionUid);
	if( iter != _Map.end() )
	{
		sptr = iter->second;
		_Map.erase(iter);
		_uUserCount --;
	}
	_Lock.UnLock();

	if( sptr )
	{
		sptr->uStatus = enRtspSessiontatus::EN_STOP_STATUS;
	}

	if( _uUserCount <= 0 && _hIdle )
		_hIdle(_sUrl, _uVideoType);
}

void MediaVideoStream::HandleStreamProcessEvent( NETHANDLE Node, const StreamBuf_ptr& Buf_sptr )
{
	if( !_isReady )
	{
		tagVideoPlayload_INFO	Playload;
		_pIVideoInfo->Parse( Buf_sptr->data, Buf_sptr->payload, &Playload);
		if( _pIVideoInfo->isReady() )
		{
			_isReady = true;
			_Info.sProfileLevelID = _pIVideoInfo->Get_SDP_ProfileLevelID();
			_Info.sSpropParameterSets = _pIVideoInfo->Get_SDP_SpropParameterSets();
			_Info.uFramerate = _pIVideoInfo->GetStreamInfo()->uMax_Framerate;
			_Info.uWidth = _pIVideoInfo->GetStreamInfo()->uWidth;
			_Info.uHeight = _pIVideoInfo->GetStreamInfo()->uHeight;
		} 

		if( _hReady )
		{
			_hReady(shared_from_this());
		}
	}
	else
	{
		for( RtspSession_List::iterator iter = _List.begin(); iter != _List.end(); )
		{
			if( (*iter) )
			{
				switch((*iter)->uStatus)
				{
				case enRtspSessiontatus::EN_STOP_STATUS:
					_Lock.Lock();
					iter = _List.erase(iter);
					_Lock.UnLock();
					break;
				case enRtspSessiontatus::EN_RUN_STATUS:
					(*iter)->RtspSession->HandleStreamRecv(Node, Buf_sptr);
					_Lock.Lock();
					++ iter;
					_Lock.UnLock();
					break;
				}
			}
		}
	}
}
