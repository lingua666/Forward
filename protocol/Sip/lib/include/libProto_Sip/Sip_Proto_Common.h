
#ifndef __SIP_SIP_PROTO_COMMON_20160510104022_1462848022_H__
#define __SIP_SIP_PROTO_COMMON_20160510104022_1462848022_H__

#include <libProto_Sip/Sip_Parse.h>

namespace	_sip_ {

	class Sip_Proto_Common
	{
	public:
		typedef ISip_Proto::Hdr_INFO_sptr	Hdr_INFO_sptr;
		typedef ISip_Proto::Hdr_List		Hdr_List;

		typedef SmartPTR<Via_Hdr_INFO>		Via_Hdr_sptr;
		typedef std::list<Via_Hdr_sptr>		Via_Hdr_List;
		typedef Sip_Parse::Msg_Map			Msg_Map;

	private:
		static Via_Hdr_sptr	Alloc_Via(void);

	public:
		static _string_t GetStatusCodeString(sip_status_code Status);

	public:
		Sip_Proto_Common(void);

		Sip_Proto_Common(const Sip_Proto_Common& t1);

		~Sip_Proto_Common(void);

		Sip_Proto_Common& operator=(const Sip_Proto_Common& t1);

		void Init(void);

		void Release(void);

		void SetFromUri(const _string_t& Coding, const _string_t& Domain, const _string_t& Tag);

		const Uri_INFO&	GetFromUri(void) const;

		void SetToUri(const _string_t& Coding, const _string_t& Domain, const _string_t& Tag);

		const Uri_INFO&	GetToUri(void) const;

		int AddVia(const Via_Hdr_INFO& Via);

		int RemoveFirstVia(void);

		const Via_Hdr_INFO& GetFirstVia(void);

		void SetCallID(const _string_t& CallID);

		const _string_t& GetCallID(void) const;

		void SetCSeq(long CSeq, const _string_t& MethodName);

		const long& GetCSeq(void) const;

		const _string_t& GetMethodName(void) const;

		void SetMaxForwards(unsigned char MaxForwards);

		const unsigned char& GetMaxForwards(void) const;

		void GenerateToList(Hdr_List* pList);

		bool Parse(const char* c_szBuf, UInt32 uSize);

		bool ParseHdr(const Hdr_INFO_sptr& sptr);

	private:
		void Clone(const Sip_Proto_Common& t1);

	private:
		Via_Hdr_List	_ViaList;
		Uri_INFO		_From;
		Uri_INFO		_To;
		_string_t		_CallID;
		long			_CSeq;
		_string_t		_MethodName;
		unsigned char	_MaxForwards;
	};
}

#endif
