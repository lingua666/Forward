
#include <libThirdWrap_SqlServer/SQLServerDB.h>

namespace _sqlserver_{

	SQLServer::SQLServer( void )
		: _isOpen( false )
	{
		
	}

	SQLServer::~SQLServer( void )
	{
		Close();
	}

	//连接数据库
	bool SQLServer::Connect( const char* szHost, unsigned int uiPort, 
		const char* szName, const char* szPwd, const char* DataBase )
	{
		CoInitialize(NULL);
		HRESULT hr = _pConnection.CreateInstance(_uuidof(Connection));
		if( FAILED(hr) )
		{
			printf("_ConnectionPtr对象指针实例化失败！！！\r\n");
			return false;
		}
		else 
		{
			_string_type s;
			try
			{
				s = "Driver={sql server};server=";
				s += szHost;
				s += ",";
				s += _string_type::NumberToStr(uiPort);
				s += ";";
				s += "uid=";
				s += szName;
				s += ";";
				s += "pwd=";
				s += szPwd;
				s += ";database=";
				s += DataBase;
				s += ";";

				_bstr_t strConnect = s.c_str();//"Driver={sql server};server=192.168.8.153,1433;uid=sa;pwd=P@ssw0rd;database=equipmenttable;";
				//要注意修改用户名uid、密码pwd、数据库名database，这三个参数均是你自己sql server的数据库信息
				_pConnection->Open(strConnect,"","",adModeUnknown);
			}
			catch(_com_error &e)
			{
				printf("sql error:%s\r\n", s.c_str());
				cout<<"SqlSever_Open error:"<<e.Description()<<endl;
			}
		}

		_isOpen = true;
		return true;
	}

	void SQLServer::Close( void )
	{	
		_isOpen = false;
		if( _pConnection->State )
			_pConnection->Close();
		_pConnection = NULL;
		::CoUninitialize();
	}

	//执行非查询SQL语句
	bool SQLServer::ExecuteNoQuery( const char* szSql )
	{
		try
		{
			_pConnection->Execute(szSql, NULL, 1);//执行SQL语句 
		}
		catch(_com_error &e)
		{
			printf("sql error:%s\r\n", szSql);
			cout<<"SQLServer::ExecuteNoQuery() error:"<<e.Description()<<endl;
			return false;
		}

		return true;
	}


	//开始事务
	bool SQLServer::Begin_Transaction( void )
	{
		try
		{
			_pConnection->BeginTrans();
		}
		catch(_com_error &e)
		{
			cout<<"SQLServer::Begin_Transaction() error:"<<e.Description()<<endl;
			return false;
		}

		return true;
	}

	//提交事务
	bool SQLServer::Commit_Transaction( void )
	{
		try
		{
			_pConnection->CommitTrans();
		}
		catch(_com_error &e)
		{
			cout<<"SQLServer::Commit_Transaction() error:"<<e.Description()<<endl;
			return false;
		}

		return true;
	}

	//事务回滚
	bool SQLServer::RollBack( void )
	{
		try
		{
			_pConnection->RollbackTrans();
		}
		catch(_com_error &e)
		{
			cout<<"SQLServer::RollBack() error:"<<e.Description()<<endl;
			return false;
		}

		return true;
	}

}//sqlite