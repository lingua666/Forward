
#ifndef __OPENSSL_SSL_20200727094622_1462848022_H__
#define __OPENSSL_SSL_20200727094622_1462848022_H__

#include <libThirdWrap_Openssl/External.h>

namespace	_openssl_ {

	class Ssl
	{
	public:
		typedef _foundation_::_string_t						_ssl_string;
		typedef _foundation_::_string_big_t					_ssl_string_big;

		enum SSL_STATUS
		{
			EN_Status_Err_HandShake,
			EN_Status_Err_Encode,
			EN_Status_Err_Decode,
			EN_Status_Err_Send,
		};

		typedef function20_handle<int(const char*, UInt32)>	HFNSend;
		typedef function20_handle<void(SSL_STATUS, void*)>	HFNEvent;

	public:
		static _ssl_string GetSubjectName(const X509* Cert);

		static _ssl_string GetIssuerName(const X509* Cert);

		static bool CertificateCompareD(X509* pSrcCert, const char* c_szData);

		static bool CertificateCompareF(X509* pSrcCert, const char* c_szPath);

		static bool IsCertificateExpired(X509* pSrcCert);

		static X509* LoadDataToCert(const char* c_szData);

		static X509* LoadFileToCert(const char* c_szPath);

		static _ssl_string GetCertSerialNumber(X509* pSrcCert);

	protected:
		static int HandleBioCreate(BIO *bio);

		static int HandleBioDestroy(BIO *bio);

		static long HandleBioCtrl(BIO* bio, int cmd, long num, void* ptr);

		// 返回值: >0 实际读取到的数据长度; <=0 发生错误,由 bio_flags 指定原因
		static int HandleBioRead(BIO *bio, char *out, int outl);

		static int HandleBioWrite(BIO *bio, const char *in, int inl);

		static int HandleBioPuts(BIO *b, const char *str);

	public:
		Ssl(void);

		~Ssl(void);

		int Init(bool isAccept, const HFNSend& hSend, const HFNEvent& hEvent);

		void Release(void);

		int Open(void);

		void Close(void);

		int DoHandShake(const char* c_szData = NULL, UInt32 uSize = 0);

		int Encode(const char* c_szData, UInt32 uSize);

		_ssl_string	Decode(const char* c_szData, UInt32 uSize);

		void Clear(void);

		X509*	GetCertificate(void);

		bool IsOpen(void) const
		{
			return	_isOpen;
		}

	protected:
		int Send(const char* c_szData, UInt32 uSize);

		const HFNEvent& GetEvent(void) const;

		const char* ReadData(int iSize);

	private:
		HFNSend		_hSend;
		HFNEvent	_hEvent;
		SSL_CTX*	_SslCtx;
		SSL*		_Ssl;
		BIO*		_Bio;
		CLock		_Lock;
		BIO_METHOD* _Method;
		_ssl_string_big	_Data;
		int			_ReadPos;
		bool		_isAccept;
		bool		_isOpen;
	};
}


#endif//__ROUTEAPPS_20160510104022_1462848022_H__
