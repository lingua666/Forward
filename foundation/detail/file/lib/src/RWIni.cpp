
#include <libFile/RWIni.h>
#include <libFile/CMyDirectory.h>

#if defined(PLATFORM_OS_FAMILY_UNIX)
	#include <libFile/IniParserWrap.h>
#endif

namespace	_file_{

	_string_type RWIni::GetCurrentPath( void )
	{
		return CMyDirectory::GetCurrentDirPath();
	}

	void RWIni::Write( const _string_type& sSegName,const _string_type& sKeyName,
					const _string_type& sValue, const _string_type& sFileName )
	{
		#if defined(PLATFORM_OS_FAMILY_WINDOWS)

			WritePrivateProfileStringA(sSegName.c_str(), sKeyName.c_str(),
				sValue.c_str(), sFileName.c_str());

		#elif defined(PLATFORM_OS_FAMILY_UNIX)

			INIHANDLE ini = NULL;
			ini = Ini_Load(sFileName.c_str());
			if( ini != NULL )
			{
				_string_type Key = sSegName + ":" + sKeyName;
				Ini_Set(ini, Key.c_str(), sValue.c_str());
				Ini_Freedict(ini);
			}

		#endif
	}

	_string_type	RWIni::Read( const _string_type& sSegName, const _string_type& sKeyName,
					const _string_type& sFileName )
	{
		_string_type s;
		s.resize(1024);
		#if defined(PLATFORM_OS_FAMILY_WINDOWS)

			GetPrivateProfileStringA(sSegName.c_str(), sKeyName.c_str(),
				"", (char*)s.c_str(), s.size(), sFileName.c_str());
			s.update_size(strlen(s.c_str()));

		#elif defined(PLATFORM_OS_FAMILY_UNIX)
			
			INIHANDLE ini = NULL;
			ini = Ini_Load(sFileName.c_str());
			if( ini != NULL )
			{
				_string_type Key = sSegName + ":" + sKeyName;
				s = Ini_GetString(ini, Key.c_str(), NULL);
				Ini_Freedict(ini);
				s.update_size(strlen(s.c_str()));
			}

		#endif

		return s;
	}

	int RWIni::ReadInt( const _string_type& sSegName, const _string_type& sKeyName,
					const _string_type& sFileName )
	{
		#if defined(PLATFORM_OS_FAMILY_WINDOWS)
		
			return GetPrivateProfileIntA(sSegName.c_str(), sKeyName.c_str(),
						0, sFileName.c_str());

		#elif defined(PLATFORM_OS_FAMILY_UNIX)

			INIHANDLE ini = NULL;
			ini = Ini_Load(sFileName.c_str());
			if( ini != NULL )
			{
				_string_type Key = sSegName + ":" + sKeyName;
				int i = Ini_GetInt(ini, Key.c_str(), 0);
				Ini_Freedict(ini);
				return i;
			}

		#endif
	}

} // Foundation
