
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
	SetCurrentDirectory(path);  //����
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
		{//·��������
			return -1;
		}

		if( !S_ISDIR(f_stat.st_mode) )
			return 0;//����Ŀ¼

		return 1;//Ŀ¼
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
		case -1://·��������
		case 0://��Ŀ¼
			{
				int iLen = strrchr( sPath.c_str(), SPLIT_CHAR_DEF ) - sPath.c_str();
				if( iLen == 0 )
					return "/";

				return _string_type(sPath.c_str(), iLen);
			}
			break;
		case 1://Ŀ¼
			
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
	{//�����༶Ŀ¼
		if( sPath == "" )
			return false;

		int iLen = sPath.size();

		// �����м�Ŀ¼
		_string_type sDir;
		for (int i = 0;i < iLen;i ++)
		{
			sDir.append(1, sPath.c_str()[i]);
			if (sPath.c_str()[i] == SPLIT_CHAR_DEF)
			{ 
				//���������,����
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

		//���������,����
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
	{//�ݹ�Ŀ¼
		DIR *dp;                      // ������Ŀ¼��ָ��  
		struct dirent *entry;         // ����dirent�ṹָ�뱣�����Ŀ¼  
		struct stat statbuf;          // ����statbuf�ṹ�����ļ�����  
		if((dp = opendir(sPath.c_str())) == NULL) // ��Ŀ¼����ȡ��Ŀ¼��ָ�룬�жϲ����Ƿ�ɹ�  
		{  
			return ;  
		}  
		_chdir (sPath.c_str());                     // �л�����ǰĿ¼  
		while((entry = readdir(dp)) != NULL)  // ��ȡ��һ��Ŀ¼��Ϣ�����δ����ѭ��  
		{  
			lstat(entry->d_name, &statbuf); // ��ȡ��һ����Ա����  
			if(S_IFDIR & statbuf.st_mode)    // �ж���һ����Ա�Ƿ���Ŀ¼  
			{  
				if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)  
					continue;  

				Recursive(sPath + SPLIT_STRING_DEF + entry->d_name, hfnNotify, iDepth + 4);              // �ݹ��������ɨ����һ��Ŀ¼������  
			}  
			/*else if( _string_type(entry->d_name).find(".xml") != _string_type::npos )
			{
			//LoadXML(entry->d_name);
			}*/
			else
			{
				//Log4c::Debug(DEBUG_OUTSTRING "%*snot xml file:%s/\r\n",depth, "", entry->d_name);
				//printf("%*s%s\n", iDepth, "", entry->d_name);  // ������Բ���Ŀ¼�ĳ�Ա  
				if( hfnNotify )
					hfnNotify(sPath.c_str(), entry->d_name);
			}
		}  
		_chdir("..");                                                // �ص��ϼ�Ŀ¼  
		closedir(dp);                                               // �ر���Ŀ¼��  
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




