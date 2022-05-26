
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
		{//ʧ��
			LOG_Print_Error(libFrameWork_FtpClients, "FtpClient_Impl::Connect(%s:%d): Failed", sFtpIP.c_str(), uFtpPort);

			//�ϱ�������
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
		{//����ʧ��
			LOG_Print_Error(libFrameWork_FtpClients, "FtpClient_Impl::HandleConnect() Failed!!");

			//�ϱ�������
			Publish(EN_ERR_CMD_HCONNECT);
		}
		else
		{//���ӳɹ�
			_NetCmd = hNet;
			GetAsynStreamClientsInstance()->SetRecvHandle(_NetCmd,
				function20_bind(&FtpClient_Impl::HandleToken, shared_from_this(),
					_function_::_1, _function_::_2, _function_::_3));

			if (GetAsynStreamClientsInstance()->SetDestroyHandle(_NetCmd,
				function20_bind(&FtpClient_Impl::HandleDestroy, shared_from_this(),
					_function_::_1)) == -1)
			{//ʧ��
				GetAsynStreamClientsInstance()->Close(_NetCmd);

				//�ϱ�������
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
		{//��ʱû��Ӧ��δ�õ���ȷ��Ӧ
			Close();

			LOG_Print_Error(libFrameWork_FtpClients, "FtpClient_Impl::HandleCheck() TimeOut!!");

			//�ϱ�������
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
				{//����
					switch (HeadInfo.CodeType)
					{
					case FTP_MEMBER_GET_ENUM(220):
						{//ftp��������ӭ��Ϣ
							GetAsynStreamClientsInstance()->SetRecvHandle(_NetCmd,
								function20_bind(&FtpClient_Impl::HandleRecv, shared_from_this(),
									_function_::_1, _function_::_2, _function_::_3));

							//���͵�¼��Ϣ
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
				{//����

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
						{//��Ҫ����
							_ftp_string s = _Ftp.Proto_PASS(_tagLogin.sPwd.c_str());
							Send(s.c_str(), s.size());
						}
						break;
					case FTP_MEMBER_GET_ENUM(230):
						{//��¼�ɹ�

							//GetFiles("/");

							//UpLoad("C:\\Users\\2020\\Downloads\\Compressed\\jiandanftpserver\\jyFTPServer\\Config.ini","/");
							//DownLoad("/Tengxun_FoxitReader_CHS_8.0.4.815.exe", "F:\\test\\dir\\Tengxun_FoxitReader_CHS_8.0.4.815.exe");
							//DownLoad("/MicrosoftOffice_V2003_XiTongZhiJia.rar", "F:\\test\\dir\\MicrosoftOffice_V2003_XiTongZhiJia.rar");
							//DownLoad("/Ӣ�İ�/vs2015.3.ent_enu.iso", "F:\\test\\dir\\vs2015.3.ent_enu.iso");

							//RemoveDir("/ftp/");
							//DeleteFile("/ftp/io/Config.ini");
							
							//�ϱ�������
							Publish(EN_LOGIN_SUCCESS);
						}
						break;
					case FTP_MEMBER_GET_ENUM(150):
						{//�ϴ�/����
							
						}
						break;
					case FTP_MEMBER_GET_ENUM(215):
						{//ϵͳ���ͻظ�
							
						}
						break;
					case FTP_MEMBER_GET_ENUM(227):
						{//����ģʽ��Ӧ
							tagHost_INFO Host;
							if (_Ftp.ParsePASV(&Stream_sptr->data[iRet], Stream_sptr->payload - iRet, &Host) == 1)
							{
								if (_Exec)
									_Exec(Host.sIP, Host.uPort);
							}
						}
						break;
					case FTP_MEMBER_GET_ENUM(257):
						{//��ѯ·����
							_ftp_string s = _Ftp.ParsePWD(&Stream_sptr->data[iRet], Stream_sptr->payload - iRet);
							if (s != "")
							{
								_CurDir = s;
							}

							Coroutine_Resume(_Coroutine);
						}
						break;
					case FTP_MEMBER_GET_ENUM(250):
						{//�л�·��
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
					case FTP_MEMBER_GET_ENUM(421):	//����ر�
					case FTP_MEMBER_GET_ENUM(425):	//�޷�����������
					case FTP_MEMBER_GET_ENUM(426):	//��������
					case FTP_MEMBER_GET_ENUM(450):	//�ļ�������
					case FTP_MEMBER_GET_ENUM(451):	//�������ش���
					case FTP_MEMBER_GET_ENUM(452):	//���̿ռ䲻��
					case FTP_MEMBER_GET_ENUM(500):	//��Ч����
					case FTP_MEMBER_GET_ENUM(501):	//�������
					case FTP_MEMBER_GET_ENUM(502):	//����û��ִ��
					case FTP_MEMBER_GET_ENUM(503):	//����ָ������
					case FTP_MEMBER_GET_ENUM(504):	//��Ч�������
					case FTP_MEMBER_GET_ENUM(530):	//δ��¼����
					case FTP_MEMBER_GET_ENUM(532):	//�洢�ļ���Ҫ�ʺ�
					case FTP_MEMBER_GET_ENUM(550):	//�ļ�������
					case FTP_MEMBER_GET_ENUM(551):	//��֪����ҳ����
					case FTP_MEMBER_GET_ENUM(552):	//�����洢����
					case FTP_MEMBER_GET_ENUM(553):	//�ļ���������
						{
							_Status = (enFtpCode)HeadInfo.CodeType;

							//�ϱ�������
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
	{//����
		Close();

		//�ϱ�������
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
				
		//�������ͣ�A = ASCII��E = EBCDIC��I = binary��
		_ftp_string s = _Ftp.Proto_TYPE("I");
		if (Send(s.c_str(), s.size()) == -1)
		{
			LOG_Print_Error(libFrameWork_FtpClients, "FtpClient_Impl::GetFiles() Send TYPE Command Failed!!");
			return -1;
		}

		Coroutine_Yield(_Coroutine);

		//����ģʽ
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
		const _ftp_string& sSaveDir/*������·��*/)
	{//�ϴ�
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

		//����ģʽ
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

	int FtpClient_Impl::UpLoad_Impl(const _ftp_string& sSrcPath, const _ftp_string& sSaveDir/*������·��*/,
		const _ftp_string& sFtpIP, const UInt16 uFtpPort)
	{
		DataConnect(sFtpIP, uFtpPort, sSrcPath, FTP_MEMBER_GET_ENUM(STOR));
		_Exec.reset();
		return 1;
	}

	int FtpClient_Impl::DownLoad(const _ftp_string& sSrcPath,
		const _ftp_string& sSavePath/*����·��*/)
	{//����
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

		//����ģʽ
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

	int FtpClient_Impl::DownLoad_Impl(const _ftp_string& sSrcPath, const _ftp_string& sSavePath/*����·��*/,
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
		{//ʧ��

			//�ϱ�������
			Publish(EN_ERR_DATA_CONNECT);
		}
	}

	void FtpClient_Impl::HandleDataConnect(const _ftp_string& sSvrIP, UInt16 uSvrPort,
		const _ftp_string& sPath, CmdCodeReq Code, NETHANDLE hNet)
	{
		if (hNet == -1)
		{//����ʧ��
		 //�ϱ�������
			Publish(EN_ERR_DATA_HCONNECT);
		}
		else
		{//���ӳɹ�
			_NetData = hNet;

			switch (Code)
			{
			case FTP_MEMBER_GET_ENUM(LIST):
			{//��ȡ�ļ��б�
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

				//�ϱ�������
				Publish(EN_ERR_GET_FILES);
			}
			break;
			case FTP_MEMBER_GET_ENUM(STOR):
			{//�ϴ�
				_ftp_string s = _Ftp.Proto_STOR(_file_::CMyFile::GetFileName(sPath.c_str()).c_str());
				Send(s.c_str(), s.size());

				THandle hRead = GetAsyncFilesInstance()->Open(sPath.c_str(), _async_::_files_::File_impl::enMode_Read);
				if (hRead != -1)
				{//�ɹ�

					if (GetAsynStreamClientsInstance()->SetDestroyHandle(_NetData,
						function20_bind(&FtpClient_Impl::HandleDataDestroy, shared_from_this(), hRead,
							_function_::_1)) == 1)
					{//�ɹ�
						GetAsyncFilesInstance()->SetCallBack(hRead,
							function20_bind(&FtpClient_Impl::HandleFileRead, shared_from_this(),
								hNet, _function_::_1, _function_::_2, _function_::_3));

						if (GetAsyncFilesInstance()->Read(hRead, 1024) == 1)
						{//�ɹ�
							return;
						}
					}
				}

				//ʧ��
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

				//�ϱ�������
				Publish(EN_ERR_UPLOAD);
			}
			break;
			case FTP_MEMBER_GET_ENUM(RETR):
			{//����
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
					{//�ɹ�

						_ftp_string s = _Ftp.Proto_RETR(_file_::CMyFile::GetFileName(sPath.c_str()).c_str());
						Send(s.c_str(), s.size());
						return;
					}
				}
				
				//ʧ��
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

				//�ϱ�������
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
			{//��ȡ�ļ��б�
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
		{//�ļ��Ѿ�����
			CloseData();
		}
		else
		{
			GetAsynStreamClientsInstance()->Send(hNet, szData, uSize);
			if (GetAsyncFilesInstance()->Read(Handle, 1024) == -1)
			{//ʧ��
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

