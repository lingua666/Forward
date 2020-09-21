
#include <libProto_Sip/Sip_Parse_Data.h>

namespace	_sip_ {

	/******************************Sip_IParse******************************/

	Sip_IParse::Sip_IParse(void)
	{}

	Sip_IParse::~Sip_IParse(void)
	{}

	const Uri_INFO&	Sip_IParse::GetFromUri(void) const
	{
		return _Common.GetFromUri();
	}

	const Uri_INFO&	Sip_IParse::GetToUri(void) const
	{
		return _Common.GetToUri();
	}

	const Via_Hdr_INFO& Sip_IParse::GetFirstVia(void)
	{
		return _Common.GetFirstVia();
	}

	const _string_t& Sip_IParse::GetCallID(void) const
	{
		return _Common.GetCallID();
	}

	const long& Sip_IParse::GetCSeq(void) const
	{
		return _Common.GetCSeq();
	}

	const _string_t& Sip_IParse::GetMethodName(void) const
	{
		return _Common.GetMethodName();
	}

	const unsigned char& Sip_IParse::GetMaxForwards(void) const
	{
		return _Common.GetMaxForwards();
	}

	const Sip_Proto_Common&	Sip_IParse::GetProtoCommon(void) const
	{
		return _Common;
	}



	/******************************Sip_Parse_Request******************************/

	Sip_Parse_Request::Sip_Parse_Request(void)
	{

	}

	Sip_Parse_Request::~Sip_Parse_Request(void)
	{
		Release();
	}

	int Sip_Parse_Request::Init(void)
	{
		return 1;
	}

	void Sip_Parse_Request::Release(void)
	{
		_Map.clear();
	}

	//返回值为完整包的结束位置，包含换行符，如果存在多个包，也即为下个包的起始位置
	int Sip_Parse_Request::Parse(const char* c_szBuf, UInt32 uSize)
	{
		return ParseToMap(c_szBuf, uSize, &_Map);
	}

	//返回值为完整包的结束位置，包含换行符，如果存在多个包，也即为下个包的起始位置
	int Sip_Parse_Request::ParseToMap(const char* c_szBuf, UInt32 uSize, Msg_Map* pMap)
	{
		int iActiveLen = Sip_Parse::GetEndPos(c_szBuf, uSize);
		if (iActiveLen != -1)
		{
			const int iTailedLen = strlen("\r\n");
			if (!Sip_Parse::isRespone(c_szBuf, iActiveLen))
			{
				pMap->clear();//清除上次解析结果

				_sMethod = Sip_Parse::GetMethod(c_szBuf, uSize);

				//test
				printf("Method:%s\r\n", _sMethod.c_str());

				const char* pCur = c_szBuf + Sip_Parse::GetHeadEndPos(c_szBuf, uSize);

				int iPos = -1;
				do
				{
					Hdr_INFO_sptr sptr = Sip_Parse::ParseLine(pCur, iActiveLen - (int)(pCur - c_szBuf), &iPos);
					if (sptr)
					{
						//test
						printf("name:%s, content:%s\r\n", sptr->Name.c_str(), sptr->Content.c_str());

						_Common.ParseHdr(sptr);
						pMap->insert(std::make_pair(sptr->Name.c_str(), sptr));
					}
					pCur += iPos;
					if (iPos == -1 || iActiveLen - ((int)(pCur - c_szBuf)) < 4)
						break;
				} while (true);

			}
		}

		return iActiveLen;
	}

	//"SOF":内容的起始位置			"EOF":内容的结束位置
	_string_t	Sip_Parse_Request::FindData(sip_hdr_e Hdr_e, const _string_t& Start, const _string_t& End)
	{
		//查找匹配内容
		Msg_Map::iterator iter = _Map.find(sip_hdr_names[Hdr_e]);
		if (iter != _Map.end())
		{
			return Sip_Parse::FindData(iter->second->Content, Start, End);
		}

		return "";
	}



	/******************************Sip_Parse_Response******************************/

	Sip_Parse_Response::Sip_Parse_Response(void)
	{

	}

	Sip_Parse_Response::~Sip_Parse_Response(void)
	{
		Release();
	}

	int Sip_Parse_Response::Init(void)
	{
		return 1;
	}

	void Sip_Parse_Response::Release(void)
	{
		_Map.clear();
	}

	//返回值为完整包的结束位置，包含换行符，如果存在多个包，也即为下个包的起始位置
	int Sip_Parse_Response::Parse(const char* c_szBuf, UInt32 uSize)
	{
		return ParseToMap(c_szBuf, uSize, &_Map);
	}

	int Sip_Parse_Response::ParseToMap(const char* c_szBuf, UInt32 uSize, Msg_Map* pMap)
	{
		int iActiveLen = Sip_Parse::GetEndPos(c_szBuf, uSize);
		if (iActiveLen != -1)
		{
			const int iTailedLen = strlen("\r\n");
			if (Sip_Parse::isRespone(c_szBuf, iActiveLen))
			{
				pMap->clear();//清除上次解析结果

				_Status = (sip_status_code)_string_t::StringToInt(Sip_Parse::GetStatueCode(c_szBuf, uSize));

				//test
				printf("sip_status_code:%d,%s\r\n", _Status,
					Sip_Proto_Common::GetStatusCodeString(_Status).c_str());

				const char* pCur = c_szBuf + Sip_Parse::GetHeadEndPos(c_szBuf, uSize);

				int iPos = -1;
				do
				{
					Hdr_INFO_sptr sptr = Sip_Parse::ParseLine(pCur, iActiveLen - (int)(pCur - c_szBuf), &iPos);
					if (sptr)
					{
						printf("name:%s, content:%s\r\n", sptr->Name.c_str(), sptr->Content.c_str());
						_Common.ParseHdr(sptr);
						pMap->insert(std::make_pair(sptr->Name.c_str(), sptr));
					}
					pCur += iPos;
					if (iPos == -1 || iActiveLen - ((int)(pCur - c_szBuf)) < 4)
						break;
				} while (true);

			}
		}

		return iActiveLen;
	}

	//"SOF":内容的起始位置			"EOF":内容的结束位置
	_string_t	Sip_Parse_Response::FindData(sip_hdr_e Hdr_e, const _string_t& Start, const _string_t& End)
	{
		//查找匹配内容
		Msg_Map::iterator iter = _Map.find(sip_hdr_names[Hdr_e]);
		if (iter != _Map.end())
		{
			return Sip_Parse::FindData(iter->second->Content, Start, End);
		}

		return "";
	}

}
