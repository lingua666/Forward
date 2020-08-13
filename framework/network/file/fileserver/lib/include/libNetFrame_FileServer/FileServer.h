
#ifndef __MYFILESERVER_20150924143440_1443076480_H__
#define __MYFILESERVER_20150924143440_1443076480_H__

#include <libNetFrame_StreamServer/StreamServer.h>

namespace	_server_{

	typedef StreamServer	FileServer;

} // server

inline _server_::FileServer* GetFileServerInstance( void )
{
	static _server_::FileServer* pServer = GetStreamSvrInstance();
	return pServer;
}

#endif//__MYFILECLIENT_20150924143440_1443076480_H__
