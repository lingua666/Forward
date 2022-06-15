
#include <dllNetFrame_CmdServer/Exports.h>
#include <libNetFrame_CmdServer/CommandServer.h>

DLL_INIT_DEF(g_pTagTModuleInfoRecord_201602172200,
	"45000001000V2016021700000000VZZZ00000000",
	"ϵͳͨ��ָ��ģ��",
	"V1.0.0.1",
	"20160217",
	"")

DLL_LOG_DEF(Cmd_S_Module)

void _CALLTYPE g_CmdSvrClientClose201609141640( fpnMCmd_S_Close fpnClientClose,
											const _string_type& sIP,
											UInt16 uPort, 
											NETHANDLE Handle )
{
	if( fpnClientClose != NULL )
	{
		fpnClientClose(sIP.c_str(), uPort, Handle);
	}
}

/*****************************************************************
/*����˵����	�����˿�
/*����˵����	u16Port: �˿ں�
				pCMD_ServerAccept: �ͻ������ӻص�����
/*				
/*����ֵ��		>0:�ɹ�  <1: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_Listen( UInt16 u16Port,
											fpnMCmd_S_Accept fpnAccept, const char* c_szIP)
{
	LOG_Print_Info(Cmd_S_Module,"MCmd_S_Listen():\n\t16Port:%d", u16Port)

	if( GetCommandServerInstance()->Listen(u16Port, function20_bind_Ex(fpnAccept), c_szIP))
	{
		GetCommandServerInstance()->Run( get_processor_number() * 2 + 2 );
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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_Close( void )
{
	LOG_Print_Info(Cmd_S_Module,"MCmd_S_Close():")

	GetCommandServerInstance()->Stop();
	return 1;
}


/*****************************************************************
/*����˵����	�ͷŷ�������Դ(�����ڵ���CMD_Server_MClose��ִ�иýӿ�)
/*����˵����	
/*				
/*����ֵ��		>0:�ɹ�  <1: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_Free( void )
{
	LOG_Print_Info(Cmd_S_Module,"MCmd_S_Free():")

	GetCommandServerInstance()->Release();
	return 1;
}


/*****************************************************************
/*����˵����	�رտͻ�������
/*����˵����	handle: 64bit�ͻ��˾��
/*				
/*����ֵ��		>0:�ɹ�  <1: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_DisConnectClient( NETHANDLE handle )
{
	LOG_Print_Info(Cmd_S_Module,"MCmd_S_DisConnectClient():\n\thandle:%lld",handle)

	GetCommandServerInstance()->Close(handle);
	return 1;
}


/*****************************************************************
/*����˵����	���ÿͻ��˶Ͽ��ص�����
/*����˵����	handle: 64bit�ͻ��˾��
				pCMD_ClientClose: �ص�������ַ
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_SetClientDisConnect( NETHANDLE handle,
													fpnMCmd_S_Close fpnClientClose )
{
	LOG_Print_Info(Cmd_S_Module,"MCmd_S_SetClientDisConnect():\n\thandle:%lld",handle)

	/*GetCommandServerInstance()->SetCloseHandle(handle,
										function20_bind(fpnClientClose, GetIPv4ToString(handle),
													GetPort(handle), _function_::_1));*/

	return GetCommandServerInstance()->SetDestroyHandle(handle,
										function20_bind(g_CmdSvrClientClose201609141640, fpnClientClose,
											GetIPv4SFromNETNODE(handle), GetPortFromNETNODE(handle), _function_::_1));
}

/*****************************************************************
/*����˵����	���ÿͻ��˽��ջص�����
/*����˵����	handle: 64bit�ͻ��˾��
				pCMD_ClientRecData: �ص�������ַ
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_SetClientRecData( NETHANDLE handle,
												fpnMCmd_S_RecData fpnClientRecData )
{
	LOG_Print_Info(Cmd_S_Module,"MCmd_S_SetClientRecData():\n\thandle:%lld",handle)

	GetCommandServerInstance()->SetRecvHandle(handle, function20_bind_Ex(fpnClientRecData));
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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_SentData( NETHANDLE handle,
														const char* c_pData,
														UInt32 u32Size )
{
	LOG_Print_Info(Cmd_S_Module,"MCmd_S_SentData():\n\thandle:%lld\n\tSize:%d\n\tData:%s",
		handle, u32Size, c_pData)

	return GetCommandServerInstance()->Send(handle, c_pData, u32Size);
}
