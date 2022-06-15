#include <string.h>
#include <stdio.h>
#include <libThirdWrap_Onvif/ONVIF_INTERFACE.h>


//char *Pub_server_addr = "http://192.168.8.23/onvif/device_service";       //ȫ�ַ������ĵ�ַdevice_service
//char *Pub_ptz_addr = "http://192.168.8.23/onvif/PTZ";
//char *Pub_Media_addr = "http://192.168.8.23/onvif/Media";
char Pub_MediaUri[1024] = {0};

ONVIF_INTERFACE::ONVIF_INTERFACE( void )
	: _hSoap( NULL )
	, _isPTZ( false )
{
	memset(_UserName, 0, sizeof(_UserName));
	memset(_Password, 0, sizeof(_Password));
	memset(_Service_Addr, 0, sizeof(_Service_Addr));
	memset(_PTZ_Addr, 0, sizeof(_PTZ_Addr));
	memset(_Media_Addr, 0, sizeof(_Media_Addr));
	memset(_MediaUri, 0, sizeof(_MediaUri));
	memset(_ProfilesToken, 0, sizeof(_ProfilesToken));
	memset(_PTZPresetToken, 0, sizeof(_PTZPresetToken));
}

ONVIF_INTERFACE::~ONVIF_INTERFACE( void )
{
	Relase();
}

int ONVIF_INTERFACE::Init( const char* c_pService_Addr,
						const char* c_pUserName,
						const char* c_pPassword,
						bool isMain )
{
	Relase();

	strcpy(_UserName, c_pUserName);
	strcpy(_Password, c_pPassword);
	strcpy(_Service_Addr, c_pService_Addr);
	_hSoap = ONVIF_malloc(c_pUserName, c_pPassword);

	//��ȡý���ַ��PTZ��ַ
	ONVIF_GetCapabilities_C_DATE	CapDate = {0};
	if( GetCapabilities(&CapDate) != 1 )
	{
		memset(_Service_Addr, 0, sizeof(_Service_Addr));
		return -1;
	}

	strcpy(_Media_Addr, CapDate.Media_addr);
	strcpy(_PTZ_Addr, CapDate.PTZ_addr);
	_isPTZ = CapDate.isPTZ;

	ONVIF_GetProfiles_C_DATE ProDate = {0};
	if( GetProfiles(isMain, &ProDate) != 1 )
	{
		memset(_Service_Addr, 0, sizeof(_Service_Addr));
		return -1;
	}

	strcpy(_ProfilesToken, ProDate.ProfilesToken);

	PTZ_GetPresetToken();

	return 1;
}

void ONVIF_INTERFACE::Relase( void )
{
	memset(_Service_Addr, 0, sizeof(_Service_Addr));
	if( _hSoap != NULL )
	{
		ONVIF_free(_hSoap);
		_hSoap = NULL;
	}
}

const char* ONVIF_INTERFACE::GetMediaUri( void )
{
	if( ONVIF_GetUri_C(_hSoap, _Media_Addr, _ProfilesToken, 
		_UserName, _Password, _MediaUri) == 1 )
	{
		return _MediaUri;
	}
	
	return "";
}

//��ȡ�豸������ֵ��ʹ�õ����豸��ַ��Ҳ���Ƿ���������ȡý����Ϣ��ַ�ṩ��GetProfiles�ӿ�ʹ��,data��ŷ������ݵĽṹ��
int ONVIF_INTERFACE::GetCapabilities( ONVIF_GetCapabilities_C_DATE* _OUT Ret_Data )
{	
	return ONVIF_GetCapabilities_C(_hSoap, _Service_Addr, _UserName, _Password, Ret_Data);
}

//��ȡý�����Ϣ�ļ���ʹ�õ���ý����Ϣ��ַ��ʶ��������ͨ���ı���ֱ��ʣ��ṩ��GetStreamuri�ӿ�ʹ��, data����GetProfiles�����ݽṹ��
int ONVIF_INTERFACE::GetProfiles( bool isMain, ONVIF_GetProfiles_C_DATE* _OUT Ret_Data )
{ 
	return ONVIF_GetProfiles_C(_hSoap, _Media_Addr, _UserName, _Password, isMain ? 1 : 0, Ret_Data);
}

int ONVIF_INTERFACE::SetSystemTimeAndDate( const ONVIF_C_TimeAndDate* _OUT Ret_Data )
{
	ONVIF_C_TimeAndDate	Date = {0};
	if( Ret_Data == NULL )
		return -1;

	memcpy(&Date, Ret_Data, sizeof(ONVIF_C_TimeAndDate));
	int iHour = Ret_Data->Hour - 8;
	if( iHour < 0 )
	{//
		if(Date.Day == 1)
		{
			if( Date.Month == 1 )
			{
				Date.Year -= 1;
				Date.Month = 12;
				Date.Day = get_days_to_month(Date.Year, Date.Month);
			}
			else
			{
				Date.Month -= 1;
				Date.Day = get_days_to_month(Date.Year, Date.Month);
			}
		}
		else
		{
			Date.Day -= 1;
		}

		Date.Hour = 24 + iHour;
	}
	else
	{
		Date.Hour -= 8;
	}

	return ONVIF_SetSystemTimeAndDate_C(_hSoap, _Service_Addr, _UserName, _Password, &Date);
}

int ONVIF_INTERFACE::GetSystemTimeAndDate( tagOnvif_TimeAndDate* _OUT Ret_Data )
{
	return ONVIF_GetSystemTimeAndDate_C(_hSoap, _Service_Addr, Ret_Data);
}

//��ȡ��̨�豸��nodesֵ��uir��ַ�õ��Ǿ߱���̨���ܵķ�������ַ  ps:"http://192.168.8.23/onvif/PTZ"    
int ONVIF_INTERFACE::PTZ_GetNodeToken( char* _OUT Ret_GetNodeToken )
{
	return ONVIF_PTZ_GetNodes_C(_hSoap, _Service_Addr, Ret_GetNodeToken);
}

//��ȡ�豸��̨��Presetsֵ,��ַ�õ��Ǿ߱���̨���ܵķ�������ַ  ps:"http://192.168.8.23/onvif/PTZ"  
int ONVIF_INTERFACE::PTZ_GetPresetToken( void )
{
	ONVIF_PTZ_GetPresets_C_DATE Ret_Data;
	int iRet = ONVIF_PTZ_GetPresets_C(_hSoap, _ProfilesToken, _Service_Addr, _UserName, _Password, &Ret_Data);
	if( iRet == 1 )
	{
		strcpy(_PTZPresetToken, Ret_Data.PresetToken);
	}

	return iRet;
}

//�����豸��̨��Presetֵ��uir��ַ�õ��Ǿ߱���̨���ܵķ�������ַ  ps:"http://192.168.8.23/onvif/PTZ"
//����ӿ��õ���GetPresets���ص���Ϣ�����ṹ��ģ�
void ONVIF_INTERFACE::PTZ_SetPreset(void)
{
	/*char ret_PresetToken[1024] = {0};
	ONVIF_PTZ_SetPreset_C(Pri_ProfilesToken, Pri_PresetToken, Pri_PresetName, Pri_ptz_addr, ret_PresetToken);   //����Ĳ�����GetPresets���صĵ���Ϣ���
	strcpy(Pri_PresetToken, ret_PresetToken);*/
}

//�ص�Ԥ��㣬����ӿ���GetPresets ������SetPreset ���ص���Ϣ�����ṹ��  ,�Ƴ�Ԥ�裨RemovePreset��Ҳ�Ǻ�GotoPresetһ��
int ONVIF_INTERFACE::PTZ_GotoPreset( void )
{
	if( strcmp(_PTZ_Addr, "") == 0 )
		return -1;

	return ONVIF_PTZ_GotoPreset_C(_hSoap, _ProfilesToken, _PTZPresetToken, _PTZ_Addr, _UserName, _Password);
}

//��ȡ�豸��̨�ĵ������Ϣ��ʹ�õ�ConfigurationToken ���豸��̨�ķ����ַ
/*void ONVIF_INTERFACE::PTZ_GetConfigurationOptions(void)
{
	//ONVIF_PTZ_GetConfigurationOptions_C(Pri_ConfigurationToken, Pri_ptz_addr);
}*/

//��̨�豸�ľ����ƶ��� ʹ�õ��豸��ProfileToken ���豸��̨�ķ����ַ
int ONVIF_INTERFACE::PTZ_AbsoluteMove(void)
{
	if( strcmp(_PTZ_Addr, "") == 0 )
		return -1;
 
	return ONVIF_PTZ_AbsoluteMove_C(_hSoap, _ProfilesToken, _PTZ_Addr, _UserName, _Password);
}

//��̨�豸��ֹͣ�ƶ�ָ�ʹ�õ��豸��ProfileToken ���豸��̨�ķ����ַ        
int ONVIF_INTERFACE::PTZ_Stop(void)
{
	if( strcmp(_PTZ_Addr, "") == 0 )
		return -1;

	return ONVIF_PTZ_Stop_C(_hSoap, _ProfilesToken, _PTZ_Addr, _UserName, _Password);
}

//��̨�豸�������ƶ���ʹ�õ��豸��ProfileToken ���豸��̨�ķ����ַ   
int ONVIF_INTERFACE::PTZ_ContinuousMove( UInt8 uType, UInt8 uSpeed )
{
	if( strcmp(_PTZ_Addr, "") == 0 || uType >= PTZ_COUNT_CONTROL || uSpeed > 7 )
		return -1;

	return ONVIF_PTZ_ContinuousMove_C(_hSoap, _ProfilesToken, _PTZ_Addr, _UserName, _Password,
		uType, uSpeed);
}

