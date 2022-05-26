
#include <libFrameWork_FtpSvr/FtpCmdSession.h>
#include <libFrameWork_FtpSvr/FtpDataPorts.h>

namespace _ftp_ {

	FtpDataSession::self_sptr FtpDataSession::Alloc(void)
	{
		static MemPool_type s_Pool(sizeof(self_type));
		return self_sptr(s_Pool.AllocObj<self_type>(),
			function20_bind_Ex(&MemPool_type::FreeObj<self_type>, &s_Pool));
	}

	FtpDataSession::FtpDataSession(void)
		: _isOpen(false)
	{
		_Info.hListen = -1;
		_Info.hData = -1;
		_Info.uListenPort = 0;
	}

	FtpDataSession::~FtpDataSession(void)
	{
		Release();

		//test
		LOG_Print_Info(libFrameWork_FtpServer, "FtpDataSession::~FtpDataSession(%p, %d)", this, _Info.uListenPort);
		//test
	}

	int FtpDataSession::Init(void)
	{
		return 1;
	}

	void FtpDataSession::Release(void)
	{
		Close();
		_Info.hListen = -1;
		_Info.hData = -1;
		_Info.uListenPort = 0;
	}

	UInt16 FtpDataSession::Open(void)
	{
		_Info.hListen = FtpDataPorts::GetFtpDataPortsInstance()->Open(
			function20_bind_Ex(&FtpDataSession::HandleDataAccept, shared_from_this()));

		if (_Info.hListen == -1)
		{
			LOG_Print_Info(libFrameWork_FtpServer, "FtpDataSession::Open(%p) Failed!", this);
			return 0;
		}

		_isOpen = true;
		_Info.uListenPort = FtpDataPorts::GetFtpDataPortsInstance()->GetPort(_Info.hListen);

		//5秒检测是否有连接接入，没有则断开连接
		_sEventUuid = GetTimerEventKMInstance()->push_back(
			function20_bind(&FtpDataSession::HandleWaitSession, shared_from_this(), _Info.hListen),
			5000);

		//test
		LOG_Print_Info(libFrameWork_FtpServer, "FtpDataSession::Open(%p, %d)", this, _Info.uListenPort);
		//test
		
		return _Info.uListenPort;
	}

	void FtpDataSession::HandleWaitSession(NETHANDLE hListen)
	{
		if (GetStreamSvrInstance_V20()->GetSessionCount(hListen) <= 0)
		{
			GetStreamSvrInstance_V20()->Stop(hListen);

			if (hListen == _Info.hListen)
				_Info.hListen = -1;

			LOG_Print_Error(libFrameWork_FtpServer, "FtpDataSession::HandleWaitSession(%d) TimeOut",
				FtpDataPorts::GetFtpDataPortsInstance()->GetPort(hListen));
		}
	}

	void FtpDataSession::HandleDataAccept(NETHANDLE hListen, const char* c_szClientIP,
		UInt16 uClientPort, NETHANDLE hNet)
	{
		//test
		LOG_Print_Info(libFrameWork_FtpServer, "FtpDataSession::HandleDataAccept(%d,%p,%d)",
			FtpDataPorts::GetFtpDataPortsInstance()->GetPort(hListen), this, _Info.uListenPort);
		//test

		if (hNet != -1)
		{
			if (_Info.hData != -1)
				GetStreamSvrInstance_V20()->Close(hNet);

			GetStreamSvrInstance_V20()->SetRecvHandle(hNet,
				function20_bind(&FtpDataSession::HandleRecv, shared_from_this(), -1,
					_function_::_1, _function_::_2));

			if (GetStreamSvrInstance_V20()->SetDestroyHandle(hNet,
				function20_bind(&FtpDataSession::HandleDestroy, shared_from_this(), -1, _function_::_1)) == 1)
			{
				GetTimerEventKMInstance()->Remove(_sEventUuid);

				_Info.hData = hNet;
				if (_hReady)
				{
					_hReady(shared_from_this());
				}

				//5秒检测
				_sEventUuid = GetTimerEventKMInstance()->push_back(
					function20_bind(&FtpDataSession::HandleDataCheck, shared_from_this(), _Info.hListen),
					5000);
			}
		}
	}

	void FtpDataSession::HandleRecv(THandle hFile, NETHANDLE hNet, const StreamBuf_ptr& Stream_sptr)
	{
		if (hFile != -1)
		{
			GetAsyncFilesInstance()->Write(hFile, Stream_sptr->data, Stream_sptr->payload);
		}

		/*if (_hRecv)
		{
			_Time.update();
			_hRecv(shared_from_this(), hNet, Stream_sptr);
		}*/
	}

	void FtpDataSession::HandleDestroy(THandle hFile, NETHANDLE hNet)
	{
		if (hFile != -1)
		{
			GetAsyncFilesInstance()->Close(hFile);
			CompleteNotify(_sPath, EN_ACTION_UPLOAD);
		}

		Close();
	}

	void FtpDataSession::HandleDataCheck(NETHANDLE hListen)
	{
		if (_Time.isElapsed(5000 * 1000))
		{//超时关闭连接
			Close();
		}
		else
		{//继续监测
			if (isVaild())
			{
				//5秒检测
				_sEventUuid = GetTimerEventKMInstance()->push_back(
					function20_bind(&FtpDataSession::HandleDataCheck, shared_from_this(), _Info.hListen),
					5000);
			}
		}
	}

	void FtpDataSession::Close(void)
	{
		if (_isOpen)
		{
			_isOpen = false;
			if (_Info.hData != -1)
			{
				GetStreamSvrInstance_V20()->Close(_Info.hData);
			}

			GetStreamSvrInstance_V20()->Stop(_Info.hListen);

			_hComplete.reset();
		}
	}

	bool FtpDataSession::isOpen(void)
	{
		return _isOpen;
	}

	int FtpDataSession::Send(const char* c_szData, UInt32 uSize)
	{
		if (_Info.hData < 1)
			return -1;

		_Time.update();
		return GetStreamSvrInstance_V20()->Send(_Info.hData, c_szData, uSize);
	}

	int FtpDataSession::Attach(const _ftp_string& sPath, const HFNComplete& hComplete, const enFTP_ACTION_TYPE Type)
	{
		if (_Info.hListen == -1)
		{
			return -1;
		}

		_Info.uType = Type;
		_hComplete = hComplete;
		_sPath = sPath;

		if (_Info.hData != -1 && _hReady)
		{
			_hReady(shared_from_this());
		}


		switch (_Info.uType)
		{
		case EN_ACTION_LIST:
			{//获取文件列表

				GetTimerEventKMInstance()->push_back(
					function20_bind(&FtpDataSession::GetFiles, shared_from_this(), _sPath),
					5);

			}
			break;
		case EN_ACTION_DOWNLOAD:
			{//下载

				THandle hRead = GetAsyncFilesInstance()->Open(_sPath.c_str(), _async_::_files_::File_impl::enMode_Read);
				if (hRead == -1)
				{
					return -1;
				}

				GetTimerEventKMInstance()->push_back(
					function20_bind(&FtpDataSession::DownFiles, shared_from_this(), hRead, _sPath),
					5);
			}
			break;
		case EN_ACTION_UPLOAD:
			{//上传
				THandle hWrite = GetAsyncFilesInstance()->Open(_sPath.c_str(), _async_::_files_::File_impl::enMode_Write);
				if (hWrite == -1)
				{
					return -1;
				}

				GetStreamSvrInstance_V20()->SetRecvHandle(_Info.hData,
					function20_bind(&FtpDataSession::HandleRecv, shared_from_this(), hWrite,
						_function_::_1, _function_::_2));

				GetStreamSvrInstance_V20()->SetDestroyHandle(_Info.hData,
					function20_bind(&FtpDataSession::HandleDestroy, shared_from_this(), hWrite, _function_::_1));
			}
			break;
		}

		return 1;
	}

	void FtpDataSession::GetFiles(const _ftp_string& sPath)
	{
		_ftp_string s = sPath;

		_ftp_string sData;
		_file_::CMyDirectory::RecursiveAllToDepth(s.c_str(),
			function20_bind(&FtpDataSession::HandleSearchFile, this, &sData,
				_function_::_1, _function_::_2), 1);

		Send(sData.c_str(), sData.size());

		CompleteNotify(_sPath, EN_ACTION_LIST);
	}

	void FtpDataSession::CompleteNotify(const _ftp_string& sPath, FtpDataSession::enFTP_ACTION_TYPE Type)
	{
		if(_hComplete)
			_hComplete(shared_from_this(), sPath, Type);
	}

	void FtpDataSession::HandleSearchFile(_ftp_string* sData, bool isDir, const char* c_szPath)
	{
		_file_::CMyFile::tagFile_INFO Info;
		Info.Ower = "FTP";
		if (_file_::CMyFile::GetDetailedInfo(c_szPath, &Info) == 1)
		{
			*sData += FtpList::GenerateString(Info).c_str();
			*sData += "\r\n";
		}
	}

	void FtpDataSession::DownFiles(THandle hFile, const _ftp_string& sPath)
	{
		GetAsyncFilesInstance()->SetCallBack(hFile,
			function20_bind_Ex(&FtpDataSession::HandleReadFile, shared_from_this()));

		if (GetAsyncFilesInstance()->Read(hFile, 1470) != 1)
		{
			GetAsyncFilesInstance()->Close(hFile);
		}
	}

	void FtpDataSession::HandleReadFile(THandle hFile, const char* c_szData, UInt32 uSize)
	{
		if (!isOpen() || uSize == 0)
		{
			GetAsyncFilesInstance()->Close(hFile);
			if (uSize == 0)
			{
				if (_hComplete)
				{//延时1秒关闭
					GetTimerEventKMInstance()->push_back(
						function20_bind(&FtpDataSession::CompleteNotify, shared_from_this(), _sPath, EN_ACTION_DOWNLOAD),
						1000);
				}

				/*if (_hComplete)
					_hComplete(shared_from_this(), _sPath, EN_ACTION_DOWNLOAD);*/
			}
		}
		else
		{
			//发送数据
			Send(c_szData, uSize);

			if (GetAsyncFilesInstance()->Read(hFile, 1470) != 1)
			{
				GetAsyncFilesInstance()->Close(hFile);
			}
		}
	}


}