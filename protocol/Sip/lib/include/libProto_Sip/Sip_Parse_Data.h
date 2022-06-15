
#ifndef __SIP_SIP_PARSE_DATA_20160510104022_1462848022_H__
#define __SIP_SIP_PARSE_DATA_20160510104022_1462848022_H__

#include <libProto_Sip/Sip_Proto_Common.h>

namespace	_sip_ {

	/******************************Sip_IParse******************************/

	class Sip_IParse
	{
	public:
		Sip_IParse(void);

		virtual ~Sip_IParse(void);
			
		const Uri_INFO&	GetFromUri(void) const;

		const Uri_INFO&	GetToUri(void) const;

		const Via_Hdr_INFO& GetFirstVia(void);

		const _string_t& GetCallID(void) const;

		const long& GetCSeq(void) const;

		const _string_t& GetMethodName(void) const;

		const unsigned char& GetMaxForwards(void) const;

		const Sip_Proto_Common&	GetProtoCommon(void) const;

	protected:
		Sip_Proto_Common	_Common;
	};



	/******************************Sip_Parse_Request******************************/

	class Sip_Parse_Request : public Sip_IParse
	{
	public:
		typedef Sip_Parse::Msg_Map			Msg_Map;
		typedef Sip_Parse::Hdr_INFO_sptr	Hdr_INFO_sptr;

	private:
		_string_t		_sMethod;
		Msg_Map			_Map;

	public:
		Sip_Parse_Request(void);

		~Sip_Parse_Request(void);

		int Init(void);

		void Release(void);

		//返回值为完整包的结束位置，包含换行符，如果存在多个包，也即为下个包的起始位置
		int Parse(const char* c_szBuf, UInt32 uSize);

		//返回值为完整包的结束位置，包含换行符，如果存在多个包，也即为下个包的起始位置
		int ParseToMap(const char* c_szBuf, UInt32 uSize, Msg_Map* pMap);

		//"SOF":内容的起始位置			"EOF":内容的结束位置
		_string_t	FindData(sip_hdr_e Hdr_e, const _string_t& Start, const _string_t& End);
	};



	/******************************Sip_Parse_Response******************************/

	class Sip_Parse_Response : public Sip_IParse
	{
	public:
		typedef Sip_Parse::Msg_Map			Msg_Map;
		typedef Sip_Parse::Hdr_INFO_sptr	Hdr_INFO_sptr;

	private:
		Msg_Map			_Map;
		sip_status_code	_Status;

	public:
		Sip_Parse_Response(void);

		~Sip_Parse_Response(void);

		int Init(void);

		void Release(void);

		//返回值为完整包的结束位置，包含换行符，如果存在多个包，也即为下个包的起始位置
		int Parse(const char* c_szBuf, UInt32 uSize);

		int ParseToMap(const char* c_szBuf, UInt32 uSize, Msg_Map* pMap);

		//"SOF":内容的起始位置			"EOF":内容的结束位置
		_string_t	FindData(sip_hdr_e Hdr_e, const _string_t& Start, const _string_t& End);
	};

}

#endif
