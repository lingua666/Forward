
#include <dllNetFrame_CmdServer/Exports_V20.h>
#include <libNetFrame_CmdServer/CommandServer.h>

LOG_OBJ_DECL(Cmd_S_V20_Module)

void _CALLTYPE g_CmdSvrClientClose201609141640_V20( fpnMCmd_S_V20_Close fpnClientClose,
											void* pUser,
											const _string_type& sIP,
											UInt16 uPort, 
											NETHANDLE Handle )
{
	if( fpnClientClose != NULL )
	{
		fpnClientClose(pUser, sIP.c_str(), uPort, Handle);
	}
}

/*****************************************************************
/*����˵����	�����˿�
/*����˵����	u16Port: �˿ں�
				pCMD_ServerAccept: �ͻ������ӻص�����
				pUser: �û�����
/*				
/*����ֵ��		>0:�ɹ�  <1: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_V20_Listen( UInt16 u16Port,
											fpnMCmd_S_V20_Accept fpnAccept,
											void* pUser, UInt8 uIOThreadNum,
											const char* c_szIP )
{
	LOG_Print_Info(Cmd_S_V20_Module,"MCmd_S_V20_Listen():\n\t16Port:%d", u16Port)

	if( GetCommandServerInstance()->Listen(u16Port,
		function20_bind(fpnAccept, pUser, _function_::_1, _function_::_2, _function_::_3), c_szIP))
	{
		GetCommandServerInstance()->Run( uIOThreadNum == 0 ? get_processor_number() * 2 + 2 : uIOThreadNum );
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
	LOG_Print_Info(Cmd_S_V20_Module,"MCmd_S_V20_Close():")

	GetCommandServerInstance()->Stop();
	return 1;
}


/*****************************************************************
/*����˵����	�ͷŷ�������Դ(�����ڵ���CMD_Server_MClose��ִ�иýӿ�)
/*����˵����	
/*				
/*����ֵ��		>0:�ɹ�  <1: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_V20_Free( void )
{
	LOG_Print_Info(Cmd_S_V20_Module,"MCmd_S_V20_Free():")

	GetCommandServerInstance()->Release();
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
	LOG_Print_Info(Cmd_S_V20_Module,"MCmd_S_V20_DisConnectClient():\n\thandle:%lld",handle)

	GetCommandServerInstance()->Close(handle);
	return 1;
}


/*****************************************************************
/*����˵����	���ÿͻ��˶Ͽ��ص�����
/*����˵����	handle: 64bit�ͻ��˾��
				pCMD_ClientClose: �ص�������ַ
				pUser: �û�����
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_V20_SetClientDisConnect( NETHANDLE handle,
													fpnMCmd_S_V20_Close fpnClientClose,
													void* pUser )
{
	LOG_Print_Info(Cmd_S_V20_Module,"MCmd_S_V20_SetClientDisConnect():\n\thandle:%lld",handle)

	/*GetCommandServerInstance()->SetCloseHandle(handle,
										function20_bind(fpnClientClose, GetIPv4ToString(handle),
													GetPort(handle), _function_::_1));*/

	return GetCommandServerInstance()->SetDestroyHandle(handle,
										function20_bind(g_CmdSvrClientClose201609141640_V20, fpnClientClose,
													pUser, GetIPv4SFromNETNODE(handle), GetPortFromNETNODE(handle), _function_::_1));
}

/*****************************************************************
/*����˵����	���ÿͻ��˽��ջص�����
/*����˵����	handle: 64bit�ͻ��˾��
				pCMD_ClientRecData: �ص�������ַ
				pUser: �û�����
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_V20_SetClientRecData( NETHANDLE handle,
												fpnMCmd_S_V20_RecData fpnClientRecData,
												void* pUser )
{
	LOG_Print_Info(Cmd_S_V20_Module,"MCmd_S_V20_SetClientRecData():\n\thandle:%lld",handle)

	GetCommandServerInstance()->SetRecvHandle(handle, 
											function20_bind(fpnClientRecData, pUser, _function_::_1, _function_::_2,
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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_V20_SentData( NETHANDLE handle,
														const char* c_pData,
														UInt32 u32Size )
{
	LOG_Print_Info(Cmd_S_V20_Module,"MCmd_S_V20_SentData():\n\thandle:%lld\n\tSize:%d\n\tData:%s",
		handle, u32Size, c_pData)

	return GetCommandServerInstance()->Send(handle, c_pData, u32Size);
}
