
#ifndef __BYTEOP_2016010774359_1445507039_H__
#define __BYTEOP_2016010774359_1445507039_H__

#include "os/Types.h"

//字节序
typedef enum enByteOrderType
{
	EN_BOT_LITTLE = 0,		//小端
	EN_BOT_BIG				//大端
}enByteOrderType;

inline bool is_LittleEndian(void)
{
	unsigned int ui = 1;

	return *reinterpret_cast<unsigned char*>(&ui) == 1;
}

inline UInt16 BigEndian16_UInt8( const UInt8* pData )
{
	UInt16 value = 0;
	value |= pData[0] << 8;
	value |= pData[1];
	return value;
}

inline UInt16 BigEndian16_UInt16( const UInt16 uData )
{
	return BigEndian16_UInt8((UInt8*)&uData);
}

inline UInt32 BigEndian24_UInt8( const UInt8* pData )
{
	UInt32 value = 0;
	value |= pData[0] << 16;
	value |= pData[1] << 8;
	value |= pData[2];
	return value;
}

inline UInt32 BigEndian32_UInt8( const UInt8* pData )
{
	UInt32 value = 0;
	value |= pData[0] << 24;
	value |= pData[1] << 16;
	value |= pData[2] << 8;
	value |= pData[3];
	return value;
}

inline UInt64 BigEndian64_UInt8( const UInt8* pData )
{
	UInt64 value = 0;
	value |= ((UInt64)pData[0]) << 56;
	value |= ((UInt64)pData[1]) << 48;
	value |= ((UInt64)pData[2]) << 40;
	value |= ((UInt64)pData[3]) << 32;
	value |= ((UInt64)pData[4]) << 24;
	value |= ((UInt64)pData[5]) << 16;
	value |= ((UInt64)pData[6]) << 8;
	value |= pData[7];
	return value;
}

inline UInt32 BigEndian32_UInt32( const UInt32 uData )
{
	return BigEndian32_UInt8((UInt8*)&uData);
}

inline _string_type BigEndian8_UInt16( const UInt16 uData )
{
	_string_type s;
	char* sz = (char*)&uData;
	s.append(1,sz[1]);
	s.append(1,sz[0]);
	return s;
}

inline _string_type BigEndian8_UInt32( const UInt32 uData )
{
	_string_type s;
	char* sz = (char*)&uData;
	s.append(1,sz[3]);
	s.append(1,sz[2]);
	s.append(1,sz[1]);
	s.append(1,sz[0]);
	return s;
}

inline _string_type BigEndian8_UInt64( const UInt64 uData )
{
	_string_type s;
	char* sz = (char*)&uData;
	s.append(1,sz[7]);
	s.append(1,sz[6]);
	s.append(1,sz[5]);
	s.append(1,sz[4]);
	s.append(1,sz[3]);
	s.append(1,sz[2]);
	s.append(1,sz[1]);
	s.append(1,sz[0]);
	return s;
}

inline UInt16 LittleEndian16_UInt8( const UInt8* pData )
{
	UInt16 value = 0;
	value |= pData[1] << 8;
	value |= pData[0];
	return value;
}

inline UInt16 LittleEndian16_UInt16( const UInt16 uData )
{
	return BigEndian16_UInt8((UInt8*)&uData);
}

inline UInt32 LittleEndian24_UInt8( const UInt8* pData )
{
	UInt32 value = 0;
	value |= pData[2] << 16;
	value |= pData[1] << 8;
	value |= pData[0];
	return value;
}

inline UInt32 LittleEndian32_UInt8( const UInt8* pData )
{
	UInt32 value = 0;
	value |= pData[3] << 24;
	value |= pData[2] << 16;
	value |= pData[1] << 8;
	value |= pData[0];
	return value;
}

inline UInt64 LittleEndian64_UInt8( const UInt8* pData )
{
	UInt64 value = 0;
	value |= ((UInt64)pData[7]) << 56;
	value |= ((UInt64)pData[6]) << 48;
	value |= ((UInt64)pData[5]) << 40;
	value |= ((UInt64)pData[4]) << 32;
	value |= ((UInt64)pData[3]) << 24;
	value |= ((UInt64)pData[2]) << 16;
	value |= ((UInt64)pData[1]) << 8;
	value |= pData[0];
	return value;
}

inline UInt32 LittleEndian32_UInt32( const UInt32 uData )
{
	return LittleEndian32_UInt8((UInt8*)&uData);
}

inline _string_type LittleEndian8_UInt16( const UInt16 uData )
{
	_string_type s;
	char* sz = (char*)&uData;
	s.append(1,sz[0]);
	s.append(1,sz[1]);
	return s;
}

inline _string_type LittleEndian8_UInt32( const UInt32 uData )
{
	_string_type s;
	char* sz = (char*)&uData;
	s.append(1,sz[0]);
	s.append(1,sz[1]);
	s.append(1,sz[2]);
	s.append(1,sz[3]);
	return s;
}

inline _string_type LittleEndian8_UInt64( const UInt64 uData )
{
	_string_type s;
	char* sz = (char*)&uData;
	s.append(1,sz[0]);
	s.append(1,sz[1]);
	s.append(1,sz[2]);
	s.append(1,sz[3]);
	s.append(1,sz[4]);
	s.append(1,sz[5]);
	s.append(1,sz[6]);
	s.append(1,sz[7]);
	return s;
}

inline UInt8 Bit_UInt8( UInt8 uData, UInt8 uStart, UInt8 uLen )
{
	UInt8 flag = 0xFF >> uStart ;
	uData = uData & flag;
	return uData >> (8 - (uStart + uLen) );
}

inline UInt16 Bit_UInt16( UInt16 uData, UInt8 uStart = 0, UInt8 uLen = 16 )
{
	UInt16 flag = 0xFFFF >> uStart ;
	uData = uData & flag;
	return uData >> (16 - (uStart + uLen) );
}

inline UInt32 Bit_UInt32( UInt32 uData, UInt8 uStart = 0, UInt8 uLen = 32 )
{
	UInt32 flag = 0xFFFFFFFF >> uStart ;
	uData = uData & flag;
	return uData >> (32 - (uStart + uLen) );
}

inline UInt16 Bit_BigEndian_Buf2UInt16( UInt8* pData, UInt8 uStart = 0, UInt8 uLen = 16 )
{
	UInt16 uData = BigEndian16_UInt8(pData);
	UInt16 flag = 0xFFFF >> uStart ;
	uData = uData & flag;
	return uData >> (16 - (uStart + uLen) ); 
}

inline UInt32 Bit_BigEndian_Buf2UInt32( UInt8* pData, UInt8 uStart = 0, UInt8 uLen = 32 )
{
	UInt32 uData = BigEndian32_UInt8(pData);
	UInt32 flag = 0xFFFFFFFF >> uStart ;
	uData = uData & flag;
	return uData >> (32 - (uStart + uLen) );
}

inline UInt16 LittleToBigEndian16(const UInt16 Little)
{
	UInt8* uData = (UInt8*)&Little;
	UInt16 value = 0;
	value |= ((UInt16)uData[1]) << 8;
	value |= uData[0];
	return value;
}

inline UInt32 LittleToBigEndian32(const UInt32 Little)
{
	UInt8* uData = (UInt8*)&Little;
	UInt32 value = 0;
	value |= ((UInt32)uData[3]) << 24;
	value |= ((UInt32)uData[2]) << 16;
	value |= ((UInt32)uData[1]) << 8;
	value |= uData[0];
	return value;
}

inline UInt64 LittleToBigEndian64(const UInt64 Little)
{
	UInt8* uData = (UInt8*)&Little;
	UInt64 value = 0;
	value |= ((UInt64)uData[7]) << 56;
	value |= ((UInt64)uData[6]) << 48;
	value |= ((UInt64)uData[5]) << 40;
	value |= ((UInt64)uData[4]) << 32;
	value |= ((UInt64)uData[3]) << 24;
	value |= ((UInt64)uData[2]) << 16;
	value |= ((UInt64)uData[1]) << 8;
	value |= uData[0];
	return value;
}

inline UInt16 BigToLittleEndian16(const UInt16 Big)
{
	UInt8* uData = (UInt8*)&Big;
	UInt16 value = 0;
	value |= ((UInt16)uData[0]) << 8;
	value |= uData[1];
	return value;
}

inline UInt32 BigToLittleEndian32(const UInt32 Big)
{
	UInt8* uData = (UInt8*)&Big;
	UInt32 value = 0;
	value |= ((UInt32)uData[0]) << 24;
	value |= ((UInt32)uData[1]) << 16;
	value |= ((UInt32)uData[2]) << 8;
	value |= uData[3];
	return value;
}

inline UInt64 BigToLittleEndian64(const UInt64 Big)
{
	UInt8* uData = (UInt8*)&Big;
	UInt64 value = 0;
	value |= ((UInt64)uData[0]) << 56;
	value |= ((UInt64)uData[1]) << 48;
	value |= ((UInt64)uData[2]) << 40;
	value |= ((UInt64)uData[3]) << 32;
	value |= ((UInt64)uData[4]) << 24;
	value |= ((UInt64)uData[5]) << 16;
	value |= ((UInt64)uData[6]) << 8;
	value |= uData[7];
	return value;
}

//u32：待转换内容			Type：待转换内容字节序
inline UInt16 Conversion16ToByteOrder(UInt16 uValue, enByteOrderType Type = EN_BOT_LITTLE)
{
	switch (Type)
	{
	case EN_BOT_LITTLE:
		if (!is_LittleEndian())
		{//大端
			return LittleToBigEndian16(uValue);
		}
		break;
	case EN_BOT_BIG:
		if (is_LittleEndian())
		{//大端
			return BigToLittleEndian16(uValue); 
		}
		break;
	}

	return uValue;
}

//u32：待转换内容			Type：转换结果字节序
inline UInt32 Conversion32ToByteOrder(UInt32 uValue, enByteOrderType Type = EN_BOT_LITTLE)
{
	switch (Type)
	{
	case EN_BOT_LITTLE:
		if (!is_LittleEndian())
		{//大端
			return LittleToBigEndian32(uValue);
		}
		break;
	case EN_BOT_BIG:
		if (is_LittleEndian())
		{//小端
			return BigToLittleEndian32(uValue);
		}
		break;
	}

	return uValue;
}

//u32：待转换内容			Type：转换结果字节序
inline UInt64 Conversion64ToByteOrder(UInt64 uValue, enByteOrderType Type = EN_BOT_LITTLE)
{
	switch (Type)
	{
	case EN_BOT_LITTLE:
		if (!is_LittleEndian())
		{//大端
			return LittleToBigEndian64(uValue);
		}
		break;
	case EN_BOT_BIG:
		if (is_LittleEndian())
		{//小端
			return BigToLittleEndian64(uValue);
		}
		break;
	}

	return uValue;
}

#endif//__BYTEOP_2016010774359_1445507039_H__
