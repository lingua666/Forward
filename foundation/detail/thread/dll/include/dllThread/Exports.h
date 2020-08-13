
#ifndef __EXPORTS_20160916102142_1451301281_H__
#define __EXPORTS_20160916102142_1451301281_H__


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

EXTERN_C _SYMBOL_DLL_EXPORTS char* _CALLTYPE GetMemory( void );


EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE ParseMemory( const char* Raw, const char* Delphi );


EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCSHARP_Test_1( unsigned char* sz, int uSize );


EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCSHARP_Test_2( char* sz, int uSize );


EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCSHARP_Test_3( int* iValue );


EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCSHARP_Test_4( double* dValue );

struct Test_stu
{
	char* buffer;
	int val;
	double dl;
};


EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCSHARP_Test_5( Test_stu* stu );

typedef void (_CALLTYPE *TDataEvent)(char* pData, int iLen, Test_stu* uSize);

EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCSHARP_Test_6( Test_stu* stu, TDataEvent Event );


#endif//__PLC_OMRON_20151228191441_1451301281_H__
