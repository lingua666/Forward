
#ifndef __EXPORTS_20151228191441_1451301281_H__
#define __EXPORTS_20151228191441_1451301281_H__

#include "External.h"
#include <Windows.h>

struct tagHaiK_LoginInfo
{
	char*	IP;
	int		Port;
	char*	UserName;
	char*	Password;
};

struct THK_DevLoginInfo
{
	Int64 Handle;            //设备登陆结果
	char* PDevUID;           //设备的UID号
	Int64 DevIndex;          //设备的索引号
};

struct	tagHaiK_Time
{
	int Year;		//年
	int Month;		//月
	int Day;		//日
	int Hour;		//时
	int Minute;		//分
	int Second;		//秒
};

struct tagHaiK_Record
{
	THandle	Handle;		//句柄
	int		Channel;	//通道
	char*	PDevUID;	//设备UID
	char*	PDevType;	//设备类型
	Int64	DevIndex;	//索引
};

struct tagHaiK_RecordFileInfo
{
	char* szFileName;	//文件名
	Int64 iFileSize;	//文件长度
	int	  iStreamType;	//流类型
};

struct tagHaiK_PTZControl
{
	THandle Handle;
	int  Channel;
	int  PTZCmd;
};

struct tagHaiK_PTZReset
{
	THandle Handle;
	int  Channel;
	int  PTZPresetCmd;
};

enum PLAY_SPEED_TYPE
{
	PLAY_FAST_SPEED = 0,
	PLAT_SLOW_SPEED,
	PLAT_COUNT_SPEED
};

enum PTZ_CONTROL_TYPE
{
	PTZ_TILT_UP = 0,	//云台上仰
	PTZ_TILT_DOWN,		//云台下俯
	PTZ_PAN_LEFT,		//云台左转
	PTZ_PAN_RIGHT,		//云台右转
	PTZ_UP_LEFT,		//云台上仰和左转
	PTZ_UP_RIGHT,		//云台上仰和右转
	PTZ_DOWN_LEFT,		//云台下俯和左转
	PTZ_DOWN_RIGHT,		//云台下俯和右转
	PTZ_PAN_AUTO,		//云台左右自动扫描
	PTZ_ZOOM_IN,		//近焦
	PTZ_ZOOM_OUT,		//远焦
	PTZ_FOCUS_NEAR,		//焦点前调
	PTZ_FOCUS_FAR,		//焦点后调
	PTZ_IRIS_OPEN,		//光圈扩大
	PTZ_IRIS_CLOSE,		//光圈缩小
	PTZ_COUNT_CONTROL
};

enum PTZ_RESET_TYPE
{
	PTZ_SET_PRESET = 0,	//设置预置点
	PTZ_CLE_PRESET,		//清除预置点
	PTZ_GOTO_PRESET,	//转到预置点
	PTZ_COUNT_PRESET
};

typedef void (CALLBACK *fpnExceptionCallBack)( THandle dwType, THandle Handle );

typedef	void (CALLBACK *fpnLoginResultCB)( THK_DevLoginInfo* pInfo );

typedef	void (CALLBACK *pfnHaiK_Record)( tagHaiK_RecordFileInfo*, tagHaiK_Time*,
										tagHaiK_Time*, tagHaiK_Record* );

#ifndef DLL_INIT_FLAG_DEF
	#define DLL_INIT_FLAG_DEF
	DLL_INIT_DECL_DEF()
#endif


DLL_LOG_DECL_DEF(ThirdW_Haik_Module)

/*****************************************************************
/*函数说明：	初始化(在调用后续接口前请调用)
/*参数说明：	fpnExcept: 异常回调函数
				fpnLoginResult: 登录结果回调函数
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_Init( fpnExceptionCallBack fpnExcept,
															fpnLoginResultCB fpnLoginResult );


/*****************************************************************
/*函数说明：	释放模块资源(在不使用该模块时使用)
/*参数说明：	
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_Free( void );


/*****************************************************************
/*函数说明：	登录(异步操作)
/*参数说明：	pInfo: 登录信息结构体指针
				pUser: 用户数据指针
/*返回值：		设备句柄	1:成功	<0:失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_Login( tagHaiK_LoginInfo* pInfo,
															THK_DevLoginInfo* pUser );


/*****************************************************************
/*函数说明：	注销
/*参数说明：	Handle: 设备句柄
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_Loginout( THandle Handle );


/*****************************************************************
/*函数说明：	重启设备
/*参数说明：	Handle： 设备句柄
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_Reboot( THandle Handle );


/*****************************************************************
/*函数说明：	获取通道数
/*参数说明：	Handle: 设备句柄
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_GetChannelCount( THandle Handle );


/*****************************************************************
/*函数说明：	开始预览
/*参数说明：	Handle: 设备句柄
				uChannel: 通道号
				hWnd:	显示句柄
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_StartPreview( THandle Handle, int uChannel, THandle hWnd );


/*****************************************************************
/*函数说明：	停止预览
/*参数说明：	Handle: 设备句柄
				uChannel: 通道号
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_StopPreview( THandle Handle, int uChannel );


/*****************************************************************
/*函数说明：	查找录像
/*参数说明：	pRecord: 录像信息结构体指针
				pStartTime: 起始时间			
				pStopTime: 停止时间
				pfnRecord: 录像通知回调函数
/*				
/*返回值：		>0: 成功   <0: 错误码
/*注意:			无法将参数封装到一个结构体里面
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_FindRecord( tagHaiK_Record* pRecord, tagHaiK_Time* pStartTime,
																	tagHaiK_Time* pStopTime, pfnHaiK_Record pfnRecord );


/*****************************************************************
/*函数说明：	录像回放
/*参数说明：	pPRecord: 录像信息结构体指针
				hWnd: 显示句柄
				RecordFile:录像文件名
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_PlayRecord( tagHaiK_Record* pRecord, THandle hWnd, char* RecordFile );


/*****************************************************************
/*函数说明：	根据录像时间回放
/*参数说明：	pPRecord: 录像信息结构体指针
				hWnd: 显示句柄
				pStartTime:起始时间
				pStartTime:结束时间
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_PlayRecord_ByTimer( tagHaiK_Record* pRecord,
																	THandle hWnd,
																	tagHaiK_Time* pStartTime,
																	tagHaiK_Time* pStopTime );


/*****************************************************************
/*函数说明：	录像回放位置(适用文件回放，时间回放误差较大，建议使用MThirdW_Haik_PlayPosition_ByTime接口精确定位时间)
/*参数说明：	pPRecord: 录像信息结构体指针
				hWnd: 显示句柄
/*				uPostion: 录像位置   (范围0-100)
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_PlayPosition( tagHaiK_Record* pRecord, THandle hWnd, int uPostion );


/*****************************************************************
/*函数说明：	录像回放时间定位
/*参数说明：	pPRecord: 录像信息结构体指针
				hWnd: 显示句柄
/*				pTime: 时间结构体指针
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_PlayPosition_ByTime( tagHaiK_Record* pRecord, THandle hWnd, tagHaiK_Time* pTime );


/*****************************************************************
/*函数说明：	录像播放速度
/*参数说明：	pPRecord: 录像信息结构体指针
				hWnd: 显示句柄
				uSpeedType: 0: 快播   1:慢播
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_PlaySpeed( tagHaiK_Record* pRecord, THandle hWnd, int uSpeedType );


/*****************************************************************
/*函数说明：	暂停录像回放
/*参数说明：	pPRecord: 录像信息结构体指针
				hWnd: 显示句柄
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_PauseRecord( tagHaiK_Record* pRecord, THandle hWnd );


/*****************************************************************
/*函数说明：	恢复录像回放
/*参数说明：	pPRecord: 录像信息结构体指针
				hWnd: 显示句柄
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_ResumeRecord( tagHaiK_Record* pRecord, THandle hWnd );


/*****************************************************************
/*函数说明：	停止录像回放
/*参数说明：	pPRecord: 录像信息结构体指针
				hWnd: 显示句柄
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_StopRecord( tagHaiK_Record* pRecord, THandle hWnd );


/*****************************************************************
/*函数说明：	较对时间
/*参数说明：	Handle: 设备时间
				pTime: 时间结构体指针
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_SetTimerCFG( THandle Handle, tagHaiK_Time* pTime );


/*****************************************************************
/*函数说明：	云台预置点
/*参数说明：	pReset: 云台预置结构体指针
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_PTZPreset( tagHaiK_PTZReset* pReset );


/*****************************************************************
/*函数说明：	云台控制
/*参数说明：	pControl：云台控制结构体指针
				Speed:  速度 取值范围(1-7)
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_PTZControl( tagHaiK_PTZControl* pControl, int Speed );


/*****************************************************************
/*函数说明：	停止云台控制
/*参数说明：	pControl：云台控制结构体指针
				Speed:  速度 取值范围(1-7)
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_PTZControl_Stop( tagHaiK_PTZControl* pControl, int Speed );


/*****************************************************************
/*函数说明：	焦聚控制
/*参数说明：	pControl：云台控制结构体指针
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_PTZFocus( tagHaiK_PTZControl* pControl );

/*****************************************************************
/*函数说明：	停止焦聚控制
/*参数说明：	pControl：云台控制结构体指针
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_PTZFocus_Stop( tagHaiK_PTZControl* pControl );


#endif//__PLC_OMRON_20151228191441_1451301281_H__
