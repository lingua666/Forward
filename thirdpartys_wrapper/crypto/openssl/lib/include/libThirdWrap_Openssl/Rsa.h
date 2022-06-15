
#ifndef __OPENSSL_RSA_20210330111304_1444199361_H__
#define __OPENSSL_RSA_20210330111304_1444199361_H__

#include <libThirdWrap_Openssl/External.h>

namespace _openssl_ {

	typedef _string_type	_openssl_string;
	
	class Rsa
	{
	public:
		Rsa(void);

		~Rsa(void);

		int Init(const _openssl_string& PrivateKey, const _openssl_string& PublicKey);

		void Release(void);

		_openssl_string PrivateEncrypt(const unsigned char* pData, UInt32 uSize);

		_openssl_string PrivateDecrypt(const unsigned char* pData, UInt32 uSize);

		_openssl_string PublicEncrypt(const unsigned char* pData, UInt32 uSize);

		_openssl_string PublicDecrypt(const unsigned char* pData, UInt32 uSize);

	private:
		_openssl_string _PrivateKey;
		_openssl_string _PublicKey;
	};

}


#endif//__EXTERNAL_20210330111304_1444199361_H__
