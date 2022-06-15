
#ifndef __BYTEOP_2016010774359_1445507039_H__
#define __BYTEOP_2016010774359_1445507039_H__

#include "os/Types.h"

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

#endif//__BYTEOP_2016010774359_1445507039_H__
