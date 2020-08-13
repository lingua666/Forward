
#include <libFile/IniParserWrap.h>

#if defined(PLATFORM_OS_FAMILY_UNIX)

#ifdef __cplusplus
extern "C" {
#endif

#include "../iniparser-3.1/iniparser.h"

INIHANDLE Ini_Load( const char* c_szPath )
{
	return (INIHANDLE)iniparser_load(c_szPath);
}

int Ini_Set( INIHANDLE ini, const char * entry, const char * val )
{
	return iniparser_set((dictionary *)ini, entry, val);
}

char* Ini_GetString( INIHANDLE ini, const char * key, char * def )
{
	return iniparser_getstring((dictionary *)ini, key, def);
}

int Ini_GetInt( INIHANDLE ini, const char * key, int notfound )
{
	return iniparser_getint((dictionary *)ini, key, notfound);
}

void Ini_Freedict( INIHANDLE ini )
{
	iniparser_freedict((dictionary *)ini) ;
}

#ifdef  __cplusplus
}
#endif

#endif