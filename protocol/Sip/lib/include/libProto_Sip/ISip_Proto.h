
#ifndef __SIP_ISIP_PROTO_20160510104022_1462848022_H__
#define __SIP_ISIP_PROTO_20160510104022_1462848022_H__

#include <libFoundation/modafx.h>

namespace	_sip_ {

	typedef _foundation_::_string_t				_string_t;
	typedef _foundation_::_string_big_t			_string_big_t;

	const _string_t sip_proto_version = "SIP UA Proto Release V1.00 Date 201706082116";
	const _string_t sip_version = "SIP/2.0";
	const _string_t rfc3261_branch = "z9hG4bK";

	/**
	* Message type (request or response).
	*/
	typedef enum sip_msg_type_e
	{
		SIP_REQUEST_MSG,	    /**< Indicates request message. */
		SIP_RESPONSE_MSG	    /**< Indicates response message. */
	} sip_msg_type_e;

	/**
	* This enumeration declares SIP methods as described by RFC3261. Additional
	* methods do exist, and they are described by corresponding RFCs for the SIP
	* extentensions. Since they won't alter the characteristic of the processing
	* of the message, they don't need to be explicitly mentioned here.
	*/
	typedef enum sip_method_e
	{
		SIP_INVITE_METHOD,		/**< INVITE method, for establishing dialogs.   */
		SIP_CANCEL_METHOD,		/**< CANCEL method, for cancelling request.	    */
		SIP_ACK_METHOD,			/**< ACK method.				    */
		SIP_BYE_METHOD,			/**< BYE method, for terminating dialog.	    */
		SIP_REGISTER_METHOD,	/**< REGISTER method.			    */
		SIP_OPTIONS_METHOD,		/**< OPTIONS method.			    */
		SIP_SUBSCRIBE_METHOD,	/**< SUBSCRIBE method.			    */
		SIP_MESSAGE_METHOD,		/**< MESSAGE method.			    */

		SIP_REFER_METHOD,		/**< REFER method.			    */
		SIP_NOTIFY_METHOD,		/**< NOTIFY method.			    */
		SIP_INFO_METHOD,		/**< INFO method.			    */

		SIP_OTHER_METHOD		/**< Other method.				    */

	} sip_method_e;

	const _string_t sip_method_names[] =
	{
		 "INVITE" ,   // SIP_INVITE_METHOD,
		 "CANCEL" ,   // SIP_CANCEL_METHOD,
		 "ACK" ,   // SIP_ACK_METHOD,
		 "BYE" ,   // SIP_BYE_METHOD,
		 "REGISTER" ,   // SIP_REGISTER_METHOD
		 "OPTIONS" ,   // SIP_OPTIONS_METHOD
		 "SUBSCRIBE" ,   // SIP_SUBSCRIBE_METHOD
		 "MESSAGE" ,   // SIP_MESSAGE_METHOD
		 "REFER" ,   // SIP_REFER_METHOD
		 "NOTIFY" ,   // SIP_NOTIFY_METHOD
		 "INFO" ,   // SIP_INFO_METHOD
		 "OTHER" ,   // SIP_OTHER_METHOD
	};

	struct Host_Port_INFO
	{
		_string_t Host;
		int	     Port;
	};

	struct Hdr_INFO
	{
		_string_t Name;
		_string_t Content;
	};

	struct Uri_INFO
	{
		_string_t	Coding;
		_string_t	Domain;
		_string_t	Tag;
	};

	struct Via_Hdr_INFO
	{
		bool			isUdp;
		Host_Port_INFO	Sent_by;
		int				Ttl;
		int				Rport;
		_string_t		Maddr;
		_string_t		Recvd;
		_string_t		Branch;
	};

	/**
	* Header types, as defined by RFC3261.
	*/
	typedef enum sip_hdr_e
	{
		SIP_H_ACCEPT,
		SIP_H_ACCEPT_ENCODING_UNIMP,
		SIP_H_ACCEPT_LANGUAGE_UNIMP,
		SIP_H_ALERT_INFO_UNIMP,
		SIP_H_ALLOW,
		SIP_H_AUTHENTICATION_INFO_UNIMP,
		SIP_H_AUTHORIZATION,
		SIP_H_CALL_ID,
		SIP_H_CALL_INFO_UNIMP,
		SIP_H_CONTACT,
		SIP_H_CONTENT_DISPOSITION_UNIMP,
		SIP_H_CONTENT_ENCODING_UNIMP,
		SIP_H_CONTENT_LANGUAGE_UNIMP,
		SIP_H_CONTENT_LENGTH,
		SIP_H_CONTENT_TYPE,
		SIP_H_CSEQ,
		SIP_H_DATE_UNIMP,
		SIP_H_ERROR_INFO_UNIMP,
		SIP_H_EXPIRES,
		SIP_H_FROM,
		SIP_H_IN_REPLY_TO_UNIMP,
		SIP_H_MAX_FORWARDS,
		SIP_H_MIME_VERSION_UNIMP,
		SIP_H_MIN_EXPIRES,
		SIP_H_ORGANIZATION_UNIMP,
		SIP_H_PRIORITY_UNIMP,
		SIP_H_PROXY_AUTHENTICATE,
		SIP_H_PROXY_AUTHORIZATION,
		SIP_H_PROXY_REQUIRE_UNIMP,
		SIP_H_RECORD_ROUTE,
		SIP_H_REPLY_TO_UNIMP,
		SIP_H_REQUIRE,
		SIP_H_RETRY_AFTER,
		SIP_H_ROUTE,
		SIP_H_SERVER_UNIMP,
		SIP_H_SUBJECT_UNIMP,
		SIP_H_SUPPORTED,
		SIP_H_TIMESTAMP_UNIMP,
		SIP_H_TO,
		SIP_H_UNSUPPORTED,
		SIP_H_USER_AGENT_UNIMP,
		SIP_H_VIA,
		SIP_H_WARNING_UNIMP,
		SIP_H_WWW_AUTHENTICATE,
		SIP_H_OTHER
	} sip_hdr_e;

	const _string_t sip_hdr_names[] =
	{
		 "Accept" ,   // SIP_H_ACCEPT,
		 "Accept-Encoding" ,   // SIP_H_ACCEPT_ENCODING,
		 "Accept-Language" ,   // SIP_H_ACCEPT_LANGUAGE,
		 "Alert-Info" ,   // SIP_H_ALERT_INFO,
		 "Allow" ,   // SIP_H_ALLOW,
		 "Authentication-Info" ,   // SIP_H_AUTHENTICATION_INFO,
		 "Authorization" ,   // SIP_H_AUTHORIZATION,
		 "Call-ID" ,    // SIP_H_CALL_ID,
		 "Call-Info" ,   // SIP_H_CALL_INFO,
		 "Contact" ,    // SIP_H_CONTACT,
		 "Content-Disposition" ,   // SIP_H_CONTENT_DISPOSITION,
		 "Content-Encoding" ,    // SIP_H_CONTENT_ENCODING,
		 "Content-Language" ,   // SIP_H_CONTENT_LANGUAGE,
		 "Content-Length" ,    // SIP_H_CONTENT_LENGTH,
		 "Content-Type" ,    // SIP_H_CONTENT_TYPE,
		 "CSeq" ,   // SIP_H_CSEQ,
		 "Date" ,   // SIP_H_DATE,
		 "Error-Info" ,   // SIP_H_ERROR_INFO,
		 "Expires" ,   // SIP_H_EXPIRES,
		 "From" ,    // SIP_H_FROM,
		 "In-Reply-To" ,   // SIP_H_IN_REPLY_TO,
		 "Max-Forwards" ,   // SIP_H_MAX_FORWARDS,
		 "MIME-Version" ,   // SIP_H_MIME_VERSION,
		 "Min-Expires" ,   // SIP_H_MIN_EXPIRES,
		 "Organization" ,   // SIP_H_ORGANIZATION,
		 "Priority" ,   // SIP_H_PRIORITY,
		 "Proxy-Authenticate" ,   // SIP_H_PROXY_AUTHENTICATE,
		 "Proxy-Authorization" ,   // SIP_H_PROXY_AUTHORIZATION,
		 "Proxy-Require" ,   // SIP_H_PROXY_REQUIRE,
		 "Record-Route" ,   // SIP_H_RECORD_ROUTE,
		 "Reply-To" ,   // SIP_H_REPLY_TO,
		 "Require" ,   // SIP_H_REQUIRE,
		 "Retry-After" ,   // SIP_H_RETRY_AFTER,
		 "Route" ,   // SIP_H_ROUTE,
		 "Server" ,   // SIP_H_SERVER,
		 "Subject" ,    // SIP_H_SUBJECT,
		 "Supported" ,    // SIP_H_SUPPORTED,
		 "Timestamp" ,   // SIP_H_TIMESTAMP,
		 "To" ,    // SIP_H_TO,
		 "Unsupported" ,   // SIP_H_UNSUPPORTED,
		 "User-Agent" ,   // SIP_H_USER_AGENT,
		 "Via" ,    // SIP_H_VIA,
		 "Warning" ,   // SIP_H_WARNING,
		 "WWW-Authenticate" ,   // SIP_H_WWW_AUTHENTICATE,
		 "_Unknown-Header"    // SIP_H_OTHER,
	};

	typedef enum sip_status_code
	{
		SIP_SC_TRYING = 100,
		SIP_SC_RINGING = 180,
		SIP_SC_CALL_BEING_FORWARDED = 181,
		SIP_SC_QUEUED = 182,
		SIP_SC_PROGRESS = 183,

		SIP_SC_OK = 200,
		SIP_SC_ACCEPTED = 202,

		SIP_SC_MULTIPLE_CHOICES = 300,
		SIP_SC_MOVED_PERMANENTLY = 301,
		SIP_SC_MOVED_TEMPORARILY = 302,
		SIP_SC_USE_PROXY = 305,
		SIP_SC_ALTERNATIVE_SERVICE = 380,

		SIP_SC_BAD_REQUEST = 400,
		SIP_SC_UNAUTHORIZED = 401,
		SIP_SC_PAYMENT_REQUIRED = 402,
		SIP_SC_FORBIDDEN = 403,
		SIP_SC_NOT_FOUND = 404,
		SIP_SC_METHOD_NOT_ALLOWED = 405,
		SIP_SC_NOT_ACCEPTABLE = 406,
		SIP_SC_PROXY_AUTHENTICATION_REQUIRED = 407,
		SIP_SC_REQUEST_TIMEOUT = 408,
		SIP_SC_GONE = 410,
		SIP_SC_REQUEST_ENTITY_TOO_LARGE = 413,
		SIP_SC_REQUEST_URI_TOO_LONG = 414,
		SIP_SC_UNSUPPORTED_MEDIA_TYPE = 415,
		SIP_SC_UNSUPPORTED_URI_SCHEME = 416,
		SIP_SC_BAD_EXTENSION = 420,
		SIP_SC_EXTENSION_REQUIRED = 421,
		SIP_SC_SESSION_TIMER_TOO_SMALL = 422,
		SIP_SC_INTERVAL_TOO_BRIEF = 423,
		SIP_SC_TEMPORARILY_UNAVAILABLE = 480,
		SIP_SC_CALL_TSX_DOES_NOT_EXIST = 481,
		SIP_SC_LOOP_DETECTED = 482,
		SIP_SC_TOO_MANY_HOPS = 483,
		SIP_SC_ADDRESS_INCOMPLETE = 484,
		SIP_AC_AMBIGUOUS = 485,
		SIP_SC_BUSY_HERE = 486,
		SIP_SC_REQUEST_TERMINATED = 487,
		SIP_SC_NOT_ACCEPTABLE_HERE = 488,
		SIP_SC_BAD_EVENT = 489,
		SIP_SC_REQUEST_UPDATED = 490,
		SIP_SC_REQUEST_PENDING = 491,
		SIP_SC_UNDECIPHERABLE = 493,

		SIP_SC_INTERNAL_SERVER_ERROR = 500,
		SIP_SC_NOT_IMPLEMENTED = 501,
		SIP_SC_BAD_GATEWAY = 502,
		SIP_SC_SERVICE_UNAVAILABLE = 503,
		SIP_SC_SERVER_TIMEOUT = 504,
		SIP_SC_VERSION_NOT_SUPPORTED = 505,
		SIP_SC_MESSAGE_TOO_LARGE = 513,
		SIP_SC_PRECONDITION_FAILURE = 580,

		SIP_SC_BUSY_EVERYWHERE = 600,
		SIP_SC_DECLINE = 603,
		SIP_SC_DOES_NOT_EXIST_ANYWHERE = 604,
		SIP_SC_NOT_ACCEPTABLE_ANYWHERE = 606,

		SIP_SC_TSX_TIMEOUT = SIP_SC_REQUEST_TIMEOUT,
		/*SIP_SC_TSX_RESOLVE_ERROR = 702,*/
		SIP_SC_TSX_TRANSPORT_ERROR = SIP_SC_SERVICE_UNAVAILABLE,

		/* This is not an actual status code, but rather a constant
		* to force GCC to use 32bit to represent this enum, since
		* we have a code in PJSUA-LIB that assigns an integer
		* to this enum (see pjsua_acc_get_info() function).
		*/
		SIP_SC__force_32bit = 0x7FFFFFFF

	} sip_status_code;

	class ISip_Proto
	{
	public:
		typedef SmartPTR<Hdr_INFO>				Hdr_INFO_sptr;
		typedef std::list<Hdr_INFO_sptr>		Hdr_List;

		static Hdr_INFO_sptr	Alloc(void);

	protected:

		static _string_t  GenerateCallID(void);

		static _string_t  GenerateBranch(void);

		static _string_t  GenerateTag(void);

	protected:
		Hdr_List		_HdrList;
		_string_t		_Content;

	protected:
		ISip_Proto(void);

		virtual ~ISip_Proto(void);

		void	push_front(const Hdr_INFO_sptr& sptr);

		void	pop_front(void);

		void	push_back(const Hdr_INFO_sptr& sptr);

		void clear(void);

		void AddFields(const _string_t& Name, const _string_t& Content);

		void SetFromUri(const _string_t& Coding, const _string_t& Domain, const _string_t& Tag);

		void SetToUri(const _string_t& Coding, const _string_t& Domain, const _string_t& Tag);

		void AddVia(const Via_Hdr_INFO& Via);

		void SetCallID(const _string_t& CallID);

		void SetCSeq(long CSeq);

		void SetMaxForwards(unsigned char MaxForwards);

		void	SetContent(const _string_t& Content);

		//Head 参数内容不包含换行符
		_string_big_t	GenerateString(const _string_t& Head);
	};
}

#endif
