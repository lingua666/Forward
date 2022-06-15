#include <libProto_Sip/Sip_Parse_Data.h>
#include <libProto_Sip/Sip_Request.h>
#include <libProto_Sip/Sip_Response.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

int main()  
{
	while(true)
	{
		char szData[1024 * 2] = { 0 };
		char* pCur = szData;
		FILE* fp = fopen("H:\\sdp(ÑùÀý).txt", "rb");
		int iLen = fread(szData, 1, sizeof(szData), fp);

		_sip_::Sip_SDP::sdp_session ses;
		_sip_::Sip_SDP				Sdp;
		Sdp.InitSessionSdp(&ses);

		int iOffset = 0;

		do
		{
			if(iLen < 150)
			{
				int i = 0;
			}

			iOffset = Sdp.ParseLine(pCur, iLen, &ses);
			iLen -= iOffset;
			if (iOffset == -1 || iLen <= 0)
			{
				break;
			}

			pCur += iOffset;
		} while (true);

		if (fp != NULL)
		{
			fclose(fp);
			fp = NULL;
		}

		_sip_::_string_t swl = Sdp.toString(ses);
		printf("%s\r\n", swl.c_str());
	}
	

	while (true)
	{
		FILE* fp = fopen("D:\\Work\\Êý¾Ý.txt", "rb");

		char szData[1024 * 2] = { 0 };
		char* p = szData;
		int iLen = fread(szData, 1, sizeof(szData), fp);
		int iPos = 0;

		do
		{
			if (_sip_::Sip_Parse::isRespone(p, iLen))
			{
				_sip_::Sip_Parse_Response Resp;
				iPos = Resp.Parse(p, iLen);

				_sip_::Sip_Proto_Common Com;
				Com.Parse(p, iLen);
				

				_sip_::_string_t sRet = Resp.FindData(_sip_::SIP_H_VIA, "branch=", ";|EOF");
				printf("FindData(Response):%s\r\n", sRet.c_str());
				if (iPos > 0)
				{
					p += iPos;
					iLen -= iPos;
				};
			}
			else
			{
				_sip_::Sip_Parse_Request Request;
				iPos = Request.Parse(p, iLen);

				_sip_::Sip_Proto_Common Com;
				Com.Parse(p, iLen);

				_sip_::_string_t sRet = Request.FindData(_sip_::SIP_H_VIA, "branch=", ";|EOF");
				printf("FindData(Request):%s\r\n", sRet.c_str());
				if (iPos > 0)
				{
					p += iPos;
					iLen -= iPos;
				};
			}

		} while (iLen > 0);

		if (fp != NULL)
		{
			fclose(fp);
			fp = NULL;
		}
	}

	while (true)
	{
		_sip_::Sip_Request_Register	Reg;
		Reg.Init({ "192.168.8.120",5060 }, 1);
		Reg.SetVia({ "192.168.8.110",8080 }, "xxBranchyy");
		Reg.SetFromTo("10000", ".sip.d.", "xxTagyy");
		Reg.SetCallID("xxxxCallIDyyyy");

		_sip_::_string_big_t str_big = Reg.Generate();

		Reg.SetToken("admin", ".sip.d.", "xxxNonceyyy", "xxxResponseyyy");
		//Reg.SetVia({ "192.168.8.111",8080 }, "xxBranchyy");
		str_big = Reg.Generate();
	}

	getchar();
    return 0;  
}
