
#ifndef __SIP_SIP_RESPONSE_20160510104022_1462848022_H__
#define __SIP_SIP_RESPONSE_20160510104022_1462848022_H__

#include <libProto_Sip/Sip_Proto_Common.h>

namespace	_sip_ {

	class Sip_Response : public ISip_Proto
	{
	public:
		Sip_Response(void);

		virtual ~Sip_Response(void)
		{}

		int Init(sip_status_code Status);

		void SetVia(const Host_Port_INFO& Local, const _string_t& Branch);

		void SetFromUri(const _string_t& Coding, const _string_t& Domain, const _string_t& Tag);

		void SetToUri(const _string_t& Coding, const _string_t& Domain, const _string_t& Tag);

		void SetCallID(const _string_t& CallID);

		void SetContent(const _string_t& Content, const _string_t& ContentType);

		int SetVia(const _sip_::Via_Hdr_INFO& Via);

		void AddVia(const _string_t& Content, const _string_t& Recvd);

		void AddToUri(const _string_t& Content, const _string_t& Tag);

		void AddFields(sip_hdr_e Hdr_e, const _string_t& Content);

		_string_big_t	Generate(bool isOnlyList = false);

	protected:
		Sip_Proto_Common	_Common;
		bool				_isUdp;

	private:
		sip_status_code	_Status;
	};

	class Sip_Response_Ack : public ISip_Proto
	{
	public:
		Sip_Response_Ack(void);

		virtual ~Sip_Response_Ack(void)
		{}

		int Init(const Host_Port_INFO& DstHost, bool isUdp);

		void SetVia(const Host_Port_INFO& Local, const _string_t& Branch);

		void SetFromUri(const _string_t& Coding, const _string_t& Domain, const _string_t& Tag);

		void SetToUri(const _string_t& Coding, const _string_t& Domain, const _string_t& Tag);

		void SetCallID(const _string_t& CallID);

		void SetContent(const _string_t& Content, const _string_t& ContentType);

		void SetCSeq(long CSeq);

		int SetVia(const _sip_::Via_Hdr_INFO& Via);

		void AddVia(const _string_t& Content, const _string_t& Recvd);

		void AddToUri(const _string_t& Content, const _string_t& Tag);

		void AddFields(sip_hdr_e Hdr_e, const _string_t& Content);

		_string_big_t	Generate(bool isOnlyList = false);

	protected:
		Sip_Proto_Common	_Common;
		bool				_isUdp;
		Host_Port_INFO		_DstHost;

	private:
		sip_status_code	_Status;
	};
	
}

#endif
