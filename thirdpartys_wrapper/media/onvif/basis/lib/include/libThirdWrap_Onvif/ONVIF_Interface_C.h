#ifndef _ONVIF_INTERFACE_C_H_  
#define _ONVIF_INTERFACE_C_H_

#define _OUT
#define _IN

#ifdef __cplusplus
extern "C" {
#endif

	typedef	void*	HSOAP;

	enum ONVIF_PTZ_CONTROL_TYPE
	{
		PTZ_TILT_UP = 0,	//云台上仰
		PTZ_TILT_DOWN,		//云台下俯
		PTZ_PAN_LEFT,		//云台左转
		PTZ_PAN_RIGHT,		//云台右转
		PTZ_UP_LEFT,		//云台上仰和左转
		PTZ_UP_RIGHT,		//云台上仰和右转
		PTZ_DOWN_LEFT,		//云台下俯和左转
		PTZ_DOWN_RIGHT,		//云台下俯和右转
		PTZ_PAN_AUTO,		//云台左右自动扫描(无效)
		PTZ_ZOOM_IN,		//近焦
		PTZ_ZOOM_OUT,		//远焦
		PTZ_FOCUS_NEAR,		//焦点前调(无效)
		PTZ_FOCUS_FAR,		//焦点后调(无效)
		PTZ_IRIS_OPEN,		//光圈扩大(无效)
		PTZ_IRIS_CLOSE,		//光圈缩小(无效)
		PTZ_COUNT_CONTROL
	};

//*******************接收返回数据的结构体****如果需要返回多个参数就要定义一个结构体来接收数据*****************************

#pragma pack(push, 1)
	typedef struct tagOnvif_DiscoveryInfo_C
	{
		char IP[20];				//设备ip
		char UUID[200];				//设备UUID
		char Service_Addr[200];		//设备服务地址
		char Device_t[200];			//设备类型
		char Manufacturer[200];		//厂商
		char EP_Address[200];		// EP Address
	}tagOnvif_DiscoveryInfo;

	typedef struct tagOnvif_DevInfo_C
	{
		char Manufacturer[200];		//厂商
		char HardwareId[200];		//硬件ID
		char FirmwareVersion[200];	//固件版本
		char SerialNumber[200];		//序列号
	}tagOnvif_DevInfo;

	typedef struct ONVIF_GetProfiles_Date
	{
		//尽量不使用指针，涉及到生命周期的问题，会释放掉，改为数组比较安全
		char VideoSourcesToken[1024];                    //视频源的令牌
		char ProfilesToken[1024];                        //Profiles文件信息的令牌

	}ONVIF_GetProfiles_C_DATE;

	typedef struct tagOnvif_TimeAndDate
	{
		int Hour;
		int Minute;
		int Second;
		int Year;
		int Month;
		int Day;
	}ONVIF_C_TimeAndDate;

	typedef struct ONVIF_PTZ_GetPresets
	{
		char PresetToken[1024];                       //预设位的令牌
		char PresetName[1024];                        //预设位的名字

	}ONVIF_PTZ_GetPresets_C_DATE;

	typedef struct ONVIF_PTZ_GetConfigurations
	{
		char ConfigurationToken[1024];                 //Configuration的令牌
		char TimeOut[1024];                            //超时时间
		char spcae[1024];                             //速度空间

	}ONVIF_PTZ_GetConfigurations_C_DATE;

	typedef struct ONVIF_GetCapabilities
	{
		int isPTZ;                   //判断是不是球机的标志位，0为不是，1位是；
		char Media_addr[1024];       //媒体地址
		char PTZ_addr[1024];         //设备云台地址

	}ONVIF_GetCapabilities_C_DATE;

#pragma pack(pop)

typedef int (*fpnOnvif_DiscoveryCallBack)( const tagOnvif_DiscoveryInfo* pDiscoveryInfo,
										void* pUser );

HSOAP	ONVIF_malloc( const char* c_pUserName, const char* c_pPassword );

void	ONVIF_free( HSOAP hSoap );

int		ONVIF_SetUserPwd( HSOAP hSoap, const char* c_pUserName, const char* c_pPassword );

//**************************************************************************onvif接口：begin*******************************
//获取设备信息，这里的解析数据只适用于海康的设备。

//设备发现，获取设备的服务地址,ret_server_addr是用来接收返回数据的 RecvTimeout:单位秒
int ONVIF_ClientDiscovery_C( int RecvTimeout,
						fpnOnvif_DiscoveryCallBack fpnDiscovery,
						void* pUser,
						const char* szIP );

int ONVIF_GetDevInformation_C( const char* pDevice_t, const char* pService_Addr,
							const char* pUserName, const char* pPassword,
							tagOnvif_DevInfo* _OUT DevInfo );

//获取设备的能力值，使用的是设备地址，也就是服务器，获取媒体信息地址提供给GetProfiles接口使用,ret_data存放返回数据的结构体
int ONVIF_GetCapabilities_C( HSOAP hSoap, const char *pService_Addr,
	const char* pUserName,
	const char* pPassword,
	ONVIF_GetCapabilities_C_DATE* _OUT Ret_Data );

//获取设备服务，使用的是设备服务地址
int ONVIF_GetService_C( HSOAP hSoap, const char *pService_Addr,
	const char* pUserName,
	const char* pPassword );

//获取媒体的信息文件，使用的是媒体信息地址，识别主，子通道的编码分辨率，提供给GetStreamuri接口使用, ret_data返回GetProfiles的数据结构体
int ONVIF_GetProfiles_C( HSOAP hSoap, const char* Media_Addr,
	const char* pUserName,
	const char* pPassword,
	unsigned char isMain, //true: 主码流  false: 子码流
	ONVIF_GetProfiles_C_DATE* _OUT Ret_Data );

//获取流媒体的地址，使用的是媒体信息地址，提供给RTSP获取视频流，但是要在uri地址上加上用户名和密码， ret_MediaUri存放返回的流媒体的地址
int ONVIF_GetUri_C( HSOAP hSoap, const char *Media_Addr, 
	const char* Profiles_token,
	const char* pUserName,
	const char* pPassword,
	char* _OUT Ret_MediaUri);

//获取视频源，使用的是媒体信息地址Media_Addr
int ONVIF_GetRecordingSummary_C( const char *MediaUri, 
	const char* pUserName,
	const char* pPassword );

//获取视频源的信息， 使用的是媒体地址和视频源的token
int ONVIF_GetRecordingInformation_C( const char *MediaUri,
	const char* pUserName,
	const char* pPassword );

//获取视频的输出集，使用的是流媒体的地址
int ONVIF_GetVideoSources_C( HSOAP hSoap, const char *Media_Addr );

int ONVIF_SetSystemTimeAndDate_C(HSOAP hSoap, const char *pService_Addr,
					const char* pUserName, const char* pPassword,
					const ONVIF_C_TimeAndDate* pTimeAndDate );

int ONVIF_GetSystemTimeAndDate_C( HSOAP hSoap, const char *pService_Addr,
							ONVIF_C_TimeAndDate* pTimeAndDate );

//获取云台的设备的状态，uir地址用的是具备云台功能的服务器地址 ，使用的是ProfilesToken 和设备的云台服务地址，ps:"http://192.168.8.23/onvif/PTZ"
int ONVIF_GetVideoSourceConfiguration_C(const char *Media_Addr,
	const char *VideoSourcesToken,
	const char* pUserName,
	const char* pPassword);

int ONVIF_GetVideoOutputs_C(const char *Media_Addr,
	const char* pUserName,
	const char* pPassword );


//获取云台的设备的状态，地址用的是具备云台功能的服务器地址 ，使用的是ProfilesToken 和设备的云台服务地址，ps:"http://192.168.8.23/onvif/PTZ"
int ONVIF_PTZ_GetStatus_C( const char *ProfilesToken,
	const char *pService_Addr,
	const char* pUserName,
	const char* pPassword );

//获取云台设备的nodes值，地址用的是具备云台功能的服务器地址  ps:"http://192.168.8.23/onvif/PTZ",ret_GetNodeToken存放返回的NodeToken
int ONVIF_PTZ_GetNodes_C( HSOAP hSoap, const char *pService_Addr,
	char* _OUT Ret_GetNodeToken );

 
//获取云台设备的node值, 地址用的是具备云台功能的服务器地址  ps:"http://192.168.8.23/onvif/PTZ"
int ONVIF_PTZ_GetNode_C( HSOAP hSoap, const char *Token,
	const char *pService_Addr,
	const char* pUserName,
	const char* pPassword );


//获取设备云台的Presets值,地址用的是具备云台功能的服务器地址  ps:"http://192.168.8.23/onvif/PTZ"  ret_data用于返回的数据的结构体
int ONVIF_PTZ_GetPresets_C( HSOAP hSoap, const char *ProfileToken,
	const char *pService_Addr,
	const char* pUserName,
	const char* pPassword,
	ONVIF_PTZ_GetPresets_C_DATE* _OUT Ret_Data);


//设置设备云台的Preset值，地址用的是具备云台功能的服务器地址  ps:"http://192.168.8.23/onvif/PTZ"  ret_PresetToken用于返回的数据
//这个接口用的是GetPresets返回的信息来填充结构体的，
int ONVIF_PTZ_SetPreset_C( const char *ProfileToken, 
	const char *PresetTokenm,
	const char *PresetName, 
	const char *pService_Addr,
	const char* pUserName,
	const char* pPassword,
	char* _OUT Ret_PresetToken );

//回到预设点，这个接口用GetPresets 或者是SetPreset 返回的信息来填充结构体  ,移除预设（RemovePreset）也是和GotoPreset一样
//通过GetPresets 或者是SetPreset来获取和设置预设点之后再用GotoPreset回到预设点，前提是，在你设置好预设点之后，球机要变化当前位置，调用该接口才会生效，之后才会回到预设点
int ONVIF_PTZ_GotoPreset_C( HSOAP hSoap, const char *ProfileToken,
	const char *PresetToken, 
	const char *pService_Addr,
	const char* pUserName,
	const char* pPassword );

//获取设备云台的全部信息，使用的是设备云台的服务地址,ret_data 用于返回数据的结构体
int ONVIF_PTZ_GetConfigurations_C( const char *pService_Addr, 
	const char* pUserName,
	const char* pPassword,
	ONVIF_PTZ_GetConfigurations_C_DATE *Ret_Data );

//获取设备云台的的输出信息，使用的ConfigurationToken 和设备云台的服务地址
int ONVIF_PTZ_GetConfigurationOptions_C( const char *ConfigurationToken,
	const char *pService_Addr,
	const char* pUserName,
	const char* pPassword );

//云台设备的绝对移动， 使用的设备的ProfileToken 和设备云台的服务地址
int ONVIF_PTZ_AbsoluteMove_C( HSOAP hSoap, const char *ProfileToken,
	const char *pService_Addr,
	const char* pUserName,
	const char* pPassword );

//云台设备的连续移动，使用的设备的ProfileToken 和设备云台的服务地址
int ONVIF_PTZ_ContinuousMove_C( HSOAP hSoap, const char *ProfileToken,
	const char *pService_Addr,
	const char* pUserName,
	const char* pPassword,
	unsigned char uType, unsigned char uSpeed );

//云台设备的停止移动指令，使用的设备的ProfileToken 和设备云台的服务地址
int ONVIF_PTZ_Stop_C( HSOAP hSoap, const char *ProfileToken, 
	const char *pService_Addr,
	const char* pUserName,
	const char* pPassword );

//设置云台设备的家位置点， 使用的设备的ProfileToken 和设备云台的服务地址
int ONVIF_PTZ_SetHomePosition_C( const char *ProfileToken,
	const char *pService_Addr,
	const char* pUserName,
	const char* pPassword );

//回到云台设备的家位置点， 使用的设备的ProfileToken 和设备云台的服务地址
int ONVIF_PTZ_GotoHomePosition_C( const char *ProfileToken,
	const char *pService_Addr,
	const char* pUserName,
	const char* pPassword );


//**************************************************************************onvif接口：End********************

#ifdef  __cplusplus
}
#endif

#endif