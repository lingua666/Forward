
#include <libProto_Sip/ISip_Proto.h>

namespace	_sip_ {

	ISip_Proto::Hdr_INFO_sptr	ISip_Proto::Alloc(void)
	{
		static	MemPool_type	s_Pool(sizeof(Hdr_INFO));
		return Hdr_INFO_sptr(s_Pool.AllocObj<Hdr_INFO>(),
			function20_bind_Ex(&MemPool_type::FreeObj<Hdr_INFO>, &s_Pool));
	}


	_string_t  ISip_Proto::GenerateCallID(void)
	{
		return _uuid_::UUIDGenerator().UUIDToString().c_str();
	}

	_string_t  ISip_Proto::GenerateBranch(void)
	{
		_string_t Branch = rfc3261_branch;
		Branch.append(1, 80);	//参考pjsip
		Branch.append(1, 106);	//参考pjsip
		Branch += _uuid_::UUIDGenerator().UUIDToString().c_str();
		return Branch;
	}

	_string_t  ISip_Proto::GenerateTime(void)
	{
		return _string_t::NumberToStr(Timestamp_type().unixTime());
	}

	_string_t  ISip_Proto::GenerateTag(void)
	{
		return GenerateCallID();
	}

	bool ISip_Proto::IsIP(const _string_t& Content)
	{
		return strstr(Content.c_str(), ".") != NULL ? true : false;
	}


	ISip_Proto::ISip_Proto(void)
	{

	}

	ISip_Proto::~ISip_Proto(void)
	{
		clear();
	}

	void	ISip_Proto::push_front(const Hdr_INFO_sptr& sptr)
	{
		_HdrList.push_front(sptr);
	}

	void	ISip_Proto::pop_front(void)
	{
		_HdrList.pop_front();
	}

	void	ISip_Proto::push_back(const Hdr_INFO_sptr& sptr)
	{
		_HdrList.push_back(sptr);
	}

	void ISip_Proto::clear(void)
	{
		_HdrList.clear();
	}

	void ISip_Proto::AddFields(sip_hdr_e Hdr_e, const _string_t& Content)
	{
		Hdr_INFO_sptr sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[Hdr_e];
		sptr->Content = Content;
		push_back(sptr);
	}

	void ISip_Proto::AddFields(const _string_t& Name, const _string_t& Content)
	{
		Hdr_INFO_sptr sptr = ISip_Proto::Alloc();
		sptr->Name = Name;
		sptr->Content = Content;
		push_back(sptr);
	}

	void ISip_Proto::SetFromUri(const _string_t& Coding, const _string_t& Domain, const _string_t& Tag)
	{
		Hdr_INFO_sptr	sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_FROM];
		sptr->Content = Coding;
		sptr->Content += "<sip:" + Coding;
		sptr->Content += "@" + Domain + ">";
		if (Tag.size() > 0)
		{
			sptr->Content += ";tag=" + Tag;
		}

		push_back(sptr);
	}

	void ISip_Proto::SetToUri(const _string_t& Coding, const _string_t& Domain, const _string_t& Tag)
	{
		Hdr_INFO_sptr	sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_TO];
		sptr->Content = Coding;
		sptr->Content += "<sip:" + Coding;
		sptr->Content += "@" + Domain + ">";
		if (Tag.size() > 0)
		{
			sptr->Content += ";tag=" + Tag;
		}

		push_back(sptr);
	}

	void ISip_Proto::AddVia(const Via_Hdr_INFO& Via)
	{
		Hdr_INFO_sptr	sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_VIA];
		sptr->Content = sip_version + "/";// "SIP/2.0/";
		if (Via.isUdp)
		{
			sptr->Content += "UDP ";
		}
		else
		{
			sptr->Content += "TCP ";
		}

		sptr->Content += Via.Sent_by.Host;
		if (Via.Sent_by.Port != -1)
			sptr->Content += ":" + _string_t::NumberToStr(Via.Sent_by.Port);
		sptr->Content += ";";
		sptr->Content += "branch=" + Via.Branch;
		if (Via.Maddr.size() > 0)
		{
			sptr->Content += ";";
			sptr->Content += "maddr=" + Via.Maddr;
		}

		if (Via.Rport != -1)
		{
			sptr->Content += ";";
			sptr->Content += "rport";
			if (Via.Rport > 0)
				sptr->Content += "=" + _string_t::NumberToStr(Via.Rport);
		}

		if (Via.Ttl != -1)
		{
			sptr->Content += ";";
			sptr->Content += "ttl";
			if (Via.Ttl > 0)
				sptr->Content += "=" + _string_t::NumberToStr(Via.Ttl);
		}

		if (Via.Recvd.size() > 0)
		{
			sptr->Content += ";";
			sptr->Content += "received=" + Via.Recvd;
		}
		push_front(sptr);
	}

	void ISip_Proto::SetCallID(const _string_t& CallID)
	{
		Hdr_INFO_sptr sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_CALL_ID];
		sptr->Content = CallID;
		push_back(sptr);
	}

	void ISip_Proto::SetCSeq(long CSeq)
	{
		Hdr_INFO_sptr sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_CSEQ];
		sptr->Content = _string_t::NumberToStr(CSeq);
		push_back(sptr);
	}

	void ISip_Proto::SetMaxForwards(unsigned char MaxForwards)
	{
		Hdr_INFO_sptr sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_MAX_FORWARDS];
		sptr->Content = _string_t::NumberToStr(MaxForwards);
		push_back(sptr);
	}

	void	ISip_Proto::SetContent(const _string_t& Content, const _string_t& ContentType)
	{
		_ContentType = ContentType;
		_Content = Content + "\r\n";
	}

	//Head 参数内容不包含换行符
	_string_big_t	ISip_Proto::GenerateString(const _string_t& Head)
	{
		Hdr_INFO_sptr sptr;
		_string_big_t	str = Head.c_str();
		str += "\r\n";

		for (Hdr_List::iterator iter = _HdrList.begin(); iter != _HdrList.end(); iter++)
		{
			sptr = *iter;
			str += sptr->Name.c_str();
			str += ": ";
			str += sptr->Content.c_str();
			str += "\r\n";
		}

		if (_ContentType != "")
		{
			str += sip_hdr_names[SIP_H_CONTENT_TYPE].c_str();
			str += ": ";
			str += _ContentType.c_str();
			str += "\r\n";
		}

		str += "Content-Length: ";
		if (_Content.size() > 0)
		{
			str += _string_t::NumberToStr(_Content.size()).c_str();	
			str += "\r\n\r\n";
			str.append(_Content.c_str(), _Content.size());
		}
		else
		{
			str += "0";
			str += "\r\n\r\n";
		}

		return str;
	}
}
