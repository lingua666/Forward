
#include <libThirdWrap_Openssl/Aes.h>

namespace _openssl_ {

	Aes::Aes(void)
	{

	}

	Aes::~Aes(void)
	{
		Release();
	}

	int Aes::Init(const _openssl_string& UserKey)
	{
		int iSize = UserKey.size() * 8;
		if (iSize != 128 && iSize != 192 && iSize != 256)
		{
			return -1;
		}

		_UserKey = UserKey;
		return 1;
	}

	void Aes::Release(void)
	{

	}

	_openssl_string Aes::Encrypt(const unsigned char* pData, UInt32 uSize)
	{
		if (isVaild())
			return "";

		_openssl_string	sRet;
		sRet.resize(uSize + AES_BLOCK_SIZE);

		//设置加密key及秘钥长度
		AES_KEY key;
		AES_set_encrypt_key((const unsigned char *)_UserKey.c_str(), _UserKey.size() * 8, &key);

		bool isPadding = false;
		UInt32 uLen = uSize % AES_BLOCK_SIZE;
		if (uLen > 0)
			isPadding = true;

		UInt32 uOffset = 0;
		while (uOffset < (uSize - uLen))
		{
			AES_encrypt(pData + uOffset, (unsigned char*)&sRet[uOffset], &key);
			uOffset += AES_BLOCK_SIZE;
		};

		if (isPadding)
		{//补0
			unsigned char Padding[AES_BLOCK_SIZE] = { 0 };
			memcpy(Padding, pData + (uSize - uLen), uLen);
			memset(&Padding[uLen], 0, AES_BLOCK_SIZE - uLen);
			AES_encrypt(Padding, (unsigned char*)&sRet[uOffset], &key);
			sRet.update_size(uSize + AES_BLOCK_SIZE - uLen);
		}
		else
		{
			sRet.update_size(uSize);
		}

		return sRet;
	}

	_openssl_string Aes::Decrypt(const unsigned char* pData, UInt32 uSize)
	{
		if (isVaild())
			return "";

		_openssl_string	sRet;
		sRet.resize(uSize + AES_BLOCK_SIZE);

		//设置解密key及秘钥长度
		AES_KEY key;
		AES_set_decrypt_key((const unsigned char *)_UserKey.c_str(), _UserKey.size() * 8, &key);

		bool isPadding = false;
		UInt32 uLen = uSize % AES_BLOCK_SIZE;
		if (uLen > 0)
			isPadding = true;

		UInt32 uOffset = 0;
		while (uOffset < (uSize - uLen))
		{
			AES_decrypt(pData + uOffset, (unsigned char*)&sRet[uOffset], &key);
			uOffset += AES_BLOCK_SIZE;
		};

		if (isPadding)
		{//补0
			unsigned char Padding[AES_BLOCK_SIZE] = { 0 };
			memcpy(Padding, pData + (uSize - uLen), uLen);
			memset(&Padding[uLen], 0, AES_BLOCK_SIZE - uLen);
			AES_decrypt(Padding, (unsigned char*)&sRet[uOffset], &key);
			sRet.update_size(uSize + AES_BLOCK_SIZE - uLen);
		}
		else
		{
			sRet.update_size(uSize);
		}

		return sRet;
	}

	bool Aes::isVaild(void)
	{
		int iSize = _UserKey.size() * 8;
		if (iSize != 128 && iSize != 192 && iSize != 256)
		{
			return true;
		}

		return false;
	}

}

