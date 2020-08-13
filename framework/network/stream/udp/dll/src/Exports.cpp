
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
