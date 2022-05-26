
#ifndef __WEBSOCKET_CLIENTS_20201222154204_3344199361_H__
#define __WEBSOCKET_CLIENTS_20201222154204_3344199361_H__

#include <libFrameWork_WebsocketClients/DataSession.h>

namespace _websocket_ {

	class WebsocketClients
	{
	public:
		typedef _clients_::DataSession::self_sptr	DataSession_sptr;
		typedef Container_HashMap_type<NETHANDLE, DataSession_sptr>	SessionMap;

	public:

		WebsocketClients(void);

		~WebsocketClients(void);

		int Init(UInt8 uWorkerThreads = 0);

		void Release(void);

		DataSession_sptr	Open(const _string_t& sSvrIP, const UInt16 uSvrPort);

		int Close(const DataSession_sptr& sptr);

		int CloseAll(void);

	protected:

		void	HandleDestroy(NETHANDLE hNet);

	private:
		SessionMap	_Map;
		CLock		_Lock;
	};

}

#endif

