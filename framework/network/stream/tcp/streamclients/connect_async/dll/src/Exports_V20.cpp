
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
/*����˵����	��ʼ���ͻ��˼�
/*����˵����	isSequence: �Ƿ�����ģʽ�ύ���յ�������
/*				
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_V20_Init( bool isSequence )
{
	LOG_Print_Info(Stream_C_V20_Module,"MStream_C_V20_Init():")

	int iThreadNum = get_processor_number();
	GetAsynStreamClientsInstance()->Init( __max(iThreadNum / 2, 2), __max(iThreadNum / 2, 2), isSequence );
	return 1;
}

/*****************************************************************
/*����˵����	�ͷſͻ���ģ����Դ(�ڲ�ʹ�ø�ģ��ʱʹ��)
/*����˵����	
/*				
/*����ֵ��		>1:�ɹ�  <1: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_V20_Free( void )
{
	LOG_Print_Info(Stream_C_V20_Module,"MStream_C_V20_Free():")

	GetAsynStreamClientsInstance()->Release();
	return 1;
}

/*****************************************************************
/*����˵����	����������Ӧ�ص�����
/*����˵����	pCMD_ClientConnect: �ص�������ַ
/*				
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_V20_SetConnect( fpnMStream_C_V20_Connect fpnConnect )
{
	LOG_Print_Info(Stream_C_V20_Module,"MStream_C_V20_SetConnect():")

	g_fpnMStream_C_Connect20160526 = fpnConnect;
	return 1;
}

/*****************************************************************
/*����˵����	���ӷ�����
/*����˵����	c_szIP: ������IP
				u16Port: �������˿ں�
/*				
/*����ֵ��		64bit���Ӿ��
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
/*����˵����	�ر�����
/*����˵����	Handle: 64bit���Ӿ��
/*				
/*����ֵ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_V20_Close( NETHANDLE Handle)
{
	LOG_Print_Info(Stream_C_V20_Module,"MStream_C_V20_Close():\n\tHandle:%lld", Handle)

	GetAsynStreamClientsInstance()->Close(Handle);
	return 1;
}

/*****************************************************************
/*����˵����	���öϿ��ص�����
/*����˵����	Handle: 64bit���Ӿ��
				pCMD_ClientRecData: �ص�������ַ
/*����ֵ��		
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
/*����˵����	���ý��ջص�����
/*����˵����	Handle: 64bit���Ӿ��
				pCMD_ClientRecData: �ص�������ַ
/*����ֵ��		
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
/*����˵����	��������
/*����˵����	Handle: 64bit���Ӿ��
				c_pData: ���ݻ�����
				u32Size: ���ݳ���
/*				
/*����ֵ��		>0:�ɹ�		<=0:ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_V20_SentData( NETHANDLE Handle,
														const char* c_pData,
														UInt32 u32Size )
{
	LOG_Print_Info(Stream_C_V20_Module,"MStream_C_V20_SentData():\n\tHandle:%lld\n\tSize:%d\n\tData:%s",
		Handle, u32Size, c_pData)

	return GetAsynStreamClientsInstance()->Send(Handle, c_pData, u32Size);
}