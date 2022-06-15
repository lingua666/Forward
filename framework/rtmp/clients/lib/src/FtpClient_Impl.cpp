
#include <libFrameWork_FtpClients/FtpClient_Impl.h>

namespace _ftp_ {

	FtpClient_Impl::FtpClient_Impl(void)
		: _NetCmd(-1)
		, _NetData(-1)
		, _Status(EN_CODE_NULL)
		, _CmdCode(FTP_MEMBER_GET_ENUM(NUL))
	{
		_Ftp.Init();
	}

	FtpClient_Impl::~FtpClient_Impl(void)
	{
		Release();
	}

	int FtpClient_Impl::Init(const HFNSubscribe& hSubscribe, const HFNFilesSubscribe& hSubFiles)
	{
		_hSubscribe = hSubscribe;
		_hSubFiles = hSubFiles;
		return 1;
	}

	void FtpClient_Impl::Release(void)
	{
		Close();
		_Ftp.Release();
		_Status = EN_CODE_NULL;
	}

	void FtpClient_Impl::Close(void)
	{
		CloseData();

		NETHANDLE	h = -1;
		if (_NetCmd != -1)
		{
			h = _NetCmd;
			_NetCmd = -1;
			GetAsynStreamClientsInstance()->Close(h);
		}

		Coroutine_Close(_Coroutine);
	}

	int FtpClient_Impl::Open(const _ftp_string& sFtpIP, const UInt16 uFtpPort,
		const _ftp_string& sUserName, const _ftp_string& sPwd)
	{
		if (_NetCmd != -1)
			return -1;

		Connect(sFtpIP, uFtpPort, sUserName, sPwd);
		return 1;
	}

	void FtpClient_Impl::Connect(const _ftp_string& sFtpIP, const UInt16 uFtpPort,
		const _ftp_string& sUserName, const _ftp_string& sPwd)
	{
		_tagLogin.sFtpIP = sFtpIP;
		_tagLogin.uFtpPort = uFtpPort;
		_tagLogin.sUserName = sUserName;
		_tagLogin.sPwd = sPwd;

		if (GetAsynStreamClientsInstance()->Connect(sFtpIP.c_str(),
			uFtpPort,
			function20_bind(&FtpClient_Impl::HandleConnect, shared_from_this(), _function_::_1)) == -1)
		{//失败
			LOG_Print_Error(libFrameWork_FtpClients, "FtpClient_Impl::Connect(%s:%d): Failed", sFtpIP.c_str(), uFtpPort);

			//上报订阅者
			Publish(EN_ERR_CMD_CONNECT);
		}
	}

	void FtpClient_Impl::Publish(enFtpCode Code)
	{
		_Status = Code;
		if (_hSubscribe)
			_hSubscribe(shared_from_this(), Code);
	}

	void FtpClient_Impl::HandleConnect(NETHANDLE hNet)
	{
		if (hNet == -1)
		{//连接失败
			LOG_Print_Error(libFrameWork_FtpClients, "FtpClient_Impl::HandleConnect() Failed!!");

			//上报订阅者
			Publish(EN_ERR_CMD_HCONNECT);
		}
		else
		{//连接成功
			_NetCmd = hNet;
			GetAsynStreamClientsInstance()->SetRecvHandle(_NetCmd,
				function20_bind(&FtpClient_Impl::HandleToken, shared_from_this(),
					_function_::_1, _function_::_2, _function_::_3));

			if (GetAsynStreamClientsInstance()->SetDestroyHandle(_NetCmd,
				function20_bind(&FtpClient_Impl::HandleDestroy, shared_from_this(),
					_function_::_1)) == -1)
			{//失败
				GetAsynStreamClientsInstance()->Close(_NetCmd);

				//上报订阅者
				Publish(EN_ERR_CMD_SESSION_INIT);
				return;
			}

			GetTimerEventInstance()->push_back(function20_bind(&FtpClient_Impl::HandleCheck,
				shared_from_this()), 5000);
		}
	}

	void FtpClient_Impl::HandleCheck(void)
	{
		if (_Status == EN_CODE_NULL)
		{//超时没响应或未得到正确响应
			Close();

			LOG_Print_Error(libFrameWork_FtpClients, "FtpClient_Impl::HandleCheck() TimeOut!!");

			//上报订阅者
			Publish(EN_ERR_CMD_SESSION_TIMEOUT);
		}
	}

	void FtpClient_Impl::HandleToken(NETHANDLE hNet, const StreamBuf_ptr& Stream_sptr, long lRemain)
	{
		//test
		LOG_Print_Info(libFrameWork_FtpClients, "FtpClient_Impl::HandleToken(%d):%s\r\n", Stream_sptr->payload, Stream_sptr->data);
		//test

		if (Stream_sptr->payload > 0)
		{
			tagHead_INFO HeadInfo = { 0 };
			int iRet = _Ftp.Parse_Head(Stream_sptr->data, Stream_sptr->payload, &HeadInfo);
			if (iRet > 0)
			{
				if (!HeadInfo.isReq)
				{//请求
					switch (HeadInfo.CodeType)
					{
					case FTP_MEMBER_GET_ENUM(220):
						{//ftp服务器欢迎信息
							GetAsynStreamClientsInstance()->SetRecvHandle(_NetCmd,
								function20_bind(&FtpClient_Impl::HandleRecv, shared_from_this(),
									_function_::_1, _function_::_2, _function_::_3));

							//发送登录信息
							_ftp_string s = _Ftp.Proto_USER(_tagLogin.sUserName.c_str());
							Send(s.c_str(), s.size());
						}
						break;
					default:
						LOG_Print_Error(libFrameWork_FtpClients,
							"FtpClient_Impl::HandleToken(%d) Unknow!!", HeadInfo.CodeType);
						break;
					}
				}
				else
				{
					LOG_Print_Error(libFrameWork_FtpClients, "FtpClient_Impl::HandleToken() Not Respond!!");
				}
			}
			else
			{
				LOG_Print_Error(libFrameWork_FtpClients, "FtpClient_Impl::HandleToken() Ftp Parse_Head Failed!!");
			}
		}
	}

	void FtpClient_Impl::HandleRecv(NETHANDLE hNet, const StreamBuf_ptr& Stream_sptr, long lRemain)
	{
		//test
		LOG_Print_Info(libFrameWork_FtpClients, "FtpClient_Impl::HandleRecv(%d):%s\r\n", Stream_sptr->payload, Stream_sptr->data);
		//test

		if (Stream_sptr->payload > 0)
		{
			tagHead_INFO HeadInfo = { 0 };
			int iRet = _Ftp.Parse_Head(Stream_sptr->data, Stream_sptr->payload, &HeadInfo);
			if (iRet > 0)
			{
				if (HeadInfo.isReq)
				{//请求

				}
				else
				{
					switch (HeadInfo.CodeType)
					{
					case FTP_MEMBER_GET_ENUM(200):
						{
							Coroutine_Resume(_Coroutine);
						}
						break;
					case FTP_MEMBER_GET_ENUM(331):
						{//需要密码
							_ftp_string s = _Ftp.Proto_PASS(_tagLogin.sPwd.c_str());
							Send(s.c_str(), s.size());
						}
						break;
					case FTP_MEMBER_GET_ENUM(230):
						{//登录成功

							//GetFiles("/");

							//UpLoad("C:\\Users\\2020\\Downloads\\Compressed\\jiandanftpserver\\jyFTPServer\\Config.ini","/");
							//DownLoad("/Tengxun_FoxitReader_CHS_8.0.4.815.exe", "F:\\test\\dir\\Tengxun_FoxitReader_CHS_8.0.4.815.exe");
							//DownLoad("/MicrosoftOffice_V2003_XiTongZhiJia.rar", "F:\\test\\dir\\MicrosoftOffice_V2003_XiTongZhiJia.rar");
							//DownLoad("/英文版/vs2015.3.ent_enu.iso", "F:\\test\\dir\\vs2015.3.ent_enu.iso");

							//RemoveDir("/ftp/");
							//DeleteFile("/ftp/io/Config.ini");
							
							//上报订阅者
							Publish(EN_LOGIN_SUCCESS);
						}
						break;
					case FTP_MEMBER_GET_ENUM(150):
						{//上传/下载
							
						}
						break;
					case FTP_MEMBER_GET_ENUM(215):
						{//系统类型回复
							
						}
						break;
					case FTP_MEMBER_GET_ENUM(227):
						{//被动模式响应
							tagHost_INFO Host;
							if (_Ftp.ParsePASV(&Stream_sptr->data[iRet], Stream_sptr->payload - iRet, &Host) == 1)
							{
								if (_Exec)
									_Exec(Host.sIP, Host.uPort);
							}
						}
						break;
					case FTP_MEMBER_GET_ENUM(257):
						{//查询路径名
							_ftp_string s = _Ftp.ParsePWD(&Stream_sptr->data[iRet], Stream_sptr->payload - iRet);
							if (s != "")
							{
								_CurDir = s;
							}

							Coroutine_Resume(_Coroutine);
						}
						break;
					case FTP_MEMBER_GET_ENUM(250):
						{//切换路径
							switch (_CmdCode)
							{
							case FTP_MEMBER_GET_ENUM(CWD):
								{
									_ftp_string s = _Ftp.ParseAckCWD(&Stream_sptr->data[iRet], Stream_sptr->payload - iRet);
									if (s != "")
									{
										_CurDir = s;
									}

									Coroutine_Resume(_Coroutine);
								}
								break;
							}
						}
						break;
					case FTP_MEMBER_GET_ENUM(226):
						{
							CloseData();
						}
						break;
					case FTP_MEMBER_GET_ENUM(421):	//服务关闭
					case FTP_MEMBER_GET_ENUM(425):	//无法打开数据连接
					case FTP_MEMBER_GET_ENUM(426):	//结束连接
					case FTP_MEMBER_GET_ENUM(450):	//文件不可用
					case FTP_MEMBER_GET_ENUM(451):	//遇到本地错误
					case FTP_MEMBER_GET_ENUM(452):	//磁盘空间不足
					case FTP_MEMBER_GET_ENUM(500):	//无效命令
					case FTP_MEMBER_GET_ENUM(501):	//错误参数
					case FTP_MEMBER_GET_ENUM(502):	//命令没有执行
					case FTP_MEMBER_GET_ENUM(503):	//错误指令序列
					case FTP_MEMBER_GET_ENUM(504):	//无效命令参数
					case FTP_MEMBER_GET_ENUM(530):	//未登录网络
					case FTP_MEMBER_GET_ENUM(532):	//存储文件需要帐号
					case FTP_MEMBER_GET_ENUM(550):	//文件不可用
					case FTP_MEMBER_GET_ENUM(551):	//不知道的页类型
					case FTP_MEMBER_GET_ENUM(552):	//超过存储分配
					case FTP_MEMBER_GET_ENUM(553):	//文件名不允许
						{
							_Status = (enFtpCode)HeadInfo.CodeType;

							//上报订阅者
							Publish((enFtpCode)HeadInfo.CodeType);
							_Exec.reset();
						}
						break;
					}
				}
			}
		}
	}

	void FtpClient_Impl::HandleDestroy(NETHANDLE hNet)
	{//销毁
		Close();

		//上报订阅者
		Publish(EN_CMD_SESSION_DESTROY);
	}

	int FtpClient_Impl::Send(const char* c_szData, UInt32 uSize)
	{
		if (_NetCmd == -1)
			return -1;

		return GetAsynStreamClientsInstance()->Send(_NetCmd, c_szData, uSize);
	}

	int FtpClient_Impl::RemoveDir(const _ftp_string& sSrcDir)
	{
		Coroutine_Begin(_Coroutine);

		Coroutine_Open(_Coroutine,
			function20_bind(&FtpClient_Impl::RemoveDir, shared_from_this(),
				sSrcDir));

		_ftp_string s = _Ftp.Proto_CWD("/");
		if (Send(s.c_str(), s.size()) == -1)
		{
			LOG_Print_Error(libFrameWork_FtpClients, "FtpClient_Impl::RemoveDir() Send RMD Command Failed!!");
			return -1;
		}

		_CmdCode = FTP_MEMBER_GET_ENUM(CWD);

		Coroutine_Yield(_Coroutine);

		_ftp_string s = _Ftp.Proto_PWD();
		if (Send(s.c_str(), s.size()) == -1)
		{
			LOG_Print_Error(libFrameWork_FtpClients, "FtpClient_Impl::RemoveDir() Send RMD Command Failed!!");
			return -1;
		}

		Coroutine_Yield(_Coroutine);

		_ftp_string s = _Ftp.Proto_RMD(sSrcDir.c_str());
		if (Send(s.c_str(), s.size()) == -1)
		{
			LOG_Print_Error(libFrameWork_FtpClients, "FtpClient_Impl::RemoveDir() Send RMD Command Failed!!");
			return -1;
		}

		Coroutine_Close(_Coroutine);

		Coroutine_End(_Coroutine);

		return 1;

	}

	int FtpClient_Impl::DeleteFile(const _ftp_string& sSrcPath)
	{
		_ftp_string s = _Ftp.Proto_DELE(sSrcPath.c_str());
		if (Send(s.c_str(), s.size()) == -1)
		{
			LOG_Print_Error(libFrameWork_FtpClients, "FtpClient_Impl::DeleteFile() Send DELE Command Failed!!");
			return -1;
		}

		return 1;
	}

	int FtpClient_Impl::GetFiles(const _ftp_string& sDir)
	{
		if (_Exec)
			return -1;

		Coroutine_Begin(_Coroutine);

		Coroutine_Open(_Coroutine,
			function20_bind(&FtpClient_Impl::GetFiles, shared_from_this(),
				sDir));

		_ftp_string s = _Ftp.Proto_CWD(sDir.c_str());
		if (Send(s.c_str(), s.size()) == -1)
		{
			LOG_Print_Error(libFrameWork_FtpClients, "FtpClient_Impl::GetFiles() Send CWD Command Failed!!");
			return -1;
		}

		_CmdCode = FTP_MEMBER_GET_ENUM(CWD);

		Coroutine_Yield(_Coroutine);
				
		//数据类型（A = ASCII，E = EBCDIC，I = binary）
		_ftp_string s = _Ftp.Proto_TYPE("I");
		if (Send(s.c_str(), s.size()) == -1)
		{
			LOG_Print_Error(libFrameWork_FtpClients, "FtpClient_Impl::GetFiles() Send TYPE Command Failed!!");
			return -1;
		}

		Coroutine_Yield(_Coroutine);

		//被动模式
		_ftp_string s = _Ftp.Proto_PASV();
		if (Send(s.c_str(), s.size()) == -1)
		{
			LOG_Print_Error(libFrameWork_FtpClients, "FtpClient_Impl::GetFiles() Send PASV Command Failed!!");
			return -1;
		}

		_Exec = function20_bind(&FtpClient_Impl::GetFiles_Impl, shared_from_this(),
			sDir, _function_::_1, _function_::_2);

		Coroutine_Close(_Coroutine);

		Coroutine_End();

		return 1;
	}

	int FtpClient_Impl::GetFiles_Impl(const _ftp_string& sDir,
		const _ftp_string& sFtpIP, const UInt16 uFtpPort)
	{
		DataConnect(sFtpIP, uFtpPort, sDir, FTP_MEMBER_GET_ENUM(LIST));
		_Exec.reset();
		return 1;
	}

	int FtpClient_Impl::UpLoad(const _ftp_string& sSrcPath,
		const _ftp_string& sSaveDir/*服务器路径*/)
	{//上传
		if (_Exec)
			return -1;

		Coroutine_Begin(_Coroutine);

		Coroutine_Open(_Coroutine,
				function20_bind(&FtpClient_Impl::UpLoad, shared_from_this(),
					sSrcPath, sSaveDir));

		_ftp_string s = _Ftp.Proto_CWD(sSaveDir.c_str());
		if (Send(s.c_str(), s.size()) == -1)
		{
			LOG_Print_Error(libFrameWork_FtpClients, "FtpClient_Impl::UpLoad() Send CWD Command Failed!!");
			return -1;
		}

		_CmdCode = FTP_MEMBER_GET_ENUM(CWD);

		Coroutine_Yield(_Coroutine);

		//被动模式
		_ftp_string s = _Ftp.Proto_PASV();
		if (Send(s.c_str(), s.size()) == -1)
		{
			LOG_Print_Error(libFrameWork_FtpClients, "FtpClient_Impl::UpLoad() Send PASV Command Failed!!");
			return -1;
		}

		_Exec = function20_bind(&FtpClient_Impl::UpLoad_Impl, shared_from_this(),
			sSrcPath, sSaveDir, _function_::_1, _function_::_2);

		Coroutine_Close(_Coroutine);
	
		Coroutine_End(_Coroutine);

		return 1;
	}

	int FtpClient_Impl::UpLoad_Impl(const _ftp_string& sSrcPath, const _ftp_string& sSaveDir/*服务器路径*/,
		const _ftp_string& sFtpIP, const UInt16 uFtpPort)
	{
		DataConnect(sFtpIP, uFtpPort, sSrcPath, FTP_MEMBER_GET_ENUM(STOR));
		_Exec.reset();
		return 1;
	}

	int FtpClient_Impl::DownLoad(const _ftp_string& sSrcPath,
		const _ftp_string& sSavePath/*本地路径*/)
	{//下载
		if (_Exec)
			return -1;

		Coroutine_Begin(_Coroutine);

		Coroutine_Open(_Coroutine,
			function20_bind(&FtpClient_Impl::DownLoad, shared_from_this(),
				sSrcPath, sSavePath));

		_ftp_string s = _Ftp.Proto_CWD(sSrcPath.substr(0, sSrcPath.rfind('/') + 1).c_str());
		if (Send(s.c_str(), s.size()) == -1)
		{
			LOG_Print_Error(libFrameWork_FtpClients, "FtpClient_Impl::DownLoad() Send CWD Command Failed!!");
			return -1;
		}

		_CmdCode = FTP_MEMBER_GET_ENUM(CWD);

		Coroutine_Yield(_Coroutine);

		//被动模式
		_ftp_string s = _Ftp.Proto_PASV();

		if (Send(s.c_str(), s.size()) == -1)
		{
			LOG_Print_Error(libFrameWork_FtpClients, "FtpClient_Impl::DownLoad() Send PASV Command Failed!!");
			return -1;
		}

		_Exec = function20_bind(&FtpClient_Impl::DownLoad_Impl, shared_from_this(),
			sSrcPath, sSavePath, _function_::_1, _function_::_2);

		Coroutine_Close(_Coroutine);

		Coroutine_End(_Coroutine);

		return 1;
	}

	int FtpClient_Impl::DownLoad_Impl(const _ftp_string& sSrcPath, const _ftp_string& sSavePath/*本地路径*/,
		const _ftp_string& sFtpIP, const UInt16 uFtpPort)
	{
		DataConnect(sFtpIP, uFtpPort, sSavePath, FTP_MEMBER_GET_ENUM(RETR));
		_Exec.reset();
		return 1;
	}

	void FtpClient_Impl::DataConnect(const _ftp_string& sSvrIP, const UInt16 uSvrPort,
		const _ftp_string& sPath, CmdCodeReq Code)
	{
		if (GetAsynStreamClientsInstance()->Connect(sSvrIP.c_str(), uSvrPort,
			function20_bind(&FtpClient_Impl::HandleDataConnect, shared_from_this(),
				sSvrIP, uSvrPort, sPath, Code, _function_::_1)) == -1)
		{//失败

			//上报订阅者
			Publish(EN_ERR_DATA_CONNECT);
		}
	}

	void FtpClient_Impl::HandleDataConnect(const _ftp_string& sSvrIP, UInt16 uSvrPort,
		const _ftp_string& sPath, CmdCodeReq Code, NETHANDLE hNet)
	{
		if (hNet == -1)
		{//连接失败
		 //上报订阅者
			Publish(EN_ERR_DATA_HCONNECT);
		}
		else
		{//连接成功
			_NetData = hNet;

			switch (Code)
			{
			case FTP_MEMBER_GET_ENUM(LIST):
			{//获取文件列表
				GetAsynStreamClientsInstance()->SetRecvHandle(_NetData,
					function20_bind(&FtpClient_Impl::HandleDataRecv, shared_from_this(),
						Code, _function_::_1, _function_::_2, _function_::_3));

				if (GetAsynStreamClientsInstance()->SetDestroyHandle(_NetData,
					function20_bind(&FtpClient_Impl::HandleDataDestroy, shared_from_this(), -1,
						_function_::_1)) == 1)
				{
					_ftp_string s = _Ftp.Proto_LIST();
					Send(s.c_str(), s.size());
					return;
				}

				LOG_Print_Error(libFrameWork_FtpClients, "FtpClient_Impl::HandleDataConnect(STOR): SetDestroyHandle() Failed");
				GetAsynStreamClientsInstance()->Close(_NetData);

				//上报订阅者
				Publish(EN_ERR_GET_FILES);
			}
			break;
			case FTP_MEMBER_GET_ENUM(STOR):
			{//上传
				_ftp_string s = _Ftp.Proto_STOR(_file_::CMyFile::GetFileName(sPath.c_str()).c_str());
				Send(s.c_str(), s.size());

				THandle hRead = GetAsyncFilesInstance()->Open(sPath.c_str(), _async_::_files_::File_impl::enMode_Read);
				if (hRead != -1)
				{//成功

					if (GetAsynStreamClientsInstance()->SetDestroyHandle(_NetData,
						function20_bind(&FtpClient_Impl::HandleDataDestroy, shared_from_this(), hRead,
							_function_::_1)) == 1)
					{//成功
						GetAsyncFilesInstance()->SetCallBack(hRead,
							function20_bind(&FtpClient_Impl::HandleFileRead, shared_from_this(),
								hNet, _function_::_1, _function_::_2, _function_::_3));

						if (GetAsyncFilesInstance()->Read(hRead, 1024) == 1)
						{//成功
							return;
						}
					}
				}

				//失败
				if (hRead != -1)
				{
					GetAsyncFilesInstance()->Close(hRead);
					LOG_Print_Error(libFrameWork_FtpClients,
						"FtpClient_Impl::HandleDataConnect(STOR): SetDestroyHandle() Failed");
				}
				else
				{
					LOG_Print_Error(libFrameWork_FtpClients,
						"FtpClient_Impl::HandleDataConnect(STOR): Open File:%s Failed", sPath.c_str());
				}

				GetAsynStreamClientsInstance()->Close(_NetData);

				//上报订阅者
				Publish(EN_ERR_UPLOAD);
			}
			break;
			case FTP_MEMBER_GET_ENUM(RETR):
			{//下载
				THandle hWrite = GetAsyncFilesInstance()->Open(sPath.c_str(), _async_::_files_::File_impl::enMode_Write);
				if (hWrite != -1)
				{
					GetAsyncFilesInstance()->SetCallBack(hWrite,
						function20_bind(&FtpClient_Impl::HandleFileRead, shared_from_this(),
							hNet, _function_::_1, _function_::_2, _function_::_3));

					GetAsynStreamClientsInstance()->SetRecvHandle(_NetData,
						function20_bind(&FtpClient_Impl::HandleDownLoad, shared_from_this(),
							hWrite, _function_::_1, _function_::_2, _function_::_3));

					if (GetAsynStreamClientsInstance()->SetDestroyHandle(_NetData,
						function20_bind(&FtpClient_Impl::HandleDataDestroy, shared_from_this(),
							hWrite, _function_::_1)) == 1)
					{//成功

						_ftp_string s = _Ftp.Proto_RETR(_file_::CMyFile::GetFileName(sPath.c_str()).c_str());
						Send(s.c_str(), s.size());
						return;
					}
				}
				
				//失败
				if (hWrite != -1)
				{
					GetAsyncFilesInstance()->Close(hWrite);
					LOG_Print_Error(libFrameWork_FtpClients,
						"FtpClient_Impl::HandleDataConnect(RETR): SetDestroyHandle() Failed");
				}
				else
				{
					LOG_Print_Error(libFrameWork_FtpClients,
						"FtpClient_Impl::HandleDataConnect(RETR): Open File:%s Failed", sPath.c_str());
				}

				GetAsynStreamClientsInstance()->Close(_NetData);

				//上报订阅者
				Publish(EN_ERR_DOWNLOAD);
			}
			break;
			}
		}
	}

	void FtpClient_Impl::HandleDataRecv(CmdCodeReq Code, NETHANDLE hNet,
		const StreamBuf_ptr& Stream_sptr, long lRemain)
	{
		//test
		LOG_Print_Info(libFrameWork_FtpClients, "FtpClient_Impl::HandleDataRecv(%d):\r\n%s\r\n", Stream_sptr->payload, Stream_sptr->data);
		//test

		switch (Code)
		{
		case FTP_MEMBER_GET_ENUM(LIST):
			{//获取文件列表
				if (_hSubFiles)
				{
					FtpList List;
					FtpList::FileList_tpye Files;
					List.Parse(Stream_sptr->data, Stream_sptr->payload, &Files);
					_hSubFiles(shared_from_this(), Files);
					Files.clear();
				}
			}
			break;
		}
	}
		
	void FtpClient_Impl::HandleDownLoad(THandle hWrite, NETHANDLE hNet,
		const StreamBuf_ptr& Stream_sptr, long lRemain)
	{
		if (hWrite != -1)
		{
			GetAsyncFilesInstance()->Write(hWrite, Stream_sptr->data, Stream_sptr->payload);
		}
	}

	void FtpClient_Impl::HandleDataDestroy(THandle hFile, NETHANDLE hNet)
	{
		if (hFile != -1)
		{
			GetTimerEventInstance()->push_back(
				function20_bind(&_async_::_files_::AsyncFiles::Close, GetAsyncFilesInstance(), hFile),
				1000);
		}
	}

	int FtpClient_Impl::HandleFileRead(NETHANDLE hNet, THandle Handle, const char* szData, UInt32 uSize)
	{
		if (uSize == 0)
		{//文件已经读完
			CloseData();
		}
		else
		{
			GetAsynStreamClientsInstance()->Send(hNet, szData, uSize);
			if (GetAsyncFilesInstance()->Read(Handle, 1024) == -1)
			{//失败
				CloseData();
			}
		}

		return 1;
	}

	void FtpClient_Impl::CloseData(void)
	{
		NETHANDLE	h = -1;
		if (_NetData != -1)
		{
			h = _NetData;
			_NetData = -1;
			GetAsynStreamClientsInstance()->Close(h);
		}
	}

	enFtpCode	FtpClient_Impl::GetStatus(void)
	{
		return _Status;
	}

	THandle	FtpClient_Impl::Get(void)
	{
		return _NetCmd;
	}

}

