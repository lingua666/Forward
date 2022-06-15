
#include <libProto_Sip_V20/Sip_Response.h>

namespace	_sip_v20_ {

	Sip_Response::Sip_Response(void)
	{

	}

	int Sip_Response::Init(sip_status_code Status)
	{
		_Status = Status;
		_ContentType = "";
		_Content = "";
		_Common.Release();
		clear();
		return 1;
	}

	void Sip_Response::SetVia(const Host_Port_INFO& Local, const _string_t& Branch)
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

	void Sip_Response::SetFromUri(const _string_t& Coding, const _string_t& Domain, const _string_t& Tag)
	{
		_Common.SetFromUri(Coding, Domain, Tag);
	}

	void Sip_Response::SetToUri(const _string_t& Coding, const _string_t& Domain, const _string_t& Tag)
	{
		_Common.SetToUri(Coding, Domain, Tag);
	}

	void Sip_Response::SetCallID(const _string_t& CallID)
	{
		_Common.SetCallID(CallID);
	}

	void Sip_Response::SetContent(const _string_t& Content, const _string_t& ContentType)
	{
		ISip_Proto::SetContent(Content, ContentType);
	}

	int Sip_Response::SetVia(const Via_Hdr_INFO& Via)
	{
		return _Common.AddVia(Via);
	}

	void Sip_Response::AddVia(const Via_Hdr_INFO& Via, const _string_t& Recvd)
	{
		_string_t& Content = sip_version + "/";// "SIP/2.0/";
		if (Via.isUdp)
		{
			Content += "UDP ";
		}
		else
		{
			Content += "TCP ";
		}

		Content += Via.Sent_by.Host;
		if (Via.Sent_by.Port != -1)
			Content += ":" + _string_t::NumberToStr(Via.Sent_by.Port);
		Content += ";";
		Content += "branch=" + Via.Branch;
		if (Via.Maddr.size() > 0)
		{
			Content += ";";
			Content += "maddr=" + Via.Maddr;
		}

		if (Via.Rport != -1)
		{
			Content += ";";
			Content += "rport";
			if (Via.Rport > 0)
				Content += "=" + _string_t::NumberToStr(Via.Rport);
		}

		if (Via.Ttl != -1)
		{
			Content += ";";
			Content += "ttl";
			if (Via.Ttl > 0)
				Content += "=" + _string_t::NumberToStr(Via.Ttl);
		}

		AddVia(Content, Recvd);
	}

	void Sip_Response::AddVia(const _string_t& Content, const _string_t& Recvd)
	{
		if (Recvd != "")
		{
			ISip_Proto::AddFields(SIP_H_VIA, Content + ";received=" + Recvd);
			return;
		}

		ISip_Proto::AddFields(SIP_H_VIA, Content);
	}

	void Sip_Response::AddToUri(const _string_t& Content, const _string_t& Tag)
	{
		if (Tag != "")
		{
			ISip_Proto::AddFields(SIP_H_TO, Content + ";tag=" + Tag);
			return;
		}

		ISip_Proto::AddFields(SIP_H_TO, Content);
	}

	void Sip_Response::AddFields(sip_hdr_e Hdr_e, const _string_t& Content)
	{
		if(Content != "")
			ISip_Proto::AddFields(Hdr_e, Content);
	}

	_string_big_t	Sip_Response::Generate(bool isOnlyList)
	{
		_string_t Head = "SIP/2.0 ";
		Head += _string_t::NumberToStr(_Status) + " ";
		Head += Sip_Proto_Common::GetStatusCodeString(_Status);

		//clear();
		if(!isOnlyList)
			_Common.GenerateToList(&_HdrList);

		return GenerateString(Head);
	}




	Sip_Response_Ack::Sip_Response_Ack(void)
	{

	}

	int Sip_Response_Ack::Init(const Host_Port_INFO& DstHost, bool isUdp)
	{
		_DstHost.Host = DstHost.Host;
		_DstHost.Port = DstHost.Port;
		_isUdp = isUdp;
		_Content = "";
		_Common.Release();
		clear();
		return 1;
	}

	void Sip_Response_Ack::SetVia(const Host_Port_INFO& Local, const _string_t& Branch)
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

	void Sip_Response_Ack::SetFromUri(const _string_t& Coding, const _string_t& Domain, const _string_t& Tag)
	{
		_Common.SetFromUri(Coding, Domain, Tag);
	}

	void Sip_Response_Ack::SetToUri(const _string_t& Coding, const _string_t& Domain, const _string_t& Tag)
	{
		_Common.SetToUri(Coding, Domain, Tag);
	}

	void Sip_Response_Ack::SetCallID(const _string_t& CallID)
	{
		_Common.SetCallID(CallID);
	}

	void Sip_Response_Ack::SetContent(const _string_t& Content, const _string_t& ContentType)
	{
		ISip_Proto::SetContent(Content, ContentType);
	}

	void Sip_Response_Ack::SetCSeq(long CSeq)
	{
		ISip_Proto::AddFields(SIP_H_CSEQ, _string_t::NumberToStr(CSeq) + " " + sip_method_names[SIP_ACK_METHOD]);
	}

	int Sip_Response_Ack::SetVia(const Via_Hdr_INFO& Via)
	{
		return _Common.AddVia(Via);
	}

	void Sip_Response_Ack::AddVia(const _string_t& Content, const _string_t& Recvd)
	{
		if (Recvd != "")
		{
			ISip_Proto::AddFields(SIP_H_VIA, Content + ";received=" + Recvd);
			return;
		}

		ISip_Proto::AddFields(SIP_H_VIA, Content);
	}

	void Sip_Response_Ack::AddToUri(const _string_t& Content, const _string_t& Tag)
	{
		if (Tag != "")
		{
			ISip_Proto::AddFields(SIP_H_TO, Content + ";tag=" + Tag);
			return;
		}

		ISip_Proto::AddFields(SIP_H_TO, Content);
	}

	void Sip_Response_Ack::AddFields(sip_hdr_e Hdr_e, const _string_t& Content)
	{
		if (Content != "")
			ISip_Proto::AddFields(Hdr_e, Content);
	}

	_string_big_t	Sip_Response_Ack::Generate(bool isOnlyList)
	{
		_string_t Head =  sip_method_names[SIP_ACK_METHOD] +  " sip: ";
		Head += _DstHost.Host.c_str();
		if (_DstHost.Port > 0)
		{
			Head += ":";
			Head += _string_t::NumberToStr(_DstHost.Port).c_str();
		}

		Head += " ";
		Head += sip_version.c_str();

		//clear();
		if (!isOnlyList)
			_Common.GenerateToList(&_HdrList);

		return GenerateString(Head);
	}

}

