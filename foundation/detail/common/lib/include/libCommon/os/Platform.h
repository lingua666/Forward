//
// Platform.h
//
// $Id: //poco/1.3/Foundation/include/Poco/Platform.h#4 $
//
// Library: Foundation
// Package: Core
// Module:  Platform
//
// Platform and architecture identification macros.
//
// NOTE: This file may be included from both C++ and C code, so it
//       must not contain any C++ specific things.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef __PLATFORM_20151127152350_1448609030_H__
#define __PLATFORM_20151127152350_1448609030_H__

//
// Platform Identification
//
#define PLATFORM_OS_FREE_BSD      0x0001
#define PLATFORM_OS_AIX           0x0002
#define PLATFORM_OS_HPUX          0x0003
#define PLATFORM_OS_TRU64         0x0004
#define PLATFORM_OS_LINUX         0x0005
#define PLATFORM_OS_MAC_OS_X      0x0006
#define PLATFORM_OS_NET_BSD       0x0007
#define PLATFORM_OS_OPEN_BSD      0x0008
#define PLATFORM_OS_IRIX          0x0009
#define PLATFORM_OS_SOLARIS       0x000a
#define PLATFORM_OS_QNX           0x000b
#define PLATFORM_OS_VXWORKS       0x000c
#define PLATFORM_OS_CYGWIN        0x000d
#define PLATFORM_OS_UNKNOWN_UNIX  0x00ff
#define PLATFORM_OS_WINDOWS_NT    0x1001
#define PLATFORM_OS_WINDOWS_CE    0x1011
#define PLATFORM_OS_VMS           0x2001
#define PLATFORM_OS_ANDROID		  0xFF00

#if defined(__Android__)
	#define PLATFORM_OS_FAMILY_UNIX 1
	#define PLATFORM_OS PLATFORM_OS_ANDROID
#elif defined(__FreeBSD__)
	#define PLATFORM_OS_FAMILY_UNIX 1
	#define PLATFORM_OS_FAMILY_BSD 1
	#define PLATFORM_OS PLATFORM_OS_FREE_BSD
#elif defined(_AIX) || defined(__TOS_AIX__)
	#define PLATFORM_OS_FAMILY_UNIX 1
	#define PLATFORM_OS PLATFORM_OS_AIX
#elif defined(hpux) || defined(_hpux)
	#define PLATFORM_OS_FAMILY_UNIX 1
	#define PLATFORM_OS PLATFORM_OS_HPUX
#elif defined(__digital__) || defined(__osf__)
	#define PLATFORM_OS_FAMILY_UNIX 1
	#define PLATFORM_OS PLATFORM_OS_TRU64
#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__TOS_LINUX__)
	#define PLATFORM_OS_FAMILY_UNIX 1
	#define PLATFORM_OS PLATFORM_OS_LINUX
#elif defined(__APPLE__) || defined(__TOS_MACOS__)
	#define PLATFORM_OS_FAMILY_UNIX 1
	#define PLATFORM_OS_FAMILY_BSD 1
	#define PLATFORM_OS PLATFORM_OS_MAC_OS_X
#elif defined(__NetBSD__)
	#define PLATFORM_OS_FAMILY_UNIX 1
	#define PLATFORM_OS_FAMILY_BSD 1
	#define PLATFORM_OS PLATFORM_OS_NET_BSD
#elif defined(__OpenBSD__)
	#define PLATFORM_OS_FAMILY_UNIX 1
	#define PLATFORM_OS_FAMILY_BSD 1
	#define PLATFORM_OS PLATFORM_OS_OPEN_BSD
#elif defined(sgi) || defined(__sgi)
	#define PLATFORM_OS_FAMILY_UNIX 1
	#define PLATFORM_OS PLATFORM_OS_IRIX
#elif defined(sun) || defined(__sun)
	#define PLATFORM_OS_FAMILY_UNIX 1
	#define PLATFORM_OS PLATFORM_OS_SOLARIS
#elif defined(__QNX__)
	#define PLATFORM_OS_FAMILY_UNIX 1
	#define PLATFORM_OS PLATFORM_OS_QNX
#elif defined(unix) || defined(__unix) || defined(__unix__)
	#define PLATFORM_OS_FAMILY_UNIX 1
	#define PLATFORM_OS PLATFORM_OS_UNKNOWN_UNIX
#elif defined(_WIN32_WCE)
	#define PLATFORM_OS_FAMILY_WINDOWS 1
	#define PLATFORM_OS PLATFORM_OS_WINDOWS_CE
#elif defined(_WIN32) || defined(_WIN64)
	#define PLATFORM_OS_FAMILY_WINDOWS 1
	#define PLATFORM_OS PLATFORM_OS_WINDOWS_NT
#elif defined(__CYGWIN__)
	#define PLATFORM_OS_FAMILY_UNIX 1
	#define PLATFORM_OS PLATFORM_OS_CYGWIN
#elif defined(__VMS)
	#define PLATFORM_OS_FAMILY_VMS 1
	#define PLATFORM_OS PLATFORM_OS_VMS
#endif


//
// Hardware Architecture and Byte Order
//
#define PLATFORM_ARCH_ALPHA   0x01
#define PLATFORM_ARCH_IA32    0x02
#define PLATFORM_ARCH_IA64    0x03
#define PLATFORM_ARCH_MIPS    0x04
#define PLATFORM_ARCH_HPPA    0x05
#define PLATFORM_ARCH_PPC     0x06
#define PLATFORM_ARCH_POWER   0x07
#define PLATFORM_ARCH_SPARC   0x08
#define PLATFORM_ARCH_AMD64   0x09
#define PLATFORM_ARCH_ARM     0x0a
#define PLATFORM_ARCH_M68K    0x0b
#define PLATFORM_ARCH_S390    0x0c


#if defined(__ALPHA) || defined(__alpha) || defined(__alpha__) || defined(_M_ALPHA)
	#define PLATFORM_ARCH PLATFORM_ARCH_ALPHA
	#define PLATFORM_ARCH_LITTLE_ENDIAN 1
#elif defined(i386) || defined(__i386) || defined(__i386__) || defined(_M_IX86)
	#define PLATFORM_ARCH PLATFORM_ARCH_IA32
	#define PLATFORM_ARCH_LITTLE_ENDIAN 1
#elif defined(_IA64) || defined(__IA64__) || defined(__ia64__) || defined(__ia64) || defined(_M_IA64)
	#define PLATFORM_ARCH PLATFORM_ARCH_IA64
	#if defined(hpux) || defined(_hpux)
		#define PLATFORM_ARCH_BIG_ENDIAN 1
	#else
		#define PLATFORM_ARCH_LITTLE_ENDIAN 1
	#endif
#elif defined(__x86_64__) || defined(_M_X64)
	#define PLATFORM_ARCH PLATFORM_ARCH_AMD64
	#define PLATFORM_ARCH_LITTLE_ENDIAN 1
#elif defined(__mips__) || defined(__mips) || defined(__MIPS__) || defined(_M_MRX000)
	#define PLATFORM_ARCH PLATFORM_ARCH_MIPS
	#define PLATFORM_ARCH_BIG_ENDIAN 1
#elif defined(__hppa) || defined(__hppa__)
	#define PLATFORM_ARCH PLATFORM_ARCH_HPPA
	#define PLATFORM_ARCH_BIG_ENDIAN 1
#elif defined(__PPC) || defined(__POWERPC__) || defined(__powerpc) || defined(__PPC__) || \
      defined(__powerpc__) || defined(__ppc__) || defined(_ARCH_PPC) || defined(_M_PPC)
	#define PLATFORM_ARCH PLATFORM_ARCH_PPC
	#define PLATFORM_ARCH_BIG_ENDIAN 1
#elif defined(_POWER) || defined(_ARCH_PWR) || defined(_ARCH_PWR2) || defined(_ARCH_PWR3) || \
      defined(_ARCH_PWR4) || defined(__THW_RS6000)
	#define PLATFORM_ARCH PLATFORM_ARCH_POWER
	#define PLATFORM_ARCH_BIG_ENDIAN 1
#elif defined(__sparc__) || defined(__sparc) || defined(sparc)
	#define PLATFORM_ARCH PLATFORM_ARCH_SPARC
	#define PLATFORM_ARCH_BIG_ENDIAN 1
#elif defined(__arm__) || defined(__arm) || defined(ARM) || defined(_ARM_) || defined(__ARM__) || defined(_M_ARM)
	#define PLATFORM_ARCH PLATFORM_ARCH_ARM
	#if defined(__ARMEB__)
		#define PLATFORM_ARCH_BIG_ENDIAN 1
	#else
		#define PLATFORM_ARCH_LITTLE_ENDIAN 1
	#endif
#elif defined(__m68k__)
    #define PLATFORM_ARCH PLATFORM_ARCH_M68K
    #define PLATFORM_ARCH_BIG_ENDIAN 1
#elif defined(__s390__)
    #define PLATFORM_ARCH PLATFORM_ARCH_S390
    #define PLATFORM_ARCH_BIG_ENDIAN 1
#endif

#if __cplusplus >= 201103L
	#define PLATFORM_CXX11_SUPPORT
#endif // __cplusplus


//�ֽڶ��뷽ʽ
#if defined( PLATFORM_OS_FAMILY_UNIX )
	#define	__ALIGNED_IMPL__(n)		__attribute__ ((aligned (n)))	//[nΪ��С������ֽ���]
	#define	__ALIGNED_PACKED_IMPL__	__attribute__ ((__packed__))	//��ʹ����С���롱��ʽ�����Ա������ֽڶ��룬��������λ����
#else
	#define	__ALIGNED_IMPL__(n)
	#define	__ALIGNED_PACKED_IMPL__
#endif

#define __M_ALIGNED__(n)		__ALIGNED_IMPL__(n)
#define __M_ALIGNED_PACKED__	__ALIGNED_PACKED_IMPL__

#if defined(PLATFORM_OS_FAMILY_WINDOWS)
	#include <libCommon/os/Platform_WIN32.h>
	#include <libCommon/os/windowsi.h>
	#if _MSC_VER < 1900 // 1900 == vs2005
		#define snprintf _snprintf
	#endif
#elif defined(PLATFORM_OS_FAMILY_VMS)
	#include <libCommon/os/Platform_VMS.h>
#elif defined(PLATFORM_OS_FAMILY_UNIX)
	#include <libCommon/os/Platform_POSIX.h>
#endif

#define HIGH_STRING_TYPE
//#define STD_STRING_TYPE

#if defined(HIGH_STRING_TYPE)
	#include <libString_v20/string.hpp>
	typedef	_string_v20_::string	_string_type;
	typedef	_string_v20_::size_type	_size_type;

#elif defined(STD_STRING_TYPE)
	#include <string>
	typedef	std::string		_string_type;
	typedef	std::size_type	_size_type;

#else
	#include <libString/string.hpp>
	typedef	_string_::string	_string_type;
	typedef	_string_::size_type	_size_type;
#endif

#endif // __CUSTOM_PLATFORM_HEAD_DEF__