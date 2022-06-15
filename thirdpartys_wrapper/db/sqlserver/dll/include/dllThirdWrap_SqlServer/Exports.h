
#ifndef __EXPORTS_20170726153904_1451301281_H__
#define __EXPORTS_20170726153904_1451301281_H__

#include "External.h"

#ifndef DLL_INIT_FLAG_DEF
	#define DLL_INIT_FLAG_DEF
	DLL_INIT_DECL_DEF()
#endif

DLL_LOG_DECL_DEF(SQLServer_Module)

/*****************************************************************
/*函数说明：	连接数据库服务器
/*参数说明：	szHost:	服务器IP
/*				uPort:  服务器端口号
				szUserName: 用户名
				szPwd:		密码
/*返回值：		true: 成功   false: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE MSQLServer_Connect( const char* szHost, UInt16 uPort, 
															const char* szUserName, const char* szPwd,
															const char* szDataBase );


/*****************************************************************
/*函数说明：	关闭数据库连接
/*参数说明：
/*				
/*返回值：		true: 成功   false: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE MSQLServer_DisConnect( void );


/*****************************************************************
/*函数说明：	执行SQL语句
/*参数说明：	szSQL:	 SQL语句
/*				
/*返回值：		true: 成功   false: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE MSQLServer_ExecuteSQL( const char* szSQL );


/*****************************************************************
/*函数说明：	数据库查询
/*参数说明：	szSQL:	 SQL语句
/*				
/*返回值：		记录集句柄:    0: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS HMRecordSet _CALLTYPE MSQLServer_Query_Open( const char* szSQL );


/*****************************************************************
/*函数说明：	数据库查询
/*参数说明：	RSet:	 记录集句柄
/*				
/*返回值：		1：	成功 
				<=0：错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLServer_Query_Close( HMRecordSet RSet );


/*****************************************************************
/*函数说明：	第一条记录集
/*参数说明：	RSet:	 记录集句柄
/*				
/*返回值：		1：	成功 
				<=0：错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLServer_Query_MoveFirst( HMRecordSet RSet );


/*****************************************************************
/*函数说明：	下一条记录集
/*参数说明：	RSet:	 记录集句柄
/*				
/*返回值：		1：	成功 
				<=0：错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLServer_Query_MoveNext( HMRecordSet RSet );


/*****************************************************************
/*函数说明：	最后一条记录集
/*参数说明：	RecordSet:	 记录集句柄
/*				
/*返回值：		1：	成功 
				<=0：错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLServer_Query_MoveLast( HMRecordSet RSet );

/*****************************************************************
/*函数说明：	获取值
/*参数说明：	RSet:	 记录集句柄
/*				u32Col:	 列数
/*返回值：		1：	成功 
				<=0：错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS const char* _CALLTYPE MSQLServer_Query_GetValue( HMRecordSet RSet,
																		UInt32 u32Col );


/*****************************************************************
/*函数说明：	获取记录集行数
/*参数说明：	RSet:	 记录集句柄
/*返回值：		行数
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLServer_Query_RowCount( HMRecordSet RSet );


/*****************************************************************
/*函数说明：	获取记录集列数
/*参数说明：	RSet:	 记录集句柄
/*返回值：		列数
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLServer_Query_ColumnCount( HMRecordSet RSet );


/*****************************************************************
/*函数说明：	将结果转换为int
/*参数说明：	pValue:	 值指针
/*返回值：		结果
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLServer_Query_ValueToInt( const char* pValue );


/*****************************************************************
/*函数说明：	将结果转换为long
/*参数说明：	pValue:	 值指针
/*返回值：		结果
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS long _CALLTYPE MSQLServer_Query_ValueToLong( const char* pValue );


/*****************************************************************
/*函数说明：	将结果转换为float
/*参数说明：	pValue:	 值指针
/*返回值：		结果
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS float _CALLTYPE MSQLServer_Query_ValueToFloat( const char* pValue );


/*****************************************************************
/*函数说明：	将结果转换为double
/*参数说明：	pValue:	 值指针
/*返回值：		结果
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS double _CALLTYPE MSQLServer_Query_ValueToDouble( const char* pValue );


#endif//__PLC_OMRON_20151228191441_1451301281_H__
