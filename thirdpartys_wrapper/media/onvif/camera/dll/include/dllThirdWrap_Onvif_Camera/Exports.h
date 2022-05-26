
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
	Int64 Handle;            //�豸��½���
	char* PDevUID;           //�豸��UID��
	Int64 DevIndex;          //�豸��������
};

struct	tagHaiK_Time
{
	int Year;		//��
	int Month;		//��
	int Day;		//��
	int Hour;		//ʱ
	int Minute;		//��
	int Second;		//��
};

struct tagHaiK_Record
{
	THandle	Handle;		//���
	int		Channel;	//ͨ��
	char*	PDevUID;	//�豸UID
	char*	PDevType;	//�豸����
	Int64	DevIndex;	//����
};

struct tagHaiK_RecordFileInfo
{
	char* szFileName;	//�ļ���
	Int64 iFileSize;	//�ļ�����
	int	  iStreamType;	//������
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
	PTZ_TILT_UP = 0,	//��̨����
	PTZ_TILT_DOWN,		//��̨�¸�
	PTZ_PAN_LEFT,		//��̨��ת
	PTZ_PAN_RIGHT,		//��̨��ת
	PTZ_UP_LEFT,		//��̨��������ת
	PTZ_UP_RIGHT,		//��̨��������ת
	PTZ_DOWN_LEFT,		//��̨�¸�����ת
	PTZ_DOWN_RIGHT,		//��̨�¸�����ת
	PTZ_PAN_AUTO,		//��̨�����Զ�ɨ��
	PTZ_ZOOM_IN,		//����
	PTZ_ZOOM_OUT,		//Զ��
	PTZ_FOCUS_NEAR,		//����ǰ��
	PTZ_FOCUS_FAR,		//������
	PTZ_IRIS_OPEN,		//��Ȧ����
	PTZ_IRIS_CLOSE,		//��Ȧ��С
	PTZ_COUNT_CONTROL
};

enum PTZ_RESET_TYPE
{
	PTZ_SET_PRESET = 0,	//����Ԥ�õ�
	PTZ_CLE_PRESET,		//���Ԥ�õ�
	PTZ_GOTO_PRESET,	//ת��Ԥ�õ�
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
/*����˵����	��ʼ��(�ڵ��ú����ӿ�ǰ�����)
/*����˵����	fpnExcept: �쳣�ص�����
				fpnLoginResult: ��¼����ص�����
/*				
/*����ֵ��		>0: �ɹ�   <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_Init( fpnExceptionCallBack fpnExcept,
															fpnLoginResultCB fpnLoginResult );


/*****************************************************************
/*����˵����	�ͷ�ģ����Դ(�ڲ�ʹ�ø�ģ��ʱʹ��)
/*����˵����	
/*				
/*����ֵ��		>0: �ɹ�   <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_Free( void );


/*****************************************************************
/*����˵����	��¼(�첽����)
/*����˵����	pInfo: ��¼��Ϣ�ṹ��ָ��
				pUser: �û�����ָ��
/*����ֵ��		�豸���	1:�ɹ�	<0:ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_Login( tagHaiK_LoginInfo* pInfo,
															THK_DevLoginInfo* pUser );


/*****************************************************************
/*����˵����	ע��
/*����˵����	Handle: �豸���
/*				
/*����ֵ��		>0: �ɹ�   <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_Loginout( THandle Handle );


/*****************************************************************
/*����˵����	�����豸
/*����˵����	Handle�� �豸���
/*				
/*����ֵ��		>0: �ɹ�   <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_Reboot( THandle Handle );


/*****************************************************************
/*����˵����	��ȡͨ����
/*����˵����	Handle: �豸���
/*				
/*����ֵ��		>0: �ɹ�   <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_GetChannelCount( THandle Handle );


/*****************************************************************
/*����˵����	��ʼԤ��
/*����˵����	Handle: �豸���
				uChannel: ͨ����
				hWnd:	��ʾ���
/*				
/*����ֵ��		>0: �ɹ�   <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_StartPreview( THandle Handle, int uChannel, THandle hWnd );


/*****************************************************************
/*����˵����	ֹͣԤ��
/*����˵����	Handle: �豸���
				uChannel: ͨ����
/*				
/*����ֵ��		>0: �ɹ�   <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_StopPreview( THandle Handle, int uChannel );


/*****************************************************************
/*����˵����	����¼��
/*����˵����	pRecord: ¼����Ϣ�ṹ��ָ��
				pStartTime: ��ʼʱ��			
				pStopTime: ֹͣʱ��
				pfnRecord: ¼��֪ͨ�ص�����
/*				
/*����ֵ��		>0: �ɹ�   <0: ������
/*ע��:			�޷���������װ��һ���ṹ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_FindRecord( tagHaiK_Record* pRecord, tagHaiK_Time* pStartTime,
																	tagHaiK_Time* pStopTime, pfnHaiK_Record pfnRecord );


/*****************************************************************
/*����˵����	¼��ط�
/*����˵����	pPRecord: ¼����Ϣ�ṹ��ָ��
				hWnd: ��ʾ���
				RecordFile:¼���ļ���
/*				
/*����ֵ��		>0: �ɹ�   <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_PlayRecord( tagHaiK_Record* pRecord, THandle hWnd, char* RecordFile );


/*****************************************************************
/*����˵����	����¼��ʱ��ط�
/*����˵����	pPRecord: ¼����Ϣ�ṹ��ָ��
				hWnd: ��ʾ���
				pStartTime:��ʼʱ��
				pStartTime:����ʱ��
/*				
/*����ֵ��		>0: �ɹ�   <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_PlayRecord_ByTimer( tagHaiK_Record* pRecord,
																	THandle hWnd,
																	tagHaiK_Time* pStartTime,
																	tagHaiK_Time* pStopTime );


/*****************************************************************
/*����˵����	¼��ط�λ��(�����ļ��طţ�ʱ��ط����ϴ󣬽���ʹ��MThirdW_Haik_PlayPosition_ByTime�ӿھ�ȷ��λʱ��)
/*����˵����	pPRecord: ¼����Ϣ�ṹ��ָ��
				hWnd: ��ʾ���
/*				uPostion: ¼��λ��   (��Χ0-100)
/*����ֵ��		>0: �ɹ�   <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_PlayPosition( tagHaiK_Record* pRecord, THandle hWnd, int uPostion );


/*****************************************************************
/*����˵����	¼��ط�ʱ�䶨λ
/*����˵����	pPRecord: ¼����Ϣ�ṹ��ָ��
				hWnd: ��ʾ���
/*				pTime: ʱ��ṹ��ָ��
/*����ֵ��		>0: �ɹ�   <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_PlayPosition_ByTime( tagHaiK_Record* pRecord, THandle hWnd, tagHaiK_Time* pTime );


/*****************************************************************
/*����˵����	¼�񲥷��ٶ�
/*����˵����	pPRecord: ¼����Ϣ�ṹ��ָ��
				hWnd: ��ʾ���
				uSpeedType: 0: �첥   1:����
/*				
/*����ֵ��		>0: �ɹ�   <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_PlaySpeed( tagHaiK_Record* pRecord, THandle hWnd, int uSpeedType );


/*****************************************************************
/*����˵����	��ͣ¼��ط�
/*����˵����	pPRecord: ¼����Ϣ�ṹ��ָ��
				hWnd: ��ʾ���
/*				
/*����ֵ��		>0: �ɹ�   <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_PauseRecord( tagHaiK_Record* pRecord, THandle hWnd );


/*****************************************************************
/*����˵����	�ָ�¼��ط�
/*����˵����	pPRecord: ¼����Ϣ�ṹ��ָ��
				hWnd: ��ʾ���
/*				
/*����ֵ��		>0: �ɹ�   <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_ResumeRecord( tagHaiK_Record* pRecord, THandle hWnd );


/*****************************************************************
/*����˵����	ֹͣ¼��ط�
/*����˵����	pPRecord: ¼����Ϣ�ṹ��ָ��
				hWnd: ��ʾ���
/*				
/*����ֵ��		>0: �ɹ�   <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_StopRecord( tagHaiK_Record* pRecord, THandle hWnd );


/*****************************************************************
/*����˵����	�϶�ʱ��
/*����˵����	Handle: �豸ʱ��
				pTime: ʱ��ṹ��ָ��
/*				
/*����ֵ��		>0: �ɹ�   <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_SetTimerCFG( THandle Handle, tagHaiK_Time* pTime );


/*****************************************************************
/*����˵����	��̨Ԥ�õ�
/*����˵����	pReset: ��̨Ԥ�ýṹ��ָ��
/*				
/*����ֵ��		>0: �ɹ�   <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_PTZPreset( tagHaiK_PTZReset* pReset );


/*****************************************************************
/*����˵����	��̨����
/*����˵����	pControl����̨���ƽṹ��ָ��
				Speed:  �ٶ� ȡֵ��Χ(1-7)
/*				
/*����ֵ��		>0: �ɹ�   <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_PTZControl( tagHaiK_PTZControl* pControl, int Speed );


/*****************************************************************
/*����˵����	ֹͣ��̨����
/*����˵����	pControl����̨���ƽṹ��ָ��
				Speed:  �ٶ� ȡֵ��Χ(1-7)
/*				
/*����ֵ��		>0: �ɹ�   <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_PTZControl_Stop( tagHaiK_PTZControl* pControl, int Speed );


/*****************************************************************
/*����˵����	���ۿ���
/*����˵����	pControl����̨���ƽṹ��ָ��
/*				
/*����ֵ��		>0: �ɹ�   <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_PTZFocus( tagHaiK_PTZControl* pControl );

/*****************************************************************
/*����˵����	ֹͣ���ۿ���
/*����˵����	pControl����̨���ƽṹ��ָ��
/*				
/*����ֵ��		>0: �ɹ�   <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MThirdW_Haik_PTZFocus_Stop( tagHaiK_PTZControl* pControl );


#endif//__PLC_OMRON_20151228191441_1451301281_H__
