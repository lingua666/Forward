
#ifndef __FRAMEWORK_HTTP_20200727094622_1462848022_H__
#define __FRAMEWORK_HTTP_20200727094622_1462848022_H__

#include <libFrameWork_Http/External.h>

namespace	_http_ {

	#define HTTP_KEEPALIVED_TIME				5000

	template<typename T1>
	class Http : public _enable_shared_from_this< Http<T1> >
	{
	public:
		typedef enum StatusCode
		{
			EN_CODE_CONNECT_SUCCESS,
			EN_CODE_REPONES_SUCCESS,
			EN_CODE_CONNECT_FAILED,
			EN_CODE_TIMEOUT,
			EN_CODE_EXCEPT,
			EN_CODE_REPONES_FAILED,
			EN_CODE_DATA_START,
			EN_CODE_DATA_CHUNK,
			EN_CODE_DATA_FINISH,
		}StatusCode;

		typedef T1									_http_string;

		typedef struct tagHost_INFO
		{
			_http_string	IP;
			UInt16			uPort;
			_http_string	Host;
			_http_string	Url;
		};

		typedef struct tagHdr_INFO
		{
			_http_string	Name;
			_http_string	Value;
			_http_string	ContentType;
		};

		typedef Http<_http_string>					self_type;
		typedef SmartPTR< self_type >				self_sptr;
		typedef _session_::_tcp_::StreamBuf_ptr		StreamBuf_ptr;
		typedef function20_handle<void(const self_sptr&, StatusCode, void*)>		HEvent;
		typedef typename LinkList_type<tagHdr_INFO>			HdrList_type;

		static int InitEnvironment(UInt16 uIOThreadNum = 1, UInt8 uProcThreadNum = 1, bool isSequence = true)
		{
			LOG_Print_SetLog(libFrameWork_Http, MLog_GetAllLevel());

			GetAsynStreamClientsInstance()->Init(uIOThreadNum, uProcThreadNum, isSequence);
			return 1;
		}

		static self_sptr Alloc(void)
		{
			static MemPool_type s_Pool(sizeof(self_type));
			return self_sptr(s_Pool.AllocObj<self_type>(),
				function20_bind_Ex(&MemPool_type::FreeObj<self_type>, &s_Pool));
		}

		static void GetHostInfo(const char* c_szHostUrl, tagHost_INFO* pInfo)
		{
			char* pData = (char*)c_szHostUrl;
			UInt16 uPort = 80;
			_http_string sIP = _foundation_::ScanChar::FindValueAndInc(&pData, "http://", "/").c_str();
			int iPos = sIP.find(":");
			if (iPos != -1)
			{
				uPort = _http_string::StringToInt(_http_string(&sIP[iPos + 1]));
				sIP = _http_string(sIP.c_str(), iPos);
			}

			int iCount = 0;
			pInfo->Host = sIP;
			pInfo->IP = GetIPByDomain(sIP.c_str(), &iCount).c_str();
			pInfo->uPort = uPort;
			pInfo->Url = pData;
		}

	public:
		Http(void)
			: _hNet(-1)
			, _uPort(0)
			, _iStatus(0)
			, _iLen(0)
			, _isOpen(false)
			, _isShort(true)
		{
			
		}

		~Http(void)
		{
			Release();
		}

		int Init(void)
		{
			return 1;
		}

		int OpenLog(void)
		{
			//test
			LOG_Print_SetLog(libFrameWork_Http, MLog_GetAllLevel());
			//test
			return 1;
		}

		void Release(void)
		{
			Close();
		}

		int Open(const _http_string& sIP, UInt16 uPort, const HEvent& hEvent, HdrList_type* pHdrList = NULL)
		{
			_sIP = sIP;
			_uPort = uPort;
			if (pHdrList != NULL)
				_HdrList = *pHdrList;

			_Event = hEvent;
			if (GetAsynStreamClientsInstance()->Connect(sIP.c_str(), uPort,
				function20_bind(&Http::HandleConnect, this->shared_from_this(), sIP, uPort, _function_::_1)) == -1)
			{
				LOG_Print_Error(libFrameWork_Http, "Http::Open(%s:%d)", sIP.c_str(), uPort);
				return -1;
			}

			return 1;
		}

		void Close(void)
		{
			if (_isOpen)
			{
				_isOpen = false;
				GetAsynStreamClientsInstance()->Close(_hNet);
				GetTimerEventKMInstance()->Remove(_sEventID);
			}
		}

		void SetMode(bool isShort)
		{// true 短连接   false:长连接
			_isShort = isShort;
		}

		int PostData(const char* c_szData, UInt32 uSize,
			const _http_string& Url, const _http_string& ContentType = "application/json")
		{
			_http_string sContent = "POST " + Url + " HTTP/1.1\r\n";
			sContent += "Host: " + _sIP + ":" + _http_string::NumberToStr(_uPort) + "\r\n";

			for (typename HdrList_type::iterator iter = _HdrList.begin(); iter != _HdrList.end(); iter ++)
			{
				sContent += (*iter).Name + ": ";
				sContent += (*iter).Value + "\r\n";
			}
			
			sContent += "Content-Type: ";
			sContent += ContentType + "\r\n";
			sContent += "Content-Length: " + _http_string::NumberToStr(uSize) + "\r\n\r\n";
			if (uSize > 0)
			{
				sContent.append(c_szData, uSize);
				sContent += "\r\n\r\n";
			}

			int iRet = Send(sContent.c_str(), sContent.size());
			if (iRet > 0)
			{
				_Time.update();
			}
			else
			{
				LOG_Print_Error(libFrameWork_Http, "Http::PostData(%s:%d/%s) Failed, Data(%d):%s", _sIP.c_str(), _uPort, Url.c_str(), uSize, c_szData)
			}

			LOG_Print_Info(libFrameWork_Http, "Http::PostData(%d,%s) Data():%s", sContent.size(), Url.c_str(), sContent.c_str());

			return iRet;
		}

		int GetData(const char* c_szData, UInt32 uSize,
			const _http_string& Url, const _http_string& ContentType = "application/json")
		{
			_http_string sContent = "GET " + Url + " HTTP/1.1\r\n";
			sContent += "Host: " + _sIP + /*":" + _http_string::NumberToStr(_uPort) +*/ "\r\n";
			sContent += "Content-Type: ";
			sContent += ContentType + "\r\n";
			sContent += "Content-Length: " + _http_string::NumberToStr(uSize) + "\r\n\r\n";
			if (uSize > 0)
			{
				sContent.append(c_szData, uSize);
				sContent += "\r\n\r\n";
			}

			int iRet = Send(sContent.c_str(), sContent.size());
			if (iRet != -1)
			{
				_Url = Url;
				_Time.update();
			}
			else
			{
				LOG_Print_Error(libFrameWork_Http, "Http::GetData(%s:%d/%s) Failed, Data(%d):%s", _sIP.c_str(), _uPort, Url.c_str(), uSize, c_szData)
			}

			LOG_Print_Info(libFrameWork_Http, "Http::GetData(%d,%s) Data:%s", sContent.size(), Url.c_str(), sContent.c_str());
			return iRet;
		}

		int Parse(const char* c_szData, UInt32 uSize, const HEvent& hEvent)
		{
			const char* c_szEnd = "\r\n\r\n";

			//test
			LOG_Print_Info(libFrameWork_Http, "Http::HandleRecv(%d) Data:%s", uSize, c_szData);
			//test

			_sData.append(c_szData, uSize);
			uSize = _sData.size();
gt_start:
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
							hEvent(this->shared_from_this(), EN_CODE_DATA_START, (void*)FileName.c_str());
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
								int iPos = _Url.rfind("/");
								if (iPos != -1)
								{
									FileName = &_Url[iPos + 1];
								}

								_sData = _http_string(&_sData[iEnd + strlen(c_szEnd)], _sData.size() - (iEnd + strlen(c_szEnd)));
								hEvent(this->shared_from_this(), EN_CODE_DATA_START, (void*)FileName.c_str());
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

									hEvent(this->shared_from_this(), EN_CODE_DATA_START, NULL);

									_MEMBUF_PRT Data = { 0 };
									Data.pBuf = (char*)(pEnd + strlen(c_szEnd));
									Data.uLen = uSize - iSize - strlen(c_szEnd);//有效负载数据

									if (uSize - iSize - strlen(c_szEnd) >= iCount)
									{
										hEvent(this->shared_from_this(), EN_CODE_DATA_CHUNK, (void*)&Data);
										hEvent(this->shared_from_this(), EN_CODE_DATA_FINISH, NULL);
										_sData.clear();

										if(_isShort)
											Close();
									}
									else
									{
										if (Data.uLen > 0)
										{
											hEvent(this->shared_from_this(), EN_CODE_DATA_CHUNK, (void*)&Data);
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

										_iStatus = 0;

										if (_isShort)
										{//短连接
											hEvent(this->shared_from_this(), EN_CODE_REPONES_SUCCESS, (void*)(pEnd + strlen(c_szEnd)));
											_sData.clear();
											Close();
										}
										else
										{//长连接
											int iRemain = _sData.size() - (iSize + strlen(c_szEnd) + iCount);
											if (iRemain > 0)
											{
												hEvent(this->shared_from_this(), EN_CODE_REPONES_SUCCESS,
													(void*)_http_string((pEnd + strlen(c_szEnd)), iCount).c_str());

												std::memmove(&_sData[0], &_sData[_sData.size() - iRemain], iRemain);
												_sData.update_size(iRemain);
												goto gt_start;
											}
											else
											{
												hEvent(this->shared_from_this(), EN_CODE_REPONES_SUCCESS, (void*)(pEnd + strlen(c_szEnd)));
												_sData.clear();
											}
										}
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
							hEvent(this->shared_from_this(), EN_CODE_DATA_FINISH, NULL);
							_sData.clear();
							_iStatus = 0;

							if (_isShort)
								Close();
						}
						else
						{
							_sData = _http_string(&_sData[iPos + strlen("\r\n")], _sData.size() - (iPos + strlen("\r\n")));
							if (_sData.size() + 2 >= _iLen)
							{
								_MEMBUF_PRT Data = { 0 };
								Data.pBuf = (char*)_sData.c_str();
								Data.uLen = _iLen - 2;//有效负载数据

								hEvent(this->shared_from_this(), EN_CODE_DATA_CHUNK, (void*)&Data);
								hEvent(this->shared_from_this(), EN_CODE_DATA_FINISH, NULL);

								_sData.clear();
								_iStatus = 0;

								if (_isShort)
									Close();
							}							
						}
					}
				}
				else if (_iLen <= _sData.size())
				{
					_MEMBUF_PRT Data = { 0 };
					Data.pBuf = (char*)_sData.c_str();
					Data.uLen = _iLen - 2;//块与块之间的回车换行不是有效负载数据

					hEvent(this->shared_from_this(), EN_CODE_DATA_CHUNK, (void*)&Data);

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
						hEvent(this->shared_from_this(), EN_CODE_DATA_CHUNK, (void*)&Data);

						_iLen -= _sData.size();
						if (_iLen == 0)
						{
							hEvent(this->shared_from_this(), EN_CODE_DATA_FINISH, NULL);
							_sData.clear();
							_iStatus = 0;

							if (_isShort)
								Close();
						}
						else if (_iLen < 0)
						{
							LOG_Print_Info(libFrameWork_Http, "Http::HandleRecv(%d,%d) Error", uSize, _iLen);
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

		int Send(const char* c_szData, UInt32 uSize)
		{
			if (!_isOpen)
				return -1;

			//test
			LOG_Print_Info(libFrameWork_Http, "Http::Send(%d) Data:%s", uSize, c_szData);
			//test

			return GetAsynStreamClientsInstance()->Send(_hNet, c_szData, uSize);
		}

		void HandleCheck(void)
		{
			if (_Time.isElapsed(HTTP_KEEPALIVED_TIME))
			{//超时
				LOG_Print_Error(libFrameWork_Http, "Http::HandleCheck(%s:%d) TimeOut", _sIP.c_str(), _uPort)
				_Event(this->shared_from_this(), EN_CODE_TIMEOUT, NULL);
				Close();
				return;
			}
			else
			{
				_sEventID = GetTimerEventKMInstance()->push_back(function20_bind(&Http::HandleCheck, this->shared_from_this()), HTTP_KEEPALIVED_TIME);
			}
		}

		void HandleConnect(const _http_string& sIP, UInt16 uPort, NETHANDLE hNet)
		{
			StatusCode Code;

			if (hNet == -1)
			{//连接失败
				LOG_Print_Error(libFrameWork_Http, "Http::HandleConnect(%s:%d) Failed!!", sIP.c_str(), uPort);
				Code = EN_CODE_CONNECT_FAILED;
			}
			else
			{
				GetAsynStreamClientsInstance()->SetRecvHandle(hNet,
					function20_bind(&Http<_http_string>::HandleRecv, this->shared_from_this(),
						_function_::_1, _function_::_2, _function_::_3));

				if (GetAsynStreamClientsInstance()->SetDestroyHandle(hNet,
					function20_bind(&Http<_http_string>::HandleDestroy, this->shared_from_this(), _function_::_1)) == -1)
				{//失败
					_Event(this->shared_from_this(), EN_CODE_CONNECT_FAILED, NULL);
					return ;
				}

				//test
				GetAsynStreamClientsInstance()->SetSendCompleteNotify(hNet,
					function20_bind(&Http<_http_string>::HandleSend, this->shared_from_this(),
						_function_::_1, _function_::_2));
				//test

				_isOpen = true;
				_hNet = hNet;
				Code = EN_CODE_CONNECT_SUCCESS;
				_sEventID = GetTimerEventKMInstance()->push_back(function20_bind(&Http::HandleCheck, this->shared_from_this()), HTTP_KEEPALIVED_TIME);
			}

			if (_Event)
			{
				_Event(this->shared_from_this(), Code, NULL);
			}
		}

		void HandleRecv(NETHANDLE hNet, const StreamBuf_ptr& Buf_ptr, long lRemain)
		{
			Parse(Buf_ptr->data, Buf_ptr->payload, _Event);
		}

		void HandleDestroy(NETHANDLE hNet)
		{
			if (_isOpen)
			{
				Close();
				_Event(this->shared_from_this(), EN_CODE_EXCEPT, NULL);
			}
		}

		//test
		void HandleSend(const char* pHasData, UInt32 HasSize)
		{
			LOG_Print_Info(libFrameWork_Http, "===========Send End===========");
		}
		//test

	private:
		TimerEventKM::_string_	_sEventID;
		HdrList_type			_HdrList;
		HEvent		_Event;
		NETHANDLE	_hNet;
		_http_string	_sIP;
		UInt16		_uPort;
		_http_string	_Url;
		_http_string	_sData;
		Timestamp_type	_Time;
		bool		_isOpen;
		int			_iStatus;
		int			_iLen;
		bool		_isShort;
	};

}


#endif//__ROUTEAPPS_20160510104022_1462848022_H__
