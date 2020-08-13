
#ifndef __MYFILECLIENT_20150924143440_1443076480_H__
#define __MYFILECLIENT_20150924143440_1443076480_H__

#include <libNetFrame_AsyncStreamClients/StreamClients.h>

namespace	_client_{
	
	namespace	_async_{

		typedef StreamClients	FileClients;

	} // namespace	_async_

} // namespace	_client_

inline _client_::_async_::FileClients* GetAsynFileClientsInstance( void )
{
	static _client_::_async_::FileClients* pClients = GetAsynStreamClientsInstance();
	return pClients;
}

#endif//__MYFILECLIENT_20150924143440_1443076480_H__
