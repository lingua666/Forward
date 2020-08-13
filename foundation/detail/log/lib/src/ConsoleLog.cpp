
#include <libLog/ConsoleLog.h>
#include <libLog/LogImpl.hpp>
#include <libCommon/modafx.h>

/*# Windows CMD命令行 字体颜色定义 text colors
	FOREGROUND_BLACK = 0x00 # black.
	FOREGROUND_DARKBLUE = 0x01 # dark blue.
	FOREGROUND_DARKGREEN = 0x02 # dark green.
	FOREGROUND_DARKSKYBLUE = 0x03 # dark skyblue.
	FOREGROUND_DARKRED = 0x04 # dark red.
	FOREGROUND_DARKPINK = 0x05 # dark pink.
	FOREGROUND_DARKYELLOW = 0x06 # dark yellow.
	FOREGROUND_DARKWHITE = 0x07 # dark white.
	FOREGROUND_DARKGRAY = 0x08 # dark gray.
	FOREGROUND_BLUE = 0x09 # blue.
	FOREGROUND_GREEN = 0x0a # green.
	FOREGROUND_SKYBLUE = 0x0b # skyblue.
	FOREGROUND_RED = 0x0c # red.
	FOREGROUND_PINK = 0x0d # pink.
	FOREGROUND_YELLOW = 0x0e # yellow.
	FOREGROUND_WHITE = 0x0f # white.


# Windows CMD命令行 背景颜色定义 background colors
	BACKGROUND_BLUE = 0x10 # dark blue.
	BACKGROUND_GREEN = 0x20 # dark green.
	BACKGROUND_DARKSKYBLUE = 0x30 # dark skyblue.
	BACKGROUND_DARKRED = 0x40 # dark red.
	BACKGROUND_DARKPINK = 0x50 # dark pink.
	BACKGROUND_DARKYELLOW = 0x60 # dark yellow.
	BACKGROUND_DARKWHITE = 0x70 # dark white.
	BACKGROUND_DARKGRAY = 0x80 # dark gray.
	BACKGROUND_BLUE = 0x90 # blue.
	BACKGROUND_GREEN = 0xa0 # green.
	BACKGROUND_SKYBLUE = 0xb0 # skyblue.
	BACKGROUND_RED = 0xc0 # red.
	BACKGROUND_PINK = 0xd0 # pink.
	BACKGROUND_YELLOW = 0xe0 # yellow.
	BACKGROUND_WHITE = 0xf0 # white.*/

namespace	_log_{

#if defined(PLATFORM_OS_FAMILY_WINDOWS)
	const unsigned char fgColors_20160831_1646[8] = {0x00, 0x0c, 0x0a, 0x0e, 0x09, 0x0d, 0x02, 0x0f};
	const unsigned char bgColors_20160831_1646[8] = {0x00, 0xc0, 0xa0, 0xe0, 0x90, 0xd0, 0x20, 0xf0};
	HANDLE Console_GetInstance( void )
	{
		static HANDLE hConsole_20160831_1212 = GetStdHandle(STD_OUTPUT_HANDLE);
		return hConsole_20160831_1212;
	}
#elif defined(PLATFORM_OS_FAMILY_UNIX)
	const char* fgColors_20160831_1646[8] = {"\033[1;30", "\033[1;31", "\033[1;32",
													"\033[1;33", "\033[1;34", "\033[1;35",
													"\033[1;36", "\033[1;37"};

	const char* bgColors_20160831_1646[8] = {";40m", ";41m", ";42m", ";43m", ";44m", ";45m", ";46m", ";47m"};
	int fgColorsIndex_20160831_1646 = 0;
	int bgColorsIndex_20160831_1646 = 0;
	bool bEnable_20160831_1646 = false;
#endif

	void Console_SetColor( unsigned int ufgColor, unsigned int ubgColor )
	{
#if defined(PLATFORM_OS_FAMILY_WINDOWS)
		SetConsoleTextAttribute(Console_GetInstance(), fgColors_20160831_1646[ufgColor] | FOREGROUND_INTENSITY);
#elif defined(PLATFORM_OS_FAMILY_UNIX)
		fgColorsIndex_20160831_1646 = ufgColor;
		bgColorsIndex_20160831_1646 = ubgColor;
		bEnable_20160831_1646 = true;
#endif
	}

	void Console_ClearColor( void )
	{
#if defined(PLATFORM_OS_FAMILY_WINDOWS)
		SetConsoleTextAttribute(Console_GetInstance(), 0x07);
#elif defined(PLATFORM_OS_FAMILY_UNIX)
		fwrite("\033[0m", strlen("\033[0m"), 1, stdout);
		bEnable_20160831_1646 = false;
#endif
	}

	void Console_Warning( const char *format, ... )
	{
#if defined(PLATFORM_OS_FAMILY_WINDOWS)
		Console_SetColor(CONSOLE_FOREGROUND_YELLOW);
		VA_ARGS_LIST(format);
		fwrite(log_buffer, iSize, 1, stdout);
		Console_ClearColor();
#elif defined(PLATFORM_OS_FAMILY_UNIX)
		Console_SetColor(CONSOLE_FOREGROUND_YELLOW);
		VA_ARGS_LIST(format);
		if( bEnable_20160831_1646 )
		{
			_string_type s(fgColors_20160831_1646[fgColorsIndex_20160831_1646]);
			s += bgColors_20160831_1646[bgColorsIndex_20160831_1646];
			fwrite(s.c_str(), s.size(), 1, stdout);
		}
		fwrite(log_buffer, iSize, 1, stdout);
		Console_ClearColor();
#endif
	}

	void Console_Error( const char *format, ... )
	{
#if defined(PLATFORM_OS_FAMILY_WINDOWS)
		Console_SetColor(CONSOLE_FOREGROUND_RED);
		VA_ARGS_LIST(format);
		fwrite(log_buffer, iSize, 1, stdout);
		Console_ClearColor();
#elif defined(PLATFORM_OS_FAMILY_UNIX)
		Console_SetColor(CONSOLE_FOREGROUND_RED);
		VA_ARGS_LIST(format);
		if( bEnable_20160831_1646 )
		{
			_string_type s(fgColors_20160831_1646[fgColorsIndex_20160831_1646]);
			s += bgColors_20160831_1646[bgColorsIndex_20160831_1646];
			fwrite(s.c_str(), s.size(), 1, stdout);
		}
		fwrite(log_buffer, iSize, 1, stdout);
		Console_ClearColor();
#endif
	}

}// namespace	_log_

