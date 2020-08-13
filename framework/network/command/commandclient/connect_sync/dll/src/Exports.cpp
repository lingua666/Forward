
#include <dllNetFrame_SyncCmdClients/Exports.h>
#include <libNetFrame_SyncCmdClients/CommandClients.h>

DLL_INIT_DEF(g_pTagTModuleInfoRecord_201602172205,
			"45000001000V2016021700000000VZZZ00000000",
			"ϵͳͨ��ָ��ģ��",
			"V1.0.0.1",
			"20160217",
			"")


			
void _CALLTYPE g_Cmd_SynC_Close201609141643( fpnMCmd_SynC_Close fpnClose,
											const _string_type& sIP,
											UInt16 uPort,
											NETHANDLE Handle )
{
	if( fpnClose != NULL )
	{
		fpnClose(sIP.c_str(), uPort, Handle);
	}
}

/*****************************************************************
/*����˵����	��ʼ���ͻ��˼�
/*����˵����	CMD_Clients_Init
/*				
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SynC_Init( void )
{
	Singleton<syncCommandClients_type>::instance()->Init(get_processor_number() * 2 + 2);
	return 1;
}

/*****************************************************************
/*����˵����	�ͷſͻ���ģ����Դ(�ڲ�ʹ�ø�ģ��ʱʹ��)
/*����˵����	
/*				
/*����ֵ��		>1:�ɹ�  <1: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SynC_Free( void )
{
	Singleton<syncCommandClients_type>::instance()->Release();
	return 1;
}

/*****************************************************************
/*����˵����	���ӷ�����
/*����˵����	c_szIP: ������IP
				u16Port: �������˿ں�
/*				
/*����ֵ��		64bit���Ӿ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS NETHANDLE _CALLTYPE MCmd_SynC_Connect( const char* c_szIP,
																	UInt16 u16Port )
{
	return Singleton<syncCommandClients_type>::instance()->Connect(c_szIP, u16Port);
}

/*****************************************************************
/*����˵����	�ر�����
/*����˵����	handle: 64bit���Ӿ��
/*				
/*����ֵ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SynC_Close( NETHANDLE handle )
{
	Singleton<syncCommandClients_type>::instance()->Close(handle);
	return 1;
}

/*****************************************************************
/*����˵����	���öϿ��ص�����
/*����˵����	handle: 64bit���Ӿ��
				pCMD_ClientRecData: �ص�������ַ
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SynC_SetClose( NETHANDLE handle,
															fpnMCmd_SynC_Close fpnClose )
{
	Singleton<syncCommandClients_type>::instance()->SetCloseHandle(handle, 
																function20_bind(g_Cmd_SynC_Close201609141643, fpnClose,
																GetIPv4ToString(handle),
																GetPort(handle), _function_::_1));
	return 1;
}


/*****************************************************************
/*����˵����	���ý��ջص�����
/*����˵����	handle: 64bit���Ӿ��
				pCMD_ClientRecData: �ص�������ַ
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SynC_SetRecData( NETHANDLE handle,
																fpnMCmd_SynC_RecData fpnRecData )
{
	Singleton<syncCommandClients_type>::instance()->SetRecvHandle(handle, function20_bind_Ex(fpnRecData));
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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SynC_SentData( NETHANDLE handle,
															const char* c_pData,
															UInt32 u32Size )
{
	return Singleton<syncCommandClients_type>::instance()->Send(handle, c_pData, u32Size);
}