
#include <dllNetFrame_UDP_Stream/Exports.h>
#include <libNetFrame_UDP_Stream/AppMain.h>

//typedef _session_::_udp_::StreamBuf_ptr	Udp_StreamBuf_ptr;

DLL_INIT_DEF(g_pTagTModuleInfoRecord_201602172200,
	"45000001000V2016021700000000VZZZ00000000",
	"ϵͳͨ��ָ��ģ��",
	"V1.0.0.1",
	"20160217",
	"")

DLL_LOG_DEF(NetFrame_UDP_Stream_Module)

NETHANDLE g_Node201801191632 = NULL;

struct sockaddr_in g_DestAddr201801191632 = {0};


/*****************************************************************
/*����˵����	��ʼ��
/*����˵����	LocalPort: ���ض˿�
/*				c_szDestIP: �㲥ϵͳ���ip��ַ
/*				DestPort: �㲥ϵͳ����˿ں�
/*����ֵ��		1:�ɹ�  <0:������
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
/*����˵����	�ͷ���Դ
/*����˵����	
/*				
/*����ֵ��		1:�ɹ�  <0:������
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
/*����˵����	������������
/*����˵����	
/*				
/*����ֵ��		1:�ɹ�  <0:������
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
/*����˵����	�Ҷ���������
/*����˵����	
/*				
/*����ֵ��		1:�ɹ�  <0:������
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
