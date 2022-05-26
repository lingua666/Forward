
#include <libFrameWork_FtpClients/FtpClients.h>

namespace _ftp_ {

	FtpClients::FtpClient_Impl_sptr FtpClients::Alloc(void)
	{
		static MemPool_type s_Pool(sizeof(FtpClient_Impl));
		return FtpClient_Impl_sptr(s_Pool.AllocObj<FtpClient_Impl>(),
			function20_bind_Ex(&MemPool_type::FreeObj<FtpClient_Impl>, &s_Pool));
	}

	FtpClients::FtpClients(void)
		: _Pool(sizeof(FtpClient_Impl))
		, _uSeq(0)
	{
		
	}

	FtpClients::~FtpClients(void)
	{
		Release();
	}

	int FtpClients::Init(const HFNSubscribe& hSubscribe, const HFNFilesSubscribe& hSubFiles)
	{
		//test
		LOG_Print_SetLog(libFrameWork_FtpClients, MLog_GetAllLevel())
		//test

		GetTimerEventInstance()->Init(1);
		GetAsynStreamClientsInstance()->Init(0, 0, true);
		GetAsyncFilesInstance()->Init();
		_hSubscribe = hSubscribe;
		_hSubFiles = hSubFiles;
		return 1;
	}

	void FtpClients::Release(void)
	{
		GetTimerEventInstance()->Release();
		GetAsynStreamClientsInstance()->Release();
		GetAsyncFilesInstance()->Release();
	}

	FtpClients::FtpClient_Impl_sptr	FtpClients::Open(const _ftp_string& sFtpIP, const UInt16 uFtpPort,
		const _ftp_string& sUserName, const _ftp_string& sPwd, bool isSync)
	{
		FtpClient_Impl_sptr sptr = FtpClients::Alloc();
		if (sptr)
		{
			sptr->Init(function20_bind_Ex(&FtpClients::HandleSubscribe, this),
				_hSubFiles);
			if (sptr->Open(sFtpIP, uFtpPort, sUserName, sPwd) == 1)
			{
				if (isSync)
				{//
					while (sptr->GetStatus() == _ftp_::EN_CODE_NULL)
					{
						Sleep(100);
					}
				}
			}
			else
			{
				sptr.reset();
			}
		}

		return sptr;
	}

	void FtpClients::HandleSubscribe(const FtpClient_Impl_sptr& sptr, int StatusCode)
	{
		switch (StatusCode)
		{
		case EN_LOGIN_SUCCESS:
			{
				_ClientsMap.insert(std::make_pair(sptr->Get(), sptr));
			}
			break;
		case EN_CMD_SESSION_DESTROY:
			{
				_ClientsMap.erase(sptr->Get());
			}
			break;
		}

		if (_hSubscribe)
		{
			_hSubscribe(sptr, StatusCode);
		}
	}

	int FtpClients::Close(const FtpClient_Impl_sptr& sptr)
	{
		sptr->Close();
		return 1;
	}

	int FtpClients::CloseAll(void)
	{
		for (ClientsMap::iterator iter = _ClientsMap.begin(); iter != _ClientsMap.end(); iter++)
		{
			iter->second->Close();
		}

		_ClientsMap.clear();
		return 1;
	}


}

