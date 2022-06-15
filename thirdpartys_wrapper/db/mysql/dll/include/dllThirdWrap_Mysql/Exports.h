
#ifndef __EXPORTS_20151228191441_1451301281_H__
#define __EXPORTS_20151228191441_1451301281_H__

#include "External.h"

#ifndef DLL_INIT_FLAG_DEF
	#define DLL_INIT_FLAG_DEF
	DLL_INIT_DECL_DEF()
#endif

DLL_LOG_DECL_DEF(Mysql_Module)

/*****************************************************************
/*函数说明：	初始化MYSQL （该接口必须在Mysql_Connect前调用)
/*参数说明：	isReconnect: 是否重连
				u32ConnectTimeOut: 超时多长时间重连   单位/毫秒
/*				
/*返回值：		true: 成功   false: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE Mysql_Init( bool isReconnect = true,
													UInt32 u32ConnectTimeOut = 60 );

/*****************************************************************
/*函数说明：释放MYSQL
/*参数说明：	
/*
/*返回值：		true: 成功   false: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE Mysql_Release(void);


/*****************************************************************
/*函数说明：	连接MYSQL服务器
/*参数说明：	szHost:	 MYSQL服务器IP
/*				u16Port: MYSQL服务器端口号
				szUserName: 用户名
				szPwd:		密码
				szDBName:   数据库名
				u8Characters: 字符集
							0: GB2312
							1: GBK
							2: UTF8
/*返回值：		true: 成功   false: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE Mysql_Connect(
							const char* szHost, UInt16 u16Port,
							const char* szUserName, const char* szPwd,
							const char* szDBName, UInt8 u8Characters );


/*****************************************************************
/*函数说明：	关闭数据库连接
/*参数说明：
/*				
/*返回值：		true: 成功   false: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE Mysql_DisConnect( void );


/*****************************************************************
/*函数说明：	创建数据库
/*参数说明：	szDBName:	 数据库名
/*				
/*返回值：		true: 成功   false: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE Mysql_CreateDB( const char* szDBName );


/*****************************************************************
/*函数说明：	删除数据库
/*参数说明：	szDBName:	 数据库名
/*				
/*返回值：		true: 成功   false: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE Mysql_DeleteDB( const char* szDBName );


/*****************************************************************
/*函数说明：	执行SQL语句
/*参数说明：	szSQL:	 SQL语句
/*				
/*返回值：		true: 成功   false: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE Mysql_ExecuteSQL( const char* szSQL );


/*****************************************************************
/*函数说明：	数据库查询
/*参数说明：	szSQL:	 SQL语句
/*				
/*返回值：		记录集句柄:    0: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS HMRecordSet _CALLTYPE Mysql_Query_Open( const char* szSQL );


/*****************************************************************
/*函数说明：	数据库查询
/*参数说明：	RSet:	 记录集句柄
/*				
/*返回值：		1：	成功 
				<=0：错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE Mysql_Query_Close( HMRecordSet RSet );


/*****************************************************************
/*函数说明：	第一条记录集
/*参数说明：	RSet:	 记录集句柄
/*				
/*返回值：		1：	成功 
				<=0：错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE Mysql_Query_MoveFirst( HMRecordSet RSet );


/*****************************************************************
/*函数说明：	下一条记录集
/*参数说明：	RSet:	 记录集句柄
/*				
/*返回值：		1：	成功 
				<=0：错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE Mysql_Query_MoveNext( HMRecordSet RSet );


/*****************************************************************
/*函数说明：	最后一条记录集
/*参数说明：	RecordSet:	 记录集句柄
/*				
/*返回值：		1：	成功 
				<=0：错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE Mysql_Query_MoveLast( HMRecordSet RSet );

/*****************************************************************
/*函数说明：	获取值
/*参数说明：	RSet:	 记录集句柄
/*				u32Col:	 列数
/*返回值：		空：成功 
				非空：成功
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS const char* _CALLTYPE Mysql_Query_GetValue( HMRecordSet RSet,
																		UInt32 u32Col );


/*****************************************************************
/*函数说明：	获取记录集行数
/*参数说明：	RSet:	 记录集句柄
/*返回值：		行数
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE Mysql_Query_RowCount( HMRecordSet RSet );


/*****************************************************************
/*函数说明：	获取记录集列数
/*参数说明：	RSet:	 记录集句柄
/*返回值：		列数
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE Mysql_Query_ColumnCount( HMRecordSet RSet );


/*****************************************************************
/*函数说明：	将Mysql_Query_GetValue()获取的结果转换为int
/*参数说明：	pValue:	 值指针
/*返回值：		结果
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE Mysql_Query_ValueToInt( const char* pValue );


/*****************************************************************
/*函数说明：	将Mysql_Query_GetValue()获取的结果转换为long
/*参数说明：	pValue:	 值指针
/*返回值：		结果
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS long _CALLTYPE Mysql_Query_ValueToLong( const char* pValue );


/*****************************************************************
/*函数说明：	将Mysql_Query_GetValue()获取的结果转换为float
/*参数说明：	pValue:	 值指针
/*返回值：		结果
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS float _CALLTYPE Mysql_Query_ValueToFloat( const char* pValue );


/*****************************************************************
/*函数说明：	将Mysql_Query_GetValue()获取的结果转换为double
/*参数说明：	pValue:	 值指针
/*返回值：		结果
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS double _CALLTYPE Mysql_Query_ValueToDouble( const char* pValue );


#endif//__PLC_OMRON_20151228191441_1451301281_H__
