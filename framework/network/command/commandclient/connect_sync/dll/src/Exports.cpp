
#include <dllNetFrame_SyncCmdClients/Exports.h>
#include <libNetFrame_SyncCmdClients/CommandClients.h>

DLL_INIT_DEF(g_pTagTModuleInfoRecord_201602172205,
			"45000001000V2016021700000000VZZZ00000000",
			"系统通信指令模块",
			"V1.0.0.1",
			"20160217",
			"")


			
void _CALLTYPE g_Cmd_SynC_Close201609141643( fpnMCmd_SynC_Close fpnClose,
											const _string_type& sIP,
											UInt16 uPort,
											NETHANDLE Handle )
{
	if( fpnClose != NULL )
	{
		fpnClose(sIP.c_str(), uPort, Handle);
	}
}

/*****************************************************************
/*函数说明：	初始化客户端集
/*参数说明：	CMD_Clients_Init
/*				
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SynC_Init( void )
{
	Singleton<syncCommandClients_type>::instance()->Init(get_processor_number() * 2 + 2);
	return 1;
}

/*****************************************************************
/*函数说明：	释放客户端模块资源(在不使用该模块时使用)
/*参数说明：	
/*				
/*返回值：		>1:成功  <1: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SynC_Free( void )
{
	Singleton<syncCommandClients_type>::instance()->Release();
	return 1;
}

/*****************************************************************
/*函数说明：	连接服务器
/*参数说明：	c_szIP: 服务器IP
				u16Port: 服务器端口号
/*				
/*返回值：		64bit连接句柄
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS NETHANDLE _CALLTYPE MCmd_SynC_Connect( const char* c_szIP,
																	UInt16 u16Port )
{
	return Singleton<syncCommandClients_type>::instance()->Connect(c_szIP, u16Port);
}

/*****************************************************************
/*函数说明：	关闭连接
/*参数说明：	handle: 64bit连接句柄
/*				
/*返回值：
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SynC_Close( NETHANDLE handle )
{
	Singleton<syncCommandClients_type>::instance()->Close(handle);
	return 1;
}

/*****************************************************************
/*函数说明：	设置断开回调函数
/*参数说明：	handle: 64bit连接句柄
				pCMD_ClientRecData: 回调函数地址
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SynC_SetClose( NETHANDLE handle,
															fpnMCmd_SynC_Close fpnClose )
{
	Singleton<syncCommandClients_type>::instance()->SetCloseHandle(handle, 
																function20_bind(g_Cmd_SynC_Close201609141643, fpnClose,
																GetIPv4ToString(handle),
																GetPort(handle), _function_::_1));
	return 1;
}


/*****************************************************************
/*函数说明：	设置接收回调函数
/*参数说明：	handle: 64bit连接句柄
				pCMD_ClientRecData: 回调函数地址
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SynC_SetRecData( NETHANDLE handle,
																fpnMCmd_SynC_RecData fpnRecData )
{
	Singleton<syncCommandClients_type>::instance()->SetRecvHandle(handle, function20_bind_Ex(fpnRecData));
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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SynC_SentData( NETHANDLE handle,
															const char* c_pData,
															UInt32 u32Size )
{
	return Singleton<syncCommandClients_type>::instance()->Send(handle, c_pData, u32Size);
}