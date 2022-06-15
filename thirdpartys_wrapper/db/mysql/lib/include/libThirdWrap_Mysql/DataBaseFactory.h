
#ifndef  _CUSTOM_DATABASEFACTORY_HEADER_DEF__
#define _CUSTOM_DATABASEFACTORY_HEADER_DEF__

#include <libThirdWrap_Mysql/AbstractDataBases.h>
#include <stdint.h>

namespace _mysql_{

class CDataBaseFactory
{
public:
	CDataBaseFactory( void );
	~CDataBaseFactory( void );

	//获取CMySql实例
	static CDataBaseFactory* GetInstance( void );

	//初始化
	bool Init( bool bIsReconnect = true, uint32_t uiConnectTimeOut = 60 );

	//释放
	void Release( void );

	//连接
	bool Connect( const char* szdb, const char* szHost,
					const char* szName, const char* szPwd,
					uint32_t uiPort, UInt8 u8Characters );

	//重新连接
	bool ReConnect( void );

	//断开连接
	bool Disconnect( void );

	//开始事务
	bool Begin( void );

	//事务提交
	bool Commit( void );

	//创建数据库
	bool CreateDB( const char* szDBName );

	//删除数据库
	bool DeleteDB( const char* szDBName );

	//创建表
	bool CreateTable( const char* szTableName, SDBTABLEHEADER* lpDBTableHeader,
					uint32_t uiTableType = 0);

	//删除表
	bool DeleteTable( const char* szTableName );

	//插入记录
	bool InsertRecord( const char* szTableName, DBTABLEFIELDS* lpDBTableFields = NULL, uint32_t uiFieldCount = 0,
					   const char* szwhere = NULL ); 

	//更新记录
	bool UpdateRecord( const char* szTableName, DBTABLEFIELDS* lpDBTableFields = NULL, uint32_t uiFieldCount = 0,
						const char* szwhere = NULL );

	//删除数据
	bool DeleteRecord( const char* szTableName, const char* szwhere = NULL );

	//执行SQL语句
	bool ExecuteNoQuerySql( const char* szSql );

	inline void BindDataBases(CAbstractDataBases*  lpCAbstractDB)
	{
		_pCAbstractDB = lpCAbstractDB;
	}

private:
	//实例指针
	CAbstractDataBases*  _pCAbstractDB;
};

} // namespace _mysql_

#endif

