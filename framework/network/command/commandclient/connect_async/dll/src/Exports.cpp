
#include <dllNetFrame_AsyncCmdClients/Exports.h>
#include <libNetFrame_AsyncCmdClients/CommandClients.h>

DLL_INIT_DEF(g_pTagTModuleInfoRecord_201602172205,
			"45000001000V2016021700000000VZZZ10000000",
			"系统通信指令模块",
			"V1.0.0.1",
			"20160217",
			"")

DLL_LOG_DEF(Cmd_C_Module)

//test
//LOG_OBJ_DECL(Cmd_C_DataError)
//test

fpnMCmd_C_Connect	g_fpnMCmd_C_Connect20160526;


void _CALLTYPE	gConnectResultBack20160526( fpnMCmd_C_Connect fpnConnect, 
										const _string_type& sIP,
										int u16Port,
										NETHANDLE Handle )
{
	if( fpnConnect != NULL )
	{
		fpnConnect(sIP.c_str(), u16Port, Handle);
	}
}

void _CALLTYPE	gCloseBack20160526( fpnMCmd_C_Close fpnClose, 
									const _string_type& sIP,
									int u16Port,
									NETHANDLE Handle )
{
	if( fpnClose != NULL )
	{
		fpnClose(sIP.c_str(), u16Port, Handle);
	}
}

/*****************************************************************
/*函数说明：	初始化客户端集
/*参数说明：	pCMD_ClientConnect: 回调函数地址
/*				
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_Init( int iThreadNum )
{
	//test
	//LOG_Print_SetLog(Cmd_C_Module, 1)
	//LOG_Print_SetLog(Cmd_C_DataError, 1)
	//test

	LOG_Print_Info(Cmd_C_Module,"MCmd_C_Init():")
	
	GetAsynCmdClientsInstance()->Init( iThreadNum );
	return 1;
}

/*****************************************************************
/*函数说明：	释放客户端模块资源(在不使用该模块时使用)
/*参数说明：	
/*				
/*返回值：		>1:成功  <1: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_Free( void )
{
	LOG_Print_Info(Cmd_C_Module,"MCmd_C_Free():")

	GetAsynCmdClientsInstance()->Release();
	return 1;
}

/*****************************************************************
/*函数说明：	设置连接响应回调函数
/*参数说明：	pCMD_ClientConnect: 回调函数地址
/*				
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_SetConnect( fpnMCmd_C_Connect fpnConnect )
{
	LOG_Print_Info(Cmd_C_Module,"MCmd_C_SetConnect():")

	g_fpnMCmd_C_Connect20160526 = fpnConnect;
	return 1;
}

/*****************************************************************
/*函数说明：	连接服务器
/*参数说明：	c_szIP: 服务器IP
				u16Port: 服务器端口号
/*				
/*返回值：		64bit连接句柄
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS NETHANDLE _CALLTYPE MCmd_C_Connect( const char* c_szIP,
																int u16Port )
{
	LOG_Print_Info(Cmd_C_Module,"MCmd_C_Connect():\n\tc_szIP:%s\n\tu16Port:%d",c_szIP, u16Port)

	return GetAsynCmdClientsInstance()->Connect(c_szIP, u16Port,
							function20_bind(gConnectResultBack20160526,
									g_fpnMCmd_C_Connect20160526, 
									_string_type(c_szIP), u16Port, _function_::_1));
}

/*****************************************************************
/*函数说明：	关闭连接
/*参数说明：	handle: 64bit连接句柄
/*				
/*返回值：
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_Close( NETHANDLE handle )
{
	LOG_Print_Info(Cmd_C_Module,"MCmd_C_Close():\n\thandle:%lld",handle)

	GetAsynCmdClientsInstance()->Close(handle);
	return 1;
}

/*****************************************************************
/*函数说明：	设置断开回调函数
/*参数说明：	handle: 64bit连接句柄
				pCMD_ClientRecData: 回调函数地址
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_SetClose( NETHANDLE handle,
															fpnMCmd_C_Close fpnClose )
{
	LOG_Print_Info(Cmd_C_Module,"MCmd_C_SetClose():\n\thandle:%lld",handle)

	return GetAsynCmdClientsInstance()->SetDestroyHandle(handle,
							function20_bind(gCloseBack20160526, fpnClose,
							GetIPv4ToString(handle),
							GetPort(handle),_function_::_1));
}


/*****************************************************************
/*函数说明：	设置接收回调函数
/*参数说明：	handle: 64bit连接句柄
				pCMD_ClientRecData: 回调函数地址
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_SetRecData( NETHANDLE handle,
															fpnMCmd_C_RecData fpnRecData )
{
	LOG_Print_Info(Cmd_C_Module,"MCmd_C_SetRecData():\n\thandle:%lld",handle)

	GetAsynCmdClientsInstance()->SetRecvHandle(handle, function20_bind_Ex(fpnRecData));
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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_SentData( NETHANDLE handle,
														const char* c_pData,
														int u32Size )
{
	LOG_Print_Info(Cmd_C_Module,"MCmd_C_SentData():\n\thandle:%lld\n\tSize:%d\n\tData:%s",
					handle, u32Size, c_pData)

	return GetAsynCmdClientsInstance()->Send(handle, c_pData, u32Size);
}

/*****************************************************************
/*函数说明：	地址错误发生
/*参数说明：	szData: 错误缓冲区
/*				
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MCmd_C_Error( const char* szData )
{
	/*LOG_Print_Info(Cmd_C_DataError,"MCmd_C_Error(): Data Address:%p", szData)
	LOG_Print_Info(Cmd_C_DataError,"MCmd_C_Error(): Data Content:%s", szData)*/
}