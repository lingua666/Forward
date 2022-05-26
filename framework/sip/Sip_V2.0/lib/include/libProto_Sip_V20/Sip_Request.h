
#ifndef __SIP_SIP_REQUEST_20160510104022_1462848022_H__
#define __SIP_SIP_REQUEST_20160510104022_1462848022_H__

#include <libProto_Sip_V20/Sip_Sdp.h>

namespace	_sip_v20_ {

	/******************************Sip_Request******************************/

	class Sip_Request : public ISip_Proto
	{
	protected:
		Sip_Request(void);

		virtual ~Sip_Request(void);

		void SetMethodType(const sip_method_e& MethodType);

		const sip_method_e GetMethodType(void) const;

		const _string_t GetMethodName(void) const;

	public:
		void SetVia(const Host_Port_INFO& Local, const _string_t& Branch);

		void SetFromUri(const _string_t& Coding, const _string_t& Domain, const _string_t& Tag);

		void SetToUri(const _string_t& Coding, const _string_t& Domain, const _string_t& Tag);

		void SetCallID(const _string_t& CallID);

		void SetDate(const _string_t& Date);

		void SetContent(const _string_t& Content, const _string_t& ContentType);

	protected:
		Sip_Proto_Common	_Common;
		bool				_isUdp;

	private:
		sip_method_e	_MethodType;
	};



	/******************************Sip_Request_Register******************************/

	class Sip_Request_Register : public Sip_Request
	{
	public:
		Sip_Request_Register(void);

		~Sip_Request_Register(void);

		int Init(const UriDetails_INFO& Src, 
			const UriDetails_INFO& Dest, UInt32 uExpires,
			long CSeq, const _string_t& Rinstance = "", bool isUdp = true);

		int Release(void);

		void SetToken( const _string_t& UserName,
			const _string_t& Realm,
			const _string_t& Nonce,
			const _string_t& Response,
			const _string_t& Algorithm = "MD5" );

		_string_big_t Generate( void );

	private:
		UInt32			_uExpires;
		UriDetails_INFO	_Src;
		UriDetails_INFO	_Dest;
		_string_t		_Rinstance;
		_string_t		_UserName;
		_string_t		_Realm;
		_string_t		_Nonce;
		_string_t		_Algorithm;
		_string_t		_Token;
	};



	/******************************Sip_Request_Invite******************************/

	class Sip_Request_Invite : public Sip_Request
	{
	public:
		Sip_Request_Invite(void);

		~Sip_Request_Invite(void);

		int Init(const UriDetails_INFO& Src,
			const UriDetails_INFO& Dest, long CSeq, bool isUdp = true);

		int Release(void);

		void SetSdp(const Sip_SDP& Sdp);

		void SetSdp(const Sip_SDP::sdp_session& Sdp_Session);

		_string_t GetSdpString(void);

		_string_big_t Generate(void);

	private:
		UriDetails_INFO	_Src;
		UriDetails_INFO	_Dest;
		Sip_SDP			_Sdp;
	};



	/******************************Sip_Request_Ack******************************/

	class Sip_Request_Ack : public Sip_Request
	{
	public:
		Sip_Request_Ack(void);

		~Sip_Request_Ack(void);

		int Init(const UriDetails_INFO& Src,
			const UriDetails_INFO& Dest, long CSeq, bool isUdp = true);

		int Release(void);

		_string_big_t Generate(void);

	private:
		UriDetails_INFO	_Src;
		UriDetails_INFO	_Dest;
	};



	/******************************Sip_Request_Bye******************************/

	class Sip_Request_Bye : public Sip_Request
	{
	public:
		Sip_Request_Bye(void);

		~Sip_Request_Bye(void);

		int Init(const UriDetails_INFO& Src,
			const UriDetails_INFO& Dest, long CSeq, bool isUdp = true);

		int Release(void);

		_string_big_t Generate(void);

	private:
		UriDetails_INFO	_Src;
		UriDetails_INFO	_Dest;
	};



	/******************************Sip_Request_Message******************************/

	class Sip_Request_Message : public Sip_Request
	{
	public:
		Sip_Request_Message(void);

		~Sip_Request_Message(void);

		int Init(const UriDetails_INFO& Src,
			const UriDetails_INFO& Dest, long CSeq, bool isUdp = true);

		int Release(void);

		void SetToken(const _string_t& UserName,
			const _string_t& Realm,
			const _string_t& Nonce,
			const _string_t& Response,
			const _string_t& Algorithm = "MD5");

		_string_big_t Generate(void);

	private:
		UriDetails_INFO	_Src;
		UriDetails_INFO	_Dest;
		_string_t		_UserName;
		_string_t		_Realm;
		_string_t		_Nonce;
		_string_t		_Algorithm;
		_string_t		_Token;
	};



	/******************************Sip_Request_Message******************************/

	class Sip_Request_Options  : public Sip_Request
	{
	public:
		Sip_Request_Options(void);

		~Sip_Request_Options(void);

		int Init(const UriDetails_INFO& Src,
			const UriDetails_INFO& Dest, long CSeq, bool isUdp = true);

		int Release(void);

		_string_big_t Generate(void);

	private:
		UriDetails_INFO	_Src;
		UriDetails_INFO	_Dest;
	};



	/******************************Sip_Request_Message******************************/

	class Sip_Request_Cancel : public Sip_Request
	{
	public:
		Sip_Request_Cancel(void);

		~Sip_Request_Cancel(void);

		int Init(const UriDetails_INFO& Src,
			const UriDetails_INFO& Dest, long CSeq, bool isUdp = true);

		int Release(void);

		_string_big_t Generate(void);

	private:
		UriDetails_INFO	_Src;
		UriDetails_INFO	_Dest;
	};
}

#endif
