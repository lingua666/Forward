
#include <dllNetFrame_AsyncRawClients/Exports.h>
#include <libNetFrame_AsyncRawClients/RawClients.h>

DLL_INIT_DEF(g_pTagTModuleInfoRecord_201602172205,
			"45000001000V2016021700000000VZZZ10000000",
			"ϵͳͨ��ָ��ģ��",
			"V1.0.0.1",
			"20160217",
			"")

DLL_LOG_DEF(Raw_C_Module)

fpnMRaw_C_Connect	g_fpnMRaw_C_Connect20160526;

void _CALLTYPE	gConnectResultBack20160626( fpnMRaw_C_Connect fpnConnect, 
										const _string_type& sIP,
										UInt16 u16Port,
										NETHANDLE Handle )
{
	if( fpnConnect != NULL )
	{
		fpnConnect(sIP.c_str(), u16Port, Handle);
	}
}

void _CALLTYPE	gCloseBack20160626( fpnMRaw_C_Close fpnClose, 
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
/*����˵����	pCMD_ClientConnect: �ص�������ַ
/*				
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRaw_C_Init( void )
{
	LOG_Print_Info(Raw_C_Module,"MRaw_C_Init():")

	GetAsynRawClientsInstance()->Init( get_processor_number() * 2 + 2 );
	return 1;
}

/*****************************************************************
/*����˵����	�ͷſͻ���ģ����Դ(�ڲ�ʹ�ø�ģ��ʱʹ��)
/*����˵����	
/*				
/*����ֵ��		>1:�ɹ�  <1: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRaw_C_Free( void )
{
	LOG_Print_Info(Raw_C_Module,"MRaw_C_Free():")

	GetAsynRawClientsInstance()->Release();
	return 1;
}

/*****************************************************************
/*����˵����	����������Ӧ�ص�����
/*����˵����	pCMD_ClientConnect: �ص�������ַ
/*				
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRaw_C_SetConnect( fpnMRaw_C_Connect fpnConnect )
{
	LOG_Print_Info(Raw_C_Module,"MRaw_C_SetConnect():")

	g_fpnMRaw_C_Connect20160526 = fpnConnect;
	return 1;
}

/*****************************************************************
/*����˵����	���ӷ�����
/*����˵����	c_szIP: ������IP
				u16Port: �������˿ں�
/*				
/*����ֵ��		64bit���Ӿ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS NETHANDLE _CALLTYPE MRaw_C_Connect( const char* c_szIP,
																UInt16 u16Port )
{
	LOG_Print_Info(Raw_C_Module,"MRaw_C_Connect():\n\tc_szIP:%s\n\tu16Port:%d",c_szIP, u16Port)

	return GetAsynRawClientsInstance()->Connect(c_szIP, u16Port,
							function20_bind(gConnectResultBack20160626,
									g_fpnMRaw_C_Connect20160526, 
									_string_type(c_szIP), u16Port, _function_::_1));
}

/*****************************************************************
/*����˵����	�ر�����
/*����˵����	handle: 64bit���Ӿ��
/*				
/*����ֵ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRaw_C_Close( NETHANDLE handle )
{
	LOG_Print_Info(Raw_C_Module,"MRaw_C_Close():\n\thandle:%lld",handle)

	GetAsynRawClientsInstance()->Close(handle);
	return 1;
}

/*****************************************************************
/*����˵����	���öϿ��ص�����
/*����˵����	handle: 64bit���Ӿ��
				pCMD_ClientRecData: �ص�������ַ
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRaw_C_SetClose( NETHANDLE handle,
															fpnMRaw_C_Close fpnClose )
{
	LOG_Print_Info(Raw_C_Module,"MRaw_C_SetClose():\n\thandle:%lld",handle)

	return GetAsynRawClientsInstance()->SetDestroyHandle(handle,
							function20_bind(gCloseBack20160626, fpnClose,
							GetIPv4ToString(handle),
							GetPort(handle),_function_::_1));
}


/*****************************************************************
/*����˵����	���ý��ջص�����
/*����˵����	handle: 64bit���Ӿ��
				pCMD_ClientRecData: �ص�������ַ
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRaw_C_SetRecData( NETHANDLE handle,
															fpnMRaw_C_RecData fpnRecData )
{
	LOG_Print_Info(Raw_C_Module,"MRaw_C_SetRecData():\n\thandle:%lld",handle)

	GetAsynRawClientsInstance()->SetRecvHandle(handle, function20_bind_Ex(fpnRecData));
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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRaw_C_SentData( NETHANDLE handle,
														const char* c_pData,
														UInt32 u32Size )
{
	LOG_Print_Info(Raw_C_Module,"MRaw_C_SentData():\n\thandle:%lld\n\tSize:%d\n\tData:%s",
					handle, u32Size, c_pData)

	return GetAsynRawClientsInstance()->Send(handle, c_pData, u32Size);
}