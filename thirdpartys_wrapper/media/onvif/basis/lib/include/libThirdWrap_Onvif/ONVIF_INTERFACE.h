
#ifndef _ONVIF_INTERFACE_H_
#define _ONVIF_INTERFACE_H_

#include "ONVIF_Interface_C.h"
#include "External.h"
#include <windows.h>

class ONVIF_INTERFACE
{
public:
	ONVIF_INTERFACE( void );

	~ONVIF_INTERFACE( void );

	int Init( const char* c_pService_Addr,
			const char* c_pUserName,
			const char* c_pPassword,
			bool isMain = true );

	void Relase( void );

	const char* GetMediaUri( void );

	int GetCapabilities( ONVIF_GetCapabilities_C_DATE* _OUT Ret_Data );

	int GetProfiles( bool isMain, ONVIF_GetProfiles_C_DATE* _OUT Ret_Data );

	int SetSystemTimeAndDate( const ONVIF_C_TimeAndDate* _OUT Ret_Data );

	int GetSystemTimeAndDate( ONVIF_C_TimeAndDate* _OUT Ret_Data );

	int PTZ_GetNodeToken( char* _OUT Ret_GetNodeToken );

	int PTZ_GetPresetToken(void);

	void PTZ_SetPreset(void);

	int PTZ_GotoPreset(void);

	int PTZ_AbsoluteMove(void);

	int PTZ_Stop(void);

	//uSpeed 范围1-7
	int PTZ_ContinuousMove( UInt8 uType, UInt8 uSpeed = 0 );

	const char* GetServiceAddr( void ) const
	{
		return _Service_Addr;
	}

	bool is_open( void ) const
	{
		return strcmp(_Service_Addr, "") == 0 ? false : true;
	}

private:
	char _UserName[100];
	char _Password[100];
	char _Service_Addr[200];  //设备的地址
	char _PTZ_Addr[200];      //设备的云台地址
	bool _isPTZ;
	char _Media_Addr[200];
	char _MediaUri[200];
	char _ProfilesToken[200];
	char _PTZPresetToken[200];
	HSOAP	_hSoap;
};
#endif