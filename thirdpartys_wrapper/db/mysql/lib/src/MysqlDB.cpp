
#include <libThirdWrap_Mysql/MysqlDB.h>
#include <libFoundation/modafx.h>

namespace _mysql_{

#define TYPE_MYSQLVALID(handle) \
	{\
		if(!handle)\
		{\
			return false;\
		}\
	}

#define TYPE_MYSQLVALID_NULL(handle) \
	{\
		if(!handle)\
		{\
			return NULL;\
		}\
	}

static bool	GBKToUTF8( _string_type &szOut )
{
	szOut = szOut.gbk_utf8();
	return true;
}

static bool NoneCharEncoding( _string_type &szOut )
{
	return true;
}

typedef bool (*CharEncoding)( _string_type &szOut );


static const char* g_c_szMySQLDBFieldType[] = {"integer","double","datetime","bit","varchar","text"};
static const char* g_c_szMySQLDBTableType[] = {"MyISAM","MyISAM","MRG_MYISAM","InnoDB","MEMORY"};
static CharEncoding	g_CharEncoding[] = {NoneCharEncoding, NoneCharEncoding, GBKToUTF8};

static const char* c_szMySQLActionTypes[] = {"CREATE","DROP","SELECT","INSERT","UPDATE","DELETE","ALL"};
static const char* c_szMySQLCharacters[] = {"gb2312", "gbk", "utf8"};

MySqlDB::MySqlDB( void )
{
	_pMySQLConn = NULL;
	_pQueryResult = 0;
	_isAutoCommit = false;
	_isConnect = false;
}

MySqlDB::~MySqlDB( void )
{
	Release();
}

MySqlDB* MySqlDB::GetInstance( void )
{
	static MySqlDB sMySqlDB;
	return &sMySqlDB;
}

bool MySqlDB::Init( bool isReconnect, uint32_t uiConnectTimeOut )
{
	//test
	LOG_Print_SetLog(libThirdWrap_Mysql, MLog_GetAllLevel());
	//test

	//关闭
	Disconnect();

	//初始化数据结构
	if(NULL == (_pMySQLConn = MySQLInit()))
	{
		_pMySQLConn = NULL;
		//EXCEPTION_THROW(DataBaseException,"MySqlDB Init MySql Fail!!\r\n");
		LOG_Print_Error(libThirdWrap_Mysql, "MySqlDB Init MySql Fail!!\r\n");
		return false;
	}
	
	//设置连接超时
	unsigned timeout = uiConnectTimeOut;
	if(mysql_options(_pMySQLConn, MYSQL_OPT_CONNECT_TIMEOUT, (char *)&timeout))
	{
		//EXCEPTION_THROW(DataBaseException,"MySqlDB Init Fail,mysql_options errorcode:%s\r\n",GetErrorCode());
		LOG_Print_Error(libThirdWrap_Mysql, "MySqlDB Init Fail,mysql_options errorcode:%s\r\n", GetErrorCode());
		return false;
	}

	//启动再连接  5.0.19之前必须在mysql_real_connect()之后设置
	my_bool reconnect = isReconnect;
	if(mysql_options(_pMySQLConn, MYSQL_OPT_RECONNECT, (char *)&reconnect))
	{
		//EXCEPTION_THROW(DataBaseException,"MySqlDB Init Fail,mysql_options errorcode:%s\r\n",GetErrorCode());
		LOG_Print_Error(libThirdWrap_Mysql, "MySqlDB Init Fail,mysql_options errorcode:%s\r\n", GetErrorCode());
		return false;
	}

	return true;
}

void MySqlDB::Release( void )
{
	Disconnect();
	_pMySQLConn = NULL;
}

bool MySqlDB::Connect( const char* szHost, unsigned int uiPort, 
		const char* szName, const char* szPwd,
		const char* szDb, UInt8 u8Characters )
{
	TYPE_MYSQLVALID(_pMySQLConn);
	if( szHost == NULL || szName == NULL || 
		szPwd == NULL || szDb == NULL )
	{
		return false;
	}

	//关闭连接
	Disconnect();

	if(NULL == MySQLConnect(szHost, szName, szPwd, szDb, uiPort))
	{
		//EXCEPTION_THROW(DataBaseException,"MySqlDB Connect Fail,mysql_real_connect errorcode:%s\r\n",GetErrorCode());
		LOG_Print_Error(libThirdWrap_Mysql, "MySqlDB Connect Fail,mysql_real_connect errorcode:%s\r\n", GetErrorCode());
		return false;
	}

	//设置连接字符集，默认为gb2312
	//if(mysql_set_character_set(_pMySQLConn, "gb2312"))
	if(mysql_set_character_set(_pMySQLConn, c_szMySQLCharacters[u8Characters]))
	{
		//如果设置字符集不成功，则关闭连接
		Disconnect();
		//EXCEPTION_THROW(DataBaseException,"MySqlDB Connect Fail,mysql_set_character_set errorcode:%s\r\n",GetErrorCode());
		LOG_Print_Error(libThirdWrap_Mysql, "MySqlDB Connect Fail,mysql_set_character_set errorcode:%s\r\n", GetErrorCode());
		return false;
	}

	//设置允许多语句支持
	//mysql_set_server_option(_pMySQLConn, MYSQL_OPTION_MULTI_STATEMENTS_ON);
	_isConnect = true;
	return true;
}

//重新连接
bool MySqlDB::ReConnect( void )
{
	TYPE_MYSQLVALID(_pMySQLConn);
	//自动重新连接
	if(mysql_ping(_pMySQLConn) != 0)
	{
		//EXCEPTION_THROW(DataBaseException,"MySqlDB ReConnect Fail,mysql_ping errorcode:%s\r\n",GetErrorCode());
		LOG_Print_Error(libThirdWrap_Mysql, "MySqlDB ReConnect Fail,mysql_ping errorcode:%s\r\n", GetErrorCode());
		return false;
	}
	return true;
}

bool MySqlDB::Disconnect( void )
{
	TYPE_MYSQLVALID(_pMySQLConn);

	if(_isConnect)
	{
		MySQLClose();
		_pMySQLConn = NULL;
		_isConnect = false;
	}

	return true;
}

bool MySqlDB::ExecuteNoQuery( const char* szSql )
{
	TYPE_MYSQLVALID(_pMySQLConn);
	if(szSql == NULL)
		return 0;

	bool uRet = true ;
	//撤消自动提交模式
	//mysql_autocommit(_pMySQLConn, 0);
	if(MySQLQuery(szSql))
	{
		//EXCEPTION_THROW(DataBaseException,"MySqlDB ExecuteNoQuery Fail,mysql_query errorcode:%s\r\n\tsql:%s",GetErrorCode(),szSql);
		LOG_Print_Error(libThirdWrap_Mysql, "MySqlDB ExecuteNoQuery Fail,mysql_query errorcode:%s\r\n\tsql:%s", GetErrorCode());
		return false;
	}

	//如果没有成功
	/*if(mysql_commit(_pMySQLConn))
	{
		//提交失败
		printf("MySqlDB ExecuteNoQuery Fail,errorcode:%s\r\n",GetErrorCode());

		//回滚事务
		RollBack();
		uRet = false;
	}*/
	//恢复自动提交模式
	//mysql_autocommit(_pMySQLConn, 0); 
	return uRet;
}

//开始事务
bool MySqlDB::Begin( void )
{
	TYPE_MYSQLVALID(_pMySQLConn);
	if(mysql_autocommit(_pMySQLConn, 0))
	{
		//EXCEPTION_THROW(DataBaseException,"MySqlDB Begin Fail,mysql_autocommit errorcode:%s\r\n",GetErrorCode());
		LOG_Print_Error(libThirdWrap_Mysql, "MySqlDB Begin Fail,mysql_autocommit errorcode:%s\r\n", GetErrorCode());
		return false;
	}
	
	_isAutoCommit = false;
	return true;
}

//事务提交
bool MySqlDB::Commit( void )
{
	TYPE_MYSQLVALID(_pMySQLConn);
	if(_isAutoCommit)
		return false;

	if(mysql_commit(_pMySQLConn))
	{	
		//回滚事务
		RollBack();
		//EXCEPTION_THROW(DataBaseException,"MySqlDB Commit Fail,mysql_commit errorcode:%s\r\n",GetErrorCode());
		LOG_Print_Error(libThirdWrap_Mysql, "MySqlDB Commit Fail,mysql_commit errorcode:%s\r\n", GetErrorCode());
		return false;
	}

	if(mysql_autocommit(_pMySQLConn, 1))
	{
		//EXCEPTION_THROW(DataBaseException,"MySqlDB Commit Fail,mysql_autocommit errorcode:%s\r\n",GetErrorCode());
		LOG_Print_Error(libThirdWrap_Mysql, "MySqlDB Commit Fail,mysql_autocommit errorcode:%s\r\n", GetErrorCode());
		return false;
	}

	_isAutoCommit = true;
	return true;
}

//事务回滚
bool MySqlDB::RollBack( void )
{
	TYPE_MYSQLVALID(_pMySQLConn);
	if(mysql_rollback(_pMySQLConn))
	{
		//EXCEPTION_THROW(DataBaseException,"MySqlDB RollBack Fail,mysql_rollback errorcode:%s\r\n",GetErrorCode());
		LOG_Print_Error(libThirdWrap_Mysql, "MySqlDB RollBack Fail,mysql_rollback errorcode:%s\r\n", GetErrorCode());
		return false;
	}

	return true;
}

//创建数据表
bool MySqlDB::CreateTable( const char* szTableName, SDBTABLEHEADER* lpDBTableHeader,
				          uint32_t uiTableType )
{
	if(uiTableType > DBTABLE_TYPE_MEMORY)
		return false;

	bool IsPrimaryKey = false;
	char szSql[1024] = {0};
	char szPrimaryKey[300] = {0};
	char szType[30] = {0};
	bool bResult = false;

	strcpy(szPrimaryKey,"primary key(");
	sprintf(szSql,"Create table %s(",szTableName);
	int iLen = 0;
	for(int i = 0; i < lpDBTableHeader->SFieldCount; i ++)
	{
		if(lpDBTableHeader->SFieldHeader[i].uiFieldType > DBFIELD_TYPE_TEXT)
			continue;

		strcpy(szType,g_c_szMySQLDBFieldType[lpDBTableHeader->SFieldHeader[i].uiFieldType]);
		if(strcmp(szType,"varchar") == 0)
		{
			if(lpDBTableHeader->uiFieldLength[i] <= 0)
			{
				printf("字段类型为：varchar的长度不能小于等于0");
				return false;
			}

			sprintf(szType,"varchar(%d)",lpDBTableHeader->uiFieldLength[i]);
		}

		sprintf(&szSql[strlen(szSql)],"%s %s not null,",\
				lpDBTableHeader->SFieldHeader[i].cField_Name,szType);

		//主键字符串
		if(lpDBTableHeader->bIsPrimaryKey[i])
		{
			IsPrimaryKey = true;
			iLen = strlen(szPrimaryKey);
			sprintf(&szPrimaryKey[iLen],"%s,",lpDBTableHeader->SFieldHeader[i].cField_Name);
		}
	}

	//将最后一个'，'换为‘）’
	if(IsPrimaryKey)
	{
		iLen = strlen(szPrimaryKey);
		szPrimaryKey[iLen - 1] = ')';
		strcat(szSql,szPrimaryKey);
		strcat(szSql,")");
	}
	else
	{
		iLen = strlen(szSql);
		szSql[iLen - 1] = ')';
	}

	/*strcat(szSql," type = ");
	strcat(szSql,g_c_szMySQLDBTableType[uiTableType]);*/
	bResult =  ExecuteNoQuery(szSql);
	return bResult;
}

bool MySqlDB::ChangeUser( const char* szName, const char *szPwd, const char *szDb )
{
	TYPE_MYSQLVALID(_pMySQLConn);

	bool bRet = true;
	//如果失败，返回FALSE
	if(mysql_change_user(_pMySQLConn, szName, szPwd, szDb) )
	{
		bRet = false;
		//EXCEPTION_THROW(DataBaseException,"MySqlDB ChangeUser Fail,mysql_change_user errorcode:%s\r\n",GetErrorCode());
		LOG_Print_Error(libThirdWrap_Mysql, "MySqlDB ChangeUser Fail,mysql_change_user errorcode:%s\r\n", GetErrorCode());
		return false;
	}
	return bRet;
}

bool MySqlDB::SelectDB( const char* szDb )
{
	TYPE_MYSQLVALID(_pMySQLConn);

	if(mysql_select_db(_pMySQLConn, szDb))
	{
		//EXCEPTION_THROW(DataBaseException,"MySqlDB SelectDB Fail,mysql_select_db errorcode:%s\r\n",GetErrorCode());
		LOG_Print_Error(libThirdWrap_Mysql, "MySqlDB SelectDB Fail,mysql_select_db errorcode:%s\r\n", GetErrorCode());
		return false;
	}
	return true;
}

bool MySqlDB::AddUserGrants(const char* szHost, const char* szUserName, 
					 const char* szPassword, const char* dzDBName, 
					 unsigned long ulActionType )
{
	TYPE_MYSQLVALID(szUserName);
	TYPE_MYSQLVALID(szPassword);
	
	DeleteUserGrants(szUserName,szHost);

	char szSql[200] = {0};
	char szGrants[100] = {0};

	if((ulActionType & ACTION_TYPE_ALL) > 0)
	{
		strcpy(szGrants,"ALL");
	}
	else
	{
		if((ulActionType & ACTION_TYPE_CREATE) > 0)
		{
			strcat(szGrants,c_szMySQLActionTypes[0]);
		}

		if((ulActionType & ACTION_TYPE_DROP) > 0)
		{
			if(strlen(szGrants) > 1)
				strcat(szGrants,",");
			strcat(szGrants,c_szMySQLActionTypes[1]);
		}

		if((ulActionType & ACTION_TYPE_SELECT) > 0)
		{
			if(strlen(szGrants) > 1)
				strcat(szGrants,",");
			strcat(szGrants,c_szMySQLActionTypes[2]);
		}

		if((ulActionType & ACTION_TYPE_INSERT) > 0)
		{
			if(strlen(szGrants) > 1)
				strcat(szGrants,",");
			strcat(szGrants,c_szMySQLActionTypes[3]);
		}

		if((ulActionType & ACTION_TYPE_UPDATE) > 0)
		{
			if(strlen(szGrants) > 1)
				strcat(szGrants,",");
			strcat(szGrants,c_szMySQLActionTypes[4]);
		}

		if((ulActionType & ACTION_TYPE_DELETE) > 0)
		{
			if(strlen(szGrants) > 1)
				strcat(szGrants,",");
			strcat(szGrants,c_szMySQLActionTypes[5]);
		}
	}
	
	if(dzDBName == NULL)
	{
		sprintf(szSql,"GRANT %s PRIVILEGES ON *.* TO '%s'@'%s' IDENTIFIED BY '%s'",szGrants,szUserName,
				szHost,szPassword);
	}
	else
	{
		sprintf(szSql,"GRANT %s PRIVILEGES ON %s.* TO '%s'@'%s' IDENTIFIED BY '%s'",szGrants,dzDBName,szUserName,
				szHost,szPassword);
	}
	
	if(MySQLInit() == NULL)
	{
		//EXCEPTION_THROW(DataBaseException,"MySqlDB AddUserGrants FAIL, mysql_init errorcode:%s\r\n",GetErrorCode());
		LOG_Print_Error(libThirdWrap_Mysql, "MySqlDB AddUserGrants FAIL, mysql_init errorcode:%s\r\n", GetErrorCode());
		return false;
	}

	//连接MYSQL数据库 
	if(NULL == MySQLConnect("localhost","root",NULL,"mysql",0)) 
	{ 
		//EXCEPTION_THROW(DataBaseException,"MySqlDB AddUserGrants FAIL, mysql_real_connect errorcode:%s\r\n",GetErrorCode());
		LOG_Print_Error(libThirdWrap_Mysql, "MySqlDB AddUserGrants FAIL, mysql_real_connect errorcode:%s\r\n", GetErrorCode());
		return false;
	} 

	if(MySQLQuery(szSql))
	{
		//EXCEPTION_THROW(DataBaseException,"MySqlDB AddUserGrants FAIL, mysql_query errorcode:%s\r\n",GetErrorCode());
		LOG_Print_Error(libThirdWrap_Mysql, "MySqlDB AddUserGrants FAIL, mysql_query errorcode:%s\r\n", GetErrorCode());
		return false;
	}

	//刷新授权
	if(MySQLQuery("flush privileges;"))
	{
		//EXCEPTION_THROW(DataBaseException,"MySqlDB AddUserGrants FAIL, mysql_query errorcode:%s\r\n",GetErrorCode());
		LOG_Print_Error(libThirdWrap_Mysql, "MySqlDB AddUserGrants FAIL, mysql_query errorcode:%s\r\n", GetErrorCode());
		return false;
	}

	MySQLClose();
	return true;
}

bool MySqlDB::DeleteUserGrants( const char* szUserName, const char* szHost )
{
	TYPE_MYSQLVALID(szUserName);
	TYPE_MYSQLVALID(szHost);
	char szSql[100] = {0};
	sprintf(szSql,"DROP USER '%s'@'%s'",szUserName,szHost);

	if(MySQLInit() == NULL)
	{
		//EXCEPTION_THROW(DataBaseException,"MySqlDB DeleteUserGrants FAIL, mysql_init errorcode:%s\r\n",GetErrorCode());
		LOG_Print_Error(libThirdWrap_Mysql, "MySqlDB DeleteUserGrants FAIL, mysql_init errorcode:%s\r\n", GetErrorCode());
		return false;
	}

	//连接MYSQL数据库 
	if(NULL == MySQLConnect("localhost","root",NULL,"mysql",0)) 
	{ 
		//EXCEPTION_THROW(DataBaseException,"MySqlDB DeleteUserGrants FAIL, mysql_real_connect errorcode:%s\r\n",GetErrorCode());
		LOG_Print_Error(libThirdWrap_Mysql, "MySqlDB DeleteUserGrants FAIL, mysql_real_connect errorcode:%s\r\n", GetErrorCode());
		return false;
	} 

	if(MySQLQuery(szSql))
	{
		//EXCEPTION_THROW(DataBaseException,"MySqlDB DeleteUserGrants FAIL, mysql_query errorcode:%s\r\n",GetErrorCode());
		LOG_Print_Error(libThirdWrap_Mysql, "MySqlDB DeleteUserGrants FAIL, mysql_query errorcode:%s\r\n", GetErrorCode());
		return false;
	}

	MySQLClose();
	return true;
}

//协议版本信息
uint32_t MySqlDB::GetProtolVersion( void ) const
{
	TYPE_MYSQLVALID_NULL(_pMySQLConn);
	return mysql_get_proto_info(_pMySQLConn);
}

//获取服务器版本信息
const char* MySqlDB::GetServerVersion( void ) const
{
	TYPE_MYSQLVALID_NULL(_pMySQLConn);
	return mysql_get_server_info(_pMySQLConn);
}

//获取客户端版本信息
const char* MySqlDB::GetClientVersion( void ) const
{
	TYPE_MYSQLVALID_NULL(_pMySQLConn);
	return mysql_get_client_info();
}

//获取所使用连接类型的字符串，包括服务器主机名
const char* MySqlDB::GetHostInfo( void ) const
{
	TYPE_MYSQLVALID_NULL(_pMySQLConn);
	return mysql_get_host_info(_pMySQLConn);
}

//获取服务器状态
const char* MySqlDB::GetState( void ) const
{
	TYPE_MYSQLVALID_NULL(_pMySQLConn);
	const char* szState = mysql_stat(_pMySQLConn);
	if (szState == NULL)
	{
		//EXCEPTION_THROW(DataBaseException, "MySqlDB GetState Fail,mysql_stat errorcode:%s\r\n", GetErrorCode());
		LOG_Print_Error(libThirdWrap_Mysql, "MySqlDB GetState Fail,mysql_stat errorcode:%s\r\n", GetErrorCode());
		return "";
	}
	return szState;
}

//获取错误码
const char* MySqlDB::GetErrorCode( void ) const
{
	TYPE_MYSQLVALID_NULL(_pMySQLConn);
	return mysql_error(_pMySQLConn);
}

MYSQL* MySqlDB::MySQLInit( void )
{
	return mysql_init(_pMySQLConn);
}

MYSQL* MySqlDB::MySQLConnect( const char* szHost, const char* szUserName,
	const char* szPassword, const char* szDBName, uint32_t uiPort )
{
	return mysql_real_connect(_pMySQLConn,szHost,szUserName,szPassword,szDBName,uiPort,NULL,0);
}

int    MySqlDB::MySQLQuery(  const char* szSql )
{
	return mysql_query(_pMySQLConn,szSql);
}

void   MySqlDB::MySQLClose( void )
{
	mysql_close(_pMySQLConn);
}

} // namespace _mysql_
