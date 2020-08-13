
#include <libValidityCheck/ExeFile.h>
#include <libUUID/MD5Engine.h>
#include <libCommon/os/Platform.h>

namespace	_validity_{

	int ExeFile::GetKeySize( void )
	{
		return KEY_SIZE_20160713_DEF + KEY_RESERVE_20160713_DEF;
	}

	const char* ExeFile::GetHead( void )
	{
		static char s_Key20160713[KEY_RESERVE_20160713_DEF] = 
							{0xBB, 0xFF, 0xE2, 0x28, 0xAC, 0x03, 0x0, 0x04, 0x0};
		return s_Key20160713;
	}

	int ExeFile::GetKey( const char* c_szPath, const char* SubKey,
					char* szOutBuf, UInt16 uOutSize )
	{
		if( szOutBuf == NULL )
			return GetKeySize();

		_file_::CMyFile	hFile;
		if ( hFile.Open(c_szPath, "rb") != 1 )
			return -1;

		char szOut[KEY_SIZE_20160713_DEF] = {0};
		Encode(hFile, SubKey, szOut, sizeof(szOut));
		_string_type s( GetHead(), KEY_RESERVE_20160713_DEF );
		s.append(szOut, sizeof(szOut));

		memcpy(szOutBuf, s.c_str(), __min(s.size(), uOutSize));

		hFile.Close();
		return s.size();
	}


	int ExeFile::ReadKey( _file_::CMyFile& hFile, char* szBuf, UInt16 uSize )
	{
		if( hFile.GetLength() < GetKeySize() )
			return -1;

		hFile.Seek( -1 * GetKeySize(), SEEK_END);

		char szHead[KEY_RESERVE_20160713_DEF] = {0};

		if( hFile.Read(szHead, KEY_RESERVE_20160713_DEF) == 0 )
			goto gt_end;

		if( strncmp( szHead, GetHead(), KEY_RESERVE_20160713_DEF) != 0 )
			goto gt_end;

		if( hFile.Read(szBuf, uSize) == 0 )
			goto gt_end;

		hFile.Seek(0, SEEK_SET);
		return 1;

gt_end:
		hFile.Seek(0, SEEK_SET);
		return -1;
	}

	int ExeFile::Encode( _file_::CMyFile& hFile,
						const char* SubKey,
						const char* pOut,
						UInt16 uOutSize )
	{
		_uuid_::MD5Engine	Md5;

		int iSize = 0;

		if( isExsitKey(hFile.GetPath().c_str()) )
			iSize = hFile.GetLength() - GetKeySize();
		else
			iSize = hFile.GetLength();

		int nLength = 0;       //number of bytes read from the file  
		const int nBufferSize = 1024; //checksum the file in blocks of 1024 bytes  
		unsigned char Buffer[nBufferSize];   //buffer for data read from the file  

		do 
		{
			nLength = hFile.Read(reinterpret_cast<char*>(Buffer), __min(nBufferSize, iSize));

			if( nLength > 0 )
				Md5.update(Buffer, nLength);

			iSize -= nLength;
		} while ( iSize > 0 );

		Md5.update(SubKey, strlen(SubKey));

		typedef _uuid_::DigestEngine::Digest	Digest;

		Digest Dig = Md5.digest();

		char szBuf[KEY_SIZE_20160713_DEF] = {0};

		for (int i = 0; i < Dig.size(); i ++)
		{
			szBuf[i] = Dig.at(i);
		}

		return ChangePostion(szBuf, sizeof(szBuf), pOut, uOutSize);
	}

	int ExeFile::ChangePostion( const char* c_szInput, UInt16 uInSize,
		const char* c_szOut,  UInt16 uOutSize )
	{
		if( uInSize > uOutSize )
			return -1;

		for( int i = 4; i < uInSize; i ++ )
		{
			if( i % 8 == 0 )
			{
				*((UInt32*)&c_szOut[i - 8]) = *((UInt32*)&c_szInput[i - 4]);
				*((UInt32*)&c_szOut[i - 4]) = *((UInt32*)&c_szInput[i - 8]);
			}
		}
		return 1;
	}

	bool ExeFile::isExsitKey( const char* c_szPath )
	{
		_file_::CMyFile	hFile;
		if( hFile.Open(c_szPath, "rb") != 1 )
			return false;

		char szHead[KEY_RESERVE_20160713_DEF] = {0};
		if( hFile.GetLength() < GetKeySize() )
		{
			goto gt_end;
		}

		hFile.Seek(hFile.GetLength() - GetKeySize(), SEEK_SET);

		if( hFile.Read(szHead, KEY_RESERVE_20160713_DEF) == 0 )
		{
			goto gt_end;
		}

		if( strncmp( szHead, GetHead(), KEY_RESERVE_20160713_DEF) != 0 )
		{
			goto gt_end;
		}
		hFile.Close();
		return true;

gt_end:
		hFile.Close();
		return false;
	}

} // namespace	_validity_

