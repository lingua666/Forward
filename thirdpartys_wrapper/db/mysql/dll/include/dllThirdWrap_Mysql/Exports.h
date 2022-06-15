
#ifndef __EXPORTS_20151228191441_1451301281_H__
#define __EXPORTS_20151228191441_1451301281_H__

#include "External.h"

#ifndef DLL_INIT_FLAG_DEF
	#define DLL_INIT_FLAG_DEF
	DLL_INIT_DECL_DEF()
#endif

DLL_LOG_DECL_DEF(Mysql_Module)

/*****************************************************************
/*����˵����	��ʼ��MYSQL ���ýӿڱ�����Mysql_Connectǰ����)
/*����˵����	isReconnect: �Ƿ�����
				u32ConnectTimeOut: ��ʱ�೤ʱ������   ��λ/����
/*				
/*����ֵ��		true: �ɹ�   false: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE Mysql_Init( bool isReconnect = true,
													UInt32 u32ConnectTimeOut = 60 );

/*****************************************************************
/*����˵�����ͷ�MYSQL
/*����˵����	
/*
/*����ֵ��		true: �ɹ�   false: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE Mysql_Release(void);


/*****************************************************************
/*����˵����	����MYSQL������
/*����˵����	szHost:	 MYSQL������IP
/*				u16Port: MYSQL�������˿ں�
				szUserName: �û���
				szPwd:		����
				szDBName:   ���ݿ���
				u8Characters: �ַ���
							0: GB2312
							1: GBK
							2: UTF8
/*����ֵ��		true: �ɹ�   false: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE Mysql_Connect(
							const char* szHost, UInt16 u16Port,
							const char* szUserName, const char* szPwd,
							const char* szDBName, UInt8 u8Characters );


/*****************************************************************
/*����˵����	�ر����ݿ�����
/*����˵����
/*				
/*����ֵ��		true: �ɹ�   false: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE Mysql_DisConnect( void );


/*****************************************************************
/*����˵����	�������ݿ�
/*����˵����	szDBName:	 ���ݿ���
/*				
/*����ֵ��		true: �ɹ�   false: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE Mysql_CreateDB( const char* szDBName );


/*****************************************************************
/*����˵����	ɾ�����ݿ�
/*����˵����	szDBName:	 ���ݿ���
/*				
/*����ֵ��		true: �ɹ�   false: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE Mysql_DeleteDB( const char* szDBName );


/*****************************************************************
/*����˵����	ִ��SQL���
/*����˵����	szSQL:	 SQL���
/*				
/*����ֵ��		true: �ɹ�   false: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE Mysql_ExecuteSQL( const char* szSQL );


/*****************************************************************
/*����˵����	���ݿ��ѯ
/*����˵����	szSQL:	 SQL���
/*				
/*����ֵ��		��¼�����:    0: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS HMRecordSet _CALLTYPE Mysql_Query_Open( const char* szSQL );


/*****************************************************************
/*����˵����	���ݿ��ѯ
/*����˵����	RSet:	 ��¼�����
/*				
/*����ֵ��		1��	�ɹ� 
				<=0��������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE Mysql_Query_Close( HMRecordSet RSet );


/*****************************************************************
/*����˵����	��һ����¼��
/*����˵����	RSet:	 ��¼�����
/*				
/*����ֵ��		1��	�ɹ� 
				<=0��������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE Mysql_Query_MoveFirst( HMRecordSet RSet );


/*****************************************************************
/*����˵����	��һ����¼��
/*����˵����	RSet:	 ��¼�����
/*				
/*����ֵ��		1��	�ɹ� 
				<=0��������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE Mysql_Query_MoveNext( HMRecordSet RSet );


/*****************************************************************
/*����˵����	���һ����¼��
/*����˵����	RecordSet:	 ��¼�����
/*				
/*����ֵ��		1��	�ɹ� 
				<=0��������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE Mysql_Query_MoveLast( HMRecordSet RSet );

/*****************************************************************
/*����˵����	��ȡֵ
/*����˵����	RSet:	 ��¼�����
/*				u32Col:	 ����
/*����ֵ��		�գ��ɹ� 
				�ǿգ��ɹ�
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS const char* _CALLTYPE Mysql_Query_GetValue( HMRecordSet RSet,
																		UInt32 u32Col );


/*****************************************************************
/*����˵����	��ȡ��¼������
/*����˵����	RSet:	 ��¼�����
/*����ֵ��		����
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE Mysql_Query_RowCount( HMRecordSet RSet );


/*****************************************************************
/*����˵����	��ȡ��¼������
/*����˵����	RSet:	 ��¼�����
/*����ֵ��		����
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE Mysql_Query_ColumnCount( HMRecordSet RSet );


/*****************************************************************
/*����˵����	��Mysql_Query_GetValue()��ȡ�Ľ��ת��Ϊint
/*����˵����	pValue:	 ֵָ��
/*����ֵ��		���
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE Mysql_Query_ValueToInt( const char* pValue );


/*****************************************************************
/*����˵����	��Mysql_Query_GetValue()��ȡ�Ľ��ת��Ϊlong
/*����˵����	pValue:	 ֵָ��
/*����ֵ��		���
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS long _CALLTYPE Mysql_Query_ValueToLong( const char* pValue );


/*****************************************************************
/*����˵����	��Mysql_Query_GetValue()��ȡ�Ľ��ת��Ϊfloat
/*����˵����	pValue:	 ֵָ��
/*����ֵ��		���
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS float _CALLTYPE Mysql_Query_ValueToFloat( const char* pValue );


/*****************************************************************
/*����˵����	��Mysql_Query_GetValue()��ȡ�Ľ��ת��Ϊdouble
/*����˵����	pValue:	 ֵָ��
/*����ֵ��		���
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS double _CALLTYPE Mysql_Query_ValueToDouble( const char* pValue );


#endif//__PLC_OMRON_20151228191441_1451301281_H__
