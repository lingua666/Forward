
#include <libProto_Sip_V20/Sip_Parse.h>

namespace	_sip_v20_ {

	bool Sip_Parse::isRespone(const char* c_szBuf, UInt32 uSize)
	{
		const char* SIP = "SIP";
		const int iLen = strlen(SIP);

		if (uSize > 4)
		{
			if( strncmp(c_szBuf, SIP, iLen) == 0 &&
				(c_szBuf[iLen] == '/' || c_szBuf[iLen] == ' ' || c_szBuf[iLen] == '\t'))
			return true;

			UInt8 uCount = strlen("ACK sip:");

			if (uSize > uCount && strncmp(c_szBuf, "ACK sip:", uCount) == 0)
				return true;
		}

		/**if (uSize > 4 && strncmp(c_szBuf, SIP, iLen) == 0 &&
			(c_szBuf[iLen] == '/' || c_szBuf[iLen] == ' ' || c_szBuf[iLen] == '\t'))
			return true;*/

		return false;
	}

	/* SIP Version */
	_string_t Sip_Parse::parse_sip_version(const char* c_szBuf, UInt32 uSize)
	{
		const char* SIP = "SIP";
		char* pStart = strstr((char*)c_szBuf, "Via:");
		if (pStart != NULL)
		{
			pStart += strlen("Via:");
			pStart = strstr(pStart, "SIP/");
			const char* pEnd = strstr(pStart, "/");
			if (pEnd != NULL)
			{
				pStart += strlen("SIP/");
				int iLen = pEnd - pStart;
				return _string_t(pStart, iLen);
			}
		}

		return "";
	}

	/* SIP Transfer Type */
	bool Sip_Parse::is_sip_transfer_udp(const char* c_szBuf, UInt32 uSize)
	{
		char Dst[20] = { 0 };
		char* pStart = strstr((char*)c_szBuf, "Via:");
		if (pStart != NULL)
		{
			pStart += strlen("Via:");
			memcpy(Dst, pStart, sizeof(Dst));
			pStart = strstr(pStart, "/TCP");
			if (pStart != NULL)
				return false;
		}

		return true;
	}

	int Sip_Parse::GetHeadEndPos(const char* c_szBuf, UInt32 uSize)
	{
		if (GetEndPos(c_szBuf, uSize) != -1)
		{
			char* pEnd = strstr((char*)c_szBuf, "\r\n");
			if (pEnd != NULL)
			{
				return (pEnd + 2 - c_szBuf);
			}
		}

		return -1;
	}

	int Sip_Parse::GetEndPos(const char* c_szBuf, UInt32 uSize)
	{
		char* pContent = strstr((char*)c_szBuf, "Content-Length:");
		if (pContent != 0)
		{
			const int iLen = strlen("\r\n\r\n");
			pContent += strlen("Content-Length:");
			const char* pEnd = strstr(pContent, "\r\n\r\n");
			if (pEnd != NULL)
			{
				_string_t str = _string_t(pContent, pEnd - pContent).strim();
				int Content_Length = _string_t::StringToInt(str);
				if (Content_Length == 0)
				{
					return ((int)(pEnd - c_szBuf)) + iLen;
				}
				else if (uSize - (((int)(pEnd - c_szBuf)) + iLen) >= Content_Length)
				{
					return ((int)(pEnd - c_szBuf)) + iLen + Content_Length;
				}
				else
				{
					//test
					printf("Sip_Parse::GetEndPos(%d) Failed, Content_Length:%d:%d!!\r\n", uSize, 
						Content_Length, uSize - (((int)(pEnd - c_szBuf)) + iLen));
					//test
				}
			}
			else
			{
				//test
				printf("Sip_Parse::GetEndPos() Failed, Not Found Content End!!\r\n");
				//test
			}
		}
		else
		{
			//test
			printf("Sip_Parse::GetEndPos() Failed, Not Found Content-Length!!\r\n");
			//test
		}

		return -1;
	}

	_string_t Sip_Parse::GetMethod(const char* c_szBuf, UInt32 uSize)
	{
		if (GetHeadEndPos(c_szBuf, uSize) != -1 && !isRespone(c_szBuf, uSize))
		{
			char* pCur = strchr((char*)c_szBuf, ' ');
			if (pCur != NULL)
			{
				return _string_t(c_szBuf, (int)(pCur - c_szBuf));
			}
		}

		return "";
	}

	_string_t Sip_Parse::GetStatueCode(const char* c_szBuf, UInt32 uSize)
	{
		if (GetHeadEndPos(c_szBuf, uSize) != -1 && isRespone(c_szBuf, uSize))
		{
			char* pStart = strchr((char*)c_szBuf, ' ');
			if (pStart[4] == ' ')
			{
				return _string_t(pStart + 1, 3);
			}
		}

		return "";
	}

	Sip_Parse::Hdr_INFO_sptr Sip_Parse::ParseLine(const char* szData, int iDataSize, int* pReadLen)
	{
		Hdr_INFO_sptr	sptr;
		int iRet = -1;
		char* pStart = (char*)szData;
		char* pCur = strchr(pStart, ':');
		*pReadLen = -1;
		if (pCur != NULL)
		{
			sptr = ISip_Proto::Alloc();
			sptr->Name = _string_t(pStart, ((int)(pCur - pStart))).strim();
			pCur += 1;
			pStart = pCur;
			pCur = strstr((char*)pCur, "\r\n");
			if (pCur != NULL)
			{
				sptr->Content = _string_t(pStart, ((int)(pCur - pStart))).strim();
				if (sptr->Name == "Content-Length")
				{
					int iCount = _string_t::StringToInt(sptr->Content);
					if (iCount > 0 && iCount > 2)
					{
						//iCount -= 2;//Content-Length中的长度 包含了"\r\n",必须减去
						pCur += 4;	//两个"\r\n"
						sptr->Content = _string_t(pCur, iCount);
						pCur += iCount;
					}
					else
					{
						pCur += 2;	//"\r\n"行结束符
					}
				}

				*pReadLen = pCur - szData + 2;// 加上最后的"\r\n"结束符
			}
		}

		return sptr;
	}

	//"SOF":内容的起始位置			"EOF":内容的结束位置
	_string_t	Sip_Parse::FindData(const _string_t& Content,
		const _string_t& Start, const _string_t& End)
	{
		if (Content == "" || Start == "" || End == "")
			return "";

		if (Start == "SOF" && End == "EOF")
			return Content;

		const char cMask = '|';
		int iFilterCount = 0, iLen = 0;

		//解析字符串中的结束过滤条件
		_string_t Filters[10];
		char* pFilterCur = (char*)End.c_str();
		char* pFilterEnd = NULL;
		do
		{
			pFilterEnd = strchr(pFilterCur, cMask);
			if (pFilterEnd == NULL)
			{
				Filters[iFilterCount] = pFilterCur;
				iFilterCount++;
				break;
			}
			else
			{
				iLen = (int)(pFilterEnd - pFilterCur);
				Filters[iFilterCount] = _string_t(pFilterCur, iLen);
				pFilterCur = pFilterCur + 1 + iLen;
				iFilterCount++;
			}

		} while (true);

		//查找匹配内容
		char* pBegin = NULL;
		char* pTmp = NULL;
		if (Start == "SOF")
		{//内容起始位置
			pBegin = (char*)&Content[0];
			pTmp = pBegin;
		}
		else
		{
			pBegin = strstr((char*)&Content[0], Start.c_str());
			pTmp = (char*)(pBegin + Start.size());
		}

		if (pBegin != NULL)
		{
			for (int i = 0; i < iFilterCount; i++)
			{
				const char* pEnd = strstr(pTmp, Filters[i].c_str());
				if (pEnd != NULL)
				{
					return _string_t(pTmp, (int)(pEnd - pTmp));
				}
				else if (Filters[i] == "EOF")//起始到结束
				{
					return	pTmp;
				}
			}
		}

		return "";
	}

}

