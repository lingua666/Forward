#include <libProto_Sip/Sip_Parse_Data.h>
#include <libProto_Sip/Sip_Request.h>
#include <libProto_Sip/Sip_Response.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

void Test(void)
{
	//心跳
	_sip_::Sip_Request_Message	Heart;
	Heart.Init({ "192.168.8.120",5060 }, 1, false);
	Heart.SetVia({ "192.168.8.110", 8080 }, "xxBranchyy");
	Heart.SetFromUri("10000", "from.sip.d.", "xxTagyy");
	Heart.SetToUri("10000", "to.sip.d.", "");
	Heart.SetCallID("xxxxCallIDyyyy");
	Heart.SetContent("123456789", "text/plain");
	_sip_::_string_big_t str_big = Heart.Generate();

	printf("Sip_Request_Message:%s\r\n", str_big.c_str());

	//解析
	_sip_::Sip_Parse_Request ParseMessaage;
	ParseMessaage.Parse(str_big.c_str(), str_big.size());
	const _sip_::Via_Hdr_INFO& Via = ParseMessaage.GetFirstVia();
	const _sip_::Uri_INFO& FromUri = ParseMessaage.GetFromUri();
	const _sip_::Uri_INFO& ToUri = ParseMessaage.GetToUri();
	const _sip_::_string_t& MethonName = ParseMessaage.GetMethodName();
	const _sip_::_string_t& Content = ParseMessaage.GetContent();

	//心跳响应
	_sip_::Sip_Response HeartAck;
	HeartAck.Init(_sip_::SIP_SC_OK);
	HeartAck.AddVia(ParseMessaage.FindData(_sip_::SIP_H_VIA), "192.168.1.164");
	HeartAck.AddFields(_sip_::SIP_H_FROM, ParseMessaage.FindData(_sip_::SIP_H_FROM));
	HeartAck.AddToUri(ParseMessaage.FindData(_sip_::SIP_H_TO), _sip_::ISip_Proto::GenerateTag());
	HeartAck.AddFields(_sip_::SIP_H_CALL_ID, ParseMessaage.FindData(_sip_::SIP_H_CALL_ID));
	HeartAck.AddFields(_sip_::SIP_H_MAX_FORWARDS, ParseMessaage.FindData(_sip_::SIP_H_MAX_FORWARDS));
	HeartAck.AddFields(_sip_::SIP_H_CSEQ, ParseMessaage.FindData(_sip_::SIP_H_CSEQ));
	HeartAck.AddFields(_sip_::SIP_H_EXPIRES, ParseMessaage.FindData(_sip_::SIP_H_EXPIRES));
	HeartAck.AddFields(_sip_::SIP_H_CONTACT, ParseMessaage.FindData(_sip_::SIP_H_CONTACT));

	HeartAck.SetContent("1234", "text/plain");
	str_big = HeartAck.Generate(true);

	printf("Sip_Response_Message:%s\r\n", str_big.c_str());

	//注册请求
	_sip_::Sip_Request_Register	Reg;
	Reg.Init({ "192.168.8.120",5060 }, 1, 3600);
	Reg.SetVia({ "192.168.8.110",8080 }, "xxBranchyy");
	Reg.SetFromUri("10000", ".sip.d.", "xxTagyy");
	Reg.SetToUri("10000", ".sip.d.", "");
	Reg.SetCallID("xxxxCallIDyyyy");
	str_big = Reg.Generate();

	printf("注册请求:%s\r\n", str_big.c_str());

	//注册响应
	ParseMessaage.Parse(str_big.c_str(), str_big.size());
	HeartAck.Init(_sip_::SIP_SC_OK);
	HeartAck.AddVia(ParseMessaage.FindData(_sip_::SIP_H_VIA), "192.168.1.164");
	HeartAck.AddFields(_sip_::SIP_H_FROM, ParseMessaage.FindData(_sip_::SIP_H_FROM));
	HeartAck.AddToUri(ParseMessaage.FindData(_sip_::SIP_H_TO), _sip_::ISip_Proto::GenerateTag());
	HeartAck.AddFields(_sip_::SIP_H_CALL_ID, ParseMessaage.FindData(_sip_::SIP_H_CALL_ID));
	HeartAck.AddFields(_sip_::SIP_H_MAX_FORWARDS, ParseMessaage.FindData(_sip_::SIP_H_MAX_FORWARDS));
	HeartAck.AddFields(_sip_::SIP_H_CSEQ, ParseMessaage.FindData(_sip_::SIP_H_CSEQ));
	HeartAck.AddFields(_sip_::SIP_H_EXPIRES, ParseMessaage.FindData(_sip_::SIP_H_EXPIRES));
	HeartAck.AddFields(_sip_::SIP_H_CONTACT, ParseMessaage.FindData(_sip_::SIP_H_CONTACT));
	str_big = HeartAck.Generate(true);

	printf("注册响应 Respond:%s\r\n", str_big.c_str());


	//注销请求
	Reg.Init({ "192.168.8.120",5060 }, 0, 1);
	Reg.SetVia({ "192.168.8.110",8080 }, "xxBranchyy");
	Reg.SetFromUri("10000", ".sip.d.", "xxTagyy");
	Reg.SetToUri("10000", ".sip.d.", "");
	Reg.SetCallID("xxxxCallIDyyyy");
	str_big = Reg.Generate();

	printf("注销请求:%s\r\n", str_big.c_str());

	//注销响应
	ParseMessaage.Parse(str_big.c_str(), str_big.size());
	HeartAck.Init(_sip_::SIP_SC_OK);
	HeartAck.AddVia(ParseMessaage.FindData(_sip_::SIP_H_VIA), "192.168.1.164");
	HeartAck.AddFields(_sip_::SIP_H_FROM, ParseMessaage.FindData(_sip_::SIP_H_FROM));
	HeartAck.AddToUri(ParseMessaage.FindData(_sip_::SIP_H_TO), _sip_::ISip_Proto::GenerateTag());
	HeartAck.AddFields(_sip_::SIP_H_CALL_ID, ParseMessaage.FindData(_sip_::SIP_H_CALL_ID));
	HeartAck.AddFields(_sip_::SIP_H_MAX_FORWARDS, ParseMessaage.FindData(_sip_::SIP_H_MAX_FORWARDS));
	HeartAck.AddFields(_sip_::SIP_H_CSEQ, ParseMessaage.FindData(_sip_::SIP_H_CSEQ));
	HeartAck.AddFields(_sip_::SIP_H_EXPIRES, ParseMessaage.FindData(_sip_::SIP_H_EXPIRES));
	HeartAck.AddFields(_sip_::SIP_H_CONTACT, ParseMessaage.FindData(_sip_::SIP_H_CONTACT));
	str_big = HeartAck.Generate(true);

	printf("注销响应:%s\r\n", str_big.c_str());

	//获取任务列表
	_sip_::Sip_Request_Message	Message;
	Message.Init({ "192.168.8.120",5060 }, 1, false);
	Message.SetVia({ "192.168.8.110", 8080 }, "xxBranchyy");
	Message.SetFromUri("10000", "from.sip.d.", "xxTagyy");
	Message.SetToUri("10000", "to.sip.d.", "");
	Message.SetCallID("xxxxCallIDyyyy");
	Message.SetContent("123456789", "text/plain");
	str_big = Message.Generate();

	printf("获取任务列表请求:%s\r\n", str_big.c_str());

	//获取任务响应
	ParseMessaage.Parse(str_big.c_str(), str_big.size());
	_sip_::Sip_Response MessageAck;
	MessageAck.Init(_sip_::SIP_SC_OK);
	MessageAck.AddVia(ParseMessaage.FindData(_sip_::SIP_H_VIA), "192.168.1.164");
	MessageAck.AddFields(_sip_::SIP_H_FROM, ParseMessaage.FindData(_sip_::SIP_H_FROM));
	MessageAck.AddToUri(ParseMessaage.FindData(_sip_::SIP_H_TO), _sip_::ISip_Proto::GenerateTag());
	MessageAck.AddFields(_sip_::SIP_H_CALL_ID, ParseMessaage.FindData(_sip_::SIP_H_CALL_ID));
	MessageAck.AddFields(_sip_::SIP_H_MAX_FORWARDS, ParseMessaage.FindData(_sip_::SIP_H_MAX_FORWARDS));
	MessageAck.AddFields(_sip_::SIP_H_CSEQ, ParseMessaage.FindData(_sip_::SIP_H_CSEQ));
	MessageAck.AddFields(_sip_::SIP_H_EXPIRES, ParseMessaage.FindData(_sip_::SIP_H_EXPIRES));
	MessageAck.AddFields(_sip_::SIP_H_CONTACT, ParseMessaage.FindData(_sip_::SIP_H_CONTACT));
	MessageAck.SetContent("123456789", "text/plain");

	str_big = MessageAck.Generate(true);

	printf("获取任务列表响应:%s\r\n", str_big.c_str());

	//修改音量请求
	Message.Init({ "192.168.8.120",5060 }, 1, false);
	Message.SetVia({ "192.168.8.110", 8080 }, "xxBranchyy");
	Message.SetFromUri("10000", "from.sip.d.", "xxTagyy");
	Message.SetToUri("10000", "to.sip.d.", "");
	Message.SetCallID("xxxxCallIDyyyy");
	Message.SetContent("123456789", "text/plain");
	str_big = Message.Generate();

	printf("修改音量请求:%s\r\n", str_big.c_str());

	//修改音量请求响应
	ParseMessaage.Parse(str_big.c_str(), str_big.size());
	MessageAck.Init(_sip_::SIP_SC_OK);
	MessageAck.AddVia(ParseMessaage.FindData(_sip_::SIP_H_VIA), "192.168.1.164");
	MessageAck.AddFields(_sip_::SIP_H_FROM, ParseMessaage.FindData(_sip_::SIP_H_FROM));
	MessageAck.AddToUri(ParseMessaage.FindData(_sip_::SIP_H_TO), _sip_::ISip_Proto::GenerateTag());
	MessageAck.AddFields(_sip_::SIP_H_CALL_ID, ParseMessaage.FindData(_sip_::SIP_H_CALL_ID));
	MessageAck.AddFields(_sip_::SIP_H_MAX_FORWARDS, ParseMessaage.FindData(_sip_::SIP_H_MAX_FORWARDS));
	MessageAck.AddFields(_sip_::SIP_H_CSEQ, ParseMessaage.FindData(_sip_::SIP_H_CSEQ));
	MessageAck.AddFields(_sip_::SIP_H_EXPIRES, ParseMessaage.FindData(_sip_::SIP_H_EXPIRES));
	MessageAck.AddFields(_sip_::SIP_H_CONTACT, ParseMessaage.FindData(_sip_::SIP_H_CONTACT));
	MessageAck.SetContent("123456789", "text/plain");
	str_big = MessageAck.Generate(true);

	printf("修改音量请求响应:%s\r\n", str_big.c_str());

	//重启请求
	Message.Init({ "192.168.8.120",5060 }, 1, false);
	Message.SetVia({ "192.168.8.110", 8080 }, "xxBranchyy");
	Message.SetFromUri("10000", "from.sip.d.", "xxTagyy");
	Message.SetToUri("10000", "to.sip.d.", "");
	Message.SetCallID("xxxxCallIDyyyy");
	Message.SetContent("123456789", "text/plain");
	str_big = Message.Generate();

	printf("重启请求:%s\r\n", str_big.c_str());


	//重启请求(响应)
	ParseMessaage.Parse(str_big.c_str(), str_big.size());
	MessageAck.Init(_sip_::SIP_SC_OK);
	MessageAck.AddVia(ParseMessaage.FindData(_sip_::SIP_H_VIA), "192.168.1.164");
	MessageAck.AddFields(_sip_::SIP_H_FROM, ParseMessaage.FindData(_sip_::SIP_H_FROM));
	MessageAck.AddToUri(ParseMessaage.FindData(_sip_::SIP_H_TO), _sip_::ISip_Proto::GenerateTag());
	MessageAck.AddFields(_sip_::SIP_H_CALL_ID, ParseMessaage.FindData(_sip_::SIP_H_CALL_ID));
	MessageAck.AddFields(_sip_::SIP_H_MAX_FORWARDS, ParseMessaage.FindData(_sip_::SIP_H_MAX_FORWARDS));
	MessageAck.AddFields(_sip_::SIP_H_CSEQ, ParseMessaage.FindData(_sip_::SIP_H_CSEQ));
	MessageAck.AddFields(_sip_::SIP_H_EXPIRES, ParseMessaage.FindData(_sip_::SIP_H_EXPIRES));
	MessageAck.AddFields(_sip_::SIP_H_CONTACT, ParseMessaage.FindData(_sip_::SIP_H_CONTACT));
	MessageAck.SetContent("123456789", "text/plain");
	str_big = MessageAck.Generate(true);

	printf("重启请求(响应):%s\r\n", str_big.c_str());

	//一键关闭音频请求
	Message.Init({ "192.168.8.120",5060 }, 1, false);
	Message.SetVia({ "192.168.8.110", 8080 }, "xxBranchyy");
	Message.SetFromUri("10000", "from.sip.d.", "xxTagyy");
	Message.SetToUri("10000", "to.sip.d.", "");
	Message.SetCallID("xxxxCallIDyyyy");
	Message.SetContent("123456789", "text/plain");
	str_big = Message.Generate();

	printf("一键关闭音频请求:%s\r\n", str_big.c_str());

	//一键关闭音频请求(响应)
	ParseMessaage.Parse(str_big.c_str(), str_big.size());
	MessageAck.Init(_sip_::SIP_SC_OK);
	MessageAck.AddVia(ParseMessaage.FindData(_sip_::SIP_H_VIA), "192.168.1.164");
	MessageAck.AddFields(_sip_::SIP_H_FROM, ParseMessaage.FindData(_sip_::SIP_H_FROM));
	MessageAck.AddToUri(ParseMessaage.FindData(_sip_::SIP_H_TO), _sip_::ISip_Proto::GenerateTag());
	MessageAck.AddFields(_sip_::SIP_H_CALL_ID, ParseMessaage.FindData(_sip_::SIP_H_CALL_ID));
	MessageAck.AddFields(_sip_::SIP_H_MAX_FORWARDS, ParseMessaage.FindData(_sip_::SIP_H_MAX_FORWARDS));
	MessageAck.AddFields(_sip_::SIP_H_CSEQ, ParseMessaage.FindData(_sip_::SIP_H_CSEQ));
	MessageAck.AddFields(_sip_::SIP_H_EXPIRES, ParseMessaage.FindData(_sip_::SIP_H_EXPIRES));
	MessageAck.AddFields(_sip_::SIP_H_CONTACT, ParseMessaage.FindData(_sip_::SIP_H_CONTACT));
	MessageAck.SetContent("123456789", "text/plain");
	str_big = MessageAck.Generate(true);

	printf("一键关闭音频请求(响应):%s\r\n", str_big.c_str());


	//实时语音广播请求
	_sip_::Sip_SDP::sdp_session	sdp_ses;
	sdp_ses.proto_version = 0;
	sdp_ses.origin.user = "-";
	sdp_ses.origin.id = 7896;
	sdp_ses.origin.id = 0;
	sdp_ses.origin.net_type = "IN";
	sdp_ses.origin.addr_type = "IP4";
	sdp_ses.origin.addr = "192.168.1.164";
	sdp_ses.name = "Speek";

	sdp_ses.session_info = _sip_::Sip_SDP::Alloc_session_info();
	sdp_ses.session_info->author = "test";
	sdp_ses.session_info->copyright = "namkoo";

	sdp_ses.conn = _sip_::Sip_SDP::Alloc_conn();
	sdp_ses.conn->net_type = "IN";
	sdp_ses.conn->addr_type = "IP4";
	sdp_ses.conn->addr = "192.168.1.164";

	sdp_ses.bandw_count = 3;
	for (int i = 0; i < sdp_ses.bandw_count; i ++)
	{
		sdp_ses.bandw[i] = _sip_::Sip_SDP::Alloc_bandw();
		sdp_ses.bandw[i]->modifier = "AS";
		sdp_ses.bandw[i]->value = 2000 + i;
	}

	sdp_ses.time.start = 0;
	sdp_ses.time.stop = 0;

	sdp_ses.attr_count = 3;
	for (int i = 0; i < sdp_ses.attr_count; i++)
	{
		sdp_ses.attr[i] = _sip_::Sip_SDP::Alloc_attr();
	}

	sdp_ses.attr[0]->name = "fmtp";
	sdp_ses.attr[0]->value = "99 mode-change-capability=2;max-red=0";

	sdp_ses.attr[1]->name = "rtpmap";
	sdp_ses.attr[1]->value = "97 AMR/8000/1";

	sdp_ses.attr[2]->name = "fmtp";
	sdp_ses.attr[2]->value = "105 telephone-event/16000";

	sdp_ses.media_count = 1;
	sdp_ses.media[0] = _sip_::Sip_SDP::Alloc_media();

	sdp_ses.media[0]->desc.media = "audio";
	sdp_ses.media[0]->desc.port = 61000;
	sdp_ses.media[0]->desc.transport = "RTP/AVP";
	sdp_ses.media[0]->desc.fmt_count = 1;
	sdp_ses.media[0]->desc.fmt[0] = "122";

	sdp_ses.media[0]->attr_count = 2;
	sdp_ses.media[0]->attr[0] = _sip_::Sip_SDP::Alloc_attr();
	sdp_ses.media[0]->attr[0]->name = "rtpmap";
	sdp_ses.media[0]->attr[0]->value = "122 PCMU/8000";

	sdp_ses.media[0]->attr[1] = _sip_::Sip_SDP::Alloc_attr();
	sdp_ses.media[0]->attr[1]->name = "sendrecv";
	sdp_ses.media[0]->attr[1]->value = "";

	_sip_::Sip_SDP Sdp;
	Sdp.Copy(sdp_ses);
	_sip_::_string_t swl = Sdp.toString();

	//printf("swl:%s\r\n", swl.c_str());

	_sip_::Sip_Request_Invite Invite;
	Invite.Init({ "192.168.8.120",5060 }, 1, false);
	Invite.SetVia({ "192.168.8.110", 8080 }, "xxBranchyy");
	Invite.SetFromUri("10000", "from.sip.d.", "xxTagyy");
	Invite.SetToUri("10000", "to.sip.d.", "");
	Invite.SetCallID("xxxxCallIDyyyy");
	Invite.SetSdp(Sdp);
	
	str_big = Invite.Generate();
	printf("实时语音广播请求:%s\r\n", str_big.c_str());


	//实时语音广播请求(响应)
	ParseMessaage.Parse(str_big.c_str(), str_big.size());
	_sip_::Sip_Response_Ack InviteAck;
	InviteAck.Init({ "192.168.8.120",5060 }, false);
	InviteAck.AddVia(ParseMessaage.FindData(_sip_::SIP_H_VIA), "192.168.1.164");
	InviteAck.AddFields(_sip_::SIP_H_FROM, ParseMessaage.FindData(_sip_::SIP_H_FROM));
	InviteAck.AddToUri(ParseMessaage.FindData(_sip_::SIP_H_TO), _sip_::ISip_Proto::GenerateTag());
	InviteAck.AddFields(_sip_::SIP_H_CALL_ID, ParseMessaage.FindData(_sip_::SIP_H_CALL_ID));
	InviteAck.AddFields(_sip_::SIP_H_MAX_FORWARDS, ParseMessaage.FindData(_sip_::SIP_H_MAX_FORWARDS));
	InviteAck.SetCSeq(ParseMessaage.GetCSeq());
	InviteAck.AddFields(_sip_::SIP_H_EXPIRES, ParseMessaage.FindData(_sip_::SIP_H_EXPIRES));
	InviteAck.AddFields(_sip_::SIP_H_CONTACT, ParseMessaage.FindData(_sip_::SIP_H_CONTACT));
	//MessageAck.SetContent("123456789", "text/plain");
	str_big = InviteAck.Generate(true);

	printf("实时语音广播请求(响应):%s\r\n", str_big.c_str());

	//语音广播结束请求
	_sip_::Sip_Request_Bye Bye;
	Bye.Init({ "192.168.8.120",5060 }, 1, false);
	Bye.SetVia({ "192.168.8.110", 8080 }, "xxBranchyy");
	Bye.SetFromUri("10000", "from.sip.d.", "xxTagyy");
	Bye.SetToUri("10000", "to.sip.d.", "");
	Bye.SetCallID("xxxxCallIDyyyy");

	str_big = Bye.Generate();
	printf("语音广播结束请求:%s\r\n", str_big.c_str());


	//语音广播结束请求(响应)
	ParseMessaage.Parse(str_big.c_str(), str_big.size());
	MessageAck.Init(_sip_::SIP_SC_OK);
	MessageAck.AddVia(ParseMessaage.FindData(_sip_::SIP_H_VIA), "192.168.1.164");
	MessageAck.AddFields(_sip_::SIP_H_FROM, ParseMessaage.FindData(_sip_::SIP_H_FROM));
	MessageAck.AddToUri(ParseMessaage.FindData(_sip_::SIP_H_TO), _sip_::ISip_Proto::GenerateTag());
	MessageAck.AddFields(_sip_::SIP_H_CALL_ID, ParseMessaage.FindData(_sip_::SIP_H_CALL_ID));
	MessageAck.AddFields(_sip_::SIP_H_MAX_FORWARDS, ParseMessaage.FindData(_sip_::SIP_H_MAX_FORWARDS));
	MessageAck.AddFields(_sip_::SIP_H_CSEQ, ParseMessaage.FindData(_sip_::SIP_H_CSEQ));
	MessageAck.AddFields(_sip_::SIP_H_EXPIRES, ParseMessaage.FindData(_sip_::SIP_H_EXPIRES));
	MessageAck.AddFields(_sip_::SIP_H_CONTACT, ParseMessaage.FindData(_sip_::SIP_H_CONTACT));

	str_big = MessageAck.Generate(true);
	printf("语音广播结束请求(响应):%s\r\n", str_big.c_str());

	/*_sip_::Sip_Response_Register RegAck;
	RegAck.Init(_sip_::SIP_SC_OK);
	str_big = RegAck.Generate();*/

	getchar();
}

int main()  
{
	Test();

	/*while(true)
	{
		char szData[1024 * 2] = { 0 };
		char* pCur = szData;
		FILE* fp = fopen("H:\\sdp(样例).txt", "rb");
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
		FILE* fp = fopen("D:\\Work\\数据.txt", "rb");

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
	}*/

	while (true)
	{
		_sip_::Sip_Request_Register	Reg;
		Reg.Init({ "192.168.8.120",5060 }, 1, 3600);
		Reg.SetVia({ "192.168.8.110",8080 }, "xxBranchyy");
		Reg.SetFromUri("10000", ".sip.d.", "xxTagyy");
		Reg.SetToUri("10000", ".sip.d.", "xxTagyy");
		Reg.SetCallID("xxxxCallIDyyyy");

		_sip_::_string_big_t str_big = Reg.Generate();

		Reg.SetToken("admin", ".sip.d.", "xxxNonceyyy", "xxxResponseyyy");
		//Reg.SetVia({ "192.168.8.111",8080 }, "xxBranchyy");
		str_big = Reg.Generate();
	}

	getchar();
    return 0;
}
