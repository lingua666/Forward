// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <libFoundation/modafx.h>
#include <dllThirdWrap_Sqlite/Exports.h>

void Test( void )
{
	bool isResult;
	isResult = MSQLite_Connect("D:\\SystemDB.dat");
	if( isResult )
	{
		bool bl = MSQLite_ExecuteSQL("CREATE TABLE IF NOT EXISTS card_info(\
									 ID 		 		INTEGER PRIMARY KEY AUTOINCREMENT,\
									 CARD_ID 	INTEGER	UNIQUE NOT NULL,\
									 NAME	TEXT    NOT NULL,\
									 SEX	TEXT     NOT NULL,\
									 BIRTHDAY TEXT  NOT NULL,\
									 NATION  TEXT   NOT NULL,\
									 CARD_STAUT TEXT NOT NULL,\
									 DEVICE_PORT TEXT NOT NULL,\
									 QianFaJiGou TEXT NOT NULL,\
									 ADDRESS TEXT,\
									 REMARKS TEXT   NOT NULL);");

		HMRecordSet hRSet = MSQLite_Query_Open("select * from log_traffic");
		if( hRSet != NULL )
		{
			for(int j = 0; j < MSQLite_Query_RowCount(hRSet); j ++)
			{
				for(int i = 0; i < MSQLite_Query_ColumnCount(hRSet); i ++)
					printf("%s\r\n",MSQLite_Query_GetValue(hRSet,i));

				MSQLite_Query_MoveNext(hRSet);
			}

			printf("int: %d\r\n",MSQLite_Query_ValueToInt(MSQLite_Query_GetValue(hRSet,0)));
			printf("double: %f\r\n",MSQLite_Query_ValueToDouble(MSQLite_Query_GetValue(hRSet,3)));
		}

		MSQLite_Query_Close(hRSet);
	}
	std::cin.get();

	MSQLite_DisConnect();
}

void InsertTest( void )
{
	bool isResult;
	isResult = MSQLite_Connect("E:\\ProjectCode\\Deploy\\trunk\\Project\\ATS\\主消息中心\\软件\\主消息中心\\Data\\MRoute.dat");
	if( isResult )
	{
		const char* RouteCentreUidHead = "1B000001000F20160530000000000ZZZ";
		const char* IC2000SvrUidHead = "1B000001000K20180911000000000ZZ1";

		_string_type sRouteCentreName = "级联消息中心";
		sRouteCentreName = sRouteCentreName.gbk_utf8();

		_string_type sIC2000SvrName = "IC2000设备管理服务器";
		sIC2000SvrName = sIC2000SvrName.gbk_utf8();

		char szSql[1024] = {0};
		for(int i = 1; i < 500; i ++)
		{
			int iValue = BigEndian32_UInt32(i);
			_string_type sUid = RouteCentreUidHead;
			sUid += _string_type::HexToStr((char*)&iValue, sizeof(int));
			sprintf(szSql, "insert into authorize (uid,remarks) values('%s','%s%d');", sUid.c_str(), sRouteCentreName.c_str(), i);
			MSQLite_ExecuteSQL(szSql);

			printf("RouteCentre uid:%s\r\n", sUid.c_str());
		}

		for(int i = 0; i < 500; i ++)
		{
			int iValue = BigEndian32_UInt32(i);
			_string_type sUid = IC2000SvrUidHead;
			sUid += _string_type::HexToStr((char*)&iValue, sizeof(int));
			sprintf(szSql, "insert into authorize (uid,remarks) values('%s','%s%d');", sUid.c_str(), sIC2000SvrName.c_str(), i);
			MSQLite_ExecuteSQL(szSql);

			printf("IC2000SVR uid:%s\r\n", sUid.c_str());
		}
	}
}


int main(int argc, char* argv[])
{
	InsertTest();
	return 0;
}