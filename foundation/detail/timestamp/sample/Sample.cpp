// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <libTimestamp/Timestamp.h>
#include <libTimestamp/HighTimeCounter.h>

#include <sys/timeb.h>

void Test( void )
{
	_string_type str1 = _timestamp_::Timestamp().DateMicroString();
	printf("%s\r\n", str1.c_str());

	_timestamp_::HighTimeCounter	st0;
	_string_type str2 = _timestamp_::Timestamp(st0.get()).DateMicroString();
	printf("%s\r\n", str2.c_str());

	Sleep(1200);
	UInt64 ul = _timestamp_::Timestamp().epochMicroseconds() ;
	UInt64 ul1 = st0.getMicroseconds() ;

	int iTimer = 0, iNum = 100000000;
	iTimer = GetTickCount();
	for(int i = 0 ; i < iNum; i ++)
	{
		_timestamp_::Timestamp	st;
		int iys = _timestamp_::Timestamp().epochMicroseconds() - st.epochMicroseconds();
	}
	printf("Timestamp epochMicroseconds timer:%d\r\n", GetTickCount() - iTimer);

	_timestamp_::HighTimeCounter	st1;
	_timestamp_::HighTimeCounter	st2;

	iTimer = GetTickCount();
	for(int i = 0 ; i < iNum; i ++)
	{
		_timestamp_::HighTimeCounter	st11;
		_timestamp_::HighTimeCounter	st22;
		//int iys = st1.get() - st2.get();
	}
	printf("HighTime Utc timer:%d\r\n", GetTickCount() - iTimer);


	
	iTimer = GetTickCount();
	for(int i = 0 ; i < iNum; i ++)
	{
		int iys = st1.getMicroseconds() - st2.getMicroseconds();
	}
	printf("HighTime getMicroseconds timer:%d\r\n", GetTickCount() - iTimer);
}

int main(int argc, char* argv[])
{
	Test();
	std::cin.get();
	return 0;
}


/*#include <icrsint.h>
#include<iostream>
#include<iomanip>
#include <string>
using namespace std;
#import "c:\program files\common files\system\ado\msado15.dll"  no_namespace rename("EOF", "adoEOF")

void main()
{
	CoInitialize(NULL);
	_ConnectionPtr  sqlSp;
	HRESULT hr=sqlSp.CreateInstance(_uuidof(Connection));
	if(FAILED(hr))
	{
		cout<<"_ConnectionPtr对象指针实例化失败！！！"<<endl;
		return ;
	}
	else {
		try {
			_bstr_t strConnect="Driver={sql server};server=192.168.8.153,1433;uid=sa;pwd=P@ssw0rd;database=equipmenttable;";
			//_bstr_t strConnect="Provider=SQLOLEDB;Server=127.0.0.1,1433;Database=PBMS;uid=sa;pwd=pp471006459;";
			//_bstr_t strConnect="Provider=SQLOLEDB.1;Password=pp471006459;Persist Security Info=True;User ID=sa;"
			//"Initial Catalog=PBMS;Data Source=127.0.0.1,1433";
			//以上这三条strConnect语句都可以用！！看自己喜好选择用吧
			//要注意修改用户名uid、密码pwd、数据库名database，这三个参数均是你自己sql server的数据库信息
			sqlSp->Open(strConnect,"","",adModeUnknown);
		}
		catch(_com_error &e) {
			cout<<e.Description()<<endl;
		}
		_RecordsetPtr m_pRecordset; //记录集对象指针，用来执行SQL语句并记录查询结果
		if(FAILED(m_pRecordset.CreateInstance( _uuidof( Recordset ))))
		{
			cout<<"记录集对象指针实例化失败！"<<endl;
			return;
		}
		//char strSQL[]="SELECT * FROM  Books";
		try {
			m_pRecordset->Open("SELECT * FROM  equipmenttable",(IDispatch*)sqlSp,adOpenDynamic,adLockOptimistic, adCmdText);//打开数据库，执行SQL语句

		}
		catch (_com_error &e)
		{
			cout << e.Description()<<endl;
		}

		try {
			m_pRecordset->MoveFirst();
			while(!m_pRecordset->adoEOF) 
			{
				string bookname=(char*)(_bstr_t)(m_pRecordset->Fields->GetItem(_variant_t("MyUIDCode"))->Value);
				cout<<bookname<<endl;
				m_pRecordset->MoveNext();
			}
			 m_pRecordset->Close(); 

		}
		catch (_com_error &e)
		{
			cout << e.Description()<<endl;
		}

		char *sqlCommand = "INSERT INTO equipmenttable VALUES ('888888', 'aaa', 'bbb', 'ccc', 'ddd', 'eee', 'fff') WHERE not exists (select * from equipmenttable where equipmenttable.[MyUIDCode] = '888888'); ";
		sqlSp->Execute(sqlCommand, NULL, 1);//执行SQL语句 

		 char *sqlUpdate = "UPDATE equipmenttable SET [A相电流] = 'Fred', [B相电流] = 'Blue' WHERE [MyUIDCode] = '888888'; ";
		 sqlSp->Execute(sqlUpdate, NULL, 1);//执行SQL语句 

	}
	system("pause");
}*/