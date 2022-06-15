
#include <libProto_Sip/Sip_Request.h>

namespace	_sip_ {

	/******************************Sip_Request******************************/

	Sip_Request::Sip_Request(void)
	{	

	}

	Sip_Request::~Sip_Request(void)
	{

	}

	void Sip_Request::SetMethodType(const sip_method_e& MethodType)
	{
		_MethodType = MethodType;
	}

	const sip_method_e Sip_Request::GetMethodType(void) const
	{
		return _MethodType;
	}

	const _string_t Sip_Request::GetMethodName(void) const
	{
		return sip_method_names[_MethodType];
	}

	void Sip_Request::SetVia(const Host_Port_INFO& Local, const _string_t& Branch)
	{
		Via_Hdr_INFO Via;
		Via.Branch = rfc3261_branch;
		Via.Branch += Branch;
		Via.isUdp = _isUdp;
		Via.Maddr = "";
		Via.Rport = 0;
		Via.Ttl = -1;
		Via.Sent_by.Host = Local.Host;
		Via.Sent_by.Port = Local.Port;
		Via.Recvd = "";
		_Common.AddVia(Via);
	}

	void Sip_Request::SetFromUri(const _string_t& Coding, const _string_t& Domain, const _string_t& Tag)
	{
		_Common.SetFromUri(Coding, Domain, Tag);
	}

	void Sip_Request::SetToUri(const _string_t& Coding, const _string_t& Domain, const _string_t& Tag)
	{
		_Common.SetToUri(Coding, Domain, Tag);
	}

	void Sip_Request::SetCallID(const _string_t& CallID)
	{
		_Common.SetCallID(CallID);
	}

	void Sip_Request::SetDate(const _string_t& Date)
	{
		_Common.SetDate(Date);
	}

	void Sip_Request::SetSupported(const _string_t& Data)
	{
		_Common.SetSupported(Data);
	}

	void Sip_Request::SetContent(const _string_t& Content, const _string_t& ContentType)
	{
		ISip_Proto::SetContent(Content, ContentType);
	}

	/******************************Sip_Request_Register******************************/

	Sip_Request_Register::Sip_Request_Register(void)
	{
		_isUdp = true;
		SetMethodType(SIP_REGISTER_METHOD);
	}

	Sip_Request_Register::~Sip_Request_Register(void)
	{
		Release();
	}

	int Sip_Request_Register::Init(const Host_Port_INFO& DstHost, UInt32 uExpires,
		long CSeq, const _string_t& Rinstance, bool isUdp)
	{
		_uExpires = uExpires;
		_isUdp = isUdp;
		_Common.Release();
		_Common.SetCSeq(CSeq, GetMethodName());
		_Rinstance = Rinstance;
		_DstHost.Host = DstHost.Host;
		_DstHost.Port = DstHost.Port;
		return 1;
	}

	int Sip_Request_Register::Release(void)
	{
		return 1;
	}

	void Sip_Request_Register::SetToken( const _string_t& UserName,
		const _string_t& Realm,
		const _string_t& Nonce,
		const _string_t& Response,
		const _string_t& Algorithm )
	{
		_UserName = UserName;
		_Realm = Realm;
		_Nonce = Nonce;
		_Algorithm = Algorithm;
		_Token = Response;
	}

	_string_big_t Sip_Request_Register::Generate( void )
	{
		_string_t	Str = GetMethodName() + " sip:";
		Str += _DstHost.Host.c_str();
		if (_DstHost.Port > 0)
		{
			Str += ":";
			Str += _string_t::NumberToStr(_DstHost.Port).c_str();
		}

		Str += " ";
		Str += sip_version.c_str();

		clear();

		_Common.SetMaxForwards(70);
		_Common.GenerateToList(&_HdrList);

		const Uri_INFO& Uri = _Common.GetFromUri();
		Via_Hdr_INFO Via = _Common.GetFirstVia();

		Hdr_INFO_sptr	sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_CONTACT];

		sptr->Content = "<sip:" + Uri.Coding + "@" + Via.Sent_by.Host
			+ ":" + _string_t::NumberToStr(Via.Sent_by.Port);

		if (_Rinstance != "")
		{
			sptr->Content += ";rinstance=" + _Rinstance;
		}

		sptr->Content += ">";

		push_back(sptr);

		sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_EXPIRES];
		sptr->Content = _string_t::NumberToStr(_uExpires);
		push_back(sptr);

		sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_USER_AGENT_UNIMP];
		sptr->Content = sip_proto_version;// "SIP Proto Release V1.00 Date 201706082116";
		push_back(sptr);

		/*sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_ALLOW];
		sptr->Content = "INVITE, ACK, CANCEL, OPTIONS, BYE, REFER, NOTIFY, MESSAGE, SUBSCRIBE, INFO";
		push_back(sptr);*/

		if (_Token != "")
		{
			sptr = ISip_Proto::Alloc();
			sptr->Name = sip_hdr_names[SIP_H_PROXY_AUTHENTICATE];
			sptr->Content = "Digest ";
			if (_UserName != "")
			{
				sptr->Content += "username=\"" + _UserName + "\"";
				sptr->Content += ", realm=\"" + _Realm + "\"";
				sptr->Content += ", nonce=\"" + _Nonce + "\"";
				sptr->Content += ", uri=\"sip:" + Uri.Coding + "@" + Uri.Domain + "\"";
				sptr->Content += ", response=\"" + _Token + "\"";
				sptr->Content += ", algorithm=\"" + _Algorithm + "\"";
			}

			push_back(sptr);
		}

		return GenerateString(Str);
	}



	/******************************Sip_Request_Invite******************************/

	Sip_Request_Invite::Sip_Request_Invite(void)
	{
		_isUdp = false;
		SetMethodType(SIP_INVITE_METHOD);
	}

	Sip_Request_Invite::~Sip_Request_Invite(void)
	{
		Release();
	}

	int Sip_Request_Invite::Init(const Host_Port_INFO& DstHost,
		long CSeq,
		bool isUdp)
	{
		_isUdp = isUdp;
		_Common.SetCSeq(CSeq, GetMethodName());
		_DstHost.Host = DstHost.Host;
		_DstHost.Port = DstHost.Port;
		_Sdp.Init();
		return 1;
	}

	int Sip_Request_Invite::Release(void)
	{
		return 1;
	}

	void Sip_Request_Invite::SetSdp(const Sip_SDP& Sdp)
	{
		_Sdp = Sdp;
	}

	void Sip_Request_Invite::SetSdp(const Sip_SDP::sdp_session& Sdp_Session)
	{
		_Sdp.Copy(Sdp_Session);
	}

	_string_t Sip_Request_Invite::GetSdpString(void)
	{
		return _Sdp.toString();
	}

	_string_big_t Sip_Request_Invite::Generate(void)
	{
		_string_t	Str = GetMethodName() + " sip:";
		Str += _DstHost.Host.c_str();
		if (_DstHost.Port > 0)
		{
			Str += ":";
			Str += _string_t::NumberToStr(_DstHost.Port).c_str();
		}

		Str += " ";
		Str += sip_version.c_str();

		clear();

		_Common.SetMaxForwards(70);
		_Common.GenerateToList(&_HdrList);

		const Uri_INFO& Uri = _Common.GetFromUri();
		Via_Hdr_INFO Via = _Common.GetFirstVia();

		Hdr_INFO_sptr	sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_CONTACT];
		sptr->Content = "<sip:" + Uri.Coding + "@" + Via.Sent_by.Host
			+ ":" + _string_t::NumberToStr(Via.Sent_by.Port) + ">";
		push_back(sptr);

		sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_USER_AGENT_UNIMP];
		sptr->Content = sip_proto_version;// "SIP Proto UA Release V1.00 Date 201706082116";
		push_back(sptr);

		/*sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_ALLOW];
		sptr->Content = "INVITE, ACK, CANCEL, OPTIONS, BYE, REFER, NOTIFY, MESSAGE, SUBSCRIBE, INFO";
		push_back(sptr);*/

		SetContent(GetSdpString(), "application/sdp");

		return GenerateString(Str);
	}



	/******************************Sip_Request_Ack******************************/

	Sip_Request_Ack::Sip_Request_Ack(void)
	{
		_isUdp = true;
		SetMethodType(SIP_ACK_METHOD);
	}

	Sip_Request_Ack::~Sip_Request_Ack(void)
	{
		Release();
	}

	int Sip_Request_Ack::Init(const Host_Port_INFO& DstHost,
		long CSeq,
		bool isUdp)
	{
		_isUdp = isUdp;
		_Common.SetCSeq(CSeq, GetMethodName());
		_DstHost.Host = DstHost.Host;
		_DstHost.Port = DstHost.Port;
		return 1;
	}

	int Sip_Request_Ack::Release(void)
	{
		return 1;
	}

	_string_big_t Sip_Request_Ack::Generate(void)
	{
		_string_t	Str = GetMethodName() + " sip:";
		Str += _DstHost.Host.c_str();
		if (_DstHost.Port > 0)
		{
			Str += ":";
			Str += _string_t::NumberToStr(_DstHost.Port).c_str();
		}

		Str += " ";
		Str += sip_version.c_str();

		clear();

		_Common.SetMaxForwards(70);
		_Common.GenerateToList(&_HdrList);

		const Uri_INFO& Uri = _Common.GetFromUri();
		Via_Hdr_INFO Via = _Common.GetFirstVia();

		Hdr_INFO_sptr	sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_CONTACT];

		sptr->Content = "<sip:" + Uri.Coding + "@" + Via.Sent_by.Host
			+ ":" + _string_t::NumberToStr(Via.Sent_by.Port) + ">";
		push_back(sptr);

		sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_USER_AGENT_UNIMP];
		sptr->Content = sip_proto_version;// "SIP Proto UA Release V1.00 Date 201706082116";
		push_back(sptr);

		/*sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_ALLOW];
		sptr->Content = "INVITE, ACK, CANCEL, OPTIONS, BYE, REFER, NOTIFY, MESSAGE, SUBSCRIBE, INFO";
		push_back(sptr);*/

		return GenerateString(Str);
	}



	/******************************Sip_Request_Bye******************************/

	Sip_Request_Bye::Sip_Request_Bye(void)
	{
		_isUdp = true;
		SetMethodType(SIP_BYE_METHOD);
	}

	Sip_Request_Bye::~Sip_Request_Bye(void)
	{
		Release();
	}

	int Sip_Request_Bye::Init(const Host_Port_INFO& DstHost,
		long CSeq,
		bool isUdp)
	{
		_isUdp = isUdp;
		_Common.SetCSeq(CSeq, GetMethodName());
		_DstHost.Host = DstHost.Host;
		_DstHost.Port = DstHost.Port;
		return 1;
	}

	int Sip_Request_Bye::Release(void)
	{
		return 1;
	}

	_string_big_t Sip_Request_Bye::Generate(void)
	{
		_string_t	Str = GetMethodName() + " sip:";
		Str += _DstHost.Host.c_str();
		if (_DstHost.Port > 0)
		{
			Str += ":";
			Str += _string_t::NumberToStr(_DstHost.Port).c_str();
		}

		Str += " ";
		Str += sip_version.c_str();

		clear();

		_Common.SetMaxForwards(70);
		_Common.GenerateToList(&_HdrList);

		const Uri_INFO& Uri = _Common.GetFromUri();
		Via_Hdr_INFO Via = _Common.GetFirstVia();

		Hdr_INFO_sptr	sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_CONTACT];

		sptr->Content = "<sip:" + Uri.Coding + "@" + Via.Sent_by.Host
			+ ":" + _string_t::NumberToStr(Via.Sent_by.Port) + ">";
		push_back(sptr);

		sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_USER_AGENT_UNIMP];
		sptr->Content = sip_proto_version;// "SIP Proto UA Release V1.00 Date 201706082116";
		push_back(sptr);

		/*sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_ALLOW];
		sptr->Content = "INVITE, ACK, CANCEL, OPTIONS, BYE, REFER, NOTIFY, MESSAGE, SUBSCRIBE, INFO";
		push_back(sptr);*/

		return GenerateString(Str);
	}



	/******************************Sip_Request_Message******************************/

	Sip_Request_Message::Sip_Request_Message(void)
	{
		_isUdp = true;
		SetMethodType(SIP_MESSAGE_METHOD);
	}

	Sip_Request_Message::~Sip_Request_Message(void)
	{
		Release();
	}

	int Sip_Request_Message::Init(const Host_Port_INFO& DstHost,
		long CSeq,
		bool isUdp)
	{
		_isUdp = isUdp;
		_Common.Release();
		_Common.SetCSeq(CSeq, GetMethodName());
		_DstHost.Host = DstHost.Host;
		_DstHost.Port = DstHost.Port;
		return 1;
	}

	int Sip_Request_Message::Release(void)
	{
		return 1;
	}

	void Sip_Request_Message::SetToken(const _string_t& UserName,
		const _string_t& Realm,
		const _string_t& Nonce,
		const _string_t& Response,
		const _string_t& Algorithm)
	{
		_UserName = UserName;
		_Realm = Realm;
		_Nonce = Nonce;
		_Algorithm = Algorithm;
		_Token = Response;
	}

	_string_big_t Sip_Request_Message::Generate(void)
	{
		_string_t	Str = GetMethodName() + " sip:";
		Str += _DstHost.Host.c_str();
		if (_DstHost.Port > 0)
		{
			Str += ":";
			Str += _string_t::NumberToStr(_DstHost.Port).c_str();
		}

		Str += " ";
		Str += sip_version.c_str();

		clear();

		_Common.SetMaxForwards(70);
		_Common.GenerateToList(&_HdrList);

		const Uri_INFO& Uri = _Common.GetFromUri();
		Via_Hdr_INFO Via = _Common.GetFirstVia();

		Hdr_INFO_sptr	sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_CONTACT];

		_string_t Coding = _foundation_::ScanChar::FindValue(Uri.Domain.c_str(), "SOF", "@");

		sptr->Content = "<sip:" + Coding + "@" + Via.Sent_by.Host
			+ ":" + _string_t::NumberToStr(Via.Sent_by.Port) + ">";
		push_back(sptr);

		sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_USER_AGENT_UNIMP];
		sptr->Content = sip_proto_version;// "SIP Proto Release V1.00 Date 201706082116";
		push_back(sptr);

		/*sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_ALLOW];
		sptr->Content = "INVITE, ACK, CANCEL, OPTIONS, BYE, REFER, NOTIFY, MESSAGE, SUBSCRIBE, INFO";
		push_back(sptr);*/

		if (_Token != "")
		{
			sptr = ISip_Proto::Alloc();
			sptr->Name = sip_hdr_names[SIP_H_PROXY_AUTHENTICATE];
			sptr->Content = "Digest ";
			if (_UserName != "")
			{
				sptr->Content += "username=\"" + _UserName + "\"";
				sptr->Content += ", realm=\"" + _Realm + "\"";
				sptr->Content += ", nonce=\"" + _Nonce + "\"";
				sptr->Content += ", uri=\"sip:" + Uri.Coding + "@" + Uri.Domain + "\"";
				sptr->Content += ", response=\"" + _Token + "\"";
				sptr->Content += ", algorithm=\"" + _Algorithm + "\"";
			}

			push_back(sptr);
		}

		return GenerateString(Str);
	}



	/******************************Sip_Request_Options******************************/

	Sip_Request_Options::Sip_Request_Options(void)
	{
		_isUdp = true;
		SetMethodType(SIP_OPTIONS_METHOD);
	}

	Sip_Request_Options::~Sip_Request_Options(void)
	{
		Release();
	}

	int Sip_Request_Options::Init(const Host_Port_INFO& DstHost,
		long CSeq,
		bool isUdp)
	{
		_isUdp = isUdp;
		_Common.SetCSeq(CSeq, GetMethodName());
		_DstHost.Host = DstHost.Host;
		_DstHost.Port = DstHost.Port;
		return 1;
	}

	int Sip_Request_Options::Release(void)
	{
		return 1;
	}

	_string_big_t Sip_Request_Options::Generate(void)
	{
		_string_t	Str = GetMethodName() + " sip:";
		Str += _DstHost.Host.c_str();
		if (_DstHost.Port > 0)
		{
			Str += ":";
			Str += _string_t::NumberToStr(_DstHost.Port).c_str();
		}

		Str += " ";
		Str += sip_version.c_str();

		clear();

		_Common.SetMaxForwards(70);
		_Common.GenerateToList(&_HdrList);

		const Uri_INFO& Uri = _Common.GetFromUri();
		Via_Hdr_INFO Via = _Common.GetFirstVia();

		Hdr_INFO_sptr	sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_CONTACT];
		sptr->Content = "<sip:" + Uri.Coding + "@" + Via.Sent_by.Host
			+ ":" + _string_t::NumberToStr(Via.Sent_by.Port) + ">";
		push_back(sptr);

		sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_USER_AGENT_UNIMP];
		sptr->Content = sip_proto_version;// "SIP Proto Release V1.00 Date 201706082116";
		push_back(sptr);

		/*sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_ALLOW];
		sptr->Content = "INVITE, ACK, CANCEL, OPTIONS, BYE, REFER, NOTIFY, MESSAGE, SUBSCRIBE, INFO";
		push_back(sptr);*/

		return GenerateString(Str);
	}



	/******************************Sip_Request_Cancel******************************/

	Sip_Request_Cancel::Sip_Request_Cancel(void)
	{
		_isUdp = true;
		SetMethodType(SIP_CANCEL_METHOD);
	}

	Sip_Request_Cancel::~Sip_Request_Cancel(void)
	{
		Release();
	}

	int Sip_Request_Cancel::Init(const Host_Port_INFO& DstHost,
		long CSeq,
		bool isUdp)
	{
		_isUdp = isUdp;
		_Common.SetCSeq(CSeq, GetMethodName());
		_DstHost.Host = DstHost.Host;
		_DstHost.Port = DstHost.Port;
		return 1;
	}

	int Sip_Request_Cancel::Release(void)
	{
		return 1;
	}

	_string_big_t Sip_Request_Cancel::Generate(void)
	{
		_string_t	Str = GetMethodName() + " sip:";
		Str += _DstHost.Host.c_str();
		if (_DstHost.Port > 0)
		{
			Str += ":";
			Str += _string_t::NumberToStr(_DstHost.Port).c_str();
		}

		Str += " ";
		Str += sip_version.c_str();

		clear();

		_Common.SetMaxForwards(70);
		_Common.GenerateToList(&_HdrList);

		const Uri_INFO& Uri = _Common.GetFromUri();
		Via_Hdr_INFO Via = _Common.GetFirstVia();

		Hdr_INFO_sptr	sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_CONTACT];
		sptr->Content = "<sip:" + Uri.Coding + "@" + Via.Sent_by.Host
			+ ":" + _string_t::NumberToStr(Via.Sent_by.Port) + ">";
		push_back(sptr);

		sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_USER_AGENT_UNIMP];
		sptr->Content = sip_proto_version;// "SIP Proto Release V1.00 Date 201706082116";
		push_back(sptr);

		/*sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_ALLOW];
		sptr->Content = "INVITE, ACK, CANCEL, OPTIONS, BYE, REFER, NOTIFY, MESSAGE, SUBSCRIBE, INFO";
		push_back(sptr);*/

		return GenerateString(Str);
	}


	/******************************Sip_Request_INFO******************************/

	Sip_Request_INFO::Sip_Request_INFO(void)
	{
		_isUdp = true;
		SetMethodType(SIP_INFO_METHOD);
	}

	Sip_Request_INFO::~Sip_Request_INFO(void)
	{
		Release();
	}

	int Sip_Request_INFO::Init(const Host_Port_INFO& DstHost,
		long CSeq,
		bool isUdp)
	{
		_isUdp = isUdp;
		_Common.SetCSeq(CSeq, GetMethodName());
		_DstHost.Host = DstHost.Host;
		_DstHost.Port = DstHost.Port;
		return 1;
	}

	int Sip_Request_INFO::Release(void)
	{
		return 1;
	}

	_string_big_t Sip_Request_INFO::Generate(void)
	{
		_string_t	Str = GetMethodName() + " sip:";
		Str += _DstHost.Host.c_str();
		if (_DstHost.Port > 0)
		{
			Str += ":";
			Str += _string_t::NumberToStr(_DstHost.Port).c_str();
		}

		Str += " ";
		Str += sip_version.c_str();

		clear();

		_Common.SetMaxForwards(70);
		_Common.GenerateToList(&_HdrList);

		const Uri_INFO& Uri = _Common.GetFromUri();
		Via_Hdr_INFO Via = _Common.GetFirstVia();

		Hdr_INFO_sptr	sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_CONTACT];
		sptr->Content = "<sip:" + Uri.Coding + "@" + Via.Sent_by.Host
			+ ":" + _string_t::NumberToStr(Via.Sent_by.Port) + ">";
		push_back(sptr);

		sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_USER_AGENT_UNIMP];
		sptr->Content = sip_proto_version;// "SIP Proto Release V1.00 Date 201706082116";
		push_back(sptr);

		/*sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_ALLOW];
		sptr->Content = "INVITE, ACK, CANCEL, OPTIONS, BYE, REFER, NOTIFY, MESSAGE, SUBSCRIBE, INFO";
		push_back(sptr);*/

		return GenerateString(Str);
	}

}

