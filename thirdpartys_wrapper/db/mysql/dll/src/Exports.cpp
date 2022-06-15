
#include <dllThirdWrap_Mysql/Exports.h>
#include <libThirdWrap_Mysql/RecordSet.h>
#include <libThirdWrap_Mysql/MysqlDB.h>
#include <libFoundation/modafx.h>

using namespace _mysql_;

MemPool_type	g_MYSQL_MemPool( sizeof(RecordSet) );

DLL_INIT_DEF(g_pTagTModuleInfoRecord_201602172024,
	"45000001000X2016021700000000WZZZ00000000",
	"MySQL数据库模块",
	"V1.0.0.1",
	"20160217",
	"")

DLL_LOG_DEF(Mysql_Module)

/*****************************************************************
/*函数说明：	初始化MYSQL （该接口必须在Mysql_Connect前调用)
/*参数说明：
/*				
/*返回值：		true: 成功   false: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE Mysql_Init( bool isReconnect,
														UInt32 u32ConnectTimeOut )
{
	LOG_Print_Info(Mysql_Module,"Mysql_Init():\n\tisReconnect:%d\n\tu32ConnectTimeOut:%d",
				isReconnect, u32ConnectTimeOut)

	return Singleton<MySqlDB>::instance()->Init(isReconnect, u32ConnectTimeOut);
}

/*****************************************************************
/*函数说明：释放MYSQL
/*参数说明：
/*
/*返回值：		true: 成功   false: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE Mysql_Release(void)
{
	Singleton<MySqlDB>::instance()->Release();
	return true;
}

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
							const char* szDBName, UInt8 u8Characters )
{
	LOG_Print_Info(Mysql_Module,
		"Mysql_Connect():\n\tszHost:%s\n\tu16Port:%d\n\tszUserName:%s\n\tszPwd:%s\n\tszDBName:%s\n\tu8Characters:%d",
		szHost, u16Port, szUserName, szPwd, szDBName, u8Characters)

	return Singleton<MySqlDB>::instance()->Connect(szHost, u16Port, szUserName,
											szPwd, szDBName, u8Characters );
}

/*****************************************************************
/*函数说明：	关闭数据库连接
/*参数说明：
/*				
/*返回值：		true: 成功   false: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE Mysql_DisConnect( void )
{
	LOG_Print_Info(Mysql_Module, "Mysql_DisConnect():")

	return Singleton<MySqlDB>::instance()->Disconnect();
}

/*****************************************************************
/*函数说明：	创建数据库
/*参数说明：	szDBName:	 数据库名
/*				
/*返回值：		true: 成功   false: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE Mysql_CreateDB( const char* szDBName )
{
	LOG_Print_Info(Mysql_Module, "Mysql_CreateDB():\n\tszDBName:%s",szDBName)

	char szSql[100] = {0};
	strcpy(szSql,"CREATE DATABASE ");
	strcat(szSql,szDBName);
	return Singleton<MySqlDB>::instance()->ExecuteNoQuery(szSql);
}

/*****************************************************************
/*函数说明：	删除数据库
/*参数说明：	szDBName:	 数据库名
/*				
/*返回值：		true: 成功   false: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE Mysql_DeleteDB( const char* szDBName )
{
	LOG_Print_Info(Mysql_Module, "Mysql_DeleteDB():\n\tszDBName:%s",szDBName)

	char szSql[100] = {0};
	strcpy(szSql,"DROP DATABASE ");
	strcat(szSql,szDBName);
	return Singleton<MySqlDB>::instance()->ExecuteNoQuery(szSql);
}

/*****************************************************************
/*函数说明：	执行SQL语句
/*参数说明：	szSQL:	 SQL语句
/*				
/*返回值：		true: 成功   false: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE Mysql_ExecuteSQL( const char* szSQL )
{
	LOG_Print_Info(Mysql_Module, "Mysql_ExecuteSQL():\n\tszSQL:%s",szSQL)

	return Singleton<MySqlDB>::instance()->ExecuteNoQuery(szSQL);
}

/*****************************************************************
/*函数说明：	数据库查询
/*参数说明：	szSQL:	 SQL语句
/*				
/*返回值：		记录集句柄:    0: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS HMRecordSet _CALLTYPE Mysql_Query_Open( const char* szSQL )
{
	LOG_Print_Info(Mysql_Module, "Mysql_Query_Open():\n\tszSQL:%s", szSQL)

	RecordSet* p = NULL;
	try
	{
		p = g_MYSQL_MemPool.AllocObj<RecordSet>(Singleton<MySqlDB>::instance()->GetMysqlHandle());
		if (p->Open(szSQL) != 1)
		{
			g_MYSQL_MemPool.FreeObj(p);
			p = NULL;
		}
	}
	catch (...)
	{
		printf("RecordSet Open Failed!!\r\n");
		g_MYSQL_MemPool.FreeObj(p);
		p = NULL;
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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE Mysql_Query_Close( HMRecordSet RSet )
{
	LOG_Print_Info(Mysql_Module, "Mysql_Query_Close():\n\tRSet:%d",RSet)

	if( RSet != NULL )
	{
		g_MYSQL_MemPool.FreeObj(reinterpret_cast<RecordSet*>(RSet));
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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE Mysql_Query_MoveFirst( HMRecordSet RSet )
{
	LOG_Print_Info(Mysql_Module, "Mysql_Query_MoveFirst():\n\tRSet:%d",RSet)

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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE Mysql_Query_MoveNext( HMRecordSet RSet )
{
	LOG_Print_Info(Mysql_Module, "Mysql_Query_MoveNext():\n\tRSet:%d",RSet)

	reinterpret_cast<RecordSet*>(RSet)->MoveNext();
	return 1;
}


/*****************************************************************
/*函数说明：	最后一条记录集
/*参数说明：	RecordSet:	 记录集句柄
/*				
/*返回值：		1：	成功 
				<=0：错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE Mysql_Query_MoveLast( HMRecordSet RSet )
{
	LOG_Print_Info(Mysql_Module, "Mysql_Query_MoveLast():\n\tRSet:%d",RSet)

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
EXTERN_C _SYMBOL_DLL_EXPORTS const char* _CALLTYPE Mysql_Query_GetValue( HMRecordSet RSet,
																		UInt32 u32Col )
{
	LOG_Print_Info(Mysql_Module, "Mysql_Query_GetValue():\n\tRSet:%d\n\tu32Col:%d",RSet,u32Col)

	return reinterpret_cast<RecordSet*>(RSet)->GetValue(u32Col);
}

/*****************************************************************
/*函数说明：	获取记录集行数
/*参数说明：	RSet:	 记录集句柄
/*返回值：		行数
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE Mysql_Query_RowCount( HMRecordSet RSet )
{
	LOG_Print_Info(Mysql_Module, "Mysql_Query_RowCount():\n\tRSet:%d",RSet)

	return reinterpret_cast<RecordSet*>(RSet)->GetRow();
}


/*****************************************************************
/*函数说明：	获取记录集列数
/*参数说明：	RSet:	 记录集句柄
/*返回值：		列数
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE Mysql_Query_ColumnCount( HMRecordSet RSet )
{
	LOG_Print_Info(Mysql_Module, "Mysql_Query_ColumnCount():\n\tRSet:%d",RSet)

	return reinterpret_cast<RecordSet*>(RSet)->GetColumnsCount();
}

/*****************************************************************
/*函数说明：	将结果转换为int
/*参数说明：	pValue:	 值指针
/*返回值：		结果
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE Mysql_Query_ValueToInt( const char* pValue )
{
	LOG_Print_Info(Mysql_Module, "Mysql_Query_ValueToInt():\n\tpValue:%d",pValue)

	return atoi(pValue);
}

/*****************************************************************
/*函数说明：	将结果转换为long
/*参数说明：	pValue:	 值指针
/*返回值：		结果
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS long _CALLTYPE Mysql_Query_ValueToLong( const char* pValue )
{
	LOG_Print_Info(Mysql_Module, "Mysql_Query_ValueToInt():\n\tpValue:%d",pValue)

	return atol(pValue);
}


/*****************************************************************
/*函数说明：	将结果转换为float
/*参数说明：	pValue:	 值指针
/*返回值：		结果
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS float _CALLTYPE Mysql_Query_ValueToFloat( const char* pValue )
{
	LOG_Print_Info(Mysql_Module, "Mysql_Query_ValueToInt():\n\tpValue:%d",pValue)

	return atof(pValue);
}


/*****************************************************************
/*函数说明：	将结果转换为long
/*参数说明：	pValue:	 值指针
/*返回值：		结果
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS double _CALLTYPE Mysql_Query_ValueToDouble( const char* pValue )
{
	LOG_Print_Info(Mysql_Module, "Mysql_Query_ValueToInt():\n\tpValue:%d",pValue)

	return atof(pValue);
}
