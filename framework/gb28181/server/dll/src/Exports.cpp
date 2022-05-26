
#include <dllFrameWork_RtspSvr/Exports.h>
#include <libFrameWork_RtspSvr/RtspServer.h>
#include <libFrameWork_Rtsp/H265Info.h>

class MediaStreamProcess201807191056
{
public:
	#define STREAM_PROCESS_DATA_MAX	(2048 * 10)
	typedef MediaStreamProcess201807191056	self_type;
	typedef RtspServer::HFNStreamProcessEvent	HFNStreamProcessEvent;
	typedef Container_HashMap_type<THandle, HFNStreamProcessEvent>	ProcessEvent_Map;
public:
	MediaStreamProcess201807191056( void )
		: _Pool( STREAM_PROCESS_DATA_MAX )
	{

	}

	~MediaStreamProcess201807191056( void )
	{

	}

	int Add( THandle Handle, const HFNStreamProcessEvent& hEvent )
	{
		ProcessEvent_Map::iterator iter;
		_Lock.Lock();
		iter = _Map.find(Handle);
		if( iter != _Map.end() )
		{
			_Lock.UnLock();
			return -1;
		}
		_Map.insert(std::make_pair(Handle, hEvent));
		_Lock.UnLock();
		return 1;
	}

	self_type::HFNStreamProcessEvent Find( THandle Handle )
	{
		HFNStreamProcessEvent hEvent;
		ProcessEvent_Map::iterator iter;
		_Lock.Lock();
		iter = _Map.find(Handle);
		if( iter != _Map.end() )
		{
			hEvent = iter->second;
		}
		_Lock.UnLock();

		return hEvent;
	}

	void Remove( THandle Handle )
	{
		ProcessEvent_Map::iterator iter;
		_Lock.Lock();
		iter = _Map.find(Handle);
		if( iter != _Map.end() )
		{
			_Map.erase(iter);
		}
		_Lock.UnLock();
	}

	int PutData( THandle Handle, const char* c_szData, int iSize )
	{
		if( STREAM_PROCESS_DATA_MAX < iSize )
			return -2;

		RtspServer::HFNStreamProcessEvent hEvent = Find(Handle);
		if( !hEvent )
			return -1;

		_io_net_::_buf_hdr* pHdr = (_io_net_::_buf_hdr*)_Pool.Alloc();
		memcpy(pHdr->data, c_szData, iSize);
		pHdr->payload = iSize;
		StreamBuf_ptr	sptr(pHdr);
		hEvent(-1, sptr);
		return 1;
	}

private:
	ProcessEvent_Map	_Map;
	CLock				_Lock;
	MemPool_type		_Pool;
};

fpnMRtspSessionAccept	g_fpnMRtspSessionAccept201807191214 = NULL;
fpnMRtspSessionDestroy	g_fpnMRtspSessionDestroy201807191214 = NULL;
fpnMRtspSessionOPStatus g_fpnMRtspSessionOPStatus201807191214 = NULL;


void	HandleSessionOPStatusEvent201807191214( void* pUser,
	const RtspServer::RtspSession_sptr& sptr,
	const _string_type& sSessionUid,
	UInt8 uStatus )
{
	if( g_fpnMRtspSessionOPStatus201807191214 != NULL )
	{
		g_fpnMRtspSessionOPStatus201807191214(pUser, sSessionUid.c_str(), uStatus);
	}
}

void	HandleAccept201807191214( void* pUser, const _string_type& sSessionUid,
	const _string_type& sChildUrl,
	const RtspServer::RtspSession_sptr& sptr )
{
	if( g_fpnMRtspSessionAccept201807191214 != NULL )
	{
		g_fpnMRtspSessionAccept201807191214(pUser, sSessionUid.c_str(), sChildUrl.c_str());
	}
}

void	HandleDestroy201807191214( void* pUser, const _string_type& sSessionUid,
	const RtspServer::RtspSession_sptr& sptr )
{
	if( g_fpnMRtspSessionDestroy201807191214 != NULL )
	{
		g_fpnMRtspSessionDestroy201807191214(pUser, sSessionUid.c_str());
	}
}


/*****************************************************************
/*函数说明：	初始化
/*参数说明：	c_szUser: 用户名
/*				c_szPassword: 密码
/*				c_szHost: 主机名
/*				c_szPort: 端口号
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspSvr_Init( const char* c_szUser,
	const char* c_szPassword,
	const char* c_szHost,
	const char* c_szPort )
{
	return Singleton<RtspServer>::instance()->Init(c_szUser, c_szPassword, c_szHost, c_szPort);
}


/*****************************************************************
/*函数说明：	模块释放
/*参数说明：	
/*				
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MRtspSvr_Free( void )
{
	Singleton<RtspServer>::instance()->Release();
}


/*****************************************************************
/*函数说明：	运行rtsp服务器
/*参数说明：	iPort: 监听端口号
/*				iIOThreadNum: 处理网络事件的线程数	
/*				iProcThreadNum: 处理接收到的网络数据的线程数
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspSvr_Run(  int iPort, int iIOThreadNum,
	int iProcThreadNum )
{
	return Singleton<RtspServer>::instance()->Start(iPort, iIOThreadNum, iProcThreadNum);
}


/*****************************************************************
/*函数说明：	停止rtsp服务器
/*参数说明：	
/*				
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspSvr_Stop( void )
{
	return Singleton<RtspServer>::instance()->Stop();
}


/*****************************************************************
/*函数说明：	设置rtsp服务器事件
/*参数说明：	c_szSessionUid:  会话uid
/*				fpAccept: 接收rtsp会话回调函数
/*				pAptUser: 接收会话回调函数携带的用户数据
/*				fpDestroy: 接收会话销毁回调函数
/*				pDesUser: 销毁回调函数所携带的用户数据
/*				fpStatus: rtsp会话状态回调函数
/*				pStaUser: 状态回调函数携带的用户数据
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspSvr_SetEvent( const char* c_szSessionUid, 
	fpnMRtspSessionAccept fpAccept, void* pAptUser,
	fpnMRtspSessionDestroy fpDestroy, void* pDesUser,
	fpnMRtspSessionOPStatus fpStatus, void* pStaUser )
{
	Singleton<RtspServer>::instance()->SetSessionEvent( function20_bind(HandleAccept201807191214, pAptUser,
		_function_::_1, _function_::_2, _function_::_3),
		function20_bind(HandleDestroy201807191214, pDesUser, _function_::_1, _function_::_2),
		function20_bind(HandleSessionOPStatusEvent201807191214, pStaUser, _function_::_1,
						_function_::_2, _function_::_3) );
	return 1;
}

/*****************************************************************
/*函数说明：	关闭rtsp会话
/*参数说明：	c_szSessionUid:  会话uid
/*				
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspSvr_Close( const char* c_szSessionUid )
{
	Singleton<RtspServer>::instance()->Close(c_szSessionUid);
	return 1;
}


/*****************************************************************
/*函数说明：	增加一个rtsp地址
/*参数说明：	c_szUrl:  rtsp会话连接地址(不包含rtsp主机信息部分)
/*				iVideoType: 视频类型
/*							0: H264
/*							0: H265
/*返回值：		-1: 失败	>0: 句柄
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS THandle _CALLTYPE MRtspSvr_AddMediaStream( const char* c_szUrl, int iVideoType )
{
	typedef RtspServer::HFNClose	HFNClose;

	THandle	Handle = -1;
	RtspServer::HFNStreamProcessEvent hEvent = Singleton<RtspServer>::instance()->AddMediaStream(c_szUrl,
		HFNClose(), iVideoType);
	if( hEvent )
	{
		Handle = (THandle)Timestamp_type().epochMicroseconds();
		Singleton<MediaStreamProcess201807191056>::instance()->Add(Handle, hEvent);
	}

	return Handle;
}


/*****************************************************************
/*函数说明：	往rtsp地址填充视频数据
/*参数说明：	Handle:		rtsp地址所对应的句柄（MRtspSvr_AddMediaStream所返回的句柄）
/*				c_szData:	视频数据
/*				iSize:		视频数据长度
/*
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspSvr_PutMediaStream( THandle Handle, const char* c_szData, int iSize )
{
	return Singleton<MediaStreamProcess201807191056>::instance()->PutData(Handle, c_szData, iSize);
}


/*****************************************************************
/*函数说明：	移除rtsp地址
/*参数说明：	c_szUrl:	rtsp地址
/*
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspSvr_RemoveMediaStream( const char* c_szUrl, THandle Handle )
{
	Singleton<MediaStreamProcess201807191056>::instance()->Remove(Handle);
	Singleton<RtspServer>::instance()->RemoveMediaStream(c_szUrl);
	return 1;
}
