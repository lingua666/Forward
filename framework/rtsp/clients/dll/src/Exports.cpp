
#include <dllFrameWork_RtspClients/Exports.h>
#include <libFrameWork_RtspClients/RtspClients.h>

fpnMRtspClients_Except g_fnExcept201806191635 = NULL;
fpnMRtspClients_Success	g_fnSuccess201806191635 = NULL;
fpnMRtspClients_RtpData g_fnRtpData201806191635 = NULL;

void g_ExceptNT201806191635( void* pUser, const char* c_szSessionUid, Int32 iErrCode )
{
	if( g_fnExcept201806191635 )
	{
		g_fnExcept201806191635(pUser, _string_type::StringToInt64(c_szSessionUid), iErrCode);
	}
}


void g_SdpNT201806191635( void* pUser, const char* c_szSessionUid, RtspClient_Impl::tagSdpInfo* SdpInfo )
{

}

void g_SuccessNT201806191635( void* pUser,  const char* c_szSessionUid )
{
	if( g_fnSuccess201806191635 )
	{
		g_fnSuccess201806191635(pUser, _string_type::StringToInt64(c_szSessionUid));
	}
}

void g_RtpDataNT201806191635( void* pUser, const char* c_szSessionUid,
	const char* c_szData, UInt32 uDataSize )
{
	if( g_fnRtpData201806191635 )
	{
		g_fnRtpData201806191635(pUser, _string_type::StringToInt64(c_szSessionUid), c_szData, uDataSize);
	}
}


/*****************************************************************
/*����˵����	��ʼ��
/*����˵����	uRtspIOThreadNum: Rtsp�������Ӵ����̸߳���
/*				uRtspProcThreadNum: Rtsp���ݴ����̸߳���
/*				uRtpIOThreadNum: Rtp�������Ӵ����̸߳���
/*				uRtpProcThreadNum: Rtp���ݴ����̸߳���
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspClients_Init( unsigned char uRtspIOThreadNum,
	unsigned char uRtspProcThreadNum,
	unsigned char uRtpIOThreadNum,
	unsigned char uRtpProcThreadNum )
{
	Singleton<RtspClients>::instance()->Init(uRtspIOThreadNum, uRtspProcThreadNum,
				uRtpIOThreadNum, uRtpProcThreadNum);
	return 1;
}


/*****************************************************************
/*����˵����	ģ���ͷ�
/*����˵����	
/*				
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MRtspClients_Free( void )
{
	Singleton<RtspClients>::instance()->Release();
}


/*****************************************************************
/*����˵����	��Rtsp�Ự
/*����˵����	c_szRtspPath: Rtsp·��
/*				iStreamType: ������	
/*							0: TCP
/*							1: UDP
/*				iRtpPort: rtp�˿ں�
/*����ֵ��		�Ự���
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS THandle _CALLTYPE MRtspClients_Open( const char* c_szRtspPath,
	int iStreamType, int iRtpPort )
{
	_string_type sSessionUid = Singleton<RtspClients>::instance()->Open(c_szRtspPath, iStreamType, iRtpPort);
	return _string_type::StringToInt64(sSessionUid);
}


/*****************************************************************
/*����˵����	����Rtsp�Ự����ص�����
/*����˵����	Handle:  rtsp�Ự���
/*				fpExcept: �쳣�ص�����
/*				pExceptUser: �û�Я������Ϣ
/*				fpSuccess: �ɹ��ص�����
/*				pUser: �û�Я������Ϣ
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspClients_SetResultEvent( THandle Handle,
	fpnMRtspClients_Except fpExcept, void* pExceptUser,
	fpnMRtspClients_Success fpSuccess, void* pUser )
{
	g_fnExcept201806191635 = fpExcept;
	g_fnSuccess201806191635 = fpSuccess;
	_string_type sSessionUid = _string_type::NumberToStr(Handle);
	Singleton<RtspClients>::instance()->SetResultEvent(sSessionUid.c_str(),
		function20_bind(g_ExceptNT201806191635, pExceptUser, _function_::_1, _function_::_2),
		function20_bind(g_SuccessNT201806191635, pUser, _function_::_1));
	return 1;
}


/*****************************************************************
/*����˵����	�������ݻص�����
/*����˵����	Handle:  rtsp�Ự���
/*				fpRtpData: rtp���ݻص�����
/*				pUser: �û�Я������Ϣ
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspClients_SetDataEvent( THandle Handle, 
	fpnMRtspClients_RtpData fpRtpData, void* pUser )
{
	g_fnRtpData201806191635 = fpRtpData;
	_string_type sSessionUid = _string_type::NumberToStr(Handle);
	Singleton<RtspClients>::instance()->SetDataEvent(sSessionUid.c_str(),
		function20_bind(g_RtpDataNT201806191635, pUser, _function_::_1, _function_::_2, _function_::_3));
	return 1;
}


/*****************************************************************
/*����˵����	����Rtsp�Ự
/*����˵����	Handle:  rtsp�Ự���
/*				
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspClients_Play( THandle Handle )
{
	_string_type sSessionUid = _string_type::NumberToStr(Handle);
	return Singleton<RtspClients>::instance()->Play(sSessionUid.c_str());
}


/*****************************************************************
/*����˵����	�ر�Rtsp�Ự
/*����˵����	Handle:  rtsp�Ự���
/*				
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MRtspClients_Close( THandle Handle )
{
	_string_type sSessionUid = _string_type::NumberToStr(Handle);
	Singleton<RtspClients>::instance()->Close(sSessionUid.c_str());
}