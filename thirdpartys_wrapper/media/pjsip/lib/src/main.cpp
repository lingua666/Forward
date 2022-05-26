

/**
* This enumeration lists standard SIP status codes according to RFC 3261.
* In addition, it also declares new status class 7xx for errors generated
* by the stack. This status class however should not get transmitted on the
* wire.
*/
typedef enum pjsip_status_code
{
	PJSIP_SC_TRYING = 100,
	PJSIP_SC_RINGING = 180,
	PJSIP_SC_CALL_BEING_FORWARDED = 181,
	PJSIP_SC_QUEUED = 182,
	PJSIP_SC_PROGRESS = 183,

	PJSIP_SC_OK = 200,
	PJSIP_SC_ACCEPTED = 202,

	PJSIP_SC_MULTIPLE_CHOICES = 300,
	PJSIP_SC_MOVED_PERMANENTLY = 301,
	PJSIP_SC_MOVED_TEMPORARILY = 302,
	PJSIP_SC_USE_PROXY = 305,
	PJSIP_SC_ALTERNATIVE_SERVICE = 380,

	PJSIP_SC_BAD_REQUEST = 400,
	PJSIP_SC_UNAUTHORIZED = 401,
	PJSIP_SC_PAYMENT_REQUIRED = 402,
	PJSIP_SC_FORBIDDEN = 403,
	PJSIP_SC_NOT_FOUND = 404,
	PJSIP_SC_METHOD_NOT_ALLOWED = 405,
	PJSIP_SC_NOT_ACCEPTABLE = 406,
	PJSIP_SC_PROXY_AUTHENTICATION_REQUIRED = 407,
	PJSIP_SC_REQUEST_TIMEOUT = 408,
	PJSIP_SC_GONE = 410,
	PJSIP_SC_REQUEST_ENTITY_TOO_LARGE = 413,
	PJSIP_SC_REQUEST_URI_TOO_LONG = 414,
	PJSIP_SC_UNSUPPORTED_MEDIA_TYPE = 415,
	PJSIP_SC_UNSUPPORTED_URI_SCHEME = 416,
	PJSIP_SC_BAD_EXTENSION = 420,
	PJSIP_SC_EXTENSION_REQUIRED = 421,
	PJSIP_SC_SESSION_TIMER_TOO_SMALL = 422,
	PJSIP_SC_INTERVAL_TOO_BRIEF = 423,
	PJSIP_SC_TEMPORARILY_UNAVAILABLE = 480,
	PJSIP_SC_CALL_TSX_DOES_NOT_EXIST = 481,
	PJSIP_SC_LOOP_DETECTED = 482,
	PJSIP_SC_TOO_MANY_HOPS = 483,
	PJSIP_SC_ADDRESS_INCOMPLETE = 484,
	PJSIP_AC_AMBIGUOUS = 485,
	PJSIP_SC_BUSY_HERE = 486,
	PJSIP_SC_REQUEST_TERMINATED = 487,
	PJSIP_SC_NOT_ACCEPTABLE_HERE = 488,
	PJSIP_SC_BAD_EVENT = 489,
	PJSIP_SC_REQUEST_UPDATED = 490,
	PJSIP_SC_REQUEST_PENDING = 491,
	PJSIP_SC_UNDECIPHERABLE = 493,

	PJSIP_SC_INTERNAL_SERVER_ERROR = 500,
	PJSIP_SC_NOT_IMPLEMENTED = 501,
	PJSIP_SC_BAD_GATEWAY = 502,
	PJSIP_SC_SERVICE_UNAVAILABLE = 503,
	PJSIP_SC_SERVER_TIMEOUT = 504,
	PJSIP_SC_VERSION_NOT_SUPPORTED = 505,
	PJSIP_SC_MESSAGE_TOO_LARGE = 513,
	PJSIP_SC_PRECONDITION_FAILURE = 580,

	PJSIP_SC_BUSY_EVERYWHERE = 600,
	PJSIP_SC_DECLINE = 603,
	PJSIP_SC_DOES_NOT_EXIST_ANYWHERE = 604,
	PJSIP_SC_NOT_ACCEPTABLE_ANYWHERE = 606,

	PJSIP_SC_TSX_TIMEOUT = PJSIP_SC_REQUEST_TIMEOUT,
	/*PJSIP_SC_TSX_RESOLVE_ERROR = 702,*/
	PJSIP_SC_TSX_TRANSPORT_ERROR = PJSIP_SC_SERVICE_UNAVAILABLE,

	/* This is not an actual status code, but rather a constant
	* to force GCC to use 32bit to represent this enum, since
	* we have a code in PJSUA-LIB that assigns an integer
	* to this enum (see pjsua_acc_get_info() function).
	*/
	PJSIP_SC__force_32bit = 0x7FFFFFFF

} pjsip_status_code;


/**
* Message type (request or response).
*/
typedef enum pjsip_msg_type_e
{
	PJSIP_REQUEST_MSG,	    /**< Indicates request message. */
	PJSIP_RESPONSE_MSG	    /**< Indicates response message. */
} pjsip_msg_type_e;


/**
* Header types, as defined by RFC3261.
*/
typedef enum pjsip_hdr_e
{
	/*
	* These are the headers documented in RFC3261. Headers not documented
	* there must have type PJSIP_H_OTHER, and the header type itself is
	* recorded in the header name string.
	*
	* DO NOT CHANGE THE VALUE/ORDER OF THE HEADER IDs!!!.
	*/
	PJSIP_H_ACCEPT,
	PJSIP_H_ACCEPT_ENCODING_UNIMP,	/* N/A, use pjsip_generic_string_hdr */
	PJSIP_H_ACCEPT_LANGUAGE_UNIMP,	/* N/A, use pjsip_generic_string_hdr */
	PJSIP_H_ALERT_INFO_UNIMP,		/* N/A, use pjsip_generic_string_hdr */
	PJSIP_H_ALLOW,
	PJSIP_H_AUTHENTICATION_INFO_UNIMP,	/* N/A, use pjsip_generic_string_hdr */
	PJSIP_H_AUTHORIZATION,
	PJSIP_H_CALL_ID,
	PJSIP_H_CALL_INFO_UNIMP,		/* N/A, use pjsip_generic_string_hdr */
	PJSIP_H_CONTACT,
	PJSIP_H_CONTENT_DISPOSITION_UNIMP,	/* N/A, use pjsip_generic_string_hdr */
	PJSIP_H_CONTENT_ENCODING_UNIMP,	/* N/A, use pjsip_generic_string_hdr */
	PJSIP_H_CONTENT_LANGUAGE_UNIMP,	/* N/A, use pjsip_generic_string_hdr */
	PJSIP_H_CONTENT_LENGTH,
	PJSIP_H_CONTENT_TYPE,
	PJSIP_H_CSEQ,
	PJSIP_H_DATE_UNIMP,			/* N/A, use pjsip_generic_string_hdr */
	PJSIP_H_ERROR_INFO_UNIMP,		/* N/A, use pjsip_generic_string_hdr */
	PJSIP_H_EXPIRES,
	PJSIP_H_FROM,
	PJSIP_H_IN_REPLY_TO_UNIMP,		/* N/A, use pjsip_generic_string_hdr */
	PJSIP_H_MAX_FORWARDS,
	PJSIP_H_MIME_VERSION_UNIMP,		/* N/A, use pjsip_generic_string_hdr */
	PJSIP_H_MIN_EXPIRES,
	PJSIP_H_ORGANIZATION_UNIMP,		/* N/A, use pjsip_generic_string_hdr */
	PJSIP_H_PRIORITY_UNIMP,		/* N/A, use pjsip_generic_string_hdr */
	PJSIP_H_PROXY_AUTHENTICATE,
	PJSIP_H_PROXY_AUTHORIZATION,
	PJSIP_H_PROXY_REQUIRE_UNIMP,	/* N/A, use pjsip_generic_string_hdr */
	PJSIP_H_RECORD_ROUTE,
	PJSIP_H_REPLY_TO_UNIMP,		/* N/A, use pjsip_generic_string_hdr */
	PJSIP_H_REQUIRE,
	PJSIP_H_RETRY_AFTER,
	PJSIP_H_ROUTE,
	PJSIP_H_SERVER_UNIMP,		/* N/A, use pjsip_generic_string_hdr */
	PJSIP_H_SUBJECT_UNIMP,		/* N/A, use pjsip_generic_string_hdr */
	PJSIP_H_SUPPORTED,
	PJSIP_H_TIMESTAMP_UNIMP,		/* N/A, use pjsip_generic_string_hdr */
	PJSIP_H_TO,
	PJSIP_H_UNSUPPORTED,
	PJSIP_H_USER_AGENT_UNIMP,		/* N/A, use pjsip_generic_string_hdr */
	PJSIP_H_VIA,
	PJSIP_H_WARNING_UNIMP,		/* N/A, use pjsip_generic_string_hdr */
	PJSIP_H_WWW_AUTHENTICATE,

	PJSIP_H_OTHER

} pjsip_hdr_e;


/**
* This enumeration declares SIP methods as described by RFC3261. Additional
* methods do exist, and they are described by corresponding RFCs for the SIP
* extentensions. Since they won't alter the characteristic of the processing
* of the message, they don't need to be explicitly mentioned here.
*/
typedef enum pjsip_method_e
{
	PJSIP_INVITE_METHOD,    /**< INVITE method, for establishing dialogs.   */
	PJSIP_CANCEL_METHOD,    /**< CANCEL method, for cancelling request.	    */
	PJSIP_ACK_METHOD,	    /**< ACK method.				    */
	PJSIP_BYE_METHOD,	    /**< BYE method, for terminating dialog.	    */
	PJSIP_REGISTER_METHOD,  /**< REGISTER method.			    */
	PJSIP_OPTIONS_METHOD,   /**< OPTIONS method.			    */

	PJSIP_OTHER_METHOD	    /**< Other method.				    */

} pjsip_method_e;

/**
* This structure represents a SIP method.
* Application must always use either #pjsip_method_init or #pjsip_method_set
* to make sure that method name is initialized correctly. This way, the name
* member will always contain a valid method string regardless whether the ID
* is recognized or not.
*/
struct pjsip_method
{
	pjsip_method_e id;	    /**< Method ID, from \a pjsip_method_e. */
	pj_str_t	   name;    /**< Method name, which will always contain the
							method string. */
};

/** Type of data in the credential information in #pjsip_cred_info. */
typedef enum pjsip_cred_data_type
{
	PJSIP_CRED_DATA_PLAIN_PASSWD = 0, /**< Plain text password.		*/
	PJSIP_CRED_DATA_DIGEST = 1, /**< Hashed digest.			*/

	PJSIP_CRED_DATA_EXT_AKA = 16 /**< Extended AKA info is available */

} pjsip_cred_data_type;

/** Authentication's quality of protection (qop) type. */
typedef enum pjsip_auth_qop_type
{
	PJSIP_AUTH_QOP_NONE,	    /**< No quality of protection. */
	PJSIP_AUTH_QOP_AUTH,	    /**< Authentication. */
	PJSIP_AUTH_QOP_AUTH_INT,	    /**< Authentication with integrity protection. */
	PJSIP_AUTH_QOP_UNKNOWN	    /**< Unknown protection. */
} pjsip_auth_qop_type;

const pjsip_hdr_name_info_t pjsip_hdr_names[] =
{
	{ "Accept",		    6,  NULL },   // PJSIP_H_ACCEPT,
	{ "Accept-Encoding",    15, NULL },   // PJSIP_H_ACCEPT_ENCODING,
	{ "Accept-Language",    15, NULL },   // PJSIP_H_ACCEPT_LANGUAGE,
	{ "Alert-Info",	    10, NULL },   // PJSIP_H_ALERT_INFO,
	{ "Allow",		    5,  NULL },   // PJSIP_H_ALLOW,
	{ "Authentication-Info",19, NULL },   // PJSIP_H_AUTHENTICATION_INFO,
	{ "Authorization",	    13, NULL },   // PJSIP_H_AUTHORIZATION,
	{ "Call-ID",	    7,  "i" },    // PJSIP_H_CALL_ID,
	{ "Call-Info",	    9,  NULL },   // PJSIP_H_CALL_INFO,
	{ "Contact",	    7,  "m" },    // PJSIP_H_CONTACT,
	{ "Content-Disposition",19, NULL },   // PJSIP_H_CONTENT_DISPOSITION,
	{ "Content-Encoding",   16, "e" },    // PJSIP_H_CONTENT_ENCODING,
	{ "Content-Language",   16, NULL },   // PJSIP_H_CONTENT_LANGUAGE,
	{ "Content-Length",	    14, "l" },    // PJSIP_H_CONTENT_LENGTH,
	{ "Content-Type",	    12, "c" },    // PJSIP_H_CONTENT_TYPE,
	{ "CSeq",		     4, NULL },   // PJSIP_H_CSEQ,
	{ "Date",		     4, NULL },   // PJSIP_H_DATE,
	{ "Error-Info",	    10, NULL },   // PJSIP_H_ERROR_INFO,
	{ "Expires",	     7, NULL },   // PJSIP_H_EXPIRES,
	{ "From",		     4, "f" },    // PJSIP_H_FROM,
	{ "In-Reply-To",	    11, NULL },   // PJSIP_H_IN_REPLY_TO,
	{ "Max-Forwards",	    12, NULL },   // PJSIP_H_MAX_FORWARDS,
	{ "MIME-Version",	    12, NULL },   // PJSIP_H_MIME_VERSION,
	{ "Min-Expires",	    11, NULL },   // PJSIP_H_MIN_EXPIRES,
	{ "Organization",	    12, NULL },   // PJSIP_H_ORGANIZATION,
	{ "Priority",	     8, NULL },   // PJSIP_H_PRIORITY,
	{ "Proxy-Authenticate", 18, NULL },   // PJSIP_H_PROXY_AUTHENTICATE,
	{ "Proxy-Authorization",19, NULL },   // PJSIP_H_PROXY_AUTHORIZATION,
	{ "Proxy-Require",	    13, NULL },   // PJSIP_H_PROXY_REQUIRE,
	{ "Record-Route",	    12, NULL },   // PJSIP_H_RECORD_ROUTE,
	{ "Reply-To",	     8, NULL },   // PJSIP_H_REPLY_TO,
	{ "Require",	     7, NULL },   // PJSIP_H_REQUIRE,
	{ "Retry-After",	    11, NULL },   // PJSIP_H_RETRY_AFTER,
	{ "Route",		     5, NULL },   // PJSIP_H_ROUTE,
	{ "Server",		     6, NULL },   // PJSIP_H_SERVER,
	{ "Subject",	     7, "s" },    // PJSIP_H_SUBJECT,
	{ "Supported",	     9, "k" },    // PJSIP_H_SUPPORTED,
	{ "Timestamp",	     9, NULL },   // PJSIP_H_TIMESTAMP,
	{ "To",		     2, "t" },    // PJSIP_H_TO,
	{ "Unsupported",	    11, NULL },   // PJSIP_H_UNSUPPORTED,
	{ "User-Agent",	    10, NULL },   // PJSIP_H_USER_AGENT,
	{ "Via",		     3, "v" },    // PJSIP_H_VIA,
	{ "Warning",	     7, NULL },   // PJSIP_H_WARNING,
	{ "WWW-Authenticate",   16, NULL },   // PJSIP_H_WWW_AUTHENTICATE,

	{ "_Unknown-Header",    15, NULL },   // PJSIP_H_OTHER,
};


PJ_DEF_DATA(const pjsip_method) pjsip_invite_method =
{ PJSIP_INVITE_METHOD,{ "INVITE",6 } };

PJ_DEF_DATA(const pjsip_method) pjsip_cancel_method =
{ PJSIP_CANCEL_METHOD,{ "CANCEL",6 } };

PJ_DEF_DATA(const pjsip_method) pjsip_ack_method =
{ PJSIP_ACK_METHOD,{ "ACK",3 } };

PJ_DEF_DATA(const pjsip_method) pjsip_bye_method =
{ PJSIP_BYE_METHOD,{ "BYE",3 } };

PJ_DEF_DATA(const pjsip_method) pjsip_register_method =
{ PJSIP_REGISTER_METHOD,{ "REGISTER", 8 } };

PJ_DEF_DATA(const pjsip_method) pjsip_options_method =
{ PJSIP_OPTIONS_METHOD,{ "OPTIONS",7 } };


static const pj_str_t *method_names[] =
{
	&pjsip_invite_method.name,
	&pjsip_cancel_method.name,
	&pjsip_ack_method.name,
	&pjsip_bye_method.name,
	&pjsip_register_method.name,
	&pjsip_options_method.name
};

enum http_method
{
	HTTP_GET,
	HTTP_PUT,
	HTTP_DELETE
};

static const char *http_method_names[3] =
{
	"GET",
	"PUT",
	"DELETE"
};

enum http_state
{
	IDLE,
	CONNECTING,
	SENDING_REQUEST,
	SENDING_REQUEST_BODY,
	REQUEST_SENT,
	READING_RESPONSE,
	READING_DATA,
	READING_COMPLETE,
	ABORTING,
};

const pj_str_t end_hdr = { "\n\r\n", 3 };

/* SIP version */
static void parse_sip_version(pj_scanner *scanner)
{
	pj_str_t SIP = { "SIP", 3 };
	pj_str_t V2 = { "2.0", 3 };
	pj_str_t sip, version;

	pj_scan_get(scanner, &pconst.pjsip_ALPHA_SPEC, &sip);
	if (pj_scan_get_char(scanner) != '/')
		on_syntax_error(scanner);
	pj_scan_get_n(scanner, 3, &version);
	if (pj_stricmp(&sip, &SIP) || pj_stricmp(&version, &V2))
		on_syntax_error(scanner);
}

static pj_bool_t is_next_sip_version(pj_scanner *scanner)
{
	pj_str_t SIP = { "SIP", 3 };
	pj_str_t sip;
	int c;

	c = pj_scan_peek(scanner, &pconst.pjsip_ALPHA_SPEC, &sip);
	/* return TRUE if it is "SIP" followed by "/" or space.
	* we include space since the "/" may be separated by space,
	* although this would mean it would return TRUE if it is a
	* request and the method is "SIP"!
	*/
	return c && (c == '/' || c == ' ' || c == '\t') && pj_stricmp(&sip, &SIP) == 0;
}







