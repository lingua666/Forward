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

		//�����ݿ�
		bool OpenDataBase(const char* c_szDBName);

		//�ر����ݿ�
		void CloseDataBase( void );

		//ִ��SQL���
		bool ExcuteSql(const char* c_szSQL);

		//��ʼ����
		bool Begin_Transaction( void );

		//�ύ����
		bool Commit_Transaction( void );

		//����ع�
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