// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <dllThirdWrap_Openssl/Exports.h>
#include <libThirdWrap_Openssl/External.h>
#include <libThirdWrap_Openssl/Rsa.h>
#include <libThirdWrap_Openssl/Aes.h>
#include <string>

#pragma comment(lib,"ws2_32.lib")
#pragma comment (lib, "crypt32")

/*const char* g_pri = "E:\\key\\signer.pem";// "E:\\openssl\\win64\\release\\bin\\rsa_private_key.pem";//"E:\\key\\private.key";
const char* g_pub = "E:\\openssl\\win64\\release\\bin\\rsa_public_key.pem";// "E:\\key\\public.key";*/

const char* g_Pri = {"-----BEGIN PRIVATE KEY-----\r\nMIICdQIBADANBgkqhkiG9w0BAQEFAASCAl8wggJbAgEAAoGBAIIO7LBtzgQ3bpv0+sOmNwNOuGMVvLTYibDAhgY+Gghghj4T46avJBzo7gm1O/FHkdbzt7dHX/eJWpWdQo3Ar4ysvt8uUmn7qbzbUiejwP+uDCeXyD+BBj1EnSK9LclpiG8NYQMtyck5oLEDvQ4chy/uJcldJka1j/0p76usS7+lAgMBAAECgYBFAMwdUzTR0eMyhYiiU0auz45Oq9LCFH0BNTQmjb5cIXI372+CGTxMxly889YIzpMW+5nGeOz2tX5FEPmloczBRsKjpIETMOVujIbj3lI41RqtqlCnCbX4/ItPrYyu9KrGMpIR3vNtByouUk4KVTGT94eY3SSQOMT3gMw2cLlixQJBAP7Xz9/7PyLOfz3j9cftpY85TSN2Vb0a8L7pEkIPbkKClkgaFheqY4E1u+ZUXHa1wFTlqaCUd8R6UPyaBAC6mPMCQQCCphVIhNTiHSGjT1BO6jQ3UJVub14kGEdE3++FLi5/HdtSpiQ7J1OtkkT4zNc7OaDUocTzude8x8aYZssCrGsHAkBWCaqG5h5EtA7FaGccXqFcn23poRQFSkkpSyr83pTNI6V1Jkl7J+18PBX6B1VIRshuYTNJoZZl4R3iimT7kgP9AkBVWabBz3U7mohlJeVzCHHASTa2BK9dqTa1+g2B1d+uT5T+c2dr9Y8m7017tGZPXKCDCIOgLaKQw3osCctKHdV1AkBkw/zwJAY8uUrsM93O2j0oprOTJUrQBunKEBnS8u4HpO+8sXb+q4f28KJ07YQP2EwP8GwflJh4vDN17NSgAsSI\r\n-----END PRIVATE KEY-----"};
const char* g_Pub = {"-----BEGIN PUBLIC KEY-----\r\nMIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCCDuywbc4EN26b9PrDpjcDTrhjFby02ImwwIYGPhoIYIY+E+OmryQc6O4JtTvxR5HW87e3R1/3iVqVnUKNwK+MrL7fLlJp+6m821Ino8D/rgwnl8g/gQY9RJ0ivS3JaYhvDWEDLcnJOaCxA70OHIcv7iXJXSZGtY/9Ke+rrEu/pQIDAQAB\r\n-----END PUBLIC KEY-----"};
const char* g_Content = {"nkdl2021_openssl_rsa_test(南控电力)"};

_string_type RsaPriEncrypt(const char* c_szContent)
{
	//test
	/*BIO  * key = NULL;
	RSA  * r = NULL;
	key = BIO_new(BIO_s_file());
	BIO_read_filename(key, g_pri);
	r = PEM_read_bio_RSAPrivateKey(key, NULL, NULL, NULL);
	BIO_free_all(key);

	char szData[1024 * 10] = { 0 };
	FILE* fp = fopen(g_pri, "rb");
	fread(szData, 1, sizeof(szData), fp);*/
	//test

	_string_type encrypt_text;
	BIO *keybio = BIO_new_mem_buf((unsigned char *)/*szData*/g_Pri, -1);
	RSA* rsa = RSA_new();
	rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);

	if (!rsa)
	{
		BIO_free_all(keybio);
		return "";
	}

	// 获取RSA单次可以处理的数据的最大长度
	int len = RSA_size(rsa);

	// 申请内存：存贮加密后的密文数据
	/*char *text = new char[len + 1];
	memset(text, 0, len + 1);*/

	char text[1024 * 10] = { 0 };

	// 对数据进行私钥加密（返回值是加密后数据的长度）
	int ret = RSA_private_encrypt(strlen(c_szContent), (const unsigned char*)c_szContent,
		(unsigned char*)text, rsa, RSA_PKCS1_PADDING);

	if (ret >= 0) 
	{
		encrypt_text = _string_type(text, ret);
	}

	// 释放内存  
	//free(text);
	BIO_free_all(keybio);
	RSA_free(rsa);

	return encrypt_text;
}

_string_type RsaPubDecrypt(const _string_type& sContent)
{
	//test
	/*BIO  * key = NULL;
	RSA  * r = NULL;
	key = BIO_new(BIO_s_file());
	BIO_read_filename(key, g_pub);
	r = PEM_read_bio_RSAPrivateKey(key, NULL, NULL, NULL);
	BIO_free_all(key);

	char szData[1024 * 10] = { 0 };
	FILE* fp = fopen(g_pub, "rb");
	fread(szData, 1, sizeof(szData), fp);*/
	//test

	std::string decrypt_text;
	BIO *keybio = BIO_new_mem_buf((unsigned char *)/*szData*/g_Pub, -1);
	RSA *rsa = RSA_new();

	// 注意--------使用第1种格式的公钥进行解密
	//rsa = PEM_read_bio_RSAPublicKey(keybio, &rsa, NULL, NULL);
	// 注意--------使用第2种格式的公钥进行解密（我们使用这种格式作为示例）
	rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
	if (!rsa)
	{
		unsigned long err = ERR_get_error(); //获取错误号
		char err_msg[1024] = { 0 };
		ERR_error_string(err, err_msg); // 格式：error:errId:库:函数:原因
		printf("err msg: err:%ld, msg:%s\n", err, err_msg);
		BIO_free_all(keybio);
		return "";
	}

	int len = RSA_size(rsa);
	char *text = new char[len + 1];
	memset(text, 0, len + 1);
	// 对密文进行解密
	int ret = RSA_public_decrypt(sContent.size(), (const unsigned char*)sContent.c_str(), (unsigned char*)text, rsa, RSA_PKCS1_PADDING);
	if (ret >= 0) 
	{
		decrypt_text.append(std::string(text, ret));
	}

	// 释放内存  
	delete text;
	BIO_free_all(keybio);
	RSA_free(rsa);

	return decrypt_text.c_str();
}


_string_type RsaPriDecrypt(const _string_type& Content)
{
	/*char szData[1024 * 10] = { 0 };
	FILE* fp = fopen(g_pri, "rb");
	fread(szData, 1, sizeof(szData), fp);*/

	_string_type encrypt_text;
	BIO *keybio = BIO_new_mem_buf((unsigned char *)/*szData*/g_Pri, -1);
	RSA* rsa = RSA_new();
	rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);

	if (!rsa)
	{
		BIO_free_all(keybio);
		return "";
	}

	// 获取RSA单次可以处理的数据的最大长度
	int len = RSA_size(rsa);

	// 申请内存：存贮加密后的密文数据
	/*char *text = new char[len + 1];
	memset(text, 0, len + 1);*/

	char text[1024 * 10] = { 0 };

	// 对数据进行私钥加密（返回值是加密后数据的长度）
	int ret = RSA_private_decrypt(Content.size(), (const unsigned char*)Content.c_str(),
		(unsigned char*)text, rsa, RSA_PKCS1_PADDING);

	if (ret >= 0)
	{
		encrypt_text = _string_type(text, ret);
	}

	// 释放内存  
	//free(text);
	BIO_free_all(keybio);
	RSA_free(rsa);

	return encrypt_text;
}

_string_type RsaPubEncrypt(const _string_type sContent)
{
	std::string decrypt_text;
	BIO *keybio = BIO_new_mem_buf((unsigned char *)/*szData*/g_Pub, -1);
	RSA *rsa = RSA_new();

	// 注意--------使用第1种格式的公钥进行解密
	//rsa = PEM_read_bio_RSAPublicKey(keybio, &rsa, NULL, NULL);
	// 注意--------使用第2种格式的公钥进行解密（我们使用这种格式作为示例）
	rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
	if (!rsa)
	{
		unsigned long err = ERR_get_error(); //获取错误号
		char err_msg[1024] = { 0 };
		ERR_error_string(err, err_msg); // 格式：error:errId:库:函数:原因
		printf("err msg: err:%ld, msg:%s\n", err, err_msg);
		BIO_free_all(keybio);
		return "";
	}

	int len = RSA_size(rsa);
	char *text = new char[len + 1];
	memset(text, 0, len + 1);
	// 对密文进行解密

	int ret = RSA_public_encrypt(sContent.size(), (const unsigned char*)sContent.c_str(), (unsigned char*)text, rsa, RSA_PKCS1_PADDING);
	if (ret >= 0)
	{
		decrypt_text.append(std::string(text, ret));
	}

	// 释放内存  
	delete text;
	BIO_free_all(keybio);
	RSA_free(rsa);

	return _string_type(decrypt_text.c_str(), decrypt_text.size());
}

void RsaTest(void)
{
	while (true)
	{
		_string_type sEnc;

		/***********************公钥加密私钥解密*****************************/
		//将内容转utf8
		_string_type se = _string_type(g_Content).gbk_utf8();
		sEnc = RsaPubEncrypt(se);
		if (sEnc != "")
		{
			_string_type s = _common_::base64_encode((unsigned char*)sEnc.c_str(), sEnc.size()).c_str();

			//转字符串
			_string_type sRet = RsaPriDecrypt(sEnc);
			if (sRet == "")
			{
				int i = 0;
			}
			printf("(%d)sRet:%s\r\n", sEnc.size(), sRet.utf8_gbk().c_str());
			//printf("sRet:%s\r\n", sRet.utf8_gbk().c_str());
		}
		/***********************公钥加密私钥解密*****************************/

		/***********************私钥加密公钥解密*****************************/
		_string_type sContent = "CBIaaVqBb0Txki24r8QlKWTBC11j2Pk3ktXBhhcbLVjcWh8w0Wg7mEsokN8KxXQSyNQVo/bzZEd6nAF0WiXvwGA1+W9UL42I98laSfhCmGi7Iqr5Z4e2r70CSRdxxcwDOMbhT5RPuWIQ+kSbh3GEAuoOA4C5cY/w5OQVPvLOgBc=";
		_string_type s = _common_::base64_decode(sContent.c_str()).c_str();
		sEnc = RsaPubDecrypt(s);
		if (sEnc != "")
		{
			_string_type se = sEnc.utf8_gbk();
			printf("se:%s\r\n", se.c_str());
			int i = 0;
		}
		else
		{
			printf("dec failed!\r\n");
		}
		/***********************私钥加密公钥解密*****************************/
	};
}

void LibRsaTest( void )
{
	while (true)
	{
		_openssl_::Rsa	Rs;
		Rs.Init(g_Pri, g_Pub);

		_string_type sEnc;

		/***********************公钥加密私钥解密*****************************/
		//将内容转utf8
		_openssl_::_openssl_string se = _openssl_::_openssl_string(g_Content).gbk_utf8();
		sEnc = Rs.PublicEncrypt((const unsigned char*)se.c_str(), se.size());
		if (sEnc != "")
		{
			_openssl_::_openssl_string s = _common_::base64_encode((unsigned char*)sEnc.c_str(), sEnc.size()).c_str();

			//转字符串
			_string_type sRet = Rs.PrivateDecrypt((const unsigned char*)sEnc.c_str(), sEnc.size());
			if (sRet == "")
			{
				int i = 0;
			}

			printf("(%d)sRet:%s\r\n", sEnc.size(), sRet.utf8_gbk().c_str());
			//printf("sRet:%s\r\n", sRet.utf8_gbk().c_str());
		}
		/***********************公钥加密私钥解密*****************************/

		/***********************私钥加密公钥解密*****************************/
		_openssl_::_openssl_string sContent = "CBIaaVqBb0Txki24r8QlKWTBC11j2Pk3ktXBhhcbLVjcWh8w0Wg7mEsokN8KxXQSyNQVo/bzZEd6nAF0WiXvwGA1+W9UL42I98laSfhCmGi7Iqr5Z4e2r70CSRdxxcwDOMbhT5RPuWIQ+kSbh3GEAuoOA4C5cY/w5OQVPvLOgBc=";
		_openssl_::_openssl_string s = _common_::base64_decode(sContent.c_str()).c_str();
		sEnc = Rs.PublicDecrypt((unsigned char*)s.c_str(), s.size());
		if (sEnc != "")
		{
			_string_type se = sEnc.utf8_gbk();
			printf("se:%s\r\n", se.c_str());
			int i = 0;
		}
		else
		{
			printf("dec failed!\r\n");
		}
		/***********************私钥加密公钥解密*****************************/
	}
}

_string_type SSLEncoder(SSL *pSsl, BIO* pBio, const char* c_szData, UInt32 uSize)
{
	//加密
	_string_type Dst;
	UInt32 uRemain = uSize;
	do 
	{
		int	ret = SSL_write(pSsl, &c_szData[uSize - uRemain], uRemain);
		int	ssl_error = SSL_get_error(pSsl, ret);
		switch (ssl_error)
		{
		case SSL_ERROR_NONE:
		case SSL_ERROR_WANT_READ:
		case SSL_ERROR_WANT_WRITE:
		case SSL_ERROR_WANT_CONNECT:
		case SSL_ERROR_WANT_ACCEPT:
			break;
			/*return false;
			default: return true;*/
		}

		uRemain -= ret;

		char EncoderBuf[1024] = { 0 };
		//获取加密后的数据
		while (BIO_pending(pBio))
		{
			ret = BIO_read(pBio, EncoderBuf, sizeof(EncoderBuf));
			if (ret > 0)
			{//加密成功
				Dst.append(EncoderBuf, ret);
			}
			else
			{
				ssl_error = SSL_get_error(pSsl, ret);
				switch (ssl_error)
				{
				case SSL_ERROR_NONE:
					break;
				case SSL_ERROR_WANT_READ:
				case SSL_ERROR_WANT_WRITE:
				case SSL_ERROR_WANT_CONNECT:
				case SSL_ERROR_WANT_ACCEPT:
				{
						return "";
				}
					break;
					/*return false;
					default: return true;*/
				}
			}
		}

	} while (uRemain > 0);

	return Dst;
}

_string_type SSLDecode(SSL* pSsl, BIO* pBio, const char* c_szData, UInt32 uSize)
{
	_string_type Dst;
	UInt32 uRemain = uSize;
	do 
	{
		int ret = BIO_write(pBio, &c_szData[uSize - uRemain], uRemain);
		if (ret > 0 && ret <= uSize)
		{
			do
			{
				char Decode[1024] = { 0 };
				ret = SSL_read(pSsl, Decode, sizeof(Decode));
				if (ret > 0)
				{
					Dst.append(Decode, ret);
				}
				else
				{
					int ssl_error = SSL_get_error(pSsl, ret);
					switch (ssl_error)
					{
					case SSL_ERROR_NONE:
						break;
					case SSL_ERROR_WANT_READ:
					case SSL_ERROR_WANT_WRITE:
					case SSL_ERROR_WANT_CONNECT:
					case SSL_ERROR_WANT_ACCEPT:
						{
							return "";
						}
						break;
						/*return false;
						default: return true;*/
					}
				}
			}while (ret > 0);

			uRemain -= uSize;
		}
		else
		{
			int ssl_error = SSL_get_error(pSsl, ret);
			switch (ssl_error)
			{
			case SSL_ERROR_NONE:
			case SSL_ERROR_WANT_READ:
			case SSL_ERROR_WANT_WRITE:
			case SSL_ERROR_WANT_CONNECT:
			case SSL_ERROR_WANT_ACCEPT:
				break;
				/*return false;
				default: return true;*/
			}
			return "";
		}

	} while (uRemain > 0);

	return Dst;
}

void CreateClientSSLContext(void)
{
	const SSL_METHOD* m_SslMethod = TLSv1_2_method();//TLSv1_method //SSLv23_method;
	SSL_CTX *m_SslCtx = SSL_CTX_new(m_SslMethod);

	//不验证证书
	SSL_CTX_set_verify(m_SslCtx, SSL_VERIFY_NONE, NULL);

	SSL *m_Ssl = SSL_new(m_SslCtx);

	BIO* BioSend = BIO_new(BIO_s_mem());
	BIO* BioRecv = BIO_new(BIO_s_mem());
	SSL_set_bio(m_Ssl, BioRecv, BioSend);
}

void SetSSLCertificate( void )
{
	/*int length = strlen(server_cert_key_pem);
	BIO *bio_cert = BIO_new_mem_buf((void*)server_cert_key_pem, length);
	X509 *cert = PEM_read_bio_X509(bio_cert, nullptr, nullptr, nullptr);
	EVP_PKEY *pkey = PEM_read_bio_PrivateKey(bio_cert, 0, 0, 0);
	int ret = SSL_CTX_use_certificate(m_SslCtx, cert);

	if (ret != 1)
		throw EXCEPTION(SSLException(m_Ssl, ret));

	ret = SSL_CTX_use_PrivateKey(m_SslCtx, pkey);

	if (ret != 1)
		throw EXCEPTION(SSLException(m_Ssl, ret));

	X509_free(cert);
	EVP_PKEY_free(pkey);
	BIO_free(bio_cert);*/
}

void CreateServerSSLContext(void)
{
	const SSL_METHOD* m_SslMethod = TLSv1_2_server_method();//TLSv1_method// SSLv23_method;
	SSL_CTX *m_SslCtx = SSL_CTX_new(m_SslMethod);

	//不验证证书
	SSL_CTX_set_verify(m_SslCtx, SSL_VERIFY_NONE, NULL);

	SetSSLCertificate();

	SSL *m_Ssl = SSL_new(m_SslCtx);

	BIO* BioEncoder = BIO_new(BIO_s_mem());
	BIO* BioDecoder = BIO_new(BIO_s_mem());
	SSL_set_bio(m_Ssl, BioEncoder, BioDecoder);

	char* EncodeData = "Openssl TLS1.2 Test";
	SSLEncoder(m_Ssl, BioEncoder, EncodeData, strlen(EncodeData));
}

void SSLTest(void)
{
	SSL_library_init();
	SSL_load_error_strings();
}



int AESTest(void)
{
	while(true)
	{
		char* data = "NK-AG-ZHGJ-1";// "{\"macAddress\":\"NK-AG-ZHGJ-210415-0001\",\"battery\": 14,\"newPM10\":  35,\"newPM25\":  24,\"newTemp\": 27,\"newHumidity\": 74,\"pressure\":1009,\"Bat_DataFlg\":1,\"AirCavi_PM25_DataFlg\":1,\"AirCavi_PM10_DataFlg\":1,\"AirCaviTemp_DataFlg\":1,\"AirCaviHumi_DataFlg\":1,\"Pressure_DataFlg\":1,\"Time\":\"2021 - 04 - 25 - 09 - 01 - 30\"}";

		/*int PaddingLen = 15;
		unsigned char *data = (unsigned char *)malloc(AES_BLOCK_SIZE * 3);
		memset((void*)data, 'p', AES_BLOCK_SIZE * 3 + PaddingLen);*/

		_openssl_::Aes AES;
		AES.Init("NanKongAiGanZhiHuiGongJiao210630");
		_openssl_::_openssl_string Enc = AES.Encrypt((unsigned char*)data, strlen((const char*)data));

		_string_type sBase64 = _common_::base64_encode((unsigned char*)Enc.c_str(), Enc.size()).c_str();

		_openssl_::_openssl_string Dec = AES.Decrypt((unsigned char*)Enc.c_str(), Enc.size());
		if (memcmp((void*)Dec.c_str(), data, strlen(Dec.c_str())) == 0)
		{
			printf("Dec(%d):%s\r\n", strlen(Dec.c_str()), Dec.c_str());
			int y = 0;
		}

		int i = 0;

		//std::cin.get();
		Sleep(100);
	};

	{
		unsigned char userKey[AES_BLOCK_SIZE];
		unsigned char *data = (unsigned char *)malloc(AES_BLOCK_SIZE * 3);
		unsigned char *encrypto = (unsigned char *)malloc(AES_BLOCK_SIZE * 3);
		unsigned char *plain = (unsigned char *)malloc(AES_BLOCK_SIZE * 3);
		AES_KEY key;

		memcpy(userKey, "zheshiopensslexq", 16);
		

		//设置加密key及秘钥长度
		//理论上AES加密是根据S盒进行字节替换的，因此原文和密文个字节一一对应大小相同
		AES_set_encrypt_key((const unsigned char *)userKey, AES_BLOCK_SIZE * 8, &key);

		int len = 0;
		//循环加密， 每次加密长度是AES_BLOCK_SIZE长度的数据
		while (len < AES_BLOCK_SIZE * 3)
		{
			AES_encrypt(data + len, encrypto + len, &key);
			len += AES_BLOCK_SIZE;
		}

		//设置解密key及秘钥长度
		AES_set_decrypt_key((const unsigned char *)userKey, AES_BLOCK_SIZE * 8, &key);

		len = 0;
		//循环解密
		//每次输入16字节，输出16字节，如果不够需要填充
		while (len < AES_BLOCK_SIZE * 3)
		{
			AES_decrypt(encrypto + len, plain + len, &key);
			len += AES_BLOCK_SIZE;
		}

		//解密后与元数据是否一致
		if (memcmp(plain, data, AES_BLOCK_SIZE * 3) == 0)
		{
			printf("test success\n");
		}
		else
		{
			printf("test failed\n");
		}
	}

	return 0;
}

void Dll_AES_Test(void)
{
	char Key[] = "NKYT-ZHYD-01-001";
	char InData[] = "{\"Protocol\":{\"CommandInfo\":{\"CommandName\":\"ControlCmd\",\"CommandUID\":\"664\"},\"Content\":{\"DeviceID\":\"NK-ZHLJT-NK-ZHLJT-2\",\"ControlList\":[{\"Type\":\"1\",\"Value\":\"1\",\"Remark\":\"\"}]}}}";
	char OutData[1024] = { 0 };
	int iLen = MOpenssl_AESEncode(Key, InData, sizeof(InData), true, OutData, sizeof(OutData));

	char DecData[1024] = { 0 };
	iLen = MOpenssl_AESDecode(Key, OutData, iLen, true, DecData, sizeof(DecData));
	int i = 0;
}

int main(int argc, char* argv[])
{
	Dll_AES_Test();

	LibRsaTest();

	AESTest();

	SSL_CTX* ctx = NULL;
	SSL* ssl = NULL;
	ctx = SSL_CTX_new(SSLv23_method());
	
	std::cin.get();
	return 0;
}
