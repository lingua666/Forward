
#ifndef __EXTERNAL_MODBUS_20170616104004_1444199361_H__
#define __EXTERNAL_MODBUS_20170616104004_1444199361_H__

#include <libNetFrame_AsyncStreamClients/StreamClients.h>
#include <libNetFrame_StreamServer/StreamServer.h>
#include <libNetFrame_StreamServer_V20/StreamServer_V20.h>
#include <libFrameWork_AsyncUarts/AsyncUarts.h>
#include <libUUID/MD5Engine.h>
#include <modbus.h>
#include <modbus-private.h>
#include <dllCommon/DllCommon.h>

namespace _modbus_ {
	typedef _foundation_::_string_t			_mod_string;
	typedef _foundation_::_string_big_t		_mod_string_big;
}

LOG_OBJ_DECL(libFrameWork_Modbus)

#endif//__EXTERNAL_20161014105604_1444199361_H__
