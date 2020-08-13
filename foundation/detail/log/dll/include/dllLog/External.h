
#ifndef __EXTERNAL_20160616102142_3442199361_H__
#define __EXTERNAL_20160616102142_3442199361_H__

#include <string>

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

struct ILog
{
	virtual ~ILog( void ) {}

	virtual int	Open( const char* c_szPath ) = 0;

	virtual bool isOpen( void ) const = 0;

	virtual void Close( void ) = 0;

	virtual int SetLevel( unsigned char uLevel ) = 0;

	virtual void Info( const char *format, ... ) = 0;

	virtual void Warning( const char *format, ... ) = 0;

	virtual void Debug( const char *format, ... ) = 0;

	virtual void Error( const char *format, ... ) = 0;

	virtual void FATAL( const char *format, ... ) = 0;
};


#endif//__EXTERNAL_20151014105604_3442199361_H__
