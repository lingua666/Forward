
#ifndef __MODAFX_20160615173305_1438860785_H__
#define __MODAFX_20160615173305_1438860785_H__

//
// Platform Identification
//
#define PLATFORM_OS_LINUX         0x0005
#define PLATFORM_OS_WINDOWS_NT    0x1001
#define PLATFORM_OS_WINDOWS_CE    0x1011
#define PLATFORM_OS_ANDROID		  0xFF00

#if defined(__Android__)
	#define PLATFORM_OS_FAMILY_UNIX 1
	#define PLATFORM_OS PLATFORM_OS_ANDROID
#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__TOS_LINUX__)
	#define PLATFORM_OS_FAMILY_UNIX 1
	#define PLATFORM_OS PLATFORM_OS_LINUX
#elif defined(_WIN32_WCE)
	#define PLATFORM_OS_FAMILY_WINDOWS 1
	#define PLATFORM_OS PLATFORM_OS_WINDOWS_CE
#elif defined(_WIN32) || defined(_WIN64)
	#define PLATFORM_OS_FAMILY_WINDOWS 1
	#define PLATFORM_OS PLATFORM_OS_WINDOWS_NT
#endif

#if __cplusplus >= 201103L
	#define PLATFORM_CXX11_SUPPORT
#endif // __cplusplus


#if !defined(__INCLUDE_TYPE_HEAD__20150828)
	#define __INCLUDE_TYPE_HEAD__20150828

	#if defined(_MSC_VER)
		//
		// Windows/Visual C++
		//
		typedef signed char            int8_t;
		typedef unsigned char          uint8_t;
		typedef signed short           int16_t;
		typedef unsigned short         uint16_t;
		typedef signed int             int32_t;
		typedef unsigned int           uint32_t;
		typedef signed __int64         int64_t;
		typedef unsigned __int64       uint64_t;
		#if defined(_WIN64)
			#define TYPES_ARCH_64_BIT	1
			typedef signed __int64     intptr_t;
			typedef unsigned __int64   uintptr_t;
		#else
			typedef __w64 int   intptr_t;
			typedef __w64 unsigned int uintptr_t;
		#endif
		#define TYPES_HAVE_INT64 1
	#elif defined(__GNUC__)
		//
		// Unix/GCC
		//
		typedef signed char            int8_t;
		typedef unsigned char          uint8_t;
		typedef signed short           int16_t;
		typedef unsigned short         uint16_t;
		typedef signed int             int32_t;
		typedef unsigned int           uint32_t;

		#if defined(__LP64__)
			#define TYPES_ARCH_64_BIT 		1
			#define TYPES_LONG_IS_64_BIT	1
			typedef signed long        int64_t;
			typedef unsigned long      uint64_t;
			
			//#if !defined(__intptr_t_defined)
				typedef signed long            intptr_t;
				typedef unsigned long          uintptr_t;
			//#endif
		#else
			typedef signed long long   int64_t;
			typedef unsigned long long uint64_t;
			
			//#if !defined(__intptr_t_defined)
				typedef signed int            intptr_t;
				typedef unsigned int          uintptr_t;
			//#endif
		#endif
		#define TYPES_HAVE_INT64 1
	#endif

	typedef	int8_t		Int8;
	typedef	uint8_t		UInt8;
	typedef	int16_t		Int16;
	typedef	uint16_t	UInt16;
	typedef	int32_t		Int32;
	typedef	uint32_t	UInt32;
	typedef	int64_t		Int64;
	typedef	uint64_t	UInt64;
	typedef	intptr_t	IntPtr;
	typedef	uintptr_t	UIntPtr;
	typedef	Int64		THandle;

	#if defined(_WIN32) 
		#if defined(_DLL_)
			#if defined(_EXPORTS_)
				#define _SYMBOL_DLL_API __declspec(dllexport)
			#else
				#define _SYMBOL_DLL_API __declspec(dllimport)	
			#endif
			#define _SYMBOL_DLL_EXPORTS __declspec(dllexport)
			#define _SYMBOL_DLL_IMPORT __declspec(dllimport)
		#endif
		#define	_CALLTYPE	__stdcall
	#else
		#define	_CALLTYPE
	#endif


	#if !defined(_SYMBOL_DLL_API)
		#define _SYMBOL_DLL_API
		#define _SYMBOL_DLL_EXPORTS
		#define _SYMBOL_DLL_IMPORT
	#endif

	#define EXTERN_C    extern "C"

	#define _OUT	
	#define _IN
#endif //__INCLUDE_TYPE_HEAD__20150828


#endif // __MODAFX_20160615173305_1438860785_H__
