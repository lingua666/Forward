
#include <libThirdWrap_Openssl/Ssl.h>

#if defined( PLATFORM_OS_FAMILY_WINDOWS )
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "crypt32")
#endif

namespace	_openssl_ {

	int convert_ASN1TIME(ASN1_TIME *t, char* buf, size_t len)
	{
		int rc;
		BIO *b = BIO_new(BIO_s_mem());
		rc = ASN1_TIME_print(b, t);
		if (rc <= 0) 
		{
			LOG_Print_Info(libThirdWrap_Openssl, "fetchdaemon", "ASN1_TIME_print failed or wrote no data.\n");
			BIO_free(b);
			return -1;
		}

		rc = BIO_gets(b, buf, len);
		if (rc <= 0) 
		{
			LOG_Print_Info(libThirdWrap_Openssl, "fetchdaemon", "BIO_gets call failed to transfer contents to buf");
			BIO_free(b);
			return -1;
		}
		BIO_free(b);
		return 1;
	}

	class SSL_Init
	{
	public:
		SSL_Init(void)
		{
			/*
			* 初始化 OpenSSL 库
			*/
			SSL_library_init();
			SSL_load_error_strings();
			OpenSSL_add_all_algorithms();
		}

		~SSL_Init(void)
		{
			/*
			* 清除 OpenSSL 库
			*/
			ERR_free_strings();
			EVP_cleanup();
			//CRYPTO_cleanup_all_ex_data();
		}
	};

	static SSL_Init	g_SSlInit202202211724;

	int Ssl::HandleBioCreate(BIO *bio)
	{
		return 1;
	}

	int Ssl::HandleBioDestroy(BIO *bio)
	{
		return 1;
	}

	long Ssl::HandleBioCtrl(BIO* bio, int cmd, long num, void* ptr)
	{
		int ret = 0;
		switch (cmd)
		{
		case BIO_CTRL_FLUSH:
		{
			ret = 1;
			break;
		}
		default:
			ret = 0;
		}
		return ret;
	}

	// 返回值: >0 实际读取到的数据长度; <=0 发生错误,由 bio_flags 指定原因
	int Ssl::HandleBioRead(BIO *bio, char *out, int outl)
	{
		Ssl* pObject = (Ssl*)BIO_get_data(bio);
		int ret = -1;
		if (out != NULL)
		{
			const char* ptr = pObject->ReadData(outl);
			BIO_clear_retry_flags(bio);
			if (ptr == NULL)
			{
				BIO_set_retry_read(bio);
			}
			else
			{
				ret = outl;
				memcpy(out, ptr, ret);
			}
		}
		return ret;
	}

	int Ssl::HandleBioWrite(BIO *bio, const char *in, int inl)
	{
		Ssl* pObject = (Ssl*)BIO_get_data(bio);
		if (pObject->IsOpen() && pObject->Send(in, inl) == -1)
		{//发送数据
			pObject->GetEvent()(EN_Status_Err_Send, NULL);
			return -1;
		}

		BIO_clear_retry_flags(bio);
		return inl;
	}

	int Ssl::HandleBioPuts(BIO *b, const char *str)
	{
		return HandleBioWrite(b, str, strlen(str));
	}

	Ssl::_ssl_string Ssl::GetSubjectName(const X509* Cert)
	{
		if (Cert == NULL)
			return "";

		_ssl_string Name;
		X509_NAME* Ptr = X509_get_subject_name(Cert);
		if (Ptr != NULL)
		{
			char* line = X509_NAME_oneline(Ptr, NULL, 0);
			if (line != NULL)
			{
				Name = line;
				OPENSSL_free(line);
			}
		}

		return Name;
	}

	Ssl::_ssl_string Ssl::GetIssuerName(const X509* Cert)
	{
		if (Cert == NULL)
			return "";

		_ssl_string Name;
		X509_NAME* Ptr = X509_get_issuer_name(Cert);
		if (Ptr != NULL)
		{
			char* line = X509_NAME_oneline(Ptr, NULL, 0);
			if (line != NULL)
			{
				Name = line;
				OPENSSL_free(line);
			}
		}

		return Name;
	}

	X509* Ssl::LoadDataToCert(const char* c_szData)
	{
		if (c_szData == NULL)
			return NULL;

		BIO* Bio = BIO_new(BIO_s_mem()); //Bio = BIO_new_mem_buf((void*)cert_data, cert_data_size);
		BIO_puts(Bio, c_szData);
		X509 *Cert = PEM_read_bio_X509(Bio, NULL, NULL, NULL);
		BIO_free(Bio);
		return Cert;
	}

	X509* Ssl::LoadFileToCert(const char* c_szPath)
	{
		if (c_szPath == NULL)
			return NULL;

		FILE* Fp = fopen(c_szPath, "rb");
		if (Fp == NULL)
			return NULL;

		X509 *Cert1 = PEM_read_X509(Fp, NULL, NULL, NULL);
		fclose(Fp);
		return Cert1;
	}

	bool Ssl::CertificateCompareD(X509* pSrcCert, const char* c_szData)
	{
		bool bRet = false;
		if (c_szData == NULL || pSrcCert == NULL)
			return bRet;

		X509 *pDstCert = LoadDataToCert(c_szData);
		if (pDstCert != NULL && X509_cmp(pSrcCert, pDstCert) == 0)
			bRet = true;

		if(pDstCert != NULL)
			X509_free(pDstCert);
		return bRet;
	}

	bool Ssl::CertificateCompareF(X509* pSrcCert, const char* c_szPath)
	{
		bool bRet = false;
		if (c_szPath == NULL || pSrcCert == NULL)
			return bRet;

		X509 *pDstCert = LoadFileToCert(c_szPath);
		if (pDstCert != NULL && X509_cmp(pSrcCert, pDstCert) == 0)
			bRet = true;

		if (pDstCert != NULL)
			X509_free(pDstCert);
		return bRet;
	}

	Ssl::_ssl_string Ssl::GetCertSerialNumber(X509* pSrcCert)
	{
		_ssl_string sRet;
		if (pSrcCert == NULL)
			return sRet;

		ASN1_INTEGER* pAsn1 = X509_get_serialNumber(pSrcCert);
		if (pAsn1 != NULL)
		{
			BIGNUM* pNum = ASN1_INTEGER_to_BN(pAsn1, NULL);
			if (pNum != NULL)
			{
				char* line = BN_bn2hex(pNum);
				BN_free(pNum);
				if (line != NULL)
				{
					sRet = line;
					OPENSSL_free(line);
				}
			}
		}

		return sRet;
	}

	bool Ssl::IsCertificateExpired(X509* pSrcCert)
	{
		ASN1_TIME *not_before = X509_get_notBefore(pSrcCert);
		char not_before_str[128];

		convert_ASN1TIME(not_before, not_before_str, 128);
		printf("not_before_str: %s\n", not_before_str);

		ASN1_TIME *not_after = X509_get_notAfter(pSrcCert);
		char not_after_str[128];
		convert_ASN1TIME(not_after, not_after_str, 128);
		printf("not_after_str: %s\n", not_after_str);

		return true;
	}


	Ssl::Ssl(void)
		: _SslCtx(NULL)
		, _Ssl(NULL)
		, _Bio(NULL)
		, _Method(NULL)
		, _ReadPos(0)
		, _isAccept(false)
		, _isOpen(false)
	{

	}

	Ssl::~Ssl(void)
	{
		Release();
	}

	int Ssl::Init(bool isAccept, const HFNSend& hSend, const HFNEvent& hEvent)
	{
		_isAccept = isAccept;
		_hSend = hSend;
		_hEvent = hEvent;
		return 1;
	}

	void Ssl::Release(void)
	{
		Close();
		if(_hSend)
			_hSend.reset();

		if(_hEvent)
			_hEvent.reset();
	}

	int Ssl::Open(void)
	{
		if (_SslCtx == NULL)
		{
			//服务器端
			if (_isAccept)
			{
				_SslCtx = SSL_CTX_new(SSLv23_server_method());
			}
			else
			{
				//客户端
				_SslCtx = SSL_CTX_new(SSLv23_method());
			}

			if (_Ssl == NULL)
			{
				_Ssl = SSL_new(_SslCtx);
			}

			if (_Method == NULL)
			{
				_Method = BIO_meth_new(BIO_get_new_index() | BIO_TYPE_SOURCE_SINK, "bio_iocp");
				//BIO_meth_set_create(_Method, bio_iocp_create);// 可选 
				//BIO_meth_set_destroy(m, bio_iocp_destroy);// 可选 
				//BIO_meth_set_puts(m, bio_iocp_puts); // 可选
				BIO_meth_set_ctrl(_Method, HandleBioCtrl); // 自定义 BIO 必须实现 
				BIO_meth_set_read(_Method, HandleBioRead);// 自定义 BIO 必须实现 
				BIO_meth_set_write(_Method, HandleBioWrite);// 自定义 BIO 必须实现 
			}

			if (_Bio == NULL)
			{
				_Bio = BIO_new(_Method);
				BIO_set_data(_Bio, (void*)this);
				BIO_set_init(_Bio, 1);
				BIO_set_shutdown(_Bio, 0);
			}

			SSL_set_bio(_Ssl, _Bio, _Bio);

			if (_isAccept)
			{
				// 通过 accept 生成的实例认为是服务端,设置为在 SSL_Read 时自动握手
				SSL_set_accept_state(_Ssl);
			}
			else
			{
				SSL_set_connect_state(_Ssl);
			}
		}

		_isOpen = true;
		return 1;
	}

	void Ssl::Close(void)
	{
		_Lock.Lock();
		if (_isOpen)
		{
			_isOpen = false;
			_Lock.UnLock();
		}
		else
		{
			_Lock.UnLock();
			return;
		}

		if (_Ssl != NULL)
		{
			int shr = SSL_shutdown(_Ssl);
			int err = SSL_get_error(_Ssl, shr);
			if (SSL_ERROR_WANT_READ == err)
			{
			}
			else if (SSL_ERROR_WANT_WRITE == err)
			{

			}
			else
			{//关闭错误
				//LOG_Print_Error(libThirdWrap_Openssl, "Ssl::Close(%d) Failed!\r\n", err);
			}

			SSL_free(_Ssl);
			_Ssl = NULL;
		}

		if (_Method != NULL)
		{
			BIO_meth_free(_Method);
			_Method = NULL;
		}

		if (_SslCtx != NULL)
		{
			SSL_CTX_free(_SslCtx);
			_SslCtx = NULL;
		}
	}

	int Ssl::DoHandShake(const char* c_szData, UInt32 uSize)
	{
		if (!IsOpen())
			return -1;

		if (c_szData != NULL)
		{
			_Lock.Lock();
			_Data.append(c_szData, uSize);
			_Lock.UnLock();
		}

		if (!SSL_is_init_finished(_Ssl))
		{//错误
			int iRet = SSL_do_handshake(_Ssl);
			if (iRet != 1)
			{
				int ssl_error = SSL_get_error(_Ssl, iRet);
				if (ssl_error == SSL_ERROR_WANT_WRITE)
				{// 需要等待一次写入事件以继续完成握手

				}
				else if (ssl_error == SSL_ERROR_WANT_READ)
				{// 需要等待一次读取事件以继续完成握手

				}
				else
				{//握手出错
					LOG_Print_Error(libThirdWrap_Openssl, "Ssl::DoHandShake(%d) Failed!\r\n", ssl_error);

					if (_hEvent)
						_hEvent(EN_Status_Err_HandShake, NULL);
				}
				return -1;
			}

			// 查看对方的证书
			X509* cert = NULL;
			char* line = NULL;
			cert = SSL_get_peer_certificate(_Ssl);
			if (cert)
			{
				printf("certificate:\n");

				line = X509_NAME_oneline(X509_get_subject_name(cert), NULL, 0);
				printf("subject name: %s\n", line);

				line = X509_NAME_oneline(X509_get_issuer_name(cert), NULL, 0);
				printf("issuer name: %s\n", line);

				STACK_OF(X509) *sk = sk_X509_new_null();
				sk_X509_push(sk, cert);

				X509_free(cert);
			}
			else
			{
				printf("no certificate received\n");
			}
		}

		return 1;
	}

	int Ssl::Encode(const char* c_szData, UInt32 uSize)
	{
		if (!IsOpen())
			return -1;

		int w = SSL_write(_Ssl, c_szData, uSize);
		if (w <= 0)
		{
			int err = SSL_get_error(_Ssl, w);
			if (err == SSL_ERROR_WANT_WRITE)
			{

			}
			else if (err == SSL_ERROR_ZERO_RETURN)
			{// 检测到对方安全关闭 SSL 连接
				LOG_Print_Error(libThirdWrap_Openssl, "Ssl::Encode(%d) The Other Side is Closed error:%d!\r\n", err);
				
				if (_hEvent)
					_hEvent(EN_Status_Err_Encode, NULL);

				return -1;
			}
			else
			{	//本地错误
				LOG_Print_Error(libThirdWrap_Openssl, "Ssl::Encode(%d) Failed, error:%d!\r\n", err);

				if (_hEvent)
					_hEvent(EN_Status_Err_Encode, NULL);
				return -1;
			}
		}

		return 1;
	}

	Ssl::_ssl_string	Ssl::Decode(const char* c_szData, UInt32 uSize)
	{
		if (!IsOpen())
			return "";

		char Buf[8192] = { 0 };
		if (c_szData != NULL)
		{
			_Lock.Lock();
			_Data.append(c_szData, uSize);
			_Lock.UnLock();
		}

		int r = SSL_read(_Ssl, Buf, 8192);
		if (r <= 0)
		{
			int err = SSL_get_error(_Ssl, r);
			if (err == SSL_ERROR_WANT_READ)
			{// 使用相同参数下次继续调用
				return "";
			}
			else if (err == SSL_ERROR_ZERO_RETURN)
			{// 检测到对方安全关闭 SSL 连接
				LOG_Print_Error(libThirdWrap_Openssl, "Ssl::Decode(%d) The Other Side is Closed error:%d!\r\n", err);

				if (_hEvent)
					_hEvent(EN_Status_Err_Decode, NULL);
				return "";
			}
			else
			{// 本地错误
				LOG_Print_Error(libThirdWrap_Openssl, "Ssl::Decode(%d) Failed, error:%d!\r\n", err);

				if (_hEvent)
					_hEvent(EN_Status_Err_Decode, NULL);
				return "";
			}
		}

		return _ssl_string(Buf, r);
	}

	void Ssl::Clear(void)
	{
		_Lock.Lock();
		_Data.clear();
		_Lock.UnLock();
		_ReadPos = 0;
	}

	X509*	Ssl::GetCertificate(void)
	{
		X509* Cert = NULL;
		if (IsOpen())
			Cert = SSL_get_peer_certificate(_Ssl);
		return Cert;
	}

	int Ssl::Send(const char* c_szData, UInt32 uSize)
	{
		if (!_hSend)
			return -1;
		return _hSend(c_szData, uSize);
	}

	const Ssl::HFNEvent& Ssl::GetEvent(void) const
	{
		return _hEvent;
	}

	const char* Ssl::ReadData(int iSize)
	{//自动偏移
		if (_Data.size() < _ReadPos + iSize)
			return NULL;
		
		int iRemain = _Data.size() - _ReadPos;
		if (_ReadPos > 0)
		{
			_Lock.Lock();
			std::memmove(&_Data[0], &_Data[_ReadPos], iRemain);
			_Data.update_size(iRemain);
			_Lock.UnLock();
			_ReadPos = 0;
		}

		const char* ptr = &_Data[_ReadPos];
		_ReadPos += iSize;
		return ptr;
	}
}


