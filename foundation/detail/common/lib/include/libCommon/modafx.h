
#ifndef __MODAFX_20160518173305_1438860785_H__
#define __MODAFX_20160518173305_1438860785_H__

#include <libCommon/os/Platform.h>
#include <libCommon/os/Types.h>
#include <libCommon/os/error.h>

#define IS_INVALID_ZERO(p)    \
	if(p == NULL) { return 0; }

#define IS_INVALID_NONE(p)    \
	if(p == NULL) { return ; }

#include <libCommon/macro/Macro_for.hpp>
#include <libCommon/Type_Basis.h>
#include <libCommon/ByteOP.h>
#include <libCommon/Struct.h>
#include <libCommon/SysInfo.h>
#include <libCommon/CMyException.h>
#include <libCommon/WDump.h>

#include <string>

inline void __itoa__(int iValue, char* szBuf, int iLen )
{
	snprintf(szBuf, (size_t)iLen, "%d", iValue);
}

#define _NCFW_VER_	0x1000

#endif // __MODAFX_20160518173305_1438860785_H__
