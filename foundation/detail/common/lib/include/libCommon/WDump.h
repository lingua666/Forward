
#ifndef __WDUMP_20151022174359_1445507039_H__
#define __WDUMP_20151022174359_1445507039_H__

#include <libCommon/os/Platform.h>

	#if defined(PLATFORM_OS_FAMILY_WINDOWS)
		#include <libCommon/os/windowsi.h>
		#include <time.h>
		#include <DbgHelp.h>  
		#pragma comment(lib, "dbghelp.lib")

		/*!
		* @class  WDump
		* @brief   WDump
		* @{
		*/

		class WDump
		{
		public:
			static  _string_type	GetLocalTime( void );

			static	LONG __stdcall ExceptCallBack( EXCEPTION_POINTERS *pExcPointer);

		public:
			/*!
			* @function   WDump
			* @brief    
			* @return  
			*/
			WDump( void );
		
			/*!
			* @function   ~WDump
			* @brief    
			* @return  
			*/
			~WDump( void );

		};
		/** @} end WDump */

	#endif

#endif//__WDUMP_20151022174359_1445507039_H__
