
#ifndef __CMYDIRECTORY_20151127152255_1448608976_H__
#define __CMYDIRECTORY_20151127152255_1448608976_H__

#include <libCommon/modafx.h>
#include <libFunction_v20/CMyFunction.hpp>
#include <stdio.h>
#include <sys/stat.h>
#include <stdarg.h>

#define S_ISREAD(m) (((m) & ~0170000) & (00400))  
#define S_ISWRITE(m) (((m) & ~0170000) & (00200))  
#define S_ISEXEC(m) (((m) & ~0170000) & (00100))

#if defined(PLATFORM_OS_FAMILY_WINDOWS)
#include <direct.h>
#include <io.h>
#include <algorithm>
#include <sys/types.h>

#define MKDIR(a) _mkdir((a)) 
#define S_ISREG(m) (((m) & 0170000) == (0100000))   
#define S_ISDIR(m) (((m) & 0170000) == (0040000))  

#define S_IFDIR  0040000
#define SPLIT_CHAR_DEF '\\'
#define SPLIT_STRING_DEF "\\"

typedef struct _dirdesc {
	int     dd_fd;      // file descriptor associated with directory 
	long    dd_loc;     // offset in current buffer 
	long    dd_size;    // amount of data returned by getdirentries 
	char    *dd_buf;    // data buffer 
	int     dd_len;     // size of data buffer 
	long    dd_seek;    // magic cookie returned by getdirentries 
	HANDLE  hFind; 
} DIR;

# define __dirfd(dp)    ((dp)->dd_fd)

struct dirent
{
	long d_ino;              // inode number
	off_t d_off;             // offset to this dirent
	unsigned short d_reclen; // length of this d_name
	unsigned char d_type;    // the type of d_name
	char d_name[512];        // file name (null-terminated)
};

int lstat(const char *path, struct stat *buf);

int _chdir(const char * path);

DIR *opendir(const char *name) ;

struct dirent *readdir(DIR *d) ;

int closedir(DIR *d) ;

#elif defined(PLATFORM_OS_FAMILY_UNIX)
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#define MKDIR(a)	mkdir((a),0755)
#define _chdir(a)	chdir(a)
#define SPLIT_CHAR_DEF '/'
#define SPLIT_STRING_DEF "/"
#endif 

namespace	_file_{

	struct	CMyDirectory
	{
		typedef	 function20_handle<void (const char*, const char*)>	HFNFile;

		typedef	 function20_handle<void(bool, const char*)>	HFNFileV20;

		static bool IsExit( const char* c_szPath );

		static int IsDir( const _string_type& sPath );

		static _string_type GetCurrentFilePath( void );

		static _string_type GetCurrentDirPath( void );

		static _string_type GetDirPath( const _string_type& sPath );

		static const char*	GetSplit( void );

		static	bool Create( const _string_type& sPath );

		static void RecursiveOnlyFile( const _string_type& sPath,
							const HFNFile& hfnNotify,
							int iDepth = 0 );

		static void RecursiveAll(const _string_type& sPath,
							const HFNFileV20& hfnNotify,
							int iDepth = 0);

		static void RecursiveAllToDepth(const _string_type& sPath,
							const HFNFileV20& hfnNotify,
							UInt32 uDepth = -1);

		static UInt64 GetDirectSize( const char* c_szDir );

		static bool RemoveDir( const char* c_szDir );

		static _string_type NormcasePath(const _string_type& sDir);
	};

}

#endif//__CMYDIRECTORY_20151127152255_1448608976_H__
