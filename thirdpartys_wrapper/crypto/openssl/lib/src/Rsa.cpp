
#include <libThirdWrap_Openssl/Rsa.h>

#if defined( PLATFORM_OS_FAMILY_WINDOWS )
	#pragma comment(lib,"ws2_32.lib")
	#pragma comment (lib, "crypt32")
#endif

namespace _openssl_ {
	
	Rsa::Rsa(void)
	{
		//test
		LOG_Print_SetLog(libThirdWrap_Openssl, MLog_GetAllLevel());
		//test
	}

	Rsa::~Rsa(void)
	{
		Release();
	}

	int Rsa::Init(const _openssl_string& PrivateKey, const _openssl_string& PublicKey)
	{
		OpenSSL_add_all_algorithms();
		OpenSSL_add_all_ciphers();
		ERR_load_crypto_strings();

		_PrivateKey = PrivateKey;
		_PublicKey = PublicKey;
		return 1;
	}

	void Rsa::Release(void)
	{

	}

	_openssl_string Rsa::PrivateEncrypt(const unsigned char* pData, UInt32 uSize)
	{
		_openssl_string encrypt_text;
		BIO *keybio = BIO_new_mem_buf((unsigned char *)_PrivateKey.c_str(), _PrivateKey.size());
		if (keybio == NULL)
		{
			LOG_Print_Error(libThirdWrap_Openssl, "Rsa::PrivateEncrypt() BIO_new_mem_buf is empty");
			return "";
		}

		RSA* rsa = RSA_new();
		if (rsa == NULL)
		{
			LOG_Print_Error(libThirdWrap_Openssl, "Rsa::PrivateEncrypt() RSA_new is empty");
			return "";
		}

		rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
		if (!rsa)
		{
			LOG_Print_Error(libThirdWrap_Openssl,
				"Rsa::PrivateEncrypt() PEM_read_bio_RSAPrivateKey Failed");
			BIO_free_all(keybio);
			return "";
		}

		// ��ȡRSA���ο��Դ�������ݵ���󳤶�
		int len = RSA_size(rsa);

		// �����ڴ棺�������ܺ����������
		encrypt_text.resize(len);

		// �����ݽ���˽Կ���ܣ�����ֵ�Ǽ��ܺ����ݵĳ��ȣ�
		int ret = RSA_private_encrypt(uSize, pData,
			(unsigned char*)encrypt_text.c_str(), rsa, RSA_PKCS1_PADDING);

		if (ret >= 0)
		{
			encrypt_text.update_size(ret);
		}
		else
		{
			LOG_Print_Error(libThirdWrap_Openssl,
				"Rsa::PrivateEncrypt() RSA_private_encrypt Failed, Data(%d):%s",
				uSize, pData);
		}

		// �ͷ��ڴ�
		BIO_free_all(keybio);
		RSA_free(rsa);

		return encrypt_text;
	}

	_openssl_string Rsa::PrivateDecrypt(const unsigned char* pData, UInt32 uSize)
	{
		_openssl_string decrypt_text;
		BIO *keybio = BIO_new_mem_buf((unsigned char *)_PrivateKey.c_str(), _PrivateKey.size());
		if (keybio == NULL)
		{
			LOG_Print_Error(libThirdWrap_Openssl, "Rsa::PrivateDecrypt() BIO_new_mem_buf is empty");
			return "";
		}

		RSA* rsa = RSA_new();
		if (rsa == NULL)
		{
			LOG_Print_Error(libThirdWrap_Openssl, "Rsa::PrivateDecrypt() RSA_new is empty");
			return "";
		}

		rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
		if (!rsa)
		{
			LOG_Print_Error(libThirdWrap_Openssl, "Rsa::PrivateDecrypt() PEM_read_bio_RSAPrivateKey is empty");
			BIO_free_all(keybio);
			return "";
		}

		// ��ȡRSA���ο��Դ�������ݵ���󳤶�
		int len = RSA_size(rsa);

		// �����ڴ棺�������ܺ����������
		decrypt_text.resize(len);

		// �����ݽ���˽Կ���ܣ�����ֵ�Ǽ��ܺ����ݵĳ��ȣ�
		int ret = RSA_private_decrypt(uSize, pData,
			(unsigned char*)decrypt_text.c_str(), rsa, RSA_PKCS1_PADDING);

		if (ret >= 0)
		{
			decrypt_text.update_size(ret);
		}
		else
		{
			LOG_Print_Error(libThirdWrap_Openssl,
				"Rsa::PrivateDecrypt() RSA_private_decrypt Failed, Data(%d):%s",
				uSize, pData);
		}

		// �ͷ��ڴ�  
		BIO_free_all(keybio);
		RSA_free(rsa);

		return decrypt_text;
	}

	_openssl_string Rsa::PublicEncrypt(const unsigned char* pData, UInt32 uSize)
	{
		_openssl_string encrypt_text;
		BIO *keybio = BIO_new_mem_buf((unsigned char *)_PublicKey.c_str(), _PublicKey.size());
		if (keybio == NULL)
		{
			LOG_Print_Error(libThirdWrap_Openssl, "Rsa::PublicEncrypt() BIO_new_mem_buf is empty");
			return "";
		}

		RSA *rsa = RSA_new();
		if (rsa == NULL)
		{
			LOG_Print_Error(libThirdWrap_Openssl, "Rsa::PublicEncrypt() RSA_new is empty");
			return "";
		}

		// ע��--------ʹ�õ�1�ָ�ʽ�Ĺ�Կ���н���
		//rsa = PEM_read_bio_RSAPublicKey(keybio, &rsa, NULL, NULL);
		// ע��--------ʹ�õ�2�ָ�ʽ�Ĺ�Կ���н��ܣ�����ʹ�����ָ�ʽ��Ϊʾ����
		rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
		if (!rsa)
		{
			unsigned long err = ERR_get_error(); //��ȡ�����
			char err_msg[1024] = { 0 };
			ERR_error_string(err, err_msg); // ��ʽ��error:errId:��:����:ԭ��
			printf("err msg: err:%ld, msg:%s\n", err, err_msg);
			BIO_free_all(keybio);
			return "";
		}

		int len = RSA_size(rsa);

		// �����ڴ棺�������ܺ����������
		encrypt_text.resize(len);

		// �����Ľ��н���

		int ret = RSA_public_encrypt(uSize, (const unsigned char*)pData, (unsigned char*)encrypt_text.c_str(), rsa, RSA_PKCS1_PADDING);
		if (ret >= 0)
		{
			encrypt_text.update_size(ret);
		}
		else
		{
			LOG_Print_Error(libThirdWrap_Openssl,
				"Rsa::PublicEncrypt() RSA_public_encrypt Failed, Data(%d):%s",
				uSize, pData);
		}

		// �ͷ��ڴ�  
		BIO_free_all(keybio);
		RSA_free(rsa);

		return encrypt_text;
	}

	_openssl_string Rsa::PublicDecrypt(const unsigned char* pData, UInt32 uSize)
	{
		_openssl_string decrypt_text;
		BIO *keybio = BIO_new_mem_buf((unsigned char *)_PublicKey.c_str(), _PublicKey.size());
		if (keybio == NULL)
		{
			LOG_Print_Error(libThirdWrap_Openssl, "Rsa::PublicEncrypt() BIO_new_mem_buf is empty");
			return "";
		}

		RSA *rsa = RSA_new();
		if (rsa == NULL)
		{
			LOG_Print_Error(libThirdWrap_Openssl, "Rsa::PublicEncrypt() RSA_new is empty");
			return "";
		}

		// ע��--------ʹ�õ�1�ָ�ʽ�Ĺ�Կ���н���
		//rsa = PEM_read_bio_RSAPublicKey(keybio, &rsa, NULL, NULL);
		// ע��--------ʹ�õ�2�ָ�ʽ�Ĺ�Կ���н��ܣ�����ʹ�����ָ�ʽ��Ϊʾ����
		rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
		if (!rsa)
		{
			unsigned long err = ERR_get_error(); //��ȡ�����
			char err_msg[1024] = { 0 };
			ERR_error_string(err, err_msg); // ��ʽ��error:errId:��:����:ԭ��
			printf("err msg: err:%ld, msg:%s\n", err, err_msg);
			BIO_free_all(keybio);
			return "";
		}

		int len = RSA_size(rsa);

		// �����ڴ棺�������ܺ����������
		decrypt_text.resize(len);

		// �����Ľ��н���
		int ret = RSA_public_decrypt(uSize, (const unsigned char*)pData, (unsigned char*)decrypt_text.c_str(), rsa, RSA_PKCS1_PADDING);
		if (ret >= 0)
		{
			decrypt_text.update_size(ret);
		}
		else
		{
			LOG_Print_Error(libThirdWrap_Openssl,
				"Rsa::PublicDecrypt() RSA_public_decrypt Failed, Data(%d):%s",
				uSize, pData);
		}

		// �ͷ��ڴ�
		BIO_free_all(keybio);
		RSA_free(rsa);
		return decrypt_text;
	}

}

