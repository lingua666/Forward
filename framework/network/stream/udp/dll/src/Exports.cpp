
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
/*����˵����	��ʼ���ͻ��˼�
/*����˵����	uIOThreadNum�� IO�߳���
/*			uProcThreadNum: ���ݴ����߳�
/*			isSequence:		�Ƿ�˳���ύ
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUdpStream_Init(UInt8 uIOThreadNum,
	UInt8 uProcThreadNum, bool isSequence)
{
	return GetUDPStreamInstance()->Init(uIOThreadNum, uProcThreadNum, isSequence);
}


/*****************************************************************
/*����˵����	�ͷſͻ���ģ����Դ(�ڲ�ʹ�ø�ģ��ʱʹ��)
/*����˵����
/*
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUdpStream_Free(void)
{
	GetUDPStreamInstance()->Release();
	return 1;
}

/*****************************************************************
/*����˵����	�ر�����
/*����˵����	Handle: 64bit���Ӿ��
/*			uPort: ���ض˿�
/*          c_szIP: ����IP��ַ�� NULL��ѡ��Ĭ��
/*
/*����ֵ��		Handle > 0: �ɹ�		<=0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS NETHANDLE _CALLTYPE MUdpStream_Open(UInt16 uPort, const char* c_szIP)
{
	return GetUDPStreamInstance()->Open(uPort, c_szIP);
}

/*****************************************************************
/*����˵����	�ر�����
/*����˵����	Handle: 64bit���Ӿ��
/*
/*����ֵ��		1: �ɹ�		<0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUdpStream_Close(NETHANDLE Handle)
{
	return GetUDPStreamInstance()->Close(Handle);
}


/*****************************************************************
/*����˵����	���öϿ��ص�����
/*����˵����	Handle: 64bit���Ӿ��
/*			fpnClose: �ص�������ַ
/*			pUser: �û�����
/*����ֵ��		1: �ɹ�		<0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUdpStream_SetClose(NETHANDLE Handle,
	fpnMUdpStream_Close fpnClose, void* pUser)
{
	return GetUDPStreamInstance()->SetDestroyHandle(Handle, function20_bind(g_UdpCloseBack20160526,
		fpnClose, pUser, GetPortFromNETNODE(Handle), _function_::_1));
}

/*****************************************************************
/*����˵����	���ý��ջص�����
/*����˵����	Handle: 64bit���Ӿ��
/* pCMD_ClientRecData: �ص�������ַ
/* pUser: �û�����
/* ����ֵ��		1: �ɹ�		<0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUdpStream_SetRecData(NETHANDLE Handle,
	fpnMUdpStream_RecData fpnRecData, void* pUser)
{
	return GetUDPStreamInstance()->SetRecvHandle(Handle, function20_bind(g_UdpStreamRecvBack201610040936,
		fpnRecData, pUser, _function_::_1, _function_::_2, _function_::_3));
}

/*****************************************************************
/*����˵����	��������
/*����˵����	Handle: 64bit���Ӿ��
/*			c_pData: ���ݻ�����
/*			u32Size: ���ݳ���
/*          pAddr: Ŀ���ַ
/*
/*����ֵ��	1: �ɹ�		<0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUdpStream_SentData(NETHANDLE Handle,
	struct sockaddr_in* pAddr, const char* c_pData, UInt32 u32Size)
{
	return GetUDPStreamInstance()->Send(Handle, pAddr, c_pData, u32Size);
}


/*****************************************************************
/*����˵����	��������
/*����˵����	Handle: 64bit���Ӿ��
/*			c_pData: ���ݻ�����
/*			u32Size: ���ݳ���
/*			c_szIP: Ŀ��IP
/*			uPort: Ŀ��˿�
/*
/*����ֵ��	1: �ɹ�		<0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUdpStream_SentData2(NETHANDLE Handle,
	const char* c_szIP, UInt16 uPort, const char* c_pData, UInt32 u32Size)
{
	return GetUDPStreamInstance()->Send(Handle, c_szIP, uPort, c_pData, u32Size);
}

