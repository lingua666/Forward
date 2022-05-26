
#ifndef __WEBSOCEKT_HANDSHARKE_PARSE_20201030183022_1438860785_H__
#define __WEBSOCEKT_HANDSHARKE_PARSE_20201030183022_1438860785_H__

#include <libFrameWork_WebsocketCommon/Common.h>

namespace _websocket_ {

	class HandSharkeParse
	{
	public:
		struct Hdr_INFO
		{
			_string_t Name;
			_string_t Content;
		};

		typedef SmartPTR<Hdr_INFO>							Hdr_INFO_sptr;
		typedef std::multimap<_string_t, Hdr_INFO_sptr>		Msg_Map;

		static Hdr_INFO_sptr	Alloc(void);

	public:
		HandSharkeParse(void);

		~HandSharkeParse(void);

		int Init(void);

		void Release(void);

		int GetHeadEndPos(const char* c_szBuf, UInt32 uSize);

		int GetEndPos(const char* c_szBuf, UInt32 uSize);

		bool isRespone(const char* c_szBuf, UInt32 uSize);

		_string_t GetVersion(const char* c_szBuf, UInt32 uSize);

		_string_t GetMethod(const char* c_szBuf, UInt32 uSize);

		int Parse(const char* c_szBuf, UInt32 uSize);

		Hdr_INFO_sptr ParseLine(const char* szData, int iDataSize, int* pReadLen);

		Hdr_INFO_sptr FindContent(const char* FieldName);

	private:
		mutable Msg_Map	_Map;
	};

}

#endif
