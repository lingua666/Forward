
#include <dllNetFrame_AsyncCmdClients/Exports_V20.h>
#include <libNetFrame_AsyncCmdClients/CommandClients.h>

LOG_OBJ_DECL(Cmd_C_V20_Module)

fpnMCmd_C_V20_Connect	g_fpnMCmd_C_Connect20160526_v20;


void _CALLTYPE	gConnectResultBack20160526_V20( fpnMCmd_C_V20_Connect fpnConnect, 
										void* pUser,
										const _string_type& sIP,
										int u16Port,
										NETHANDLE Handle )
{
	if( fpnConnect != NULL )
	{
		fpnConnect(pUser, sIP.c_str(), u16Port, Handle);
	}
}

void _CALLTYPE	gCloseBack20160526_V20( fpnMCmd_C_V20_Close fpnClose, 
									void* pUser,
									const _string_type& sIP,
									int u16Port,
									NETHANDLE Handle )
{
	if( fpnClose != NULL )
	{
		fpnClose(pUser, sIP.c_str(), u16Port, Handle);
	}
}

/*****************************************************************
/*����˵����	��ʼ���ͻ��˼�
/*����˵����	iIOThreadNum : IO�����߳�����, �������������Ĭ��
/*				iProcThreadNum : ���ݴ����߳�����, �������������Ĭ��
/*				
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_V20_Init( int iIOThreadNum, int iProcThreadNum )
{
	//test
	//LOG_Print_SetLog(Cmd_C_Module, 1)
	//LOG_Print_SetLog(Cmd_C_DataError, 1)
	//test

	LOG_Print_Info(Cmd_C_V20_Module,"MCmd_C_V20_Init():")
	
	GetAsynCmdClientsInstance()->Init( iIOThreadNum, iProcThreadNum );
	return 1;
}

/*****************************************************************
/*����˵����	�ͷſͻ���ģ����Դ(�ڲ�ʹ�ø�ģ��ʱʹ��)
/*����˵����	
/*				
/*����ֵ��		>1:�ɹ�  <1: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_V20_Free( void )
{
	LOG_Print_Info(Cmd_C_V20_Module,"MCmd_C_V20_Free():")

	GetAsynCmdClientsInstance()->Release();
	return 1;
}

/*****************************************************************
/*����˵����	����������Ӧ�ص�����
/*����˵����	pCMD_ClientConnect: �ص�������ַ
/*				
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_V20_SetConnect( fpnMCmd_C_V20_Connect fpnConnect )
{
	LOG_Print_Info(Cmd_C_V20_Module,"MCmd_C_V20_SetConnect():")

	g_fpnMCmd_C_Connect20160526_v20 = fpnConnect;
	return 1;
}

/*****************************************************************
/*����˵����	���ӷ�����
/*����˵����	c_szIP: ������IP
				u16Port: �������˿ں�
				pUser: �����û�����
/*				
/*����ֵ��		64bit���Ӿ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS NETHANDLE _CALLTYPE MCmd_C_V20_Connect( const char* c_szIP,
																int u16Port,void* pUser )
{
	LOG_Print_Info(Cmd_C_V20_Module,"MCmd_C_V20_Connect():\n\tc_szIP:%s\n\tu16Port:%d",c_szIP, u16Port)

	return GetAsynCmdClientsInstance()->Connect(c_szIP, u16Port,
							function20_bind(gConnectResultBack20160526_V20,
									g_fpnMCmd_C_Connect20160526_v20, pUser,
									_string_type(c_szIP), u16Port, _function_::_1));
}

/*****************************************************************
/*����˵����	�ر�����
/*����˵����	handle: 64bit���Ӿ��
/*				
/*����ֵ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_V20_Close( NETHANDLE handle )
{
	LOG_Print_Info(Cmd_C_V20_Module,"MCmd_C_V20_Close():\n\thandle:%lld",handle)

	GetAsynCmdClientsInstance()->Close(handle);
	return 1;
}

/*****************************************************************
/*����˵����	���öϿ��ص�����
/*����˵����	handle: 64bit���Ӿ��
				pCMD_ClientRecData: �ص�������ַ
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_V20_SetClose( NETHANDLE handle,
															fpnMCmd_C_V20_Close fpnClose,
															void* pUser )
{
	LOG_Print_Info(Cmd_C_V20_Module,"MCmd_C_V20_SetClose():\n\thandle:%lld",handle)

	return GetAsynCmdClientsInstance()->SetDestroyHandle(handle,
							function20_bind(gCloseBack20160526_V20, fpnClose,
							pUser, GetIPv4ToString(handle),
							GetPort(handle),_function_::_1));
}


/*****************************************************************
/*����˵����	���ý��ջص�����
/*����˵����	handle: 64bit���Ӿ��
				pCMD_ClientRecData: �ص�������ַ
				pUser: �û�����
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_V20_SetRecData( NETHANDLE handle,
															fpnMCmd_C_V20_RecData fpnRecData,
															void* pUser )
{
	LOG_Print_Info(Cmd_C_V20_Module,"MCmd_C_V20_SetRecData():\n\thandle:%lld",handle)

	GetAsynCmdClientsInstance()->SetRecvHandle(handle,
										function20_bind(fpnRecData, pUser,
												_function_::_1, _function_::_2, _function_::_3));
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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_V20_SentData( NETHANDLE handle,
														const char* c_pData,
														int u32Size )
{
	LOG_Print_Info(Cmd_C_V20_Module,"MCmd_C_V20_SentData():\n\thandle:%lld\n\tSize:%d\n\tData:%s",
					handle, u32Size, c_pData)

	return GetAsynCmdClientsInstance()->Send(handle, c_pData, u32Size);
}

/*****************************************************************
/*����˵����	��ַ������
/*����˵����	szData: ���󻺳���
/*				
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MCmd_C_V20_Error( const char* szData )
{
	/*LOG_Print_Info(Cmd_C_DataError,"MCmd_C_Error(): Data Address:%p", szData)
	LOG_Print_Info(Cmd_C_DataError,"MCmd_C_Error(): Data Content:%s", szData)*/
}