
#ifndef __EXPORTS_20170726153904_1451301281_H__
#define __EXPORTS_20170726153904_1451301281_H__

#include "External.h"

#ifndef DLL_INIT_FLAG_DEF
	#define DLL_INIT_FLAG_DEF
	DLL_INIT_DECL_DEF()
#endif

DLL_LOG_DECL_DEF(SQLServer_Module)

/*****************************************************************
/*����˵����	�������ݿ������
/*����˵����	szHost:	������IP
/*				uPort:  �������˿ں�
				szUserName: �û���
				szPwd:		����
/*����ֵ��		true: �ɹ�   false: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE MSQLServer_Connect( const char* szHost, UInt16 uPort, 
															const char* szUserName, const char* szPwd,
															const char* szDataBase );


/*****************************************************************
/*����˵����	�ر����ݿ�����
/*����˵����
/*				
/*����ֵ��		true: �ɹ�   false: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE MSQLServer_DisConnect( void );


/*****************************************************************
/*����˵����	ִ��SQL���
/*����˵����	szSQL:	 SQL���
/*				
/*����ֵ��		true: �ɹ�   false: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE MSQLServer_ExecuteSQL( const char* szSQL );


/*****************************************************************
/*����˵����	���ݿ��ѯ
/*����˵����	szSQL:	 SQL���
/*				
/*����ֵ��		��¼�����:    0: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS HMRecordSet _CALLTYPE MSQLServer_Query_Open( const char* szSQL );


/*****************************************************************
/*����˵����	���ݿ��ѯ
/*����˵����	RSet:	 ��¼�����
/*				
/*����ֵ��		1��	�ɹ� 
				<=0��������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLServer_Query_Close( HMRecordSet RSet );


/*****************************************************************
/*����˵����	��һ����¼��
/*����˵����	RSet:	 ��¼�����
/*				
/*����ֵ��		1��	�ɹ� 
				<=0��������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLServer_Query_MoveFirst( HMRecordSet RSet );


/*****************************************************************
/*����˵����	��һ����¼��
/*����˵����	RSet:	 ��¼�����
/*				
/*����ֵ��		1��	�ɹ� 
				<=0��������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLServer_Query_MoveNext( HMRecordSet RSet );


/*****************************************************************
/*����˵����	���һ����¼��
/*����˵����	RecordSet:	 ��¼�����
/*				
/*����ֵ��		1��	�ɹ� 
				<=0��������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLServer_Query_MoveLast( HMRecordSet RSet );

/*****************************************************************
/*����˵����	��ȡֵ
/*����˵����	RSet:	 ��¼�����
/*				u32Col:	 ����
/*����ֵ��		1��	�ɹ� 
				<=0��������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS const char* _CALLTYPE MSQLServer_Query_GetValue( HMRecordSet RSet,
																		UInt32 u32Col );


/*****************************************************************
/*����˵����	��ȡ��¼������
/*����˵����	RSet:	 ��¼�����
/*����ֵ��		����
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLServer_Query_RowCount( HMRecordSet RSet );


/*****************************************************************
/*����˵����	��ȡ��¼������
/*����˵����	RSet:	 ��¼�����
/*����ֵ��		����
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLServer_Query_ColumnCount( HMRecordSet RSet );


/*****************************************************************
/*����˵����	�����ת��Ϊint
/*����˵����	pValue:	 ֵָ��
/*����ֵ��		���
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLServer_Query_ValueToInt( const char* pValue );


/*****************************************************************
/*����˵����	�����ת��Ϊlong
/*����˵����	pValue:	 ֵָ��
/*����ֵ��		���
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS long _CALLTYPE MSQLServer_Query_ValueToLong( const char* pValue );


/*****************************************************************
/*����˵����	�����ת��Ϊfloat
/*����˵����	pValue:	 ֵָ��
/*����ֵ��		���
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS float _CALLTYPE MSQLServer_Query_ValueToFloat( const char* pValue );


/*****************************************************************
/*����˵����	�����ת��Ϊdouble
/*����˵����	pValue:	 ֵָ��
/*����ֵ��		���
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS double _CALLTYPE MSQLServer_Query_ValueToDouble( const char* pValue );


#endif//__PLC_OMRON_20151228191441_1451301281_H__
