
#ifndef __SYSTEM_INFO_20160328140444_1459145084_H__
#define __SYSTEM_INFO_20160328140444_1459145084_H__

#include <dllCommon/DllCommon.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

struct tagCpu_INFO
{
	char* pSerialNumber;	//CPU���к�
	char* pVendor;			//CPU����
	char* pBrand;			//CPU�ͺ�
};

struct tagDisk_INFO
{
	char* pManufacturer;	//����������
	char* pModel;			//�����������������̵��ͺ�
	char* pSerialNumber;	//�������к�
	char* pInterfaceType;	//Ӳ���ӿ�����  IDE��SATA
	Int64 uSize;			//Ӳ�̴�С
};

typedef void (_CALLTYPE *fpnMGetDiskInfo)(tagDisk_INFO* pInfo);


/*****************************************************************
/*����˵����	��ȡCPU��Ϣ
/*����˵����	
/*				
/*����ֵ��		CPU��Ϣ�ṹ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS tagCpu_INFO* _CALLTYPE GetCpuInfo( void );


/*****************************************************************
/*����˵����	��ȡӲ����Ϣ
/*����˵����	pFunc:  Ӳ����Ϣ�ص�����
/*				
/*����ֵ��		CPU��Ϣ�ṹ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MGetDiskInfo( fpnMGetDiskInfo pFunc );


#endif//__COMMON_20160328140444_1459145084_H__
