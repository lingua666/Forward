
#include <dllNetFrame_AsyncCmdClients/Exports_V20.h>
#include <libNetFrame_AsyncCmdClients/CommandClients.h>

LOG_OBJ_DECL(Cmd_C_V20_Module)

fpnMCmd_C_V20_Connect	g_fpnMCmd_C_Connect20160526_v20;


void _CALLTYPE	gConnectResultBack20160526_V20( fpnMCmd_C_V20_Connect fpnConnect, 
										void* pUser,
										const _string_type& sIP,
										int u16Port,
										NETHANDLE Handle )
{
	if( fpnConnect != NULL )
	{
		fpnConnect(pUser, sIP.c_str(), u16Port, Handle);
	}
}

void _CALLTYPE	gCloseBack20160526_V20( fpnMCmd_C_V20_Close fpnClose, 
									void* pUser,
									const _string_type& sIP,
									int u16Port,
									NETHANDLE Handle )
{
	if( fpnClose != NULL )
	{
		fpnClose(pUser, sIP.c_str(), u16Port, Handle);
	}
}

/*****************************************************************
/*函数说明：	初始化客户端集
/*参数说明：	iIOThreadNum : IO操作线程数量, 无特殊情况，请默认
/*				iProcThreadNum : 数据处理线程数量, 无特殊情况，请默认
/*				
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_V20_Init( int iIOThreadNum, int iProcThreadNum )
{
	//test
	//LOG_Print_SetLog(Cmd_C_Module, 1)
	//LOG_Print_SetLog(Cmd_C_DataError, 1)
	//test

	LOG_Print_Info(Cmd_C_V20_Module,"MCmd_C_V20_Init():")
	
	GetAsynCmdClientsInstance()->Init( iIOThreadNum, iProcThreadNum );
	return 1;
}

/*****************************************************************
/*函数说明：	释放客户端模块资源(在不使用该模块时使用)
/*参数说明：	
/*				
/*返回值：		>1:成功  <1: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_V20_Free( void )
{
	LOG_Print_Info(Cmd_C_V20_Module,"MCmd_C_V20_Free():")

	GetAsynCmdClientsInstance()->Release();
	return 1;
}

/*****************************************************************
/*函数说明：	设置连接响应回调函数
/*参数说明：	pCMD_ClientConnect: 回调函数地址
/*				
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_V20_SetConnect( fpnMCmd_C_V20_Connect fpnConnect )
{
	LOG_Print_Info(Cmd_C_V20_Module,"MCmd_C_V20_SetConnect():")

	g_fpnMCmd_C_Connect20160526_v20 = fpnConnect;
	return 1;
}

/*****************************************************************
/*函数说明：	连接服务器
/*参数说明：	c_szIP: 服务器IP
				u16Port: 服务器端口号
				pUser: 连接用户数据
/*				
/*返回值：		64bit连接句柄
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS NETHANDLE _CALLTYPE MCmd_C_V20_Connect( const char* c_szIP,
																int u16Port,void* pUser )
{
	LOG_Print_Info(Cmd_C_V20_Module,"MCmd_C_V20_Connect():\n\tc_szIP:%s\n\tu16Port:%d",c_szIP, u16Port)

	return GetAsynCmdClientsInstance()->Connect(c_szIP, u16Port,
							function20_bind(gConnectResultBack20160526_V20,
									g_fpnMCmd_C_Connect20160526_v20, pUser,
									_string_type(c_szIP), u16Port, _function_::_1));
}

/*****************************************************************
/*函数说明：	关闭连接
/*参数说明：	handle: 64bit连接句柄
/*				
/*返回值：
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_V20_Close( NETHANDLE handle )
{
	LOG_Print_Info(Cmd_C_V20_Module,"MCmd_C_V20_Close():\n\thandle:%lld",handle)

	GetAsynCmdClientsInstance()->Close(handle);
	return 1;
}

/*****************************************************************
/*函数说明：	设置断开回调函数
/*参数说明：	handle: 64bit连接句柄
				pCMD_ClientRecData: 回调函数地址
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_V20_SetClose( NETHANDLE handle,
															fpnMCmd_C_V20_Close fpnClose,
															void* pUser )
{
	LOG_Print_Info(Cmd_C_V20_Module,"MCmd_C_V20_SetClose():\n\thandle:%lld",handle)

	return GetAsynCmdClientsInstance()->SetDestroyHandle(handle,
							function20_bind(gCloseBack20160526_V20, fpnClose,
							pUser, GetIPv4ToString(handle),
							GetPort(handle),_function_::_1));
}


/*****************************************************************
/*函数说明：	设置接收回调函数
/*参数说明：	handle: 64bit连接句柄
				pCMD_ClientRecData: 回调函数地址
				pUser: 用户参数
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_V20_SetRecData( NETHANDLE handle,
															fpnMCmd_C_V20_RecData fpnRecData,
															void* pUser )
{
	LOG_Print_Info(Cmd_C_V20_Module,"MCmd_C_V20_SetRecData():\n\thandle:%lld",handle)

	GetAsynCmdClientsInstance()->SetRecvHandle(handle,
										function20_bind(fpnRecData, pUser,
												_function_::_1, _function_::_2, _function_::_3));
	return 1;
}

/*****************************************************************
/*函数说明：	发送数据
/*参数说明：	handle: 64bit连接句柄
				c_pData: 数据缓冲区
				u32Size: 数据长度
/*				
/*返回值：		>0:成功		<=0:失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_V20_SentData( NETHANDLE handle,
														const char* c_pData,
														int u32Size )
{
	LOG_Print_Info(Cmd_C_V20_Module,"MCmd_C_V20_SentData():\n\thandle:%lld\n\tSize:%d\n\tData:%s",
					handle, u32Size, c_pData)

	return GetAsynCmdClientsInstance()->Send(handle, c_pData, u32Size);
}

/*****************************************************************
/*函数说明：	地址错误发生
/*参数说明：	szData: 错误缓冲区
/*				
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MCmd_C_V20_Error( const char* szData )
{
	/*LOG_Print_Info(Cmd_C_DataError,"MCmd_C_Error(): Data Address:%p", szData)
	LOG_Print_Info(Cmd_C_DataError,"MCmd_C_Error(): Data Content:%s", szData)*/
}