
#ifndef __WINDOWSI_20150806193305_1438860785_H__
#define __WINDOWSI_20150806193305_1438860785_H__

	#if defined(_WIN32) || defined(_WIN64)

		#define WIN32_LEAN_AND_MEAN
		#include <windows.h>

	#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__TOS_LINUX__)

	#endif

#endif