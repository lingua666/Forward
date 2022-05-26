
#ifndef __SIP_SIP_SDP_20160510104022_1462848022_H__
#define __SIP_SIP_SDP_20160510104022_1462848022_H__

#include <libProto_Sip_V20/Sip_Proto_Common.h>

namespace	_sip_v20_ {


#   define SDP_FMT_MAX		32
#   define SDP_BANDW_MAX	4
#   define SDP_ATTR_MAX		(SDP_FMT_MAX * 2 + 4)
#   define SDP_MEDIA_MAX	16

	class Sip_SDP
	{
	public:
		/**
		* This structure describes SDP Session info ("i=" line).
		*/
		struct sdp_session_info
		{
			_string_t author;
			_string_t copyright;
		};

		/**
		* This structure describes SDP connection info ("c=" line).
		*/
		struct sdp_conn
		{
			_string_t	net_type;	/**< Network type ("IN").			*/
			_string_t	addr_type;	/**< Address type ("IP4", "IP6").	*/
			_string_t	addr;		/**< The address.					*/
		};

		/**
		* This structure describes SDP bandwidth info ("b=" line).
		*/
		struct sdp_bandw
		{
			_string_t	modifier;	/**< Bandwidth modifier.		*/
			UInt32		value;	    /**< Bandwidth value.	        */
		};

		/**
		* Generic representation of attribute ("a=" line).
		*/
		struct sdp_attr
		{
			_string_t		name;	    /**< Attribute name.    */
			_string_t		value;	    /**< Attribute value.   */
		};

		typedef SmartPTR<sdp_session_info>		sdp_session_info_sptr;
		typedef SmartPTR<sdp_conn>				sdp_conn_sptr;
		typedef SmartPTR<sdp_bandw>				sdp_bandw_sptr;
		typedef SmartPTR<sdp_attr>				sdp_attr_sptr;

		/**
		* This structure describes SDP media descriptor. A SDP media descriptor
		* starts with "m=" line and contains the media attributes and optional
		* connection line.
		*/
		struct sdp_media
		{
			/** Media descriptor line ("m=" line) */
			struct
			{
				_string_t		media;		/**< Media type ("audio", "video")  */
				UInt16			port;		/**< Port number.		    */
				unsigned int    port_count;		/**< Port count, used only when >2  */
				_string_t		transport;		/**< Transport ("RTP/AVP»òRTP/AVP/TCP")	*/
				unsigned int    fmt_count;		/**< Number of formats.		    */
				_string_t		fmt[SDP_FMT_MAX];       /**< Media formats.	    */
			} desc;

			sdp_conn_sptr		conn;		/**< Optional connection info.	    */
			unsigned int        bandw_count;	/**< Number of bandwidth info.	    */
			sdp_bandw_sptr		bandw[SDP_BANDW_MAX]; /**< Bandwidth info.  */
			unsigned int        attr_count;	/**< Number of attributes.	    */
			sdp_attr_sptr		attr[SDP_ATTR_MAX];   /**< Attributes.	    */
		};

		typedef SmartPTR<sdp_media>		sdp_media_sptr;

		/**
		* This structure describes SDP session description. A SDP session descriptor
		* contains complete information about a session, and normally is exchanged
		* with remote media peer using signaling protocol such as SIP.
		*/
		typedef struct sdp_session
		{
			UInt32 proto_version;	//(v= line)

			/** Session origin (o= line) */
			struct
			{
				_string_t    user;	    /**< User 				*/
				UInt32		 id;		    /**< Session ID			*/
				UInt32		 version;	    /**< Session version		*/
				_string_t    net_type;	    /**< Network type ("IN")		*/
				_string_t    addr_type;	    /**< Address type ("IP4", "IP6")	*/
				_string_t    addr;	    /**< The address.			*/
			} origin;

			_string_t			name;	    /**< Subject line (s=)		*/
			sdp_session_info_sptr	session_info;	/** Session information**/
			sdp_conn_sptr		conn;	    /**< Connection line (c=)		*/
			unsigned int	    bandw_count; /**< Number of bandwidth info (b=)	*/
			sdp_bandw_sptr	bandw[SDP_BANDW_MAX];
			/**< Bandwidth info array (b=)	*/

			/** Session time (t= line)	*/
			struct
			{
				UInt32 start;	    /**< Start time.			*/
				UInt32 stop;	    /**< Stop time.			*/
			} time;

			unsigned int	attr_count;		/**< Number of attributes.  */
			sdp_attr_sptr	attr[SDP_ATTR_MAX]; /**< Attributes array.   */

			unsigned int	media_count;		/**< Number of media.	    */
			sdp_media_sptr	media[SDP_MEDIA_MAX];	/**< Media array.   */
		}sdp_session;

		typedef Sip_SDP	self_type;

	private:
		static MemPool_type*	GetMemPoolInstance(void);

	public:

		static void Init_sdp_session(sdp_session* pSdp);

		static sdp_session_info_sptr Alloc_session_info(void);

		static sdp_conn_sptr Alloc_conn(void);

		static sdp_bandw_sptr Alloc_bandw(void);

		static sdp_attr_sptr Alloc_attr(void);

		static sdp_media_sptr Alloc_media(void);

	public:
		Sip_SDP(void);

		Sip_SDP(const self_type& t1);

		~Sip_SDP(void);

		self_type&	operator=(const self_type& t1);

		int Init(void);

		void Release(void);

		const sdp_session* Parse(const char* c_szData, UInt32 uSize);

		int ParseLine(const char* szData, int iDataSize, sdp_session* pSdp);

		void Copy(const sdp_session& Session);

		_string_t FindValueAndInc(char** szBuf, const _string_t& Start, const _string_t& End);

		void CloneMediaInfo(sdp_media* dst, sdp_media* src);

		void CloneConnInfo(sdp_conn* dst, sdp_conn* src);

		void CloneBandwInfo(sdp_bandw* dst, sdp_bandw* src);

		void CloneAttrInfo(sdp_attr* dst, sdp_attr* src);

		const sdp_session* Get(void) const;

		_string_t toString(void);

	protected:

		void CloneSdp(sdp_session* dst, sdp_session* src);

	private:
		sdp_session _Sdp;
	};

}

#endif

