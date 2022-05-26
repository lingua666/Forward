

#ifndef __CRC32_HPP_20150806193305_1438860785_H__
#define __CRC32_HPP_20150806193305_1438860785_H__

#include <libCommon/os/Types.h>

namespace _common_ {

	UInt32 crc32(UInt8 const* data, unsigned int dataLength, UInt32 initialValue = 0xFFFFFFFF);

} // namespace _common_

#endif // _BASE64_HPP_
