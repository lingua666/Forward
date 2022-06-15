
#include <libFrameWork_Modbus/Uarts_HalfDuplex.h>

namespace _modbus_ {

	namespace _rtu_ {

		Uarts_HalfDuplex::Uarts_HalfDuplex(void)
			: _Handle(NULL)
			, _uTime(0)
			, _isSending(false)
			, _isDestroy(false)
		{
			memset((void*)&_Recv, 0, sizeof(_Recv));
			_uReSendTime = UART_DATA_RESEND_DEF;
			_uReSendInterval = UART_DATA_OVERTIME_DEF;
			_uOccupyTimer = UART_RECV_TURATION_DEF;

			LOG_Print_SetLog(libFrameWork_Modbus, MLog_GetAllLevel());
		}

		Uarts_HalfDuplex::~Uarts_HalfDuplex(void)
		{
			Release();
		}

		void Uarts_HalfDuplex::Init(const HFNFullPacket& hFull)
		{
			_hFull = hFull;
		}

		void Uarts_HalfDuplex::Release(void)
		{
			Close();
		}

		THandle Uarts_HalfDuplex::Open(const _mod_string& ComName, const _mod_string& BaudRate,
			const _mod_string& DataBit, const _mod_string& StopBit, const _mod_string& Parity,
			const HFNDestroy& hDestroy)
		{
			int  iParity = 0;
#if defined( PLATFORM_OS_FAMILY_WINDOWS )
			if (Parity == "None" || Parity == "0")//无校验
			{
				iParity = 0;
			}
			else if (Parity == "Odd")//奇校验
			{
				iParity = 1;
			}
			else if (Parity == "Even")//偶校验
			{
				iParity = 2;
			}

			int iStopBit = _mod_string::StringToInt(StopBit);
			if (iStopBit == 1)
				iStopBit = 0;

#elif defined( PLATFORM_OS_FAMILY_UNIX )
			if (Parity == "None" || Parity == "0")//无校验
			{
				iParity = 'S';
			}
			else if (Parity == "Odd")//奇校验
			{
				iParity = 'O';
			}
			else if (Parity == "Even")//偶校验
			{
				iParity = 'E';
			}

			int iStopBit = _mod_string::StringToInt(StopBit);
#endif

			_Handle = _async_::_uarts_::GetAsyncUartsInstance()->Open(ComName.c_str(),
				_mod_string::StringToLong(BaudRate),
				_mod_string::StringToLong(DataBit),
				(UInt8)iStopBit,
				iParity);

			if (_Handle == -1)
			{
				LOG_Print_Error(libFrameWork_Modbus, "Uarts_HalfDuplex::Open Failed!! ComName:%s,"
					"BaudRate:%s, DataBit:%s, StopBit:%s, Parity:%s",
					ComName.c_str(), BaudRate.c_str(), DataBit.c_str(), StopBit.c_str(), Parity.c_str());
				return -1;
			}

			_async_::_uarts_::GetAsyncUartsInstance()->SetCallBack(_Handle,
				function20_bind_Ex(&Uarts_HalfDuplex::HandleRecv, shared_from_this()),
				function20_bind_Ex(&Uarts_HalfDuplex::HandleDestroy, shared_from_this()));

			_isSending = false;
			_hDestroy = hDestroy;
			return _Handle;
		}

		int Uarts_HalfDuplex::Close(void)
		{
			THandle h;
			if (is_open())
			{
				h = _Handle;
				_Handle = -1;
				_async_::_uarts_::GetAsyncUartsInstance()->Close(h);
				_List.clear();
				_isSending = false;
				while (!_isDestroy)
				{
					Sleep(10);
				}
			}

			_HRecvEvents.disconnect_all();
			return 1;
		}

		void Uarts_HalfDuplex::HandleRecv(THandle Handle, const char* szData, UInt32 uSize)
		{
			//test
			LOG_Print_Info(libFrameWork_Modbus, "Uarts_HalfDuplex::HandleRecv(%d):%s",
				uSize, _mod_string::HexToStr(szData, uSize).c_str());
			//test

			if (_uTime == 0)
				_uTuration = _uTime = Timestamp_type::getcurrent_microseconds();

			int Interval = Timestamp_type::getcurrent_microseconds() - _uTime;

			if (_Recv.uSize + uSize >= sizeof(_Recv.Buf))
				_Recv.uSize = 0;

			memcpy(&_Recv.Buf[_Recv.uSize], szData, uSize);
			_Recv.uSize += uSize;

			char* pData = _Recv.Buf;
			int iRet = 0, iBytesRead = 0;
gt_start:
			if (!_hFull || (iRet = _hFull(pData, _Recv.uSize, &iBytesRead)) != -1)
			{//完整一个包

			 /*LOG_Print_Info(libFrameWork_Modbus, "Uarts_HalfDuplex::HandleRecv(%d):%s",
			 _Recv.uSize, _mod_string::HexToStr(_Recv.Buf, _Recv.uSize).c_str());*/

				if (iRet == -2)
				{
					_Recv.uSize -= iBytesRead;
				}
				else
				{
					HFNUartFinish hFinish;

					_Lock.Lock();
					if (_Data)
					{
						tagUartData_INFO* pData = (tagUartData_INFO*)(_Data->Data);
						hFinish = pData->hFinish;
						pData->isVaild = false;
					}
					if (_List.size() > 0)
					{
						_Data = GetPacket();
						if (!_Data)
							_isSending = false;
					}
					else
					{
						_isSending = false;
						_Data.reset();
					}
					DBuffer_sptr sptr = _Data;
					_Lock.UnLock();

					if (hFinish)
					{
						hFinish(1, &pData[iRet], iBytesRead - iRet - 2);
					}
					else if (_hFullData)
					{
						_hFullData(1, &pData[iRet], iBytesRead - iRet - 2);
					}

					_Recv.uSize -= iBytesRead;
					pData += iBytesRead;

					if (_Recv.uSize > 0)
						goto gt_start;

					if (sptr)
					{
						Send(sptr);
					}
				}
			}

			if (Interval > UART_RECV_INTERVAL_DEF)
			{
				_uTuration = _uTime = Timestamp_type::getcurrent_microseconds();
			}
			else
			{
				Timestamp_type::TimeVal Cur = Timestamp_type::getcurrent_microseconds();
				if (Cur - _uTuration >= (_uOccupyTimer * 1000))
				{//占用总线5秒  报错
					LOG_Print_Error(libFrameWork_Modbus, "Uarts_HalfDuplex::HandleRecv Bus Work Abnormal(%lld,%lld)!!", Cur, _uTuration);
				}
				_uTime = Timestamp_type::getcurrent_microseconds();
			}

			if (_HRecvEvents)
				_HRecvEvents(Handle, szData, uSize);
		}

		int Uarts_HalfDuplex::Send(const char* c_szData, UInt32 uSize,
			const _mod_string& sIdentity, const HFNUartFinish& hFinish,
			UInt8 u8Priority, Int16 ReSendTimer, Int16 ReSendInterval)
		{
			if (!is_open())
				return -1;

			DBuffer_sptr sptr = GetDataBufferInstance()->Alloc(function20_bind_Ex(&Uarts_HalfDuplex::Free));
			sptr->uSize = uSize;
			tagUartData_INFO* pData = new((void*)sptr->Data) tagUartData_INFO();
			pData->isVaild = true;
			pData->sIdentity = sIdentity;
			pData->hFinish = hFinish;
			pData->ReSendTimer = ReSendTimer == -1 ? _uReSendTime : ReSendTimer;
			pData->ReSendInterval = ReSendInterval == -1 ? _uReSendInterval : ReSendInterval;
			pData->isPush = false;
			memcpy(pData->Data, c_szData, uSize);

			_Lock.Lock();
			if (!_isSending)
			{
				_isSending = true;
				_Data = sptr;
				_Lock.UnLock();
				Send(sptr);
			}
			else
			{
				pData->isPush = true;
				_List.push_back(sptr, u8Priority);
				_Lock.UnLock();
			}
			return 1;
		}

		int Uarts_HalfDuplex::Send(const DBuffer_sptr& sptr, bool isAdd)
		{
			if (!is_open())
				return -1;

			//发送前, 清除接收缓冲区
			//_Recv.uSize = 0;

			tagUartData_INFO* pData = (tagUartData_INFO*)sptr->Data;

			//test
			LOG_Print_Info(libFrameWork_Modbus, "Uarts_HalfDuplex::Send(%d):%s",
				sptr->uSize, _mod_string::HexToStr(pData->Data, sptr->uSize).c_str());
			//test

			if(pData->hFinish)
				AddEvent(sptr, isAdd);

			return _async_::_uarts_::GetAsyncUartsInstance()->Send(_Handle, pData->Data, sptr->uSize);
		}

		void Uarts_HalfDuplex::HandleDestroy(THandle Handle)
		{
			_isDestroy = true;
			if (_hDestroy)
				_hDestroy(Handle);
		}

		bool Uarts_HalfDuplex::is_open(void)
		{
			return _Handle == -1 ? false : true;
		}

		int Uarts_HalfDuplex::RegisterRecvEvent(const HFNRecvEvent& hEvent)
		{
			_HRecvEvents.connect(hEvent);
			return 1;
		}

		int Uarts_HalfDuplex::AddEvent(const DBuffer_sptr& sptr, bool isInit)
		{
			if (!is_open())
				return -1;

			tagUartData_INFO* pData = (tagUartData_INFO*)sptr->Data;
			if (isInit)
			{
				pData->isVaild = true;
				pData->ReSendTimer = pData->ReSendTimer + 1;
				pData->EndTime = Timestamp_type::getcurrent_microseconds() + pData->ReSendInterval * 1000;
			}
			else
			{
				pData->EndTime = Timestamp_type::getcurrent_microseconds() + pData->ReSendInterval * 1000;
			}

			GetTimerEventInstance()->push_back(function20_bind(&Uarts_HalfDuplex::EventProcess, shared_from_this(), sptr),
				pData->ReSendInterval);
			return 1;
		}

		void Uarts_HalfDuplex::EventProcess(const DBuffer_sptr& sptr)
		{
			if (!is_open())
				return;

			tagUartData_INFO* pData = (tagUartData_INFO*)sptr->Data;
			if (pData->isVaild)
			{
				pData->ReSendTimer--;
				if (pData->ReSendTimer > 0)
				{
					Send(sptr, false);
				}
				else
				{//超时
					_Recv.uSize = 0;
					pData->isVaild = false;
					//过滤失败设备的所有包
					FilterPacket(pData->sIdentity);
					//发送下个包
					_Lock.Lock();
					_Data = GetPacket();
					if (!_Data)
					{
						_isSending = false;
					}
					DBuffer_sptr Buf = _Data;
					_Lock.UnLock();
					if (Buf)
					{
						Send(Buf);
					}

					if (pData->hFinish)
					{
						LOG_Print_Error(libFrameWork_Modbus, "Uarts_HalfDuplex::EventProcess Timeout Data(%d):%s",
							sptr->uSize, _mod_string::HexToStr(pData->Data, sptr->uSize).c_str());

						pData->hFinish(-1, pData->Data, sptr->uSize);
					}
				}
			}
		}

		void Uarts_HalfDuplex::FilterPacket(const _mod_string& sIdentity)
		{
			DBuffer_sptr sptr;
			LList::iterator iter;
			_Lock.Lock();
			for (iter = _List.begin(); iter; iter++)
			{
				sptr = *iter;
				tagUartData_INFO* pData = (tagUartData_INFO*)sptr->Data;
				if (pData->sIdentity == sIdentity)
					pData->isVaild = false;
			}
			_Lock.UnLock();
		}


		DBuffer_sptr Uarts_HalfDuplex::GetPacket(void)
		{
			DBuffer_sptr sptr;
			while (_List.size() > 0)
			{
				sptr = _List.front();
				if (sptr && ((tagUartData_INFO*)sptr->Data)->isVaild)
				{
					break;
				}
				else
				{
					_List.pop();
				}
			}

			if (sptr && !((tagUartData_INFO*)sptr->Data)->isVaild)
				return DBuffer_sptr();

			return sptr;
		}

	} //_ic2000_

}
