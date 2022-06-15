
#ifndef __CMYFILE_20151127152255_1448608976_H__
#define __CMYFILE_20151127152255_1448608976_H__

#include <libFile/CMyDirectory.h>
#include <fcntl.h>
#include <time.h>

namespace	_file_{

	typedef	FILE*	FHANDLE;

	class	CMyFile
	{
	public:
		typedef struct tagTime_INFO
		{
			UInt16	Year;
			UInt8	Month;
			UInt8	Day;
			UInt8	Hour;
			UInt8	Minute;
			UInt8	Second;
		}tagTime_INFO;

		typedef struct tagPermissions_INFO
		{
			bool isRead;			//��Ȩ��
			bool isWrite;			//дȨ��
			bool isExec;			//ִ��Ȩ��
		}tagPermissions_INFO;

		typedef struct tagFile_INFO
		{
			bool isDir;						//�Ƿ�Ŀ¼
			tagPermissions_INFO	Permissions;//�ļ�Ȩ��
			UInt16				HardTime;	//Ӳ���Ӵ���
			_string_type		Ower;		//�ļ�ӵ����
			UInt64				Size;		//�ļ���С
			tagTime_INFO		Time;		//�ļ���������
			_string_type		Name;		//�ļ���
		}tagFile_INFO;

	public:
		static	bool IsExit( const char* c_szPath );

		static	Int64	GetFileSize( const _string_type& sPath );

		static	int GetFileCreateTime( const _string_type& sPath, struct tm* Time );

		static	int CopyFile(const _string_type& sSrcPath, const _string_type& sDstPath);

		static	int MoveFile(const _string_type& sSrcPath, const _string_type& sDstPath);

		static	int	DeleteFile( const _string_type& sPath );

		static	_string_type GetDirPath( const _string_type& sPath );

		static	_string_type GetFileName( const _string_type& sPath );
		
		static	_string_type GetSuffix( const _string_type& sPath );

		static _string_type NormalPath( const _string_type& sPath );

		static int GetDetailedInfo(const _string_type& sPath, tagFile_INFO* pInfo);

	public:
		CMyFile( void );

		CMyFile( const CMyFile& t1 );

		~CMyFile( void );

		CMyFile& operator=( const CMyFile& t1 );

		int	Open( const _string_type& sPath, const _string_type& sMode, bool isCreateDir = true );

		int	ReOpen( void );

		int	Close( void );

		size_t Write( const char* c_szData, UInt32 u32Size );

		void Flush( void );

		size_t Read( char* c_szBuf, UInt32 u32Size );

		int Seek( long _Offset, int _Origin );

		long Tell( void );

		bool	isEof( void );

		int	GetLength( void ) const;

		_string_type	GetLineData( void );

		int GetCreateTime(struct tm* Time);

		const _string_type& GetPath( void ) const;

		bool	is_open( void ) const;

	private:
		FHANDLE				_FHandle;
		_string_type		_Path;
		_string_type		_Mode;
	};

} //_file_

typedef _file_::CMyFile	File_type;

#endif
