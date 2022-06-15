
#include <dllNetFrame_UDP_Stream/Exports.h>
#include <libNetFrame_UDP_Stream/AppMain.h>

//typedef _session_::_udp_::StreamBuf_ptr	Udp_StreamBuf_ptr;

DLL_INIT_DEF(g_pTagTModuleInfoRecord_201602172200,
	"45000001000V2016021700000000VZZZ00000000",
	"系统通信指令模块",
	"V1.0.0.1",
	"20160217",
	"")

DLL_LOG_DEF(NetFrame_UDP_Stream_Module)

NETHANDLE g_Node201801191632 = NULL;

struct sockaddr_in g_DestAddr201801191632 = {0};


/*****************************************************************
/*函数说明：	初始化
/*参数说明：	LocalPort: 本地端口
/*				c_szDestIP: 广播系统软件ip地址
/*				DestPort: 广播系统软件端口号
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MEmergency_Telephone_Init( int LocalPort,
									const char* c_szDestIP, int iDestPort )
{
	GetUDPStreamInstance()->Init(1, 1);
	g_Node201801191632 = GetUDPStreamInstance()->Open(LocalPort);
	if( g_Node201801191632 == -1 )
		return -1;

	g_DestAddr201801191632.sin_port			= htons( iDestPort );
	g_DestAddr201801191632.sin_addr.s_addr	= inet_addr( c_szDestIP );
	g_DestAddr201801191632.sin_family			= AF_INET;
	return 1;
}


/*****************************************************************
/*函数说明：	释放资源
/*参数说明：	
/*				
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MEmergency_Telephone_Free( void )
{
	if( g_Node201801191632 != -1 )
	{
		GetUDPStreamInstance()->Close(g_Node201801191632);
		g_Node201801191632 = -1;
	}

	GetUDPStreamInstance()->Release();
	return 1;
}


/*****************************************************************
/*函数说明：	呼叫所有喇叭
/*参数说明：	
/*				
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MEmergency_Telephone_Call( void )
{
	if( g_Node201801191632 != -1 )
	{
		const UInt8 uData[] = {0xBB,0xBB,0x0C,0x10,0x1F,0x01,0xF1,0x01};
		return GetUDPStreamInstance()->Send(g_Node201801191632, &g_DestAddr201801191632, (char*)uData, sizeof(uData));
	}

	return -1;
}


/*****************************************************************
/*函数说明：	挂断所有喇叭
/*参数说明：	
/*				
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MEmergency_Telephone_Hangup( void )
{
	if( g_Node201801191632 != -1 )
	{
		const UInt8 uData[] = {0xBB,0xBB,0x0C,0x10,0x1F,0x01,0xF1,0x02};
		return GetUDPStreamInstance()->Send(g_Node201801191632, &g_DestAddr201801191632, (char*)uData, sizeof(uData));
	}

	return -1;
}

void _CALLTYPE	g_UdpStreamRecvBack201610040936(fpnMUdpStream_RecData fnRecv,
	void* pUser, NETHANDLE Handle, struct sockaddr_in* pSrc, const _io_net_::StreamBuf_ptr& BStream)
{
	if (fnRecv != NULL)
	{
		fnRecv(pUser, Handle, pSrc, BStream->data, BStream->payload);
	}
}

void _CALLTYPE	g_UdpCloseBack20160526(fpnMUdpStream_Close fpnClose,
	void* pUser, UInt16 uPort, NETHANDLE Handle)
{
	if (fpnClose != NULL)
	{
		fpnClose(pUser, uPort, Handle);
	}
}

/*****************************************************************
/*函数说明：	初始化客户端集
/*参数说明：	uIOThreadNum： IO线程数
/*			uProcThreadNum: 数据处理线程
/*			isSequence:		是否顺序提交
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUdpStream_Init(UInt8 uIOThreadNum,
	UInt8 uProcThreadNum, bool isSequence)
{
	return GetUDPStreamInstance()->Init(uIOThreadNum, uProcThreadNum, isSequence);
}


/*****************************************************************
/*函数说明：	释放客户端模块资源(在不使用该模块时使用)
/*参数说明：
/*
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUdpStream_Free(void)
{
	GetUDPStreamInstance()->Release();
	return 1;
}

/*****************************************************************
/*函数说明：	关闭连接
/*参数说明：	Handle: 64bit连接句柄
/*			uPort: 本地端口
/*          c_szIP: 本地IP地址， NULL则选择默认
/*
/*返回值：		Handle > 0: 成功		<=0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS NETHANDLE _CALLTYPE MUdpStream_Open(UInt16 uPort, const char* c_szIP)
{
	return GetUDPStreamInstance()->Open(uPort, c_szIP);
}

/*****************************************************************
/*函数说明：	关闭连接
/*参数说明：	Handle: 64bit连接句柄
/*
/*返回值：		1: 成功		<0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUdpStream_Close(NETHANDLE Handle)
{
	return GetUDPStreamInstance()->Close(Handle);
}


/*****************************************************************
/*函数说明：	设置断开回调函数
/*参数说明：	Handle: 64bit连接句柄
/*			fpnClose: 回调函数地址
/*			pUser: 用户参数
/*返回值：		1: 成功		<0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUdpStream_SetClose(NETHANDLE Handle,
	fpnMUdpStream_Close fpnClose, void* pUser)
{
	return GetUDPStreamInstance()->SetDestroyHandle(Handle, function20_bind(g_UdpCloseBack20160526,
		fpnClose, pUser, GetPortFromNETNODE(Handle), _function_::_1));
}

/*****************************************************************
/*函数说明：	设置接收回调函数
/*参数说明：	Handle: 64bit连接句柄
/* pCMD_ClientRecData: 回调函数地址
/* pUser: 用户参数
/* 返回值：		1: 成功		<0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUdpStream_SetRecData(NETHANDLE Handle,
	fpnMUdpStream_RecData fpnRecData, void* pUser)
{
	return GetUDPStreamInstance()->SetRecvHandle(Handle, function20_bind(g_UdpStreamRecvBack201610040936,
		fpnRecData, pUser, _function_::_1, _function_::_2, _function_::_3));
}

/*****************************************************************
/*函数说明：	发送数据
/*参数说明：	Handle: 64bit连接句柄
/*			c_pData: 数据缓冲区
/*			u32Size: 数据长度
/*          pAddr: 目标地址
/*
/*返回值：	1: 成功		<0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUdpStream_SentData(NETHANDLE Handle,
	struct sockaddr_in* pAddr, const char* c_pData, UInt32 u32Size)
{
	return GetUDPStreamInstance()->Send(Handle, pAddr, c_pData, u32Size);
}


/*****************************************************************
/*函数说明：	发送数据
/*参数说明：	Handle: 64bit连接句柄
/*			c_pData: 数据缓冲区
/*			u32Size: 数据长度
/*			c_szIP: 目标IP
/*			uPort: 目标端口
/*
/*返回值：	1: 成功		<0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUdpStream_SentData2(NETHANDLE Handle,
	const char* c_szIP, UInt16 uPort, const char* c_pData, UInt32 u32Size)
{
	return GetUDPStreamInstance()->Send(Handle, c_szIP, uPort, c_pData, u32Size);
}

