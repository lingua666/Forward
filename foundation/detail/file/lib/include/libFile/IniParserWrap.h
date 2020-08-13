#ifndef __INIPARSER_2017073105854_1446224334_H__
#define __INIPARSER_2017073105854_1446224334_H__

#include <libCommon/modafx.h>

#if defined(PLATFORM_OS_FAMILY_UNIX)

typedef void* INIHANDLE;

#ifdef __cplusplus
extern "C" {
#endif

INIHANDLE Ini_Load( const char* c_szPath );

int Ini_Set( INIHANDLE ini, const char * entry, const char * val );

char* Ini_GetString( INIHANDLE ini, const char * key, char * def );

int Ini_GetInt( INIHANDLE ini, const char * key, int notfound );

void Ini_Freedict( INIHANDLE ini );

#ifdef  __cplusplus
}
#endif

#endif

#endif