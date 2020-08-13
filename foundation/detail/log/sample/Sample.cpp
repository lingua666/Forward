// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <libLog/Logger.h>
#include <dllLog/Exports.h>

LOG_OBJ_DECL(SampleLog)


static void CheckFailed( int iValue )
{

}

int main(int argc, char* argv[])
{
	LOG_Print_SetLog(SampleLog, 0)
	while(true)
	{
		LOG_Print_Info(SampleLog, "11111111")
		Sleep(1000);
	}


	_log_::Console_Warning("fsafjsadlfjl%d",10);
	_log_::Console_Error("fsafjsadlfjl%d",10);

	printf("111111");

	_log_::Logger<_log_::LogFile_Date,0>	log("d:\\1.txt");
	log.Info("1111111123%d%s",10,"1234");
	Sleep(5000);
	log.Info("1111111123%d%s",10,"1234");

	ILog* pl = MLog_AllocObj();
	pl->SetLevel(0);
	char szPath[1024] = {0};
	int len = MLog_GetModuleLogPath("LogTest", szPath, sizeof(szPath));
	pl->Open(szPath);
	pl->Debug("1234567890");
	pl->Close();
	MLog_FreeObj(pl);

	std::cin.get();
	return 0;
}