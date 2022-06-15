
#ifndef __APPS_ERRORCODE_20160612182129_1464344489_H__
#define __APPS_ERRORCODE_20160612182129_1464344489_H__

#include <libApps_ErrorCode/External.h>

static	Int64 g_Error20160612[] = {
									0x0,		//主消息中心
									0x00010000,	//备份消息中心
									0x00020000,	//主接入服务器
									0x00030000,	//备接入服务器
									0x00040000,	//帐号服务器
									0x00050000,	//GIS服务器
									0x00060000,	//流媒体服务器
									0x00070000,	//用户服务器
									0x00080000,	//设备管理服务器
									0x00090000,	//文件服务器
									0x000A0000, //设备资源服务器
									0x000B0000, //报警服务器
									0x000C0000, //数据库服务器
									0x000D0000, //日志服务器
									0x000E0000, //计费服务器
									0x000F0000, //运维服务器
									0x00100000,	//消息通知服务器
									0x00200000, //视频管理服务器
									0x00300000, //欧姆龙PLC中间件管理服务器
								};

#define APPS_DATA_FORMAT_ERROR				0xFF000001

////////////////////////////////////////////////主消息中心////////////////////////////////////////////////
#define	APPS_CENTRE_SVR_NOT_BACKUP_SVR			0x00000000
#define	APPS_CENTRE_SVR_NOT_DEST_INDEX			0x00000001
#define	APPS_CENTRE_SVR_NOT_DEST_UID			0x00000002
#define	APPS_CENTRE_SVR_BACKUP_CENTRE_EXSIT		0x00000003
#define	APPS_CENTRE_SVR_INVAILD_UID				0x00000004
#define	APPS_CENTRE_SVR_NOT_ONLINE				0x00000005
#define	APPS_CENTRE_SVR_USER_EXIST				0x00000006



////////////////////////////////////////////////备用消息中心////////////////////////////////////////////////
#define	APPS_BACKUP_CENTRE_SVR_NOT_DEST_INDEX		0x00010000
#define	APPS_BACKUP_CENTRE_SVR_NOT_DEST_UID			0x00010001
#define	APPS_BACKUP_CENTRE_SVR_BACKUP_CENTRE_EXSIT	0x00010002
#define	APPS_BACKUP_CENTRE_SVR_INVAILD_UID			0x00010003
#define	APPS_BACKUP_CENTRE_SVR_NOT_ONLINE			0x00010004


////////////////////////////////////////////////消息通知服务器////////////////////////////////////////////////
#define APPS_NOTIFY_SVR_NULL					0x00100000
#define APPS_NOTIFY_SVR_NOT_READY				0x00100001
#define APPS_NOTIFY_SVR_NOT_FOUND				0x00100002



////////////////////////////////////////////////帐号服务器////////////////////////////////////////////////
#define APPS_ACCOUNT_SVR_NULL					0x00100000
#define APPS_ACCOUNT_SVR_NOT_READY				0x00100001
#define APPS_ACCOUNT_SVR_NOT_FOUND				0x00100002



////////////////////////////////////////////////主接入服务器////////////////////////////////////////////////
#define APPS_ACCESS_SVR_NULL					0x00020000
#define APPS_ACCESS_SVR_NOT_READY				0x00020001


////////////////////////////////////////////////设备管理服务器////////////////////////////////////////////////
#define APPS_DEVICE_MANAGE_LV1_SVR_NULL					0x00080000
#define APPS_DEVICE_MANAGE_LV1_SVR_NOT_READY			0x00080001
#define APPS_DEVICE_MANAGE_LV1_SVR_NOT_FOUND			0x00080002
#define APPS_DEVICE_MANAGE_LV1_SVR_NOT_MIDWARE			0x00080003


////////////////////////////////////////////////视频管理服务器LV1////////////////////////////////////////////////
#define APPS_MEDIA_LV1_SVR_NULL					0x00200000
#define APPS_MEDIA_LV1_NOT_READY				0x00200001

void Apps_SetErrorCode( int iErrorCode );

int Apps_GetErrorCode( void );

bool Apps_EqualErrorCode( int iErrorCode );

_string_type Apps_FromErrorToString( int iErrorCode );

_string_type Apps_GetErrorToString( void );

#endif //__ERRORCODE_20160612182129_1464344489_H__
