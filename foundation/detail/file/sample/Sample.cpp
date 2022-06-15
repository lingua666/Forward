// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <libFile/CMyFile.h>
#include <libFile/RWIni.h>

void FileNotify( const char* c_szPath, const char* c_szFileName )
{
	_string_type sPath(c_szPath);
	sPath += SPLIT_STRING_DEF;
	sPath += c_szFileName;

	_string_type s = _file_::CMyFile::GetDirPath(sPath);
	_string_type s1 = _file_::CMyFile::GetFileName(sPath);
	_string_type s2 = _file_::CMyFile::GetSuffix(sPath);
	printf("%s\r\n", sPath.c_str());
	printf("GetDirPath :%s\r\n", s.c_str());
	printf("GetFileName :%s\r\n", s1.c_str());
	printf("GetSuffix :%s\r\n", s2.c_str());
}

int main(int argc, char* argv[])
{
	_string_type s22("D:\\Test\\Test\\Log\\ThirdW_Haik_Module\\");
	_string_type s11 = s22;
	s11 = s11.strim("\\");

	_string_type s333("D:\\Test\\Test\\Log\\ThirdW_Haik_Module\\");
	_string_type s31 = s333;
	s31 = s31.replace("Test","111");

	_string_type s = _file_::CMyDirectory::GetCurrentDirPath();
	//_file_::CMyDirectory::Create("d:\\123\\1\\");
	_file_::CMyDirectory::Create("D:\\Test\\Test\\Log\\ThirdW_Haik_Module\\");
	_file_::CMyDirectory::Recursive("D:\\Work\\ProjectCode\\bulid\\x86\\NetControl\\bin\\", function20_bind_Ex(FileNotify));

	std::cin.get();
	return 0;
}