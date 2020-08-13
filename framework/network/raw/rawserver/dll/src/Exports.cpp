
#include <dllNetFrame_RawServer/Exports.h>
#include <libNetFrame_RawServer/RawServer.h>

DLL_INIT_DEF(g_pTagTModuleInfoRecord_201602172200,
	"45000001000V2016021700000000VZZZ00000000",
	"ϵͳͨ��ָ��ģ��",
	"V1.0.0.1",
	"20160217",
	"")

DLL_LOG_DEF(Raw_S_Module)

void _CALLTYPE	g_RawSvrClientClose201609141640( pfnRaw_S_ClientClose pClientClose,
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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRaw_S_Init( UInt8 uWorkerThreads )
{
	/*UInt8 uNum = (uWorkerThreads == 0 ? 1 : uWorkerThreads);
	GetStreamSvrInstance()->Init(uNum);*/
	return 1;
}

/*****************************************************************
/*����˵����	�����˿�
/*����˵����	u16Port: �˿ں�
				pfnRaw_S_Accept: �ͻ������ӻص�����
/*				
/*����ֵ��		>0:�ɹ�  <1: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRaw_S_Listen( UInt16 u16Port,
											pfnRaw_S_Accept pfnAccept )
{
	if( GetRawSvrInstance()->Listen(u16Port, function20_bind_Ex(pfnAccept)) )
	{
		GetRawSvrInstance()->Run( get_processor_number() * 2 + 2 );
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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRaw_S_Close( void )
{
	GetRawSvrInstance()->Stop();
	return 1;
}


/*****************************************************************
/*����˵����	�ͷŷ�������Դ(�����ڵ���CMD_Server_MClose��ִ�иýӿ�)
/*����˵����	
/*				
/*����ֵ��		>0:�ɹ�  <1: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRaw_S_Free( void )
{
	GetRawSvrInstance()->Release();
	return 1;
}


/*****************************************************************
/*����˵����	�رտͻ�������
/*����˵����	handle: 64bit�ͻ��˾��
/*				
/*����ֵ��		>0:�ɹ�  <1: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRaw_S_DisConnectClient( NETHANDLE handle )
{
	GetRawSvrInstance()->Close(handle);
	return 1;
}


/*****************************************************************
/*����˵����	���ÿͻ��˶Ͽ��ص�����
/*����˵����	handle: 64bit�ͻ��˾��
				pCMD_ClientClose: �ص�������ַ
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRaw_S_SetClientDisConnect( NETHANDLE handle,
													pfnRaw_S_ClientClose pClientClose )
{
	return GetRawSvrInstance()->SetDestroyHandle(handle,
		function20_bind(g_RawSvrClientClose201609141640, pClientClose,
		GetIPv4ToString(handle),
		GetPort(handle), _function_::_1));
}

/*****************************************************************
/*����˵����	���ÿͻ��˽��ջص�����
/*����˵����	handle: 64bit�ͻ��˾��
				pCMD_ClientRecData: �ص�������ַ
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRaw_S_SetClientRecData( NETHANDLE handle,
												pfnRaw_S_ClientRecData pClientRecData )
{
	GetRawSvrInstance()->SetRecvHandle(handle, function20_bind_Ex(pClientRecData));
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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRaw_S_SentData( NETHANDLE handle,
															const char* c_pData,
															UInt32 u32Size )
{
	return GetRawSvrInstance()->Send(handle, c_pData, u32Size);
}
