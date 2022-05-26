
#ifndef __SIP_SIP_PARSE_20160510104022_1462848022_H__
#define __SIP_SIP_PARSE_20160510104022_1462848022_H__

#include <libProto_Sip_V20/ISip_Proto.h>

namespace	_sip_v20_ {

	class Sip_Parse
	{
	public:
		typedef ISip_Proto::Hdr_INFO_sptr							Hdr_INFO_sptr;
		typedef std::multimap<_string_t, Hdr_INFO_sptr>				Msg_Map;

	public:
		static int GetEndPos(const char* c_szBuf, UInt32 uSize);

		static bool isRespone(const char* c_szBuf, UInt32 uSize);

		/* SIP Version */
		static _string_t parse_sip_version(const char* c_szBuf, UInt32 uSize);

		/* SIP Transfer Type */
		static bool is_sip_transfer_udp(const char* c_szBuf, UInt32 uSize);

		static int GetHeadEndPos(const char* c_szBuf, UInt32 uSize);

		static _string_t GetMethod(const char* c_szBuf, UInt32 uSize);

		static _string_t GetStatueCode(const char* c_szBuf, UInt32 uSize);

		static Hdr_INFO_sptr ParseLine(const char* szData, int iDataSize, int* pReadLen);

		//"SOF":内容的起始位置			"EOF":内容的结束位置
		static _string_t	FindData(const _string_t& Content,
			const _string_t& Start, const _string_t& End);
	};
}

#endif
