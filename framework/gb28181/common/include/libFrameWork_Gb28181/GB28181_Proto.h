
#ifndef __GB28181_PROTO_20171226154204_3344199361_H__
#define __GB28181_PROTO_20171226154204_3344199361_H__

#include "gb28181_req.hpp"
#include "gb28181_rsp.hpp"

namespace	_gb28181_ {

	class IGB28181Session : public _enable_shared_from_this< IGB28181Session >
	{
	public:
		typedef SmartPTR<IGB28181Session>	self_sptr;
		typedef function20_handle<int (void)>	HFNClose;
		typedef function20_handle<void (const self_sptr&, void* pParam)>	HFNEvent;
		typedef _io_net_::StreamBuf_ptr	StreamBuf_ptr;

		enum enDEVICE_STATUS
		{
			EN_STATUS_ONLINE,
		};

	public:
		static _gb_string GetTokenNone(const char* c_szUserName, const char* c_szPassWord,
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

			_gb_string sResponse = _uuid_::DigestEngine::digestToHex(Ha3Md5.digest()).c_str();
			return sResponse;
		}

		static _gb_string GetTokenAuth(const char* c_szUserName, const char* c_szPassWord,
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

			_gb_string sResponse = _uuid_::DigestEngine::digestToHex(Ha3Md5.digest()).c_str();
			return sResponse;
		}

		static _gb_string GetTokenAuthInt(const char* c_szUserName, const char* c_szPassWord,
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

			_gb_string sResponse = _uuid_::DigestEngine::digestToHex(Ha3Md5.digest()).c_str();
			return sResponse;
		}

	public:
		IGB28181Session(void)
			: _uSeq(0)
		{}

		virtual ~IGB28181Session(void)
		{}

		virtual void Release(void) = 0;

		virtual int Close(void) = 0;

		virtual	int Send(const char* c_szData, UInt32 uDataSize) = 0;

		virtual void HandleDestroy(const self_sptr& Owner) = 0;

		virtual int Invite(UInt16 uPort, const SIP_NAMESPACE::Sip_SDP::sdp_media* pMedia, UInt16 uCount) = 0;

		int GetCatalogInfo(void)
		{
			SIP_NAMESPACE::Sip_Request_Message	Req;
			Req.Init(_LocalHost, _DstHost, 20, true);
			Req.SetVia(_LocalHost.Host, SIP_NAMESPACE::ISip_Proto::GenerateTime());

			SIP_NAMESPACE::_string_t sFrom = _LocalHost.Coding;
			sFrom += "@";
			sFrom += _LocalHost.Host.Host;
			if (_LocalHost.Host.Port != 0)
			{
				sFrom += ":" + SIP_NAMESPACE::_string_t::NumberToStr(_LocalHost.Host.Port);
			}

			SIP_NAMESPACE::_string_t sTo = _sDeviceID;
			sTo += "@";
			sTo += _DstHost.Host.Host;
			if (_DstHost.Host.Port != 0)
			{
				sTo += ":" + SIP_NAMESPACE::_string_t::NumberToStr(_DstHost.Host.Port);
			}

			Req.SetFromUri("", sFrom, SIP_NAMESPACE::ISip_Proto::GenerateTag());
			Req.SetToUri("", sTo, "");
			Req.SetCallID(SIP_NAMESPACE::ISip_Proto::GenerateCallID());
			Timestamp_type Time;
			SIP_NAMESPACE::_string_t Date = Time.DateString("-").c_str();
			Date += "T";
			Date += Time.MSTimeString(":").c_str();
			Req.SetDate(Date);

			_gb28181_::_request_::Catalog Content;
			Content.Query.DeviceID = _sDeviceID;
			Content.Query.SN = SIP_NAMESPACE::_string_t::NumberToStr(GetSeq());
			Req.SetContent(Content.Save().c_str(), "Application/MANSCDP+xml");

			SIP_NAMESPACE::_string_big_t s = Req.Generate();
			return Send(s.c_str(), s.size());
		}

		int GetDeviceInfo(void)
		{
			SIP_NAMESPACE::Sip_Request_Message	Req;
			Req.Init(_LocalHost, _DstHost, 21, true);
			Req.SetVia(_LocalHost.Host, SIP_NAMESPACE::ISip_Proto::GenerateTime());

			SIP_NAMESPACE::_string_t sFrom = _LocalHost.Coding;
			sFrom += "@";
			sFrom += _LocalHost.Host.Host;
			if (_LocalHost.Host.Port != 0)
			{
				sFrom += ":" + SIP_NAMESPACE::_string_t::NumberToStr(_LocalHost.Host.Port);
			}

			SIP_NAMESPACE::_string_t sTo = _sDeviceID;
			sTo += "@";
			sTo += _DstHost.Host.Host;
			if (_DstHost.Host.Port != 0)
			{
				sTo += ":" + SIP_NAMESPACE::_string_t::NumberToStr(_DstHost.Host.Port);
			}

			Req.SetFromUri("", sFrom, SIP_NAMESPACE::ISip_Proto::GenerateTag());
			Req.SetToUri("", sTo, "");
			Req.SetCallID(SIP_NAMESPACE::ISip_Proto::GenerateCallID());
			Timestamp_type Time;
			SIP_NAMESPACE::_string_t Date = Time.DateString("-").c_str();
			Date += "T";
			Date += Time.MSTimeString(":").c_str();
			Req.SetDate(Date);

			_gb28181_::_request_::DeviceInfo Content;
			Content.Query.DeviceID = _sDeviceID;
			Content.Query.SN = SIP_NAMESPACE::_string_t::NumberToStr(GetSeq());
			Req.SetContent(Content.Save().c_str(), "Application/MANSCDP+xml");

			SIP_NAMESPACE::_string_big_t s = Req.Generate();
			return Send(s.c_str(), s.size());
		}

		int GetDeviceStatus(void)
		{
			SIP_NAMESPACE::Sip_Request_Message	Req;
			Req.Init(_LocalHost, _DstHost, 22, true);
			Req.SetVia(_LocalHost.Host, SIP_NAMESPACE::ISip_Proto::GenerateTime());

			SIP_NAMESPACE::_string_t sFrom = _LocalHost.Coding;
			sFrom += "@";
			sFrom += _LocalHost.Host.Host;
			if (_LocalHost.Host.Port != 0)
			{
				sFrom += ":" + SIP_NAMESPACE::_string_t::NumberToStr(_LocalHost.Host.Port);
			}

			SIP_NAMESPACE::_string_t sTo = _sDeviceID;
			sTo += "@";
			sTo += _DstHost.Host.Host;
			if (_DstHost.Host.Port != 0)
			{
				sTo += ":" + SIP_NAMESPACE::_string_t::NumberToStr(_DstHost.Host.Port);
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

			_gb28181_::_request_::DeviceStatus Content;
			Content.Query.DeviceID = _sDeviceID;
			Content.Query.SN = SIP_NAMESPACE::_string_t::NumberToStr(GetSeq());
			Req.SetContent(Content.Save().c_str(), "Application/MANSCDP+xml");

			SIP_NAMESPACE::_string_big_t s = Req.Generate();
			return Send(s.c_str(), s.size());
		}

		int KeepaliveResponse(SIP_NAMESPACE::Sip_Parse_Request& Req)
		{
			SIP_NAMESPACE::Sip_Response Ack;
			Ack.Init(SIP_NAMESPACE::SIP_SC_OK);
			Ack.AddVia(Req.FindData(SIP_NAMESPACE::SIP_H_VIA), "");
			Ack.AddFields(SIP_NAMESPACE::SIP_H_FROM, Req.FindData(SIP_NAMESPACE::SIP_H_FROM));
			Ack.AddToUri(Req.FindData(SIP_NAMESPACE::SIP_H_TO), SIP_NAMESPACE::ISip_Proto::GenerateTag());
			Ack.AddFields(SIP_NAMESPACE::SIP_H_CALL_ID, Req.FindData(SIP_NAMESPACE::SIP_H_CALL_ID));
			Ack.AddFields(SIP_NAMESPACE::SIP_H_CSEQ, Req.FindData(SIP_NAMESPACE::SIP_H_CSEQ));
			SIP_NAMESPACE::_string_big_t s = Ack.Generate(true);
			return Send(s.c_str(), s.size());
		}

		UInt32 GetSeq(void)
		{
			return ++_uSeq;
		}

	protected:
		_gb_string _sDeviceID;
		_gb_string _sVideoID;
		UInt32	_uSeq;
		SIP_NAMESPACE::UriDetails_INFO _LocalHost;
		SIP_NAMESPACE::UriDetails_INFO _DstHost;
	};

	class GB28181SessionTcp : public IGB28181Session
	{
	public:
		typedef	GB28181SessionTcp	self_type;
		typedef	IGB28181Session::self_sptr	self_sptr;

		typedef function20_handle<Int32(const char*, UInt16)> HFNSend;
		typedef _session_::_tcp_::Stream_HRecv		Stream_HRecv;
		typedef _session_::_tcp_::Stream_HDestroy	Stream_HDestroy;
		typedef function20_handle<int(const Stream_HRecv&)> HFNSetRecv;
		typedef function20_handle<int(const Stream_HDestroy&)> HFNSetDestroy;

		static self_sptr	Alloc(void)
		{
			static MemPool_type	s_Pool(sizeof(self_type));
			return self_sptr(s_Pool.AllocObj<self_type>(),
				function20_bind(&MemPool_type::FreeMemObj<self_type>, &s_Pool, _function_::_1));
		}

		static self_sptr ParseRegister(const HFNSend& hSend, const HFNClose& hClose,
			const HFNEvent& hEvent, const HFNSetRecv& hSetRecv, const HFNSetDestroy& hSetDestroy,
			const SIP_NAMESPACE::UriDetails_INFO& LocalHost,
			const char* Realm, const char* Nonce,
			const char* UserName, const char* Password,
			const char* c_szData, UInt32 uSize)
		{
			self_sptr	Session_sptr;
			SIP_NAMESPACE::Sip_Parse_Request Parse;
			if (Parse.Parse(c_szData, uSize) != -1)
			{
				SIP_NAMESPACE::_string_t sMethod = Parse.GetMethodName();
				if (sMethod == SIP_NAMESPACE::sip_method_names[SIP_NAMESPACE::SIP_REGISTER_METHOD])
				{
					if (Parse.FindData(SIP_NAMESPACE::SIP_H_EXPIRES) == "0")
					{//注销
						hClose();
						return self_sptr();
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
						Auth += Realm;
						Auth += "\",algorithm=MD5,nonce=\"";
						Auth += Nonce;// _common_::base64_encode("").c_str();
						Auth += "\"";
						//Auth += ",qop=\"auth\"";
						Auth += ",qop=\"auth-int\"";
						Ack.AddFields(SIP_NAMESPACE::SIP_H_WWW_AUTHENTICATE, Auth);
						SIP_NAMESPACE::_string_big_t s = Ack.Generate(true);

						//发送数据
						if (hSend)
							hSend(s.c_str(), s.size());
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

							if (strcmp(GetTokenNone(UserName, Password, Realm, uri.c_str(),
								Nonce).c_str(), response.c_str()) == 0)
							{//鉴权成功
								isOK = true;
							}
						}
						else if (sQop == "auth")
						{
							SIP_NAMESPACE::_string_t uri = SIP_NAMESPACE::Sip_Parse::FindData(sAuth, "uri=\"", "\"");
							SIP_NAMESPACE::_string_t response = SIP_NAMESPACE::Sip_Parse::FindData(sAuth, "response=\"", "\"");
							SIP_NAMESPACE::_string_t nc = SIP_NAMESPACE::Sip_Parse::FindData(sAuth, "nc=", ",|EOF");
							SIP_NAMESPACE::_string_t cnonce = SIP_NAMESPACE::Sip_Parse::FindData(sAuth, "cnonce=\"", "\"");

							if (strcmp(GetTokenAuth(UserName, Password, Realm, uri.c_str(),
								Nonce, nc.c_str(), cnonce.c_str(), sQop.c_str()).c_str(), response.c_str()) == 0)
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
							if (strcmp(GetTokenAuthInt(UserName, Password, Realm, uri.c_str(),
								Nonce, nc.c_str(), cnonce.c_str(), sQop.c_str(), c_szData).c_str(),
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
							hSend(s.c_str(), s.size());

							SIP_NAMESPACE::UriDetails_INFO DstHost = SIP_NAMESPACE::ISip_Proto::ConvertFromUri(Parse.GetFromUri().Domain);
							SIP_NAMESPACE::Via_Hdr_INFO Hdr = Parse.GetFirstVia();
							DstHost.Host.Host = Hdr.Sent_by.Host;
							DstHost.Host.Port = Hdr.Sent_by.Port;
							
							Session_sptr = self_type::Alloc();
							if (((self_type*)Session_sptr.get())->Init() == 1)
							{
								if (((self_type*)Session_sptr.get())->Open(hClose,
									hSend, hEvent, hSetRecv, hSetDestroy, sDevID, LocalHost, DstHost) == -1)
								{
									LOG_Print_Error(libFrameWork_GB28181,
										"GB28181SessionTcp::ParseRegister() Open(%s) Failed!", sDevID.c_str());
									Session_sptr.reset();
								}
							}
						}
						else
						{
							hClose();
						}
					}
				}
			}

			return Session_sptr;
		}

	public:
		GB28181SessionTcp(void)
			: _isOpen(true)
		{

		}

		~GB28181SessionTcp(void)
		{
			Release();
		}

		int Init(void)
		{
			return 1;
		}

		void Release(void)
		{
			Close();
		}

		int Open(const HFNClose& hClose, const HFNSend& hSend,
			const HFNEvent& hEvent, const HFNSetRecv& hSetRecv,
			const HFNSetDestroy& hSetDestroy,
			const _gb_string& sDeviceID, SIP_NAMESPACE::UriDetails_INFO LocalHost,
			SIP_NAMESPACE::UriDetails_INFO DstHost)
		{
			_hSend = hSend;
			_hClose = hClose;
			_hEvent = hEvent;
			_sDeviceID = sDeviceID;
			_LocalHost = LocalHost;
			_DstHost = DstHost;
			if (hSetRecv)
			{
				hSetRecv( function20_bind(&self_type::HandleRecv, this,
					shared_from_this(), _function_::_1, _function_::_2, _function_::_3) );
			}

			if (hSetDestroy)
			{
				return hSetDestroy( function20_bind(&self_type::HandleDestroy, this, shared_from_this()) );
			}

			return 1;
		}

		int Close(void)
		{
			if (_hClose)
				return _hClose();

			return -1;
		}

		int Send(const char* c_szData, UInt32 uDataSize)
		{
			int iRet = -1;
			if (_hSend)
			{
				iRet = _hSend(c_szData, uDataSize);
				LOG_Print_Info(libFrameWork_GB28181, "GB28181SessionTcp::Send(%d,%s) Data:%s",
					uDataSize, iRet == 1 ? "Success" : "Failed", c_szData);
			}

			return iRet;
		}

		int Invite(UInt16 uPort, const SIP_NAMESPACE::Sip_SDP::sdp_media* pMedia, UInt16 uCount)
		{
			//实时语音广播请求
			SIP_NAMESPACE::Sip_SDP::sdp_session	sdp_ses;
			sdp_ses.proto_version = 0;
			sdp_ses.origin.user = _LocalHost.Coding;
			sdp_ses.origin.id = 0;
			sdp_ses.origin.version = 0;
			sdp_ses.origin.net_type = "IN";
			sdp_ses.origin.addr_type = "IP4";
			sdp_ses.origin.addr = _LocalHost.Host.Host;
			sdp_ses.name = "Play";

			sdp_ses.session_info = SIP_NAMESPACE::Sip_SDP::Alloc_session_info();
			sdp_ses.session_info->author = "test";
			sdp_ses.session_info->copyright = "namkoo";

			sdp_ses.conn = SIP_NAMESPACE::Sip_SDP::Alloc_conn();
			sdp_ses.conn->net_type = "IN";
			sdp_ses.conn->addr_type = "IP4";
			sdp_ses.conn->addr = _LocalHost.Host.Host;

			sdp_ses.bandw_count = 0;

			sdp_ses.time.start = 0;
			sdp_ses.time.stop = 0;
			sdp_ses.attr_count = 0;

			sdp_ses.media_count = uCount;
			for (UInt16 i = 0; i < uCount; i ++)
			{
				sdp_ses.media[i] = SIP_NAMESPACE::Sip_SDP::Alloc_media();

				sdp_ses.media[i]->desc.media = pMedia[i].desc.media;
				sdp_ses.media[i]->desc.port = pMedia[i].desc.port;
				sdp_ses.media[i]->desc.transport = pMedia[i].desc.transport;

				sdp_ses.media[i]->desc.fmt_count = pMedia[i].desc.fmt_count;
				for(UInt16 j = 0; j < sdp_ses.media[i]->desc.fmt_count; j ++)
					sdp_ses.media[i]->desc.fmt[j] = pMedia[i].desc.fmt[j];

				sdp_ses.media[i]->attr_count = pMedia[i].attr_count;
				for (UInt16 k = 0; k < sdp_ses.media[i]->attr_count; k++)
				{
					sdp_ses.media[i]->attr[k] = SIP_NAMESPACE::Sip_SDP::Alloc_attr();
					sdp_ses.media[i]->desc.fmt[k] = pMedia[i].desc.fmt[k];
					sdp_ses.media[i]->attr[k]->name = pMedia[i].attr[k]->name;
					sdp_ses.media[i]->attr[k]->value = pMedia[i].attr[k]->value;
				}
			}

			SIP_NAMESPACE::Sip_SDP Sdp;
			Sdp.Copy(sdp_ses);
			SIP_NAMESPACE::_string_t swl = Sdp.toString();

			SIP_NAMESPACE::UriDetails_INFO Info;
			Info.Coding = _sVideoID;
			Info.Host = _DstHost.Host;

			SIP_NAMESPACE::Sip_Request_Invite Invite;
			Invite.Init(_LocalHost, Info, GetSeq(), false);
			Invite.SetVia(_LocalHost.Host, SIP_NAMESPACE::ISip_Proto::GenerateTime());
			Invite.SetFromUri(_LocalHost.Coding, _LocalHost.Coding + "@" + _LocalHost.Host.Host + ":" + SIP_NAMESPACE::_string_t::NumberToStr(_LocalHost.Host.Port),
				SIP_NAMESPACE::ISip_Proto::GenerateTag());
			Invite.SetToUri(Info.Coding, Info.Coding + "@" + Info.Host.Host + ":" + SIP_NAMESPACE::_string_t::NumberToStr(Info.Host.Port), "");
			Invite.SetCallID(SIP_NAMESPACE::ISip_Proto::GenerateCallID());
			Invite.SetSdp(Sdp);

			SIP_NAMESPACE::_string_big_t str_big = Invite.Generate();
			return Send(str_big.c_str(), str_big.size());
		}

	protected:

		void HandleDestroy(const self_sptr& Owner)
		{
			/*if (_hEvent)
			_hEvent();*/
		}

		void HandleRecv(const self_sptr& Owner, NETHANDLE hNet, const StreamBuf_ptr& Buf_ptr, long lRemain)
		{
			SIP_NAMESPACE::Sip_Parse_Request Parse;
			if (Parse.Parse(Buf_ptr->data, Buf_ptr->payload) != -1)
			{
				SIP_NAMESPACE::_string_t sMethod = Parse.GetMethodName();
				if (sMethod == SIP_NAMESPACE::sip_method_names[SIP_NAMESPACE::SIP_REGISTER_METHOD])
				{
					if (Parse.FindData(SIP_NAMESPACE::SIP_H_EXPIRES) == "0")
					{//注销
						Close();
					}
				}
				else if (sMethod == SIP_NAMESPACE::sip_method_names[SIP_NAMESPACE::SIP_MESSAGE_METHOD])
				{
					SIP_NAMESPACE::_string_t sContent = Parse.GetContent();
					_gb28181_::CComandType	Type;
					if (Type.Parse(sContent.c_str()) == 1)
					{
						SIP_NAMESPACE::_string_t sCmdType = Type.GetCommandType();
						if (sCmdType == "Keepalive")
						{//心跳
							KeepaliveResponse(Parse);
						}
						else if (sCmdType == "Catalog")
						{//设备目录
							SIP_NAMESPACE::Sip_Response Ack;
							Ack.Init(SIP_NAMESPACE::SIP_SC_OK);

							SIP_NAMESPACE::Via_Hdr_INFO Hdr = Parse.GetFirstVia();
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
							Send(s.c_str(), s.size());

							_gb28181_::_response_::Catalog Content;
							if (Content.Load(Type))
							{
								_gb28181_::_response_::Catalog::CResponse::LisDeviceList::CItem Item;
								for (_gb28181_::_response_::Catalog::CResponse::LisDeviceList::LList_type::iterator iter = Content.Response.DeviceList.DeviceList->begin();
									iter != Content.Response.DeviceList.DeviceList->end(); iter++)
								{
									Item = *iter;
									_sVideoID = Item.DeviceID;
								}

								//获取设备信息
								GetDeviceInfo();
							}
						}
						else if (sCmdType == "DeviceInfo")
						{//响应设备信息
							SIP_NAMESPACE::Sip_Response Ack;
							Ack.Init(SIP_NAMESPACE::SIP_SC_OK);

							SIP_NAMESPACE::Via_Hdr_INFO Hdr = Parse.GetFirstVia();
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
							Send(s.c_str(), s.size());

							_gb28181_::_response_::DeviceInfo Content;
							if (Content.Load(Type))
							{
								//获取设备状态
								GetDeviceStatus();
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
							Send(s.c_str(), s.size());

							_gb28181_::_response_::DeviceStatus Content;
							if (Content.Load(Type))
							{
								SIP_NAMESPACE::_string_t sVideoType = "96";//"98";

								//test
								SIP_NAMESPACE::Sip_SDP::sdp_media media[1];
								media[0].desc.media = "video";
								media[0].desc.port = 61000;
								media[0].desc.transport = "RTP/AVP/TCP";
								media[0].desc.fmt_count = 1;
								media[0].desc.fmt[0] = sVideoType;

								media[0].attr_count = 2;
								media[0].attr[0] = SIP_NAMESPACE::Sip_SDP::Alloc_attr();
								media[0].attr[0]->name = "rtpmap";
								//media[0].attr[0]->value = sVideoType + " H264/90000";
								media[0].attr[0]->value = sVideoType + " PS/90000";

								media[0].attr[1] = SIP_NAMESPACE::Sip_SDP::Alloc_attr();
								media[0].attr[1]->name = "recvonly";
								media[0].attr[1]->value = "";

								Invite(61000, media, 1);
								//test
							}
						}
					}
				}
			}
		}

	private:
		bool	_isOpen;
		HFNSend	_hSend;
		HFNClose _hClose;
		HFNEvent _hEvent;
	};


	class GB28181SessionUdp : public IGB28181Session
	{
	public:
		typedef GB28181SessionUdp			self_type;
		typedef	IGB28181Session::self_sptr	self_sptr;

		typedef function20_handle<Int32(struct sockaddr_in*, const char*, UInt16, UInt16)> HFNSend;
		typedef _session_::_udp_::Stream_HRecv		Stream_HRecv;
		typedef _session_::_udp_::Stream_HDestroy	Stream_HDestroy;
		typedef function20_handle<int(const Stream_HRecv&)> HFNSetRecv;
		typedef function20_handle<int(const Stream_HDestroy&)> HFNSetDestroy;
		
		static self_sptr	Alloc(void)
		{
			static MemPool_type s_Pool(sizeof(self_type));
			return self_sptr(s_Pool.AllocObj<self_type>(),
				function20_bind(&MemPool_type::FreeMemObj<self_type>, &s_Pool, _function_::_1));
		}

		static self_sptr ParseRegister(const HFNSend& hSend, const HFNClose& hClose,
			const HFNEvent& hEvent, const HFNSetRecv& hSetRecv, const HFNSetDestroy& hSetDestroy,
			struct sockaddr_in* pSrc,
			const SIP_NAMESPACE::UriDetails_INFO& LocalHost,
			const char* Realm, const char* Nonce,
			const char* UserName, const char* Password,
			const char* c_szData, UInt32 uSize)
		{
			self_sptr	Session_sptr;
			SIP_NAMESPACE::Sip_Parse_Request Parse;
			if (Parse.Parse(c_szData, uSize) != -1)
			{
				SIP_NAMESPACE::_string_t sMethod = Parse.GetMethodName();
				if (sMethod == SIP_NAMESPACE::sip_method_names[SIP_NAMESPACE::SIP_REGISTER_METHOD])
				{
					if (Parse.FindData(SIP_NAMESPACE::SIP_H_EXPIRES) == "0")
					{//注销
						hClose();
						return self_sptr();
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
						Auth += Realm;
						Auth += "\",algorithm=MD5,nonce=\"";
						Auth += Nonce;// _common_::base64_encode("").c_str();
						Auth += "\"";
						//Auth += ",qop=\"auth\"";
						Auth += ",qop=\"auth-int\"";
						Ack.AddFields(SIP_NAMESPACE::SIP_H_WWW_AUTHENTICATE, Auth);
						SIP_NAMESPACE::_string_big_t s = Ack.Generate(true);

						//发送数据
						if (hSend)
							hSend(pSrc, s.c_str(), s.size(), 0);
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

							if (strcmp(GetTokenNone(UserName, Password, Realm, uri.c_str(),
								Nonce).c_str(), response.c_str()) == 0)
							{//鉴权成功
								isOK = true;
							}
						}
						else if (sQop == "auth")
						{
							SIP_NAMESPACE::_string_t uri = SIP_NAMESPACE::Sip_Parse::FindData(sAuth, "uri=\"", "\"");
							SIP_NAMESPACE::_string_t response = SIP_NAMESPACE::Sip_Parse::FindData(sAuth, "response=\"", "\"");
							SIP_NAMESPACE::_string_t nc = SIP_NAMESPACE::Sip_Parse::FindData(sAuth, "nc=", ",|EOF");
							SIP_NAMESPACE::_string_t cnonce = SIP_NAMESPACE::Sip_Parse::FindData(sAuth, "cnonce=\"", "\"");

							if (strcmp(GetTokenAuth(UserName, Password, Realm, uri.c_str(),
								Nonce, nc.c_str(), cnonce.c_str(), sQop.c_str()).c_str(), response.c_str()) == 0)
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
							if (strcmp(GetTokenAuthInt(UserName, Password, Realm, uri.c_str(),
								Nonce, nc.c_str(), cnonce.c_str(), sQop.c_str(), c_szData).c_str(),
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
							hSend(pSrc, s.c_str(), s.size(), 0);

							SIP_NAMESPACE::UriDetails_INFO DstHost = SIP_NAMESPACE::ISip_Proto::ConvertFromUri(Parse.GetFromUri().Domain);
							SIP_NAMESPACE::Via_Hdr_INFO Hdr = Parse.GetFirstVia();
							DstHost.Host.Host = Hdr.Sent_by.Host;
							DstHost.Host.Port = Hdr.Sent_by.Port;

							Session_sptr = self_type::Alloc();
							if (((self_type*)Session_sptr.get())->Init() == 1)
							{
								if (((self_type*)Session_sptr.get())->Open(hClose,
									hSend, hEvent, hSetRecv, hSetDestroy, pSrc, sDevID, LocalHost, DstHost) == -1)
								{
									LOG_Print_Error(libFrameWork_GB28181,
										"GB28181SessionTcp::ParseRegister() Open(%s) Failed!", sDevID.c_str());
									Session_sptr.reset();
								}
							}

							//获取设备信息
							//GetCatalogInfo(hNet, pSrc, sDevID.c_str(), "1", SrcHost, DstHost);
						}
						else
						{
							hClose();
						}
					}
				}

			}

			return Session_sptr;
		}

	public:
		GB28181SessionUdp(void)
		{
			memset(&_DestAddr, 0, sizeof(struct sockaddr_in));
		}

		~GB28181SessionUdp(void)
		{
			Release();
		}

		int Init(void)
		{
			return 1;
		}

		void Release(void)
		{
			Close();
		}

		int Open(const HFNClose& hClose, const HFNSend& hSend,
			const HFNEvent& hEvent, const HFNSetRecv& hSetRecv,
			const HFNSetDestroy& hSetDestroy,
			struct sockaddr_in* pSrc,
			const _gb_string& sDeviceID, SIP_NAMESPACE::UriDetails_INFO LocalHost,
			SIP_NAMESPACE::UriDetails_INFO DstHost)
		{
			memcpy(&_DestAddr, pSrc, sizeof(struct sockaddr_in));
			_hSend = hSend;
			_hClose = hClose;
			_hEvent = hEvent;
			_sDeviceID = sDeviceID;
			_LocalHost = LocalHost;
			_DstHost = DstHost;
			if (hSetRecv)
			{
				hSetRecv(function20_bind(&self_type::HandleRecv, this,
					shared_from_this(), _function_::_1, _function_::_2, _function_::_3));
			}

			if (hSetDestroy)
			{
				return hSetDestroy(function20_bind(&self_type::HandleDestroy, this, shared_from_this()));
			}

			return 1;
		}

		int Close(void)
		{
			if (_hClose)
				return _hClose();

			return -1;
		}

		int Send(const char* c_szData, UInt32 uDataSize)
		{
			int iRet = -1;
			if (_hSend)
			{
				iRet = _hSend(&_DestAddr, c_szData, uDataSize, 0);
				LOG_Print_Info(libFrameWork_GB28181, "GB28181SessionTcp::Send(%d,%s) Data:%s",
					uDataSize, iRet == 1 ? "Success" : "Failed", c_szData);
			}

			return iRet;
		}

		int Invite(UInt16 uPort, const SIP_NAMESPACE::Sip_SDP::sdp_media* pMedia, UInt16 uCount)
		{
			//实时语音广播请求
			SIP_NAMESPACE::Sip_SDP::sdp_session	sdp_ses;
			sdp_ses.proto_version = 0;
			sdp_ses.origin.user = _LocalHost.Coding;
			sdp_ses.origin.id = 0;
			sdp_ses.origin.version = 0;
			sdp_ses.origin.net_type = "IN";
			sdp_ses.origin.addr_type = "IP4";
			sdp_ses.origin.addr = _LocalHost.Host.Host;
			sdp_ses.name = "Play";

			sdp_ses.session_info = SIP_NAMESPACE::Sip_SDP::Alloc_session_info();
			sdp_ses.session_info->author = "test";
			sdp_ses.session_info->copyright = "namkoo";

			sdp_ses.conn = SIP_NAMESPACE::Sip_SDP::Alloc_conn();
			sdp_ses.conn->net_type = "IN";
			sdp_ses.conn->addr_type = "IP4";
			sdp_ses.conn->addr = _LocalHost.Host.Host;

			sdp_ses.bandw_count = 0;

			sdp_ses.time.start = 0;
			sdp_ses.time.stop = 0;
			sdp_ses.attr_count = 0;

			sdp_ses.media_count = uCount;
			for (UInt16 i = 0; i < uCount; i++)
			{
				sdp_ses.media[i] = SIP_NAMESPACE::Sip_SDP::Alloc_media();

				sdp_ses.media[i]->desc.media = pMedia[i].desc.media;
				sdp_ses.media[i]->desc.port = pMedia[i].desc.port;
				sdp_ses.media[i]->desc.transport = pMedia[i].desc.transport;

				sdp_ses.media[i]->desc.fmt_count = pMedia[i].desc.fmt_count;
				for (UInt16 j = 0; j < sdp_ses.media[i]->desc.fmt_count; j++)
					sdp_ses.media[i]->desc.fmt[j] = pMedia[i].desc.fmt[j];

				sdp_ses.media[i]->attr_count = pMedia[i].attr_count;
				for (UInt16 k = 0; k < sdp_ses.media[i]->attr_count; k++)
				{
					sdp_ses.media[i]->attr[k] = SIP_NAMESPACE::Sip_SDP::Alloc_attr();
					sdp_ses.media[i]->desc.fmt[k] = pMedia[i].desc.fmt[k];
					sdp_ses.media[i]->attr[k]->name = pMedia[i].attr[k]->name;
					sdp_ses.media[i]->attr[k]->value = pMedia[i].attr[k]->value;
				}
			}

			SIP_NAMESPACE::Sip_SDP Sdp;
			Sdp.Copy(sdp_ses);
			SIP_NAMESPACE::_string_t swl = Sdp.toString();

			SIP_NAMESPACE::Sip_Request_Invite Invite;
			Invite.Init(_LocalHost, _DstHost, GetSeq());
			Invite.SetVia(_LocalHost.Host, SIP_NAMESPACE::ISip_Proto::GenerateTime());
			Invite.SetFromUri(_LocalHost.Coding, _LocalHost.Coding + "@" + _LocalHost.Host.Host + ":" + SIP_NAMESPACE::_string_t::NumberToStr(_LocalHost.Host.Port),
				SIP_NAMESPACE::ISip_Proto::GenerateTag());
			Invite.SetToUri(_DstHost.Coding, _DstHost.Coding + "@" + _DstHost.Host.Host + ":" + SIP_NAMESPACE::_string_t::NumberToStr(_DstHost.Host.Port), "");
			Invite.SetCallID(SIP_NAMESPACE::ISip_Proto::GenerateCallID());
			Invite.SetSdp(Sdp);

			SIP_NAMESPACE::_string_big_t str_big = Invite.Generate();
			return Send(str_big.c_str(), str_big.size());
		}

	protected:

		void HandleDestroy(const self_sptr& Owner)
		{

		}

		void HandleRecv(const self_sptr& Owner, NETHANDLE hNet,
			struct sockaddr_in* pSrc, const StreamBuf_ptr& Buf_ptr)
		{
			SIP_NAMESPACE::Sip_Parse_Request Parse;
			if (Parse.Parse(Buf_ptr->data, Buf_ptr->payload) != -1)
			{
				SIP_NAMESPACE::_string_t sMethod = Parse.GetMethodName();
				if (sMethod == SIP_NAMESPACE::sip_method_names[SIP_NAMESPACE::SIP_REGISTER_METHOD])
				{
					if (Parse.FindData(SIP_NAMESPACE::SIP_H_EXPIRES) == "0")
					{//注销
						Close();
					}
				}
				else if (sMethod == SIP_NAMESPACE::sip_method_names[SIP_NAMESPACE::SIP_MESSAGE_METHOD])
				{
					SIP_NAMESPACE::_string_t sContent = Parse.GetContent();
					_gb28181_::CComandType	Type;
					if (Type.Parse(sContent.c_str()) == 1)
					{
						SIP_NAMESPACE::_string_t sCmdType = Type.GetCommandType();
						if (sCmdType == "Keepalive")
						{//心跳
							KeepaliveResponse(Parse);
						}
						else if (sCmdType == "Catalog")
						{//设备目录
							SIP_NAMESPACE::Sip_Response Ack;
							Ack.Init(SIP_NAMESPACE::SIP_SC_OK);

							SIP_NAMESPACE::Via_Hdr_INFO Hdr = Parse.GetFirstVia();
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
							Send(s.c_str(), s.size());

							_gb28181_::_response_::Catalog Content;
							if (Content.Load(Type))
							{
								_gb28181_::_response_::Catalog::CResponse::LisDeviceList::CItem Item;
								for (_gb28181_::_response_::Catalog::CResponse::LisDeviceList::LList_type::iterator iter = Content.Response.DeviceList.DeviceList->begin();
									iter != Content.Response.DeviceList.DeviceList->end(); iter++)
								{
									Item = *iter;
									_sVideoID = Item.DeviceID;
								}

								//获取设备信息
								GetDeviceInfo();
							}
						}
						else if (sCmdType == "DeviceInfo")
						{//响应设备信息
							SIP_NAMESPACE::Sip_Response Ack;
							Ack.Init(SIP_NAMESPACE::SIP_SC_OK);

							SIP_NAMESPACE::Via_Hdr_INFO Hdr = Parse.GetFirstVia();
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
							Send(s.c_str(), s.size());

							_gb28181_::_response_::DeviceInfo Content;
							if (Content.Load(Type))
							{
								//获取设备状态
								GetDeviceStatus();
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
							Send(s.c_str(), s.size());

							_gb28181_::_response_::DeviceStatus Content;
							if (Content.Load(Type))
							{
								SIP_NAMESPACE::_string_t sVideoType = "96";//"98";

								//test
								SIP_NAMESPACE::Sip_SDP::sdp_media media[1];
								media[0].desc.media = "video";
								media[0].desc.port = 61000;
								media[0].desc.transport = "RTP/AVP";
								media[0].desc.fmt_count = 1;
								media[0].desc.fmt[0] = sVideoType;

								media[0].attr_count = 2;
								media[0].attr[0] = SIP_NAMESPACE::Sip_SDP::Alloc_attr();
								media[0].attr[0]->name = "rtpmap";
								//media[0].attr[0]->value = sVideoType + " H264/90000";
								media[0].attr[0]->value = sVideoType + " PS/90000";

								media[0].attr[1] = SIP_NAMESPACE::Sip_SDP::Alloc_attr();
								media[0].attr[1]->name = "recvonly";
								media[0].attr[1]->value = "";

								Invite(61000, media, 1);
								//test
							}
						}
					}
				}
			}
		}

	private:
		bool	_isOpen;
		struct sockaddr_in _DestAddr;
		HFNSend	_hSend;
		HFNClose _hClose;
		HFNEvent _hEvent;
	};

}

#endif
