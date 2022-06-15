
#include <dllThirdWrap_Sqlite/Exports.h>
#include <libThirdWrap_Sqlite/RecordSet.hpp>
#include <libThirdWrap_Sqlite/SQLiteDB.h>
#include <libFoundation/modafx.h>

using namespace _sqlite_;

MemPool_type	g_SQLite_MemPool( sizeof(RecordSet) );

DLL_INIT_DEF(g_pTagTModuleInfoRecord_201602172020,
	"45000001000W2016021700000000WZZZ00000000",
	"SQLite数据库模块",
	"V1.0.0.1",
	"20160217",
	"")

DLL_LOG_DEF(SQLite_Module)

/*****************************************************************
/*函数说明：	连接SQLite服务器
/*参数说明：	szHost:	 SQLite服务器IP
/*				u16Port: SQLite服务器端口号
				szUserName: 用户名
				szPwd:		密码
				szDBName:   数据库名
				u8Characters: 字符集
							0: GB2312
							1: GBK
							2: UTF8
/*返回值：		true: 成功   false: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE MSQLite_Connect( const char* szPath )
{
	LOG_Print_Info(SQLite_Module, "SQLite_Connect():\n\tszPath:%s",szPath)

	return Singleton<SQLiteDB>::instance()->OpenDataBase(szPath);
}

/*****************************************************************
/*函数说明：	关闭数据库连接
/*参数说明：
/*				
/*返回值：		true: 成功   false: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE MSQLite_DisConnect( void )
{
	LOG_Print_Info(SQLite_Module, "SQLite_DisConnect():")

	Singleton<SQLiteDB>::instance()->CloseDataBase();
	return true;
}

/*****************************************************************
/*函数说明：	执行SQL语句
/*参数说明：	szSQL:	 SQL语句
/*				
/*返回值：		true: 成功   false: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE MSQLite_ExecuteSQL( const char* szSQL )
{
	LOG_Print_Info(SQLite_Module, "SQLite_ExecuteSQL():\n\tszSQL:%s",szSQL)

	return Singleton<SQLiteDB>::instance()->ExcuteSql(szSQL);
}

/*****************************************************************
/*函数说明：	数据库查询
/*参数说明：	szSQL:	 SQL语句
/*				
/*返回值：		记录集句柄:    0: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS HMRecordSet _CALLTYPE MSQLite_Query_Open( const char* szSQL )
{
	LOG_Print_Info(SQLite_Module, "SQLite_Query_Open():\n\tszSQL:%s",szSQL)

	RecordSet* p = g_SQLite_MemPool.AllocObj<RecordSet>(Singleton<SQLiteDB>::instance()->GetSQLiteHandle());
	if( p->Open(szSQL) != 1 )
	{
		g_SQLite_MemPool.FreeObj(p);
		return NULL;
	}

	return p;
}

/*****************************************************************
/*函数说明：	释放记录集资源  （切记在不使用记录集的时候调用该接口，释放资源）
/*参数说明：	RecordSet:	 记录集句柄
/*				
/*返回值：		1：	成功 
				<=0：错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLite_Query_Close( HMRecordSet RSet )
{
	LOG_Print_Info(SQLite_Module, "SQLite_Query_Close():\n\tRSet:%d",RSet)

	if( RSet != NULL )
	{
		g_SQLite_MemPool.FreeObj(reinterpret_cast<RecordSet*>(RSet));
	}
	return 1;
}

/*****************************************************************
/*函数说明：	第一条记录集
/*参数说明：	RecordSet:	 记录集句柄
/*				
/*返回值：		1：	成功 
				<=0：错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLite_Query_MoveFirst( HMRecordSet RSet )
{
	LOG_Print_Info(SQLite_Module, "SQLite_Query_MoveFirst():\n\tRSet:%d",RSet)

	reinterpret_cast<RecordSet*>(RSet)->MoveFirst();
	return 1;
}


/*****************************************************************
/*函数说明：	下一条记录集
/*参数说明：	RecordSet:	 记录集句柄
/*				
/*返回值：		1：	成功 
				<=0：错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLite_Query_MoveNext( HMRecordSet RSet )
{
	LOG_Print_Info(SQLite_Module, "SQLite_Query_MoveNext():\n\tRSet:%d",RSet)

	return reinterpret_cast<RecordSet*>(RSet)->MoveNext();
}


/*****************************************************************
/*函数说明：	最后一条记录集
/*参数说明：	RecordSet:	 记录集句柄
/*				
/*返回值：		1：	成功 
				<=0：错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLite_Query_MoveLast( HMRecordSet RSet )
{
	LOG_Print_Info(SQLite_Module, "SQLite_Query_MoveLast():\n\tRSet:%d",RSet)

	reinterpret_cast<RecordSet*>(RSet)->MoveLast();
	return 1;
}


/*****************************************************************
/*函数说明：	获取值
/*参数说明：	RSet:	 记录集句柄
/*				u32Col:	 列数       从0开始
/*返回值：		1：	成功 
				<=0：错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS const char* _CALLTYPE MSQLite_Query_GetValue( HMRecordSet RSet,
																		UInt32 u32Col )
{
	LOG_Print_Info(SQLite_Module, "SQLite_Query_GetValue():\n\tRSet:%d",RSet)

	return reinterpret_cast<RecordSet*>(RSet)->GetValue(u32Col);
}

/*****************************************************************
/*函数说明：	获取值
/*参数说明：	RSet:	 记录集句柄
/*			c_szColName:字段名
/*返回值：	1：	成功
<=0：错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS const char* _CALLTYPE MSQLite_QueryN_GetValue(HMRecordSet RSet,
	const char* c_szColName)
{
	LOG_Print_Info(SQLite_Module, "MSQLite_QueryN_GetValue():\n\tRSet:%d", RSet);

	uint32_t uCount = reinterpret_cast<RecordSet*>(RSet)->GetColumnsCount();
	for (int i = 0; i < uCount; i ++)
	{
		if (strcmp(reinterpret_cast<RecordSet*>(RSet)->GetColumnName(i), c_szColName) == 0)
		{
			return reinterpret_cast<RecordSet*>(RSet)->GetValue(i);
		}
	}

	return NULL;
}

/*****************************************************************
/*函数说明：	获取记录集行数
/*参数说明：	RSet:	 记录集句柄
/*返回值：		行数
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLite_Query_RowCount( HMRecordSet RSet )
{
	LOG_Print_Info(SQLite_Module, "SQLite_Query_RowCount():\n\tRSet:%d",RSet)

	return reinterpret_cast<RecordSet*>(RSet)->GetRow();
}


/*****************************************************************
/*函数说明：	获取记录集列数
/*参数说明：	RSet:	 记录集句柄
/*返回值：		列数
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLite_Query_ColumnCount( HMRecordSet RSet )
{
	LOG_Print_Info(SQLite_Module, "SQLite_Query_ColumnCount():\n\tRSet:%d",RSet)

	return reinterpret_cast<RecordSet*>(RSet)->GetColumnsCount();
}

/*****************************************************************
/*函数说明：	将结果转换为int
/*参数说明：	pValue:	 值指针
/*返回值：		结果
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLite_Query_ValueToInt( const char* pValue )
{
	LOG_Print_Info(SQLite_Module, "SQLite_Query_ValueToInt():\n\tpValue:%s",pValue)

	return atoi(pValue);
}

/*****************************************************************
/*函数说明：	将结果转换为long
/*参数说明：	pValue:	 值指针
/*返回值：		结果
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS long _CALLTYPE MSQLite_Query_ValueToLong( const char* pValue )
{
	LOG_Print_Info(SQLite_Module, "SQLite_Query_ValueToLong():\n\tpValue:%s",pValue)

	return atol(pValue);
}


/*****************************************************************
/*函数说明：	将结果转换为float
/*参数说明：	pValue:	 值指针
/*返回值：		结果
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS float _CALLTYPE MSQLite_Query_ValueToFloat( const char* pValue )
{
	LOG_Print_Info(SQLite_Module, "SQLite_Query_ValueToFloat():\n\tpValue:%s",pValue)

	return atof(pValue);
}


/*****************************************************************
/*函数说明：	将结果转换为long
/*参数说明：	pValue:	 值指针
/*返回值：		结果
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS double _CALLTYPE MSQLite_Query_ValueToDouble( const char* pValue )
{
	LOG_Print_Info(SQLite_Module, "SQLite_Query_ValueToDouble():\n\tpValue:%s",pValue)

	return atof(pValue);
}


/*****************************************************************
/*函数说明：	开始事务
/*参数说明：	
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE MSQLite_BeginTransaction( void )
{
	return Singleton<SQLiteDB>::instance()->Begin_Transaction();
}


/*****************************************************************
/*函数说明：	提交事务
/*参数说明：	
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE MSQLite_CommitTransaction( void )
{
	return Singleton<SQLiteDB>::instance()->Commit_Transaction();
}


/*****************************************************************
/*函数说明：	事务回滚
/*参数说明：	
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE MSQLite_RollBack( void )
{
	return Singleton<SQLiteDB>::instance()->RollBack();
}