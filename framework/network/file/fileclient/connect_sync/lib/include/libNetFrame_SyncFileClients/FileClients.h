
#ifndef __MYFILECLIENT_20150924143440_1443076480_H__
#define __MYFILECLIENT_20150924143440_1443076480_H__

#include <libNetFrame_SyncMediaClient/MediaClients.h>

namespace	_client_{

	namespace	_sync_{

		typedef MediaClients	FileClients;

	} // namespace	_sync_

} // namespace _client_

inline _client_::_sync_::FileClients* GetSyncFileClientsInstance( void )
{
	static _client_::_sync_::FileClients* pClients = GetSyncMediaClientsInstance();
	return pClients;
}

#endif//__MYFILECLIENT_20150924143440_1443076480_H__
