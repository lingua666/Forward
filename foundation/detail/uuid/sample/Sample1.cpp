// MediaServerSample.cpp : Defines the entry point for the console application.
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <libCommon/MyApi.h>

#pragma comment(lib, "version")

enum enumWType
{
	WTYPE_TEXT = 1,
	WTYPE_BINARY = 0,
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


#define _L	L
typedef	wchar_t	char_type;

#define _strlen(c)		wcslen(c)
#define	_size_char(n)	((n) * sizeof(char_type))



///////////////////////////////////////////////////////////////////////////
static char_type* SZKEY_VS_VERSION_INFO = L"VS_VERSION_INFO";
static char_type* SZKEY_STRINGFILEINFO = L"StringFileInfo";
static char_type* SZKEY_VARFILEINFO = L"VarFileInfo";
static char_type* SZKEY_TRANSLATION = L"Translation";

#pragma pack(push, 1)
struct tagPEFileInfo
{
	std::string _Comments;
	std::string _CompanyName;
	std::string _FileDescription;
	std::string _FileVersion;
	std::string _InternalName;
	std::string _LegalCopyright;
	std::string _LegalTrademarks;
	std::string _OriginalFilename;
	std::string _PrivateBuild;
	std::string _ProductName;
	std::string _ProductVersion;
	std::string _SpecialBuild;
};
#pragma pack(pop)

int FromSizeAlignment( int iSize )
{
	if(iSize % 4 == 0) 
		return 0;

	return 4 - (iSize % 4);
}

int	Alignment( std::string& sData )
{
	if(sData.size() % 4 == 0) 
		return 0;

	return 4 - (sData.size() % 4);
}

void AppendAlignment( std::string& sData ) 
{
	if(sData.size() % 4 == 0) 
		return ;

	static BYTE aligmentsValues[3] = {0,0,0};
	DWORD aligments = 4 - (sData.size() % 4);
	sData.append((char*)aligmentsValues, aligments);
}

class VersionInfo
{
public:
	VersionInfo( void )
	{
		memset((char*)&_FileInfo, 0, sizeof(VS_FIXEDFILEINFO) );

		//未知
		WORD value = 0;
		_sHead.append((char*)&value, sizeof(WORD));
		
		//长度
		value = sizeof(VS_FIXEDFILEINFO);
		_sHead.append((char*)&value, sizeof(WORD));
		value = 0;
		
		//数据类型
		value = WTYPE_BINARY;
		_sHead.append((char*)&value, sizeof(WORD));

		value = 0;

		//字段描述
		_sHead.append((char*)SZKEY_VS_VERSION_INFO, _size_char(_strlen(SZKEY_VS_VERSION_INFO) + 1));

		//对齐
		AppendAlignment(_sHead);
	}

	int Parse( const char* lpBuffer, int iSize )
	{
		const int iLength = _sHead.size() + sizeof(VS_FIXEDFILEINFO);
		if( iSize < iLength )
			return -1;

		memcpy(&_sHead[0], lpBuffer, _sHead.size());
		memcpy((void*)&_FileInfo, &lpBuffer[_sHead.size()], sizeof(VS_FIXEDFILEINFO));
		return iLength + FromSizeAlignment( iLength );
	}

	VS_FIXEDFILEINFO*	Get( void )
	{
		return &_FileInfo;
	}

	std::string	GetString( void )
	{
		std::string sBuf = _sHead;
		sBuf.append((char*)&_FileInfo, sizeof(VS_FIXEDFILEINFO));
		AppendAlignment(sBuf);

		//test
		//*((WORD*)&sBuf[0]) = sBuf.size();
		//test

		return sBuf;
	}

private:
	VS_FIXEDFILEINFO	_FileInfo;
	std::string			_sHead;
};

class StringFileInfo
{
public: 
	typedef		std::map<std::string, std::string>	Map_type;

public:
	StringFileInfo( void )
	{
		WORD value = 0;
		_Map[Comments] = "";
		_Map[CompanyName] = "";
		_Map[FileDescription] = "";
		_Map[FileVersion] = "";
		_Map[InternalName] = "";
		_Map[LegalCopyright] = "";
		_Map[LegalTrademarks] = "";
		_Map[OriginalFilename] = "";
		_Map[PrivateBuild] = "";
		_Map[ProductName] = "";
		_Map[ProductVersion] = "";
		_Map[SpecialBuild] = "";

		//长度
		_sHead.append((char*)&value, sizeof(WORD));

		//未知
		_sHead.append((char*)&value, sizeof(WORD));

		//数据类型
		value = WTYPE_TEXT;
		_sHead.append((char*)&value, sizeof(WORD));

		value = 0;

		//语言信息
		char_type Lan[] = L"080404B0";
		_sHead.append((char*)Lan, _size_char(_strlen(Lan) + 1) );	//040904B0为英文 080404B0为中文

		//对齐
		AppendAlignment(_sHead);

	}

	std::string Append( const std::string& sName, const std::string& sContent )
	{
		WORD value = 0;
		std::string sBuffer;

		//长度
		sBuffer.append((char*)&value, sizeof(WORD));

		//未知
		value = _strlen((char_type*)&CharToWchar(sContent)[0]);

		sBuffer.append((char*)&value, sizeof(WORD));
		value = 0;

		//数据类型
		value = WTYPE_TEXT;
		sBuffer.append((char*)&value, sizeof(WORD));
		value = 0;

		//节点名
		sBuffer.append(CharToWchar(sName).c_str(), CharToWchar(sName).size());

		//对齐
		AppendAlignment(sBuffer);

		//节点内容
		sBuffer.append(CharToWchar(sContent).c_str(), CharToWchar(sContent).size());

		AppendAlignment(sBuffer);

		*((WORD*)sBuffer.c_str()) = sBuffer.size();

		return sBuffer;
	}

	int Parse( const char* lpBuffer, int iSize )
	{
		if( iSize < _sHead.size() )
			return -1;

		//char* pData = (char*)&lpBuffer[_sHead.size()];
		int iRemain = *((WORD*)lpBuffer) - _sHead.size();
		memcpy(&_sHead[0], lpBuffer, _sHead.size() );
		
		int iNum = 0;
		while(true)
		{
			iNum = ParseValue( (char*)&lpBuffer[*((WORD*)lpBuffer) - iRemain], iRemain );
			if( iNum == -1 )
				break;

			iRemain -= iNum;
			iRemain -= FromSizeAlignment(*((WORD*)lpBuffer) - iRemain);
		}

		return *((WORD*)_sHead.c_str());
	}

	int ParseValue( const char* lpBuffer, int iSize )
	{
		if( iSize <= 0 )
			return -1;

		int iOffset = 6 + _size_char(_strlen((char_type*)&lpBuffer[6]) + 1);
		iOffset += FromSizeAlignment(iOffset);
		std::string sName = std::string((char*)&lpBuffer[6], _size_char(_strlen((char_type*)&lpBuffer[6]) + 1));
		std::string sContent = std::string((char*)&lpBuffer[iOffset], _size_char(_strlen((char_type*)&lpBuffer[iOffset]) + 1));
		SetValueFromW(sName, sContent);
		return *((WORD*)lpBuffer);
	}

	void SetValue( const std::string& sName, const std::string& sContent )
	{
		_Map[&sName[0]] = sContent;
	}

	std::string GetValue( const std::string& sName )
	{
		return _Map[sName];
	}

	std::string GetString( void )
	{
		std::string sBuf = _sHead;
		sBuf += Append(Comments,_Map[Comments]);
		AppendAlignment(sBuf);

		sBuf += Append(CompanyName,_Map[CompanyName]);
		AppendAlignment(sBuf);

		sBuf += Append(FileDescription,_Map[FileDescription]);
		AppendAlignment(sBuf);

		sBuf += Append(FileVersion,_Map[FileVersion]);
		AppendAlignment(sBuf);

		sBuf += Append(InternalName,_Map[InternalName]);
		AppendAlignment(sBuf);

		sBuf += Append(LegalCopyright,_Map[LegalCopyright]);
		AppendAlignment(sBuf);

		sBuf += Append(LegalTrademarks,_Map[LegalTrademarks]);
		AppendAlignment(sBuf);

		sBuf += Append(OriginalFilename,_Map[OriginalFilename]);
		AppendAlignment(sBuf);

		sBuf += Append(PrivateBuild,_Map[PrivateBuild]);
		AppendAlignment(sBuf);

		sBuf += Append(ProductName,_Map[ProductName]);
		AppendAlignment(sBuf);

		sBuf += Append(ProductVersion,_Map[ProductVersion]);
		AppendAlignment(sBuf);

		sBuf += Append(SpecialBuild,_Map[SpecialBuild]);
		AppendAlignment(sBuf);

		*(WORD*)&sBuf[0] = sBuf.size();
		return sBuf;
	}

protected:
	void SetValueFromW( const std::string& sName, const std::string& sContent )
	{
		_Map[&WcharToChar()[0]] = &WcharToChar(sContent)[0];
	}

private:
	Map_type			_Map;
	std::string			_sHead;
};

class StringFileInfos
{
public:
	StringFileInfos( void )
	{
		WORD	value = 0;

		//长度
		_sHead.append((char*)&value, sizeof(WORD));

		//未知
		_sHead.append((char*)&value, sizeof(WORD));

		//类型
		value = WTYPE_TEXT;
		_sHead.append((char*)&value, sizeof(WORD));

		value = 0;

		//字段描述
		_sHead.append((char*)SZKEY_STRINGFILEINFO, _size_char(_strlen(SZKEY_STRINGFILEINFO) + 1));

		//对齐
		AppendAlignment(_sHead);
	}

	~StringFileInfos( void )
	{

	}

	int Parse( const char* lpBuffer, int iSize )
	{
		if( iSize < _sHead.size() )
			return -1;

		memcpy(&_sHead[0], lpBuffer, _sHead.size());
		_FileInfo.Parse(&lpBuffer[_sHead.size()], iSize - _sHead.size());
		return *((WORD*)&lpBuffer[0]);
	}

	void SetValue( const std::string& sName, const std::string& sContent )
	{
		_FileInfo.SetValue(sName, sContent);
	}

	std::string GetValue( const std::string& sName )
	{
		return _FileInfo.GetValue(sName);
	}

	std::string GetString( void )
	{
		std::string sBuf = _sHead;
		sBuf += _FileInfo.GetString();
		AppendAlignment(sBuf);
		*((WORD*)&sBuf[0]) = sBuf.size();
		return sBuf;
	}

private:
	StringFileInfo	_FileInfo;
	std::string		_sHead;
};

class VarFileInfo
{
public:
	VarFileInfo( void )
	{
		WORD value = 0;

		_Langage.wLanguage = 2052;
		_Langage.wCodePage = 1200;

		//长度
		_sHead.append((char*)&value, sizeof(WORD));

		//未知
		value = sizeof(LANGANDCODEPAGE);
		_sHead.append((char*)&value, sizeof(WORD));

		//类型
		value = WTYPE_BINARY;
		_sHead.append((char*)&value, sizeof(WORD));

		//字段描述
		_sHead.append((char*)SZKEY_TRANSLATION, _size_char(_strlen(SZKEY_TRANSLATION) + 1));

		//对齐
		AppendAlignment(_sHead);
	}

	~VarFileInfo( void )
	{

	}

	int Parse( const char* lpBuffer, int iSize )
	{
		if( iSize < _sHead.size() + sizeof(LANGANDCODEPAGE) )
			return -1;

		memcpy(&_sHead[0], lpBuffer, _sHead.size() );

		std::string s(&lpBuffer[_sHead.size()], 16);

		memcpy((void*)&_Langage, &lpBuffer[_sHead.size()], sizeof(LANGANDCODEPAGE));
		return *((WORD*)_sHead.c_str());
	}

	LANGANDCODEPAGE* Get( void )
	{
		return &_Langage;
	}

	std::string	GetString( void )
	{
		std::string sBuf = _sHead;
		sBuf.append((char*)&_Langage, sizeof(LANGANDCODEPAGE));
		AppendAlignment(sBuf);
		*(WORD*)&sBuf[0] = sBuf.size();
		return sBuf;
	}

private:
	LANGANDCODEPAGE	_Langage;
	std::string _sHead;
};

class VarFileInfos
{
public:
	VarFileInfos( void )
	{
		WORD value = 0;

		//长度
		_sHead.append((char*)&value, sizeof(WORD));

		//未知
		value = 0;//sizeof(LANGANDCODEPAGE);
		_sHead.append((char*)&value, sizeof(WORD));

		//类型
		value = WTYPE_TEXT;
		_sHead.append((char*)&value, sizeof(WORD));

		value = 0;

		//字段描述
		_sHead.append((char*)SZKEY_VARFILEINFO, _size_char((_strlen(SZKEY_VARFILEINFO) + 1)));

		//对齐
		AppendAlignment(_sHead);
	}

	~VarFileInfos( void )
	{

	}

	int Parse( const char* lpBuffer, int iSize )
	{
		if( iSize < _sHead.size() )
			return -1;

		memcpy(&_sHead[0], lpBuffer, _sHead.size());
		_FileInfo.Parse(&lpBuffer[_sHead.size()], iSize - _sHead.size());
		return *((WORD*)&lpBuffer[0]);
	}

	std::string GetString( void )
	{
		std::string sBuf = _sHead;
		sBuf += _FileInfo.GetString();
		AppendAlignment(sBuf);
		*((WORD*)&sBuf[0]) = sBuf.size();
		return sBuf;
	}

	LANGANDCODEPAGE* Get( void )
	{
		return _FileInfo.Get();
	}

private:
	std::string	_sHead;
	VarFileInfo	_FileInfo;
};

class PEFileInfo
{
public:
	PEFileInfo( const std::string& sPath )
		: _sPath(sPath)
	{
		Init(sPath);
	}

	~PEFileInfo( void )
	{

	}

	int Update( void )
	{
		HANDLE hResource = BeginUpdateResourceA(_sPath.c_str(), FALSE);
		if( !hResource )
			return false;

		std::string s = _VersionInfo.GetString();
		s += _StringFileInfos.GetString();
		s += _VarFileInfos.GetString();
		*((WORD*)&s[0]) = s.size();

		// 更新  
		if (UpdateResourceA(hResource, RT_VERSION, MAKEINTRESOURCE(VS_VERSION_INFO), 
			_VarFileInfos.Get()->wLanguage, (BYTE*)&s[0], s.size()) != FALSE)  
		{  
			if( !EndUpdateResourceA(hResource, FALSE) )  
				return -1;
		}  

		return 1;
	}

	void SetValue( const std::string& sName, const std::string& sContent )
	{
		_StringFileInfos.SetValue(sName, sContent);
	}

	std::string GetValue( const std::string& sName )
	{
		return _StringFileInfos.GetValue(sName);
	}

	int Parse( void )
	{
		if( _lpVersionInfoBuffer == NULL )
			return -1;

		int iSize = _VersionInfo.Parse(_lpVersionInfoBuffer, _dwInfoSize);
		iSize += _StringFileInfos.Parse(&_lpVersionInfoBuffer[iSize], _dwInfoSize - iSize);
		_VarFileInfos.Parse(&_lpVersionInfoBuffer[iSize], _dwInfoSize - iSize);
		return 1;
	}

protected:
	void Init( const std::string& sPath )
	{
		DWORD	dwHandle;
		_dwInfoSize = GetFileVersionInfoSizeA(sPath.c_str(),&dwHandle);
		if( _dwInfoSize<= 0 )
		{
			int i = GetLastError();
			dwHandle = 0;
		}

		//申请内存，用于存储Versioninfo数据
		_lpVersionInfoBuffer = new char[_dwInfoSize];
		if( !_lpVersionInfoBuffer )
		{
			return ;
		}

		//获取VersionInfo
		if ( !GetFileVersionInfoA(sPath.c_str(),0,_dwInfoSize,_lpVersionInfoBuffer) )
		{
			delete _lpVersionInfoBuffer;
			_lpVersionInfoBuffer = NULL;
		}

		//查询
		/*UINT cbTranslate = 0;

		//查询失败
		if ( !VerQueryValueA(_lpVersionInfoBuffer,"\\VarFileInfo\\Translation",
			(void**)&_lpTranslate, &cbTranslate) || 
			(cbTranslate/sizeof(struct LANGANDCODEPAGE))== 0 )
		{
			delete _lpVersionInfoBuffer;
			_lpVersionInfoBuffer = NULL;
			_lpTranslate = NULL;
		}*/
	}

	VarFileInfos	_VarFileInfos;
	StringFileInfos	_StringFileInfos;
	VersionInfo		_VersionInfo;

private:
	DWORD	_dwInfoSize;
	char* _lpVersionInfoBuffer;
	std::string	_sPath;
};


class ReadPEFileInfo
{
public:
	friend class WritePEFileInfo;

public:
	ReadPEFileInfo( const std::string& sPath )
		: _lpVersionInfoBuffer(NULL)
		, _lpTranslate(NULL)
	{
		Init(sPath);
	}

	~ReadPEFileInfo( void )
	{
		if( _lpVersionInfoBuffer != NULL )
		{
			delete _lpVersionInfoBuffer;
			_lpVersionInfoBuffer = NULL;
		}
	}

	tagPEFileInfo ReadAll( void )
	{
		if( _lpVersionInfoBuffer != NULL )
		{
			tagPEFileInfo	tagInfo;
			tagInfo._Comments = ReadContent("Comments");
			tagInfo._CompanyName = ReadContent("CompanyName");
			tagInfo._FileDescription = ReadContent("FileDescription");
			tagInfo._FileVersion = ReadContent("FileVersion");
			tagInfo._InternalName = ReadContent("InternalName");
			tagInfo._LegalCopyright = ReadContent("LegalCopyright");
			tagInfo._LegalTrademarks = ReadContent("LegalTrademarks");
			tagInfo._OriginalFilename = ReadContent("OriginalFilename");
			tagInfo._PrivateBuild = ReadContent("PrivateBuild");
			tagInfo._ProductName = ReadContent("ProductName");
			tagInfo._ProductVersion = ReadContent("ProductVersion");
			tagInfo._SpecialBuild = ReadContent("SpecialBuild");
			return tagInfo;
		}

		return tagPEFileInfo();
	}

	char* Read( const std::string& sContent )
	{
		if( _lpVersionInfoBuffer != NULL )
		{
			unsigned char *lpBuffer = NULL;
			UINT cbSizeBuf = 0;
			char szSubBlock[50] = {0};
			std::string szPerfix = "\\StringFileInfo\\%04x%04x\\";
			szPerfix += sContent;
			sprintf(szSubBlock, szPerfix.c_str(), _lpTranslate[0].wLanguage, _lpTranslate[0].wCodePage);
			if ( VerQueryValueA(_lpVersionInfoBuffer, szSubBlock, (void**)&lpBuffer, &cbSizeBuf) )
			{
				return reinterpret_cast<char*>(lpBuffer);
			}
		}
		
		return NULL;
	}

protected:
	std::string ReadContent( const std::string& sContent )
	{
		char* p = Read(sContent);
		if( p == NULL )
			return "error";

		return p;
	}
	
	void Init( const std::string& sPath )
	{
		_dwInfoSize = GetFileVersionInfoSizeA(sPath.c_str(),&_dwHandle);
		if( _dwInfoSize<= 0 )
		{
			int i = GetLastError();
			_dwHandle = 0;
		}

		//申请内存，用于存储Versioninfo数据
		_lpVersionInfoBuffer = new unsigned char[_dwInfoSize];
		if( !_lpVersionInfoBuffer )
		{
			return ;
		}

		//获取VersionInfo
		if ( !GetFileVersionInfoA(sPath.c_str(),0,_dwInfoSize,_lpVersionInfoBuffer) )
		{
			delete _lpVersionInfoBuffer;
			_lpVersionInfoBuffer = NULL;
		}

		//查询
		UINT cbTranslate = 0;

		//查询失败
		if ( !VerQueryValueA(_lpVersionInfoBuffer,"\\VarFileInfo\\Translation",
				(void**)&_lpTranslate, &cbTranslate) || 
				(cbTranslate/sizeof(struct LANGANDCODEPAGE))== 0 )
		{
			delete _lpVersionInfoBuffer;
			_lpVersionInfoBuffer = NULL;
			_lpTranslate = NULL;
		}
	}

private:
	LPLANGANDCODEPAGE _lpTranslate;
	DWORD	_dwInfoSize;
	DWORD	_dwHandle;
	unsigned char* _lpVersionInfoBuffer;
};


class WritePEFileInfo
{
public:
	WritePEFileInfo( const std::string& sPath )
		: _ReadInfo( sPath )
		, _sPath( sPath )
		, _hResource( NULL )
	{

	}

	bool Begin( void )
	{
		_hResource = BeginUpdateResourceA(_sPath.c_str(), FALSE);
		if( !_hResource )
			return false;

		return true;
	}

	bool Write( const std::string& sName, const std::string& sContent )
	{
		char* s = _ReadInfo.Read(sName);
		if( s == NULL )
			return false;

		memcpy(s, sContent.c_str(), sContent.size());
		s[sContent.size()] = 0;
		return true;
	}

	bool End( void )
	{
		if( !_hResource )
			return false;

		// 更新  
		if (UpdateResourceA(_hResource, RT_VERSION, MAKEINTRESOURCE(VS_VERSION_INFO), 
			_ReadInfo._lpTranslate->wLanguage, _ReadInfo._lpVersionInfoBuffer, _ReadInfo._dwInfoSize) != FALSE)  
		{  
			//test
			std::string s((char*)_ReadInfo._lpVersionInfoBuffer,_ReadInfo._dwInfoSize);
			//test

			if( !EndUpdateResourceA(_hResource, FALSE) )  
				return false;
		}  
		return true;
	}

private:
	ReadPEFileInfo	_ReadInfo;
	std::string		_sPath;
	HANDLE			_hResource;
};

enum enPEMachine
{
	PE_MACHINE_UNKNOWN,
	PE_MACHINE_X86,
	PE_MACHINE_X64
};

int GetPEMachine(std::string path)
{
	FILE* file = fopen(path.c_str(), "rb");

	_IMAGE_DOS_HEADER tagDos;
	fread((char*)&tagDos, 1, sizeof(tagDos), file);

	//移动到e_lfanew的位置处
	fseek(file,tagDos.e_lfanew, SEEK_SET);

	_IMAGE_NT_HEADERS	tagNt;
	fread((char*)&tagNt, 1, sizeof(tagNt), file);
	
	if( tagNt.FileHeader.Machine == 0x8664 )
	{//x64
		return PE_MACHINE_X64;
	}
	else if( tagNt.FileHeader.Machine == 0x014c )
	{//x86
		return PE_MACHINE_X86;
	}
	
	return PE_MACHINE_UNKNOWN;
}

    /*#include <stdio.h>
    #include <windows.h>
    #include <tchar.h>

#pragma pack(push,2)
    struct ICONDIRENTRY
    {
        BYTE bWidth;
        BYTE bHeight;
        BYTE bColorCount;
        BYTE bReserved;
        WORD wPlanes;
        WORD wBitCount;
        DWORD dwBytesInRes;
        DWORD dwImageOffset;
    };

    struct ICONDIR
    {
        WORD idReserved;
        WORD idType;
        WORD idCount;
        //ICONDIRENTRY idEntries;
    };

    struct GRPICONDIRENTRY
    {
        BYTE bWidth;
        BYTE bHeight;
        BYTE bColorCount;
        BYTE bReserved;
        WORD wPlanes;
        WORD wBitCount;
        DWORD dwBytesInRes;
        WORD nID;
    };
    struct GRPICONDIR
    {
        WORD idReserved;
        WORD idType;
        WORD idCount;
        GRPICONDIRENTRY idEntries;
    };
	#pragma pack(pop) 
    //////////////////////////////////////////////
    //函数说明：修改EXE图标
    //
    //参    数：IconFile 图标文件
    //              ExeFile 被修改的EXE文件
    //
    //返回值： 成功为True，否则False
    /////////////////////////////////////////////
    bool ChangeExeIcon(const std::string& IconFile, const std::string& ExeFile)
    {
        ICONDIR stID;
        ICONDIRENTRY stIDE;
        GRPICONDIR stGID;
        HANDLE hFile;
        DWORD nSize, nGSize, dwReserved;
        HANDLE hUpdate;
        PBYTE pIcon, pGrpIcon;
        BOOL ret;
        hFile = CreateFileA(IconFile.c_str(), GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
        {
           return false;
        }
        ZeroMemory(&stID, sizeof(ICONDIR));
        ret = ReadFile(hFile, &stID, sizeof(ICONDIR), &dwReserved, NULL);
        ZeroMemory(&stIDE, sizeof(ICONDIRENTRY));
        ret = ReadFile(hFile, &stIDE, sizeof(ICONDIRENTRY), &dwReserved, NULL);
        nSize = stIDE.dwBytesInRes;
        pIcon = (PBYTE)malloc(nSize);
        SetFilePointer(hFile, stIDE.dwImageOffset, NULL, FILE_BEGIN);
        ret = ReadFile(hFile, (LPVOID)pIcon, nSize, &dwReserved, NULL);
        if (!ret)
        {
           CloseHandle(hFile);
           return false;
        }
        ZeroMemory(&stGID, sizeof(GRPICONDIR));
        stGID.idCount = stID.idCount;
        stGID.idReserved = 0;
        stGID.idType = 1;
        CopyMemory(&stGID.idEntries, &stIDE, 12);
        stGID.idEntries.nID = 0;
        nGSize = sizeof(GRPICONDIR);
		pGrpIcon = (PBYTE)malloc(nGSize);
		CopyMemory(pGrpIcon, &stGID, nGSize);

        hUpdate = BeginUpdateResourceA(ExeFile.c_str(), false);
		ret = UpdateResourceA(hUpdate, RT_ICON, MAKEINTRESOURCE(stGID.idEntries.nID), 0, (LPVOID)pIcon, nSize);
		ret = UpdateResourceA(hUpdate, RT_GROUP_ICON, MAKEINTRESOURCE(128), 0, (LPVOID)pGrpIcon, nGSize);
		ret = EndUpdateResourceA(hUpdate, false);
        if (!ret)
        {
           CloseHandle(hFile);
           return false;
        }
        CloseHandle(hFile);
        return true;
    }

	BOOL ReplaceICO(const std::string& lpszICO, const std::string& lpszApp)
{
    HANDLE    hICO;
    //打开硬盘上的图标<strong>文件</strong>
    if (!(hICO = ::CreateFile(lpszICO.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
    {
        return FALSE;
    }
    LONG    lOffset;    //<strong>资源</strong>在<strong>文件</strong>中的偏移量
    DWORD    dwSize;        //加载后<strong>资源</strong>的大小
    DWORD    dwReadByte;    //读取<strong>文件</strong>时实际读取大小，API强制要求。其实没用……
    LPVOID    lpRes;        //指向更新RT_ICON的<strong>资源</strong>
    LPVOID    lpGIco;        //指向更新RT_GROUP_ICON的<strong>资源</strong>
    CONST BYTE byGSize = 0x18;    //RT_GROUP_ICON<strong>资源</strong>的大小，为常数。
    //分配RT_GROUP_ICON<strong>资源</strong>的内存。
    if (!(lpGIco = new BYTE[byGSize]))  {::CloseHandle(hICO); return FALSE;}
    memset(lpGIco, 0, byGSize);//清零RT_GROUP_ICON<strong>资源</strong>内存
    ::ReadFile(hICO, lpGIco, 0x06, &dwReadByte, NULL);//从<strong>文件</strong>中读取RT_GROUP_ICON<strong>资源</strong>头
    ::ReadFile(hICO, (LPVOID)((BYTE*)lpGIco + 0x08), 0x0C, &dwReadByte, NULL);//从<strong>文件</strong>中读取RT_GROUP_ICON<strong>资源</strong>
    //获取ICO<strong>资源</strong>的大小。
    memcpy((void*)&dwSize, (void*)((BYTE*)lpGIco + 0x10), sizeof(DWORD));
    //获取ICO<strong>资源</strong>的偏移量。
    ::ReadFile(hICO, (void*)&lOffset, sizeof(LONG), &dwReadByte, NULL);
    //分配ICO<strong>资源</strong>的内存。
    if (!(lpRes = new BYTE[dwSize])) {delete[] lpGIco; ::CloseHandle(hICO); return FALSE;}
    //偏移<strong>文件</strong>指针
    ::SetFilePointer(hICO, lOffset, NULL, FILE_BEGIN);
    //读取图标<strong>资源</strong>到内存。
    ::ReadFile(hICO, lpRes, dwSize, &dwReadByte, NULL);
    ::CloseHandle(hICO);
    //打开要更改图标的可<strong>执行</strong><strong>文件</strong>。
    HANDLE    hApp;
    if (NULL == (hApp = ::BeginUpdateResource(lpszApp.c_str(), NULL)))
    {
        delete[] lpGIco;
        delete[] lpRes;
        return FALSE;
    }
    //标记更新<strong>资源</strong>
    ::UpdateResource(hApp, RT_GROUP_ICON, MAKEINTRESOURCE(1), 0, lpGIco, byGSize);
    ::UpdateResource(hApp, RT_ICON, MAKEINTRESOURCE(1), 0, lpRes, dwSize);
    //写入新<strong>资源</strong>
    int error=0;
    if (!::EndUpdateResource(hApp, FALSE))
    {
        delete[] lpGIco;
        delete[] lpRes;
        error=GetLastError();
        return FALSE;
    }
    delete[] lpGIco;
    delete[] lpRes;
    return TRUE;
}*/




	typedef struct tagHEADER  
    {  
        WORD idReserved;  
        WORD idType;  
        WORD idCount;  
    }HEADER, *LPHEADER;  
      
    typedef struct tagICONDIRENTRY  
    {  
        BYTE bWidth;  
        BYTE bHeight;  
        BYTE bColorCount;  
        BYTE bReserved;  
        WORD wPlanes;  
        WORD wBitCount;  
        DWORD dwBytesInRes;  
        DWORD dwImageOffset;  
    }ICONDIRENTRY, *LPICONDIRENTRY;  
      
    #pragma pack( push, 2 )  
    typedef struct tagGRPICONDIRENTRY  
    {  
        BYTE bWidth;  
        BYTE bHeight;  
        BYTE bColorCount;  
        BYTE bReserved;  
        WORD wPlanes;  
        WORD wBitCount;  
        DWORD dwBytesInRes;  
        WORD nID;  
    }GRPICONDIRENTRY, *LPGRPICONDIRENTRY;;  
      
    typedef struct tagGRPICONDIR  
    {  
        WORD idReserved;  
        WORD idType;  
        WORD idCount;  
        GRPICONDIRENTRY idEntries[1];  
    }GRPICONDIR, *LPGRPICONDIR;  
    #pragma pack( pop )  






	  void ChangeExeIcon(const std::string& lpIconFile, const std::string& lpExeName)  
    {  
        LPICONDIRENTRY pIconDirEntry(NULL);  
        LPGRPICONDIR pGrpIconDir(NULL);  
        HEADER header;  
        LPBYTE pIconBytes(NULL);  
        HANDLE hIconFile(NULL);  
        DWORD dwRet(0), nSize(0), nGSize(0), dwReserved(0);  
        HANDLE hUpdate(NULL);  
        BOOL ret(FALSE);  
        WORD i(0);  
      
        //打开图标文件  
        hIconFile = CreateFile(lpIconFile.c_str(), GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);  
        if (hIconFile == INVALID_HANDLE_VALUE)  
        {  
            return;  
        }  
        //读取文件头部信息  
        ret=ReadFile(hIconFile, &header, sizeof(HEADER), &dwReserved, NULL);  
        if (!ret)  
        {  
            CloseHandle(hIconFile);  
            return;  
        }  
        //建立每一个图标的目录信息存放区域  
        pIconDirEntry = (LPICONDIRENTRY)new BYTE[header.idCount * sizeof(ICONDIRENTRY)];  
        if (pIconDirEntry==NULL)  
        {  
            CloseHandle(hIconFile);  
            return;  
        }  
        //从Icon文件中读取每一个图标的目录信息  
        ret = ReadFile(hIconFile, pIconDirEntry, header.idCount * sizeof(ICONDIRENTRY), &dwReserved, NULL);  
        if (!ret)  
        {  
            delete[] pIconDirEntry;  
            CloseHandle(hIconFile);  
            return;  
        }  

        //建立EXE文件中RT_GROUP_ICON所需的数据结构存放区域  
        nGSize = sizeof(GRPICONDIR) + header.idCount * sizeof(ICONDIRENTRY);  
        pGrpIconDir = (LPGRPICONDIR)new BYTE[nGSize];  

        //填充信息，这里相当于一个转换的过程  
        pGrpIconDir->idReserved=header.idReserved;  
        pGrpIconDir->idType=header.idType;  
        pGrpIconDir->idCount=header.idCount;  

        //复制信息并设置每一个图标对应的ID。ID为位置索引号  
        for(i = 0;i < header.idCount; i ++)  
        {  
            pGrpIconDir->idEntries[i].bWidth=pIconDirEntry[i].bWidth;  
            pGrpIconDir->idEntries[i].bHeight=pIconDirEntry[i].bHeight;  
            pGrpIconDir->idEntries[i].bColorCount=pIconDirEntry[i].bColorCount;  
            pGrpIconDir->idEntries[i].bReserved=pIconDirEntry[i].bReserved;  
            pGrpIconDir->idEntries[i].wPlanes=pIconDirEntry[i].wPlanes;  
            pGrpIconDir->idEntries[i].wBitCount=pIconDirEntry[i].wBitCount;  
            pGrpIconDir->idEntries[i].dwBytesInRes=pIconDirEntry[i].dwBytesInRes;  
            pGrpIconDir->idEntries[i].nID = i + 1;  
        }  

        //开始更新EXE中的图标资源，ID定为最小０，如果原来存在０ID的图标信息则被替换为新的。  
        hUpdate = BeginUpdateResourceA(lpExeName.c_str(), false);  
        if (hUpdate!=NULL)  
        {  
            //首先更新RT_GROUP_ICON信息  
            ret = UpdateResource(hUpdate, RT_GROUP_ICON, MAKEINTRESOURCE(128), MAKELANGID(LANG_CHINESE, SUBLANG_SYS_DEFAULT), (LPVOID)pGrpIconDir, nGSize);  
            if (!ret)  
            {  
                delete[] pIconDirEntry;  
                delete[] pGrpIconDir;  
                CloseHandle(hIconFile);  
                return;  
            }  
            //接着的是每一个Icon的信息存放  
            for(i=0;i<header.idCount;i++)  
            {  
                //Icon的字节数  
                nSize = pIconDirEntry[i].dwBytesInRes;  
                //偏移文件的指针到当前图标的开始处  
                dwRet=SetFilePointer(hIconFile, pIconDirEntry[i].dwImageOffset, NULL, FILE_BEGIN);  
                if (dwRet==INVALID_SET_FILE_POINTER)  
                {  
                    break;  
                }  
                //准备pIconBytes来存放文件里的Byte信息用于更新到EXE中。  
                delete[] pIconBytes;  
                pIconBytes = new BYTE[nSize];  
                ret = ReadFile(hIconFile, (LPVOID)pIconBytes, nSize, &dwReserved, NULL);  
                if(!ret)  
                {  
                    break;  
                }  
                //更新每一个ID对应的RT_ICON信息  
                ret = UpdateResource(hUpdate, RT_ICON, MAKEINTRESOURCE(pGrpIconDir->idEntries[i].nID), MAKELANGID(LANG_CHINESE, SUBLANG_SYS_DEFAULT), (LPVOID)pIconBytes, nSize);  
                if(!ret)  
                {  
                    break;  
                }  
            }  
            //结束EXE资源的更新操作  
            if (pIconBytes!=NULL)  
            {  
                delete[] pIconBytes;  
            }  
            EndUpdateResource(hUpdate, false);  
        }  
        //清理资源并关闭Icon文件，到此更新操作结束！  
        delete[] pGrpIconDir;  
        delete[] pIconDirEntry;  
        CloseHandle(hIconFile);  
    } 


int main(int argc, char* argv[])
{
	/*ChangeExeIcon("E:\\lysTest\\Code\\NetControl\\SVN\\trunk\\build1064\\bin\\Release\\message01.ico",
		"E:\\lysTest\\Code\\NetControl\\SVN\\trunk\\build1064\\bin\\Release\\SampleUUID.exe");
	//GetPEMachine("E:\\lysTest\\Code\\NetControl\\SVN\\trunk\\build1064\\bin\\Release\\dllNetFrame_AsyncCmdClient.dll");

	return 0;*/

	char fullpath[1024] = {0};
	GetModuleFileNameA(NULL,fullpath,sizeof(fullpath));
	PEFileInfo pe(fullpath);
	//PEFileInfo pe("d:\\QQ.exe");a
	//PEFileInfo pe("d:\\SampleUUID.exe");
	pe.Parse();

	std::string s = pe.GetValue(FileVersion);
	pe.SetValue(FileVersion, "13333331");
	pe.SetValue(ProductName, "广州传蔚");
	pe.SetValue(FileDescription, "22");
	pe.SetValue(ProductVersion, "22");
	pe.Update();

	/*ReadPEFileInfo Info("F:\\Program Files\\Tencent\\QQ\\Bin\\QQ.exe");
	Info.ReadAll();*/

	/*char fullpath[1024] = {0};
	GetModuleFileNameA(NULL,fullpath,sizeof(fullpath));
	

	WritePEFileInfo Info( "d:\\QQ.exe" );
	Info.Begin();
	//Info.Write("ProductName","广州传蔚智能科技有限公司");
	Info.Write("ProductName","1111111111111111");
	Info.End();*/

	std::cin.get();
	return 0;
}