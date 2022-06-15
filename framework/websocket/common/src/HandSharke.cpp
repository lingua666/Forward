
#include <libFrameWork_WebsocketCommon/HandSharke.h>

namespace _websocket_ {

	_string_t CHandSharke::GenerateKey(void)
	{
		uint32_converter conv;
		unsigned char raw_key[16];

		for (int i = 0; i < 4; i++) {
			conv.i = (UInt32)Timestamp_type().epochMicroseconds();
			std::copy(conv.c, conv.c + 4, &raw_key[i * 4]);
		}

		return  websocketpp::base64_encode(raw_key, 16).c_str();
	}


	CHandSharke::CHandSharke(void)
	{
		Init();
	}

	CHandSharke::~CHandSharke(void)
	{
		Release();
	}

	int CHandSharke::Init(void)
	{
		_Parse.Init();
		return 1;
	}

	void CHandSharke::Release(void)
	{
		_Parse.Release();
	}

	processor_errors CHandSharke::validate_handshake(const char* c_szData, UInt32 uSize)
	{
		if (_Parse.GetMethod(c_szData, uSize) != "GET")
		{
			return websocket_invalid_http_method;
		}

		if (_Parse.GetVersion(c_szData, uSize) != websocket_version)
		{
			return websocket_invalid_http_version;
		}

		// required headers
		// Host is required by HTTP/1.1
		// Connection is required by is_websocket_handshake
		// Upgrade is required by is_websocket_handshake
		const char* pKey = strstr(c_szData, "Sec-WebSocket-Key");
		if (pKey == NULL)
		{
			return websocket_missing_required_header;
		}

		return websocket_ok;
	}

	_string_t CHandSharke::process_handshake_key(const _string_t& key) const
	{
		_string_t sRet = key + websocket_handshake_guid;

		unsigned char message_digest[20];
		websocketpp::sha1::calc(sRet.c_str(), sRet.size(), message_digest);
		sRet = websocketpp::base64_encode(message_digest, 20).c_str();
		return sRet;
	}

	_string_t CHandSharke::Parse(const char* c_szData, UInt32 uSize, int* uRead)
	{
		*uRead = _Parse.Parse(c_szData, uSize);
		if (*uRead > 0)
		{
			processor_errors Ret = validate_handshake(c_szData, uSize);
			if (Ret == websocket_ok)
			{//ÓÐÐ§
				return _foundation_::ScanChar::FindValue(c_szData, "Sec-WebSocket-Key: ", "\r\n");
			}
		}

		return "";
	}

	_string_t	CHandSharke::Request(const char* c_szResource, const char* c_szHost, const char* szKey)
	{
		_string_t sReq = "GET ";
		sReq += c_szResource;
		sReq += " ";
		sReq += websocket_version;
		sReq += "\r\n";
		sReq += "Host: ";
		sReq += c_szHost;
		sReq += "\r\n";
		sReq += "Upgrade: websocket\r\n";
		sReq += "Connection: Upgrade\r\n";
		sReq += "Sec-WebSocket-Key: ";
		sReq += GenerateKey() + "\r\n";
		sReq += "Sec-WebSocket-Protocol: chat\r\n";
		sReq += "Sec-WebSocket-Version: 13\r\n";
		sReq += "Origin: http://example.com\r\n";
		sReq += "\r\n";
		return sReq;
	}

	_string_t	CHandSharke::Responds(const char* c_szKey)
	{
		_string_t sRespone = websocket_version;
		sRespone += " 101 Switching Protocols\r\n";
		sRespone += "Upgrade: websocket\r\n";
		sRespone += "Connection: Upgrade\r\n";
		sRespone += "Sec-WebSocket-Accept: ";
		sRespone += process_handshake_key(c_szKey);
		sRespone += "\r\n\r\n";
		return sRespone;
	}

	processor_errors CHandSharke::validate_handshake_response(const char* c_szData, UInt32 uSize, const char* c_szKey)
	{
		char* Buffer = (char*)c_szData;

		// A valid response has an HTTP 101 switching protocols code
		_foundation_::_string_t sValue = _foundation_::ScanChar::FindValue(Buffer, "HTTP/1.1 ", " ");
		if (sValue != "101")
		{//Switching Protocols
			return websocket_invalid_http_status;
		}

		// And the upgrade token in an upgrade header
		sValue = _foundation_::ScanChar::FindValue(Buffer, "Upgrade: ", "\r\n");
		if (sValue != websocket_upgrade_token)
		{
			return websocket_missing_required_header;
		}

		// And the websocket token in the connection header
		sValue = _foundation_::ScanChar::FindValue(Buffer, "Connection: ", "\r\n");
		if (sValue != websocket_connection_token)
		{
			return websocket_missing_required_header;
		}

		// And has a valid Sec-WebSocket-Accept value
		sValue = _foundation_::ScanChar::FindValue(Buffer, "Sec-WebSocket-Accept: ", "\r\n");
		if (sValue == "")
		{
			return websocket_missing_required_header;
		}

		if (process_handshake_key(c_szKey) != sValue)
		{
			return websocket_missing_required_header;
		}

		return websocket_ok;
	}

}

