
#include <libThirdWrap_Sqlite/External.h>
#include <libThirdWrap_Sqlite/SQLiteDB.h>

#if defined(PLATFORM_OS_FAMILY_WINDOWS)
	#include <Windows.h>
	#include <conio.h>
#endif

#include <stdio.h>
#include <memory.h>

namespace _sqlite_{

	SQLiteDB::SQLiteDB()
	{
		m_db = NULL;
		m_bOpen = false;
	}

	SQLiteDB::~SQLiteDB()
	{
		CloseDataBase();
	}

	bool SQLiteDB::OpenDataBase(const char* c_szDBName)
	{
		//��ָ�������ݿ��ļ�,��������ڽ�����һ��ͬ�������ݿ��ļ� 
		if(c_szDBName == NULL)
			return false;

		//UTFת������֧������·��
		_string_type sPath = _string_type(c_szDBName)/*.gbk_utf8()*/;

		int rc = sqlite3_open(sPath.c_str(), &m_db); 
		if( rc ) 
		{ 
			printf("SQLiteDB::OpenDataBase() Can't open database: %s\n", sqlite3_errmsg(m_db));
			sqlite3_close(m_db);
			return false;
		}

		printf("SQLiteDB::OpenDataBase() open database: %s success\n", c_szDBName);

		m_bOpen = true;
		return true;
	}

	void SQLiteDB::CloseDataBase()
	{	
		m_bOpen = false;
		if(m_db)
		{
			sqlite3_close(m_db); //�ر����ݿ�
			m_db = NULL;
		}
	}

	bool SQLiteDB::ExcuteSql(const char* c_szSQL)
	{
		if(c_szSQL == NULL || m_db == NULL)
			return false;

		//ִ��SQL���
		char* zErrMsg;
		int rc = sqlite3_exec( m_db, c_szSQL, 0, 0, &zErrMsg ); 
		if(rc)
		{
			printf("SQLiteDB::ExcuteSql() %s\r\n",zErrMsg);
			return false;
		}
		return true;
	}


	//��ʼ����
	bool SQLiteDB::Begin_Transaction()
	{
		return ExcuteSql("begin;");  //��ʼ����
	}

	//�ύ����
	bool SQLiteDB::Commit_Transaction()
	{
		return ExcuteSql("commit;");  //��ʼ�ύ
	}

	//����ع�
	bool SQLiteDB::RollBack( void )
	{
		return ExcuteSql("rollback;");  //��ʼ�ύ
	}

}//sqlite