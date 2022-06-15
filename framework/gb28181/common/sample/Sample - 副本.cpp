// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <libFrameWork_Gb28181/External.h>
#include "gb28181_req.hpp"
#include "gb28181_rsp.hpp"
#include <sstream>
#define SIP_NAMESPACE _sip_v20_

#define SERVER_ID		"34020000002000000002"
#define SERVER_IP		"192.168.1.58"	
#define SERVER_P0RT		5060
#define SERVER_DOMAIN	"34020000"
#define DEVICE_USERNAME "34020000001320000001"
#define DEVICE_PASSWORD	"12345678"
#define DEVICE_NONCE	"1615184577"
#define DEVICE_RESPONSE	"f587dbc0dba4b43ee9d247c05519451d"

SIP_NAMESPACE::UriDetails_INFO	g_SrcHost;
SIP_NAMESPACE::UriDetails_INFO	g_DstHost;
SIP_NAMESPACE::_string_t		g_sDeviceID;


_string_type GetTokenNone(const char* c_szUserName, const char* c_szPassWord,
	const char* c_szRealm, const char* c_szUri, const char* c_szNonce)
{
	//response计算过程:
	//1. HA1 = MD5(A1) = MD5(username:realm:password)
	//2. qop 值为"auth"或未指定: HA2 = MD5(A2) = MD5(method : uri)
	//3. qop 未指定: response = MD5(HA1:nonce:HA2)

	_uuid_::MD5Engine	Ha1Md5;
	_string_type sData = c_szUserName;
	sData += ":";
	sData += c_szRealm;
	sData += ":";
	sData += c_szPassWord;
	Ha1Md5.update(sData);
	_string_type sHa1 = _uuid_::DigestEngine::digestToHex(Ha1Md5.digest());

	_uuid_::MD5Engine	Ha2Md5;
	sData = "REGISTER:";
	sData += c_szUri;
	Ha2Md5.update(sData);
	_string_type sHa2 = _uuid_::DigestEngine::digestToHex(Ha2Md5.digest());

	_uuid_::MD5Engine	Ha3Md5;
	sData = sHa1 + ":" + c_szNonce + ":" + sHa2;
	Ha3Md5.update(sData);

	_string_type sResponse = _uuid_::DigestEngine::digestToHex(Ha3Md5.digest());
	return sResponse;
}

_string_type GetTokenAuth(const char* c_szUserName, const char* c_szPassWord,
	const char* c_szRealm, const char* c_szUri, const char* c_szNonce,
	const char* c_szNC, const char* c_szCNonce, const char* c_szQop)
{
	//response计算过程:
	//1. HA1 = MD5(A1) = MD5(username:realm:password)
	//2. 如果 qop 值为"auth"或未指定: HA2 = MD5(A2) = MD5(method : uri)
	//3. 如果 qop 值为"auth"或"auth-int": response=MD5(HA1:nonce:nc:cnonce:qop:HA2)
	_uuid_::MD5Engine	Ha1Md5;
	_string_type sData = c_szUserName;
	sData += ":";
	sData += c_szRealm;
	sData += ":";
	sData += c_szPassWord;
	Ha1Md5.update(sData);
	_string_type sHa1 = _uuid_::DigestEngine::digestToHex(Ha1Md5.digest());

	_uuid_::MD5Engine	Ha2Md5;
	sData = "REGISTER:";
	sData += c_szUri;
	Ha2Md5.update(sData);
	_string_type sHa2 = _uuid_::DigestEngine::digestToHex(Ha2Md5.digest());

	_uuid_::MD5Engine	Ha3Md5;
	sData = sHa1 + ":" + c_szNonce + ":" + c_szNC + ":" + c_szCNonce + ":" + c_szQop + ":" + sHa2;
	Ha3Md5.update(sData);

	_string_type sResponse = _uuid_::DigestEngine::digestToHex(Ha3Md5.digest());
	return sResponse;
}

_string_type GetTokenAuthInt(const char* c_szUserName, const char* c_szPassWord,
	const char* c_szRealm, const char* c_szUri, const char* c_szNonce,
	const char* c_szNC, const char* c_szCNonce, const char* c_szQop, const char* c_szBody)
{
	//response计算过程:
	//1. HA1 = MD5(A1) = MD5(username:realm:password)
	//2. 如果 qop 值为"auth-int" : HA2=MD5(A2)=MD5(method:uri:MD5(entity-body))
	//3. 如果 qop 值为"auth"或"auth-int": response=MD5(HA1:nonce:nc:cnonce:qop:HA2)
	_uuid_::MD5Engine	Ha1Md5;
	_string_type sData = c_szUserName;
	sData += ":";
	sData += c_szRealm;
	sData += ":";
	sData += c_szPassWord;
	Ha1Md5.update(sData);
	_string_type sHa1 = _uuid_::DigestEngine::digestToHex(Ha1Md5.digest());

	_uuid_::MD5Engine	BodyMd5;
	BodyMd5.update(c_szBody);
	_string_type sBody = _uuid_::DigestEngine::digestToHex(BodyMd5.digest());

	_uuid_::MD5Engine	Ha2Md5;
	sData = "REGISTER:";
	sData += c_szUri;
	sData += ":";
	sData += sBody;
	Ha2Md5.update(sData);
	_string_type sHa2 = _uuid_::DigestEngine::digestToHex(Ha2Md5.digest());

	_uuid_::MD5Engine	Ha3Md5;
	sData = sHa1 + ":" + c_szNonce + ":" + c_szNC + ":" + c_szCNonce + ":" + c_szQop + ":" + sHa2;
	Ha3Md5.update(sData);

	_string_type sResponse = _uuid_::DigestEngine::digestToHex(Ha3Md5.digest());
	return sResponse;
}

int Send(NETHANDLE hNet, struct sockaddr_in* pSrc, const char* c_szData, UInt16 uSize)
{
	LOG_Print_Info(libFrameWork_GB28181, "Send Data(%d):%s\r\n", uSize, c_szData);
	return GetUDPStreamInstance()->Send(hNet, pSrc, c_szData, uSize);
}

int GetCatalogInfo(NETHANDLE hNet, struct sockaddr_in* pSrc,
	const char* c_szDeviceID, const char* c_szSN,
	const SIP_NAMESPACE::UriDetails_INFO SrcHost, const SIP_NAMESPACE::UriDetails_INFO DstHost)
{
	SIP_NAMESPACE::Sip_Request_Message	Req;
	Req.Init(SrcHost, DstHost, 20, true);
	Req.SetVia(SrcHost.Host, SIP_NAMESPACE::ISip_Proto::GenerateTime());

	SIP_NAMESPACE::_string_t sFrom = SERVER_ID;
	sFrom += "@";
	sFrom += SrcHost.Host.Host;
	if (SrcHost.Host.Port != 0)
	{
		sFrom += ":" + SIP_NAMESPACE::_string_t::NumberToStr(SrcHost.Host.Port);
	}

	SIP_NAMESPACE::_string_t sTo = c_szDeviceID;
	sTo += "@";
	sTo += DstHost.Host.Host;
	if (DstHost.Host.Port != 0)
	{
		sTo += ":" + SIP_NAMESPACE::_string_t::NumberToStr(DstHost.Host.Port);
	}

	Req.SetFromUri("", sFrom, SIP_NAMESPACE::ISip_Proto::GenerateTag());
	Req.SetToUri("", sTo, "");
	Req.SetCallID(SIP_NAMESPACE::ISip_Proto::GenerateCallID());
	Timestamp_type Time;
	SIP_NAMESPACE::_string_t Date = Time.DateString("-").c_str();
	Date += "T";
	Date += Time.MSTimeString(":").c_str();
	Req.SetDate(Date);

	_apps_proto_::_xml_::_gb28181_req_::Catalog Content;
	Content.Query.DeviceID = c_szDeviceID;
	Content.Query.SN = c_szSN;
	Req.SetContent(Content.Save().c_str(), "Application/MANSCDP+xml");

	SIP_NAMESPACE::_string_big_t s = Req.Generate();
	return Send(hNet, pSrc, s.c_str(), s.size());
}

int GetDeviceInfo(NETHANDLE hNet, struct sockaddr_in* pSrc,
	const char* c_szDeviceID, const char* c_szSN,
	const SIP_NAMESPACE::UriDetails_INFO SrcHost, const SIP_NAMESPACE::UriDetails_INFO DstHost)
{
	SIP_NAMESPACE::Sip_Request_Message	Req;
	Req.Init(SrcHost, DstHost, 21, true);
	Req.SetVia(SrcHost.Host, SIP_NAMESPACE::ISip_Proto::GenerateTime());

	SIP_NAMESPACE::_string_t sFrom = SERVER_ID;
	sFrom += "@";
	sFrom += SrcHost.Host.Host;
	if (SrcHost.Host.Port != 0)
	{
		sFrom += ":" + SIP_NAMESPACE::_string_t::NumberToStr(SrcHost.Host.Port);
	}

	SIP_NAMESPACE::_string_t sTo = c_szDeviceID;
	sTo += "@";
	sTo += DstHost.Host.Host;
	if (DstHost.Host.Port != 0)
	{
		sTo += ":" + SIP_NAMESPACE::_string_t::NumberToStr(DstHost.Host.Port);
	}

	Req.SetFromUri("", sFrom, SIP_NAMESPACE::ISip_Proto::GenerateTag());
	Req.SetToUri("", sTo, "");
	Req.SetCallID(SIP_NAMESPACE::ISip_Proto::GenerateCallID());
	Timestamp_type Time;
	SIP_NAMESPACE::_string_t Date = Time.DateString("-").c_str();
	Date += "T";
	Date += Time.MSTimeString(":").c_str();
	Req.SetDate(Date);

	_apps_proto_::_xml_::_gb28181_req_::DeviceInfo Content;
	Content.Query.DeviceID = c_szDeviceID;
	Content.Query.SN = c_szSN;
	Req.SetContent(Content.Save().c_str(), "Application/MANSCDP+xml");

	SIP_NAMESPACE::_string_big_t s = Req.Generate();
	return Send(hNet, pSrc, s.c_str(), s.size());
}

int GetDeviceStatus(NETHANDLE hNet, struct sockaddr_in* pSrc,
	const char* c_szDeviceID, const char* c_szSN,
	const SIP_NAMESPACE::UriDetails_INFO SrcHost, const SIP_NAMESPACE::UriDetails_INFO DstHost)
{
	SIP_NAMESPACE::Sip_Request_Message	Req;
	Req.Init(SrcHost, DstHost, 22, true);
	Req.SetVia(SrcHost.Host, SIP_NAMESPACE::ISip_Proto::GenerateTime());

	SIP_NAMESPACE::_string_t sFrom = SERVER_ID;
	sFrom += "@";
	sFrom += SrcHost.Host.Host;
	if (SrcHost.Host.Port != 0)
	{
		sFrom += ":" + SIP_NAMESPACE::_string_t::NumberToStr(SrcHost.Host.Port);
	}

	SIP_NAMESPACE::_string_t sTo = c_szDeviceID;
	sTo += "@";
	sTo += DstHost.Host.Host;
	if (DstHost.Host.Port != 0)
	{
		sTo += ":" + SIP_NAMESPACE::_string_t::NumberToStr(DstHost.Host.Port);
	}

	Req.SetFromUri("", sFrom, SIP_NAMESPACE::ISip_Proto::GenerateTag());
	Req.SetToUri("", sTo, "");
	Req.SetCallID(SIP_NAMESPACE::ISip_Proto::GenerateCallID());
	Req.SetDate(SIP_NAMESPACE::ISip_Proto::GenerateCallID());
	Timestamp_type Time;
	SIP_NAMESPACE::_string_t Date = Time.DateString("-").c_str();
	Date += "T";
	Date += Time.MSTimeString(":").c_str();
	Req.SetDate(Date);

	_apps_proto_::_xml_::_gb28181_req_::DeviceStatus Content;
	Content.Query.DeviceID = c_szDeviceID;
	Content.Query.SN = c_szSN;
	Req.SetContent(Content.Save().c_str(), "Application/MANSCDP+xml");

	SIP_NAMESPACE::_string_big_t s = Req.Generate();
	return Send(hNet, pSrc, s.c_str(), s.size());
}

int KeepaliveResponse(NETHANDLE hNet, struct sockaddr_in* pSrc, SIP_NAMESPACE::Sip_Parse_Request& Req)
{
	SIP_NAMESPACE::Sip_Response Ack;
	Ack.Init(SIP_NAMESPACE::SIP_SC_OK);
	Ack.AddVia(Req.FindData(SIP_NAMESPACE::SIP_H_VIA), "");
	Ack.AddFields(SIP_NAMESPACE::SIP_H_FROM, Req.FindData(SIP_NAMESPACE::SIP_H_FROM));
	Ack.AddToUri(Req.FindData(SIP_NAMESPACE::SIP_H_TO), SIP_NAMESPACE::ISip_Proto::GenerateTag());
	Ack.AddFields(SIP_NAMESPACE::SIP_H_CALL_ID, Req.FindData(SIP_NAMESPACE::SIP_H_CALL_ID));
	Ack.AddFields(SIP_NAMESPACE::SIP_H_CSEQ, Req.FindData(SIP_NAMESPACE::SIP_H_CSEQ));
	SIP_NAMESPACE::_string_big_t s = Ack.Generate(true);
	return Send(hNet, pSrc, s.c_str(), s.size());
}

void HandleGB28181Destroy(NETHANDLE hNet, UInt16 uPort)
{
	printf("HandleGB28181Destroy\r\n");
}

void HandleGB28181Recv(NETHANDLE hNet, struct sockaddr_in* pSrc, const _io_net_::StreamBuf_ptr& Buf_ptr)
{
	LOG_Print_Info(libFrameWork_GB28181, "HandleGB28181Recv Data(%d):%s", Buf_ptr->payload, Buf_ptr->data);

	SIP_NAMESPACE::Sip_Parse_Request Parse;
	if (Parse.Parse(Buf_ptr->data, Buf_ptr->payload) != -1)
	{
		SIP_NAMESPACE::_string_t sMethod = Parse.GetMethodName();
		if (sMethod == SIP_NAMESPACE::sip_method_names[SIP_NAMESPACE::SIP_REGISTER_METHOD])
		{
			if (Parse.FindData(SIP_NAMESPACE::SIP_H_EXPIRES) == "0")
			{//注销

				return;
			}

			SIP_NAMESPACE::_string_t sAuth = Parse.FindData(SIP_NAMESPACE::SIP_H_AUTHORIZATION);
			if (sAuth == "")
			{//没有接收验证信息
				SIP_NAMESPACE::Sip_Response Ack;
				Ack.Init(SIP_NAMESPACE::SIP_SC_UNAUTHORIZED);
				Ack.AddVia(Parse.FindData(SIP_NAMESPACE::SIP_H_VIA), "");
				Ack.AddFields(SIP_NAMESPACE::SIP_H_FROM, Parse.FindData(SIP_NAMESPACE::SIP_H_FROM));
				Ack.AddToUri(Parse.FindData(SIP_NAMESPACE::SIP_H_TO), SIP_NAMESPACE::ISip_Proto::GenerateTag());
				Ack.AddFields(SIP_NAMESPACE::SIP_H_CALL_ID, Parse.FindData(SIP_NAMESPACE::SIP_H_CALL_ID));
				Ack.AddFields(SIP_NAMESPACE::SIP_H_CSEQ, Parse.FindData(SIP_NAMESPACE::SIP_H_CSEQ));

				Timestamp_type Time;
				SIP_NAMESPACE::_string_t Date = Time.DateString("-").c_str();
				Date += "T";
				Date += Time.MSTimeString(":").c_str();
				Ack.AddFields(SIP_NAMESPACE::SIP_H_DATE_UNIMP, Date);

				SIP_NAMESPACE::_string_t Auth = "Digest realm=\"";
				Auth += SERVER_DOMAIN;
				Auth += "\",algorithm=MD5,nonce=\"";
				Auth += DEVICE_NONCE;// _common_::base64_encode("").c_str();
				Auth += "\"";
				//Auth += ",qop=\"auth\"";
				Auth += ",qop=\"auth-int\"";
				Ack.AddFields(SIP_NAMESPACE::SIP_H_WWW_AUTHENTICATE, Auth);
				SIP_NAMESPACE::_string_big_t s = Ack.Generate(true);
				Send(hNet, pSrc, s.c_str(), s.size());
			}
			else
			{//验证信息
				 //response计算过程:
				 //1. HA1 = MD5(A1) = MD5(username:realm:password)
				 //2. a.如果 qop 值为"auth"或未指定: HA2 = MD5(A2) = MD5(method : uri)
				 //   b.如果 qop 值为"auth-int" : HA2=MD5(A2)=MD5(method:uri:MD5(entity-body))
				 //3. a.如果 qop 值为"auth"或"auth-int": response=MD5(HA1:nonce:nc:cnonce:qop:HA2)
				 //   b.如果 qop 未指定: response = MD5(HA1:nonce:HA2)

				SIP_NAMESPACE::_string_t sDevID = Parse.FindData(SIP_NAMESPACE::SIP_H_FROM, "sip:", "@");
				bool isOK = false;
				SIP_NAMESPACE::_string_t sQop = SIP_NAMESPACE::Sip_Parse::FindData(sAuth, "qop=", ",|EOF");
				if (sQop == "")
				{
					SIP_NAMESPACE::_string_t uri = SIP_NAMESPACE::Sip_Parse::FindData(sAuth, "uri=\"", "\"");
					SIP_NAMESPACE::_string_t response = SIP_NAMESPACE::Sip_Parse::FindData(sAuth, "response=\"", "\"");

					if ( strcmp(GetTokenNone(DEVICE_USERNAME, DEVICE_PASSWORD, SERVER_DOMAIN, uri.c_str(),
						DEVICE_NONCE).c_str(), response.c_str()) == 0)
					{//鉴权成功
						isOK = true;
					}
				}
				else if(sQop == "auth")
				{
					SIP_NAMESPACE::_string_t uri = SIP_NAMESPACE::Sip_Parse::FindData(sAuth, "uri=\"", "\"");
					SIP_NAMESPACE::_string_t response = SIP_NAMESPACE::Sip_Parse::FindData(sAuth, "response=\"", "\"");
					SIP_NAMESPACE::_string_t nc = SIP_NAMESPACE::Sip_Parse::FindData(sAuth, "nc=", ",|EOF");
					SIP_NAMESPACE::_string_t cnonce = SIP_NAMESPACE::Sip_Parse::FindData(sAuth, "cnonce=\"", "\"");

					if (strcmp(GetTokenAuth(DEVICE_USERNAME, DEVICE_PASSWORD, SERVER_DOMAIN, uri.c_str(),
						DEVICE_NONCE, nc.c_str(), cnonce.c_str(), sQop.c_str()).c_str(), response.c_str()) == 0)
					{//鉴权成功
						isOK = true;
					}
				}
				else if (sQop == "auth-int")
				{
					SIP_NAMESPACE::_string_t uri = SIP_NAMESPACE::Sip_Parse::FindData(sAuth, "uri=\"", "\"");
					SIP_NAMESPACE::_string_t response = SIP_NAMESPACE::Sip_Parse::FindData(sAuth, "response=\"", "\"");
					SIP_NAMESPACE::_string_t nc = SIP_NAMESPACE::Sip_Parse::FindData(sAuth, "nc=", ",|EOF");
					SIP_NAMESPACE::_string_t cnonce = SIP_NAMESPACE::Sip_Parse::FindData(sAuth, "cnonce=\"", "\"");
					if (strcmp(GetTokenAuthInt(DEVICE_USERNAME, DEVICE_PASSWORD, SERVER_DOMAIN, uri.c_str(),
						DEVICE_NONCE, nc.c_str(), cnonce.c_str(), sQop.c_str(), Buf_ptr->data).c_str(),
						response.c_str()) == 0)
					{//鉴权成功
						isOK = true;
					}
				}

				if (isOK)
				{//成功响应
					SIP_NAMESPACE::Sip_Response Ack;
					Ack.Init(SIP_NAMESPACE::SIP_SC_OK);
					Ack.AddVia(Parse.FindData(SIP_NAMESPACE::SIP_H_VIA), "");
					Ack.AddFields(SIP_NAMESPACE::SIP_H_FROM, Parse.FindData(SIP_NAMESPACE::SIP_H_FROM));
					Ack.AddToUri(Parse.FindData(SIP_NAMESPACE::SIP_H_TO), SIP_NAMESPACE::ISip_Proto::GenerateTag());
					Ack.AddFields(SIP_NAMESPACE::SIP_H_CALL_ID, Parse.FindData(SIP_NAMESPACE::SIP_H_CALL_ID));
					Ack.AddFields(SIP_NAMESPACE::SIP_H_CSEQ, Parse.FindData(SIP_NAMESPACE::SIP_H_CSEQ));
					SIP_NAMESPACE::_string_big_t s = Ack.Generate(true);
					Send(hNet, pSrc, s.c_str(), s.size());
					
					SIP_NAMESPACE::UriDetails_INFO SrcHost;
					SrcHost.Coding = SERVER_ID;
					SrcHost.Host.Host = SERVER_IP;
					SrcHost.Host.Port = SERVER_P0RT;

					SIP_NAMESPACE::UriDetails_INFO DstHost = SIP_NAMESPACE::ISip_Proto::ConvertFromUri(Parse.GetFromUri().Domain);
					SIP_NAMESPACE::Via_Hdr_INFO Hdr = Parse.GetFirstVia();
					DstHost.Host.Host = Hdr.Sent_by.Host;
					DstHost.Host.Port = Hdr.Sent_by.Port;

					/*DstHost.Host = Parse.FindData(SIP_NAMESPACE::SIP_H_FROM, "@", ":|>");

					DstHost.Port = SIP_NAMESPACE::_string_t::StringToInt(Parse.FindData(SIP_NAMESPACE::SIP_H_FROM, DstHost.Host + ":", ">")) ;
					if (DstHost.Port == 0)
					{
						DstHost.Port = 5060;
					}*/
					
					//获取设备信息
					g_SrcHost = SrcHost;
					g_DstHost = DstHost;
					g_sDeviceID = sDevID;

					GetCatalogInfo(hNet, pSrc, sDevID.c_str(), "1", SrcHost, DstHost);
				}
			}
		}
		else if (sMethod == SIP_NAMESPACE::sip_method_names[SIP_NAMESPACE::SIP_MESSAGE_METHOD])
		{
			SIP_NAMESPACE::_string_t sContent = Parse.GetContent();
			_apps_proto_::_xml_::CComandType	Type;
			if (Type.Parse(sContent.c_str()) == 1)
			{
				SIP_NAMESPACE::_string_t sCmdType = Type.GetCommandType();
				if (sCmdType == "Keepalive")
				{//心跳
					KeepaliveResponse(hNet, pSrc, Parse);
				}
				else if (sCmdType == "Catalog")
				{//设备目录
					SIP_NAMESPACE::Sip_Response Ack;
					Ack.Init(SIP_NAMESPACE::SIP_SC_OK);

					_sip_v20_::Via_Hdr_INFO Hdr = Parse.GetFirstVia();
					if (strstr(Parse.FindData(SIP_NAMESPACE::SIP_H_VIA).c_str(), "rport"))
					{//rport存在，需填充端口和接收端口

						Hdr.Rport = GetPortFromAddr(*pSrc);
						Ack.AddVia(Hdr, GetIPv4FromAddr(*pSrc).c_str());
					}
					else
					{
						Ack.AddVia(Parse.FindData(SIP_NAMESPACE::SIP_H_VIA), "");
					}
					Ack.AddFields(SIP_NAMESPACE::SIP_H_FROM, Parse.FindData(SIP_NAMESPACE::SIP_H_FROM));
					Ack.AddToUri(Parse.FindData(SIP_NAMESPACE::SIP_H_TO), SIP_NAMESPACE::ISip_Proto::GenerateTag());
					Ack.AddFields(SIP_NAMESPACE::SIP_H_CALL_ID, Parse.FindData(SIP_NAMESPACE::SIP_H_CALL_ID));
					Ack.AddFields(SIP_NAMESPACE::SIP_H_CSEQ, Parse.FindData(SIP_NAMESPACE::SIP_H_CSEQ));
					SIP_NAMESPACE::_string_big_t s = Ack.Generate(true);
					Send(hNet, pSrc, s.c_str(), s.size());
					
					_apps_proto_::_xml_::_gb28181_rsp_::Catalog Content;
					if (Content.Load(Type))
					{
						_apps_proto_::_xml_::_gb28181_rsp_::Catalog::CResponse::LisDeviceList::CItem Item;
						for (_apps_proto_::_xml_::_gb28181_rsp_::Catalog::CResponse::LisDeviceList::LList_type::iterator iter = Content.Response.DeviceList.DeviceList->begin();
							iter != Content.Response.DeviceList.DeviceList->end(); iter++)
						{
							Item = *iter;
						}

						//获取设备信息
						GetDeviceInfo(hNet, pSrc, g_sDeviceID.c_str(), "2", g_SrcHost, g_DstHost);
					}
				}
				else if (sCmdType == "DeviceInfo")
				{//响应设备信息
					SIP_NAMESPACE::Sip_Response Ack;
					Ack.Init(SIP_NAMESPACE::SIP_SC_OK);

					_sip_v20_::Via_Hdr_INFO Hdr = Parse.GetFirstVia();
					if (strstr(Parse.FindData(SIP_NAMESPACE::SIP_H_VIA).c_str(), "rport"))
					{//rport存在，需填充端口和接收端口
					
						Hdr.Rport = GetPortFromNETNODE(hNet);
						Ack.AddVia(Hdr, GetIPv4SFromNETNODE(hNet).c_str());
					}
					else
					{
						Ack.AddVia(Parse.FindData(SIP_NAMESPACE::SIP_H_VIA), "");
					}

					Ack.AddFields(SIP_NAMESPACE::SIP_H_FROM, Parse.FindData(SIP_NAMESPACE::SIP_H_FROM));
					Ack.AddToUri(Parse.FindData(SIP_NAMESPACE::SIP_H_TO), SIP_NAMESPACE::ISip_Proto::GenerateTag());
					Ack.AddFields(SIP_NAMESPACE::SIP_H_CALL_ID, Parse.FindData(SIP_NAMESPACE::SIP_H_CALL_ID));
					Ack.AddFields(SIP_NAMESPACE::SIP_H_CSEQ, Parse.FindData(SIP_NAMESPACE::SIP_H_CSEQ));
					SIP_NAMESPACE::_string_big_t s = Ack.Generate(true);
					Send(hNet, pSrc, s.c_str(), s.size());

					_apps_proto_::_xml_::_gb28181_rsp_::DeviceInfo Content;
					if (Content.Load(Type))
					{
						//获取设备状态
						GetDeviceStatus(hNet, pSrc, g_sDeviceID.c_str(), "3", g_SrcHost, g_DstHost);
					}
				}
				else if (sCmdType == "DeviceStatus")
				{//响应设备状态
					SIP_NAMESPACE::Sip_Response Ack;
					Ack.Init(SIP_NAMESPACE::SIP_SC_OK);
					Ack.AddVia(Parse.FindData(SIP_NAMESPACE::SIP_H_VIA), "");
					Ack.AddFields(SIP_NAMESPACE::SIP_H_FROM, Parse.FindData(SIP_NAMESPACE::SIP_H_FROM));
					Ack.AddToUri(Parse.FindData(SIP_NAMESPACE::SIP_H_TO), SIP_NAMESPACE::ISip_Proto::GenerateTag());
					Ack.AddFields(SIP_NAMESPACE::SIP_H_CALL_ID, Parse.FindData(SIP_NAMESPACE::SIP_H_CALL_ID));
					Ack.AddFields(SIP_NAMESPACE::SIP_H_CSEQ, Parse.FindData(SIP_NAMESPACE::SIP_H_CSEQ));
					SIP_NAMESPACE::_string_big_t s = Ack.Generate(true);
					Send(hNet, pSrc, s.c_str(), s.size());

					_apps_proto_::_xml_::_gb28181_rsp_::DeviceStatus Content;
					if (Content.Load(Type))
					{
						int i = 0;
					}
				}
			}
		}
	}
}

void Gb28181Test(void)
{
	LOG_Print_SetLog(libFrameWork_GB28181, 0);

	LOG_Print_Info(libFrameWork_GB28181, "Gb28181Test Start");

	GetUDPStreamInstance()->Init(1,1);
	NETHANDLE hNet = GetUDPStreamInstance()->Open(5060);
	if (hNet != -1)
	{
		GetUDPStreamInstance()->SetRecvHandle(hNet,
			function20_bind_Ex(HandleGB28181Recv));

		GetUDPStreamInstance()->SetDestroyHandle(hNet,
			function20_bind(HandleGB28181Destroy, hNet, 5060));
	}

	while (true)
	{
		Sleep(1000);
	};
}

int main(int argc, char* argv[])
{
	Gb28181Test();

	std::cin.get();
	return 0;
}