
#ifndef __FRAMEWORK_MODBUS_20170621191441_1451301281_H__
#define __FRAMEWORK_MODBUS_20170621191441_1451301281_H__

#include "External.h"

#ifndef DLL_INIT_FLAG_DEF
	#define DLL_INIT_FLAG_DEF
DLL_INIT_DECL_DEF()
#endif

DLL_LOG_DECL_DEF(dllFramework_Modbus);


/*****************************************************************
/*����˵����	��ʼ��modbus tcpģ��
/*����˵����
/*
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MModbus_Init_Tcp(UInt16 uIOThreadNum = 1, UInt8 uProcThreadNum = 1, bool isSequence = true);


/*****************************************************************
/*����˵����	��ʼ��modbus tcpģ��
/*����˵����
/*
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MModbus_Init_Rtu(UInt16 uIOThreadNum = 1, UInt8 uProcThreadNum = 1);


/*****************************************************************
/*����˵����	�ͷ�modbusģ����Դ(�ڲ�ʹ�ø�ģ��ʱʹ��)
/*����˵����
/*
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MModbus_Free(void);


#endif//__PLC_OMRON_20151228191441_1451301281_H__
