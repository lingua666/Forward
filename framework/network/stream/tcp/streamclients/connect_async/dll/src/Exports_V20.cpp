
#include <dllNetFrame_AsyncStreamClients/Exports_V20.h>
#include <libNetFrame_AsyncStreamClients/StreamClients.h>

typedef _session_::_tcp_::StreamBuf_ptr	StreamBuf_ptr;	

LOG_OBJ_DECL(Stream_C_V20_Module)

static fpnMStream_C_V20_Connect	g_fpnMStream_C_Connect20160526;


void _CALLTYPE	g_StreamRecvBack201610040936_V20( fpnMStream_C_V20_RecData fnRecv,
											 void* pUser,
                                             NETHANDLE Handle,
											 const StreamBuf_ptr& BStream,
											 long lRemain )
{
	//GetAsynStreamClientsInstance()->Send(Handle, BStream->data, BStream->payload);

	if( fnRecv != NULL )
	{
		fnRecv(pUser, Handle, BStream->data, BStream->payload);
	}
}

void _CALLTYPE	g_ConnectResultBack20160526_V20( fpnMStream_C_V20_Connect fpnConnect, 
										void* pUser,
										const _string_type& sIP,
										UInt16 u16Port,
										NETHANDLE Handle)
{
	if( fpnConnect != NULL )
	{
		fpnConnect(pUser, sIP.c_str(), u16Port, Handle);
	}
}

void _CALLTYPE	g_CloseBack20160526_V20( fpnMStream_C_V20_Close fpnClose, 
									void* pUser,
									const _string_type& sIP,
									UInt16 u16Port,
									NETHANDLE Handle )
{
	if( fpnClose != NULL )
	{
		fpnClose(pUser, sIP.c_str(), u16Port, Handle);
	}
}

/*****************************************************************
/*函数说明：	初始化客户端集
/*参数说明：	isSequence: 是否序列模式提交接收到的数据
/*				
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_V20_Init( bool isSequence )
{
	LOG_Print_Info(Stream_C_V20_Module,"MStream_C_V20_Init():")

	int iThreadNum = get_processor_number();
	GetAsynStreamClientsInstance()->Init( __max(iThreadNum / 2, 2), __max(iThreadNum / 2, 2), isSequence );
	return 1;
}

/*****************************************************************
/*函数说明：	释放客户端模块资源(在不使用该模块时使用)
/*参数说明：	
/*				
/*返回值：		>1:成功  <1: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_V20_Free( void )
{
	LOG_Print_Info(Stream_C_V20_Module,"MStream_C_V20_Free():")

	GetAsynStreamClientsInstance()->Release();
	return 1;
}

/*****************************************************************
/*函数说明：	设置连接响应回调函数
/*参数说明：	pCMD_ClientConnect: 回调函数地址
/*				
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_V20_SetConnect( fpnMStream_C_V20_Connect fpnConnect )
{
	LOG_Print_Info(Stream_C_V20_Module,"MStream_C_V20_SetConnect():")

	g_fpnMStream_C_Connect20160526 = fpnConnect;
	return 1;
}

/*****************************************************************
/*函数说明：	连接服务器
/*参数说明：	c_szIP: 服务器IP
				u16Port: 服务器端口号
/*				
/*返回值：		64bit连接句柄
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_V20_Connect( const char* c_szIP,
																UInt16 u16Port, void* pUser )
{
	LOG_Print_Info(Stream_C_V20_Module,"MStream_C_V20_Connect():\n\tc_szIP:%s\n\tu16Port:%d",c_szIP, u16Port)

	return GetAsynStreamClientsInstance()->Connect(c_szIP, u16Port,
							function20_bind(g_ConnectResultBack20160526_V20,
									g_fpnMStream_C_Connect20160526, pUser,
									_string_type(c_szIP), u16Port, _function_::_1), -1, -1);
}

/*****************************************************************
/*函数说明：	关闭连接
/*参数说明：	Handle: 64bit连接句柄
/*				
/*返回值：
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_V20_Close( NETHANDLE Handle)
{
	LOG_Print_Info(Stream_C_V20_Module,"MStream_C_V20_Close():\n\tHandle:%lld", Handle)

	GetAsynStreamClientsInstance()->Close(Handle);
	return 1;
}

/*****************************************************************
/*函数说明：	设置断开回调函数
/*参数说明：	Handle: 64bit连接句柄
				pCMD_ClientRecData: 回调函数地址
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_V20_SetClose( NETHANDLE Handle,
															fpnMStream_C_V20_Close fpnClose,
															void* pUser )
{
	LOG_Print_Info(Stream_C_V20_Module,"MStream_C_V20_SetClose():\n\tHandle:%lld", Handle)

	/*GetAsynCmdClientsInstance()->SetCloseHandle(Handle,
							function20_bind(fpnClose, GetIPv4ToString(Handle),
							GetPort(Handle),_function_::_1));*/

	return GetAsynStreamClientsInstance()->SetDestroyHandle(Handle,
							function20_bind(g_CloseBack20160526_V20, fpnClose,
							pUser, GetIPv4SFromNETNODE(Handle),
							GetPortFromNETNODE(Handle),_function_::_1));
}


/*****************************************************************
/*函数说明：	设置接收回调函数
/*参数说明：	Handle: 64bit连接句柄
				pCMD_ClientRecData: 回调函数地址
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_V20_SetRecData( NETHANDLE Handle,
															fpnMStream_C_V20_RecData fpnRecData,
															void* pUser )
{
	LOG_Print_Info(Stream_C_V20_Module,"MStream_C_V20_SetRecData():\n\tHandle:%lld", Handle)

	GetAsynStreamClientsInstance()->SetRecvHandle(Handle, function20_bind(g_StreamRecvBack201610040936_V20,
																		fpnRecData, pUser,
																		_function_::_1,
																		_function_::_2,
																		_function_::_3));
	return 1;
}

/*****************************************************************
/*函数说明：	发送数据
/*参数说明：	Handle: 64bit连接句柄
				c_pData: 数据缓冲区
				u32Size: 数据长度
/*				
/*返回值：		>0:成功		<=0:失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_V20_SentData( NETHANDLE Handle,
														const char* c_pData,
														UInt32 u32Size )
{
	LOG_Print_Info(Stream_C_V20_Module,"MStream_C_V20_SentData():\n\tHandle:%lld\n\tSize:%d\n\tData:%s",
		Handle, u32Size, c_pData)

	return GetAsynStreamClientsInstance()->Send(Handle, c_pData, u32Size);
}