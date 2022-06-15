
#include <libFrameWork_FtpSvr/FtpCmdSession.h>

namespace _ftp_ {

	FtpCmdSession::self_sptr FtpCmdSession::Alloc(void)
	{
		static MemPool_type s_Pool(sizeof(self_type));
		return self_sptr(s_Pool.AllocObj<self_type>(),
			function20_bind_Ex(&MemPool_type::FreeObj<self_type>, &s_Pool));
	}

	FtpCmdSession::FtpCmdSession(const self_type& t1)
	{
		this->_SessionInfo.sUser = t1._SessionInfo.sUser;
		this->_SessionInfo.sPassword = t1._SessionInfo.sPassword;
		this->_SessionInfo._sRootDir = t1._SessionInfo._sRootDir;
		this->_SessionInfo._sCurDir = t1._SessionInfo._sCurDir;
		this->_SessionInfo._isTemporary = t1._SessionInfo._isTemporary;
		this->_SessionInfo.hNet = t1._SessionInfo.hNet;
		this->_sLocalUrl = t1._sLocalUrl;
		this->_isLogin = t1._isLogin;
	}

	FtpCmdSession::FtpCmdSession(void)
		: _isLogin(false)
	{
		_SessionInfo.hNet = -1;
		_SessionInfo._isTemporary = true;
	}

	FtpCmdSession::self_type& FtpCmdSession::operator=(const self_type& t1)
	{
		this->_SessionInfo.sUser = t1._SessionInfo.sUser;
		this->_SessionInfo.sPassword = t1._SessionInfo.sPassword;
		this->_SessionInfo._sRootDir = t1._SessionInfo._sRootDir;
		this->_SessionInfo._sCurDir = t1._SessionInfo._sCurDir;
		this->_SessionInfo._isTemporary = t1._SessionInfo._isTemporary;
		this->_SessionInfo.hNet = t1._SessionInfo.hNet;
		this->_sLocalUrl = t1._sLocalUrl;
		this->_isLogin = t1._isLogin;
		return *this;
	}

	FtpCmdSession::~FtpCmdSession(void)
	{
		Release();
	}

	int FtpCmdSession::Init(const _ftp_string& sLocalUrl, const _ftp_string& sUser,
		const _ftp_string& sPWD, bool isTemporary)
	{
		_sLocalUrl = sLocalUrl;
		_SessionInfo.sUser = sUser;
		_SessionInfo.sPassword = sPWD;
		_SessionInfo._isTemporary = isTemporary;

		_Ftp.Init();
		return 1;
	}

	void FtpCmdSession::Release(void)
	{
		Close();
	}

	bool FtpCmdSession::Authentication(const _ftp_string& sUser, const _ftp_string& sPWD)
	{
		if (_SessionInfo.sUser == sUser && _SessionInfo.sPassword == sPWD)
			return true;

		return false;
	}

	int FtpCmdSession::Open(NETHANDLE hNet, const _ftp_string& sRootDir)
	{
		if (_SessionInfo.hNet != -1)
			return 1;

		_SessionInfo._sRootDir = sRootDir;
		_SessionInfo._sCurDir = "/";
		_SessionInfo.hNet = hNet;
		return GetStreamSvrInstance()->SetRecvHandle(hNet,
			function20_bind_Ex(&FtpCmdSession::HandleRecv, shared_from_this()));
	}

	void FtpCmdSession::Close(void)
	{
		if (_SessionInfo.hNet != -1)
		{
			if (_Data_sptr)
				_Data_sptr->Close();

			NETHANDLE h = _SessionInfo.hNet;
			_SessionInfo.hNet = -1;
			GetStreamSvrInstance()->Close(h);

			_isLogin = false;
		}
	}

	int FtpCmdSession::Send(const char* c_szData, UInt32 uSize)
	{
		if (_SessionInfo.hNet != -1)
			return GetStreamSvrInstance()->Send(_SessionInfo.hNet, c_szData, uSize);

		return -1;
	}

	_ftp_string FtpCmdSession::GetUser(void)
	{
		return _SessionInfo.sUser;
	}

	NETHANDLE	FtpCmdSession::GetNet(void)
	{
		return _SessionInfo.hNet;
	}

	bool FtpCmdSession::IsLogin(void) const
	{
		return _isLogin;
	}

	bool FtpCmdSession::IsTemporary(void) const
	{
		return _SessionInfo._isTemporary;
	}

	void FtpCmdSession::HandleRecv(NETHANDLE hNet, const StreamBuf_ptr& Stream_sptr)
	{
		if (Stream_sptr->payload > 0)
		{
			//test
			LOG_Print_Info(libFrameWork_FtpServer, "FtpCmdSession::HandleRecv(%d) Data:%s",
				Stream_sptr->payload, Stream_sptr->data);
			//test

			tagHead_INFO HeadInfo = { 0 };
			int iRet = _Ftp.Parse_Head(Stream_sptr->data, Stream_sptr->payload, &HeadInfo);
			if (iRet > 0)
			{
				if (HeadInfo.isReq)
				{//请求
					switch (HeadInfo.CodeType)
					{
					case FTP_MEMBER_GET_ENUM(USER):
					{
						if (_isLogin)
						{//已经登录
							_ftp_string s = _Ftp.Proto_230("Already logged-in.");
							Send(s.c_str(), s.size());
						}
					}
					break;
					case FTP_MEMBER_GET_ENUM(PASS):
					{
						_ftp_string sRet = _Ftp.ParsePASS(&Stream_sptr->data[iRet], Stream_sptr->payload - iRet);
						if (sRet != _SessionInfo.sPassword)
						{//密码不正确
							_ftp_string s = _Ftp.Proto_530("Authentication failed.");
							Send(s.c_str(), s.size());

							//断开连接
							GetTimerEventKMInstance()->push_back(
								function20_bind(&FtpCmdSession::Close, shared_from_this()), 1000
							);
							return;
						}

						if (_isLogin)
						{//已经登录
							_ftp_string s = _Ftp.Proto_202("Already logged-in.");
							Send(s.c_str(), s.size());
							return ;
						}

						sRet = "User ";
						sRet += _SessionInfo.sUser + " logged in.";
						_ftp_string s = _Ftp.Proto_230(sRet.c_str());
						Send(s.c_str(), s.size());
						_isLogin = true;
					}
					break;
					case FTP_MEMBER_GET_ENUM(SYST):
					{
						_ftp_string s = _Ftp.Proto_215("UNIX Type : L8 Internet Component Suite.");
						Send(s.c_str(), s.size());
					}
					break;
					case FTP_MEMBER_GET_ENUM(PWD):
					{
						_ftp_string sDir = "\"";
						sDir += _SessionInfo._sCurDir.gbk_utf8() + "\" is current directory.";
						_ftp_string s = _Ftp.Proto_257(sDir.c_str());
						Send(s.c_str(), s.size());
					}
					break;
					case FTP_MEMBER_GET_ENUM(CWD):
					{
						_ftp_string sRet = _Ftp.ParseCWD(&Stream_sptr->data[iRet], Stream_sptr->payload - iRet).utf8_gbk();
						_ftp_string sDir = _SessionInfo._sRootDir + _file_::CMyDirectory::GetSplit();
						_ftp_string sCur = sRet;
						if (sRet[0] == '/')
						{//完整路径
							/*if (sRet.size() > 1)
							{
								sDir = NormcasePath(_SessionInfo._sRootDir + sRet + "/");
								sCur = sRet + "/";
							}*/

							_ftp_string sSplit;
							if (sRet[sRet.size() - 1] != '/')
								sSplit = '/';

							sDir = NormcasePath(_SessionInfo._sRootDir + sRet + sSplit);
							sCur = sRet + sSplit;
						}
						else
						{//目录名
							sDir = NormcasePath(_SessionInfo._sCurDir + sRet +"/");
							sCur = _SessionInfo._sCurDir + sRet + "/";
						}

						_ftp_string s;
						_ftp_string sReMark;
						if (_file_::CMyDirectory::IsExit(sDir.c_str()))
						{
							_SessionInfo._sCurDir = sCur;
							sReMark = "CWD command successful. \"";
							sReMark += sCur + "\" is current directory.";
							s = _Ftp.Proto_250(sReMark.c_str());

							//test
							printf("sReMark:%s\r\n", sReMark.c_str());
							//test
						}
						else
						{
							sReMark = "CWD \"";
							sReMark += sCur + "\" No such directory.";
							s = _Ftp.Proto_550(sReMark.c_str());
						}

						Send(s.c_str(), s.size());
					}
					break;
					case FTP_MEMBER_GET_ENUM(TYPE):
					{
						_ftp_string sRet = _Ftp.ParseTYPE(&Stream_sptr->data[iRet], Stream_sptr->payload - iRet);
						_ftp_string sReMark = "Type set to ";
						sReMark += sRet + ".";
						_ftp_string s = _Ftp.Proto_200(sReMark.c_str());
						Send(s.c_str(), s.size());
					}
					break;
					case FTP_MEMBER_GET_ENUM(PASV):
					{
						_Data_sptr = FtpDataSession::Alloc();
						_Data_sptr->Init();

						if (!_Data_sptr->isVaild() && _sLocalUrl != "")
						{
							tagHost_INFO tagHost;
							tagHost.sIP = _sLocalUrl;
							tagHost.uPort = _Data_sptr->Open();

							if (tagHost.uPort > 0)
							{
								_ftp_string s = _Ftp.Proto_227(&tagHost);
								Send(s.c_str(), s.size());
								return;
							}
						}

						//失败
						_ftp_string s = _Ftp.Proto_425("Cannot open passive connection.");
						Send(s.c_str(), s.size());
					}
					break;
					case FTP_MEMBER_GET_ENUM(MKD):
					{
						_ftp_string sRet = _Ftp.ParseMKD(&Stream_sptr->data[iRet], Stream_sptr->payload - iRet);
						_ftp_string sDir;
						_ftp_string sCur;
						if (sRet[0] == '/')
						{//完整路径
							sDir = NormcasePath(_SessionInfo._sRootDir + sRet);
							sCur = sRet;
						}
						else
						{//目录名
							sDir = NormcasePath(_SessionInfo._sCurDir + "/" + sRet);
							sCur = _SessionInfo._sCurDir + "/" + sRet;
						}

						_ftp_string s;
						_ftp_string sReMark;
						if (_file_::CMyDirectory::IsExit(sDir.c_str()))
						{
							sReMark = "MKD command \"";
							sReMark += sCur + "\" Already exists.";
							s = _Ftp.Proto_550(sReMark.c_str());
						}
						else
						{
							//创建目录
							if (_file_::CMyDirectory::Create(sDir.c_str()))
							{//成功
								sReMark = "\"";
								sReMark += sCur + "\": directory created.";
								s = _Ftp.Proto_257(sReMark.c_str());
							}
							else
							{//失败
								sReMark = "MKD command \"";
								sReMark += sCur + "\": Cannot create directory.";
								s = _Ftp.Proto_550(sReMark.c_str());
							}
						}

						Send(s.c_str(), s.size());
					}
					break;
					case FTP_MEMBER_GET_ENUM(RMD):
					{
						_ftp_string sRet = _Ftp.ParseRMD(&Stream_sptr->data[iRet], Stream_sptr->payload - iRet);
						_ftp_string sDir;
						_ftp_string sCur;
						if (sRet[0] == '/')
						{//完整路径
							sDir = NormcasePath(_SessionInfo._sRootDir + sRet);
							sCur = sRet;
						}
						else
						{//目录名
							sDir = NormcasePath(_SessionInfo._sCurDir + "/" + sRet);
							sCur = _SessionInfo._sCurDir + "/" + sRet;
						}

						_ftp_string s;

						if (_file_::CMyDirectory::IsExit(sDir.c_str()))
						{
							_file_::CMyDirectory::RemoveDir(sDir.c_str());

							_ftp_string sReMark = "\"";
							sReMark += sCur + "\": Directory removed.";

							s = _Ftp.Proto_250(sReMark.c_str());
						}
						else
						{
							_ftp_string sReMark = "RMD command \"";
							sReMark += sCur + "\": Not a valid directory.";
							s = _Ftp.Proto_550(sReMark.c_str());
						}

						Send(s.c_str(), s.size());
					}
					break;
					case FTP_MEMBER_GET_ENUM(DELE):
					{
						_ftp_string sRet = _Ftp.ParseRMD(&Stream_sptr->data[iRet], Stream_sptr->payload - iRet);
						_ftp_string sPath;
						_ftp_string sCur;
						if (sRet[0] == '/')
						{//完整路径
							sPath = NormcasePath(_SessionInfo._sRootDir + sRet);
							sCur = sRet;
						}
						else
						{//目录名
							sPath = NormcasePath(_SessionInfo._sCurDir + "/" + sRet);
							sCur = _SessionInfo._sCurDir;
						}

						_ftp_string s;
						if (_file_::CMyFile::IsExit(sPath.c_str()))
						{
							if (_file_::CMyFile::DeleteFile(sPath.c_str()) == 1)
							{
								_ftp_string sReMark = "File \"";
								sReMark += sCur + "\" deleted.";
								s = _Ftp.Proto_250(sReMark.c_str());
							}
							else
							{//失败
								_ftp_string sReMark = "File \"";
								sReMark += sCur + "\" Can't delete file.";
								s = _Ftp.Proto_450(sReMark.c_str());
							}
						}
						else
						{
							_ftp_string sReMark = "File \"";
							sReMark += sCur + "\" Not a valid file";
							s = _Ftp.Proto_550(sReMark.c_str());
						}

						Send(s.c_str(), s.size());
					}
					break;
					case FTP_MEMBER_GET_ENUM(LIST):
					{
						if (!_Data_sptr)
						{
							_ftp_string s = _Ftp.Proto_425("Cannot open data connection.");
							Send(s.c_str(), s.size());
							return;
						}

						if (_Data_sptr->Attach(NormcasePath(_SessionInfo._sCurDir),
							function20_bind_Ex(&FtpCmdSession::HandleDataComplete, shared_from_this()),
							FtpDataSession::EN_ACTION_LIST) == -1)
						{
							_ftp_string s = _Ftp.Proto_425("Cannot open data connection.");
							Send(s.c_str(), s.size());
						}
						else
						{
							_ftp_string s = _Ftp.Proto_150("Opening data connection for directory list.");
							Send(s.c_str(), s.size());
						}
					}
					break;
					case FTP_MEMBER_GET_ENUM(RETR):
					{//下载
						if (!_Data_sptr)
						{
							_ftp_string s = _Ftp.Proto_425("Cannot open data connection.");
							Send(s.c_str(), s.size());
							return;
						}

						_ftp_string sFilePath = _SessionInfo._sCurDir + _Ftp.ParseRETR(Stream_sptr->data, Stream_sptr->payload).utf8_gbk();

						if (_Data_sptr->Attach(NormcasePath(sFilePath),
							function20_bind_Ex(&FtpCmdSession::HandleDataComplete, shared_from_this()),
							FtpDataSession::EN_ACTION_DOWNLOAD) == -1)
						{
							_ftp_string s = _Ftp.Proto_425("Cannot open data connection.");
							Send(s.c_str(), s.size());
						}
						else
						{
							_ftp_string s = _Ftp.Proto_150("Opening data connection for directory list.");
							Send(s.c_str(), s.size());
						}
					}
					break;
					case FTP_MEMBER_GET_ENUM(STOR):
					{//上传
						if (!_Data_sptr)
						{
							_ftp_string s = _Ftp.Proto_425("Cannot open data connection.");
							Send(s.c_str(), s.size());
							return;
						}

						_ftp_string sFilePath = _SessionInfo._sCurDir + _Ftp.ParseSTOR(Stream_sptr->data, Stream_sptr->payload).utf8_gbk();

						if (_Data_sptr->Attach(NormcasePath(sFilePath),
							function20_bind_Ex(&FtpCmdSession::HandleDataComplete, shared_from_this()),
							FtpDataSession::EN_ACTION_UPLOAD) == -1)
						{
							_ftp_string s = _Ftp.Proto_425("Cannot open data connection.");
							Send(s.c_str(), s.size());
						}
						else
						{
							_ftp_string s = _Ftp.Proto_150("Opening data connection for directory list.");
							Send(s.c_str(), s.size());
						}
					}
					break;
					case FTP_MEMBER_GET_ENUM(CDUP):
					{
						if (_SessionInfo._sCurDir != "/")
						{
							_ftp_string sPath = _SessionInfo._sCurDir.substr(0, _SessionInfo._sCurDir.rfind('/', _SessionInfo._sCurDir.size() - 2) + 1);
							_SessionInfo._sCurDir = sPath;
						}
						
						_ftp_string sReMark = "CWD command successful. ";
						sReMark += _SessionInfo._sCurDir + " is current directory.\r\n";

						_ftp_string s = _Ftp.Proto_250(sReMark.c_str());
						Send(s.c_str(), s.size());
					}
					break;
					/*case FTP_MEMBER_GET_ENUM(PASS):
					{

					}
					break;*/
					}
				}
			}
		}
	}

	void FtpCmdSession::HandleDataComplete(const FtpDataSession_sptr& sptr,
		const _ftp_string& sPath, FtpDataSession::enFTP_ACTION_TYPE Type)
	{
		switch (Type)
		{
		case FtpDataSession::EN_ACTION_LIST:
		case FtpDataSession::EN_ACTION_DOWNLOAD:
			{//下载
				_ftp_string s = _Ftp.Proto_226("File sent ok.");
				Send(s.c_str(), s.size());
			}
			break;
		case FtpDataSession::EN_ACTION_UPLOAD:
			{//上传
				_ftp_string s = _Ftp.Proto_226("Successfully transferred.");
				Send(s.c_str(), s.size());
			}
			break;
		}

		sptr->Close();

		/*_ftp_string s = NormcasePath(sPath);

		_ftp_string sData;
		_file_::CMyDirectory::RecursiveAllToDepth(s.c_str(),
			function20_bind(&FtpCmdSession::HandleSearchFile, this, &sData,
				_function_::_1, _function_::_2), 1);

		sptr->Send(sData.c_str(), sData.size());*/

		//2000后断开连接
		/*GetTimerEventKMInstance()->push_back(function20_bind(&FtpDataSession::Close, sptr),
			2000);*/

		/*s = _Ftp.Proto_226("File sent ok.");
		Send(s.c_str(), s.size());

		sptr->Close();

		//test
		printf("FtpCmdSession::HandleListReady(%s) End\r\n", s.c_str());
		//test*/
	}

	/*void FtpCmdSession::HandleSearchFile(_ftp_string* sData, bool isDir, const char* c_szPath)
	{
		_file_::CMyFile::tagFile_INFO Info;
		Info.Ower = "FTP";
		if (_file_::CMyFile::GetDetailedInfo(c_szPath, &Info) == 1)
		{
			*sData += FtpList::GenerateString(Info).c_str();
			*sData += "\r\n";
		}
	}*/

	void FtpCmdSession::HandleDataRecv(const _ftp_string& sPath, const FtpDataSession_sptr& sptr,
		NETHANDLE hNet, const StreamBuf_ptr& Stream_sptr)
	{

	}

	_ftp_string FtpCmdSession::NormcasePath(const _ftp_string& sDir)
	{
#if defined( PLATFORM_OS_FAMILY_WINDOWS )
		_ftp_string sPath;
		if (sDir[0] == '/')
			sPath = _SessionInfo._sRootDir + sDir;
		else
			sPath = sDir;
		return const_cast<_ftp_string&>(sPath).replace("/", "\\");
#else
		return sDir;
#endif
	}
}

