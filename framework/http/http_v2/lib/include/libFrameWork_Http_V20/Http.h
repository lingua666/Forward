
#ifndef __FRAMEWORK_HTTP_V20_20200727094622_1462848022_H__
#define __FRAMEWORK_HTTP_V20_20200727094622_1462848022_H__

#include <libFrameWork_Http_V20/External.h>

namespace	_http_ {

	#define HTTP_KEEPALIVED_TIME				5000

	template<typename T1>
	class Http_V20 : public _enable_shared_from_this< Http_V20<T1> >
	{
	public:
		typedef enum StatusCode
		{
			EN_Status_Data_Start,
			EN_Status_Data,
			EN_Status_Data_End,
			EN_Status_Repones,
			EN_Status_Close,
			EN_Status_UpLoad,
			EN_Status_Err_Connect,
			EN_Status_Err_Exception,
			EN_Status_Err_Timeout,
			EN_Status_Err_Encode,
			EN_Status_Err_Decode,
			EN_Status_Err_Send,
			EN_Status_Err_SendFile
		}StatusCode;

		typedef Http_V20							self_type;
		typedef	SmartPTR<self_type>					self_sptr;
		typedef T1									_http_string;
		typedef _session_::_tcp_::StreamBuf_ptr		StreamBuf_ptr;

		typedef function20_handle<void(const self_sptr&, StatusCode, const char*, UInt32)>	HFNEvent;

		typedef struct tagHdr_INFO
		{
			_http_string	Name;
			_http_string	Value;
			_http_string	ContentType;
		};

		typedef typename LinkList_type<tagHdr_INFO>			HdrList_type;

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

		static _client_::_async_::StreamClients* GetHttpClientsInstance(void)
		{
			static _client_::_async_::StreamClients Clients;
			return &Clients;
		}

		static int HttpClientsEnvironmentInit(UInt8 uIOThreadNum = 0, UInt8 uProcThreadNum = 0)
		{
			//test
			LOG_Print_SetLog(libFrameWork_Http_V20, MLog_GetAllLevel());
			//test

			GetHttpClientsInstance()->Init(uIOThreadNum, uProcThreadNum, true);
			return 1;
		}

		static void HttpClientsEnvironmentRelease(void)
		{
			GetHttpClientsInstance()->Release();
		}

		static self_sptr	Alloc(void)
		{
			static MemPool_type s_Pool(sizeof(self_type));
			return	self_sptr(s_Pool.AllocObj<self_type>(),
				function20_bind_Ex(&MemPool_type::FreeObj<self_type>, &s_Pool));
		}

	public:
		Http_V20(void)
			: _hNet(-1)
			, _iStatus(0)
			, _iLen(0)
			, _isOpen(false)
			, _hFile(NULL)
		{
			
		}

		~Http_V20(void)
		{
			Release();
		}

		int Init(const HFNEvent& hEvent)
		{
			_hEvent = hEvent;
			return 1;
		}

		void Release(void)
		{
			Close();
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
			if (_isOpen)
			{
				_isOpen = false;
				GetHttpClientsInstance()->Close(_hNet);
				if (_sEventID != "")
				{
					GetTimerEventKMInstance()->Remove(_sEventID);
					_sEventID = "";
				}
			}

			return 1;
		}

		bool IsOpen(void)
		{
			return _isOpen;
		}

		int Parse(const char* c_szData, UInt32 uSize, const HFNEvent& hEvent)
		{
			const char* c_szEnd = "\r\n\r\n";

			//test
			LOG_Print_Info(libFrameWork_Http_V20, "Http::HandleRecv(%d) Data:%s", uSize, c_szData);
			//test

			_sData.append(c_szData, uSize);
			uSize = _sData.size();
			switch (_iStatus)
			{
			case 0:	//获取头部信息
			{
				int iEnd = _sData.find(c_szEnd);
				if (iEnd != -1)
				{
					_foundation_::_string_t sContentType = _foundation_::ScanChar::FindValue(_sData.c_str(),
						"Content-Type: ", "\r\n");

					_foundation_::_string_t Transfer_Encoding = _foundation_::ScanChar::FindValue(_sData.c_str(),
						"Transfer-Encoding: ", "\r\n");

					if (sContentType.find("octet-stream") != -1 || Transfer_Encoding.find("chunked") != -1)
					{//字节流
						GetTimerEventKMInstance()->Remove(_sEventID);

						_foundation_::_string_t FileName = _foundation_::ScanChar::FindValue(_sData.c_str(),
							"filename=", "\r\n|;");

						if (Transfer_Encoding.find("chunked") != -1)
						{//块
							_iStatus = 1;
							_iLen = 0;
							_sData = _http_string(&_sData[iEnd + strlen(c_szEnd)], _sData.size() - (iEnd + strlen(c_szEnd)));
							hEvent(this->shared_from_this(), EN_Status_Data_Start, FileName.c_str(), FileName.size());
							goto gt_data;
						}
						else
						{
							_foundation_::_string_t Length = _foundation_::ScanChar::FindValue(_sData.c_str(),
								"Content-Length: ", "\r\n");
							_iLen = _foundation_::_string_t::StringToInt(Length);
							if (_iLen > 0)
							{
								_iStatus = 2;
								int iPos = _Info.sUrl.rfind("/");
								if (iPos != -1)
								{
									FileName = &_Info.sUrl[iPos + 1];
								}

								_sData = _http_string(&_sData[iEnd + strlen(c_szEnd)], _sData.size() - (iEnd + strlen(c_szEnd)));
								hEvent(this->shared_from_this(), EN_Status_Data_Start, FileName.c_str(), FileName.size());
								goto gt_data;
							}
						}
					}
					else
					{
						_foundation_::_string_t sContent = _foundation_::ScanChar::FindValue(_sData.c_str(),
							"Content-Length: ", "\r\n");

						if (sContent != "")
						{
							int iCount = _foundation_::_string_t::StringToInt(sContent);
							const char* pEnd = strstr(_sData.c_str(), c_szEnd);
							if (pEnd != NULL)
							{
								_foundation_::_string_t sRanges = _foundation_::ScanChar::FindValue(_sData.c_str(),
									"Accept-Ranges: ", "\r\n");
								if (sRanges == "bytes")
								{//二进制
									GetTimerEventKMInstance()->Remove(_sEventID);

									int iSize = pEnd - _sData.c_str();

									hEvent(this->shared_from_this(), EN_Status_Data_Start, NULL, 0);

									_MEMBUF_PRT Data = { 0 };
									Data.pBuf = (char*)(pEnd + strlen(c_szEnd));
									Data.uLen = uSize - iSize - strlen(c_szEnd);//有效负载数据

									if (uSize - iSize - strlen(c_szEnd) >= iCount)
									{
										hEvent(this->shared_from_this(), EN_Status_Data, Data.pBuf, Data.uLen);
										hEvent(this->shared_from_this(), EN_Status_Data_End, NULL, 0);
										_sData.clear();
										Close();
									}
									else
									{
										if (Data.uLen > 0)
										{
											hEvent(this->shared_from_this(), EN_Status_Data, Data.pBuf, Data.uLen);
											_iLen = iCount - Data.uLen;//有效负载数据
											_iStatus = 2;
											_sData.clear();
										}
									}
								}
								else
								{
									int iSize = pEnd - _sData.c_str();

									if (uSize - iSize - strlen(c_szEnd) >= iCount)
									{
										GetTimerEventKMInstance()->Remove(_sEventID);

										_http_string sStatusCode = _foundation_::ScanChar::FindValue(_sData.c_str(), "HTTP/1.1 ", "\r\n").c_str();

										hEvent(this->shared_from_this(), EN_Status_Repones, pEnd + strlen(c_szEnd), strlen(pEnd + strlen(c_szEnd)));
										_sData.clear();
										Close();
									}
								}

							}
						}
					}
				}
			}
			break;
			case 1:
			{
			gt_data:
				if (_iLen == 0)
				{
					int iPos = _sData.find("\r\n");
					if (iPos != -1)
					{
						_http_string sLength(_sData.c_str(), iPos);
						_iLen = _http_string::StrHexToInt(sLength);
						_iLen = _iLen > 0 ? _iLen + 2 : _iLen;	//块与块之间用回车换行分割，需要+2
						if (_iLen == 0)
						{
							hEvent(this->shared_from_this(), EN_Status_Data_End, NULL, 0);
							_sData.clear();
							_iStatus = 0;
							Close();
						}
						else
						{
							_sData = _http_string(&_sData[iPos + strlen("\r\n")], _sData.size() - (iPos + strlen("\r\n")));
							if (_sData.size() >= _iLen)
							{
								_MEMBUF_PRT Data = { 0 };
								Data.pBuf = (char*)_sData.c_str();
								Data.uLen = _iLen - 2;//有效负载数据

								hEvent(this->shared_from_this(), EN_Status_Data, Data.pBuf, Data.uLen);
								hEvent(this->shared_from_this(), EN_Status_Data_End, NULL, 0);
							}							
						}
					}
				}
				else if (_iLen <= _sData.size())
				{
					_MEMBUF_PRT Data = { 0 };
					Data.pBuf = (char*)_sData.c_str();
					Data.uLen = _iLen - 2;//块与块之间的回车换行不是有效负载数据

					hEvent(this->shared_from_this(), EN_Status_Data, Data.pBuf, Data.uLen);

					_sData = _http_string(&_sData[_iLen], _sData.size() - _iLen);
					_iLen = 0;
					goto gt_data;
				}
			}
			break;
			case 2:
			{
			gt_data2:
				if (_sData.size() > 0)
				{
					_MEMBUF_PRT Data = { 0 };
					Data.pBuf = (char*)_sData.c_str();
					Data.uLen = _sData.size();
					if (_iLen > 0)
					{
						hEvent(this->shared_from_this(), EN_Status_Data, Data.pBuf, Data.uLen);
						_iLen -= _sData.size();
						if (_iLen == 0)
						{
							hEvent(this->shared_from_this(), EN_Status_Data_End, NULL, 0);
							_sData.clear();
							_iStatus = 0;
							Close();
						}
						else if (_iLen < 0)
						{
							LOG_Print_Info(libFrameWork_Http_V20, "Http::HandleRecv(%d,%d) Error", uSize, _iLen);
						}
					}

					_sData.clear();
				}
			}
			break;
			}

			return 1;
		}

	protected:

		int Connect(ActionType Type, const char* c_szUrl, const _http_string& sData,
			const _http_string& sContentType)
		{
			char* pData = (char*)c_szUrl;
			_Info.iType = Type;
			_Info.uPort = 80;
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
			if (GetHttpClientsInstance()->Connect(_Info.sIP.c_str(), _Info.uPort,
				function20_bind(&self_type::HandleConnect, this->shared_from_this(),
					_Info, sData, sContentType, _function_::_1)) == -1)
			{// 错误
				LOG_Print_Error(libFrameWork_Http_V20, "Http::Connect() Failed, Url:%s", c_szUrl);

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
				LOG_Print_Error(libFrameWork_Http_V20, "Http::HandleConnect(%s:%d) Failed!!", Info.sUrl.c_str());
				if (_hEvent)
					_hEvent(this->shared_from_this(), EN_Status_Err_Connect, NULL, 0);
			}
			else
			{
				GetHttpClientsInstance()->SetRecvHandle(hNet,
					function20_bind(&self_type::HandleRecv, this->shared_from_this(),
						Info, sData, sContentType,
						_function_::_1, _function_::_2, _function_::_3));

				if (GetHttpClientsInstance()->SetDestroyHandle(hNet,
					function20_bind(&self_type::HandleDestroy, this->shared_from_this(), _function_::_1)) != -1)
				{
					_hNet = hNet;
					_isOpen = true;

					_sEventID = GetTimerEventKMInstance()->push_back(function20_bind(&self_type::HandleCheck, this->shared_from_this()), HTTP_KEEPALIVED_TIME);

					//发送数据
					_http_string s = Generate(Info, sData, sContentType);
					Send(s.c_str(), s.size());
				}
			}
		}

		_http_string Generate(const tagHttp_INFO& Info, const _http_string& sData,
			const _http_string& sContentType)
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

			sContent += "Host: " + Info.sIP /*+ ":" + _http_string::NumberToStr(Info.uPort)*/ + "\r\n";
			sContent += "referer: ";
			sContent += "http://" + Info.sHost + "/\r\n";
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

		int ConnectFile(ActionType Type, const char* c_szUrl,
			const HdrList_type& Hdrs, const _http_string& sFilePath)
		{
			char* pData = (char*)c_szUrl;
			_Info.iType = Type;
			_Info.uPort = 80;
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
			if (GetHttpClientsInstance()->Connect(_Info.sIP.c_str(), _Info.uPort,
				function20_bind(&self_type::HandleConnectFile, this->shared_from_this(),
					_Info, Hdrs, sFilePath, _function_::_1)) == -1)
			{// 错误
				LOG_Print_Error(libFrameWork_Http_V20, "Http::ConnectFile() Failed, Url:%s", c_szUrl);

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
				LOG_Print_Error(libFrameWork_Http_V20, "Http::HandleConnectFile(%s:%d) Failed!!", Info.sUrl.c_str());
				if (_hEvent)
					_hEvent(this->shared_from_this(), EN_Status_Err_Connect, NULL, 0);
			}
			else
			{
				GetHttpClientsInstance()->SetRecvHandle(hNet,
					function20_bind(&self_type::HandleRecv, this->shared_from_this(),
						Info, "", "", _function_::_1, _function_::_2, _function_::_3));

				if (GetHttpClientsInstance()->SetDestroyHandle(hNet,
					function20_bind(&self_type::HandleDestroy, this->shared_from_this(), _function_::_1)) != -1)
				{
					_hNet = hNet;
					_isOpen = true;

					_sEventID = GetTimerEventKMInstance()->push_back(function20_bind(&self_type::HandleCheck, this->shared_from_this()), HTTP_KEEPALIVED_TIME);

					//发送数据
					int iRet = SendFileData(Info, Hdrs, sFilePath);
					if (iRet == -1)
					{
						LOG_Print_Error(libFrameWork_Http_V20, "Http::HandleConnectFile(%s:%d) Failed!!", Info.sUrl.c_str());
						if (_hEvent)
							_hEvent(this->shared_from_this(), EN_Status_Err_SendFile, NULL, 0);
					}
				}
			}
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
				if((*iter).Value != "")
					sData += (*iter).Value + "\r\n";
			}

			sContent += "Content-Length: " + _http_string::NumberToStr(uFileSize + 2 + sData.size() + sBoundary.size() + 4) + "\r\n\r\n";
			sContent += sData;

			int iRet = Send(sContent.c_str(), sContent.size());
			if (iRet == -1)
				return iRet;

			_hFile = fopen(sFilePath.c_str(), "rb");
			if (_hFile != NULL)
			{
				if (GetHttpClientsInstance()->SetSendCompleteNotify(_hNet,
					function20_bind(&self_type::HandleSend, this->shared_from_this(),
						sBoundary, _function_::_1, _function_::_2)) == -1)
				{
					fclose(_hFile);
					_hFile = NULL;
					return -1;
				}

				char szData[1460] = { 0 };
				int iCount = fread(szData, 1, sizeof(szData), _hFile);
				return Send(szData, iCount);
			}

			return -1;
		}

		int Send(const char* c_szData, UInt32 uSize)
		{
			if (!_isOpen)
				return -1;

			//test
			LOG_Print_Info(libFrameWork_Http_V20, "Http::Send(%d) Data:%s", uSize, c_szData);
			//test
			
			int iRet = GetHttpClientsInstance()->Send(_hNet, c_szData, uSize);
			if (iRet != -1)
				_Time.update();

			return iRet;
		}

		void HandleCheck(void)
		{
			if (_Time.isElapsed(HTTP_KEEPALIVED_TIME))
			{//超时
				LOG_Print_Error(libFrameWork_Http_V20, "Http::HandleCheck(%s:%d) TimeOut", _Info.sIP.c_str(), _Info.uPort);
				_hEvent(this->shared_from_this(), EN_Status_Err_Timeout, NULL, 0);
				Close();
				return;
			}
			else
			{
				_sEventID = GetTimerEventKMInstance()->push_back(function20_bind(&self_type::HandleCheck, this->shared_from_this()), HTTP_KEEPALIVED_TIME);
			}
		}

		void HandleRecv(const tagHttp_INFO& Info, const _http_string& sData, const _http_string& sContentType,
			NETHANDLE hNet, const StreamBuf_ptr& Buf_ptr, long lRemain)
		{
			Parse(Buf_ptr->data, Buf_ptr->payload, _hEvent);
		}

		void HandleDestroy(NETHANDLE hNet)
		{
			if (_isOpen)
			{
				Close();
				_hEvent(this->shared_from_this(), EN_Status_Err_Exception, NULL, 0);
			}

			if (_hFile != NULL)
			{
				fclose(_hFile);
				_hFile = NULL;
			}
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
					Send(End.c_str(), End.size());

					if (_sEventID != "")
					{
						GetTimerEventKMInstance()->Remove(_sEventID);
						_sEventID = GetTimerEventKMInstance()->push_back(function20_bind(&self_type::HandleCloseFile,
							this->shared_from_this()), 1000);
					}

					return;
				}

				Send(szData, iCount);
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
		TimerEventKM::_string_	_sEventID;
		//HdrList_type			_HdrList;
		HFNEvent				_hEvent;
		tagHttp_INFO			_Info;
		NETHANDLE				_hNet;
		Timestamp_type			_Time;
		_http_string			_sData;
		FILE*					_hFile;
		bool					_isOpen;
		int						_iStatus;
		int						_iLen;
	};

}


#endif//__ROUTEAPPS_20160510104022_1462848022_H__
