
#ifndef __EXTERNAL_20180327231042_1451301281_H__
#define __EXTERNAL_20180327231042_1451301281_H__

#include <dllCommon/DllCommon.h>

//笔记本信息结构体
struct IOTPenBook_INFO
{
	const char*	c_szName;	//名称
	int PX;					//X 方向页数
	int PY;					//Y 方向页数
	double SX;				//X 方向起点
	double SY;				//Y 方向起点
	double LX;				//X 单页长度
	double LY;				//Y 单页长度
	int PageWidth;			//纸张宽度
	int PageHeight;			//纸张高度
	int EntityWidth;		//实体纸宽
	int EntityHeight;		//实体纸高
};

//笔记本数据解析结果结构体
struct IOTPenParse_INFO
{
	int   iValid;	// 有效位  
	// 0: 无效
	// 1: 有效
	int	  iType;	// 数据类型
	// 0: OID1.5
	// 1: OID2
	// 2: OID3
	// 3: Position Code
	int   iSAR;		//SAR 暂时没有使用
	int	  rX;		//原始X坐标
	int	  rY;		//原始Y坐标
	int   iIndex;	//数据传输索引 值范围 0-255 循环使用
	int   pointZ;	//压力值
	double fX;		//实际纸张X坐标
	double fY;		//实际纸张Y坐标
	int iPage;		//页数
};


/*****************************************************************
/* 回调函数原型说明：	测试数据回调函数（功能等同蓝牙基础模块中设置的接收回调函数）
/*参数说明：	pData：数据内容（等同从蓝牙接收到的数据）
/*				iLen: 数据内容长度
/*
/*返回值：
/*注意:			该回调函数中的数据参数已经将文件中的坐标转换成智能笔的原始蓝牙数据，
/*              也就是10个字节的数据，需要调用正式接口MIOTPen_Proto_Parse解析出坐标数据
*****************************************************************/
//typedef void (_CALLTYPE *fpnMTPen_Proto_Test_RecData)(const char* pData, int iLen);



/*****************************************************************
/* 回调函数原型说明：	测试数据回调函数（功能等同蓝牙基础模块中设置的接收回调函数）
/*参数说明：	pData：数据内容（等同从蓝牙接收到的数据）
/*				iLen: 数据内容长度
/*
/*返回值：
/*注意:			该回调函数中的数据参数已经将文件中的坐标转换成智能笔的原始蓝牙数据，
/*              也就是10个字节的数据，需要调用正式接口MIOTPen_Proto_Parse解析出坐标数据
*****************************************************************/
typedef void (_CALLTYPE *fpnMTPen_Proto_Test_ParseData)(IOTPenParse_INFO* pRetData);


/*****************************************************************
/*函数说明：	解析数据
/*参数说明：	fnRec: 数据接收回调函数
/*返回值：		1:成功(解析出有效数据)  <0: 错误码			
*****************************************************************/
//EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MIOTPen_Proto_Test_SetRecv( fpnMTPen_Proto_Test_RecData fnRec );


/*****************************************************************
/*函数说明：	解析数据
/*参数说明：	fnRec: 数据接收回调函数
/*返回值：		1:成功(解析出有效数据)  <0: 错误码			
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MIOTPen_Proto_Test_SetParse( fpnMTPen_Proto_Test_ParseData fnRec );


/*****************************************************************
/*函数说明：	解析数据
/*参数说明：	c_szFilePath: 测试文件路径
/*返回值：		1:成功(解析出有效数据)  <0: 错误码			
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MIOTPen_Proto_Test_Parse( const char* c_szFilePath );

#endif//__EXTERNAL_20151014105604_3442199361_H__