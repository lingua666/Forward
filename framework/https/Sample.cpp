
#include <libNetFrame_AsyncStreamClients/StreamClients.h>
#include <libNetFrame_StreamServer/StreamServer.h>
#include <libThirdWrap_Openssl/External.h>
#include <libThirdWrap_Openssl/Rsa.h>
#include <iostream>


#if defined( PLATFORM_OS_FAMILY_WINDOWS )
	#pragma comment(lib,"ws2_32.lib")
	#pragma comment (lib, "crypt32")
#endif

typedef _foundation_::_string_t		_app_string;
typedef _foundation_::_string_big_t	_app_string_big;
typedef _session_::_tcp_::StreamBuf_ptr		StreamBuf_ptr;

#define RECONNECT_INTERVAL		10000//毫秒
#define HTTPS_DATA_MAXSIZE		24576//12K

void HttpsInit(void)
{
	/*
	* 初始化 OpenSSL 库
	*/
	SSL_library_init();
	SSL_load_error_strings();
	OpenSSL_add_all_algorithms();
}

void HttpsRelease(void)
{
	/*
	* 清除 OpenSSL 库
	*/
	ERR_free_strings();
	EVP_cleanup();
	//CRYPTO_cleanup_all_ex_data();
}

class Https
{
public:
	enum ActionType
	{
		EN_TYPE_POST,
		EN_TYPE_GET,
	};

	struct tagHttp_INFO
	{
		ActionType iType;
		_app_string sHost;
		_app_string sIP;
		UInt16 uPort;
		_app_string sUrl;

		tagHttp_INFO(void)
		{}

		tagHttp_INFO(const tagHttp_INFO& t1)
		{
			this->iType = t1.iType;
			this->sHost = t1.sHost;
			this->sIP = t1.sIP;
			this->uPort = t1.uPort;
			this->sUrl = t1.sUrl;
		}

		tagHttp_INFO& operator= (const tagHttp_INFO& t1)
		{
			this->iType = t1.iType;
			this->sHost = t1.sHost;
			this->sIP = t1.sIP;
			this->uPort = t1.uPort;
			this->sUrl = t1.sUrl;
			return *this;
		}
	};

public:
	static int HandleBioCreate(BIO *bio)
	{
		return 1;
	}

	static int HandleBioDestroy(BIO *bio)
	{
		return 1;
	}

	static long HandleBioCtrl(BIO* bio, int cmd, long num, void* ptr)
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
	static int HandleBioRead(BIO *bio, char *out, int outl)
	{
		Https* pObject = (Https*)BIO_get_data(bio);
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

	static int HandleBioWrite(BIO *bio, const char *in, int inl)
	{
		Https* pObject = (Https*)BIO_get_data(bio);
		if (pObject->Send(in, inl) == -1)
		{
			return -1;
		}

		BIO_clear_retry_flags(bio);
		return inl;
	}

	static int HandleBioPuts(BIO *b, const char *str)
	{
		return HandleBioWrite(b, str, strlen(str));
	}

public:
	Https(void)
		: _SslCtx(NULL)
		, _Ssl(NULL)
		, _Bio(NULL)
		, _Method(NULL)
		, _hNet(-1)
		, _ReadPos(0)
		, _isOpen(false)
		, _isAccept(false)
		, _isHandShaked(false)
	{

	}

	~Https(void)
	{

	}

	int Init(bool isAccept)
	{
		_isAccept = isAccept;
		return 1;
	}

	void Release(void)
	{

	}

	int Post(const char* c_szUrl, const _app_string& sData,
		const _app_string& sContentType = "application/json")
	{
		int iRet = OpenSSL();
		if (iRet == -1)
		{//发现错误
			return iRet;
		}

		char* pData = (char*)c_szUrl;
		_Info.iType = EN_TYPE_POST;
		_Info.uPort = 443;
		_Info.sHost = _foundation_::ScanChar::FindValueAndInc(&pData, "://", "/");
		_Info.sUrl = -- pData;
		int iPos = _Info.sHost.find(":");
		if (iPos != -1)
		{
			_Info.uPort = _app_string::StringToInt(_app_string(&_Info.sHost[iPos + 1]));
			_Info.sHost = _app_string(_Info.sHost.c_str(), iPos);
		}

		int iCount = 0;
		_Info.sIP = GetIPByDomain(_Info.sHost.c_str(), &iCount).c_str();

		//连接
		if (GetAsynStreamClientsInstance()->Connect(_Info.sIP.c_str(), _Info.uPort,
			function20_bind(&Https::HandleConnect, this,
				_Info, sData, sContentType, _function_::_1)) == -1)
		{// 错误

		}

		return 1;
	}

	int Close(void)
	{
		_isHandShaked = false;
		if (_isOpen)
		{
			_isOpen = false;
			GetAsynStreamClientsInstance()->Close(_hNet);
		}

		CloseSSL();

		return 1;
	}

	int OpenSSL(void)
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

			if(_Ssl == NULL)
				_Ssl = SSL_new(_SslCtx);

			if (_Method == NULL)
			{
				_Method = BIO_meth_new(BIO_get_new_index() | BIO_TYPE_SOURCE_SINK, "bio_iocp");
				//BIO_meth_set_create(_Method, bio_iocp_create);/* 可选 */
				//BIO_meth_set_destroy(m, bio_iocp_destroy);/* 可选 */
				//BIO_meth_set_puts(m, bio_iocp_puts); /* 可选 */
				BIO_meth_set_ctrl(_Method, HandleBioCtrl); /* 自定义 BIO 必须实现 */
				BIO_meth_set_read(_Method, HandleBioRead);/* 自定义 BIO 必须实现 */
				BIO_meth_set_write(_Method, HandleBioWrite);/* 自定义 BIO 必须实现 */
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

		return 1;
	}

	int CloseSSL(void)
	{
		if (_Bio != NULL)
		{
			BIO_free(_Bio);
			_Bio = NULL;
		}

		if (_SslCtx != NULL)
		{
			SSL_CTX_free(_SslCtx);
			_SslCtx = NULL;
		}

		return 1;
	}

	int WriteSSL(const char* c_szData, UInt32 uSize)
	{
		//test
		printf("WriteSSL(%d) Data:%s\r\n", uSize, c_szData);
		//test

		int w = SSL_write(_Ssl, c_szData, uSize);
		if (w <= 0)
		{
			int err = SSL_get_error(_Ssl, w);
			if (err == SSL_ERROR_WANT_WRITE)
			{
				
			}
			else if (err == SSL_ERROR_ZERO_RETURN)
			{
				return -1;
			}
			else
			{
				return -1;
			}
		}

		return 1;
	}

	NETHANDLE Get(void) const
	{
		return _hNet;
	}

protected:

	const char* ReadData(int iSize)
	{//自动偏移
		if (_Data.size() < _ReadPos + iSize)
			return NULL;

		const char* ptr = &_Data[_ReadPos];
		_ReadPos += iSize;
		return ptr;
	}

	void Clear(void)
	{
		_Data.clear();
		_ReadPos = 0;
	}

	void HandleConnect(const tagHttp_INFO& Info, const _app_string& sData,
		const _app_string& sContentType, NETHANDLE hNet)
	{
		if (hNet == -1)
		{//连接失败
			/*LOG_Print_Error(libApps_CBC_LowTerminal, "CommandSession::HandleConnect(%s:%d) Failed!!", sIP.c_str(), uPort);
			Code = EN_CMD_Connect_Faild;

			//连接失败，重连
			GetTimerEventKMInstance()->push_back(function20_bind(&Https::Connect, this),
				RECONNECT_INTERVAL);
			goto gt_end;*/
		}
		else
		{
			GetAsynStreamClientsInstance()->SetRecvHandle(hNet,
				function20_bind(&Https::HandleHandShaked, this,
					Info, sData, sContentType,
					_function_::_1, _function_::_2, _function_::_3));

			if (GetAsynStreamClientsInstance()->SetDestroyHandle(hNet,
				function20_bind(&Https::HandleDestroy, this, _function_::_1)) != -1)
			{//失败
				/*LOG_Print_Error(libApps_CBC_LowTerminal, "CommandSession::HandleConnect(%s:%d) SetDestroyHandle() Failed!!", sIP.c_str(), uPort);
				Code = EN_CMD_Connect_Faild; 

				//连接失败，重连
				GetTimerEventKMInstance()->push_back(function20_bind(&Https::Connect, this),
					RECONNECT_INTERVAL);
				goto gt_end;*/

				_hNet = hNet;

				//握手
				DoHandShake(Info, sData, sContentType);
			}
		}
	}

	int DoHandShake(const tagHttp_INFO& Info, const _app_string& sData, const _app_string& sContentType)
	{
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
					
				}
				return -1;
			}

			//握手成功
			tagHttp_INFO tmInfo = Info;
			_isHandShaked = true;
			Clear();
			GetAsynStreamClientsInstance()->SetRecvHandle(_hNet,
				function20_bind(&Https::HandleRecv, this,
					Info, sData, sContentType, _function_::_1, _function_::_2, _function_::_3));

			//发送数据
			_app_string s = Generate(tmInfo, sData, sContentType);
			WriteSSL(s.c_str(), s.size());

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

	void HandleHandShaked(const tagHttp_INFO& Info, const _app_string& sData, const _app_string& sContentType,
		NETHANDLE hNet, const StreamBuf_ptr& Buf_ptr, long lRemain)
	{
		_Data.append(Buf_ptr->data, Buf_ptr->payload);
		DoHandShake(Info, sData, sContentType);
	}

	void HandleRecv(const tagHttp_INFO& Info, const _app_string& sData, const _app_string& sContentType,
		NETHANDLE hNet, const StreamBuf_ptr& Buf_ptr, long lRemain)
	{
		char Buf[8192] = {0};
		_Data.append(Buf_ptr->data, Buf_ptr->payload);
		int r = SSL_read(_Ssl, Buf, 8192);
		if (r <= 0)
		{
			int err = SSL_get_error(_Ssl, r);
			if (err == SSL_ERROR_WANT_READ)
			{
				// 使用相同参数下次继续调用
				
			}
			else if (err == SSL_ERROR_ZERO_RETURN)
			{
				// 检测到对方安全关闭 SSL 连接
				
			}
			else
			{
				// 本地错误
				
			}
		}
		else
		{
			printf("HandleRecv(%d) Data:%s\r\n", r, Buf);
		}
	}

	_app_string Generate(const tagHttp_INFO& Info, const _app_string& sData, const _app_string& sContentType)
	{
		_app_string sContent;
		switch (Info.iType)
		{
		case EN_TYPE_POST:
		{
			sContent = "POST " + Info.sUrl + " HTTP/1.1\r\n";
		}
		break;
		case EN_TYPE_GET:
		{
			sContent = "GET " + Info.sUrl + " HTTP/1.1\r\n";
		}
		break;
		}

		sContent += "Host: " + Info.sIP + ":" + _app_string::NumberToStr(Info.uPort) + "\r\n";
		sContent += "referer: ";
		sContent += "https://" + Info.sHost + "/\r\n";
		sContent += "Content-Type: ";
		sContent += sContentType + "\r\n";
		sContent += "Content-Length: " + _app_string::NumberToStr(sData.size()) + "\r\n\r\n";
		if (sData.size() > 0)
		{
			sContent.append(sData.c_str(), sData.size());
			sContent += "\r\n\r\n";
		}

		return sContent.c_str();
	}

	int Send(const char* c_szData, UInt32 uSize)
	{
		return GetAsynStreamClientsInstance()->Send(_hNet, c_szData, uSize);
	}

	/*const _app_string& ProcessSSL(const char* c_szData, UInt32 u32Size)
	{
		//解码
		int iTime = 0, iOffset = 0, iLen = 0, iRet = 0, ssl_error = 0;
		do
		{
			iLen = u32Size - iOffset;
			iRet = BIO_write(_BioRecv, &c_szData[iOffset], iLen);
			if (iRet > 0 && iRet <= iLen)
			{
				iOffset += iRet;
				if (iOffset >= u32Size)
				{//结束
					break;
				}
			}
			else
			{
				ssl_error = SSL_get_error(_Ssl, iRet);
				if (IsSSLError(ssl_error))
				{//错误
					return "";
				}
			}
		} while (++iTime < 100);//超过100次退出，并提示失败

		if (iTime > 100)
		{//遍历多次仍无法解析,报错
			return "";
		}

		char Buffer[HTTPS_DATA_MAXSIZE] = { 0 };

		iTime = 0;
		do
		{
			iRet = SSL_read(_Ssl, Buffer, sizeof(Buffer));
			if (iRet > 0 && iRet <= sizeof(Buffer))
			{
				_Data.append(Buffer, iRet);
			}
			else
			{//
				ssl_error = SSL_get_error(_Ssl, iRet);
				if (IsSSLError(ssl_error))
				{//发生错误
					return "";
				}
			}

		} while (++iTime < 100);

		if (iTime > 100)
		{//遍历多次仍无法解析,报错
			return "";
		}
	}*/

	bool IsSSLError(int ssl_error)
	{
		switch (ssl_error)
		{
		case SSL_ERROR_NONE:
		case SSL_ERROR_WANT_READ:
		case SSL_ERROR_WANT_WRITE:
		case SSL_ERROR_WANT_CONNECT:
		case SSL_ERROR_WANT_ACCEPT:
			return false;

		default: return true;
		}

	}

	void HandleDestroy(NETHANDLE hNet)
	{
		Close();
	}

private:
	tagHttp_INFO	_Info;
	_app_string	_Data;
	int			_ReadPos;
	bool		_isOpen;
	bool		_isAccept;
	bool		_isHandShaked;
	SSL_CTX*	_SslCtx;
	SSL*		_Ssl;
	BIO*		_Bio;
	BIO_METHOD* _Method;
	NETHANDLE	_hNet;
};







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
			} while (ret > 0);

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
	const SSL_METHOD* m_SslMethod = TLSv1_2_method();//TLSv1_method// SSLv23_method;
	SSL_CTX *m_SslCtx = SSL_CTX_new(m_SslMethod);

	//SSL_CTX_set_options(m_SslCtx, SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | SSL_OP_SINGLE_DH_USE | SSL_OP_SINGLE_ECDH_USE);

	//不验证证书
	SSL_CTX_set_verify(m_SslCtx, SSL_VERIFY_NONE, NULL);

	SSL *m_Ssl = SSL_new(m_SslCtx);

	BIO* BioSend = BIO_new(BIO_s_mem());
	BIO* BioRecv = BIO_new(BIO_s_mem());
	SSL_set_bio(m_Ssl, BioRecv, BioSend);

	//异步
	SSL_set_connect_state(m_Ssl);
}

void SetSSLCertificate(void)
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

	//异步
	SSL_set_accept_state(m_Ssl);

	char* EncodeData = "Openssl TLS1.2 Test";
	SSLEncoder(m_Ssl, BioEncoder, EncodeData, strlen(EncodeData));
}


#define DATE_LEN 128

int convert_ASN1TIME(ASN1_TIME *t, char* buf, size_t len)
{
	int rc;
	BIO *b = BIO_new(BIO_s_mem());
	rc = ASN1_TIME_print(b, t);
	if (rc <= 0) {
		printf("fetchdaemon", "ASN1_TIME_print failed or wrote no data.\n");
		BIO_free(b);
		return EXIT_FAILURE;
	}
	rc = BIO_gets(b, buf, len);
	if (rc <= 0) {
		printf("fetchdaemon", "BIO_gets call failed to transfer contents to buf");
		BIO_free(b);
		return EXIT_FAILURE;
	}
	BIO_free(b);
	return EXIT_SUCCESS;
}


void Test(void)
{
	GetAsynStreamClientsInstance()->Init();
	HttpsInit();

	Https	Http;
	Http.Init(false);
	Http.Post("https://nankgd.com/nkytms/getPubKey", _app_string());


	while (true)
	{
		Sleep(1000);
	};

	//test
	FILE *fp = fopen("D:\\http是证书.cer", "rb");
	if (!fp) 
	{
		return ;
	}

	unsigned char* szData = new unsigned char[1024 * 100];
	memset(szData, 0, 1024 * 100);
	int yi = fread(szData, 1, 1024 * 100, fp);
	fclose(fp);
	
	BIO* Bio = BIO_new(BIO_s_mem());
	BIO_puts(Bio, (char*)szData);
	X509 *cert = PEM_read_bio_X509(Bio, NULL, NULL, NULL);
	if (!cert)
	{
		int i = 0;
	}
	BIO_free(Bio);

	//fp = fopen("D:\\http是证书.cer", "rb");
	fp = fopen("D:\\https_out.cer", "rb");
	if (!fp)
	{
		return;
	}

	X509 *cert1 = PEM_read_X509(fp, NULL, NULL, NULL);
	if (!cert1) 
	{
		fclose(fp);
		return ;
	}	

	STACK_OF(X509) *sk = sk_X509_new_null();
	sk_X509_push(sk, cert);

	STACK_OF(X509) *sk1 = sk_X509_new_null();
	sk_X509_push(sk1, cert);

	EVP_PKEY* pubKey = X509_get_pubkey(cert);
	int y = X509_verify(cert1, pubKey);
	if (X509_verify(cert1, pubKey) == 1)
	{
		int yw = 0;
	}
	
	int a_len = sk_X509_num(sk);
	int b_len = sk_X509_num(sk1);
	if (a_len != b_len) 
	{
		
	}

	for (int i = 0; i < a_len; i++)
	{
		if (X509_cmp(sk_X509_value(sk, i), sk_X509_value(sk1, i)))
		{
			int iy = 0;
		}
	}


	/*BIO *bio = BIO_new(BIO_s_mem());
	if (!bio || !PEM_write_bio_X509(bio, cert)) 
	{
		// error handling
	}
	size_t keylen = BIO_pending(bio);
	_string_type key;
	key.resize(keylen);
	int len = BIO_read(bio, &key[0], (int)keylen);
	if (len <= 0) 
	{
		// error handling
	}
	
	FILE* out = fopen("D:\\https_out.cer", "wb");
	if (out != NULL)
	{
		fwrite(key.c_str(), 1, key.size(), out);
	}

	fclose(out);*/

	char* line = NULL;
	// any additional processing would go here..
	line = X509_NAME_oneline(X509_get_subject_name(cert), NULL, 0);
	printf("subject name: %s\n", line);

	line = X509_NAME_oneline(X509_get_issuer_name(cert), NULL, 0);
	printf("issuer name: %s\n", line);

	line = BN_bn2hex(ASN1_INTEGER_to_BN(X509_get_serialNumber(cert), NULL));
	printf("serialNumber: %s\n", line);

	ASN1_TIME *not_before = X509_get_notBefore(cert);
	char not_before_str[DATE_LEN];
	convert_ASN1TIME(not_before, not_before_str, DATE_LEN);
	printf("not_before_str: %s\n", not_before_str);

	ASN1_TIME *not_after = X509_get_notAfter(cert);
	char not_after_str[DATE_LEN];
	convert_ASN1TIME(not_after, not_after_str, DATE_LEN);
	printf("not_after_str: %s\n", not_after_str);

	//公钥算法类型
	//int pkey_nid = OBJ_obj2nid(cert->cert_info->key->algor->algorithm);

	//int Type = X509_certificate_type(cert, X509_get_pubkey(cert));

	/*if (pk)
	{
		if (EVP_PKEY_RSA == pk->type)
		{
			//*pulType = CERT_KEY_ALG_RSA;
		}
		else if (EVP_PKEY_EC == pk->type)
		{
			//*pulType = CERT_KEY_ALG_ECC;
		}
		else if (EVP_PKEY_DSA == pk->type)
		{
			//*pulType = CERT_KEY_ALG_DSA;
		}
		else if (EVP_PKEY_DH == pk->type)
		{
			//*pulType = CERT_KEY_ALG_DH;
		}
		else
		{
			//return CERT_KEY_ALG_UNKNOWN;
		}
	}*/
	

	
	



	/**convert_ASN1TIME(not_after, not_after_str, DATE_LEN)

	int Raw = X509_check_ca(cert);
	if ((Raw & KU_DATA_ENCIPHERMENT) == KU_DATA_ENCIPHERMENT)
	{//加密证书
		int i = 0;
	}
	else if ((Raw & KU_DIGITAL_SIGNATURE) == KU_DIGITAL_SIGNATURE)
	{//签名证书
		int i = 0;
	}*/
	
	//使用者
	line = X509_NAME_oneline(X509_get_subject_name(cert1), NULL, 0);
	printf("subject name: %s\n", line);

	//颁发者
	line = X509_NAME_oneline(X509_get_issuer_name(cert1), NULL, 0);
	printf("issuer name: %s\n", line);

	//序列号
	line = BN_bn2hex(ASN1_INTEGER_to_BN(X509_get_serialNumber(cert1), NULL));
	printf("serialNumber: %s\n", line);

	//签名


	//有效期
	ASN1_TIME *not_before1 = X509_get_notBefore(cert1);
	char not_before_str1[DATE_LEN];
	convert_ASN1TIME(not_before1, not_before_str1, DATE_LEN);
	printf("not_before_str: %s\n", not_before_str1);

	ASN1_TIME *not_after1 = X509_get_notAfter(cert1);
	char not_after_str1[DATE_LEN];
	convert_ASN1TIME(not_after1, not_after_str1, DATE_LEN);
	printf("not_after_str: %s\n", not_after_str1);

	X509_NAME *subj = X509_get_subject_name(cert);
	for (int i = 0; i < X509_NAME_entry_count(subj); i++)
	{
		X509_NAME_ENTRY *e = X509_NAME_get_entry(subj, i);
		ASN1_STRING *d = X509_NAME_ENTRY_get_data(e);
		unsigned char *str = ASN1_STRING_data(d);
		printf("data:%s\r\n", str);
	}

	X509_free(cert);
	//test
}


int main(int argc, char* argv[])
{
	Test();

	std::cin.get();
	return 1;
}