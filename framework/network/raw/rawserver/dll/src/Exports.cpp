
#include <dllNetFrame_RawServer/Exports.h>
#include <libNetFrame_RawServer/RawServer.h>

DLL_INIT_DEF(g_pTagTModuleInfoRecord_201602172200,
	"45000001000V2016021700000000VZZZ00000000",
	"系统通信指令模块",
	"V1.0.0.1",
	"20160217",
	"")

DLL_LOG_DEF(Raw_S_Module)

void _CALLTYPE	g_RawSvrClientClose201609141640( pfnRaw_S_ClientClose pClientClose,
                                                 const _string_type& sIP,
												 UInt16 uPort,
												 NETHANDLE Handle )
{
	if( pClientClose != NULL )
	{
		pClientClose(sIP.c_str(), uPort, Handle);
	}
}

/*****************************************************************
/*函数说明：	初始化
/*参数说明：	uWorkerThreads: 数据处理线程个数
/*				
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRaw_S_Init( UInt8 uWorkerThreads )
{
	/*UInt8 uNum = (uWorkerThreads == 0 ? 1 : uWorkerThreads);
	GetStreamSvrInstance()->Init(uNum);*/
	return 1;
}

/*****************************************************************
/*函数说明：	监听端口
/*参数说明：	u16Port: 端口号
				pfnRaw_S_Accept: 客户端连接回调函数
/*				
/*返回值：		>0:成功  <1: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRaw_S_Listen( UInt16 u16Port,
											pfnRaw_S_Accept pfnAccept )
{
	if( GetRawSvrInstance()->Listen(u16Port, function20_bind_Ex(pfnAccept)) )
	{
		GetRawSvrInstance()->Run( get_processor_number() * 2 + 2 );
		return 1;
	}
	return -1;
}

/*****************************************************************
/*函数说明：	关闭服务器
/*参数说明：	
/*				
/*返回值：		>0:成功  <1: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRaw_S_Close( void )
{
	GetRawSvrInstance()->Stop();
	return 1;
}


/*****************************************************************
/*函数说明：	释放服务器资源(必须在调用CMD_Server_MClose后执行该接口)
/*参数说明：	
/*				
/*返回值：		>0:成功  <1: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRaw_S_Free( void )
{
	GetRawSvrInstance()->Release();
	return 1;
}


/*****************************************************************
/*函数说明：	关闭客户端连接
/*参数说明：	handle: 64bit客户端句柄
/*				
/*返回值：		>0:成功  <1: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRaw_S_DisConnectClient( NETHANDLE handle )
{
	GetRawSvrInstance()->Close(handle);
	return 1;
}


/*****************************************************************
/*函数说明：	设置客户端断开回调函数
/*参数说明：	handle: 64bit客户端句柄
				pCMD_ClientClose: 回调函数地址
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRaw_S_SetClientDisConnect( NETHANDLE handle,
													pfnRaw_S_ClientClose pClientClose )
{
	return GetRawSvrInstance()->SetDestroyHandle(handle,
		function20_bind(g_RawSvrClientClose201609141640, pClientClose,
		GetIPv4ToString(handle),
		GetPort(handle), _function_::_1));
}

/*****************************************************************
/*函数说明：	设置客户端接收回调函数
/*参数说明：	handle: 64bit客户端句柄
				pCMD_ClientRecData: 回调函数地址
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRaw_S_SetClientRecData( NETHANDLE handle,
												pfnRaw_S_ClientRecData pClientRecData )
{
	GetRawSvrInstance()->SetRecvHandle(handle, function20_bind_Ex(pClientRecData));
	return 1;
}

/*****************************************************************
/*函数说明：	向客户端发送数据
/*参数说明：	handle: 64bit客户端句柄
				c_pData: 数据缓冲区
				u32Size: 数据长度
/*				
/*返回值：		>0:成功		<=0:失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRaw_S_SentData( NETHANDLE handle,
															const char* c_pData,
															UInt32 u32Size )
{
	return GetRawSvrInstance()->Send(handle, c_pData, u32Size);
}
