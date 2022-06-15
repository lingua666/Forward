
#ifndef __FRAMEWORK_HTTPS_20200727094622_1462848022_H__
#define __FRAMEWORK_HTTPS_20200727094622_1462848022_H__

#include <libFrameWork_Https/External.h>

#if defined( PLATFORM_OS_FAMILY_WINDOWS )
	#pragma comment(lib,"ws2_32.lib")
	#pragma comment (lib, "crypt32")
#endif

namespace	_https_ {

	template<typename T1>
	class Https : public _enable_shared_from_this< Https<T1> >
	{
	public:
		enum StatusCode
		{
			EN_Status_Data_Start,
			EN_Status_Data,
			EN_Status_Data_End,
			EN_Status_Repones,
			EN_Status_Close,
			EN_Status_UpLoad,
			EN_Status_Err_Connect,
			EN_Status_Err_Encode,
			EN_Status_Err_Decode,
			EN_Status_Err_Send
		};

		typedef Https										self_type;
		typedef	SmartPTR<self_type>							self_sptr;
		typedef T1											_http_string;
		typedef _session_::_tcp_::StreamBuf_ptr				StreamBuf_ptr;
		typedef typename _http_::Http<_http_string>::tagHdr_INFO	tagHdr_INFO;
		typedef typename LinkList_type<tagHdr_INFO>			HdrList_type;

		typedef function20_handle<void(const self_sptr&, StatusCode, const char*, UInt32)>	HFNEvent;

		enum ActionType
		{
			EN_TYPE_POST,
			EN_TYPE_GET,
		};

		struct tagHttp_INFO
		{
			ActionType iType;
			_http_string sHost;
			_http_string sIP;
			UInt16 uPort;
			_http_string sUrl;

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

		static _client_::_async_::StreamClients* GetHttpsClientsInstance(void)
		{
			static _client_::_async_::StreamClients* s_pClients = new _client_::_async_::StreamClients();
			return s_pClients;
		}

		static TimerEventKM* GetHttpsTimerEventInstance(void)
		{
			static TimerEventKM* s_pTimerEv = new TimerEventKM();
			return s_pTimerEv;
		}

		static int HttpsClientsEnvironmentInit(UInt8 uIOThreadNum = 0,
			UInt8 uProcThreadNum = 0, UInt32 uWorkMS = 1, UInt32 uDestroyMS = 1,
			UInt32 uTimeThead = 0)
		{
			//test
			LOG_Print_SetLog(libFrameWork_Https, MLog_GetAllLevel());
			//test

			GetHttpsClientsInstance()->Init(uIOThreadNum, uProcThreadNum, true);
			if(uTimeThead > 0)
				GetHttpsTimerEventInstance()->Init(uTimeThead);
			return 1;
		}

		static void HttpsClientsEnvironmentRelease(void)
		{
			GetHttpsClientsInstance()->Release();
			GetHttpsTimerEventInstance()->Release();
		}

		static self_sptr	Alloc(void)
		{
			static MemPool_type s_Pool(sizeof(self_type));
			return	self_sptr(s_Pool.AllocObj<self_type>(),
				function20_bind_Ex(&MemPool_type::FreeObj<self_type>, &s_Pool));
		}

	public:
		Https(void)
			: _hNet(-1)
			, _isOpen(false)
			, _isHandShaked(false)
			, _hFile(NULL)
		{
			_Http = _http_::Http<_http_string>::Alloc();
		}

		~Https(void)
		{
			Release();
			_Http.reset();
		}

		int Init(bool isAccept, const HFNEvent& hEvent = HFNEvent())
		{
			_hEvent = hEvent;
			_Http->Init();
			_Ssl.Init(isAccept,
				function20_bind_Ex(&Https::Send, this->shared_from_this()),
				function20_bind_Ex(&Https::HandleSSLEvent, this->shared_from_this())
				);
			return 1;
		}

		void Release(void)
		{
			Close();
			
			if (_hEvent)
				_hEvent.reset();
		}

		int Post(const char* c_szUrl, const _http_string& sData,
			const _http_string& sContentType = "application/json")
		{
			return Connect(EN_TYPE_POST, c_szUrl, sData, sContentType);
		}

		int PostFile(const char* c_szUrl, const HdrList_type& Hdrs, const _http_string& sFilePath)
		{
			if (!_file_::CMyFile::IsExit(sFilePath.c_str()))
				return -1;

			return ConnectFile(EN_TYPE_POST, c_szUrl, Hdrs, sFilePath);
		}

		int Get(const char* c_szUrl, const _http_string& sData,
			const _http_string& sContentType = "application/json")
		{
			return Connect(EN_TYPE_GET, c_szUrl, sData, sContentType);
		}

		int Close(void)
		{
			_isHandShaked = false;
			if (_isOpen)
			{
				_isOpen = false;
				GetHttpsClientsInstance()->Close(_hNet);
				if (_sEventID != "")
				{
					GetHttpsTimerEventInstance()->Remove(_sEventID);
					_sEventID = "";
				}
			}

			_Ssl.Release();
			return 1;
		}

		bool IsOpen(void)
		{
			return _isOpen;
		}

	protected:

		int Connect(ActionType Type, const char* c_szUrl, const _http_string& sData,
			const _http_string& sContentType)
		{
			int iRet = _Ssl.Open();
			if (iRet == -1)
			{//发现错误
				LOG_Print_Error(libFrameWork_Https, "HandleBioWrite(%d) Failed!\r\n", sData.size());

				if (_hEvent)
					_hEvent(this->shared_from_this(), EN_Status_Err_Connect, NULL, 0);
				return iRet;
			}

			char* pData = (char*)c_szUrl;
			_Info.iType = Type;
			_Info.uPort = 443;
			_Info.sHost = _foundation_::ScanChar::FindValueAndInc(&pData, "://", "/").c_str();
			_Info.sUrl = --pData;
			int iPos = _Info.sHost.find(":");
			if (iPos != -1)
			{
				_Info.uPort = _http_string::StringToInt(_http_string(&_Info.sHost[iPos + 1]));
				_Info.sHost = _http_string(_Info.sHost.c_str(), iPos);
			}

			int iCount = 0;
			_Info.sIP = GetIPByDomain(_Info.sHost.c_str(), &iCount).c_str();

			//连接
			if (GetHttpsClientsInstance()->Connect(_Info.sIP.c_str(), _Info.uPort,
				function20_bind(&Https::HandleConnect, this->shared_from_this(),
					_Info, sData, sContentType, _function_::_1)) == -1)
			{// 错误
				LOG_Print_Error(libFrameWork_Https, "Https::Connect() Failed, Url:%s", c_szUrl);

				if (_hEvent)
					_hEvent(this->shared_from_this(), EN_Status_Err_Connect, NULL, 0);
				return -1;
			}

			return 1;
		}

		void HandleConnect(const tagHttp_INFO& Info, const _http_string& sData,
			const _http_string& sContentType, NETHANDLE hNet)
		{
			if (hNet == -1)
			{//连接失败
				 LOG_Print_Error(libFrameWork_Https, "Https::HandleConnect(%s:%d) Failed!!", Info.sUrl.c_str());
				 if (_hEvent)
					 _hEvent(this->shared_from_this(), EN_Status_Err_Connect, NULL, 0);
			}
			else
			{
				GetHttpsClientsInstance()->SetRecvHandle(hNet,
					function20_bind(&Https::HandleHandShaked, this->shared_from_this(),
						Info, sData, sContentType,
						_function_::_1, _function_::_2, _function_::_3));

				if (GetHttpsClientsInstance()->SetDestroyHandle(hNet,
					function20_bind(&Https::HandleDestroy, this->shared_from_this(), _function_::_1)) != -1)
				{
					_hNet = hNet;

					//握手
					DoHandShake(NULL, 0, Info, sData, sContentType);
				}
			}
		}

		int DoHandShake(const char* c_szData, UInt32 uSize,
			const tagHttp_INFO& Info, const _http_string& sData, const _http_string& sContentType)
		{
			if (_Ssl.DoHandShake(c_szData, uSize) == 1)
			{//握手成功
				_isOpen = true;
				_isHandShaked = true;
				_Ssl.Clear();
				//发送数据
				_http_string s = Generate(Info, sData, sContentType);

				//发送前设置接收函数
				GetHttpsClientsInstance()->SetRecvHandle(_hNet,
					function20_bind(&Https::HandleRecv, this->shared_from_this(),
						Info, sData, sContentType, _function_::_1, _function_::_2, _function_::_3));

				SendData(s.c_str(), s.size());
				return 1;
			}

			return -1;
		}

		void HandleHandShaked(const tagHttp_INFO& Info, const _http_string& sData, const _http_string& sContentType,
			NETHANDLE hNet, const StreamBuf_ptr& Buf_ptr, long lRemain)
		{
			DoHandShake(Buf_ptr->data, Buf_ptr->payload, Info, sData, sContentType);
		}

		void HandleRecv(const tagHttp_INFO& Info, const _http_string& sData, const _http_string& sContentType,
			NETHANDLE hNet, const StreamBuf_ptr& Buf_ptr, long lRemain)
		{
			_openssl_::Ssl::_ssl_string s;
			char* pData = Buf_ptr->data;
			UInt32 uSize = Buf_ptr->payload;

			do 
			{
				s = _Ssl.Decode(pData, uSize);
				if (s.size() > 0)
				{//解析
					//LOG_Print_Info(libFrameWork_Https, "Https::HandleRecv(%d) Data:%s", s.size(), s.c_str());

					_Http->Parse(s.c_str(), s.size(),
						function20_bind_Ex(&Https::HandleHttp, this->shared_from_this()));

					pData = NULL;
					uSize = 0;
				}
				else
					break;
			} while (true);
		}

		_http_string Generate(const tagHttp_INFO& Info, const _http_string& sData, const _http_string& sContentType)
		{
			_http_string sContent;
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

			sContent += "Host: " + Info.sIP + ":" + _http_string::NumberToStr(Info.uPort) + "\r\n";
			sContent += "referer: ";
			sContent += "https://" + Info.sHost + "/\r\n";
			sContent += "Content-Type: ";
			sContent += sContentType + "\r\n";
			sContent += "Content-Length: " + _http_string::NumberToStr(sData.size()) + "\r\n\r\n";
			if (sData.size() > 0)
			{
				sContent.append(sData.c_str(), sData.size());
				sContent += "\r\n\r\n";
			}

			return sContent.c_str();
		}

		int Send(const char* c_szData, UInt32 uSize)
		{
			return GetHttpsClientsInstance()->Send(_hNet, c_szData, uSize);
		}

		int SendData(const char* c_szData, UInt32 uSize)
		{
			return _Ssl.Encode(c_szData, uSize);
		}

		int SendData(const _http_string& Data)
		{
			return _Ssl.Encode(Data.c_str(), Data.size());
		}

		void HandleSslEncoder(_openssl_::Ssl* pSsl, const char* szEncData, UInt32 uSize)
		{
			if (szEncData != NULL)
			{
				Send(szEncData, uSize);
			}
		}

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
			if (_hEvent)
				_hEvent(this->shared_from_this(), EN_Status_Close, NULL, 0);

			if (_hFile != NULL)
			{
				fclose(_hFile);
				_hFile = NULL;
			}
		}

		void HandleSSLEvent(_openssl_::Ssl::SSL_STATUS Status, void* pParamer)
		{
			switch (Status)
			{
			case _openssl_::Ssl::EN_Status_Err_HandShake:
			{
				if (_hEvent)
				{
					_hEvent(this->shared_from_this(), EN_Status_Err_Connect, NULL, 0);
				}
			}
			break;
			case _openssl_::Ssl::EN_Status_Err_Encode:
			{
				if (_hEvent)
				{
					_hEvent(this->shared_from_this(), EN_Status_Err_Encode, NULL, 0);
				}
			}
			break;
			case _openssl_::Ssl::EN_Status_Err_Decode:
			{
				if (_hEvent)
				{
					_hEvent(this->shared_from_this(), EN_Status_Err_Decode, NULL, 0);
				}
			}
			break;
			case _openssl_::Ssl::EN_Status_Err_Send:
			{
				if (_hEvent)
				{
					_hEvent(this->shared_from_this(), EN_Status_Err_Send, NULL, 0);
				}
			}
			break;
			}
		}

		void HandleHttp(const typename _http_::Http<_http_string>::self_sptr& sptr,
			typename _http_::Http<_http_string>::StatusCode Code, void* pParam)
		{
			switch (Code)
			{
			case _http_::Http<_http_string>::StatusCode::EN_CODE_CONNECT_SUCCESS:
			{}
			break;
			case _http_::Http<_http_string>::StatusCode::EN_CODE_DATA_START:
			{//准备接收数据
				if (_hEvent)
				{
					_hEvent(this->shared_from_this(), EN_Status_Data_Start, NULL, 0);
				}
				return;
			}
			break;
			case _http_::Http<_http_string>::StatusCode::EN_CODE_DATA_CHUNK:
			{//接收到数据
				_MEMBUF_PRT* pData = (_MEMBUF_PRT*)pParam;
				if (_hEvent)
				{
					_hEvent(this->shared_from_this(), EN_Status_Data, pData->pBuf, pData->uLen);
				}
				return;
			}
			break;
			case _http_::Http<_http_string>::StatusCode::EN_CODE_DATA_FINISH:
			{//数据接收完成
				if (_hEvent)
				{
					_hEvent(this->shared_from_this(), EN_Status_Data_End, NULL, 0);
				}
				Close();
			}
			break;
			case _http_::Http<_http_string>::StatusCode::EN_CODE_REPONES_SUCCESS:
			{//成功
				if (_hEvent)
				{
					_hEvent(this->shared_from_this(), EN_Status_Data, (char*)pParam, strlen((char*)pParam));
				}
			}
			break;
			case _http_::Http<_http_string>::StatusCode::EN_CODE_CONNECT_FAILED:
			case _http_::Http<_http_string>::StatusCode::EN_CODE_TIMEOUT:
			case _http_::Http<_http_string>::StatusCode::EN_CODE_EXCEPT:
			case _http_::Http<_http_string>::StatusCode::EN_CODE_REPONES_FAILED:
			{//失败

			}
			break;
			}
		}

		int ConnectFile(ActionType Type, const char* c_szUrl,
			const HdrList_type& Hdrs, const _http_string& sFilePath)
		{
			int iRet = _Ssl.Open();
			if (iRet == -1)
			{//发现错误
				LOG_Print_Error(libFrameWork_Https, "Https::ConnectFile() Failed!\r\n");

				if (_hEvent)
					_hEvent(this->shared_from_this(), EN_Status_Err_Connect, NULL, 0);
				return iRet;
			}

			char* pData = (char*)c_szUrl;
			_Info.iType = Type;
			_Info.uPort = 443;
			_Info.sHost = _foundation_::ScanChar::FindValueAndInc(&pData, "://", "/").c_str();
			_Info.sUrl = --pData;
			int iPos = _Info.sHost.find(":");
			if (iPos != -1)
			{
				_Info.uPort = _http_string::StringToInt(_http_string(&_Info.sHost[iPos + 1]));
				_Info.sHost = _http_string(_Info.sHost.c_str(), iPos);
			}

			int iCount = 0;
			_Info.sIP = GetIPByDomain(_Info.sHost.c_str(), &iCount).c_str();

			//连接
			if (GetHttpsClientsInstance()->Connect(_Info.sIP.c_str(), _Info.uPort,
				function20_bind(&self_type::HandleConnectFile, this->shared_from_this(),
					_Info, Hdrs, sFilePath, _function_::_1)) == -1)
			{// 错误
				LOG_Print_Error(libFrameWork_Https, "Https::ConnectFile() Failed, Url:%s", c_szUrl);

				if (_hEvent)
					_hEvent(this->shared_from_this(), EN_Status_Err_Connect, NULL, 0);
				return -1;
			}

			return 1;
		}

		void HandleConnectFile(const tagHttp_INFO& Info, const HdrList_type& Hdrs,
			const _http_string& sFilePath, NETHANDLE hNet)
		{
			if (hNet == -1)
			{//连接失败
				LOG_Print_Error(libFrameWork_Https, "Https::HandleConnectFile(%s:%d) Failed!!", Info.sUrl.c_str());
				if (_hEvent)
					_hEvent(this->shared_from_this(), EN_Status_Err_Connect, NULL, 0);
			}
			else
			{
				GetHttpsClientsInstance()->SetRecvHandle(hNet,
					function20_bind(&Https::HandleFileHandShaked, this->shared_from_this(),
						Info, Hdrs, sFilePath,
						_function_::_1, _function_::_2, _function_::_3));

				if (GetHttpsClientsInstance()->SetDestroyHandle(hNet,
					function20_bind(&Https::HandleDestroy, this->shared_from_this(), _function_::_1)) != -1)
				{
					_hNet = hNet;

					//握手
					FileDoHandShake(NULL, 0, Info, Hdrs, sFilePath);
				}
			}
		}

		int FileDoHandShake(const char* c_szData, UInt32 uSize,
			const tagHttp_INFO& Info, const HdrList_type& Hdrs, const _http_string& sFilePath)
		{
			if (_Ssl.DoHandShake(c_szData, uSize) == 1)
			{//握手成功
				_isOpen = true;
				_isHandShaked = true;
				_Ssl.Clear();

				//发送前设置接收函数
				tagHttp_INFO tmInfo = Info;
				HdrList_type tmHdrs = Hdrs;
				_http_string tmFilePath = sFilePath;
				GetHttpsClientsInstance()->SetRecvHandle(_hNet,
					function20_bind(&Https::HandleRecv, this->shared_from_this(),
						Info, _http_string(), _http_string(), _function_::_1, _function_::_2, _function_::_3));

				//发送数据
				int iRet = SendFileData(tmInfo, tmHdrs, tmFilePath);
				if (iRet == -1)
				{
					LOG_Print_Error(libFrameWork_Https, "Http::HandleConnectFile(%s:%d) Failed!!", tmInfo.sUrl.c_str());
				}
				return 1;
			}

			return -1;
		}

		void HandleFileHandShaked(const tagHttp_INFO& Info, const HdrList_type& Hdrs,
			const _http_string& sFilePath, NETHANDLE hNet, const StreamBuf_ptr& Buf_ptr, long lRemain)
		{
			FileDoHandShake(Buf_ptr->data, Buf_ptr->payload, Info, Hdrs, sFilePath);
		}

		int SendFileData(const tagHttp_INFO& Info, const HdrList_type& Hdrs,
			const _http_string& sFilePath)
		{
			_http_string sBoundary = "----------------------------" + _http_string::NumberToStr(Timestamp_type().utcTime());
			_http_string sContent;
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

			sContent += "Host: " + Info.sIP + /*":" + _http_string::NumberToStr(Info.uPort) +*/ "\r\n";
			sContent += "referer: ";
			sContent += "http://" + Info.sHost + "/\r\n";
			sContent += "Accept-Encoding: gzip, deflate, br\r\n";
			sContent += "Content-Type: multipart/form-data; boundary=";
			sContent += &sBoundary[2];
			sContent += "\r\n";

			UInt64 uFileSize = _file_::CMyFile::GetFileSize(sFilePath.c_str());

			_http_string sData;
			for (typename HdrList_type::iterator iter = Hdrs.begin(); iter != Hdrs.end(); iter++)
			{
				sData += sBoundary + "\r\n";
				sData += "Content-Disposition: form-data; name=\"" + (*iter).Name + "\"\r\n";
				if ((*iter).ContentType != "")
					sData += "Content-Type: " + (*iter).ContentType + "\r\n";

				sData += "\r\n";
				if ((*iter).Value != "")
					sData += (*iter).Value + "\r\n";
			}

			sContent += "Content-Length: " + _http_string::NumberToStr(uFileSize + 2 + sData.size() + sBoundary.size() + 4) + "\r\n\r\n";
			sContent += sData;

			_hFile = fopen(sFilePath.c_str(), "rb");
			if (_hFile != NULL)
			{
				if (GetHttpsClientsInstance()->SetSendCompleteNotify(_hNet,
					function20_bind(&self_type::HandleSend, this->shared_from_this(),
						sBoundary, _function_::_1, _function_::_2)) == -1)
				{
					fclose(_hFile);
					_hFile = NULL;
					return -1;
				}

				return SendData(sContent.c_str(), sContent.size());
			}
			
			return -1;
		}

		void HandleSend(const _http_string& sBoundary, const char* pHasData, UInt32 HasSize)
		{
			if (_hFile != NULL)
			{
				char szData[1460] = { 0 };
				int iCount = fread(szData, 1, sizeof(szData), _hFile);
				if (iCount <= 0)
				{//读取文件完毕
					fclose(_hFile);
					_hFile = NULL;

					_http_string End = "\r\n" + sBoundary + "--\r\n";

					//test
					GetHttpsTimerEventInstance()->push_back(function20_bind(&self_type::SendData,
						this->shared_from_this(), End), 0);
					//test

					//SendData(End.c_str(), End.size());

					if (_sEventID != "")
						GetHttpsTimerEventInstance()->Remove(_sEventID);

					_sEventID = GetHttpsTimerEventInstance()->push_back(function20_bind(&self_type::HandleCloseFile,
						this->shared_from_this()), 1000);

					return;
				}

				//test
				GetHttpsTimerEventInstance()->push_back(function20_bind(&self_type::SendData,
					this->shared_from_this(), _http_string(szData, iCount)), 0);
				//test

				//SendData(szData, iCount);
			}
		}

		void HandleCloseFile(void)
		{
			Close();
			if (_hEvent)
			{
				_hEvent(this->shared_from_this(), EN_Status_UpLoad, NULL, 0);
			}
		}

	private:
		typename _http_::Http<_http_string>::self_sptr	_Http;
		TimerEventKM::_string_	_sEventID;
		HFNEvent		_hEvent;
		tagHttp_INFO	_Info;
		_openssl_::Ssl	_Ssl;
		FILE*			_hFile;
		bool			_isOpen;
		bool			_isHandShaked;
		NETHANDLE		_hNet;
	};
}


#endif//__ROUTEAPPS_20160510104022_1462848022_H__
