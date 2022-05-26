
#include <libFrameWork_WebsocketClients/WebsocketClients.h>

namespace _websocket_ {

	WebsocketClients::WebsocketClients(void)
	{

	}

	WebsocketClients::~WebsocketClients(void)
	{
		Release();
	}

	int WebsocketClients::Init(UInt8 uWorkerThreads)
	{
		//test
		LOG_Print_SetLog(libFrameWork_WebsocketClients, MLog_GetAllLevel());
		//test

		GetAsynStreamClientsInstance()->Init();
		GetTimerEventKMInstance()->Init();
		return 1;
	}

	void WebsocketClients::Release(void)
	{
		GetAsynStreamClientsInstance()->Release();
		GetTimerEventKMInstance()->Release();
	}

	WebsocketClients::DataSession_sptr	WebsocketClients::Open(const _string_t& sSvrIP, const UInt16 uSvrPort)
	{
		DataSession_sptr sptr = _clients_::DataSession::Alloc();
		if (sptr)
		{
			sptr->Init();
			if (sptr->Open(sSvrIP, uSvrPort) != 1)
			{
				sptr.reset();
			}
		}

		return sptr;
	}

	int WebsocketClients::Close(const DataSession_sptr& sptr)
	{
		sptr->Close();
		return 1;
	}

	int WebsocketClients::CloseAll(void)
	{
		_Lock.Lock();
		for (SessionMap::iterator iter = _Map.begin(); iter != _Map.end(); iter++)
		{
			iter->second->Close();
		}
		_Lock.UnLock();

		_Map.clear();
		return 1;
	}

	void WebsocketClients::HandleDestroy(NETHANDLE hNet)
	{
		DataSession_sptr sptr;
		_Lock.Lock();
		SessionMap::iterator iter = _Map.find(hNet);
		if (iter != _Map.end())
		{
			sptr = iter->second;
		}
		_Map.erase(hNet);
		_Lock.UnLock();

		sptr->Close();
	}
}

