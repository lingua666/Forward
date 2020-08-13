
#include <libCommon/SysInfo.h>
#include <libCommon/os/Platform.h>

#if defined(PLATFORM_OS_FAMILY_WINDOWS)
#include "SysInfo_WIN32.cxx"
#elif defined(PLATFORM_OS_FAMILY_UNIX)
	#if PLATFORM_OS == PLATFORM_OS_ANDROID
		#include "SysInfo_ANDROID.cxx"
	#else
		#include "SysInfo_UNIX.cxx"
	#endif
#endif

#ifdef __cplusplus
	extern "C" {
#endif

		//获取一年中某个月的天数
		int get_days_to_month( unsigned short uYear, unsigned char uMonth )
		{
			int numberOfDays;
			if (uMonth == 4 || uMonth == 6 || uMonth == 9 || uMonth == 11)  
				numberOfDays = 30;
			else if (uMonth == 2)  
			{ 
				bool isLeapYear = (uYear % 4 == 0 && uYear % 100 != 0) || (uYear % 400 == 0); 
				if (isLeapYear)  
					numberOfDays = 29;  
				else  
					numberOfDays = 28;
			}  
			else  
				numberOfDays = 31;

			return numberOfDays;
		}

		//获取某年天数
		int get_days_to_year( unsigned short uYear )
		{
			if((uYear % 4 == 0 && uYear % 100 != 0) || uYear % 400 == 0)
				return 365;
			return 366;
		}

#ifdef  __cplusplus
	}
#endif
