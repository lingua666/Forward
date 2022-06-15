
#include <libFrameWork_Modbus/Tcp_HalfDuplex.h>

namespace _modbus_ {

	namespace _tcp_ {

		Tcp_HalfDuplex::Tcp_HalfDuplex(void)
			: _Handle(NULL)
			, _uTime(0)
			, _isSending(false)
			, _isDestroy(false)
			, _uSeq(0)
			, _isRtu(false)
		{
			memset((void*)&_Recv, 0, sizeof(_Recv));
			_uReSendTime = TCP_DATA_RESEND_DEF;
			_uReSendInterval = TCP_DATA_OVERTIME_DEF;
			_uOccupyTimer = TCP_RECV_TURATION_DEF;

			LOG_Print_SetLog(libFrameWork_Modbus, MLog_GetAllLevel());
		}

		Tcp_HalfDuplex::~Tcp_HalfDuplex(void)
		{
			Release();
		}

		void Tcp_HalfDuplex::Init(const HFNFullPacket& hFull)
		{
			_hFull = hFull;
		}

		void Tcp_HalfDuplex::Release(void)
		{
			Close();
		}

		THandle Tcp_HalfDuplex::Open(const _mod_string& sIP, UInt16 uPort,
			const HFNDestroy& hDestroy, bool isRtu)
		{
			_Handle = NULL;
			GetAsynStreamClientsInstance()->Connect(sIP.c_str(), uPort,
				function20_bind(&Tcp_HalfDuplex::HandleConnect, shared_from_this(),
					hDestroy, sIP, uPort, _function_::_1));

			while (_Handle == NULL)
			{//阻塞等待
				Sleep(10);
			}

			if (_Handle == -1)
			{
				LOG_Print_Error(libFrameWork_Modbus, "Tcp_HalfDuplex::Open(%s,%d) Failed!!",
					sIP.c_str(), uPort);
				return -1;
			}

			_isRtu = isRtu;
			return _Handle;
		}

		void Tcp_HalfDuplex::HandleConnect(const HFNDestroy& hDestroy, const _mod_string& sIP, int u16Port, NETHANDLE Handle)
		{
			_Handle = Handle;
			if (Handle != -1)
			{
				GetAsynStreamClientsInstance()->SetRecvHandle(Handle,
					function20_bind_Ex(&Tcp_HalfDuplex::HandleRecv, shared_from_this()));

				if (GetAsynStreamClientsInstance()->SetDestroyHandle(Handle,
					function20_bind(&Tcp_HalfDuplex::HandleDestroy, shared_from_this(), hDestroy, Handle)) == -1)
				{//设置成功
					_Handle = -1;
					_isDestroy = true;
				}
			}
		}

		int Tcp_HalfDuplex::Close(void)
		{
			THandle h;
			if (is_open())
			{
				h = _Handle;
				_Handle = -1;
				GetAsynStreamClientsInstance()->Close(h);
				_List.clear();
				while (!_isDestroy)
				{
					Sleep(10);
				}
			}

			_HRecvEvents.disconnect_all();
			return 1;
		}

		void Tcp_HalfDuplex::HandleRecv(THandle Handle, const StreamBuf_ptr& Buf_ptr, long lRemain)
		{
			//test
			LOG_Print_Info(libFrameWork_Modbus, "Tcp_HalfDuplex::HandleRecv(%d):%s",
				Buf_ptr->payload, _mod_string::HexToStr(Buf_ptr->data, Buf_ptr->payload).c_str());
			//test

			if (_uTime == 0)
				_uTuration = _uTime = Timestamp_type::getcurrent_microseconds();

			int Interval = Timestamp_type::getcurrent_microseconds() - _uTime;

			if (_Recv.uSize + Buf_ptr->payload >= sizeof(_Recv.Buf))
				_Recv.uSize = 0;

			memcpy(&_Recv.Buf[_Recv.uSize], Buf_ptr->data, Buf_ptr->payload);
			_Recv.uSize += Buf_ptr->payload;

			char* pData = _Recv.Buf;
			int iRet = 0, iBytesRead = 0;
gt_start:
			if (!_hFull || (iRet = _hFull(pData, _Recv.uSize, &iBytesRead)) != -1)
			{//完整一个包

			 /*LOG_Print_Info(libFrameWork_Modbus, "Tcp_HalfDuplex::HandleRecv(%d):%s",
			 _Recv.uSize, _mod_string_mod_string::HexToStr(_Recv.Buf, _Recv.uSize).c_str());*/

				if (iRet == -2)
				{
					_Recv.uSize -= iBytesRead;
				}
				else
				{
					HFNTcpFinish hFinish;
					if (_Data)
					{
						tagTcpData_INFO* pData = (tagTcpData_INFO*)(_Data->Data);
						hFinish = pData->hFinish;
						pData->isVaild = false;
					}

					_Lock.Lock();
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
					_Lock.UnLock();

					DBuffer_sptr sptr = _Data;

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

			if (Interval > TCP_RECV_INTERVAL_DEF)
			{
				_uTuration = _uTime = Timestamp_type::getcurrent_microseconds();
			}
			else
			{
				Timestamp_type::TimeVal Cur = Timestamp_type::getcurrent_microseconds();
				if (Cur - _uTuration >= (_uOccupyTimer * 1000))
				{//占用总线5秒  报错
					LOG_Print_Error(libFrameWork_Modbus, "Tcp_HalfDuplex::HandleRecv Bus Work Abnormal!!");
				}
				_uTime = Timestamp_type::getcurrent_microseconds();
			}

			if (_HRecvEvents)
				_HRecvEvents(Handle, Buf_ptr->data, Buf_ptr->payload);
		}

		int Tcp_HalfDuplex::Send(const char* c_szData, UInt32 uSize,
			const _mod_string& sIdentity, const HFNTcpFinish& hFinish, UInt8 u8Priority,
			Int16 ReSendTimer, Int16 ReSendInterval)
		{
			if (!is_open())
				return -1;

			DBuffer_sptr sptr = GetDataBufferInstance()->Alloc(function20_bind_Ex(&Tcp_HalfDuplex::Free));
			sptr->uSize = uSize;
			tagTcpData_INFO* pData = new((void*)sptr->Data) tagTcpData_INFO();
			pData->isVaild = true;
			pData->sIdentity = sIdentity;
			pData->hFinish = hFinish;
			pData->ReSendTimer = ReSendTimer == -1 ? _uReSendTime : ReSendTimer;
			pData->ReSendInterval = ReSendInterval == -1 ? _uReSendInterval : ReSendInterval;

			if (!_isRtu)
			{
				_uSeq++;
				char *Ptr = pData->Data;

				//增加MBAP头
				memcpy(Ptr, &_uSeq, sizeof(_uSeq));	//2个字节事务处理标识(序列号)
				Ptr += sizeof(_uSeq);
				memset(Ptr, 0, 2);	//2个字节协议标识
				Ptr += 2;
				UInt16 uCount = Conversion16ToByteOrder(uSize, EN_BOT_BIG);
				memcpy(Ptr, &uCount, sizeof(uCount));	//负载长度
				Ptr += 2;
				memcpy(Ptr, c_szData, uSize);
			}
			else
			{
				memcpy(pData->Data, c_szData, uSize);
			}

			_Lock.Lock();
			if (!_isSending)
			{
				_isSending = true;
				_Lock.UnLock();
				_Data = sptr;
				Send(sptr);
			}
			else
			{
				_List.push_back(sptr, u8Priority);
				_Lock.UnLock();
			}
			return 1;
		}

		int Tcp_HalfDuplex::Send(const DBuffer_sptr& sptr, bool isAdd)
		{
			if (!is_open())
				return -1;

			//发送前, 清除接收缓冲区
			//_Recv.uSize = 0;

			tagTcpData_INFO* pData = (tagTcpData_INFO*)sptr->Data;

			//test
			LOG_Print_Info(libFrameWork_Modbus, "Tcp_HalfDuplex::Send(%d):%s",
				sptr->uSize, _mod_string::HexToStr(pData->Data, sptr->uSize).c_str());
			//test

			AddEvent(sptr, isAdd);
			return GetAsynStreamClientsInstance()->Send(_Handle, pData->Data, sptr->uSize);
		}

		void Tcp_HalfDuplex::HandleDestroy(const HFNDestroy& hDestroy, THandle Handle)
		{
			_isDestroy = true;
			Close();
			if (hDestroy)
				hDestroy(Handle);
		}

		bool Tcp_HalfDuplex::is_open(void)
		{
			return _Handle == -1 ? false : true;
		}

		int Tcp_HalfDuplex::RegisterRecvEvent(const HFNRecvEvent& hEvent)
		{
			_HRecvEvents.connect(hEvent);
			return 1;
		}

		int Tcp_HalfDuplex::AddEvent(const DBuffer_sptr& sptr, bool isInit)
		{
			if (!is_open())
				return -1;

			tagTcpData_INFO* pData = (tagTcpData_INFO*)sptr->Data;
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

			GetTimerEventInstance()->push_back(function20_bind(&Tcp_HalfDuplex::EventProcess, shared_from_this(), sptr),
				pData->ReSendInterval);
			return 1;
		}

		void Tcp_HalfDuplex::EventProcess(const DBuffer_sptr& sptr)
		{
			if (!is_open())
				return;

			tagTcpData_INFO* pData = (tagTcpData_INFO*)sptr->Data;
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
					_List.pop();
					_Data = GetPacket();
					if (!_Data)
					{
						_isSending = false;
					}
					_Lock.UnLock();
					if (_Data)
					{
						Send(_Data);
					}

					if (pData->hFinish)
					{
						LOG_Print_Error(libFrameWork_Modbus, "Tcp_HalfDuplex::EventProcess Timeout Data(%d):%s",
							sptr->uSize, _mod_string::HexToStr(pData->Data, sptr->uSize).c_str());

						pData->hFinish(-1, pData->Data, sptr->uSize);
					}
				}
			}
		}

		void Tcp_HalfDuplex::FilterPacket(const _mod_string& sIdentity)
		{
			DBuffer_sptr sptr;
			LList::iterator iter;
			_Lock.Lock();
			for (iter = _List.begin(); iter; iter++)
			{
				sptr = *iter;
				tagTcpData_INFO* pData = (tagTcpData_INFO*)sptr->Data;
				if (pData->sIdentity == sIdentity)
					pData->isVaild = false;
			}
			_Lock.UnLock();
		}


		DBuffer_sptr Tcp_HalfDuplex::GetPacket(void)
		{
			DBuffer_sptr sptr;
			while (_List.size() > 0)
			{
				sptr = _List.front();
				if (sptr && ((tagTcpData_INFO*)sptr->Data)->isVaild)
				{
					break;
				}
				else
				{
					_List.pop();
				}
			}

			if (sptr && !((tagTcpData_INFO*)sptr->Data)->isVaild)
				return DBuffer_sptr();

			return sptr;
		}

	}

}
