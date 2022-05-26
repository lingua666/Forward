
#include <libFrameWork_FtpSvr/FtpDataPorts.h>

namespace _ftp_ {

	FtpDataPorts::FtpDataPorts(void)
	{

	}

	FtpDataPorts::~FtpDataPorts(void)
	{
		Release();
	}

	int FtpDataPorts::Init(UInt16 uStart, UInt16 uEnd)
	{
		for (UInt16 i = uStart; i < uEnd; i++)
		{
			_Container.push_back(i);
		}

		return 1;
	}

	void FtpDataPorts::Release(void)
	{
		_Container.clear();
	}

	NETHANDLE FtpDataPorts::Open(const Stream_HAccept& hAccept)
	{
		NETHANDLE hNet = -1;
		if (_Container.size() <= 0)
			return hNet;

		UInt16 uPort = AllocPort();

		if (uPort > 0)
		{
			hNet = GetStreamSvrInstance_V20()->Listen(uPort, hAccept);
			if (hNet == -1)
			{
				FreePort(uPort);

				LOG_Print_Error(libFrameWork_FtpServer, "FtpDataPorts::Open(%d) Failed!!!", uPort);
			}
		}

		//test
		LOG_Print_Info(libFrameWork_FtpServer, "FtpDataPorts::Open(%d,%lld)!!!", uPort, hNet);
		//test

		return hNet;
	}

	int FtpDataPorts::Close(NETHANDLE hNet)
	{
		if (GetStreamSvrInstance_V20()->Stop(hNet) == 1)
		{
			FreePort(GetPortFromNETNODE(hNet));
			return 1;
		}

		return -1;
	}

	UInt16 FtpDataPorts::AllocPort(void)
	{
		if (_Container.size() <= 0)
			return 0;

		UInt16 uPort = 0;
		_Lock.Lock();
		uPort = _Container.front();
		_Container.pop_front();
		_Lock.UnLock();

		//test
		if (uPort == 0xFFFF)
		{
			int i = 0;
		}
		//test
		
		return uPort;
	}

	void FtpDataPorts::FreePort(UInt16 uPort)
	{
		if (uPort > 0)
		{
			_Lock.Lock();
			_Container.push_back(uPort);
			_Lock.UnLock();
		}
	}

	UInt16 FtpDataPorts::GetPort(NETHANDLE hNet)
	{
		return GetPortFromNETNODE(hNet);
	}

}

