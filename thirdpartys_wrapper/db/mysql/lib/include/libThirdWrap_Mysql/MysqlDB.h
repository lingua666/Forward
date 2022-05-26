#ifndef  __CUSTOM_MYSQLDB_H_DEF__
#define __CUSTOM_MYSQLDB_H_DEF__

#include <libThirdWrap_Mysql/AbstractDataBases.h>
#include <libFoundation/modafx.h>

namespace _mysql_{

	enum MySQLActionTypes 
	{
		ACTION_TYPE_CREATE = 0x01,
		ACTION_TYPE_DROP = 0x02,
		ACTION_TYPE_SELECT = 0x04,
		ACTION_TYPE_INSERT = 0x08,
		ACTION_TYPE_UPDATE = 0x10,
		ACTION_TYPE_DELETE = 0x20,
		ACTION_TYPE_ALL = 0x40
	};

	enum MySQLDBTableTypes 
	{ 
		DBTABLE_TYPE_DEFAULT,
		DBTABLE_TYPE_MYISAM,
		DBTABLE_TYPE_MRG_MYISAM,
		DBTABLE_TYPE_INNODB,
		DBTABLE_TYPE_MEMORY,
	};

	enum MySQLDBCharacters
	{
		DBTABLE_CHARACTERS_GB2312,
		DBTABLE_CHARACTERS_GBK,
		DBTABLE_CHARACTERS_UTF8
	};

class MySqlDB
{
public:
	MySqlDB( void );
	~MySqlDB( void );

	//获取MySqlDB实例
	static MySqlDB* GetInstance( void ) ;

	//初始化
	bool Init( bool isReconnect = true, unsigned int uiConnectTimeOut = 60 );

	//释放
	void Release( void );

	//连接数据库
	bool Connect( const char* szHost, unsigned int uiPort, 
		const char* szName, const char* szPwd,
		const char* szDb, UInt8 u8Characters );

	//重新连接
	bool ReConnect( void );

	//断开连接
	bool Disconnect( void );

	//执行非查询SQL语句
	bool ExecuteNoQuery( const char* szSql );

	//改变用户
	bool ChangeUser( const char* szName, const char *szPwd, const char *szDb );
	
	//更改数据库名
	bool SelectDB( const char* szDb );

	//开始事务
	bool Begin( void );

	//事务提交
	bool Commit( void );

	//事务回滚
	bool RollBack( void );

	//创建数据表
	bool CreateTable( const char* szTableName, SDBTABLEHEADER* lpDBTableHeader, unsigned int uiTableType = 0 );


///////////////////MYSQL内部信息/////////////////
	bool AddUserGrants( const char* szHost, const char* szUserName, 
						const char* szPassword, const char* dzDBName, 
						unsigned long ulActionType );


	//仅限本地操作
	bool DeleteUserGrants( const char* szUserName, const char* szHost );
	

	//协议版本信息
	unsigned int GetProtolVersion( void ) const;

	//获取服务器版本信息
	const char* GetServerVersion( void ) const;
	
	//获取客户端版本信息
	const char* GetClientVersion( void ) const;

	//获取所使用连接类型的字符串，包括服务器主机名
	const char* GetHostInfo( void ) const;

	//获取服务器状态
	const char* GetState( void ) const;

	//获取错误码
	const char* GetErrorCode( void ) const;

	inline	MYSQL* GetMysqlHandle( void ) const
	{
		return _pMySQLConn;
	}

private:
	MYSQL* MySQLInit( void );
	MYSQL* MySQLConnect( const char* szHost, const char* szUserName,
		                const char* szPassword, const char* szDBName, unsigned int uiPort );
	int    MySQLQuery( const char* szSql );
	void   MySQLClose( void );

private:
	//一个连接
	MYSQL* _pMySQLConn;

	//MySql的结果集
	MYSQL_RES* _pQueryResult;

	bool     _isAutoCommit;
	bool     _isConnect;
};

DECLARE_EXCEPTION(DataBaseException,CMyException)

} // namespace _mysql_

#endif

