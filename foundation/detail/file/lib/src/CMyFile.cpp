
#include <libFile/CMyFile.h>

namespace	_file_{

	bool CMyFile::IsExit( const _string_type& sPath )
	{
		return CMyDirectory::IsExit(sPath);
		/*struct stat f_stat;
		if (stat(sPath.c_str(), &f_stat) == -1)
		{
		return false;
		}
		return true;*/
	}

	int	CMyFile::GetFileSize( const _string_type& sPath )
	{
		/*#ifdef WIN32
		return filelength(fileno(_FHandle));
		#else*/
		struct stat f_stat;
		if (stat(sPath.c_str(), &f_stat) == -1)
		{
			return -1;
		}
		return (long)f_stat.st_size;
		//#endif
	}

	tm* CMyFile::GetFileCreateTime( const _string_type& sPath )
	{ 
		struct stat buf; //in stat head file
		if( stat(sPath.c_str(),&buf) > -1 )
			return (tm*)localtime((time_t*)&buf.st_ctime);
		return NULL;
	}

	int	CMyFile::DeleteFile( const _string_type& sPath )
	{
		return remove(sPath.c_str()) == 0 ? 1: -1;
	}

	_string_type CMyFile::GetDirPath( const _string_type& sPath )
	{
		return CMyDirectory::GetDirPath(sPath);
	}

	_string_type CMyFile::GetFileName( const _string_type& sPath )
	{
		switch( CMyDirectory::IsDir(sPath) )
		{
		case -1://路径不存在
		case 1://目录
			return "";
			break;	
		}	

		return &sPath[strrchr( sPath.c_str(), SPLIT_CHAR_DEF ) + 1 - sPath.c_str()];
	}

	_string_type CMyFile::GetSuffix( const _string_type& sPath )
	{
		if( strrchr( sPath.c_str(), '.' ) != NULL )
			return &sPath[strrchr( sPath.c_str(), '.' ) + 1 - sPath.c_str()];
		return "";
	}

	_string_type CMyFile::NormalPath( const _string_type& sPath )
	{
#if defined(PLATFORM_OS_FAMILY_WINDOWS)
		_string_type str = sPath;
		str = str.replace("/","\\");
		str = str.replace("\\\\","\\");
		str = str.strim(" ");
		str = str.strim("\\");
		return str;
#elif defined(PLATFORM_OS_FAMILY_UNIX)
		_string_type str = sPath;
		str = str.replace("\\","/");
		str = str.replace("//","/");
		str = str.strim(" ");
		str = str.strim("/");
		return str;
#endif
	}

	CMyFile::CMyFile( void )
		: _FHandle( NULL )
	{

	}

	CMyFile::CMyFile( const CMyFile& t1 )
	{
		_Path = t1._Path;
		_FHandle = t1._FHandle;
		_Mode = t1._Mode;
		//t1._FHandle = NULL;
	}

	CMyFile::~CMyFile( void )
	{
		//Close();
	}

	CMyFile& CMyFile::operator=( const CMyFile& t1 )
	{
		_Path = t1._Path;
		_FHandle = t1._FHandle;
		_Mode = t1._Mode;
		//t1._FHandle = NULL;
		return *this;
	}

	int	CMyFile::Open( const _string_type& sPath, const _string_type& sMode, bool isCreateDir )
	{
		if( is_open() )
		{//close have open
			Close();
		}

		if( isCreateDir )
		{
			CMyDirectory::Create( CMyDirectory::GetDirPath(sPath) );
		}

		_Path = sPath;
		_Mode = sMode;
		_FHandle = fopen(sPath.c_str(),sMode.c_str());
		return _FHandle != NULL ? 1 : -1;
	}

	int	CMyFile::ReOpen( void )
	{
		if( is_open() )
		{
			Close();
		}

		return Open(_Path, _Mode);
	}

	int	CMyFile::Close( void )
	{
		if( _FHandle != NULL )
		{
			fclose(_FHandle);
			_FHandle = NULL;
		}
		return -1;
	}

	size_t CMyFile::Write( const char* c_szData, UInt32 u32Size )
	{
		return fwrite(c_szData,1,u32Size,_FHandle);
	}

	void CMyFile::Flush( void )
	{
		fflush(_FHandle);
	}

	size_t CMyFile::Read( char* c_szBuf, UInt32 u32Size )
	{
		return fread(reinterpret_cast<void*>(c_szBuf), 1, u32Size, _FHandle);
	}

	int CMyFile::Seek( long _Offset, int _Origin )
	{
		return fseek(_FHandle,_Offset,_Origin);
	}

	long CMyFile::Tell( void )
	{
		return ftell(_FHandle);
	}

	bool CMyFile::isEof( void )
	{
		return feof(_FHandle);
	}

	int	CMyFile::GetLength( void ) const
	{
		/*#ifdef WIN32
		return filelength(fileno(_FHandle));
		#else*/
		struct stat f_stat;
		if (stat(_Path.c_str(), &f_stat) == -1)
		{
			return -1;
		}
		return (long)f_stat.st_size;
		//#endif
	}

	_string_type	CMyFile::GetLineData( void )
	{
		UInt32	u32Size = 0;
		_string_type	sBuf;
		sBuf.resize(1024);
		int ch = fgetc(_FHandle);
		while(!feof(_FHandle) && (ch != '\n')) 
		{
			sBuf[u32Size] = ch;
			u32Size ++;
			ch = fgetc(_FHandle);
		}
		sBuf.update_size(u32Size);
		return sBuf;
	}

	tm* CMyFile::GetCreateTime( void )
	{ 
		struct stat buf; //in stat head file
		if( stat(_Path.c_str(),&buf) > -1 )
			return (tm*)localtime((time_t*)&buf.st_ctime);
		return NULL;
	}

	const _string_type& CMyFile::GetPath( void ) const
	{
		return _Path;
	}

	bool	CMyFile::is_open( void ) const
	{
		return _FHandle != NULL;
	}

} //_file_
