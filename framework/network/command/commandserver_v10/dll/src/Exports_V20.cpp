
#include <dllNetFrame_CmdServer_V10/Exports_V20.h>
#include <libNetFrame_CmdServer_V10/CommandServer_V10.h>

LOG_OBJ_DECL(Cmd_S_V20_Module)

void _CALLTYPE	g_CmdSvr10ClientClose201609141640_V20( pfnCmd_S_V20_Close pClientClose,
												 void* pUser,
                                                 const _string_type& sIP,
												 int uPort,
												 NETHANDLE Handle )
{
	if( pClientClose != NULL )
	{
		pClientClose(pUser, sIP.c_str(), uPort, Handle);
	}
}

/*****************************************************************
/*函数说明：	初始化
/*参数说明：	uWorkerThreads: 数据处理线程个数
/*				
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_V20_Init( int uWorkerThreads )
{
	UInt8 uNum = (uWorkerThreads == 0 ? 1 : uWorkerThreads);
	GetCommandServer_V10Instance()->Init(uNum);
	return 1;
}

/*****************************************************************
/*函数说明：	监听端口
/*参数说明：	u16Port: 端口号
/*				pfnAccept: 客户端连接回调函数
/*				pUser: 用户数据
/*				uIOThreadNum: IO线程数
/*				
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_V20_Listen( int u16Port,
											pfnCmd_S_V20_Accept pfnAccept,
											void* pUser, UInt8 uIOThreadNum )
{
	if( GetCommandServer_V10Instance()->Listen(u16Port,
		function20_bind(pfnAccept, pUser, _function_::_1, _function_::_2, _function_::_3)) )
	{
		GetCommandServer_V10Instance()->Run( uIOThreadNum == 0 ? get_processor_number() * 2 + 2 : uIOThreadNum );
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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_V20_Close( void )
{
	GetCommandServer_V10Instance()->Stop();
	return 1;
}


/*****************************************************************
/*函数说明：	释放服务器资源(必须在调用MCmd_S_V20_Close后执行该接口)
/*参数说明：	
/*				
/*返回值：		>0:成功  <1: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_V20_Free( void )
{
	GetCommandServer_V10Instance()->Release();
	return 1;
}


/*****************************************************************
/*函数说明：	关闭客户端连接
/*参数说明：	handle: 64bit客户端句柄
/*				
/*返回值：		>0:成功  <1: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_V20_DisConnectClient( NETHANDLE handle )
{
	GetCommandServer_V10Instance()->Close(handle);
	return 1;
}


/*****************************************************************
/*函数说明：	设置客户端断开回调函数
/*参数说明：	handle: 64bit客户端句柄
/*				pClientClose: 回调函数地址
/*				pUser: 用户数据
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_V20_SetClientDisConnect( NETHANDLE handle,
													pfnCmd_S_V20_Close pClientClose,
													void* pUser )
{
	/*GetCommandServer_V10Instance()->SetCloseHandle(handle,
						function20_bind(pClientClose, GetIPv4ToString(handle).c_str(),
									GetPort(handle), _function_::_1));*/

	return GetCommandServer_V10Instance()->SetDestroyHandle(handle,
		function20_bind(g_CmdSvr10ClientClose201609141640_V20, pClientClose,
		pUser, GetIPv4ToString(handle),
		GetPort(handle), _function_::_1));
}

/*****************************************************************
/*函数说明：	设置客户端接收回调函数
/*参数说明：	handle: 64bit客户端句柄
/*				pClientRecData: 回调函数地址
/*				pUser: 用户数据
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_V20_SetClientRecData( NETHANDLE handle,
												pfnCmd_S_V20_RecData pClientRecData,
												void* pUser )
{
	GetCommandServer_V10Instance()->SetRecvHandle(handle,
		function20_bind(pClientRecData, pUser, _function_::_1, _function_::_2, _function_::_3));
	return 1;
}

/*****************************************************************
/*函数说明：	向客户端发送数据
/*参数说明：	handle: 64bit客户端句柄
/*				c_pData: 数据缓冲区
/*				u32Size: 数据长度
/*				
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_V20_SentData( NETHANDLE handle,
															const char* c_pData,
															int u32Size )
{
	return GetCommandServer_V10Instance()->Send(handle, c_pData, u32Size);
}
