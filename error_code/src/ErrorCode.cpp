
#include <libApps_ErrorCode/ErrorCode.h>

static int g_ErrorCode_20160612 = 0;

void Apps_SetErrorCode( int iErrorCode )
{
	g_ErrorCode_20160612 = iErrorCode;
}

int Apps_GetErrorCode( void )
{
	return (-1) * g_ErrorCode_20160612;
}

bool Apps_EqualErrorCode( int iErrorCode )
{
	return g_ErrorCode_20160612 == iErrorCode;
}

_string_type Apps_FromErrorToString( int iErrorCode )
{
	char szError[20] = {0};
	sprintf(szError, "%d", iErrorCode);
	return szError;
}

_string_type Apps_GetErrorToString( void )
{
	char szError[20] = {0};
	sprintf(szError, "%d", g_ErrorCode_20160612);
	return szError;
}
