
#ifndef __FILECHECK_20160418173307_1438860785_H__
#define __FILECHECK_20160418173307_1438860785_H__

#include <libFunction_v20/CMyFunction.hpp>

namespace	_validity_{

	class CheckValidity
	{
	public:
		enum enFileType
		{
			EN_TYPE_DLL,
			EN_TYPE_EXE,
		};

		typedef function20_handle<bool (int)>	HFNFailed;

		static bool EncodeFile( const char* c_szFilePath );

		static bool EncodeFile_Ex( const char* c_szFilePath, const char* c_szSubKey );

	public:
		CheckValidity( const char* c_szPath,
			const HFNFailed& hfnFailed );

		~CheckValidity( void );
	};

} //namespace	_validity_


#endif // __MODAFX_20160418173305_1438860785_H__
