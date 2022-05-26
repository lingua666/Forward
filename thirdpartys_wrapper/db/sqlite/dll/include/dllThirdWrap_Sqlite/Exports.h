
#ifndef __EXPORTS_20160105191441_1451301281_H__
#define __EXPORTS_20160105191441_1451301281_H__

#include "External.h"

#ifndef DLL_INIT_FLAG_DEF
	#define DLL_INIT_FLAG_DEF
	DLL_INIT_DECL_DEF()
#endif

DLL_LOG_DECL_DEF(SQLite_Module)

/*****************************************************************
/*函数说明：	连接SQLite服务器
/*参数说明：	szPath: 文件路径
/*返回值：		true: 成功   false: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE MSQLite_Connect( const char* szPath );


/*****************************************************************
/*函数说明：	关闭数据库连接
/*参数说明：
/*				
/*返回值：		true: 成功   false: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE MSQLite_DisConnect( void );


/*****************************************************************
/*函数说明：	执行SQL语句
/*参数说明：	szSQL:	 SQL语句
/*				
/*返回值：		true: 成功   false: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE MSQLite_ExecuteSQL( const char* szSQL );


/*****************************************************************
/*函数说明：	数据库查询
/*参数说明：	szSQL:	 SQL语句
/*				
/*返回值：		记录集句柄:    0: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS HMRecordSet _CALLTYPE MSQLite_Query_Open( const char* szSQL );


/*****************************************************************
/*函数说明：	数据库查询
/*参数说明：	RSet:	 记录集句柄
/*				
/*返回值：		1：	成功 
				<=0：错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLite_Query_Close( HMRecordSet RSet );


/*****************************************************************
/*函数说明：	第一条记录集
/*参数说明：	RSet:	 记录集句柄
/*				
/*返回值：		1：	成功 
				<=0：错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLite_Query_MoveFirst( HMRecordSet RSet );


/*****************************************************************
/*函数说明：	下一条记录集
/*参数说明：	RSet:	 记录集句柄
/*				
/*返回值：		1：	成功 
				<=0：错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLite_Query_MoveNext( HMRecordSet RSet );


/*****************************************************************
/*函数说明：	最后一条记录集
/*参数说明：	RecordSet:	 记录集句柄
/*				
/*返回值：		1：	成功 
				<=0：错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLite_Query_MoveLast( HMRecordSet RSet );

/*****************************************************************
/*函数说明：	获取值
/*参数说明：	RSet:	 记录集句柄
/*				u32Col:	 列数
/*返回值：		1：	成功 
				<=0：错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS const char* _CALLTYPE MSQLite_Query_GetValue( HMRecordSet RSet,
																		UInt32 u32Col );


/*****************************************************************
/*函数说明：	获取值
/*参数说明：	RSet:	 记录集句柄
/*			c_szColName:字段名
/*返回值：	1：	成功
<=0：错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS const char* _CALLTYPE MSQLite_QueryN_GetValue(HMRecordSet RSet,
	const char* c_szColName);


/*****************************************************************
/*函数说明：	获取记录集行数
/*参数说明：	RSet:	 记录集句柄
/*返回值：		行数
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLite_Query_RowCount( HMRecordSet RSet );


/*****************************************************************
/*函数说明：	获取记录集列数
/*参数说明：	RSet:	 记录集句柄
/*返回值：		列数
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLite_Query_ColumnCount( HMRecordSet RSet );


/*****************************************************************
/*函数说明：	将结果转换为int
/*参数说明：	pValue:	 值指针
/*返回值：		结果
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLite_Query_ValueToInt( const char* pValue );


/*****************************************************************
/*函数说明：	将结果转换为long
/*参数说明：	pValue:	 值指针
/*返回值：		结果
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS long _CALLTYPE MSQLite_Query_ValueToLong( const char* pValue );


/*****************************************************************
/*函数说明：	将结果转换为float
/*参数说明：	pValue:	 值指针
/*返回值：		结果
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS float _CALLTYPE MSQLite_Query_ValueToFloat( const char* pValue );


/*****************************************************************
/*函数说明：	将结果转换为double
/*参数说明：	pValue:	 值指针
/*返回值：		结果
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS double _CALLTYPE MSQLite_Query_ValueToDouble( const char* pValue );


/*****************************************************************
/*函数说明：	开始事务
/*参数说明：	
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE MSQLite_BeginTransaction( void );


/*****************************************************************
/*函数说明：	提交事务
/*参数说明：	
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE MSQLite_CommitTransaction( void );


/*****************************************************************
/*函数说明：	事务回滚
/*参数说明：	
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE MSQLite_RollBack( void );


#endif//__PLC_OMRON_20151228191441_1451301281_H__
