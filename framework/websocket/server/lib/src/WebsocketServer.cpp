
#include <libFrameWork_WebsocketSvr/WebsocketServer.h>

#define WEBSOCKET_CHECK_TIME_DEF		5000

namespace _websocket_ {

	namespace _server_ {

		WebsocketServer::WebsocketServer(void)
		{

		}

		WebsocketServer::~WebsocketServer(void)
		{
			Release();
		}

		int WebsocketServer::Init(UInt8 uWorkerThreads)
		{
			//test
			LOG_Print_SetLog(libFrameWork_WebsocketSvr, MLog_GetAllLevel());
			//test

			GetStreamSvrInstance()->Init(uWorkerThreads);
			GetTimerEventKMInstance()->Init();
			_Subscribe.Init(DataSession::EN_EVENT_OPEN, DataSession::EN_EVENT_COUNT);
			return 1;
		}

		void WebsocketServer::Release(void)
		{
			GetStreamSvrInstance()->Release();
			GetTimerEventKMInstance()->Release();
			_Lock.Lock();
			_Map.clear();
			_Lock.UnLock();
			_Subscribe.Release();
		}

		int WebsocketServer::Run(UInt16 uPort, UInt8 uIOThreadNum)
		{
			if (!GetStreamSvrInstance()->Listen(uPort,
				function20_bind_Ex(&WebsocketServer::HandleAccept, this)))
				return -1;

			GetStreamSvrInstance()->Run(uIOThreadNum, true);
			return 1;
		}

		int WebsocketServer::Send(NETHANDLE hNet, const char* c_szData, UInt32 uSize)
		{
			return GetStreamSvrInstance()->Send(hNet, c_szData, uSize);
		}

		void WebsocketServer::Close(NETHANDLE hNet)
		{
			GetStreamSvrInstance()->Close(hNet);
		}

		int WebsocketServer::AddConsumer(DataSession::enSubscribe_Event Type, const HFNSubsEvent& hEvent)
		{
			return _Subscribe.AddConsumer(Type, hEvent);
		}

		void	WebsocketServer::HandleAccept(const char* c_szIP, UInt16 u16Port, NETHANDLE hNet)
		{
			GetStreamSvrInstance()->SetRecvHandle(hNet, function20_bind_Ex(&WebsocketServer::HandleToken, this));
			if (GetStreamSvrInstance()->SetDestroyHandle(hNet,
				function20_bind_Ex(&WebsocketServer::HandleDestroy, this)) == 1)
			{
				GetTimerEventKMInstance()->push_back(function20_bind(&WebsocketServer::HandleCheck, this, hNet),
					WEBSOCKET_CHECK_TIME_DEF);
			}
		}

		void	WebsocketServer::HandleCheck(NETHANDLE hNet)
		{
			if (_Map.find(hNet) == _Map.end())
			{//超时
				GetStreamSvrInstance()->Close(hNet);
			}
		}

		void WebsocketServer::HandleDestroy(NETHANDLE hNet)
		{

		}

		void WebsocketServer::HandleToken(NETHANDLE hNet, const StreamBuf_ptr& Stream_sptr)
		{
			if (Stream_sptr->payload > 0)
			{
				//test
				LOG_Print_Info(libFrameWork_WebsocketSvr, "WebsocketServer::HandleToken(%d) Data:%s",
					Stream_sptr->payload, Stream_sptr->data);
				//test

				_websocket_::CHandSharke Sharke;
				Sharke.Init();

				int  iRead = 0;
				_websocket_::_string_t sRet = Sharke.Parse(Stream_sptr->data, Stream_sptr->payload, &iRead);
				if (iRead == -1)
				{
					LOG_Print_Error(libFrameWork_WebsocketSvr, "WebsocketServer::HandleToken(%d) Failed!!");
					return;
				}

				DataSession_sptr sptr = _server_::DataSession::Alloc();
				if (sptr->Init(hNet, function20_bind_Ex(&WebsocketServer::Send, this)) == 1)
				{
					//响应客户端
					_websocket_::_string_t s = Sharke.Responds(sRet.c_str());
					GetStreamSvrInstance()->Send(hNet, s.c_str(), s.size());
					_Map.insert(std::make_pair(hNet, sptr));

					sptr->SetDestroyHandle(function20_bind_Ex(&WebsocketServer::HandleSessionDestroy, this));

					_Subscribe.Publish(DataSession::EN_EVENT_OPEN, sptr, NULL, NULL);
				}
			}
		}

		void WebsocketServer::Stop(void)
		{
			GetStreamSvrInstance()->Stop();
		}

		void WebsocketServer::HandleSessionDestroy(const DataSession_sptr& sptr)
		{
			_Lock.Lock();
			_Map.erase(sptr->Get());
			_Lock.UnLock();

			_Subscribe.Publish(DataSession::EN_EVENT_CLOSE, sptr, NULL, NULL);
		}

	}

}

