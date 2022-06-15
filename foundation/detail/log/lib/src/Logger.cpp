
#include <libLog/Logger.h>

namespace	_log_{

	LogIOStd::LogIOStd( void )
	{

	}

	LogIOStd::~LogIOStd( void )
	{

	}

	_string_type LogIOStd::GetTimer( void )
	{
		struct tm timeinfo = { 0 };
		Timestamp_type().epochTM(&timeinfo);
		char szTime[100] = {0};
		sprintf ( szTime,"[%04d-%02d-%02d %02d:%02d:%02d]", 
			timeinfo.tm_year + 1900, timeinfo.tm_mon + 1,
			timeinfo.tm_mday,timeinfo.tm_hour,
			timeinfo.tm_min,timeinfo.tm_sec);
		return szTime;
	}

	void LogIOStd::Print( const char* c_szMessage, const char* sz_Remark )
	{
		_string_type sDateTime;
		_Lock.Lock();
		sDateTime = GetTimer();
		fwrite("\n", strlen("\n"), 1, stdout);
		fwrite(sDateTime.c_str(), sDateTime.size(), 1, stdout);
		fwrite("  ",strlen("  "), 1, stdout);
		if( sz_Remark != NULL )
		{
			fwrite(sz_Remark, strlen(sz_Remark), 1, stdout);
			fwrite("  ", strlen("  "), 1, stdout);
		}
		fwrite(c_szMessage, strlen(c_szMessage), 1, stdout);
		fwrite("\n", strlen("\n"), 1, stdout);
		_Lock.UnLock();
	}


	LogFile_Date::LogFile_Date( void )
	{
		memset(&_CreateTime, 0, sizeof(_CreateTime));
	}

	LogFile_Date::LogFile_Date( const char* szFile )
	{
		memset(&_CreateTime, 0, sizeof(_CreateTime));
		Open(szFile);
	}

	LogFile_Date::~LogFile_Date( void )
	{
		Close();
	}

	void	LogFile_Date::Close( void )
	{
		if( _File.is_open() )
			_File.Close();
	}

	int LogFile_Date::OpenLog( const char* szFileName )
	{
		if( szFileName == NULL )
			return -1;

		char szFilePath[1024] = {0};
		char szDir[1024] = {0};
		sprintf(szDir,"%sLog%s", _file_::CMyDirectory::GetSplit(), _file_::CMyDirectory::GetCurrentDirPath().c_str());
		_file_::CMyDirectory::Create(szDir);
		sprintf(szFilePath,"%s%s",szDir, szFileName);
		_sFilePath = szFilePath;
		_File.Close();
		if( -1 == _File.Open(szFilePath, "a+") ) 
			return -1;

		_File.GetCreateTime(&_CreateTime);
		return 1;
	}

	int LogFile_Date::Open( const char* c_szFullPath )
	{
		if( c_szFullPath == NULL )
			return -1;

		_sFilePath = c_szFullPath;
		_file_::CMyDirectory::Create( _file_::CMyDirectory::GetDirPath(c_szFullPath).c_str() );
		_File.Close();
		if( -1 == _File.Open(c_szFullPath, "a+") )
		{
			printf("LogFile_Date::Open is Failed! Path:%s", c_szFullPath);
			return -1;
		}

		_File.GetCreateTime(&_CreateTime);
		return 1;
	}

	void LogFile_Date::Print( const char* c_szMessage, const char* sz_Remark )
	{
		if( _File.is_open() )
		{
			_string_type sDateTime;
			_Lock.Lock();
			if( CheckFile() )
			{
				sDateTime = GetTimer();
				_File.Write("\n", strlen("\n"));
				_File.Write(sDateTime.c_str(),sDateTime.size());
				_File.Write("  ",strlen("  "));
				if( sz_Remark != NULL )
				{
					_File.Write(sz_Remark,strlen(sz_Remark));
					_File.Write("  ",strlen("  "));
				}
				_File.Write(c_szMessage,strlen(c_szMessage));
				_File.Write("\n", strlen("\n"));
				_File.Flush();
			}
			_Lock.UnLock();
		}
	}

	bool LogFile_Date::CheckFile( void )
	{
		if( _CreateTime.tm_year == 0 )
			return false;

		Timestamp_type stamp;
		tm  NewTime = {0};
		stamp.epochTM(&NewTime);

		if( _CreateTime.tm_year != NewTime.tm_year || 
			_CreateTime.tm_mon != NewTime.tm_mon || 
			_CreateTime.tm_mday != NewTime.tm_mday )
		{
			if( _File.is_open() )
			{
				_File.Close();
				_string_type sDate = Timestamp_type().DateString() + ".txt";
				_string_type sPath = _file_::CMyDirectory::GetDirPath(_sFilePath) + _file_::CMyDirectory::GetSplit() + sDate;
				_sFilePath = sPath;
			}

			if ( -1 == _File.Open(_sFilePath.c_str(), "w"))
			{
				return false;
			}

			memcpy(&_CreateTime, &NewTime, sizeof(tm));
		}

		return true;
	}

	_string_type LogFile_Date::GetTimer( void )
	{
		struct tm timeinfo = { 0 };
		Timestamp_type().epochTM(&timeinfo);
		char szTime[100] = {0};
		sprintf ( szTime,"[%04d-%02d-%02d %02d:%02d:%02d]", 
			timeinfo.tm_year + 1900, timeinfo.tm_mon + 1,
			timeinfo.tm_mday,timeinfo.tm_hour,
			timeinfo.tm_min,timeinfo.tm_sec);
		return szTime;
	}

	LogFile_Capacity::LogFile_Capacity( UInt32 u32MaxCapacity )
		: _u32MaxCapacity(u32MaxCapacity)
	{

	}

	LogFile_Capacity::~LogFile_Capacity( void )
	{
		Close();
	}

	void LogFile_Capacity::SetCapacity( UInt32 u32MaxCapacity )
	{
		_u32MaxCapacity = u32MaxCapacity;
	}

	int LogFile_Capacity::Open( const char* c_szSavePath )
	{
		if( c_szSavePath == NULL )
			return -1;

		_sFilePath = c_szSavePath;
		if ( -1 == _File.Open(c_szSavePath, "a+") )
			return -1;

		_u32Capacity = _File.GetFileSize(c_szSavePath);
		return 1;
	}

	int LogFile_Capacity::Close( void )
	{
		if( _File.is_open() )
		{
			_File.Close();
			return 1;
		}

		return -1;
	}

	void LogFile_Capacity::Print( const char* c_szMesage, const char* sz_Remark )
	{
		_Lock.Lock();
		if( CheckFile() )
		{
			const UInt16 u16Size = strlen(c_szMesage);
			_u32Capacity += u16Size;
			if( sz_Remark != NULL )
			{
				_File.Write(sz_Remark,strlen(sz_Remark));
				_File.Write("  ",strlen("  "));
			}
			_File.Write(c_szMesage,u16Size);
			_File.Flush();
		}
		_Lock.UnLock();
	}

	bool LogFile_Capacity::CheckFile( void )
	{
		if( _u32Capacity >= _u32MaxCapacity )
		{
			Timestamp_type stamp;
			tm  Time = { 0 };
			stamp.epochTM(&Time);

			if( _File.is_open() )
			{
				_File.Close();
				char sz[20] = {0};
				sprintf(sz,"_%04d-%02d-%02d-%02d%02d%02d.", Time.tm_year + 1900, Time.tm_mon, Time.tm_mday + 1,
					Time.tm_hour, Time.tm_min, Time.tm_sec);
				_string_type sPath = _sFilePath;
				sPath = sPath.replace(".",sz,1);
				_sFilePath = sPath;
				_u32Capacity = 0;
			}

			if ( -1 == _File.Open(_sFilePath.c_str(), "w"))
			{
				return false;
			}
		}

		return true;
	}

} //_log_

