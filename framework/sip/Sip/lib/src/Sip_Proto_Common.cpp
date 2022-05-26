
#include <libProto_Sip/Sip_Proto_Common.h>

namespace	_sip_ {

	static _string_t status_phrase[710];

	class Init_status_phrase
	{
	public:
		Init_status_phrase(void)
		{
			_string_t default_reason_phrase = "Default status message";

			for (int i = 0; i< sizeof(status_phrase) / sizeof(status_phrase[0]); ++i)
				status_phrase[i] = default_reason_phrase;

			status_phrase[100] = "Trying";
			status_phrase[180] = "Ringing";
			status_phrase[181] = "Call Is Being Forwarded";
			status_phrase[182] = "Queued";
			status_phrase[183] = "Session Progress";
			status_phrase[200] = "OK";
			status_phrase[202] = "Accepted";

			status_phrase[300] = "Multiple Choices";
			status_phrase[301] = "Moved Permanently";
			status_phrase[302] = "Moved Temporarily";
			status_phrase[305] = "Use Proxy";
			status_phrase[380] = "Alternative Service";

			status_phrase[400] = "Bad Request";
			status_phrase[401] = "Unauthorized";
			status_phrase[402] = "Payment Required";
			status_phrase[403] = "Forbidden";
			status_phrase[404] = "Not Found";
			status_phrase[405] = "Method Not Allowed";
			status_phrase[406] = "Not Acceptable";
			status_phrase[407] = "Proxy Authentication Required";
			status_phrase[408] = "Request Timeout";
			status_phrase[410] = "Gone";
			status_phrase[413] = "Request Entity Too Large";
			status_phrase[414] = "Request URI Too Long";
			status_phrase[415] = "Unsupported Media Type";
			status_phrase[416] = "Unsupported URI Scheme";
			status_phrase[420] = "Bad Extension";
			status_phrase[421] = "Extension Required";
			status_phrase[422] = "Session Timer Too Small";
			status_phrase[423] = "Interval Too Brief";
			status_phrase[480] = "Temporarily Unavailable";
			status_phrase[481] = "Call/Transaction Does Not Exist";
			status_phrase[482] = "Loop Detected";
			status_phrase[483] = "Too Many Hops";
			status_phrase[484] = "Address Incompleted";
			status_phrase[485] = "Ambiguous";
			status_phrase[486] = "Busy Here";
			status_phrase[487] = "Request Terminated";
			status_phrase[488] = "Not Acceptable Here";
			status_phrase[489] = "Bad Event";
			status_phrase[490] = "Request Updated";
			status_phrase[491] = "Request Pending";
			status_phrase[493] = "Undecipherable";

			status_phrase[500] = "Internal Server Error";
			status_phrase[501] = "Not Implemented";
			status_phrase[502] = "Bad Gateway";
			status_phrase[503] = "Service Unavailable";
			status_phrase[504] = "Server Timeout";
			status_phrase[505] = "Version Not Supported";
			status_phrase[513] = "Message Too Large";
			status_phrase[580] = "Precondition Failure";

			status_phrase[600] = "Busy Everywhere";
			status_phrase[603] = "Decline";
			status_phrase[604] = "Does Not Exist Anywhere";
			status_phrase[606] = "Not Acceptable";

			status_phrase[701] = "No response from destination server";
			status_phrase[702] = "Unable to resolve destination server";
			status_phrase[703] = "Error sending message to destination server";
		}

		~Init_status_phrase(void)
		{

		}
	};

	_string_t Sip_Proto_Common::GetStatusCodeString(sip_status_code Status)
	{
		static Init_status_phrase	s_Init_status_phrase201709081120;
		return status_phrase[Status];
	}

	Sip_Proto_Common::Via_Hdr_sptr	Sip_Proto_Common::Alloc_Via(void)
	{
		static	MemPool_type	s_Pool(sizeof(Via_Hdr_INFO));
		return Via_Hdr_sptr(s_Pool.AllocObj<Via_Hdr_INFO>(),
			function20_bind_Ex(&MemPool_type::FreeObj<Via_Hdr_INFO>, &s_Pool));
	}

	Sip_Proto_Common::Sip_Proto_Common(void)
		: _CSeq(0)
		, _MaxForwards(0)
	{

	}

	Sip_Proto_Common::Sip_Proto_Common(const Sip_Proto_Common& t1)
	{
		Clone(t1);
	}

	Sip_Proto_Common::~Sip_Proto_Common(void)
	{
		Release();
	}

	Sip_Proto_Common& Sip_Proto_Common::operator=(const Sip_Proto_Common& t1)
	{
		Clone(t1);
		return *this;
	}

	void Sip_Proto_Common::Init(void)
	{

	}

	void Sip_Proto_Common::Release(void)
	{
		_ViaList.clear();
	}

	void Sip_Proto_Common::SetFromUri(const _string_t& Coding, const _string_t& Domain, const _string_t& Tag)
	{
		_From.Coding = Coding;
		_From.Domain = Domain;
		_From.Tag = Tag;
	}

	const Uri_INFO&	Sip_Proto_Common::GetFromUri(void) const
	{
		return _From;
	}

	void Sip_Proto_Common::SetToUri(const _string_t& Coding, const _string_t& Domain, const _string_t& Tag)
	{
		_To.Coding = Coding;
		_To.Domain = Domain;
		_To.Tag = Tag;
	}

	const Uri_INFO&	Sip_Proto_Common::GetToUri(void) const
	{
		return _To;
	}

	int Sip_Proto_Common::AddVia(const Via_Hdr_INFO& Via)
	{
		Via_Hdr_sptr sptr = Alloc_Via();
		if (!sptr)
			return -1;

		sptr->Branch = Via.Branch;
		sptr->Maddr = Via.Maddr;
		sptr->Recvd = Via.Recvd;
		sptr->Rport = Via.Rport;
		sptr->Sent_by = Via.Sent_by;
		sptr->isUdp = Via.isUdp;
		sptr->Ttl = Via.Ttl;
		_ViaList.push_front(sptr);
	}

	int Sip_Proto_Common::RemoveFirstVia(void)
	{
		if(_ViaList.size() > 0)
		{
			_ViaList.pop_front();
			return 1;
		}
			
		return -1;
	}

	Via_Hdr_INFO Sip_Proto_Common::GetFirstVia(void)
	{
		if(_ViaList.size() > 0)
			return *(_ViaList.front());

		return Via_Hdr_INFO();
	}

	void Sip_Proto_Common::SetCallID(const _string_t& CallID)
	{
		_CallID = CallID;
	}

	const _string_t& Sip_Proto_Common::GetCallID(void) const
	{
		return _CallID;
	}

	void Sip_Proto_Common::SetCSeq(long CSeq, const _string_t& MethodName)
	{
		_CSeq = CSeq;
		_MethodName = MethodName;
	}

	const long& Sip_Proto_Common::GetCSeq(void) const
	{
		return _CSeq;
	}

	const _string_t& Sip_Proto_Common::GetMethodName(void) const
	{
		return _MethodName;
	}

	void Sip_Proto_Common::SetMaxForwards(unsigned char MaxForwards)
	{
		_MaxForwards = MaxForwards;
	}

	const unsigned char& Sip_Proto_Common::GetMaxForwards(void) const
	{
		return _MaxForwards;
	}

	void Sip_Proto_Common::SetDate(const _string_t& Date)
	{
		_Date = Date;
	}

	const _string_t& Sip_Proto_Common::GetDate(void) const
	{
		return _Date;
	}

	void Sip_Proto_Common::SetSupported(const _string_t& Data)
	{
		_Supported = Data;
	}

	const _string_t& Sip_Proto_Common::GetSupported(void) const
	{
		return _Supported;
	}

	void Sip_Proto_Common::GenerateToList(Hdr_List* pList)
	{
		Hdr_INFO_sptr	sptr;
		Via_Hdr_sptr	Via_sptr;
		for (Via_Hdr_List::iterator iter = _ViaList.begin(); iter != _ViaList.end(); iter++)
		{
			Via_sptr = *iter;

			sptr = ISip_Proto::Alloc();
			sptr->Name = sip_hdr_names[SIP_H_VIA];
			sptr->Content = sip_version + "/";// "SIP/2.0/";
			if (Via_sptr->isUdp)
			{
				sptr->Content += "UDP ";
			}
			else
			{
				sptr->Content += "TCP ";
			}

			sptr->Content += Via_sptr->Sent_by.Host;
			if (Via_sptr->Sent_by.Port != -1)
				sptr->Content += ":" + _string_t::NumberToStr(Via_sptr->Sent_by.Port);
			sptr->Content += ";";
			sptr->Content += "branch=" + Via_sptr->Branch;
			if (Via_sptr->Maddr.size() > 0)
			{
				sptr->Content += ";";
				sptr->Content += "maddr=" + Via_sptr->Maddr;
			}

			if (Via_sptr->Rport != -1)
			{
				sptr->Content += ";";
				sptr->Content += "rport";
				if (Via_sptr->Rport > 0)
					sptr->Content += "=" + _string_t::NumberToStr(Via_sptr->Rport);
			}

			if (Via_sptr->Ttl != -1)
			{
				sptr->Content += ";";
				sptr->Content += "ttl";
				if (Via_sptr->Ttl > 0)
					sptr->Content += "=" + _string_t::NumberToStr(Via_sptr->Ttl);
			}

			if (Via_sptr->Recvd.size() > 0)
			{
				sptr->Content += ";";
				sptr->Content += "received=" + Via_sptr->Recvd;
			}
			pList->push_back(sptr);
		}

		sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_MAX_FORWARDS];
		sptr->Content = _string_t::NumberToStr(_MaxForwards);
		pList->push_back(sptr);

		sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_FROM];
		sptr->Content = _From.Coding;
		sptr->Content += "<sip:" + _From.Domain + ">";
		if (_From.Tag.size() > 0)
		{
			sptr->Content += ";tag=" + _From.Tag;
		}
		pList->push_back(sptr);

		sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_TO];
		sptr->Content = _To.Coding;
		sptr->Content += "<sip:" + _To.Domain + ">";
		if (_To.Tag.size() > 0)
		{
			sptr->Content += ";tag=" + _To.Tag;
		}
		pList->push_back(sptr);

		sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_CALL_ID];
		sptr->Content = _CallID;
		pList->push_back(sptr);

		sptr = ISip_Proto::Alloc();
		sptr->Name = sip_hdr_names[SIP_H_CSEQ];
		sptr->Content = _string_t::NumberToStr(_CSeq);
		sptr->Content += " " + _MethodName;
		pList->push_back(sptr);

		if (_Date != "")
		{
			sptr = ISip_Proto::Alloc();
			sptr->Name = sip_hdr_names[SIP_H_DATE_UNIMP];
			sptr->Content = _Date;
			pList->push_back(sptr);
		}

		if (_Supported != "")
		{
			sptr = ISip_Proto::Alloc();
			sptr->Name = sip_hdr_names[SIP_H_SUPPORTED];
			sptr->Content = _Supported;
			pList->push_back(sptr);
		}
	}

	bool Sip_Proto_Common::Parse(const char* c_szBuf, UInt32 uSize)
	{
		int iActiveLen = Sip_Parse::GetEndPos(c_szBuf, uSize);
		if (iActiveLen != -1)
		{
			const int iTailedLen = strlen("\r\n");
			const char* pCur = c_szBuf + Sip_Parse::GetHeadEndPos(c_szBuf, uSize);

			int iPos = -1;
			do
			{
				Hdr_INFO_sptr sptr = Sip_Parse::ParseLine(pCur, iActiveLen - (int)(pCur - c_szBuf), &iPos);
				ParseHdr(sptr);
				pCur += iPos;
				if (iPos == -1 || iActiveLen - ((int)(pCur - c_szBuf)) < 4)
					break;
			} while (true);
		}

		return iActiveLen;
	}

	bool Sip_Proto_Common::ParseHdr(const Hdr_INFO_sptr& sptr)
	{
		bool bRet = true;
		if (sptr)
		{
			if (sptr->Name == sip_hdr_names[SIP_H_VIA])
			{//via
				Via_Hdr_INFO Via;
				Via.Rport = -1;
				Via.Ttl = -1;
				Via.Sent_by.Host = Sip_Parse::FindData(sptr->Content, " ", ":");
				_string_t sPort = Sip_Parse::FindData(sptr->Content, ":", ";");
				Via.Sent_by.Port = _string_t::StringToInt(sPort);
				Via.Branch = Sip_Parse::FindData(sptr->Content, "branch=", ";|EOF");
				Via.isUdp = strstr(sptr->Content.c_str(), "/TCP") != NULL ? false : true;
				Via.Maddr = Sip_Parse::FindData(sptr->Content, "maddr=", ";|EOF");
				Via.Recvd = Sip_Parse::FindData(sptr->Content, "received=", ";|EOF");

				sPort = Sip_Parse::FindData(sptr->Content, ";rpor", ";|EOF");
				if (sPort != "")
				{
					if (sPort[0] == 't')
					{
						if (sPort[1] == '=')
						{
							Via.Rport = _string_t::StringToInt(&sPort[2]);
						}
						else
						{
							Via.Rport = 0;
						}
					}
				}

				_string_t sTtl = Sip_Parse::FindData(sptr->Content, ";tt", ";|EOF");
				if (sTtl != "")
				{
					if (sTtl[0] == 'l')
					{
						if (sTtl[1] == '=')
						{
							Via.Ttl = _string_t::StringToInt(&sTtl[2]);
						}
						else
						{
							Via.Ttl = 0;
						}
					}
				}

				AddVia(Via);
			}
			else if (sptr->Name == sip_hdr_names[SIP_H_MAX_FORWARDS])
			{//Max-Forwards
				SetMaxForwards(_string_t::StringToInt(sptr->Content));
			}
			else if (sptr->Name == sip_hdr_names[SIP_H_FROM])
			{//From
				_string_t Coding = Sip_Parse::FindData(sptr->Content, "SOF", "<");
				_string_t Domain = Sip_Parse::FindData(sptr->Content, "<sip:", ">");
				_string_t Tag = Sip_Parse::FindData(sptr->Content, "tag=", ";|EOF");
				SetFromUri(Coding, Domain, Tag);
			}
			else if (sptr->Name == sip_hdr_names[SIP_H_TO])
			{//TO
				_string_t Coding = Sip_Parse::FindData(sptr->Content, "SOF", "<");
				_string_t Domain = Sip_Parse::FindData(sptr->Content, "<sip:", ">");
				_string_t Tag = Sip_Parse::FindData(sptr->Content, "tag=", ";|EOF");
				SetToUri(Coding, Domain, Tag);
			}
			else if (sptr->Name == sip_hdr_names[SIP_H_CALL_ID])
			{//Call-ID
				SetCallID(sptr->Content);
			}
			else if (sptr->Name == sip_hdr_names[SIP_H_CSEQ])
			{//CSeq
				_string_t sSeq = Sip_Parse::FindData(sptr->Content, "SOF", " ");
				_string_t sMethonName = Sip_Parse::FindData(sptr->Content, " ", "EOF");
				SetCSeq(_string_t::StringToInt(sSeq), sMethonName);
			}
			else if (sptr->Name == sip_hdr_names[SIP_H_DATE_UNIMP])
			{//Call-ID
				SetDate(sptr->Content);
			}
			else
			{
				bRet = false;
			}
		}

		return bRet;
	}

	void Sip_Proto_Common::Clone(const Sip_Proto_Common& t1)
	{
		for (Via_Hdr_List::const_iterator iter = t1._ViaList.begin();
			iter != t1._ViaList.end(); iter ++)
		{
			_ViaList.push_back(*iter);
		}

		_From.Coding = t1._From.Coding;
		_From.Domain = t1._From.Domain;
		_From.Tag = t1._From.Tag;

		_To.Coding = t1._To.Coding;
		_To.Domain = t1._To.Coding;
		_To.Tag = t1._To.Tag;

		_CallID = t1._CallID;

		_CSeq = t1._CSeq;

		_MethodName = t1._MethodName;
		_MaxForwards = t1._MaxForwards;
	}

}



