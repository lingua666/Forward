
#include <libFrameWork_RtspClients/RtspClients.h>

RtspClients::RtspClients( void )
	: _Pool( sizeof(RtspClient_Impl) )
	, _uSeq( 0 )
{
	
}

RtspClients::~RtspClients( void )
{
	Release();
}

void RtspClients::Init( UInt8 uRtspIOThreadNum, UInt8 uRtspProcThreadNum,
	UInt8 uRtpIOThreadNum, UInt8 uRtpProcThreadNum )
{
	//test
	LOG_Print_SetLog(libFrameWork_RtspClients, MLog_GetAllLevel())
	//test

	GetAsynStreamClientsInstance()->Init(uRtspIOThreadNum, uRtspProcThreadNum, true);
	GetUDPStreamInstance()->Init(uRtpIOThreadNum, uRtpProcThreadNum, true);
	GetTimerEventInstance()->Init(1);
}

void RtspClients::Release( void )
{
	GetAsynStreamClientsInstance()->Release();
	GetUDPStreamInstance()->Release();
	GetTimerEventInstance()->Release();
	_ClientsMap.clear();
}

_string_type RtspClients::Open( const _string_type& sRtspPath, UInt8 uStreamType,
	UInt16 uRtpPort, HFNExceptNT hExcepNT, HFNSdpNT hSdpNT,
	HFNSuccessNT hSuccessNT, HFNRtpDataNT hRtpDataNT )
{
	int iRet = -1;
	Sleep(1); //必须延迟1毫秒,避免时间太短，生成的会话UID重复
	_string_type sSessionID = Timestamp_type().DateMilliString();
	RtspClient_Impl_sptr sptr( _Pool.AllocObj<RtspClient_Impl>(),
		function20_bind_Ex(&MemPool_type::FreeObj<RtspClient_Impl>, &_Pool) );
	sptr->Init();
	sptr->SetSeq(++ _uSeq);
	iRet = sptr->Open(sSessionID, uStreamType, sRtspPath, uRtpPort, 
		function20_bind(&RtspClients::HandleExcept, this, hExcepNT, _function_::_1, _function_::_2),
		hSdpNT, function20_bind(&RtspClients::HandleSuccess, this, hSuccessNT, _function_::_1),
		hRtpDataNT);
	if( iRet == 1 )
	{
		_Lock.Lock();
		if( _ClientsMap.find(sSessionID) != _ClientsMap.end() )
		{//已经存在会话ID
			_Lock.UnLock();
			return "";
		}

		_ClientsMap.insert(std::make_pair(sSessionID, sptr));
		_Lock.UnLock();
	}
	return sSessionID;
}

int RtspClients::Play( const char* c_szSessionID )
{
	RtspClient_Impl_sptr sptr;
	ClientsMap::iterator iter;
	_Lock.Lock();
	iter = _ClientsMap.find(c_szSessionID);
	if(iter != _ClientsMap.end())
	{
		sptr = iter->second;
	}
	_Lock.UnLock();

	int iRet = -1;
	if(sptr)
	{
		iRet = sptr->Play();
		if( iRet == -1 )
		{
			_Lock.Lock();
			_ClientsMap.erase(c_szSessionID);
			_Lock.UnLock();
		}
	}

	return iRet;
}

int RtspClients::Stop( const char* c_szSessionID )
{
	RtspClient_Impl_sptr sptr;
	_Lock.Lock();
	ClientsMap::iterator iter = _ClientsMap.find(c_szSessionID);
	if( iter != _ClientsMap.end() )
	{
		sptr = iter->second;
	}
	_uSeq = sptr->GetSeq();
	_Lock.UnLock();
	if( !sptr )
		return -1;

	sptr->Close();
	return 1;
}

void RtspClients::Close( const char* c_szSessionID )
{
	RtspClient_Impl_sptr sptr;
	_Lock.Lock();
	ClientsMap::iterator iter = _ClientsMap.find(c_szSessionID);
	if( iter != _ClientsMap.end() )
	{
		sptr = iter->second;
		_ClientsMap.erase(iter);
		_uSeq = sptr->GetSeq();
	}
	_Lock.UnLock();
	if( sptr )
		sptr->Close();
}

void RtspClients::SetResultEvent( const char* c_szSessionID, const HFNExceptNT& hExcepNT,
								const HFNSuccessNT& hSuccessNT )
{
	RtspClient_Impl_sptr sptr;
	ClientsMap::iterator iter;
	_Lock.Lock();
	iter = _ClientsMap.find(c_szSessionID);
	if(iter != _ClientsMap.end())
	{
		sptr = iter->second;
	}
	_Lock.UnLock();

	if(sptr)
	{
		sptr->SetResultEvent(function20_bind(&RtspClients::HandleExcept, this, hExcepNT, _function_::_1, _function_::_2),
			function20_bind(&RtspClients::HandleSuccess, this, hSuccessNT, _function_::_1));
	}
}

void RtspClients::SetDataEvent( const char* c_szSessionID, const HFNRtpDataNT& hRtpDataNT, const HFNSdpNT& hSdpNT )
{
	RtspClient_Impl_sptr sptr;
	ClientsMap::iterator iter;
	_Lock.Lock();
	iter = _ClientsMap.find(c_szSessionID);
	if(iter != _ClientsMap.end())
	{
		sptr = iter->second;
	}
	_Lock.UnLock();

	if(sptr)
	{
		sptr->SetDataEvent(hSdpNT, hRtpDataNT);
	}
}

tagMediaVideoINFO* RtspClients::GetMediaVideoInfo( const char* c_szSessionID )
{
	RtspClient_Impl_sptr sptr;
	ClientsMap::iterator iter;
	_Lock.Lock();
	iter = _ClientsMap.find(c_szSessionID);
	if(iter != _ClientsMap.end())
	{
		sptr = iter->second;
	}
	_Lock.UnLock();
	if(sptr)
	{
		return sptr->GetMediaVideoInfo();
	}

	return NULL;
}

void RtspClients::HandleExcept( const HFNExceptNT& hExcepNT,
	const char* c_szSessionID, Int32 iErrCode )
{
	RtspClient_Impl_sptr sptr;
	ClientsMap::iterator iter;
	_Lock.Lock();
	iter = _ClientsMap.find(c_szSessionID);
	if( iter != _ClientsMap.end() )
	{
		sptr = iter->second;
		_ClientsMap.erase(iter);
		_uSeq = sptr->GetSeq();
	}
	_Lock.UnLock();

	if( sptr )
	{
		sptr->Close();
	}
	
	if( hExcepNT && iErrCode != -1 )
	{
		hExcepNT(c_szSessionID, iErrCode);
	}
}

void RtspClients::HandleSuccess( const HFNSuccessNT& hSuccess, 
	const char* c_szSessionID )
{
	if( hSuccess )
	{
		hSuccess(c_szSessionID);
	}
}

_string_type RtspClients::GetMediaVideoHeadBuffer( const char* c_szSessionID )
{
	RtspClient_Impl_sptr sptr;
	ClientsMap::iterator iter;
	_Lock.Lock();
	iter = _ClientsMap.find(c_szSessionID);
	if(iter != _ClientsMap.end())
	{
		sptr = iter->second;
	}
	_Lock.UnLock();
	if(sptr)
	{
		return sptr->GetMediaVideoHeadBuffer();
	}

	return "";
}
