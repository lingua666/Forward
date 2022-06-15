
#ifndef __WEBSOCKET_SERVER_20201222154204_3344199361_H__
#define __WEBSOCKET_SERVER_20201222154204_3344199361_H__

#include <libFrameWork_WebsocketSvr/DataSession.h>

namespace _websocket_ {

	namespace _server_ {

		class WebsocketServer
		{
		public:
			typedef _server_::DataSession::self_sptr	DataSession_sptr;
			typedef Container_HashMap_type<NETHANDLE, DataSession_sptr>	SessionMap;

			typedef SubscribeEvents<void(const DataSession_sptr&, const char*, UInt64)>	SubscribeEvents_type;
			typedef SubscribeEvents_type::HFNEvent		HFNSubsEvent;

		public:

			WebsocketServer(void);

			~WebsocketServer(void);

			int Init(UInt8 uWorkerThreads = 0);

			void Release(void);

			int Run(UInt16 uPort, UInt8 uIOThreadNum = 0);

			void	Stop(void);

			int Send(NETHANDLE hNet, const char* c_szData, UInt32 uSize);

			void Close(NETHANDLE hNet);

			int AddConsumer(DataSession::enSubscribe_Event Type, const HFNSubsEvent& hEvent);

		protected:

			void	HandleAccept(const char* c_szIP, UInt16 u16Port, NETHANDLE hNet);

			void	HandleCheck(NETHANDLE hNet);

			void	HandleDestroy(NETHANDLE hNet);

			void	HandleToken(NETHANDLE hNet, const StreamBuf_ptr& Stream_sptr);

			void	HandleSessionDestroy(const DataSession_sptr& sptr);

		private:
			SessionMap	_Map;
			CLock		_Lock;
			SubscribeEvents_type	_Subscribe;
		};

	}

}

#endif

