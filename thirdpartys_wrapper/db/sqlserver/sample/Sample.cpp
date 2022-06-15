// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <dllThirdWrap_SqlServer/Exports.h>
#include <libFoundation/modafx.h>

/////////////////////////////////SQL SERVER 2008//////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN
#include <icrsint.h>
#include<iostream>
#include<iomanip>
#include <string>
using namespace std;
#import "c:\program files\common files\system\ado\msado15.dll"  no_namespace rename("EOF", "adoEOF")

_ConnectionPtr  g_pConnection;
_RecordsetPtr	g_pRecordset; //记录集对象指针，用来执行SQL语句并记录查询结果
Packet<PHead>	g_Packet(1470, 0);

void SqlServer_Execute(const char* szSql)
{
	try
	{
		g_pConnection->Execute(szSql, NULL, 1);//执行SQL语句 
	}
	catch (_com_error &e)
	{
		printf("sql error:%s\r\n", szSql);
		cout << "SqlServer_Execute error:" << e.Description() << endl;
	}
}

void SqlServer_Parse(const PKBuf_ptr& Buf)
{
	_string_type sData(Buf->_Data, Buf->_uPayload);
	_string_type sContent;
	_string_type sUid;
	_string_type sFieldName;
	_string_type sFieldData;

	int OldPos = 0, Pos = 0;

	do
	{
		Pos = sData.find(";", Pos);
		if (Pos != -1 || OldPos < sData.size())
		{
			if (Pos == -1)
				Pos = sData.size();

			sContent = _string_type(&sData[OldPos], Pos - OldPos);
			if (OldPos != 0)
			{
				sFieldName += ",";
				sFieldData += ",";
			}

			int ps = sContent.find(":");
			if (ps == -1)
				break;

			if (OldPos == 0)
				sUid.append(&sContent[ps + 1], sContent.size() - (ps + 1));

			sFieldName.append(&sContent[0], ps);
			sFieldData.append(&sContent[ps + 1], sContent.size() - (ps + 1));
			Pos++;
			OldPos = Pos;
		}
	} while (Pos != -1);

	printf("字段名:%s\r\n", sFieldName.c_str());
	printf("字段内容:%s\r\n", sFieldData.c_str());

	_string_type s;
	try
	{
		s = "SELECT * FROM equipmenttable WHERE MyUIDCode ='";
		s += sUid + "';";
		g_pRecordset->Open(s.c_str(), (IDispatch*)g_pConnection, adOpenDynamic, adLockOptimistic, adCmdText);//打开数据库，执行SQL语句

	}
	catch (_com_error &e)
	{
		printf("sql error:%s\r\n", s.c_str());
		cout << "SqlServer_Parse error1:" << e.Description() << endl;
	}

	try
	{
		if (g_pRecordset->adoEOF)
		{
			s = "INSERT INTO equipmenttable(";
			s += sFieldName + ") ";
			s += "VALUES (";
			s += sFieldData + ");";
			g_pConnection->Execute(s.c_str(), NULL, 1);//执行SQL语句 
		}
		else
		{
			int OldPos_Name = 0, Pos_Name = 0;
			int OldPos_Data = 0, Pos_Data = 0;

			s = "UPDATE equipmenttable SET ";

			do
			{
				Pos_Name = sFieldName.find(",", Pos_Name);
				if (Pos_Name == -1 && OldPos_Name >= sFieldName.size())
					break;

				if (Pos_Name == -1)
					Pos_Name = sFieldName.size();

				Pos_Data = sFieldData.find(",", Pos_Data);
				if (Pos_Data == -1 && OldPos_Data >= sFieldData.size())
				{
					printf("SqlServer_Parse error3: sFieldName:%s, sFieldData:%s\r\n", sFieldName.c_str(), sFieldData.c_str());
					break;
				}

				if (Pos_Data == -1)
					Pos_Data = sFieldData.size();

				if (_string_type(&sFieldName[OldPos_Name], Pos_Name - OldPos_Name) != "MyUIDCode")
				{
					if (OldPos_Name != 0 && OldPos_Name != strlen("MyUIDCode") + 1)
						s += ",";

					s += "[";
					s.append(&sFieldName[OldPos_Name], Pos_Name - OldPos_Name);
					s += "] = ";
					s += "'";
					s.append(&sFieldData[OldPos_Data], Pos_Data - OldPos_Data);
					s += "'";
				}

				Pos_Data++;
				Pos_Name++;
				OldPos_Data = Pos_Data;
				OldPos_Name = Pos_Name;

			} while (Pos_Name != -1);

			s += " WHERE [MyUIDCode] = '";
			s += sUid;
			s += "';";
			g_pConnection->Execute(s.c_str(), NULL, 1);//执行SQL语句 
		}
		g_pRecordset->Close();
	}
	catch (_com_error &e)
	{
		printf("sql error:%s\r\n", s.c_str());
		cout << "SqlServer_Parse error2:" << e.Description() << endl;
	}
}

void SqlSever_Open(const char* c_szIP, UInt16 uPort,
	const char* c_szUserName, const char* c_szPwd)
{
	g_Packet.SetFunction(SqlServer_Parse);

	CoInitialize(NULL);
	HRESULT hr = g_pConnection.CreateInstance(_uuidof(Connection));
	if (FAILED(hr))
	{
		printf("_ConnectionPtr对象指针实例化失败！！！\r\n");
		return;
	}
	else
	{
		_string_type s;
		try
		{
			s = "Driver={sql server};server=";
			s += c_szIP;
			s += ",";
			s += _string_type::NumberToStr(uPort);
			s += ";";
			s += "uid=";
			s += c_szUserName;
			s += ";";
			s += "pwd=";
			s += c_szPwd;
			s += ";database=equipmenttable;";

			_bstr_t strConnect = s.c_str();//"Driver={sql server};server=192.168.8.153,1433;uid=sa;pwd=P@ssw0rd;database=equipmenttable;";
										   //要注意修改用户名uid、密码pwd、数据库名database，这三个参数均是你自己sql server的数据库信息
			g_pConnection->Open(strConnect, "", "", adModeUnknown);
		}
		catch (_com_error &e)
		{
			printf("sql error:%s\r\n", s.c_str());
			cout << "SqlSever_Open error:" << e.Description() << endl;
		}

		if (FAILED(g_pRecordset.CreateInstance(_uuidof(Recordset))))
		{
			printf("记录集对象指针实例化失败！\r\n");
			return;
		}

		/*try
		{
		g_pRecordset->Open("SELECT * FROM  equipmenttable",(IDispatch*)sqlSp,adOpenDynamic,adLockOptimistic, adCmdText);//打开数据库，执行SQL语句

		}
		catch (_com_error &e)
		{
		cout << e.Description()<<endl;
		}

		try
		{
		g_pRecordset->MoveFirst();
		while(!g_pRecordset->adoEOF)
		{
		string bookname=(char*)(_bstr_t)(g_pRecordset->Fields->GetItem(_variant_t("MyUIDCode"))->Value);
		cout<<bookname<<endl;
		g_pRecordset->MoveNext();
		}
		g_pRecordset->Close();

		}
		catch (_com_error &e)
		{
		cout << e.Description()<<endl;
		}*/
	}
}

void SqlServer_Close(void)
{
	g_pRecordset->Close();
	g_pConnection->Close();
	::CoUninitialize();
}

int main(int argc, char* argv[])
{
	_string_type sDB_IP = _file_::RWIni::Read("DB","IP",s.c_str());
	const int iDB_Port = _file_::RWIni::ReadInt("DB","Port",s.c_str());
	_string_type sDB_UserName = _file_::RWIni::Read("DB","UserName",s.c_str());
	_string_type sDB_Password = _file_::RWIni::Read("DB","Password",s.c_str());*

	SqlSever_Open(sDB_IP.c_str(), iDB_Port, sDB_UserName.c_str(), sDB_Password.c_str());



	SqlServer_Close();
	std::cin.get();
	return 0;
}