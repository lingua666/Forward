
#ifndef __WEBSOCEKT_COMMON_20201030183022_1438860785_H__
#define __WEBSOCEKT_COMMON_20201030183022_1438860785_H__

#include "External.h"
#include <libFrameWork_WebsocketCommon/sha1/sha1.hpp>
#include <libFrameWork_WebsocketCommon/base64/base64.hpp>

namespace _websocket_ {

	typedef _foundation_::_string_t		_string_t;
	typedef _foundation_::_string_big_t	_string_big_t;

	enum value
	{
		BAD_REQUEST = 0, // Error was the result of improperly formatted user input
		INTERNAL_ERROR = 1, // Error was a logic error internal to WebSocket++
		PROTOCOL_VIOLATION = 2,
		MESSAGE_TOO_BIG = 3,
		PAYLOAD_VIOLATION = 4 // Error was due to receiving invalid payload data
	};

	enum processor_errors
	{
		websocket_ok,

		/// Catch-all error for processor policy errors that don't fit in other
		/// categories
		websocket_general = 1,

		/// Error was the result of improperly formatted user input
		websocket_bad_request,

		/// Processor encountered a protocol violation in an incoming message
		websocket_protocol_violation,

		/// Processor encountered a message that was too large
		websocket_message_too_big,

		/// Processor encountered invalid payload data.
		websocket_invalid_payload,

		/// The processor method was called with invalid arguments
		websocket_invalid_arguments,

		/// Opcode was invalid for requested operation
		websocket_invalid_opcode,

		/// Control frame too large
		websocket_control_too_big,

		/// Illegal use of reserved bit
		websocket_invalid_rsv_bit,

		/// Fragmented control message
		websocket_fragmented_control,

		/// Continuation without message
		websocket_invalid_continuation,

		/// Clients may not send unmasked frames
		websocket_masking_required,

		/// Servers may not send masked frames
		websocket_masking_forbidden,

		/// Payload length not minimally encoded
		websocket_non_minimal_encoding,

		/// Not supported on 32 bit systems
		websocket_requires_64bit,

		/// Invalid UTF-8 encoding
		websocket_invalid_utf8,

		/// Operation required not implemented functionality
		websocket_not_implemented,

		/// Invalid HTTP method
		websocket_invalid_http_method,

		/// Invalid HTTP version
		websocket_invalid_http_version,

		/// Invalid HTTP status
		websocket_invalid_http_status,

		/// Missing Required Header
		websocket_missing_required_header,

		/// Embedded SHA-1 library error
		websocket_sha1_library,

		/// No support for this feature in this protocol version.
		websocket_no_protocol_support,

		/// Reserved close code used
		websocket_reserved_close_code,

		/// Invalid close code used
		websocket_invalid_close_code,

		/// Using a reason requires a close code
		websocket_reason_requires_code,

		/// Error parsing subprotocols
		websocket_subprotocol_parse_error,

		/// Error parsing extensions
		websocket_extension_parse_error,

		/// Extension related operation was ignored because extensions are disabled
		websocket_extensions_disabled,

		/// Short Ke3 read. Hybi00 requires a third key to be read from the 8 bytes
		/// after the handshake. Less than 8 bytes were read.
		websocket_short_key3,

		websocket_Unknown,
	};

#pragma pack(push, 1)
	typedef struct Frame_Head
	{
		UInt8 opcode : 4;	//帧类型
							//0x0: Continuation Frame		0x1:	Text Frame
							//0x2: Binary Frame				0x8:	Connection Close Frame
							//0x9: Ping Frame				0xA: Pong Frame
							//0x3 - 0x7 ：保留，for non-control frame
							//0xB - 0xF ：保留，for control - frame
		UInt8 RSV3 : 1;	//保留
		UInt8 RSV2 : 1;	//保留
		UInt8 RSV1 : 1;	//保留
		UInt8 FIN : 1;	//表示信息的最后一帧
	}Frame_Head;


	typedef struct Frame_Length
	{
		UInt8 Payload : 7;	//如果是 0~125，就是净荷长度
							//如果是 126，则接下来 2 字节表示的 16 位无符号整数才是这一帧的长度
							//如果是 127，则接下来 8 字节表示的 64 位无符号整数才是这一帧的长度
		UInt8 MASK : 1;
	}Frame_Length;


	typedef struct Frame_PayLoad
	{
		UInt64 uLen;	 //数据长度
		char Masking[4]; //掩码
		char* pData;	//数据内容
		bool isFin;		//是否结束
		UInt8 Code;		//帧类型
	}Frame_PayLoad;

	union uint32_converter {
		UInt32 i;
		UInt8 c[4];
	};

#pragma pack(pop)

	/*在请求中的Sec-WebSocket-Key是随机的，服务器端会用这些数据来构造出一个SHA-1的信息摘要。
	把Sec-WebSocket-Key加上一个魔幻字符串258EAFA5-E914-47DA-95CA-C5AB0DC85B11。
	使用 SHA-1 加密，之后进行 BASE-64编码，将结果作为 Sec-WebSocket-Accept 头的值，返回给客户端。*/

	static char const websocket_version[] = "HTTP/1.1";
	static char const websocket_upgrade_token[] = "websocket";
	static char const websocket_connection_token[] = "Upgrade";
	static char const websocket_handshake_guid[] = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

	const _string_t websocket_errors_string[] =
	{
		"OK", //ok
		"Generic processor error",	// websocket_general
		"invalid user input",		// websocket_bad_request
		"Generic protocol violation", // websocket_protocol_violation
		"A message was too large",	// websocket_message_too_big
		"A payload contained invalid data", // websocket_invalid_payload
		"invalid function arguments", // websocket_invalid_arguments
		"invalid opcode", // websocket_invalid_opcode
		"Control messages are limited to fewer than 125 characters", // websocket_control_too_big
		"Invalid use of reserved bits", // websocket_invalid_rsv_bit
		"Control messages cannot be fragmented", // websocket_fragmented_control
		"Invalid message continuation", // websocket_invalid_continuation
		"Clients may not send unmasked frames", // websocket_masking_required
		"Servers may not send masked frames", // websocket_masking_forbidden
		"Payload length was not minimally encoded", // websocket_non_minimal_encoding
		"64 bit frames are not supported on 32 bit systems", // websocket_requires_64bit
		"Invalid UTF8 encoding", // websocket_invalid_utf8
		"Operation required not implemented functionality", // websocket_not_implemented
		"Invalid HTTP method.", // websocket_invalid_http_method
		"Invalid HTTP version.", // websocket_invalid_http_version
		"Invalid HTTP status.", // websocket_invalid_http_status
		"A required HTTP header is missing", // websocket_missing_required_header
		"SHA-1 library error", // websocket_sha1_library
		"The WebSocket protocol version in use does not support this feature", // websocket_no_protocol_support
		"Reserved close code used", // websocket_reserved_close_code
		"Invalid close code used", // websocket_invalid_close_code
		"Using a close reason requires a valid close code", // websocket_reason_requires_code
		"Error parsing subprotocol header", // websocket_subprotocol_parse_error
		"Error parsing extension header", // websocket_extension_parse_error
		"Extensions are disabled", // websocket_extensions_disabled
		"Short Hybi00 Key 3 read", // websocket_short_key3
	};

	static _string_t message(int value)
	{
		if (value <= websocket_short_key3)
			return websocket_errors_string[value];

		return "Unknown";
	}

	static void byte_mask(char* InOut_Data, UInt64 uSize, const char Mask[4])
	{
		for (int i = 0; i < uSize; i++)
		{
			InOut_Data[i] = InOut_Data[i] ^ Mask[(i % 4)];
		}
	}

	static _string_t get_mask_key(void)
	{
		uint32_converter conv;

		unsigned char raw_key[16];

		for (int i = 0; i < 4; i++) {
			conv.i = (UInt32)Timestamp_type().epochMicroseconds();
			std::copy(conv.c, conv.c + 4, &raw_key[i * 4]);
		}

		return  websocketpp::base64_encode(raw_key, 16).c_str();
	}

	static Int64 Websocket_Frame_Parse(const char* c_szData, UInt64 uSize, Frame_PayLoad* pPayLoad)
	{
		if (uSize > 3)
		{
			UInt64 uCount = 0, uLen = 0, uOffset = 0;
			Frame_Head	Head = *((Frame_Head*)c_szData);
			Frame_Length Length = *((Frame_Length*)&c_szData[sizeof(Frame_Head)]);

			pPayLoad->Code = Head.opcode;

			if (Length.Payload < 126)
			{
				uLen = Length.Payload;
				if (Length.MASK == 1)
				{
					uOffset = sizeof(Frame_Head) + sizeof(Frame_Length) + 4;
					memcpy(pPayLoad->Masking, &c_szData[uOffset - 4], sizeof(pPayLoad->Masking));
				}
				else
				{
					uOffset = sizeof(Frame_Head) + sizeof(Frame_Length);
				}

			}
			else if (Length.Payload == 126)
			{
				uLen = *((UInt16*)&c_szData[sizeof(Frame_Head) + sizeof(Frame_Length)]);
				uLen = Conversion16ToByteOrder(uLen, enByteOrderType::EN_BOT_BIG);

				if (Length.MASK == 1)
				{
					uOffset = sizeof(Frame_Head) + sizeof(Frame_Length) + sizeof(UInt16) + 4;
					memcpy(pPayLoad->Masking, &c_szData[uOffset - 4], sizeof(pPayLoad->Masking));
				}
				else
				{
					uOffset = sizeof(Frame_Head) + sizeof(Frame_Length) + sizeof(UInt16);
				}
			}
			else if (Length.Payload == 127)
			{
				uLen = *((UInt64*)&c_szData[sizeof(Frame_Head) + sizeof(Frame_Length)]);
				uLen = Conversion64ToByteOrder(uLen, enByteOrderType::EN_BOT_BIG);

				if (Length.MASK == 1)
				{
					uOffset = sizeof(Frame_Head) + sizeof(Frame_Length) + sizeof(UInt64) + 4;
					memcpy(pPayLoad->Masking, &c_szData[uOffset - 4], sizeof(pPayLoad->Masking));
				}
				else
				{
					uOffset = sizeof(Frame_Head) + sizeof(Frame_Length) + sizeof(UInt64);
				}
			}

			uCount = uLen + uOffset;

			if (uCount <= uSize)
			{
				pPayLoad->isFin = Head.FIN == 1 ? true : false;
				pPayLoad->uLen = uLen;
				pPayLoad->pData = (char*)&c_szData[uOffset];
				byte_mask(pPayLoad->pData, pPayLoad->uLen, pPayLoad->Masking);
				return uCount;
			}
		}

		return -1;
	}
}

#endif
