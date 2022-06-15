
#include <dllNetFrame_AsyncCmdClients/Exports.h>
#include <libNetFrame_AsyncCmdClients/CommandClients.h>

DLL_INIT_DEF(g_pTagTModuleInfoRecord_201602172205,
			"45000001000V2016021700000000VZZZ10000000",
			"ϵͳͨ��ָ��ģ��",
			"V1.0.0.1",
			"20160217",
			"")

DLL_LOG_DEF(Cmd_C_Module)

//test
//LOG_OBJ_DECL(Cmd_C_DataError)
//test

fpnMCmd_C_Connect	g_fpnMCmd_C_Connect20160526;


void _CALLTYPE	gConnectResultBack20160526( fpnMCmd_C_Connect fpnConnect, 
										const _string_type& sIP,
										int u16Port,
										NETHANDLE Handle )
{
	if( fpnConnect != NULL )
	{
		fpnConnect(sIP.c_str(), u16Port, Handle);
	}
}

void _CALLTYPE	gCloseBack20160526( fpnMCmd_C_Close fpnClose, 
									const _string_type& sIP,
									int u16Port,
									NETHANDLE Handle )
{
	if( fpnClose != NULL )
	{
		fpnClose(sIP.c_str(), u16Port, Handle);
	}
}

/*****************************************************************
/*����˵����	��ʼ���ͻ��˼�
/*����˵����	pCMD_ClientConnect: �ص�������ַ
/*				
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_Init( int iThreadNum )
{
	//test
	//LOG_Print_SetLog(Cmd_C_Module, 1)
	//LOG_Print_SetLog(Cmd_C_DataError, 1)
	//test

	LOG_Print_Info(Cmd_C_Module,"MCmd_C_Init():")
	
	GetAsynCmdClientsInstance()->Init( iThreadNum );
	return 1;
}

/*****************************************************************
/*����˵����	�ͷſͻ���ģ����Դ(�ڲ�ʹ�ø�ģ��ʱʹ��)
/*����˵����	
/*				
/*����ֵ��		>1:�ɹ�  <1: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_Free( void )
{
	LOG_Print_Info(Cmd_C_Module,"MCmd_C_Free():")

	GetAsynCmdClientsInstance()->Release();
	return 1;
}

/*****************************************************************
/*����˵����	����������Ӧ�ص�����
/*����˵����	pCMD_ClientConnect: �ص�������ַ
/*				
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_SetConnect( fpnMCmd_C_Connect fpnConnect )
{
	LOG_Print_Info(Cmd_C_Module,"MCmd_C_SetConnect():")

	g_fpnMCmd_C_Connect20160526 = fpnConnect;
	return 1;
}

/*****************************************************************
/*����˵����	���ӷ�����
/*����˵����	c_szIP: ������IP
				u16Port: �������˿ں�
/*				
/*����ֵ��		64bit���Ӿ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS NETHANDLE _CALLTYPE MCmd_C_Connect( const char* c_szIP,
																int u16Port )
{
	LOG_Print_Info(Cmd_C_Module,"MCmd_C_Connect():\n\tc_szIP:%s\n\tu16Port:%d",c_szIP, u16Port)

	return GetAsynCmdClientsInstance()->Connect(c_szIP, u16Port,
							function20_bind(gConnectResultBack20160526,
									g_fpnMCmd_C_Connect20160526, 
									_string_type(c_szIP), u16Port, _function_::_1));
}

/*****************************************************************
/*����˵����	�ر�����
/*����˵����	handle: 64bit���Ӿ��
/*				
/*����ֵ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_Close( NETHANDLE handle )
{
	LOG_Print_Info(Cmd_C_Module,"MCmd_C_Close():\n\thandle:%lld",handle)

	GetAsynCmdClientsInstance()->Close(handle);
	return 1;
}

/*****************************************************************
/*����˵����	���öϿ��ص�����
/*����˵����	handle: 64bit���Ӿ��
				pCMD_ClientRecData: �ص�������ַ
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_SetClose( NETHANDLE handle,
															fpnMCmd_C_Close fpnClose )
{
	LOG_Print_Info(Cmd_C_Module,"MCmd_C_SetClose():\n\thandle:%lld",handle)

	return GetAsynCmdClientsInstance()->SetDestroyHandle(handle,
							function20_bind(gCloseBack20160526, fpnClose,
							GetIPv4ToString(handle),
							GetPort(handle),_function_::_1));
}


/*****************************************************************
/*����˵����	���ý��ջص�����
/*����˵����	handle: 64bit���Ӿ��
				pCMD_ClientRecData: �ص�������ַ
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_SetRecData( NETHANDLE handle,
															fpnMCmd_C_RecData fpnRecData )
{
	LOG_Print_Info(Cmd_C_Module,"MCmd_C_SetRecData():\n\thandle:%lld",handle)

	GetAsynCmdClientsInstance()->SetRecvHandle(handle, function20_bind_Ex(fpnRecData));
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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_SentData( NETHANDLE handle,
														const char* c_pData,
														int u32Size )
{
	LOG_Print_Info(Cmd_C_Module,"MCmd_C_SentData():\n\thandle:%lld\n\tSize:%d\n\tData:%s",
					handle, u32Size, c_pData)

	return GetAsynCmdClientsInstance()->Send(handle, c_pData, u32Size);
}

/*****************************************************************
/*����˵����	��ַ������
/*����˵����	szData: ���󻺳���
/*				
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MCmd_C_Error( const char* szData )
{
	/*LOG_Print_Info(Cmd_C_DataError,"MCmd_C_Error(): Data Address:%p", szData)
	LOG_Print_Info(Cmd_C_DataError,"MCmd_C_Error(): Data Content:%s", szData)*/
}