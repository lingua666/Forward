
#include <libFrameWork_FtpCommon/Ftp.h>

namespace _ftp_ {

	Ftp::Ftp(void)
	{

	}

	Ftp::~Ftp(void)
	{
		Release();
	}

	int Ftp::Init(void)
	{
		return 1;
	}

	void Ftp::Release(void)
	{

	}

	int Ftp::Parse_Head(const char* c_szData, UInt32 uSize, tagHead_INFO* pOut_Info)
	{
		int iRet = -1;
		if (isVaild(c_szData, uSize))
		{
			_ftp_string sCode;
			const char* str = NULL;
			if ((c_szData[0] >= 'A' && c_szData[0] <= 'Z') ||
				(c_szData[0] >= 'a' && c_szData[0] <= 'z'))
			{
				pOut_Info->isReq = true;
				str = strchr(c_szData, ' ');
				if (str != NULL)
					sCode = _ftp_string(c_szData, str - c_szData);
				else
					sCode = _ftp_string(c_szData, uSize - 2);
			}
			else
			{
				pOut_Info->isReq = false;
				str = strchr(c_szData, ' ');
				if (str != NULL)
					sCode = _ftp_string(c_szData, str - c_szData);
			}

			if (sCode != "")
			{
				if( str != NULL )
					iRet = str - c_szData + 1;
				else
					iRet = uSize - 2;

				if (!pOut_Info->isReq)
				{
					pOut_Info->CodeType = _ftp_string::StringToInt(sCode);
				}
				else
				{
					for (int i = 0; i < sizeof(g_ftp_req_name202010091137) / sizeof(_ftp_string); i++)
					{
						if (g_ftp_req_name202010091137[i] == sCode)
						{
							pOut_Info->CodeType = i;
							break;
						}
					}
				}
			}
		}

		return iRet;
	}

	_ftp_string	Ftp::FillData(const tagHead_INFO& Info, const char* c_szParam, const char* c_szRemark)
	{
		_ftp_string Data;
		if (Info.isReq)
		{//请求
			Data += g_ftp_req_name202010091137[Info.CodeType];
		}
		else
		{
			Data += _ftp_string::NumberToStr(Info.CodeType);
		}

		if (c_szParam != NULL)
		{
			Data += " ";
			Data += c_szParam;
		}

		Data += "\r\n";

		return Data;
	}

	int Ftp::Parse_Content(const char* c_szData, UInt32 uSize)
	{
		return 1;
	}

	_ftp_string Ftp::ParseUSER(const char* c_szData, UInt32 uSize)
	{
		return _ftp_string(c_szData, uSize - 2);//出去换行符
	}

	_ftp_string Ftp::ParsePASS(const char* c_szData, UInt32 uSize)
	{
		return _ftp_string(c_szData, uSize - 2);
	}

	_ftp_string Ftp::ParsePWD(const char* c_szData, UInt32 uSize)
	{
		_ftp_string Ret;
		if (uSize > 0)
		{
			Ret = _foundation_::ScanChar::FindValueAndInc((char**)&c_szData, "\"", "\"");
		}

		return Ret;
	}

	_ftp_string Ftp::ParseCWD(const char* c_szData, UInt32 uSize)
	{
		return _ftp_string(c_szData, uSize - 2);
	}

	_ftp_string Ftp::ParseTYPE(const char* c_szData, UInt32 uSize)
	{
		return _ftp_string(c_szData, uSize - 2);
	}

	_ftp_string Ftp::ParseMKD(const char* c_szData, UInt32 uSize)
	{
		return _ftp_string(c_szData, uSize - 2);
	}

	_ftp_string Ftp::ParseRMD(const char* c_szData, UInt32 uSize)
	{
		return _ftp_string(c_szData, uSize - 2);
	}

	_ftp_string Ftp::ParseDELE(const char* c_szData, UInt32 uSize)
	{
		return _ftp_string(c_szData, uSize - 2);
	}

	_ftp_string Ftp::ParseCDUP(const char* c_szData, UInt32 uSize)
	{
		return _ftp_string(c_szData, uSize - 2);
	}

	_ftp_string Ftp::ParseRETR(const char* c_szData, UInt32 uSize)
	{
		UInt8 uLen = strlen("RETR ");
		return _ftp_string(&c_szData[uLen], uSize - uLen - 2);
	}

	_ftp_string Ftp::ParseSTOR(const char* c_szData, UInt32 uSize)
	{
		UInt8 uLen = strlen("STOR ");
		return _ftp_string(&c_szData[uLen], uSize - uLen - 2);
	}

	int Ftp::ParsePASV(const char* c_szData, UInt32 uSize, tagHost_INFO* pOut_Host)
	{
		_ftp_string sInfo = _foundation_::ScanChar::FindValueAndInc((char**)&c_szData, "(", ")");
		int Offset1 = sInfo.rfind(",");
		_ftp_string sP2 = sInfo.substr(Offset1 + 1);
		int Offset2 = sInfo.rfind(",", Offset1 - 1);
		_ftp_string sP1 = sInfo.substr(Offset2 + 1, Offset1 - Offset2 - 1);
		pOut_Host->sIP = sInfo.substr(0, Offset2).replace(",", ".");
		pOut_Host->uPort = _ftp_string::StringToInt(sP1) * 256 + _ftp_string::StringToInt(sP2);
		return 1;
	}

	_ftp_string Ftp::ParseAckCWD(const char* c_szData, UInt32 uSize)
	{
		return ParsePWD(c_szData, uSize);
	}

	bool Ftp::isVaild(const char* c_szData, UInt32 uSize)
	{
		return strstr(c_szData, "\r\n") ? true : false;
	}

	_ftp_string Ftp::Proto_Welcome(void)
	{
		tagHead_INFO Info = { false, FTP_MEMBER_GET_ENUM(220) };

		return FillData(Info, "Welcome");
	}

	_ftp_string	Ftp::Proto_USER(const char* c_szUserName)
	{
		tagHead_INFO Info = { true, FTP_MEMBER_GET_ENUM(USER) };

		return FillData(Info, c_szUserName);
	}

	_ftp_string	Ftp::Proto_PASS(const char* c_szPwd)
	{
		tagHead_INFO Info = { true, FTP_MEMBER_GET_ENUM(PASS) };

		return FillData(Info, c_szPwd);
	}

	_ftp_string	Ftp::Proto_SYST(void)
	{
		tagHead_INFO Info = { true, FTP_MEMBER_GET_ENUM(SYST) };

		return FillData(Info);
	}

	_ftp_string	Ftp::Proto_FEAT(void)
	{
		tagHead_INFO Info = { true, FTP_MEMBER_GET_ENUM(FEAT) };

		return FillData(Info);
	}

	_ftp_string	Ftp::Proto_PWD(void)
	{
		tagHead_INFO Info = { true, FTP_MEMBER_GET_ENUM(PWD) };

		return FillData(Info);
	}

	_ftp_string	Ftp::Proto_CWD(const char* c_szDir)
	{
		tagHead_INFO Info = { true, FTP_MEMBER_GET_ENUM(CWD) };

		return FillData(Info, c_szDir);
	}

	_ftp_string	Ftp::Proto_PASV(void)
	{
		tagHead_INFO Info = { true, FTP_MEMBER_GET_ENUM(PASV) };

		return FillData(Info);
	}

	_ftp_string	Ftp::Proto_PORT(void)
	{
		tagHead_INFO Info = { true, FTP_MEMBER_GET_ENUM(PORT) };

		return FillData(Info);
	}

	_ftp_string	Ftp::Proto_CDUP(void)
	{
		tagHead_INFO Info = { true, FTP_MEMBER_GET_ENUM(CDUP) };

		return FillData(Info);
	}

	_ftp_string	Ftp::Proto_RMD(const char* c_szDir)
	{
		tagHead_INFO Info = { true, FTP_MEMBER_GET_ENUM(RMD) };

		return FillData(Info, c_szDir);
	}

	_ftp_string	Ftp::Proto_DELE(const char* c_szPath)
	{
		tagHead_INFO Info = { true, FTP_MEMBER_GET_ENUM(DELE) };

		return FillData(Info, c_szPath);
	}

	_ftp_string	Ftp::Proto_TYPE(const char* c_szType)
	{
		tagHead_INFO Info = { true, FTP_MEMBER_GET_ENUM(TYPE) };

		return FillData(Info, c_szType);
	}

	//下载
	_ftp_string	Ftp::Proto_RETR(const char* c_szPath)
	{
		tagHead_INFO Info = { true, FTP_MEMBER_GET_ENUM(RETR) };

		return FillData(Info, c_szPath);
	}

	//上传
	_ftp_string	Ftp::Proto_STOR(const char* c_szPath)
	{
		tagHead_INFO Info = { true, FTP_MEMBER_GET_ENUM(STOR) };

		return FillData(Info, c_szPath);
	}

	//列表
	_ftp_string	Ftp::Proto_LIST(void)
	{
		tagHead_INFO Info = { true, FTP_MEMBER_GET_ENUM(LIST) };

		return FillData(Info);
	}

	//操作成功
	_ftp_string	Ftp::Proto_150(const char* c_szRemark)
	{
		tagHead_INFO Info = { false, FTP_MEMBER_GET_ENUM(150) };

		return FillData(Info, c_szRemark);
	}

	//
	_ftp_string	Ftp::Proto_200(const char* c_szRemark)
	{
		tagHead_INFO Info = { false, FTP_MEMBER_GET_ENUM(200) };

		return FillData(Info, c_szRemark);
	}

	//
	_ftp_string	Ftp::Proto_202(const char* c_szRemark)
	{
		tagHead_INFO Info = { false, FTP_MEMBER_GET_ENUM(202) };

		return FillData(Info, c_szRemark);
	}

	//系统类型回复
	_ftp_string	Ftp::Proto_215(const char* c_szRemark)
	{
		tagHead_INFO Info = { false, FTP_MEMBER_GET_ENUM(215) };

		return FillData(Info, c_szRemark);
	}


	//结束数据连接
	_ftp_string	Ftp::Proto_226(const char* c_szRemark)
	{
		tagHead_INFO Info = { false, FTP_MEMBER_GET_ENUM(226) };

		return FillData(Info, c_szRemark);
	}

	//
	_ftp_string	Ftp::Proto_227(tagHost_INFO* pOut_Host)
	{
		tagHead_INFO Info = { false, FTP_MEMBER_GET_ENUM(227) };
		_ftp_string s = "Entering Passive Mode (";
		s += pOut_Host->sIP.replace(".", ",");
		s += "," + _ftp_string::NumberToStr(pOut_Host->uPort / 256);
		s += "," + _ftp_string::NumberToStr(pOut_Host->uPort % 256);
		s += ").";

		return FillData(Info, s.c_str());
	}

	//
	_ftp_string	Ftp::Proto_230(const char* c_szRemark)
	{
		tagHead_INFO Info = { false, FTP_MEMBER_GET_ENUM(230) };

		return FillData(Info, c_szRemark);
	}

	//文件行为完成
	_ftp_string	Ftp::Proto_250(const char* c_szRemark)
	{
		tagHead_INFO Info = { false, FTP_MEMBER_GET_ENUM(250) };

		return FillData(Info, c_szRemark);
	}

	//
	_ftp_string	Ftp::Proto_257(const char* c_szRemark)
	{
		tagHead_INFO Info = { false, FTP_MEMBER_GET_ENUM(257) };

		return FillData(Info, c_szRemark);
	}

	//要求密码
	_ftp_string	Ftp::Proto_331(const char* c_szRemark)
	{
		tagHead_INFO Info = { false, FTP_MEMBER_GET_ENUM(331) };

		return FillData(Info, c_szRemark);
	}

	//无法打开数据连接
	_ftp_string	Ftp::Proto_425(const char* c_szRemark)
	{
		tagHead_INFO Info = { false, FTP_MEMBER_GET_ENUM(425) };

		return FillData(Info, c_szRemark);
	}

	// 文件不可用
	_ftp_string	Ftp::Proto_450(const char* c_szRemark)
	{
		tagHead_INFO Info = { false, FTP_MEMBER_GET_ENUM(450) };

		return FillData(Info, c_szRemark);
	}

	// 该操作不允许
	_ftp_string	Ftp::Proto_530(const char* c_szRemark)
	{
		tagHead_INFO Info = { false, FTP_MEMBER_GET_ENUM(530) };

		return FillData(Info, c_szRemark);
	}

	// 文件不可用
	_ftp_string	Ftp::Proto_550(const char* c_szRemark)
	{
		tagHead_INFO Info = { false, FTP_MEMBER_GET_ENUM(550) };

		return FillData(Info, c_szRemark);
	}

}

