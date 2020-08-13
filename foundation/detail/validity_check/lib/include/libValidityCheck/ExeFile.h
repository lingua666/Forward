
#ifndef __EXEFILECHECK_20160418173307_1438860785_H__
#define __EXEFILECHECK_20160418173307_1438860785_H__

#include <libCommon/os/Types.h>
#include <libFile/CMyFile.h>

#define	KEY_SIZE_20160713_DEF		200		//4µÄ±¶Êý
#define KEY_RESERVE_20160713_DEF	9

namespace	_validity_{

	class ExeFile
	{
	public:
		static int GetKeySize( void );

		static const char* GetHead( void );

		static int GetKey( const char* c_szPath, const char* SubKey,
			char* szOutBuf, UInt16 uOutSize );

		static int ReadKey( _file_::CMyFile& hFile, char* szBuf, UInt16 uSize );

		static int Encode( _file_::CMyFile& hFile,
			const char* SubKey,
			const char* pOut,
			UInt16 uOutSize );

		static bool isExsitKey( const char* c_szPath );

	protected:
		static int ChangePostion( const char* c_szInput, UInt16 uInSize,
			const char* c_szOut,  UInt16 uOutSize );
	};

} // namespace	_validity_

#endif // __MODAFX_20160418173305_1438860785_H__
