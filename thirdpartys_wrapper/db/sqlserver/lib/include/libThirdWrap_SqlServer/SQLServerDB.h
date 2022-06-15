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

		//�������ݿ�
		bool Connect( const char* szHost, unsigned int uiPort, 
			const char* szName, const char* szPwd, const char* DataBase );

		//�ر����ݿ�
		void Close( void );

		//ִ�зǲ�ѯSQL���
		bool ExecuteNoQuery( const char* szSql );

		//��ʼ����
		bool Begin_Transaction( void );

		//�ύ����
		bool Commit_Transaction( void );

		//����ع�
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