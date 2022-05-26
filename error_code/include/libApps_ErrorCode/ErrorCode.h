
#ifndef __APPS_ERRORCODE_20160612182129_1464344489_H__
#define __APPS_ERRORCODE_20160612182129_1464344489_H__

#include <libApps_ErrorCode/External.h>

static	Int64 g_Error20160612[] = {
									0x0,		//����Ϣ����
									0x00010000,	//������Ϣ����
									0x00020000,	//�����������
									0x00030000,	//�����������
									0x00040000,	//�ʺŷ�����
									0x00050000,	//GIS������
									0x00060000,	//��ý�������
									0x00070000,	//�û�������
									0x00080000,	//�豸���������
									0x00090000,	//�ļ�������
									0x000A0000, //�豸��Դ������
									0x000B0000, //����������
									0x000C0000, //���ݿ������
									0x000D0000, //��־������
									0x000E0000, //�Ʒѷ�����
									0x000F0000, //��ά������
									0x00100000,	//��Ϣ֪ͨ������
									0x00200000, //��Ƶ���������
									0x00300000, //ŷķ��PLC�м�����������
								};

#define APPS_DATA_FORMAT_ERROR				0xFF000001

////////////////////////////////////////////////����Ϣ����////////////////////////////////////////////////
#define	APPS_CENTRE_SVR_NOT_BACKUP_SVR			0x00000000
#define	APPS_CENTRE_SVR_NOT_DEST_INDEX			0x00000001
#define	APPS_CENTRE_SVR_NOT_DEST_UID			0x00000002
#define	APPS_CENTRE_SVR_BACKUP_CENTRE_EXSIT		0x00000003
#define	APPS_CENTRE_SVR_INVAILD_UID				0x00000004
#define	APPS_CENTRE_SVR_NOT_ONLINE				0x00000005
#define	APPS_CENTRE_SVR_USER_EXIST				0x00000006



////////////////////////////////////////////////������Ϣ����////////////////////////////////////////////////
#define	APPS_BACKUP_CENTRE_SVR_NOT_DEST_INDEX		0x00010000
#define	APPS_BACKUP_CENTRE_SVR_NOT_DEST_UID			0x00010001
#define	APPS_BACKUP_CENTRE_SVR_BACKUP_CENTRE_EXSIT	0x00010002
#define	APPS_BACKUP_CENTRE_SVR_INVAILD_UID			0x00010003
#define	APPS_BACKUP_CENTRE_SVR_NOT_ONLINE			0x00010004


////////////////////////////////////////////////��Ϣ֪ͨ������////////////////////////////////////////////////
#define APPS_NOTIFY_SVR_NULL					0x00100000
#define APPS_NOTIFY_SVR_NOT_READY				0x00100001
#define APPS_NOTIFY_SVR_NOT_FOUND				0x00100002



////////////////////////////////////////////////�ʺŷ�����////////////////////////////////////////////////
#define APPS_ACCOUNT_SVR_NULL					0x00100000
#define APPS_ACCOUNT_SVR_NOT_READY				0x00100001
#define APPS_ACCOUNT_SVR_NOT_FOUND				0x00100002



////////////////////////////////////////////////�����������////////////////////////////////////////////////
#define APPS_ACCESS_SVR_NULL					0x00020000
#define APPS_ACCESS_SVR_NOT_READY				0x00020001


////////////////////////////////////////////////�豸���������////////////////////////////////////////////////
#define APPS_DEVICE_MANAGE_LV1_SVR_NULL					0x00080000
#define APPS_DEVICE_MANAGE_LV1_SVR_NOT_READY			0x00080001
#define APPS_DEVICE_MANAGE_LV1_SVR_NOT_FOUND			0x00080002
#define APPS_DEVICE_MANAGE_LV1_SVR_NOT_MIDWARE			0x00080003


////////////////////////////////////////////////��Ƶ���������LV1////////////////////////////////////////////////
#define APPS_MEDIA_LV1_SVR_NULL					0x00200000
#define APPS_MEDIA_LV1_NOT_READY				0x00200001

void Apps_SetErrorCode( int iErrorCode );

int Apps_GetErrorCode( void );

bool Apps_EqualErrorCode( int iErrorCode );

_string_type Apps_FromErrorToString( int iErrorCode );

_string_type Apps_GetErrorToString( void );

#endif //__ERRORCODE_20160612182129_1464344489_H__
