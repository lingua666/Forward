// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <libThirdWrap_Pjsip/PjsipInit.hpp>
#include <libString_v30/string.hpp>
#include <libTimestamp/Timestamp.h>
#include <libMempool/MemPool.h>

namespace	_sip_ {

	typedef _string_v30_::string< MemPool_macro<300> >	_string_t;

	struct Msg_Info
	{
		_string_t Name;
		_string_t Content;
	};

	class Sip_Parse
	{
	public:
		virtual ~Sip_Parse(void) {}

		static bool isRespone(const char* c_szBuf, UInt32 uSize)
		{
			const char* SIP = "SIP";
			const int iLen = strlen(SIP);
			if (uSize > 4 && strncmp(c_szBuf, SIP, iLen) == 0 &&
				(c_szBuf[iLen] == '/' || c_szBuf[iLen] == ' ' || c_szBuf[iLen] == '\t'))
				return true;

			return false;
		}

		int GetEndPos(const char* c_szBuf, UInt32 uSize)
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
				}
			}

			return -1;
		}

		/* SIP version */
		_string_t parse_sip_version(const char* c_szBuf, UInt32 uSize)
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

		virtual int Parse(const char* c_szBuf, UInt32 uSize) = 0;

	};

	class Sip_Parse_Request : public Sip_Parse
	{
	public:
		typedef SmartPTR<Msg_Info>									Msg_Info_sptr;
		typedef Container_Map_type <_string_t, Msg_Info_sptr>		Msg_Map;

	private:
		MemPool_type	_Pool;
		Msg_Map			_Map;
		_string_t		_sMethod;

	public:
		Sip_Parse_Request(void)
			: _Pool(sizeof(Msg_Info))
		{

		}

		~Sip_Parse_Request(void)
		{
			Release();
		}

		int Init(void)
		{

		}

		void Release(void)
		{
			_Map.clear();
		}

		int Parse(const char* c_szBuf, UInt32 uSize)
		{
			int iRet = -1;
			int iActiveLen = GetEndPos(c_szBuf, uSize);
			if (iActiveLen != -1)
			{
				const int iTailedLen = strlen("\r\n");
				if (!isRespone(c_szBuf, iActiveLen))
				{
					char* pStart = NULL;
					char* pCur = strchr((char*)c_szBuf, ' ');
					if (pCur != NULL)
					{
						_sMethod = _string_t(c_szBuf, (int)(pCur - c_szBuf));

						//test
						printf("Method:%s\r\n", _sMethod.c_str());
					}

					pCur = strstr((char*)pCur, "\r\n");
					pCur += iTailedLen;

					int iPos = -1;
					do
					{
						iPos = ParseLine(pCur, iActiveLen - (int)(pCur - c_szBuf));
						pCur += iPos;
						if (iPos == -1 || iActiveLen - ((int)(pCur - c_szBuf)) < 4)
							break;
					} while (true);

				}

			}

			return iRet;
		}

	private:
		int ParseLine(const char* szData, int iDataSize)
		{
			int iRet = -1;
			char* pStart = (char*)szData;
			char* pCur = strchr(pStart, ':');
			if (pCur != NULL)
			{
				Msg_Info_sptr	sptr(_Pool.AllocObj<Msg_Info>(),
					function20_bind_Ex(&MemPool_type::FreeObj<Msg_Info>, &_Pool));

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
							iCount -= 2;//Content-Length中的长度 包含了"\r\n",必须减去
							pCur += 4;	//两个"\r\n"
							sptr->Content = _string_t(pCur, iCount);
							pCur += iCount;
						}
						else
						{
							pCur += 2;	//"\r\n"行结束符
						}
					}

					_Map.insert(std::make_pair(sptr->Name, sptr));

					//test
					printf("name:%s content:%s\r\n", sptr->Name.c_str(), sptr->Content.c_str());

					iRet = pCur - szData + 2;// 加上最后的"\r\n"结束符
				}
			}

			return iRet;
		}
	};

	class Sip_Parse_Response : public Sip_Parse
	{
	public:
		typedef SmartPTR<Msg_Info>									Msg_Info_sptr;
		typedef Container_Map_type <_string_t, Msg_Info_sptr>		Msg_Map;

	private:
		MemPool_type	_Pool;
		Msg_Map			_Map;
	public:
		Sip_Parse_Response(void)
			: _Pool(sizeof(Msg_Info))
		{

		}

		~Sip_Parse_Response(void)
		{
			Release();
		}

		int Init(void)
		{

		}

		void Release(void)
		{
			_Map.clear();
		}

		int Parse(const char* c_szBuf, UInt32 uSize)
		{
			int iRet = -1;
			int iActiveLen = GetEndPos(c_szBuf, uSize);
			if (iActiveLen != -1)
			{
				const int iTailedLen = strlen("\r\n");
				if (isRespone(c_szBuf, iActiveLen))
				{
					char* pStart = NULL;
					char* pCur = strstr((char*)c_szBuf, "\r\n");
					pCur += iTailedLen;

					int iPos = -1;
					do
					{
						iPos = ParseLine(pCur, iActiveLen - (int)(pCur - c_szBuf));
						pCur += iPos;
						if (iPos == -1 || iActiveLen - ((int)(pCur - c_szBuf)) < 4)
							break;
					} while (true);

				}

			}

			return iRet;
		}

	private:
		int ParseLine(const char* szData, int iDataSize)
		{
			int iRet = -1;
			char* pStart = (char*)szData;
			char* pCur = strchr(pStart, ':');
			if (pCur != NULL)
			{
				Msg_Info_sptr	sptr(_Pool.AllocObj<Msg_Info>(),
					function20_bind_Ex(&MemPool_type::FreeObj<Msg_Info>, &_Pool));

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
							iCount -= 2;//Content-Length中的长度 包含了"\r\n",必须减去
							pCur += 4;	//两个"\r\n"
							sptr->Content = _string_t(pCur, iCount);
							pCur += iCount;
						}
					}

					_Map.insert(std::make_pair(sptr->Name, sptr));

					//test
					printf("name:%s content:%s\r\n", sptr->Name.c_str(), sptr->Content.c_str());

					iRet = pCur + 2 - szData;
				}
			}

			return iRet;
		}
	};
}


int main(int argc, char* argv[])
{
	_thirdwrap_::PjsipInit	Init;
	std::cin.get();
	return 0;
}