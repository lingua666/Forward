
#include <libFile/CMyDirectory.h>

static void RecursiveCallBack( UInt64* pTotalSize,
							const char* c_szDir,
							const char* c_szFileName )
{ 
	_string_type sPath(c_szDir);
	sPath += "\\";
	sPath += c_szFileName;
	struct stat f_stat;
	if (stat(sPath.c_str(), &f_stat) != -1)
	{
		(*pTotalSize) += (long)f_stat.st_size;
	}
}

#if defined(PLATFORM_OS_FAMILY_WINDOWS)

int lstat(const char *path, struct stat *buf)
{
	WIN32_FIND_DATA  wfd;
	bool rValue = false;
	buf->st_mode = 0;
	HANDLE hFind = FindFirstFile(path, &wfd);
	if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		buf->st_mode = S_IFDIR;
		rValue = true;
	}
	FindClose(hFind);
	return rValue;
}

int _chdir(const char * path)
{
	SetCurrentDirectory(path);  //设置
	return 0;
}

DIR *opendir(const char *name)  
{  
	if( name == NULL )
		return NULL;

	DIR *dir = NULL;  
	WIN32_FIND_DATA FindData = {0};
	char namebuf[512] = {0};  

	sprintf(namebuf, "%s%s*.*", name, SPLIT_STRING_DEF);
	dir = (DIR *)MemoryAllocator::Alloc(sizeof(DIR));
	if( dir == NULL )  
	{  
		printf("DIR memory allocate fail\n");  
		return 0;  
	}

	memset(dir, 0, sizeof(DIR));
	dir->hFind = FindFirstFile(namebuf, &FindData );   
	if( dir->hFind == INVALID_HANDLE_VALUE )  
	{  
		printf("FindFirstFile failed (%d)\n", GetLastError());
		MemoryAllocator::Free(dir);
		return NULL;  
	}

	return dir;  
}  

struct dirent *readdir(DIR *d)  
{  
	static struct dirent dirent;  
	BOOL bf;  
	WIN32_FIND_DATA FileData;  
	if( d == NULL )  
	{  
		return 0;  
	}  

	bf = FindNextFile(d->hFind,&FileData);  
	//fail or end  
	if(!bf)  
	{  
		return 0;  
	}

	memset(dirent.d_name,0,sizeof(dirent.d_name));
	strcpy(dirent.d_name,FileData.cFileName);
	dirent.d_reclen = FileData.nFileSizeLow;  

	//check there is file or directory  
	if(FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)  
	{  
		dirent.d_type = 2;  
	}  
	else  
	{  
		dirent.d_type = 1;  
	}  

	return (&dirent);  
}  

int closedir(DIR *d)  
{  
	if( d == NULL )
		return -1;
	if( d->hFind != NULL )
	{
		FindClose(d->hFind);
		d->hFind = 0;  
	}
	MemoryAllocator::Free(d);  
	return 0;  
}

#endif

namespace	_file_{

	bool CMyDirectory::IsExit( const _string_type& sPath )
	{
		struct stat f_stat;
		if (stat(sPath.c_str(), &f_stat) == -1)
		{
			return false;
		}
		return true;
	}

	int CMyDirectory::IsDir( const _string_type& sPath )
	{
		_string_type sBuf(sPath);
		struct stat f_stat;
#if defined(PLATFORM_OS_FAMILY_WINDOWS)
		sBuf = sBuf.strim(SPLIT_STRING_DEF);
#elif defined(PLATFORM_OS_FAMILY_UNIX)
		sBuf = sBuf.rstrim(SPLIT_STRING_DEF);
#endif
		if( stat( sBuf.c_str(), &f_stat ) == -1 )
		{//路径不存在
			return -1;
		}

		if( !S_ISDIR(f_stat.st_mode) )
			return 0;//不是目录

		return 1;//目录
	}

	_string_type CMyDirectory::GetCurrentFilePath( void )
	{
		char fullpath[1024] = {0};
#if defined(PLATFORM_OS_FAMILY_WINDOWS)
		GetModuleFileNameA(NULL,fullpath,sizeof(fullpath));
#elif defined(PLATFORM_OS_FAMILY_UNIX)
		readlink("/proc/self/exe", fullpath, sizeof(fullpath));
#endif
		return _string_type(fullpath);
	}

	_string_type CMyDirectory::GetCurrentDirPath( void )
	{
		char fullpath[1024] = {0};
#if defined(PLATFORM_OS_FAMILY_WINDOWS)
		GetModuleFileNameA(NULL,fullpath,sizeof(fullpath));
#elif defined(PLATFORM_OS_FAMILY_UNIX)
		readlink("/proc/self/exe", fullpath, sizeof(fullpath));
#endif
		fullpath[strrchr( fullpath, SPLIT_CHAR_DEF ) - fullpath] = 0;
		return _string_type(fullpath);
	}

	_string_type CMyDirectory::GetDirPath( const _string_type& sPath )
	{
#if defined(PLATFORM_OS_FAMILY_WINDOWS)
		if( strrchr( sPath.c_str(), '.' ) != NULL )
		{
			int iLen = strrchr( sPath.c_str(), '\\' ) - sPath.c_str();
			return _string_type(sPath.c_str(), iLen);
		}
#elif defined(PLATFORM_OS_FAMILY_UNIX)
		switch( IsDir(sPath) )
		{
		case -1://路径不存在
		case 0://非目录
			{
				int iLen = strrchr( sPath.c_str(), SPLIT_CHAR_DEF ) - sPath.c_str();
				if( iLen == 0 )
					return "/";

				return _string_type(sPath.c_str(), iLen);
			}
			break;
		case 1://目录
			
			break;
		}		
#endif

		return sPath;
	}

	const char*	CMyDirectory::GetSplit( void )
	{
#if defined(PLATFORM_OS_FAMILY_WINDOWS)
		return "\\";
#elif defined(PLATFORM_OS_FAMILY_UNIX)
		return "/";
#endif
	}

	bool CMyDirectory::Create( const _string_type& sPath )
	{//创建多级目录
		if( sPath == "" )
			return false;

		int iLen = sPath.size();

		// 创建中间目录
		_string_type sDir;
		for (int i = 0;i < iLen;i ++)
		{
			sDir.append(1, sPath.c_str()[i]);
			if (sPath.c_str()[i] == SPLIT_CHAR_DEF)
			{ 
				//如果不存在,创建
				_string_type sTm = sDir;
				sTm = sTm.rstrim(SPLIT_STRING_DEF);
		#if defined(PLATFORM_OS_FAMILY_WINDOWS)
				if( !( sTm.size() == 2 && sTm.c_str()[1] == ':' )
					&& !IsExit( sTm ) )
		#elif defined(PLATFORM_OS_FAMILY_UNIX)
				if( sTm.size() > 1 && !IsExit( sTm ) )
		#endif
				{
					if ( 0 != MKDIR(sDir.c_str()) )
						return false;
				}
			} 
		}

		//如果不存在,创建
		sDir = sDir.rstrim(SPLIT_STRING_DEF);
		if( !IsExit( sDir ) )
		{
			if ( 0 != MKDIR(sDir.c_str()) )
				return false;
		}

		return true;
	}

	void CMyDirectory::Recursive( const _string_type& sPath,
								const HFNFile& hfnNotify,
								int iDepth )
	{//递归目录
		DIR *dp;                      // 定义子目录流指针  
		struct dirent *entry;         // 定义dirent结构指针保存后续目录  
		struct stat statbuf;          // 定义statbuf结构保存文件属性  
		if((dp = opendir(sPath.c_str())) == NULL) // 打开目录，获取子目录流指针，判断操作是否成功  
		{  
			return ;  
		}  
		_chdir (sPath.c_str());                     // 切换到当前目录  
		while((entry = readdir(dp)) != NULL)  // 获取下一级目录信息，如果未否则循环  
		{  
			lstat(entry->d_name, &statbuf); // 获取下一级成员属性  
			if(S_IFDIR & statbuf.st_mode)    // 判断下一级成员是否是目录  
			{  
				if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)  
					continue;  

				Recursive(sPath + SPLIT_STRING_DEF + entry->d_name, hfnNotify, iDepth + 4);              // 递归调用自身，扫描下一级目录的内容  
			}  
			/*else if( _string_type(entry->d_name).find(".xml") != _string_type::npos )
			{
			//LoadXML(entry->d_name);
			}*/
			else
			{
				//Log4c::Debug(DEBUG_OUTSTRING "%*snot xml file:%s/\r\n",depth, "", entry->d_name);
				//printf("%*s%s\n", iDepth, "", entry->d_name);  // 输出属性不是目录的成员  
				if( hfnNotify )
					hfnNotify(sPath.c_str(), entry->d_name);
			}
		}  
		_chdir("..");                                                // 回到上级目录  
		closedir(dp);                                               // 关闭子目录流  
		return ;
	}

	UInt64 CMyDirectory::GetDirectSize( const char* c_szDir )
	{
		UInt64 uTotalSize = 0;
		Recursive(c_szDir, function20_bind(RecursiveCallBack, &uTotalSize,
								_function_::_1, _function_::_2));
		return uTotalSize;
	}

} //_file_




