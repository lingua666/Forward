
#ifndef __CONSOLELOG_20160418173313_1438860785_H__
#define __CONSOLELOG_20160418173313_1438860785_H__

#include <stdlib.h>
#include <stdio.h>

#define CONSOLE_FOREGROUND_BLACK	0
#define CONSOLE_FOREGROUND_RED		1
#define CONSOLE_FOREGROUND_GREEN	2
#define CONSOLE_FOREGROUND_YELLOW	3
#define CONSOLE_FOREGROUND_BLUE		4
#define CONSOLE_FOREGROUND_PURPLE	5
#define CONSOLE_FOREGROUND_CYAN		6
#define CONSOLE_FOREGROUND_WHITE	7

#define CONSOLE_BACKGROUND_BLACK	0
#define CONSOLE_BACKGROUND_RED		1
#define CONSOLE_BACKGROUND_GREEN	2
#define CONSOLE_BACKGROUND_YELLOW	3
#define CONSOLE_BACKGROUND_BLUE		4
#define CONSOLE_BACKGROUND_PURPLE	5
#define CONSOLE_BACKGROUND_CYAN		6
#define CONSOLE_BACKGROUND_WHITE	7

namespace	_log_{

	void Console_SetColor( unsigned int ufgColor, unsigned int ubgColor = CONSOLE_BACKGROUND_BLACK );

	void Console_ClearColor( void );

	void Console_Warning( const char *format, ... );

	void Console_Error( const char *format, ... );

}// namespace	_log_

#endif // __MODAFX_20160418173313_1438860785_H__
