
#include <dllNetFrame_AsyncStreamClients/Exports.h>
#include <libNetFrame_AsyncStreamClients/StreamClients.h>

typedef _session_::_tcp_::StreamBuf_ptr	StreamBuf_ptr;	

DLL_INIT_DEF(g_pTagTModuleInfoRecord_201602172205,
			"45000001000V2016021700000000VZZZ10000000",
			"ϵͳͨ��ָ��ģ��",
			"V1.0.0.1",
			"20160217",
			"")

DLL_LOG_DEF(Stream_C_Module)

static fpnMStream_C_Connect	g_fpnMStream_C_Connect20160526;


void _CALLTYPE	g_StreamRecvBack201610040936( fpnMStream_C_RecData fnRecv,
                                             NETHANDLE handle,
											 const StreamBuf_ptr& BStream,
											 long lRemain )
{
	//GetAsynStreamClientsInstance()->Send(handle, BStream->data, BStream->payload);

	if( fnRecv != NULL )
	{
		fnRecv(handle, BStream->data, BStream->payload);
	}
}

void _CALLTYPE	g_ConnectResultBack20160526( fpnMStream_C_Connect fpnConnect, 
										const _string_type& sIP,
										UInt16 u16Port,
										NETHANDLE Handle )
{
	if( fpnConnect != NULL )
	{
		fpnConnect(sIP.c_str(), u16Port, Handle);
	}
}

void _CALLTYPE	g_CloseBack20160526( fpnMStream_C_Close fpnClose, 
									const _string_type& sIP,
									UInt16 u16Port,
									NETHANDLE Handle )
{
	if( fpnClose != NULL )
	{
		fpnClose(sIP.c_str(), u16Port, Handle);
	}
}

/*****************************************************************
/*����˵����	��ʼ���ͻ��˼�
/*����˵����	isSequence: �Ƿ�����ģʽ�ύ���յ�������
/*				
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_Init( bool isSequence )
{
	LOG_Print_Info(Stream_C_Module,"MStream_C_Init():")

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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_Free( void )
{
	LOG_Print_Info(Stream_C_Module,"MStream_C_Free():")

	GetAsynStreamClientsInstance()->Release();
	return 1;
}

/*****************************************************************
/*����˵����	����������Ӧ�ص�����
/*����˵����	pCMD_ClientConnect: �ص�������ַ
/*				
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_SetConnect( fpnMStream_C_Connect fpnConnect )
{
	LOG_Print_Info(Stream_C_Module,"MStream_C_SetConnect():")

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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_Connect( const char* c_szIP,
																UInt16 u16Port )
{
	LOG_Print_Info(Stream_C_Module,"MStream_C_Connect():\n\tc_szIP:%s\n\tu16Port:%d",c_szIP, u16Port)

	return GetAsynStreamClientsInstance()->Connect(c_szIP, u16Port,
							function20_bind(g_ConnectResultBack20160526,
									g_fpnMStream_C_Connect20160526, 
									_string_type(c_szIP), u16Port, _function_::_1));
}

/*****************************************************************
/*����˵����	�ر�����
/*����˵����	handle: 64bit���Ӿ��
/*				
/*����ֵ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_Close( NETHANDLE handle )
{
	LOG_Print_Info(Stream_C_Module,"MStream_C_Close():\n\thandle:%lld",handle)

	GetAsynStreamClientsInstance()->Close(handle);
	return 1;
}

/*****************************************************************
/*����˵����	���öϿ��ص�����
/*����˵����	handle: 64bit���Ӿ��
				pCMD_ClientRecData: �ص�������ַ
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_SetClose( NETHANDLE handle,
															fpnMStream_C_Close fpnClose )
{
	LOG_Print_Info(Stream_C_Module,"MStream_C_SetClose():\n\thandle:%lld",handle)

	/*GetAsynCmdClientsInstance()->SetCloseHandle(handle,
							function20_bind(fpnClose, GetIPv4ToString(handle),
							GetPort(handle),_function_::_1));*/

	return GetAsynStreamClientsInstance()->SetDestroyHandle(handle,
							function20_bind(g_CloseBack20160526, fpnClose,
							GetIPv4ToString(handle),
							GetPort(handle),_function_::_1));
}


/*****************************************************************
/*����˵����	���ý��ջص�����
/*����˵����	handle: 64bit���Ӿ��
				pCMD_ClientRecData: �ص�������ַ
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_SetRecData( NETHANDLE handle,
															fpnMStream_C_RecData fpnRecData )
{
	LOG_Print_Info(Stream_C_Module,"MStream_C_SetRecData():\n\thandle:%lld",handle)

	GetAsynStreamClientsInstance()->SetRecvHandle(handle, function20_bind(g_StreamRecvBack201610040936, 
																		fpnRecData,
																		_function_::_1,
																		_function_::_2,
																		_function_::_3));
	return 1;
}

/*****************************************************************
/*����˵����	��������
/*����˵����	handle: 64bit���Ӿ��
				c_pData: ���ݻ�����
				u32Size: ���ݳ���
/*				
/*����ֵ��		>0:�ɹ�		<=0:ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_SentData( NETHANDLE handle,
														const char* c_pData,
														UInt32 u32Size )
{
	LOG_Print_Info(Stream_C_Module,"MStream_C_SentData():\n\thandle:%lld\n\tSize:%d\n\tData:%s",
					handle, u32Size, c_pData)

	return GetAsynStreamClientsInstance()->Send(handle, c_pData, u32Size);
}