
#ifndef __EXTERNAL_20170417171104_1442199361_H__
#define __EXTERNAL_20170417171104_1442199361_H__

#include <libFoundation/modafx.h>
#include <dllLog/Exports.h>

#if defined( PLATFORM_OS_FAMILY_WINDOWS )
	#include <libIOCP_File/FileIO.h>
#elif defined( PLATFORM_OS_FAMILY_UNIX )
	#include <libEPoll_Regular_File/FileIO.h>
#endif


LOG_OBJ_DECL(Framework_AsyncFiles)

#endif//__EXTERNAL_20170417171104_1442199361_H__
