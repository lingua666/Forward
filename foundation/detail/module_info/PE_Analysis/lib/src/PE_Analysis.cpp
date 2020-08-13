
#include <libTools_PE_Info/PE_Analysis.h>
#include <string>

#if defined(PLATFORM_OS_FAMILY_WINDOWS)

enum enumWType
{
	WTYPE_TEXT = 1,
	WTYPE_BINARY = 0,
};

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

static std::string WcharToChar( const std::string& szIn )
{
	std::string szOut;

	// wide char to multi char
	int iLen = WideCharToMultiByte( CP_ACP,
		0,
		(LPWSTR)&szIn[0],
		-1,
		NULL,
		0,
		NULL,
		NULL );

	szOut.resize(iLen);

	::WideCharToMultiByte( CP_ACP,
		0,
		(LPWSTR)&szIn[0],
		-1,
		&szOut[0],
		iLen,
		NULL,
		NULL );

	return szOut;
}

static std::string CharToWchar( const std::string& szIn )
{
	std::string szOut;

	int iLen = ::MultiByteToWideChar( CP_ACP,
		0,
		szIn.c_str(),
		-1,
		NULL,
		0 ); 

	szOut.resize(iLen * sizeof(wchar_t));

	::MultiByteToWideChar( CP_ACP,
		0,
		&szIn[0],
		-1,
		(LPWSTR)&szOut[0],
		iLen ); 

	return szOut; 
}

static int FromSizeAlignment( int iSize )
{
	if(iSize % 4 == 0) 
		return 0;

	return 4 - (iSize % 4);
}

static int	Alignment( std::string& sData )
{
	if(sData.size() % 4 == 0) 
		return 0;

	return 4 - (sData.size() % 4);
}

static void AppendAlignment( std::string& sData ) 
{
	if(sData.size() % 4 == 0) 
		return ;

	static BYTE aligmentsValues[3] = {0,0,0};
	DWORD aligments = 4 - (sData.size() % 4);
	sData.append((char*)aligmentsValues, aligments);
}

VersionInfo::VersionInfo( void )
{
	memset((char*)&_FileInfo, 0, sizeof(VS_FIXEDFILEINFO) );
	_FileInfo.dwSignature = 0xFEEF04BD;
	_FileInfo.dwFileOS = 0x00040004;
	_FileInfo.dwFileType = 0x00000001;
	_FileInfo.dwStrucVersion = 0x00010000;
	_FileInfo.dwFileVersionLS = 0;
	_FileInfo.dwFileVersionMS = 0;
	_FileInfo.dwProductVersionLS = 0;
	_FileInfo.dwProductVersionMS = 0;
	_FileInfo.dwFileFlagsMask = 0x0000003F;
	

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

int VersionInfo::Parse( const char* lpBuffer, int iSize, bool isDll )
{
	const int iLength = _sHead.size() + sizeof(VS_FIXEDFILEINFO);
	if( iSize < iLength )
		return -1;

	memcpy(&_sHead[0], lpBuffer, _sHead.size());
	memcpy((void*)&_FileInfo, &lpBuffer[_sHead.size()], sizeof(VS_FIXEDFILEINFO));
	return iLength + FromSizeAlignment( iLength );
}

VS_FIXEDFILEINFO*	VersionInfo::Get( void )
{
	return &_FileInfo;
}

std::string	VersionInfo::GetString( void )
{
	std::string sBuf = _sHead;
	sBuf.append((char*)&_FileInfo, sizeof(VS_FIXEDFILEINFO));
	AppendAlignment(sBuf);

	//test
	//*((WORD*)&sBuf[0]) = sBuf.size();
	//test

	return sBuf;
}



StringFileInfo::StringFileInfo( void )
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

std::string StringFileInfo::Append( const std::string& sName, const std::string& sContent )
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

int StringFileInfo::Parse( const char* lpBuffer, int iSize )
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

int StringFileInfo::ParseValue( const char* lpBuffer, int iSize )
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

void StringFileInfo::SetValue( const std::string& sName, const std::string& sContent )
{
	_Map[&sName[0]] = sContent;
}

std::string StringFileInfo::GetValue( const std::string& sName )
{
	return _Map[sName];
}

std::string StringFileInfo::GetString( void )
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

void StringFileInfo::SetValueFromW( const std::string& sName, const std::string& sContent )
{
	_Map[&WcharToChar(sName)[0]] = &WcharToChar(sContent)[0];
}

StringFileInfos::StringFileInfos( void )
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

StringFileInfos::~StringFileInfos( void )
{

}

int StringFileInfos::Parse( const char* lpBuffer, int iSize )
{
	if( iSize < _sHead.size() )
		return -1;

	memcpy(&_sHead[0], lpBuffer, _sHead.size());
	_FileInfo.Parse(&lpBuffer[_sHead.size()], iSize - _sHead.size());
	return *((WORD*)&lpBuffer[0]);
}

void StringFileInfos::SetValue( const std::string& sName, const std::string& sContent )
{
	_FileInfo.SetValue(sName, sContent);
}

std::string StringFileInfos::GetValue( const std::string& sName )
{
	return _FileInfo.GetValue(sName);
}

std::string StringFileInfos::GetString( void )
{
	std::string sBuf = _sHead;
	sBuf += _FileInfo.GetString();
	AppendAlignment(sBuf);
	*((WORD*)&sBuf[0]) = sBuf.size();
	return sBuf;
}




VarFileInfo::VarFileInfo( void )
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

VarFileInfo::~VarFileInfo( void )
{

}

int VarFileInfo::Parse( const char* lpBuffer, int iSize )
{
	if( iSize < _sHead.size() + sizeof(LANGANDCODEPAGE) )
		return -1;

	memcpy(&_sHead[0], lpBuffer, _sHead.size() );

	std::string s(&lpBuffer[_sHead.size()], 16);

	memcpy((void*)&_Langage, &lpBuffer[_sHead.size()], sizeof(LANGANDCODEPAGE));
	return *((WORD*)_sHead.c_str());
}

LANGANDCODEPAGE* VarFileInfo::Get( void )
{
	return &_Langage;
}

std::string	VarFileInfo::GetString( void )
{
	std::string sBuf = _sHead;
	sBuf.append((char*)&_Langage, sizeof(LANGANDCODEPAGE));
	AppendAlignment(sBuf);
	*(WORD*)&sBuf[0] = sBuf.size();
	return sBuf;
}




VarFileInfos::VarFileInfos( void )
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

VarFileInfos::~VarFileInfos( void )
{

}

int VarFileInfos::Parse( const char* lpBuffer, int iSize )
{
	if( iSize < _sHead.size() )
		return -1;

	memcpy(&_sHead[0], lpBuffer, _sHead.size());
	_FileInfo.Parse(&lpBuffer[_sHead.size()], iSize - _sHead.size());
	return *((WORD*)&lpBuffer[0]);
}

std::string VarFileInfos::GetString( void )
{
	std::string sBuf = _sHead;
	sBuf += _FileInfo.GetString();
	AppendAlignment(sBuf);
	*((WORD*)&sBuf[0]) = sBuf.size();
	return sBuf;
}

LANGANDCODEPAGE* VarFileInfos::Get( void )
{
	return _FileInfo.Get();
}




PEFileInfo::PEFileInfo( const std::string& sPath )
	: _sPath(sPath)
{
	Init(sPath);
}

PEFileInfo::~PEFileInfo( void )
{
	if( _lpVersionInfoBuffer != NULL )
	{
		delete _lpVersionInfoBuffer;
		_lpVersionInfoBuffer = NULL;
	}
}

int PEFileInfo::Update( void )
{
	if( strcmp(&_sPath[_sPath.size() - 3], "dll") == 0 ||
		strcmp(&_sPath[_sPath.size() - 3], "DLL") )
		_VersionInfo.SetType(true);

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
			return false;
	}
	return true;
}

void PEFileInfo::SetValue( const std::string& sName, const std::string& sContent )
{
	_StringFileInfos.SetValue(sName, sContent);
}

std::string PEFileInfo::GetValue( const std::string& sName )
{
	return _StringFileInfos.GetValue(sName);
}

int PEFileInfo::Parse( void )
{
	if( _lpVersionInfoBuffer == NULL )
		return -1;

	int iSize = _VersionInfo.Parse(_lpVersionInfoBuffer, _dwInfoSize);
	iSize += _StringFileInfos.Parse(&_lpVersionInfoBuffer[iSize], _dwInfoSize - iSize);
	_VarFileInfos.Parse(&_lpVersionInfoBuffer[iSize], _dwInfoSize - iSize);
	return 1;
}

int PEFileInfo::ChangeExeIcon( const std::string& lpIconFile )
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
		return -1;  
	}  
	//读取文件头部信息  
	ret=ReadFile(hIconFile, &header, sizeof(HEADER), &dwReserved, NULL);  
	if (!ret)  
	{  
		CloseHandle(hIconFile);  
		return -1;  
	}  
	//建立每一个图标的目录信息存放区域  
	pIconDirEntry = (LPICONDIRENTRY)new BYTE[header.idCount * sizeof(ICONDIRENTRY)];  
	if (pIconDirEntry==NULL)  
	{  
		CloseHandle(hIconFile);  
		return -1; 
	}  
	//从Icon文件中读取每一个图标的目录信息  
	ret = ReadFile(hIconFile, pIconDirEntry, header.idCount * sizeof(ICONDIRENTRY), &dwReserved, NULL);  
	if (!ret)  
	{  
		delete[] pIconDirEntry;  
		CloseHandle(hIconFile);  
		return -1;  
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
	hUpdate = BeginUpdateResourceA(_sPath.c_str(), false);  
	if (hUpdate!=NULL)  
	{  
		//首先更新RT_GROUP_ICON信息  
		ret = UpdateResource(hUpdate, RT_GROUP_ICON, MAKEINTRESOURCE(128), MAKELANGID(LANG_CHINESE, SUBLANG_SYS_DEFAULT), (LPVOID)pGrpIconDir, nGSize);  
		if (!ret)  
		{  
			delete[] pIconDirEntry;  
			delete[] pGrpIconDir;  
			CloseHandle(hIconFile);  
			return -1;  
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
	return 1;
}

void PEFileInfo::Init( const std::string& sPath )
{
	_MachineBit = GetBit(sPath);

	DWORD	dwHandle;
	_dwInfoSize = GetFileVersionInfoSizeA(sPath.c_str(), &dwHandle);
	if( _dwInfoSize <= 0 )
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
	if ( !GetFileVersionInfoA(sPath.c_str(), 0, _dwInfoSize, _lpVersionInfoBuffer) )
	{
		delete _lpVersionInfoBuffer;
		_lpVersionInfoBuffer = NULL;
	}
}

//获取PE文件中的机器位数(32bit和64bit)
int PEFileInfo::GetBit( const std::string& sPath )
{
	FILE* file = fopen(sPath.c_str(), "rb");

	_IMAGE_DOS_HEADER tagDos;
	fread((char*)&tagDos, 1, sizeof(tagDos), file);

	//移动到e_lfanew的位置处
	fseek(file,tagDos.e_lfanew, SEEK_SET);

	_IMAGE_NT_HEADERS	tagNt;
	fread((char*)&tagNt, 1, sizeof(tagNt), file);

	if( file != NULL )
		fclose(file);

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

#endif

