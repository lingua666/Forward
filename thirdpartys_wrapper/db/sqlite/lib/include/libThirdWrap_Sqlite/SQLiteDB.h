#ifndef __CUSTOMS_SQLITEDB_HEAD_H_DEF__
#define __CUSTOMS_SQLITEDB_HEAD_H_DEF__ 

#include <sqlite3.h>
#include <map>

#define FALSE   0
#define TRUE    1

typedef int                 BOOL;

namespace _sqlite_{

	class SQLiteDB
	{
	public:
		SQLiteDB();
		~SQLiteDB();

		static SQLiteDB* GetInstance( void )
		{
			static SQLiteDB s_SQLiteDB;
			return &s_SQLiteDB;
		}

		//打开数据库
		bool OpenDataBase(const char* c_szDBName);

		//关闭数据库
		void CloseDataBase( void );

		//执行SQL语句
		bool ExcuteSql(const char* c_szSQL);

		//开始事务
		bool Begin_Transaction( void );

		//提交事务
		bool Commit_Transaction( void );

		//事务回滚
		bool RollBack( void );

		inline sqlite3* GetSQLiteHandle( void )
		{
			return m_db;
		}

	private:
		sqlite3*	m_db;
		bool		m_bOpen;
	};

}//sqlite 

#endif