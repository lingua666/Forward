// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <libTools_PE_Info/PE_Analysis.h>

int main(int argc, char* argv[])
{
	/*char fullpath[1024] = {0};
	GetModuleFileNameA(NULL,fullpath,sizeof(fullpath));
	PEFileInfo pe(fullpath);*/
	PEFileInfo pe("D:\\Test\\qq.exe");
	//pe.ChangeExeIcon("F:\\ProjectManage\\PEControl\\de_win.ICO");
	//PEFileInfo pe("d:\\SampleUUID.exe");
	pe.Parse();

	std::string s = pe.GetValue(FileVersion);
	pe.SetValue(FileVersion, "V2.0.1010");
	pe.SetValue(ProductName, "¹ãÖÝ´«Îµ");
	pe.SetValue(FileDescription, "88");
	pe.SetValue(ProductVersion, "88");
	pe.Update();

	std::cin.get();
	return 0;
}
