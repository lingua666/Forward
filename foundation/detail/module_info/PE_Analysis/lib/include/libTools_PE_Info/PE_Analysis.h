
#ifndef __PE_ANALYSIS_20160627193305_1438860785_H__
#define __PE_ANALYSIS_20160627193305_1438860785_H__

#include <libCommon/modafx.h>

#if defined(PLATFORM_OS_FAMILY_WINDOWS)

#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <string>

#include <windows.h>
#pragma comment(lib, "version")

enum enPEMachine
{
	PE_MACHINE_UNKNOWN,
	PE_MACHINE_X86,
	PE_MACHINE_X64
};

#define READ_SUCCESS	  0x00//获取成功
#define READ_ERROR		  0x01//获取失败
#define READ_INVALID_INIT 0x02//错误的初始化导致的，失败
#define READ_HEAP_REEO	  0x03//内部的new操作挂掉导致的

//Structure used to store enumerated languages and code pages.
typedef struct LANGANDCODEPAGE {
	WORD wLanguage;
	WORD wCodePage;
}*LPLANGANDCODEPAGE;

//需要获取的PE信息
#define Comments			"Comments"
#define CompanyName			"CompanyName"
#define FileDescription		"FileDescription"
#define FileVersion			"FileVersion"
#define InternalName		"InternalName"
#define LegalCopyright		"LegalCopyright"
#define LegalTrademarks		"LegalTrademarks"
#define OriginalFilename	"OriginalFilename"
#define PrivateBuild		"PrivateBuild"
#define ProductName			"ProductName"
#define ProductVersion		"ProductVersion"
#define SpecialBuild		"SpecialBuild"

typedef	wchar_t	char_type;

#define _strlen(c)		wcslen(c)
#define	_size_char(n)	((n) * sizeof(char_type))


///////////////////////////////////////////////////////////////////////////
static char_type* SZKEY_VS_VERSION_INFO = L"VS_VERSION_INFO";
static char_type* SZKEY_STRINGFILEINFO = L"StringFileInfo";
static char_type* SZKEY_VARFILEINFO = L"VarFileInfo";
static char_type* SZKEY_TRANSLATION = L"Translation";


class StringFileInfos;
class VarFileInfos;
class PEFileInfo;

class VersionInfo
{
public:
	friend	class PEFileInfo;

protected:
	VersionInfo( void );

	void	SetType( bool isDll )
	{
		if( isDll )
			_FileInfo.dwFileType = 0x00000002;
	}

	int Parse( const char* lpBuffer, int iSize, bool isDll = false );

	VS_FIXEDFILEINFO*	Get( void );

	std::string	GetString( void );

private:
	VS_FIXEDFILEINFO	_FileInfo;
	std::string			_sHead;
};

class StringFileInfo
{
public: 
	friend	class StringFileInfos;

	typedef		std::map<std::string, std::string>	Map_type;

protected:
	StringFileInfo( void );

	std::string Append( const std::string& sName, const std::string& sContent );

	int Parse( const char* lpBuffer, int iSize );

	int ParseValue( const char* lpBuffer, int iSize );

	void SetValue( const std::string& sName, const std::string& sContent );

	std::string GetValue( const std::string& sName );

	std::string GetString( void );

	void SetValueFromW( const std::string& sName, const std::string& sContent );

private:
	Map_type			_Map;
	std::string			_sHead;
};

class StringFileInfos
{
public:
	friend	class PEFileInfo;

protected:
	StringFileInfos( void );

	~StringFileInfos( void );

	int Parse( const char* lpBuffer, int iSize );

	void SetValue( const std::string& sName, const std::string& sContent );

	std::string GetValue( const std::string& sName );

	std::string GetString( void );

private:
	StringFileInfo	_FileInfo;
	std::string		_sHead;
};

class VarFileInfo
{
public:
	friend	class VarFileInfos;

protected:
	VarFileInfo( void );

	~VarFileInfo( void );

	int Parse( const char* lpBuffer, int iSize );

	LANGANDCODEPAGE* Get( void );

	std::string	GetString( void );

private:
	LANGANDCODEPAGE	_Langage;
	std::string _sHead;
};

class VarFileInfos
{
public:
	friend	class PEFileInfo;

protected:
	VarFileInfos( void );

	~VarFileInfos( void );

	int Parse( const char* lpBuffer, int iSize );

	std::string GetString( void );

	LANGANDCODEPAGE* Get( void );

private:
	std::string	_sHead;
	VarFileInfo	_FileInfo;
};

class PEFileInfo
{
public:
	PEFileInfo( const std::string& sPath );

	~PEFileInfo( void );

	int Update( void );

	void SetValue( const std::string& sName, const std::string& sContent );

	std::string GetValue( const std::string& sName );

	int Parse( void );

	int ChangeExeIcon( const std::string& IconFile );

	//获取PE文件(dll,exe)中的机器位数(32bit和64bit)
	int GetMachineBit( void ) const
	{
		return _MachineBit;
	}

protected:

	void Init( const std::string& sPath );

	int GetBit( const std::string& sPath );

private:
	VarFileInfos	_VarFileInfos;
	StringFileInfos	_StringFileInfos;
	VersionInfo		_VersionInfo;

	DWORD	_dwInfoSize;
	char* _lpVersionInfoBuffer;
	std::string	_sPath;
	int	_MachineBit;
};

#endif

#endif // BOOST_ASIO_COROUTINE_HPP

