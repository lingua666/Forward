
#include <dllFrameWork_RtspClients/Exports.h>
#include <libFrameWork_RtspClients/RtspClients.h>

fpnMRtspClients_Except g_fnExcept201806191635 = NULL;
fpnMRtspClients_Success	g_fnSuccess201806191635 = NULL;
fpnMRtspClients_RtpData g_fnRtpData201806191635 = NULL;

void g_ExceptNT201806191635( void* pUser, const char* c_szSessionUid, Int32 iErrCode )
{
	if( g_fnExcept201806191635 )
	{
		g_fnExcept201806191635(pUser, _string_type::StringToInt64(c_szSessionUid), iErrCode);
	}
}


void g_SdpNT201806191635( void* pUser, const char* c_szSessionUid, RtspClient_Impl::tagSdpInfo* SdpInfo )
{

}

void g_SuccessNT201806191635( void* pUser,  const char* c_szSessionUid )
{
	if( g_fnSuccess201806191635 )
	{
		g_fnSuccess201806191635(pUser, _string_type::StringToInt64(c_szSessionUid));
	}
}

void g_RtpDataNT201806191635( void* pUser, const char* c_szSessionUid,
	const char* c_szData, UInt32 uDataSize )
{
	if( g_fnRtpData201806191635 )
	{
		g_fnRtpData201806191635(pUser, _string_type::StringToInt64(c_szSessionUid), c_szData, uDataSize);
	}
}


/*****************************************************************
/*函数说明：	初始化
/*参数说明：	uRtspIOThreadNum: Rtsp网络连接处理线程个数
/*				uRtspProcThreadNum: Rtsp数据处理线程个数
/*				uRtpIOThreadNum: Rtp网络连接处理线程个数
/*				uRtpProcThreadNum: Rtp数据处理线程个数
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspClients_Init( unsigned char uRtspIOThreadNum,
	unsigned char uRtspProcThreadNum,
	unsigned char uRtpIOThreadNum,
	unsigned char uRtpProcThreadNum )
{
	Singleton<RtspClients>::instance()->Init(uRtspIOThreadNum, uRtspProcThreadNum,
				uRtpIOThreadNum, uRtpProcThreadNum);
	return 1;
}


/*****************************************************************
/*函数说明：	模块释放
/*参数说明：	
/*				
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MRtspClients_Free( void )
{
	Singleton<RtspClients>::instance()->Release();
}


/*****************************************************************
/*函数说明：	打开Rtsp会话
/*参数说明：	c_szRtspPath: Rtsp路径
/*				iStreamType: 流类型	
/*							0: TCP
/*							1: UDP
/*				iRtpPort: rtp端口号
/*返回值：		会话句柄
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS THandle _CALLTYPE MRtspClients_Open( const char* c_szRtspPath,
	int iStreamType, int iRtpPort )
{
	_string_type sSessionUid = Singleton<RtspClients>::instance()->Open(c_szRtspPath, iStreamType, iRtpPort);
	return _string_type::StringToInt64(sSessionUid);
}


/*****************************************************************
/*函数说明：	设置Rtsp会话结果回调函数
/*参数说明：	Handle:  rtsp会话句柄
/*				fpExcept: 异常回调函数
/*				pExceptUser: 用户携带的信息
/*				fpSuccess: 成功回调函数
/*				pUser: 用户携带的信息
/*				
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspClients_SetResultEvent( THandle Handle,
	fpnMRtspClients_Except fpExcept, void* pExceptUser,
	fpnMRtspClients_Success fpSuccess, void* pUser )
{
	g_fnExcept201806191635 = fpExcept;
	g_fnSuccess201806191635 = fpSuccess;
	_string_type sSessionUid = _string_type::NumberToStr(Handle);
	Singleton<RtspClients>::instance()->SetResultEvent(sSessionUid.c_str(),
		function20_bind(g_ExceptNT201806191635, pExceptUser, _function_::_1, _function_::_2),
		function20_bind(g_SuccessNT201806191635, pUser, _function_::_1));
	return 1;
}


/*****************************************************************
/*函数说明：	设置数据回调函数
/*参数说明：	Handle:  rtsp会话句柄
/*				fpRtpData: rtp数据回调函数
/*				pUser: 用户携带的信息
/*				
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspClients_SetDataEvent( THandle Handle, 
	fpnMRtspClients_RtpData fpRtpData, void* pUser )
{
	g_fnRtpData201806191635 = fpRtpData;
	_string_type sSessionUid = _string_type::NumberToStr(Handle);
	Singleton<RtspClients>::instance()->SetDataEvent(sSessionUid.c_str(),
		function20_bind(g_RtpDataNT201806191635, pUser, _function_::_1, _function_::_2, _function_::_3));
	return 1;
}


/*****************************************************************
/*函数说明：	播放Rtsp会话
/*参数说明：	Handle:  rtsp会话句柄
/*				
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspClients_Play( THandle Handle )
{
	_string_type sSessionUid = _string_type::NumberToStr(Handle);
	return Singleton<RtspClients>::instance()->Play(sSessionUid.c_str());
}


/*****************************************************************
/*函数说明：	关闭Rtsp会话
/*参数说明：	Handle:  rtsp会话句柄
/*				
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MRtspClients_Close( THandle Handle )
{
	_string_type sSessionUid = _string_type::NumberToStr(Handle);
	Singleton<RtspClients>::instance()->Close(sSessionUid.c_str());
}