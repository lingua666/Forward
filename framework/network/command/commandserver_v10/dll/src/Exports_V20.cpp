
#include <dllNetFrame_CmdServer_V10/Exports_V20.h>
#include <libNetFrame_CmdServer_V10/CommandServer_V10.h>

LOG_OBJ_DECL(Cmd_S_V20_Module)

void _CALLTYPE	g_CmdSvr10ClientClose201609141640_V20( pfnCmd_S_V20_Close pClientClose,
												 void* pUser,
                                                 const _string_type& sIP,
												 int uPort,
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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_V20_Init( int uWorkerThreads )
{
	UInt8 uNum = (uWorkerThreads == 0 ? 1 : uWorkerThreads);
	GetCommandServer_V10Instance()->Init(uNum);
	return 1;
}

/*****************************************************************
/*����˵����	�����˿�
/*����˵����	u16Port: �˿ں�
/*				pfnAccept: �ͻ������ӻص�����
/*				pUser: �û�����
/*				uIOThreadNum: IO�߳���
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_V20_Listen( int u16Port,
											pfnCmd_S_V20_Accept pfnAccept,
											void* pUser, UInt8 uIOThreadNum )
{
	if( GetCommandServer_V10Instance()->Listen(u16Port,
		function20_bind(pfnAccept, pUser, _function_::_1, _function_::_2, _function_::_3)) )
	{
		GetCommandServer_V10Instance()->Run( uIOThreadNum == 0 ? get_processor_number() * 2 + 2 : uIOThreadNum );
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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_V20_Close( void )
{
	GetCommandServer_V10Instance()->Stop();
	return 1;
}


/*****************************************************************
/*����˵����	�ͷŷ�������Դ(�����ڵ���MCmd_S_V20_Close��ִ�иýӿ�)
/*����˵����	
/*				
/*����ֵ��		>0:�ɹ�  <1: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_V20_Free( void )
{
	GetCommandServer_V10Instance()->Release();
	return 1;
}


/*****************************************************************
/*����˵����	�رտͻ�������
/*����˵����	handle: 64bit�ͻ��˾��
/*				
/*����ֵ��		>0:�ɹ�  <1: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_V20_DisConnectClient( NETHANDLE handle )
{
	GetCommandServer_V10Instance()->Close(handle);
	return 1;
}


/*****************************************************************
/*����˵����	���ÿͻ��˶Ͽ��ص�����
/*����˵����	handle: 64bit�ͻ��˾��
/*				pClientClose: �ص�������ַ
/*				pUser: �û�����
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_V20_SetClientDisConnect( NETHANDLE handle,
													pfnCmd_S_V20_Close pClientClose,
													void* pUser )
{
	/*GetCommandServer_V10Instance()->SetCloseHandle(handle,
						function20_bind(pClientClose, GetIPv4ToString(handle).c_str(),
									GetPort(handle), _function_::_1));*/

	return GetCommandServer_V10Instance()->SetDestroyHandle(handle,
		function20_bind(g_CmdSvr10ClientClose201609141640_V20, pClientClose,
		pUser, GetIPv4ToString(handle),
		GetPort(handle), _function_::_1));
}

/*****************************************************************
/*����˵����	���ÿͻ��˽��ջص�����
/*����˵����	handle: 64bit�ͻ��˾��
/*				pClientRecData: �ص�������ַ
/*				pUser: �û�����
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_V20_SetClientRecData( NETHANDLE handle,
												pfnCmd_S_V20_RecData pClientRecData,
												void* pUser )
{
	GetCommandServer_V10Instance()->SetRecvHandle(handle,
		function20_bind(pClientRecData, pUser, _function_::_1, _function_::_2, _function_::_3));
	return 1;
}

/*****************************************************************
/*����˵����	��ͻ��˷�������
/*����˵����	handle: 64bit�ͻ��˾��
/*				c_pData: ���ݻ�����
/*				u32Size: ���ݳ���
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_V20_SentData( NETHANDLE handle,
															const char* c_pData,
															int u32Size )
{
	return GetCommandServer_V10Instance()->Send(handle, c_pData, u32Size);
}
