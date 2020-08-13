
#include <libValidityCheck/FileCheck.h>
#include <libValidityCheck/DllFile.h>
#include <libValidityCheck/ExeFile.h>
#include <libUUID/MD5Engine.h>
#include <libTimestamp/Timestamp.h>
#include <libThread/ThreadWrap.h>
#include <libTools_PE_Info/PE_Analysis.h>

static const char* GetCompanyName( void )
{
	static _string_type Name = "广州传蔚智能科技有限公司";
	return Name.c_str();
}

static _string_type GetCompanyName_Ex( const char* c_szFilePath )
{
#if defined( PLATFORM_OS_FAMILY_WINDOWS )
	_string_type Name;
	PEFileInfo pe(c_szFilePath);
	pe.Parse();
	Name = pe.GetValue(ProductName).c_str();
	return Name;
#else
	return "";
#endif
}

namespace	_validity_{

	void Recursion( void )
	{
		Sleep(10);
		Recursion();
	}

	int* GetCheckFailed( void )
	{
		static int s_Failed = false;
		return &s_Failed;
	}

	void SetCheckFailed( int iResult )
	{
		*GetCheckFailed() = iResult;
	}

	template<int T>
	class ExitPro
	{
	public:
		ExitPro( void )
		{
			if( 0 < T && T < 100 )
			{
				int* p = NULL;
				*p = 100;
			}
			else if( 100 < T && T < 200 )
			{
				while(true)
				{
					Sleep(10);
				}
			}
			else if( 200 < T && T < 300 )
			{
				exit(0);
			}
			else if( 400 < T && T < 500 )
			{
				#if defined(PLATFORM_OS_FAMILY_WINDOWS)
					::TerminateProcess(GetCurrentProcess(), 0);
				#elif defined(PLATFORM_OS_FAMILY_UNIX)
					
				#endif
			}
			else
			{
				Recursion();
			}
		}
	};

	bool CheckValidity::EncodeFile( const char* c_szFilePath )
	{
		int iType = EN_TYPE_EXE;
		if( strncmp(&c_szFilePath[strlen(c_szFilePath) - 4], ".dll", 4) == 0 )
			iType = EN_TYPE_DLL;

		_file_::CMyFile hFile;
		if( hFile.Open(c_szFilePath, "rb+") != 1 )
			return false;

		char szKey[500] = {0};
		int iLen  = 0;
		switch(iType)
		{
		case EN_TYPE_DLL:
			{
				_string_type s = "<";
				s += "@";
				s += "dll";
				s += "@";
				s += GetCompanyName_Ex(c_szFilePath);//GetCompanyName();
				s += "@";
				s += "dll";
				s += "@";
				s += ">";

				iLen = DllFile::GetKey(c_szFilePath,
					s.c_str(),
					szKey, sizeof(szKey));

				if( DllFile::isExsitKey(c_szFilePath) )
				{
					hFile.Seek((-1) * DllFile::GetKeySize(), SEEK_END);
				}
				else
				{
					hFile.Seek(0, SEEK_END);
				}
			}
			break;
		case EN_TYPE_EXE:
			{
				_string_type s = "<";
				s += "@";
				s += "exe";
				s += "@";
				s += GetCompanyName_Ex(c_szFilePath);//GetCompanyName();
				s += "@";
				s += "exe";
				s += "@";
				s += ">";

				iLen = ExeFile::GetKey(c_szFilePath, 
					s.c_str(),
					szKey, sizeof(szKey));

				if( ExeFile::isExsitKey(c_szFilePath) )
				{
					hFile.Seek((-1) * ExeFile::GetKeySize(), SEEK_END);
				}
				else
				{
					hFile.Seek(0, SEEK_END);
				}
			}
			break;
		}

		hFile.Write(szKey, iLen);
		hFile.Close();

		return true;
	}

	bool CheckValidity::EncodeFile_Ex( const char* c_szFilePath, const char* c_szSubKey )
	{
		int iType = EN_TYPE_EXE;
		if( strncmp(&c_szFilePath[strlen(c_szFilePath) - 4], ".dll", 4) == 0 )
			iType = EN_TYPE_DLL;

		_file_::CMyFile hFile;
		if( hFile.Open(c_szFilePath, "rb+") != 1 )
			return -1;

		char szKey[500] = {0};
		int iLen  = 0;
		switch(iType)
		{
		case EN_TYPE_DLL:
			{
				_string_type s = "<";
				s += "@";
				s += "dll";
				s += "@";
				s += c_szSubKey;//GetCompanyName();
				s += "@";
				s += "dll";
				s += "@";
				s += ">";

				iLen = DllFile::GetKey(c_szFilePath,
					s.c_str(),
					szKey, sizeof(szKey));

				if( DllFile::isExsitKey(c_szFilePath) )
				{
					hFile.Seek((-1) * DllFile::GetKeySize(), SEEK_END);
				}
				else
				{
					hFile.Seek(0, SEEK_END);
				}
			}
			break;
		case EN_TYPE_EXE:
			{
				_string_type s = "<";
				s += "@";
				s += "exe";
				s += "@";
				s += c_szSubKey;//GetCompanyName();
				s += "@";
				s += "exe";
				s += "@";
				s += ">";

				iLen = ExeFile::GetKey(c_szFilePath, 
					s.c_str(),
					szKey, sizeof(szKey));

				if( ExeFile::isExsitKey(c_szFilePath) )
				{
					hFile.Seek((-1) * ExeFile::GetKeySize(), SEEK_END);
				}
				else
				{
					hFile.Seek(0, SEEK_END);
				}
			}
			break;
		}

		hFile.Write(szKey, iLen);
		hFile.Close();

		return 1;
	}

	CheckValidity::CheckValidity( const char* c_szPath,
		const HFNFailed& hfnFailed )
	{
		int iType = EN_TYPE_EXE;
		if( strncmp(&c_szPath[strlen(c_szPath) - 4], ".dll", 4) == 0 )
			iType = EN_TYPE_DLL;

		_file_::CMyFile hFile;
		char szKey[200] = {0};
		char szOut[200] = {0};
		hFile.Open(c_szPath,"rb");

		int iRet = 0;
gt_begin:
		switch(iType)
		{
		case EN_TYPE_DLL:
			iRet = DllFile::ReadKey(hFile, szKey, sizeof(szKey));
			break;
		case EN_TYPE_EXE:
			iRet = ExeFile::ReadKey(hFile, szKey, sizeof(szKey));
			break;
		}

		if( iRet != 1 )
		{
			SetCheckFailed(true);
			srand(GetTickCount());
			hFile.Close();

			int iValue = rand() % 1000;

			if( hfnFailed && hfnFailed(iValue) )
			{
				if( iValue < 100 )
				{
					ExitPro<50>	Pro;
				}
				else if( 100 < iValue  && iValue < 200)
				{
					ExitPro<150>	Pro;
				}
				else if( 200 < iValue  && iValue < 300)
				{
					ExitPro<250>	Pro;
				}
				else if( 300 < iValue  && iValue < 400)
				{
					ExitPro<350>	Pro;
				}
				else if( 400 < iValue  && iValue < 500)
				{
					ExitPro<450>	Pro;
				}
				else
				{
					ExitPro<550>	Pro;
				}	
			}
		}

		switch(iType)
		{
		case EN_TYPE_DLL:
			{
				_string_type s = "<";
				s += "@";
				s += "dll";
				s += "@";
				s += GetCompanyName_Ex(c_szPath);//GetCompanyName();
				s += "@";
				s += "dll";
				s += "@";
				s += ">";

				DllFile::Encode(hFile, s.c_str(), szOut, sizeof(szOut));
			}
			break;
		case EN_TYPE_EXE:
			{
				_string_type s = "<";
				s += "@";
				s += "exe";
				s += "@";
				s += GetCompanyName_Ex(c_szPath);//GetCompanyName();
				s += "@";
				s += "exe";
				s += "@";
				s += ">";

				ExeFile::Encode(hFile, s.c_str(), szOut, sizeof(szOut));
			}
			break;
		}

		if( memcmp(szKey, szOut, sizeof(szKey)) == 0 )
		{//验证通过
			hFile.Close();
			return ;
		}
		else
		{//验证不通过
			hFile.Close();

			int iValue = rand() % 1000;

			if( hfnFailed && hfnFailed(iValue) )
			{
				if( iValue < 100 )
				{
					ExitPro<50>	Pro;
				}
				else if( 100 < iValue  && iValue < 200)
				{
					ExitPro<150>	Pro;
				}
				else if( 200 < iValue  && iValue < 300)
				{
					ExitPro<250>	Pro;
				}
				else if( 300 < iValue  && iValue < 400)
				{
					ExitPro<350>	Pro;
				}
				else if( 400 < iValue  && iValue < 500)
				{
					ExitPro<450>	Pro;
				}
				else
				{
					ExitPro<550>	Pro;
				}
			}
		}

		goto gt_begin;
	}

	CheckValidity::~CheckValidity( void )
	{

	}

} //namespace	_validity_

