
#include <libFrameWork_WebsocketCommon/HandSharke_Parse.h>

namespace _websocket_ {

	HandSharkeParse::Hdr_INFO_sptr	HandSharkeParse::Alloc(void)
	{
		static	MemPool_type	s_Pool(sizeof(Hdr_INFO));
		return Hdr_INFO_sptr(s_Pool.AllocObj<Hdr_INFO>(),
			function20_bind_Ex(&MemPool_type::FreeObj<Hdr_INFO>, &s_Pool));
	}

	HandSharkeParse::HandSharkeParse(void)
	{
		Init();
	}

	HandSharkeParse::~HandSharkeParse(void)
	{
		Release();
	}

	int HandSharkeParse::Init(void)
	{
		return 1;
	}

	void HandSharkeParse::Release(void)
	{
		_Map.clear();
	}

	int HandSharkeParse::GetHeadEndPos(const char* c_szBuf, UInt32 uSize)
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

	int HandSharkeParse::GetEndPos(const char* c_szBuf, UInt32 uSize)
	{
		const int iLen = strlen("\r\n\r\n");
		const char* pEnd = strstr(c_szBuf, "\r\n\r\n");
		if (pEnd != NULL)
		{
			_string_t str = _string_t(c_szBuf, pEnd - c_szBuf).strim();
			return ((int)(pEnd - c_szBuf)) + iLen;
		}

		return -1;
	}

	bool HandSharkeParse::isRespone(const char* c_szBuf, UInt32 uSize)
	{
		const char* pHead = "HTTP";
		const int iLen = strlen(pHead);
		if (uSize > 4 && strncmp(c_szBuf, pHead, iLen) == 0 &&
			(c_szBuf[iLen] == '/' || c_szBuf[iLen] == ' ' || c_szBuf[iLen] == '\t'))
			return true;

		return false;
	}

	_string_t HandSharkeParse::GetVersion(const char* c_szBuf, UInt32 uSize)
	{
		if (GetEndPos(c_szBuf, uSize) != -1)
		{
			if (!isRespone(c_szBuf, uSize))
			{//请求
				char* pEnd = strstr((char*)c_szBuf, "\r\n");
				char* pCur = pEnd;
				if (pEnd != NULL)
				{
					while (pCur != c_szBuf)
					{
						if (pCur[0] == ' ')
						{
							break;
						}

						pCur--;
					}

					if (pCur != c_szBuf)
					{
						return _string_t(pCur + 1, (int)(pEnd - pCur - 1));
					}
				};
			}
			else
			{//响应
				char* pCur = strchr((char*)c_szBuf, ' ');
				if (pCur != NULL)
					return _string_t(c_szBuf, (int)(pCur - c_szBuf));
			}
		}

		return "";
	}

	_string_t HandSharkeParse::GetMethod(const char* c_szBuf, UInt32 uSize)
	{
		if (GetEndPos(c_szBuf, uSize) != -1 && !isRespone(c_szBuf, uSize))
		{
			char* pCur = strchr((char*)c_szBuf, ' ');
			if (pCur != NULL)
			{
				return _string_t(c_szBuf, (int)(pCur - c_szBuf));
			}
		}

		return "";
	}

	int HandSharkeParse::Parse(const char* c_szBuf, UInt32 uSize)
	{
		int iActiveLen = GetEndPos(c_szBuf, uSize);
		if (iActiveLen != -1)
		{
			const char* pCur = c_szBuf;

			int iPos = -1;
			do
			{
				Hdr_INFO_sptr sptr = ParseLine(pCur, iActiveLen - (int)(pCur - c_szBuf), &iPos);
				if (sptr)
				{
					//test
					//printf("name:%s, content:%s\r\n", sptr->Name.c_str(), sptr->Content.c_str());
					//test

					_Map.insert(std::make_pair(sptr->Name.c_str(), sptr));
				}
				pCur += iPos;
				if (iPos == -1 || iActiveLen - ((int)(pCur - c_szBuf)) < 4)
					break;
			} while (true);
		}

		return iActiveLen;
	}

	HandSharkeParse::Hdr_INFO_sptr HandSharkeParse::ParseLine(const char* szData, int iDataSize, int* pReadLen)
	{
		Hdr_INFO_sptr	sptr;
		int iRet = -1;
		char* pStart = (char*)szData;
		char* pCur = strchr(pStart, ':');
		*pReadLen = -1;
		if (pCur != NULL)
		{
			sptr = HandSharkeParse::Alloc();
			sptr->Name = _string_t(pStart, ((int)(pCur - pStart))).strim();
			pCur += 1;
			pStart = pCur;
			pCur = strstr((char*)pCur, "\r\n");
			if (pCur != NULL)
			{
				sptr->Content = _string_t(pStart, ((int)(pCur - pStart))).strim();
				*pReadLen = pCur - szData + 2;// 加上最后的"\r\n"结束符
			}
		}

		return sptr;
	}

	HandSharkeParse::Hdr_INFO_sptr HandSharkeParse::FindContent(const char* FieldName)
	{
		Hdr_INFO_sptr  sptr;
		Msg_Map::iterator iter = _Map.find(FieldName);
		if (iter != _Map.end())
			sptr = iter->second;

		return sptr;
	}

}

