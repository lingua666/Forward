
#ifndef __WEBSOCEKT_HANDSHARKE_20201030183022_1438860785_H__
#define __WEBSOCEKT_HANDSHARKE_20201030183022_1438860785_H__

#include <libFrameWork_WebsocketCommon/HandSharke_Parse.h>

namespace _websocket_ {

	class CHandSharke
	{
	private:
		HandSharkeParse	_Parse;

		/// Four byte conversion union
		union uint32_converter {
			UInt32 i;
			UInt8 c[4];
		};

		static _string_t GenerateKey(void);

	public:
		CHandSharke(void);

		~CHandSharke(void);

		int Init(void);

		void Release(void);

		processor_errors validate_handshake(const char* c_szData, UInt32 uSize);

		_string_t process_handshake_key(const _string_t& key) const;

		_string_t Parse(const char* c_szData, UInt32 uSize, int* uRead);

		_string_t	Request(const char* c_szResource, const char* c_szHost, const char* szKey);

		_string_t	Responds(const char* c_szKey);

		processor_errors validate_handshake_response(const char* c_szData, UInt32 uSize, const char* c_szKey);
	};
}

#endif
