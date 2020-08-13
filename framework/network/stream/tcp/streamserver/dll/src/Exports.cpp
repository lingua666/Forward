
#include <dllNetFrame_StreamServer/Exports.h>
#include <libNetFrame_StreamServer/StreamServer.h>

typedef _session_::_tcp_::StreamBuf_ptr	StreamBuf_ptr;

DLL_INIT_DEF(g_pTagTModuleInfoRecord_201602172200,
	"45000001000V2016021700000000VZZZ00000000",
	"ϵͳͨ��ָ��ģ��",
	"V1.0.0.1",
	"20160217",
	"")

DLL_LOG_DEF(Stream_S_Module)

void _CALLTYPE	g_StreamRecv201610040936( pfnStream_S_ClientRecData fnRecv,
                                             NETHANDLE handle,
											 const StreamBuf_ptr& BStream,
											 long lRemain )
{
	if( fnRecv != NULL )
	{
		fnRecv(handle, BStream->data, BStream->payload);
	}
}

void _CALLTYPE	g_StreamSvrClientClose201609141640( pfnStream_S_ClientClose pClientClose,
                                                 const _string_type& sIP,
												 UInt16 uPort,
												 NETHANDLE Handle )
{
	if( pClientClose != NULL )
	{
		pClientClose(sIP.c_str(), uPort, Handle);
	}
}

/*****************************************************************
/*����˵����	��ʼ��
/*����˵����	uWorkerThreads: ���ݴ����̸߳���
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_Init( UInt8 uWorkerThreads )
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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_Listen( UInt16 u16Port,
											pfnStream_S_Accept pfnAccept )
{
	if( GetStreamSvrInstance()->Listen(u16Port, function20_bind_Ex(pfnAccept)) )
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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_Close( void )
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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_Free( void )
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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_DisConnectClient( NETHANDLE handle )
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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_SetClientDisConnect( NETHANDLE handle,
													pfnStream_S_ClientClose pClientClose )
{
	/*GetCommandServer_V10Instance()->SetCloseHandle(handle,
						function20_bind(pClientClose, GetIPv4ToString(handle).c_str(),
									GetPort(handle), _function_::_1));*/

	return GetStreamSvrInstance()->SetDestroyHandle(handle,
		function20_bind(g_StreamSvrClientClose201609141640, pClientClose,
		GetIPv4ToString(handle),
		GetPort(handle), _function_::_1));
}

/*****************************************************************
/*����˵����	���ÿͻ��˽��ջص�����
/*����˵����	handle: 64bit�ͻ��˾��
				pCMD_ClientRecData: �ص�������ַ
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_SetClientRecData( NETHANDLE handle,
												pfnStream_S_ClientRecData pClientRecData )
{
	GetStreamSvrInstance()->SetRecvHandle(handle, 
										function20_bind(g_StreamRecv201610040936,
													pClientRecData,
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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_SentData( NETHANDLE handle,
															const char* c_pData,
															UInt32 u32Size )
{
	return GetStreamSvrInstance()->Send(handle, c_pData, u32Size);
}
