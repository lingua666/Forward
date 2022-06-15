
#include <dllThirdWrap_SDKHaiK_Camera/Exports.h>
#include <libThirdWrap_SDKHaiK_Camera/HaiK_Device.h>
#include <libFoundation/modafx.h>

DLL_INIT_DEF(g_pTagTModuleInfoRecord_201609101930,
	"45000001000420160910000000013ZZZ00000000​​​​​​",
	"海康NVR二次开发模块",
	"V1.0.0.1",
	"20160910",
	"海康威视视频管理窗口")

DLL_LOG_DEF(ThirdW_Haik_Module)


typedef SmartPTR<HaiK_Device>	HaiK_Device_sptr;
typedef Container_HashMap_type<HaiK_Device*, HaiK_Device_sptr>	HaiK_Devices_map;
typedef Container_HashMap_type<long, HaiK_Device*>			HaiK_Devices_user_map;

fpnLoginResultCB	g_fpnLoginResultCB = NULL;
const UInt8 g_PTZReset_Index_20160907[] = {8, 9, 39};
const UInt8 g_PTZControl_Index_20160907[] = {21, 22, 23, 24, 25, 26, 27, 28, 29,
											11, 12, 13, 14, 15, 16};
TimerEvent	g_evTimer;

static HaiK_Devices_map* GetHaiK_DevicesList( void )
{
	static HaiK_Devices_map	_Devices;
	return &_Devices;
}

static HaiK_Devices_user_map* GetHaiK_User_DevicesList( void )
{
	static HaiK_Devices_user_map	_Devices;
	return &_Devices;
}

void CALLBACK g_ExceptionCallBack( DWORD dwType, LONG lUserID, LONG lHandle, void *pUser )
{
	HaiK_Devices_user_map* pUser_Map = GetHaiK_User_DevicesList();
	Singleton<CLock>::instance()->Lock();
	HaiK_Devices_user_map::iterator iter_n = pUser_Map->find( lUserID );
	if( iter_n != pUser_Map->end() )
	{
		THandle Handle = (THandle)iter_n->second;
		Singleton<CLock>::instance()->UnLock();
		if( pUser != NULL )
			((fpnExceptionCallBack)pUser)(dwType, Handle);
	}
	else
		Singleton<CLock>::instance()->UnLock();
}

//录像文件查找回掉函数
void g_HaiKRecordCall( pfnHaiK_Record pfnRecord, tagHaiK_Record* pRecord,
					char* szFileName, Int64 uFileSize,
					NET_DVR_TIME* pStartTimer,
					NET_DVR_TIME* pStopTimer,
					int byStreamType )
{
	if( pfnRecord )
	{
		tagHaiK_RecordFileInfo FileInfo = {0};
		FileInfo.szFileName = szFileName;
		FileInfo.iFileSize = uFileSize;
		FileInfo.iStreamType = byStreamType;

		pfnRecord(&FileInfo, (tagHaiK_Time*)pStartTimer, (tagHaiK_Time*)pStopTimer, pRecord);
	}
}

//登录
void g_HaiKLogin( tagHaiK_LoginInfo* pInfo,
				THK_DevLoginInfo* pUser )
{
	if( g_fpnLoginResultCB != NULL )
	{
		HaiK_Device_sptr sptr = HaiK_Device_sptr( new HaiK_Device() );
		if( sptr && sptr->Connect(pInfo->IP, pInfo->Port, pInfo->UserName, pInfo->Password) >= 0 )
		{
			Singleton<CLock>::instance()->Lock();
			GetHaiK_User_DevicesList()->insert( std::make_pair(sptr->GetLoginUserID(), sptr.get()) );
			GetHaiK_DevicesList()->insert( std::make_pair(sptr.get(), sptr) );
			Singleton<CLock>::instance()->UnLock();

			LOG_Print_Info(ThirdW_Haik_Module,"g_HaiKLogin() Login: THandle:%lld", (THandle)sptr.get());

			pUser->Handle = (THandle)(sptr.get());
		}
		else
		{
			pUser->Handle = -1;
			LOG_Print_Info(ThirdW_Haik_Module,"g_HaiKLogin() Login Failed!! error:%d", NET_DVR_GetLastError());
		}

		g_fpnLoginResultCB(pUser);
	}

	if( pInfo != NULL )
	{
		if( pInfo->IP != NULL )
		{
			delete pInfo->IP;
			pInfo->IP = NULL;
		}

		if( pInfo->UserName != NULL )
		{
			delete pInfo->UserName;
			pInfo->UserName = NULL;
		}

		if( pInfo->Password != NULL )
		{
			delete pInfo->Password;
			pInfo->Password = NULL;
		}

		delete pInfo;
	}

	if( pUser != NULL )
	{
		if( pUser->PDevUID != NULL )
		{
			delete pUser->PDevUID;
			pUser->PDevUID = NULL;
		}

		delete pUser;
	}
}


/*****************************************************************
/*函数说明：	初始化(在调用后续接口前请调用)
/*参数说明：	fpnExcept： 异常捕捉函数
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_Init( fpnExceptionCallBack fpnExcept,
															fpnLoginResultCB fpnLoginResult )
{
	LOG_Print_Info(ThirdW_Haik_Module,"MThirdW_Haik_Init");

	NET_DVR_SetConnectTime();

	g_fpnLoginResultCB = fpnLoginResult;
	HaiK_Device::SetExceptionCallBack_V30(g_ExceptionCallBack, (void*)fpnExcept);

	return 1;
}


/*****************************************************************
/*函数说明：	释放模块资源(在不使用该模块时使用)
/*参数说明：	
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_Free( void )
{
	LOG_Print_Info(ThirdW_Haik_Module,"MThirdW_Haik_Free");
	return 1;
}


/*****************************************************************
/*函数说明：	登录
/*参数说明：	pInfo: 登录信息结构体指针
/*返回值：		设备句柄	-1:失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_Login( tagHaiK_LoginInfo* pInfo,
															THK_DevLoginInfo* pUser )
{
	LOG_Print_Info(ThirdW_Haik_Module,"MThirdW_Haik_Login: ip:%s, port:%d, user:%s, password:%s", pInfo->IP, pInfo->Port, pInfo->UserName, pInfo->Password);
	g_evTimer.Init(1);

	tagHaiK_LoginInfo* pLogin = new tagHaiK_LoginInfo;
	pLogin->Port = pInfo->Port;
	pLogin->IP = new char[50];
	memset(pLogin->IP, 0, 50);
	strncpy(pLogin->IP, pInfo->IP, __min(strlen(pInfo->IP), 49));

	pLogin->Password = new char[50];
	memset(pLogin->Password, 0, 50);
	strncpy(pLogin->Password, pInfo->Password, __min(strlen(pInfo->Password), 49));

	pLogin->UserName = new char[50];
	memset(pLogin->UserName, 0, 50);
	strncpy(pLogin->UserName, pInfo->UserName, __min(strlen(pInfo->UserName), 49));

	THK_DevLoginInfo* pDev = new THK_DevLoginInfo;
	pDev->Handle = 0;
	pDev->DevIndex = pUser->DevIndex;
	pDev->PDevUID = new char[50];
	memset(pDev->PDevUID, 0, 50);
	strncpy(pDev->PDevUID, pUser->PDevUID, __min(strlen(pUser->PDevUID), 49));

	g_evTimer.push_back(function20_bind(g_HaiKLogin, pLogin, pDev), 0);
	return 1;
}

/*****************************************************************
/*函数说明：	注销
/*参数说明：	Handle: 设备句柄
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_Loginout( THandle Handle )
{
	LOG_Print_Info(ThirdW_Haik_Module,"MThirdW_Haik_Loginout THandle:%lld", Handle);

	HaiK_Devices_map* pMap = GetHaiK_DevicesList();
	HaiK_Devices_user_map* pUser_Map = GetHaiK_User_DevicesList();

	Singleton<CLock>::instance()->Lock();
	HaiK_Devices_map::iterator iter = pMap->find( (HaiK_Device*)Handle );
	if( iter == pMap->end() )
	{
		Singleton<CLock>::instance()->UnLock();
		return -1;
	}

	HaiK_Devices_user_map::iterator iter_n = pUser_Map->find( iter->second->GetLoginUserID() );
	if( iter_n != pUser_Map->end() )
	{
		pUser_Map->erase(iter_n);
	}

	//关闭连接
	iter->second->Close();

	//从列表中擦除
	pMap->erase(iter);

	Singleton<CLock>::instance()->UnLock();
	return 1;
}


/*****************************************************************
/*函数说明：	重启设备
/*参数说明：	Handle： 设备句柄
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_Reboot( THandle Handle )
{
	LOG_Print_Info(ThirdW_Haik_Module,"MThirdW_Haik_Loginout THandle:%lld", Handle);

	return ((HaiK_Device*)Handle)->Reboot();
}


/*****************************************************************
/*函数说明：	获取通道数
/*参数说明：	Handle: 设备句柄
/*				
/*返回值：		通道数量  -1: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_GetChannelCount( THandle Handle )
{
	LOG_Print_Info(ThirdW_Haik_Module,"MThirdW_Haik_Loginout THandle:%lld", Handle);

	return ((HaiK_Device*)Handle)->GetChannelCount();
}


/*****************************************************************
/*函数说明：	开始预览
/*参数说明：	Handle: 设备句柄
				uChannel: 通道号
				hWnd:	显示句柄
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_StartPreview( THandle Handle, int uChannel, THandle hWnd )
{
	LOG_Print_Info(ThirdW_Haik_Module,"MThirdW_Haik_StartPreview Handle:%lld, uChannel:%d, hWnd:%lld", Handle, uChannel, hWnd);

	return ((HaiK_Device*)Handle)->Start_Preview(uChannel, (HWND)hWnd);
}


/*****************************************************************
/*函数说明：	停止预览
/*参数说明：	Handle: 设备句柄
				uChannel: 通道号
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_StopPreview( THandle Handle, int uChannel )
{
	LOG_Print_Info(ThirdW_Haik_Module,"MThirdW_Haik_StopPreview Handle:%lld, uChannel:%d", Handle, uChannel);

	return ((HaiK_Device*)Handle)->Stop_Preview(uChannel);
}


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
																	tagHaiK_Time* pStopTime, pfnHaiK_Record pfnRecord )
{
	LOG_Print_Info(ThirdW_Haik_Module,"MThirdW_Haik_FindRecord Handle:%lld, Channel:%d, StartTime:%04d%02d%02d%02d%02d%02d, StopTime:%04d%02d%02d%02d%02d%02d",
		pRecord->Handle, pRecord->Channel,
		pStartTime->Year, pStartTime->Month, pStartTime->Day, pStartTime->Hour, pStartTime->Minute, pStartTime->Second,
		pStopTime->Year, pStopTime->Month, pStopTime->Day, pStopTime->Hour, pStopTime->Minute, pStopTime->Second);

	return ((HaiK_Device*)pRecord->Handle)->Nvr_FindRecord(pRecord->Channel,
												function20_bind(g_HaiKRecordCall, 
													pfnRecord, pRecord,
													_function_::_1, _function_::_2,
													_function_::_3, _function_::_4,
													_function_::_5), 
													*((NET_DVR_TIME*)pStartTime), 
													*((NET_DVR_TIME*)pStopTime));
}


/*****************************************************************
/*函数说明：	录像回放
/*参数说明：	pPRecord: 录像信息结构体指针
				hWnd: 显示句柄
				RecordFile:录像文件名
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_PlayRecord( tagHaiK_Record* pRecord, THandle hWnd, char* RecordFile )
{
	LOG_Print_Info(ThirdW_Haik_Module,"MThirdW_Haik_PlayRecord Handle:%lld, Channel:%d, hWnd:%lld, RecordFile:%s",
		pRecord->Handle, pRecord->Channel, hWnd, RecordFile);

	return ((HaiK_Device*)pRecord->Handle)->Nvr_PlayRecord(pRecord->Channel, (HWND)hWnd, RecordFile);
}

/*****************************************************************
/*函数说明：	根据录像时间回放
/*参数说明：	pPRecord: 录像信息结构体指针
				hWnd: 显示句柄
				pStartTime:起始时间
				pStartTime:结束时间
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_PlayRecord_ByTimer( tagHaiK_Record* pRecord, THandle hWnd,
																	tagHaiK_Time* pStartTime,
																	tagHaiK_Time* pStopTime )
{
	LOG_Print_Info(ThirdW_Haik_Module,"MThirdW_Haik_PlayRecord_ByTimer Handle:%lld, Channel:%d, hWnd:%lld,StartTime:%04d%02d%02d%02d%02d%02d, StopTime:%04d%02d%02d%02d%02d%02d",
		pRecord->Handle, pRecord->Channel, hWnd,
		pStartTime->Year, pStartTime->Month, pStartTime->Day, pStartTime->Hour, pStartTime->Minute, pStartTime->Second,
		pStopTime->Year, pStopTime->Month, pStopTime->Day, pStopTime->Hour, pStopTime->Minute, pStopTime->Second);

	return ((HaiK_Device*)pRecord->Handle)->Nvr_PlayBackByTime(pRecord->Channel, (HWND)hWnd,
														*((NET_DVR_TIME*)pStartTime), 
														*((NET_DVR_TIME*)pStopTime));
}

/*****************************************************************
/*函数说明：	录像回放位置
/*参数说明：	pPRecord: 录像信息结构体指针
				hWnd: 显示句柄
/*				uPostion: 录像位置   (范围0-100)
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_PlayPosition( tagHaiK_Record* pRecord, THandle hWnd, int uPostion )
{
	LOG_Print_Info(ThirdW_Haik_Module,"MThirdW_Haik_PlayPosition Handle:%lld, Channel:%d, hWnd:%lld, uPostion:%d",
		pRecord->Handle, pRecord->Channel, hWnd, uPostion);

	return ((HaiK_Device*)pRecord->Handle)->Nvr_PlayPosition(pRecord->Channel, (HWND)hWnd, uPostion);
}

/*****************************************************************
/*函数说明：	录像回放位置
/*参数说明：	pPRecord: 录像信息结构体指针
				hWnd: 显示句柄
/*				pTime: 时间结构体指针
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_PlayPosition_ByTime( tagHaiK_Record* pRecord, THandle hWnd, tagHaiK_Time* pTime )
{
	LOG_Print_Info(ThirdW_Haik_Module,"MThirdW_Haik_PlayPosition_ByTime Handle:%lld, Channel:%d, hWnd:%lld, Time:%04d%02d%02d%02d%02d%02d",
		pRecord->Handle, pRecord->Channel, hWnd,
		pTime->Year, pTime->Month, pTime->Day, pTime->Hour, pTime->Minute, pTime->Second);

	return ((HaiK_Device*)pRecord->Handle)->Nvr_PlayPosition_ByTime(pRecord->Channel, (HWND)hWnd, *((NET_DVR_TIME*)pTime));
}


/*****************************************************************
/*函数说明：	录像播放速度
/*参数说明：	pPRecord: 录像信息结构体指针
				hWnd: 显示句柄
				uSpeedType: 0: 快播   1:慢播
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_PlaySpeed( tagHaiK_Record* pRecord, THandle hWnd, int uSpeedType )
{
	LOG_Print_Info(ThirdW_Haik_Module,"MThirdW_Haik_PlaySpeed Handle:%lld, Channel:%d, hWnd:%lld, uSpeedType:%d",
		pRecord->Handle, pRecord->Channel, hWnd, uSpeedType);

	if( uSpeedType >= PLAT_COUNT_SPEED )
		return -1;

	return ((HaiK_Device*)pRecord->Handle)->Nvr_PlaySpeed(pRecord->Channel, (HWND)hWnd, uSpeedType);
}

/*****************************************************************
/*函数说明：	暂停录像回放
/*参数说明：	pPRecord: 录像信息结构体指针
				hWnd: 显示句柄
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_PauseRecord( tagHaiK_Record* pRecord, THandle hWnd )
{
	LOG_Print_Info(ThirdW_Haik_Module,"MThirdW_Haik_PauseRecord Handle:%lld, Channel:%d, hWnd:%lld",
		pRecord->Handle, pRecord->Channel, hWnd);

	return ((HaiK_Device*)pRecord->Handle)->Nvr_PauseRecord(pRecord->Channel, (HWND)hWnd);
}


/*****************************************************************
/*函数说明：	恢复录像回放
/*参数说明：	pPRecord: 录像信息结构体指针
				hWnd: 显示句柄
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_ResumeRecord( tagHaiK_Record* pRecord, THandle hWnd )
{
	LOG_Print_Info(ThirdW_Haik_Module,"MThirdW_Haik_ResumeRecord Handle:%lld, Channel:%d, hWnd:%lld",
		pRecord->Handle, pRecord->Channel, hWnd);

	return ((HaiK_Device*)pRecord->Handle)->Nvr_ResumeRecord(pRecord->Channel, (HWND)hWnd);
}


/*****************************************************************
/*函数说明：	停止录像回放
/*参数说明：	hWnd: 显示句柄
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_StopRecord( tagHaiK_Record* pRecord, THandle hWnd )
{
	LOG_Print_Info(ThirdW_Haik_Module,"MThirdW_Haik_StopRecord Handle:%lld, Channel:%d, hWnd:%lld",
		pRecord->Handle, pRecord->Channel, hWnd);

	return ((HaiK_Device*)pRecord->Handle)->Nvr_StopRecord(pRecord->Channel, (HWND)hWnd);
}


/*****************************************************************
/*函数说明：	较对时间
/*参数说明：	Handle: 设备时间
				uChannel: 通道号
				pTime: 时间结构体指针
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_SetTimerCFG( THandle Handle, tagHaiK_Time* pTime )
{
	LOG_Print_Info(ThirdW_Haik_Module,"MThirdW_Haik_SetTimerCFG Handle:%lld,pTime:%04d%02d%02d%02d%02d%02d",
		Handle, 
		pTime->Year, pTime->Month, pTime->Day, pTime->Hour, pTime->Minute, pTime->Second );

	return ((HaiK_Device*)Handle)->SetTimerCFG(*((NET_DVR_TIME*)pTime));
}


/*****************************************************************
/*函数说明：	云台预置点
/*参数说明：	pReset: 云台预置结构体指针
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_PTZPreset( tagHaiK_PTZReset* pReset  )
{
	LOG_Print_Info(ThirdW_Haik_Module,"MThirdW_Haik_PTZPreset Handle:%lld, Channel:%d, PTZPresetCmd:%d",
		pReset->Handle, pReset->Channel, pReset->PTZPresetCmd);

	if( pReset->PTZPresetCmd >= PTZ_COUNT_PRESET )
		return -1;

	return ((HaiK_Device*)pReset->Handle)->PTZPreset(pReset->Channel, g_PTZReset_Index_20160907[pReset->PTZPresetCmd]);
}


/*****************************************************************
/*函数说明：	云台控制
/*参数说明：	pControl：云台控制结构体指针
				Speed:  速度 取值范围[1,7] 
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_PTZControl( tagHaiK_PTZControl* pControl, int Speed )
{
	LOG_Print_Info(ThirdW_Haik_Module,"MThirdW_Haik_PTZControl Handle:%lld, Channel:%d, PTZCmd:%d, Speed:%d",
		pControl->Handle, pControl->Channel, pControl->PTZCmd, Speed);

	if( pControl->PTZCmd >= PTZ_COUNT_CONTROL)
		return -1;

	return ((HaiK_Device*)pControl->Handle)->PTZControlWithSpeed(pControl->Channel,
														g_PTZControl_Index_20160907[pControl->PTZCmd],
														Speed);
}

/*****************************************************************
/*函数说明：	停止云台控制
/*参数说明：	pControl：云台控制结构体指针
				Speed:  
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_PTZControl_Stop( tagHaiK_PTZControl* pControl, int Speed )
{
	LOG_Print_Info(ThirdW_Haik_Module,"MThirdW_Haik_PTZControl_Stop Handle:%lld, Channel:%d, PTZCmd:%d, Speed:%d",
		pControl->Handle, pControl->Channel, pControl->PTZCmd, Speed);

	if( pControl->PTZCmd >= PTZ_COUNT_CONTROL)
		return -1;

	return ((HaiK_Device*)pControl->Handle)->PTZControlWithSpeed_Stop(pControl->Channel,
														g_PTZControl_Index_20160907[pControl->PTZCmd],
														Speed);
}

/*****************************************************************
/*函数说明：	焦聚控制
/*参数说明：	pControl：云台控制结构体指针
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_PTZFocus( tagHaiK_PTZControl* pControl )
{
	LOG_Print_Info(ThirdW_Haik_Module,"MThirdW_Haik_PTZFocus Handle:%lld, Channel:%d, PTZCmd:%d",
		pControl->Handle, pControl->Channel, pControl->PTZCmd);

	if( pControl->PTZCmd >= PTZ_COUNT_CONTROL)
		return -1;

	return ((HaiK_Device*)pControl->Handle)->PTZControl(pControl->Channel,
		                       g_PTZControl_Index_20160907[pControl->PTZCmd]);
}

/*****************************************************************
/*函数说明：	停止焦聚控制
/*参数说明：	pControl：云台控制结构体指针
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_PTZFocus_Stop( tagHaiK_PTZControl* pControl )
{
	LOG_Print_Info(ThirdW_Haik_Module,"MThirdW_Haik_PTZFocus_Stop Handle:%lld, Channel:%d, PTZCmd:%d",
		pControl->Handle, pControl->Channel, pControl->PTZCmd);

	if( pControl->PTZCmd >= PTZ_COUNT_CONTROL)
		return -1;

	return ((HaiK_Device*)pControl->Handle)->PTZControl_Stop(pControl->Channel,
					g_PTZControl_Index_20160907[pControl->PTZCmd]);
}