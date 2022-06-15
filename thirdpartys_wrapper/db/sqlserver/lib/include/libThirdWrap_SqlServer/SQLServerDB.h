#ifndef __CUSTOMS_SQLSERVERDB_HEAD_H_DEF__
#define __CUSTOMS_SQLSERVERDB_HEAD_H_DEF__ 

#include "RecordSet.hpp"

namespace _sqlserver_{

	class SQLServer
	{
	public:
		SQLServer( void );

		~SQLServer( void );

		static SQLServer* GetInstance( void )
		{
			static SQLServer s_SQLServer;
			return &s_SQLServer;
		}

		//连接数据库
		bool Connect( const char* szHost, unsigned int uiPort, 
			const char* szName, const char* szPwd, const char* DataBase );

		//关闭数据库
		void Close( void );

		//执行非查询SQL语句
		bool ExecuteNoQuery( const char* szSql );

		//开始事务
		bool Begin_Transaction( void );

		//提交事务
		bool Commit_Transaction( void );

		//事务回滚
		bool RollBack( void );

		_ConnectionPtr GetHandle( void )
		{
			return _pConnection;
		}

	private:
		_ConnectionPtr  _pConnection;
		bool		_isOpen;
	};

	inline SQLServer* GetSQLServerInstance( void )
	{
		static SQLServer s_SQLServer;
		return &s_SQLServer;
	}

}//sqlite 

#endif