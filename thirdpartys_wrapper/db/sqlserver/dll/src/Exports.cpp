
#include <dllThirdWrap_SqlServer/Exports.h>
#include <libThirdWrap_SqlServer/RecordSet.hpp>
#include <libThirdWrap_SqlServer/SQLServerDB.h>
#include <libFoundation/modafx.h>

using namespace _sqlserver_;

MemPool_type	g_SQLServer_MemPool( sizeof(RecordSet) );

DLL_INIT_DEF(g_pTagTModuleInfoRecord_201602172020,
	"45000001000W2016021700000000WZZZ00000000",
	"SQLite数据库模块",
	"V1.0.0.1",
	"20160217",
	"")

DLL_LOG_DEF(SQLServer_Module)

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
															const char* szDataBase )
{
	LOG_Print_Info(SQLServer_Module, "MSQLServer_Connect():\n\t"
		"szHost:%s\n\tuPort:%d\n\tszUserName:%s\n\tszPwd:%s\n\t",
		szHost, uPort, szUserName, szPwd)

	return Singleton<SQLServer>::instance()->Connect(szHost, uPort, szUserName, szPwd, szDataBase);
}

/*****************************************************************
/*函数说明：	关闭数据库连接
/*参数说明：
/*				
/*返回值：		true: 成功   false: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE MSQLServer_DisConnect( void )
{
	LOG_Print_Info(SQLServer_Module, "MSQLServer_DisConnect():")

	Singleton<SQLServer>::instance()->Close();
	return true;
}

/*****************************************************************
/*函数说明：	执行SQL语句
/*参数说明：	szSQL:	 SQL语句
/*				
/*返回值：		true: 成功   false: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE MSQLServer_ExecuteSQL( const char* szSQL )
{
	LOG_Print_Info(SQLServer_Module, "MSQLServer_ExecuteSQL():\n\tszSQL:%s",szSQL)

	return Singleton<SQLServer>::instance()->ExecuteNoQuery(szSQL);
}

/*****************************************************************
/*函数说明：	数据库查询
/*参数说明：	szSQL:	 SQL语句
/*				
/*返回值：		记录集句柄:    0: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS HMRecordSet _CALLTYPE MSQLServer_Query_Open( const char* szSQL )
{
	LOG_Print_Info(SQLServer_Module, "MSQLServer_Query_Open():\n\tszSQL:%s",szSQL)

	RecordSet* p = g_SQLServer_MemPool.AllocObj<RecordSet>(Singleton<SQLServer>::instance()->GetHandle());
	if( p->Open(szSQL) != 1 )
	{
		g_SQLServer_MemPool.FreeObj(p);
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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLServer_Query_Close( HMRecordSet RSet )
{
	LOG_Print_Info(SQLServer_Module, "MSQLServer_Query_Close():\n\tRSet:%p",RSet)

	if( RSet != NULL )
	{
		g_SQLServer_MemPool.FreeObj(reinterpret_cast<RecordSet*>(RSet));
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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLServer_Query_MoveFirst( HMRecordSet RSet )
{
	LOG_Print_Info(SQLServer_Module, "MSQLServer_Query_MoveFirst():\n\tRSet:%p",RSet)

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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLServer_Query_MoveNext( HMRecordSet RSet )
{
	LOG_Print_Info(SQLServer_Module, "MSQLServer_Query_MoveNext():\n\tRSet:%p",RSet)

	return reinterpret_cast<RecordSet*>(RSet)->MoveNext();
}


/*****************************************************************
/*函数说明：	最后一条记录集
/*参数说明：	RecordSet:	 记录集句柄
/*				
/*返回值：		1：	成功 
				<=0：错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLServer_Query_MoveLast( HMRecordSet RSet )
{
	LOG_Print_Info(SQLServer_Module, "MSQLServer_Query_MoveLast():\n\tRSet:%p",RSet)

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
EXTERN_C _SYMBOL_DLL_EXPORTS const char* _CALLTYPE MSQLServer_Query_GetValue( HMRecordSet RSet,
																		UInt32 u32Col )
{
	LOG_Print_Info(SQLServer_Module, "MSQLServer_Query_GetValue():\n\tRSet:%p",RSet)

	return reinterpret_cast<RecordSet*>(RSet)->GetValue(u32Col);
}

/*****************************************************************
/*函数说明：	获取记录集行数
/*参数说明：	RSet:	 记录集句柄
/*返回值：		行数
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLServer_Query_RowCount( HMRecordSet RSet )
{
	LOG_Print_Info(SQLServer_Module, "MSQLServer_Query_RowCount():\n\tRSet:%p",RSet)

	return reinterpret_cast<RecordSet*>(RSet)->GetRow();
}


/*****************************************************************
/*函数说明：	获取记录集列数
/*参数说明：	RSet:	 记录集句柄
/*返回值：		列数
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLServer_Query_ColumnCount( HMRecordSet RSet )
{
	LOG_Print_Info(SQLServer_Module, "MSQLServer_Query_ColumnCount():\n\tRSet:%p", RSet)

	return reinterpret_cast<RecordSet*>(RSet)->GetColumnsCount();
}

/*****************************************************************
/*函数说明：	将结果转换为int
/*参数说明：	pValue:	 值指针
/*返回值：		结果
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLServer_Query_ValueToInt( const char* pValue )
{
	LOG_Print_Info(SQLServer_Module, "MSQLServer_Query_ValueToInt():\n\tpValue:%s",pValue)

	return atoi(pValue);
}

/*****************************************************************
/*函数说明：	将结果转换为long
/*参数说明：	pValue:	 值指针
/*返回值：		结果
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS long _CALLTYPE MSQLServer_Query_ValueToLong( const char* pValue )
{
	LOG_Print_Info(SQLServer_Module, "MSQLServer_Query_ValueToLong():\n\tpValue:%s",pValue)

	return atol(pValue);
}


/*****************************************************************
/*函数说明：	将结果转换为float
/*参数说明：	pValue:	 值指针
/*返回值：		结果
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS float _CALLTYPE MSQLServer_Query_ValueToFloat( const char* pValue )
{
	LOG_Print_Info(SQLServer_Module, "MSQLServer_Query_ValueToFloat():\n\tpValue:%s",pValue)

	return atof(pValue);
}


/*****************************************************************
/*函数说明：	将结果转换为long
/*参数说明：	pValue:	 值指针
/*返回值：		结果
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS double _CALLTYPE MSQLServer_Query_ValueToDouble( const char* pValue )
{
	LOG_Print_Info(SQLServer_Module, "MSQLServer_Query_ValueToDouble():\n\tpValue:%s",pValue)

	return atof(pValue);
}
