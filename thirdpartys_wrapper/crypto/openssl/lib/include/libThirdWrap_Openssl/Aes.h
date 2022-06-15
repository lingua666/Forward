
#ifndef __OPENSSL_AES_20210330111304_1444199361_H__
#define __OPENSSL_AES_20210330111304_1444199361_H__

#include <libThirdWrap_Openssl/External.h>

namespace _openssl_ {

	typedef _string_type	_openssl_string;
	
	class Aes
	{
	public:
		Aes(void);

		~Aes(void);

		int Init(const _openssl_string& UserKey);

		void Release(void);

		_openssl_string Encrypt(const unsigned char* pData, UInt32 uSize);

		_openssl_string Decrypt(const unsigned char* pData, UInt32 uSize);

		bool isVaild(void);

	private:
		_openssl_string _UserKey;
	};

}


#endif//__EXTERNAL_20210330111304_1444199361_H__
