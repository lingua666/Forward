
#include <dllNetFrame_StreamServer/Exports_V20.h>
#include <libNetFrame_StreamServer/StreamServer.h>

typedef _session_::_tcp_::StreamBuf_ptr	StreamBuf_ptr;

LOG_OBJ_DECL(Stream_S_V20_Module)

void _CALLTYPE	g_StreamRecv201610040936_V20( pfnStream_S_V20_ClientRecData fnRecv,
											 void* pUser,
                                             NETHANDLE handle,
											 const StreamBuf_ptr& BStream,
											 long lRemain )
{
	if( fnRecv != NULL )
	{
		fnRecv(pUser, handle, BStream->data, BStream->payload);
	}
}

void _CALLTYPE	g_StreamSvrClientClose201609141640_V20( pfnStream_S_V20_ClientClose pClientClose,
												 void* pUser,
                                                 const _string_type& sIP,
												 UInt16 uPort,
												 NETHANDLE Handle )
{
	if( pClientClose != NULL )
	{
		pClientClose(pUser, sIP.c_str(), uPort, Handle);
	}
}

/*****************************************************************
/*����˵����	��ʼ��
/*����˵����	uWorkerThreads: ���ݴ����̸߳���
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_V20_Init( UInt8 uWorkerThreads )
{
	UInt8 uNum = (uWorkerThreads == 0 ? 1 : uWorkerThreads);
	GetStreamSvrInstance()->Init(uNum);
	return 1;
}

/*****************************************************************
/*����˵����	�����˿�
/*����˵����	u16Port: �˿ں�
				pfnStream_S_Accept: �ͻ������ӻص�����
/*				
/*����ֵ��		>0:�ɹ�  <1: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_V20_Listen( UInt16 u16Port,
											pfnStream_S_V20_Accept pfnAccept,
											void* pUser )
{
	if( GetStreamSvrInstance()->Listen(u16Port,
		function20_bind(pfnAccept, pUser, _function_::_1, _function_::_2, _function_::_3)) )
	{
		int iThreadNum = get_processor_number();
		GetStreamSvrInstance()->Run( __max(iThreadNum / 2, 2), __max(iThreadNum / 2, 2) );
		return 1;
	}
	return -1;
}

/*****************************************************************
/*����˵����	�رշ�����
/*����˵����	
/*				
/*����ֵ��		>0:�ɹ�  <1: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_V20_Close( void )
{
	GetStreamSvrInstance()->Stop();
	return 1;
}


/*****************************************************************
/*����˵����	�ͷŷ�������Դ(�����ڵ���CMD_Server_MClose��ִ�иýӿ�)
/*����˵����	
/*				
/*����ֵ��		>0:�ɹ�  <1: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_V20_Free( void )
{
	GetStreamSvrInstance()->Release();
	return 1;
}


/*****************************************************************
/*����˵����	�رտͻ�������
/*����˵����	handle: 64bit�ͻ��˾��
/*				
/*����ֵ��		>0:�ɹ�  <1: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_V20_DisConnectClient( NETHANDLE handle )
{
	GetStreamSvrInstance()->Close(handle);
	return 1;
}


/*****************************************************************
/*����˵����	���ÿͻ��˶Ͽ��ص�����
/*����˵����	handle: 64bit�ͻ��˾��
				pCMD_ClientClose: �ص�������ַ
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_V20_SetClientDisConnect( NETHANDLE handle,
													pfnStream_S_V20_ClientClose pClientClose,
													void* pUser )
{
	/*GetCommandServer_V10Instance()->SetCloseHandle(handle,
						function20_bind(pClientClose, GetIPv4ToString(handle).c_str(),
									GetPort(handle), _function_::_1));*/

	return GetStreamSvrInstance()->SetDestroyHandle(handle,
		function20_bind(g_StreamSvrClientClose201609141640_V20, pClientClose,
		pUser, GetIPv4ToString(handle),
		GetPort(handle), _function_::_1));
}

/*****************************************************************
/*����˵����	���ÿͻ��˽��ջص�����
/*����˵����	handle: 64bit�ͻ��˾��
				pCMD_ClientRecData: �ص�������ַ
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_V20_SetClientRecData( NETHANDLE handle,
												pfnStream_S_V20_ClientRecData pClientRecData,
												void* pUser )
{
	GetStreamSvrInstance()->SetRecvHandle(handle, 
										function20_bind(g_StreamRecv201610040936_V20,
													pClientRecData, pUser,
													_function_::_1,
													_function_::_2,
													_function_::_3));
	return 1;
}

/*****************************************************************
/*����˵����	��ͻ��˷�������
/*����˵����	handle: 64bit�ͻ��˾��
				c_pData: ���ݻ�����
				u32Size: ���ݳ���
/*				
/*����ֵ��		>0:�ɹ�		<=0:ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_V20_SentData( NETHANDLE handle,
															const char* c_pData,
															UInt32 u32Size )
{
	return GetStreamSvrInstance()->Send(handle, c_pData, u32Size);
}
