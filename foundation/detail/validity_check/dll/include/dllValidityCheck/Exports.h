
#ifndef __EXPORTS_20161110182842_1451301281_H__
#define __EXPORTS_20161110182842_1451301281_H__

#include "External.h"

/*****************************************************************
/*ԭ��������	�Ƿ��ļ��ص�����
/*����˵����	c_szFilePath: �Ƿ��ļ�·��
/*				iValue: ������
/*				
/*����ֵ��		true:�ײ������		false:�û����д���
*****************************************************************/
typedef bool (_CALLTYPE *fpnMValidity_CheckFaild)( const char* c_szFilePath,
												int iValue );


/*****************************************************************
/*����˵����	�ļ��Ϸ�����֤
/*����˵����	fpnFaild: �Ƿ��ļ��ص�����
/*				
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MValidity_Check( fpnMValidity_CheckFaild fpnFaild );

#endif//__PLC_OMRON_20151228191441_1451301281_H__
