// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <dllThirdWrap_Mysql/Exports.h>

int main(int argc, char* argv[])
{
	//test
	/*THandle	handle = (THandle)0x1234;
	tagTModuleInfoRecord* pp = ModulelIni(handle);*/
	//test

	bool isResult;
	isResult = Mysql_Init();
	//isResult = Mysql_Connect("192.168.0.120",3306,"weiwo","123","microwodb",DBTABLE_CHARACTERS_UTF8);
	isResult = Mysql_Connect("112.35.27.134", 3548, "chinamobile", "d10qE0DWQlTl5W@y56GB7MihoD23dOdT", "microwodb", DBTABLE_CHARACTERS_UTF8);
	if( isResult )
	{
		//Mysql_CreateDB("dll_test");
		//Mysql_DeleteDB("dll_test");

		//²éÑ¯´æ´¢¹ý³Ì
		HMRecordSet hRSet = Mysql_Query_Open("call proc_poc_v8_terminal_login(\"\", \"\",\"\",0,\"\",\"\")");
		const char* sz = Mysql_Query_GetValue(hRSet, 0);
		Mysql_Query_Close(hRSet);


		hRSet = Mysql_Query_Open("select * from log_traffic");
		if( hRSet != NULL )
		{
			for(int i = 0; i < 5; i ++)
				printf("%s\r\n",Mysql_Query_GetValue(hRSet,i));

			printf("int: %d\r\n",Mysql_Query_ValueToInt(Mysql_Query_GetValue(hRSet,0)));
			printf("double: %f\r\n",Mysql_Query_ValueToDouble(Mysql_Query_GetValue(hRSet,3)));
		}
	}

	Mysql_DisConnect();
	std::cin.get();
	return 0;
}