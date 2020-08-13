// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <libValidityCheck/FileCheck.h>
#include <dllValidityCheck/Exports.h>

bool FailedProcess( int iValue )
{
	printf("error\r\n");
	return true;
}

bool _CALLTYPE MValidity_CheckFaild( const char* c_szFilePath, int iValue )
{
	printf("eeeeeeee\r\n");
	return true;
}

int main(int argc, char* argv[])
{
	MValidity_Check(MValidity_CheckFaild);

	/*while(true)
	{
		_validity_::CheckValidity::EncodeFile("D:\\test.exe");

		_validity_::CheckValidity Check("D:\\test.exe",
			function20_bind_Ex(FailedProcess));

		printf("11111\r\n");
		Sleep(100);
	}*/

	/*_file_::CMyFile hFile;
	hFile.Open("F:\\ProjectManage\\ReleaseTest\\NetModules\\Test\\MyCommandServerSample.exe","rb");
	char szKey[200] = {0};
	FileCheck::ReadKey(hFile, szKey, sizeof(szKey));
	
	char szOut[200] = {0};
	FileCheck::Encode(hFile, szOut, sizeof(szOut), "<@广州传蔚智能科技有限公司@>" );

	if( memcmp(szKey, szOut, sizeof(szKey)) == 0 )
	{
		int i = 0;
	}
	else
	{
		int y = 0;
	}*/

	std::cin.get();
	return 0;
}