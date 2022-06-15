
#include <dllNetFrame_CmdServer_V10/Exports.h>
#include <libNetFrame_CmdServer_V10/CommandServer_V10.h>

DLL_INIT_DEF(g_pTagTModuleInfoRecord_201602172200,
	"45000001000V2016021700000000VZZZ00000000",
	"系统通信指令模块",
	"V1.0.0.1",
	"20160217",
	"")

DLL_LOG_DEF(Cmd_SV10_Module)

void _CALLTYPE	g_CmdSvr10ClientClose201609141640( pfnCmd_SV10_Close pClientClose,
                                                 const _string_type& sIP,
												 int uPort,
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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SV10_Init( int uWorkerThreads )
{
	UInt8 uNum = (uWorkerThreads == 0 ? 1 : uWorkerThreads);
	GetCommandServer_V10Instance()->Init(uNum);
	return 1;
}

/*****************************************************************
/*函数说明：	监听端口
/*参数说明：	u16Port: 端口号
				pCMD_ServerAccept: 客户端连接回调函数
/*				
/*返回值：		>0:成功  <1: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SV10_Listen( int u16Port,
											pfnCmd_SV10_Accept pfnAccept )
{
	if( GetCommandServer_V10Instance()->Listen(u16Port, function20_bind_Ex(pfnAccept)) )
	{
		GetCommandServer_V10Instance()->Run( get_processor_number() * 2 + 2 );
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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SV10_Close( void )
{
	GetCommandServer_V10Instance()->Stop();
	return 1;
}


/*****************************************************************
/*函数说明：	释放服务器资源(必须在调用CMD_Server_MClose后执行该接口)
/*参数说明：	
/*				
/*返回值：		>0:成功  <1: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SV10_Free( void )
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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SV10_DisConnectClient( NETHANDLE handle )
{
	GetCommandServer_V10Instance()->Close(handle);
	return 1;
}


/*****************************************************************
/*函数说明：	设置客户端断开回调函数
/*参数说明：	handle: 64bit客户端句柄
				pCMD_ClientClose: 回调函数地址
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SV10_SetClientDisConnect( NETHANDLE handle,
													pfnCmd_SV10_Close pClientClose )
{
	/*GetCommandServer_V10Instance()->SetCloseHandle(handle,
						function20_bind(pClientClose, GetIPv4ToString(handle).c_str(),
									GetPort(handle), _function_::_1));*/

	return GetCommandServer_V10Instance()->SetDestroyHandle(handle,
		function20_bind(g_CmdSvr10ClientClose201609141640, pClientClose,
		GetIPv4ToString(handle),
		GetPort(handle), _function_::_1));
}

/*****************************************************************
/*函数说明：	设置客户端接收回调函数
/*参数说明：	handle: 64bit客户端句柄
				pCMD_ClientRecData: 回调函数地址
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SV10_SetClientRecData( NETHANDLE handle,
												pfnCmd_SV10_RecData pClientRecData )
{
	GetCommandServer_V10Instance()->SetRecvHandle(handle, function20_bind_Ex(pClientRecData));
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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SV10_SentData( NETHANDLE handle,
															const char* c_pData,
															int u32Size )
{
	return GetCommandServer_V10Instance()->Send(handle, c_pData, u32Size);
}
