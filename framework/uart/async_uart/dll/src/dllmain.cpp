// dllmain.cpp : Defines the entry point for the DLL application.

#include <libCommon/os/Platform.h>

#if defined(PLATFORM_OS_FAMILY_WINDOWS)
#include <dllFramework_AsyncUarts/Exports.h>
#include <libValidityCheck/FileCheck.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

LOG_OBJ_DECL(Err_Framework_AsyncUarts_Module)

CHECK_FILE_FAILED_DEF(Err_Framework_AsyncUarts_Module)

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:

		LOAD_DLL_INIT(hModule)

		break;
	case DLL_THREAD_ATTACH:

		break;
	case DLL_THREAD_DETACH:

		break;
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}

#endif

