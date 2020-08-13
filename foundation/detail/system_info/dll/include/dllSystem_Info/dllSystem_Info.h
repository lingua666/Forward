
#ifndef __SYSTEM_INFO_20160328140444_1459145084_H__
#define __SYSTEM_INFO_20160328140444_1459145084_H__

#include <dllCommon/DllCommon.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

struct tagCpu_INFO
{
	char* pSerialNumber;	//CPU序列号
	char* pVendor;			//CPU厂家
	char* pBrand;			//CPU型号
};

struct tagDisk_INFO
{
	char* pManufacturer;	//制造商名称
	char* pModel;			//磁盘驱动器的制造商的型号
	char* pSerialNumber;	//磁盘序列号
	char* pInterfaceType;	//硬件接口类型  IDE、SATA
	Int64 uSize;			//硬盘大小
};

typedef void (_CALLTYPE *fpnMGetDiskInfo)(tagDisk_INFO* pInfo);


/*****************************************************************
/*函数说明：	获取CPU信息
/*参数说明：	
/*				
/*返回值：		CPU信息结构体
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS tagCpu_INFO* _CALLTYPE GetCpuInfo( void );


/*****************************************************************
/*函数说明：	获取硬盘信息
/*参数说明：	pFunc:  硬盘信息回调函数
/*				
/*返回值：		CPU信息结构体
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MGetDiskInfo( fpnMGetDiskInfo pFunc );


#endif//__COMMON_20160328140444_1459145084_H__
