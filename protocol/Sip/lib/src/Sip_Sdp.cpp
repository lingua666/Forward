
#include <libProto_Sip/Sip_Sdp.h>

namespace	_sip_ {

	MemPool_type*	Sip_SDP::GetMemPoolInstance(void)
	{
		static MemPool_type	s_Pool( sizeof(sdp_media) );
		return &s_Pool;
	}

	Sip_SDP::sdp_session_info_sptr Sip_SDP::Alloc_session_info(void)
	{
		return sdp_session_info_sptr(GetMemPoolInstance()->AllocObj<sdp_session_info>(),
			function20_bind_Ex(&MemPool_type::FreeObj<sdp_session_info>, GetMemPoolInstance()));
	}

	Sip_SDP::sdp_conn_sptr Sip_SDP::Alloc_conn(void)
	{
		return sdp_conn_sptr(GetMemPoolInstance()->AllocObj<sdp_conn>(),
			function20_bind_Ex(&MemPool_type::FreeObj<sdp_conn>, GetMemPoolInstance()));
	}

	Sip_SDP::sdp_bandw_sptr Sip_SDP::Alloc_bandw(void)
	{
		sdp_bandw* p = GetMemPoolInstance()->AllocObj<sdp_bandw>();
		p->value = 0;
		return sdp_bandw_sptr(p,
			function20_bind_Ex(&MemPool_type::FreeObj<sdp_bandw>, GetMemPoolInstance()));
	}

	Sip_SDP::sdp_attr_sptr Sip_SDP::Alloc_attr(void)
	{
		return sdp_attr_sptr(GetMemPoolInstance()->AllocObj<sdp_attr>(),
			function20_bind_Ex(&MemPool_type::FreeObj<sdp_attr>, GetMemPoolInstance()));
	}

	Sip_SDP::sdp_media_sptr Sip_SDP::Alloc_media(void)
	{
		sdp_media* p = GetMemPoolInstance()->AllocObj<sdp_media>();
		p->attr_count = 0;
		p->bandw_count = 0;
		p->desc.fmt_count = 0;
		p->desc.port = 0;
		p->desc.port_count = 0;
		return sdp_media_sptr(p,
			function20_bind_Ex(&MemPool_type::FreeObj<sdp_media>, GetMemPoolInstance()));
	}

	Sip_SDP::Sip_SDP(void)
	{

	}

	Sip_SDP::~Sip_SDP(void)
	{

	}

	void Sip_SDP::InitSessionSdp(sdp_session* pSdp)
	{
		if (pSdp != NULL)
		{
			pSdp->attr_count = 0;
			pSdp->bandw_count = 0;
			pSdp->media_count = 0;
			pSdp->proto_version = 0;
			pSdp->origin.id = 0;
			pSdp->origin.version = 0;
			pSdp->time.start = 0;
			pSdp->time.stop = 0;
		}
	}

	_string_t Sip_SDP::FindValueAndInc(char** szBuf, const _string_t& Start, const _string_t& End)
	{
		if (*szBuf == NULL || Start == "" || End == "")
			return "";

		const char cMask = '|';
		int iFilterCount = 0, iLen = 0;

		//解析字符串中的结束过滤条件
		_string_t Filters[10];
		char* pFilterCur = (char*)End.c_str();
		char* pFilterEnd = NULL;
		do
		{
			pFilterEnd = strchr(pFilterCur, cMask);
			if (pFilterEnd == NULL)
			{
				Filters[iFilterCount] = pFilterCur;
				iFilterCount++;
				break;
			}
			else
			{
				iLen = (int)(pFilterEnd - pFilterCur);
				Filters[iFilterCount] = _string_t(pFilterCur, iLen);
				pFilterCur = pFilterCur + 1 + iLen;
				iFilterCount++;
			}

		} while (true);

		//查找匹配内容
		char* pBegin = NULL;
		char* pTmp = NULL;
		if (Start == "SOF")
		{//内容起始位置
			pBegin = *szBuf;
			pTmp = pBegin;
		}
		else
		{
			pBegin = strstr(*szBuf, Start.c_str());
			pTmp = (char*)(pBegin + Start.size());
		}

		if (pBegin != NULL)
		{
			for (int i = 0; i < iFilterCount; i++)
			{
				const char* pEnd = strstr(pTmp, Filters[i].c_str());
				if (pEnd != NULL)
				{
					const int iSize = (pEnd - pTmp);
					*szBuf = pTmp + iSize + Filters[i].size();
					return _string_t(pTmp, iSize);
				}
				else if (Filters[i] == "EOF")//起始到结束
				{
					*szBuf += strlen(*szBuf);
					return	pTmp;
				}
			}
		}

		return "";
	}

	int Sip_SDP::ParseLine(const char* szData, int iDataSize, sdp_session* pSdp)
	{
		Sip_Parse::Hdr_INFO_sptr	sptr;
		sdp_attr		Attr;
		sdp_bandw		Bandw;
		sdp_media_sptr	media;
		if(pSdp->media_count > 0)
			media = pSdp->media[pSdp->media_count - 1];

		int iRet = -1, iReadLen = -1, iContentLen = 0;
		char* pStart = (char*)szData;
		char* pCur = strstr((char*)pStart, "\r\n");
		if (pCur != NULL)
		{
			iReadLen = pCur - pStart + 2;//包含回车换行符
			iContentLen = pCur - pStart - 2;//去除如“a=”两个字符
			pCur = strstr((char*)pStart, "=");
			if (pCur != NULL)
			{
				sptr = ISip_Proto::Alloc();
				switch ((pCur - 1)[0])
				{
				case 'a':
					{
						pCur = &pCur[1];
						Attr.name = FindValueAndInc(&pCur, "SOF", ":|\r");
						if(pCur < & szData[iReadLen - 1]) //小于结束位置
							Attr.value = FindValueAndInc(&pCur, "SOF", "\r");

						sdp_attr_sptr sptr = Alloc_attr();
						CloneAttrInfo(sptr.get(), &Attr);
						if (media)
						{
							media->attr[media->attr_count++] = sptr;
						}
						else
						{
							pSdp->attr[pSdp->attr_count++] = sptr;
						}
					}
					break;
				case 'o':
					{
						pCur = &pCur[1];
						pSdp->origin.user = FindValueAndInc(&pCur, "SOF", " ");
						pSdp->origin.id = _string_t::StringToInt(FindValueAndInc(&pCur, "SOF", " "));
						pSdp->origin.version = _string_t::StringToInt(FindValueAndInc(&pCur, "SOF", " "));

						pSdp->origin.net_type = FindValueAndInc(&pCur, "SOF", " ");
						pSdp->origin.addr_type = FindValueAndInc(&pCur, "SOF", " ");
						pSdp->origin.addr = FindValueAndInc(&pCur, "SOF", "\r");
					}
					break;
				case 's':
					{
						pCur = &pCur[1];
						pSdp->name = FindValueAndInc(&pCur, "SOF", "\r");
					}
					break;
				case 'c':
					{
						sdp_conn_sptr sptr = Alloc_conn();

						pCur = &pCur[1];
						if (media)
						{
							sptr->net_type = FindValueAndInc(&pCur, "SOF", " ");
							sptr->addr_type = FindValueAndInc(&pCur, "SOF", " ");
							sptr->addr = FindValueAndInc(&pCur, "SOF", "\r");
							media->conn = sptr;
						}
						else
						{
							sptr->net_type = FindValueAndInc(&pCur, "SOF", " ");
							sptr->addr_type = FindValueAndInc(&pCur, "SOF", " ");
							sptr->addr = FindValueAndInc(&pCur, "SOF", "\r");
							pSdp->conn = sptr;
						}
					}
					break;
				case 't':
					{
						pCur = &pCur[1];
						pSdp->time.start = _string_t::StringToInt(FindValueAndInc(&pCur, "SOF", " "));
						pSdp->time.stop = _string_t::StringToInt(FindValueAndInc(&pCur, "SOF", "\r"));
					}
					break;
				case 'm':
					{
						media = Alloc_media();
						if (pSdp->media_count < SDP_MEDIA_MAX)
						{
							pCur = &pCur[1];
							media->desc.media = FindValueAndInc(&pCur, "SOF", " ");
							_string_t sPortAndCount = FindValueAndInc(&pCur, "SOF", " ");
							char* pPortAndCount = &sPortAndCount[0];
							_string_t sPort = FindValueAndInc(&pPortAndCount, "SOF", "/");
							if (sPort == "")
							{//不存在PortCount
								media->desc.port = _string_t::StringToInt(sPortAndCount);
								media->desc.port_count = 0;
							}
							else
							{//存在PortCount
								media->desc.port = _string_t::StringToInt(sPort);
								media->desc.port_count = _string_t::StringToInt(FindValueAndInc(&pPortAndCount, "SOF", "\r"));
							}

							media->desc.transport = FindValueAndInc(&pCur, "SOF", " ");

							_string_t fmt;
							while (true)
							{
								
								const char* p = strstr(pCur, " ");
								const char* pMask = ( p != NULL && p < strstr(pCur, "\r") ) ? " " : "\r";

								fmt = FindValueAndInc(&pCur, "SOF", pMask);
								media->desc.fmt[media->desc.fmt_count ++] = fmt;
								if (pMask[0] == '\r')
									break;
							}

							pSdp->media[pSdp->media_count++] = media;
						}
					}
					break;
				case 'v':
					{
						pSdp->proto_version = _string_t::StringToInt(_string_t(&pCur[1], iContentLen));
					}
					break;
				case 'b':
					{
						pCur = &pCur[1];
						Bandw.modifier = FindValueAndInc(&pCur, "SOF", ":");
						Bandw.value = _string_t::StringToInt(FindValueAndInc(&pCur, "SOF", "\r"));

						sdp_bandw_sptr sptr = Alloc_bandw();
						CloneBandwInfo(sptr.get(), &Bandw);

						if (media)
						{	
							media->bandw[media->bandw_count++] = sptr;
						}
						else
						{
							pSdp->bandw[pSdp->bandw_count++] = sptr;
						}
					}
					break;
				case 'i':
					{
						pCur = &pCur[1];
						pSdp->session_info = Alloc_session_info();
						pSdp->session_info->author = FindValueAndInc(&pCur, "SOF", " ");
						if (pCur < &szData[iReadLen - 1]) //小于结束位置
							pSdp->session_info->copyright = FindValueAndInc(&pCur, "SOF", "\r");
					}
					break;

				}
			}
		}
		else
		{
			iReadLen = -1;
		}

		return iReadLen;
	}

	void Sip_SDP::CloneSdp(sdp_session* dst, sdp_session* src)
	{
		dst->proto_version = src->proto_version;
		dst->origin.user = src->origin.user;
		dst->origin.id = src->origin.id;
		dst->origin.version = src->origin.version;
		dst->origin.net_type = src->origin.net_type;
		dst->origin.addr_type = src->origin.addr_type;
		dst->origin.addr = src->origin.addr;

		dst->name = src->name;
		CloneConnInfo(dst->conn.get(), src->conn.get());

		dst->bandw_count = src->bandw_count;
		for (int i = 0; i < src->bandw_count; i++)
		{
			CloneBandwInfo(dst->bandw[i].get(), src->bandw[i].get());
		}

		dst->time.start = src->time.start;
		dst->time.stop = src->time.stop;

		dst->attr_count = src->attr_count;
		for (int k = 0; k < src->attr_count; k++)
		{
			CloneAttrInfo(dst->attr[k].get(), src->attr[k].get());
		}

		dst->media_count = src->media_count;
		for (int j = 0; j < src->media_count; j++)
		{
			CloneMediaInfo(dst->media[j].get(), src->media[j].get());
		}
	}

	void Sip_SDP::CloneMediaInfo(sdp_media* dst, sdp_media* src)
	{
		dst->desc.media = src->desc.media;
		dst->desc.port = src->desc.port;
		dst->desc.port_count = src->desc.port_count;
		dst->desc.transport = src->desc.transport;
		dst->desc.fmt_count = src->desc.fmt_count;
		for (unsigned int i = 0; i < src->desc.fmt_count; i++)
		{
			dst->desc.fmt[i] = src->desc.fmt[i];
		}

		CloneConnInfo(dst->conn.get(), src->conn.get());

		dst->bandw_count = src->bandw_count;
		for (int j = 0; j < src->bandw_count; j++)
		{
			CloneBandwInfo(dst->bandw[j].get(), src->bandw[j].get());
		}

		dst->attr_count = src->attr_count;
		for (int k = 0; k < src->attr_count; k++)
		{
			CloneAttrInfo(dst->attr[k].get(), src->attr[k].get());
		}
	}

	void Sip_SDP::CloneConnInfo(sdp_conn* dst, sdp_conn* src)
	{
		dst->net_type = src->net_type;
		dst->addr_type = src->addr_type;
		dst->addr = src->addr;
	}

	void Sip_SDP::CloneBandwInfo(sdp_bandw* dst, sdp_bandw* src)
	{
		dst->modifier = src->modifier;
		dst->value = src->value;
	}

	void Sip_SDP::CloneAttrInfo(sdp_attr* dst, sdp_attr* src)
	{
		dst->name = src->name;
		dst->value = src->value;
	}

	_string_t Sip_SDP::toString(const sdp_session& Sdp)
	{
		const char* End = "\r\n";

		_string_t str;

		//v=
		str = "v=" + _string_t::NumberToStr(Sdp.proto_version) + End;

		//o=
		str += "o=" + Sdp.origin.user;
		str += " " + _string_t::NumberToStr(Sdp.origin.id);
		str += " " + _string_t::NumberToStr(Sdp.origin.version);
		str += " " + Sdp.origin.net_type;
		str += " " + Sdp.origin.addr_type;
		str += " " + Sdp.origin.addr + End;

		//s=
		if(Sdp.name != "")
			str += "s=" + Sdp.name + End;

		//i=
		if (Sdp.session_info)
		{
			str += "i=" + Sdp.session_info->author + " " + Sdp.session_info->copyright + End;
		}

		//c=
		str += "c=" + Sdp.conn->net_type;
		str += " " + Sdp.conn->addr_type;
		str += " " + Sdp.conn->addr + End;

		int i;
		//b=
		for (i = 0; i < Sdp.bandw_count; i ++)
		{
			str += "b=" + Sdp.bandw[i]->modifier + ":";
			str += _string_t::NumberToStr(Sdp.bandw[i]->value) + End;
		}

		//t=
		str += "t=" + _string_t::NumberToStr(Sdp.time.start) + " " + _string_t::NumberToStr(Sdp.time.stop) + End;

		//a=
		for (i = 0; i < Sdp.attr_count; i++)
		{
			str += "a=" + Sdp.attr[i]->name + ":";
			str += Sdp.attr[i]->value + End;
		}

		//m=
		for (i = 0; i < Sdp.media_count; i++)
		{
			int j = 0;
			str += "m=" + Sdp.media[i]->desc.media;
			str += " " + _string_t::NumberToStr(Sdp.media[i]->desc.port);
			if (Sdp.media[i]->desc.port_count > 0)
			{
				str += "/" + _string_t::NumberToStr(Sdp.media[i]->desc.port_count);
			}

			str += " " + Sdp.media[i]->desc.transport;
			for (j = 0; j < Sdp.media[i]->desc.fmt_count; j++)
			{
				str += " " + Sdp.media[i]->desc.fmt[j];
			}

			str += End;

			for (j = 0;  j < Sdp.media[i]->attr_count; j ++)
			{
				str += "a=" + Sdp.media[i]->attr[j]->name + ":";
				str += Sdp.media[i]->attr[j]->value + End;
			}

			for (j = 0; j < Sdp.media[i]->bandw_count; j++)
			{
				str += "b=" + Sdp.media[i]->bandw[j]->modifier + ":";
				str += _string_t::NumberToStr(Sdp.media[i]->bandw[j]->value) + End;
			}
		}

		return str;
	}

}

