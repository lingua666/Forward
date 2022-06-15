
#include <libFrameWork_WebsocketSvr/DataSession.h>

#define WEBSOCKET_CHECK_HEART_TIME_DEF	60000

namespace _websocket_ {

	namespace _server_ {

		DataSession::self_sptr DataSession::Alloc(void)
		{
			static MemPool_type s_Pool(sizeof(DataSession));
			return self_sptr(s_Pool.AllocObj<self_type>(),
				function20_bind(&MemPool_type::FreeMemObj<self_type>, &s_Pool, _function_::_1));
		}

		DataSession::DataSession(void)
			: _isOpen(false)
		{

		}

		DataSession::~DataSession(void)
		{
			Release();
		}

		int DataSession::Init(NETHANDLE hNet, const HFNSend& hSend)
		{
			if (hNet != -1)
			{
				GetStreamSvrInstance()->SetRecvHandle(hNet,
					function20_bind(&DataSession::HandleRecv, shared_from_this(),
						_function_::_1, _function_::_2, _function_::_3));

				if (GetStreamSvrInstance()->SetDestroyHandle(hNet,
					function20_bind(&DataSession::HandleDestroy, shared_from_this(), _function_::_1)) == 1)
				{
					_hNet = hNet;
					_hSend = hSend;
					_isOpen = true;
					_Subscribe.Init(EN_EVENT_RECV, EN_EVENT_COUNT);

					_Time.update();

					_Event = GetTimerEventKMInstance()->push_back(function20_bind(&DataSession::HandleCheckHeart, shared_from_this()),
						WEBSOCKET_CHECK_HEART_TIME_DEF).c_str();

					return 1;
				}
			}

			return -1;
		}

		void DataSession::HandleCheckHeart(void)
		{
			if (_Time.isElapsed(WEBSOCKET_CHECK_HEART_TIME_DEF * 1000))
			{//超时
				Close();
				LOG_Print_Error(libFrameWork_WebsocketSvr, "DataSession::HandleCheckHeart() Timeout!!");
			}
			else
			{
				if (IsOpen())
				{
					_Event = GetTimerEventKMInstance()->push_back(function20_bind(&DataSession::HandleCheckHeart, shared_from_this()),
						WEBSOCKET_CHECK_HEART_TIME_DEF).c_str();
				}
			}
		}

		void DataSession::Release(void)
		{
			Close();
		}

		void DataSession::Close(void)
		{
			if (IsOpen())
			{
				_isOpen = false;
				GetStreamSvrInstance()->Close(_hNet);
				GetTimerEventKMInstance()->Remove(_Event.c_str());
			}
		}

		int DataSession::SendText(const char* c_szData, UInt64 uSize)
		{
			Frame_Head	Head = { 0 };
			Head.FIN = 1;
			Head.opcode = 0x01;

			return SendFrame(Head, c_szData, uSize);
		}

		int DataSession::SendBinary(const char* c_szData, UInt64 uSize)
		{
			Frame_Head	Head = { 0 };
			Head.FIN = 1;
			Head.opcode = 0x02;

			return SendFrame(Head, c_szData, uSize);
		}

		int DataSession::AddConsumer(enSubscribe_Event Type, const HFNSubsEvent& hEvent)
		{
			return _Subscribe.AddConsumer(Type, hEvent);
		}

		NETHANDLE DataSession::Get(void)
		{
			return _hNet;
		}

		int DataSession::SendFrame(const Frame_Head& Head, const char* c_szData, UInt64 uSize)
		{
			_string_big_t Data;
			Data.append((char*)&Head, sizeof(Frame_Head));

			Frame_Length Length = { 0 };
			Length.MASK = 0;
			if (uSize < 126)
			{
				Length.Payload = uSize;
				Data.append((char*)&Length, sizeof(Frame_Length));
			}
			else if (uSize < 0xFFFF)
			{
				Length.Payload = 126;
				Data.append((char*)&Length, sizeof(Frame_Length));
				UInt16 u16 = Conversion16ToByteOrder(uSize, enByteOrderType::EN_BOT_BIG);
				Data.append((char*)&u16, sizeof(UInt16));
			}
			else
			{
				Length.Payload = 127;
				Data.append((char*)&Length, sizeof(Frame_Length));
				UInt64 u64 = Conversion64ToByteOrder(uSize, enByteOrderType::EN_BOT_BIG);
				Data.append((char*)&u64, sizeof(UInt64));
			}

			Data.append(c_szData, uSize);

			return Send(Data.c_str(), Data.size());
		}

		int DataSession::Send(const char* c_szData, UInt32 uSize)
		{
			if (IsOpen() && _hSend)
				return _hSend(_hNet, c_szData, uSize);

			return -1;
		}

		void DataSession::HandleRecv(NETHANDLE hNet, const StreamBuf_ptr& Stream_sptr, long lRemain)
		{
			if (_Data.size() + Stream_sptr->payload >= 1024 * 1024)
			{//超过最大缓存
				_Data.update_size(0);
				LOG_Print_Error(libFrameWork_WebsocketSvr, "DataSession::HandleRecv() Over Maximum Cache Memory(%d)", lRemain);
			}

			_Data.append(Stream_sptr->data, Stream_sptr->payload);

		gt_begin:
			Frame_PayLoad	PayLoad;
			Int64 iPostion = Websocket_Frame_Parse(_Data.c_str(), _Data.size(), &PayLoad);
			if (iPostion != -1)
			{
				switch (PayLoad.Code)
				{
				case 0x08:	//连接断开
				{
					Close();
					return;
				}
				break;
				case 0x01:	//Text Frame
				case 0x02:	//Binary Frame
				{
					if (PayLoad.isFin)
					{//结束
						if (_RecvData.size() > 0)
						{
							_RecvData.append(PayLoad.pData, PayLoad.uLen);
							PayLoad.uLen = _RecvData.size();
							PayLoad.pData = const_cast<char*>(_RecvData.c_str());
							_Subscribe.Publish(EN_EVENT_RECV, shared_from_this(), PayLoad.pData, PayLoad.uLen);
							_RecvData.update_size(0);
						}
						else
						{
							_Subscribe.Publish(EN_EVENT_RECV, shared_from_this(), PayLoad.pData, PayLoad.uLen);
						}
					}
					else
					{
						_RecvData.append(PayLoad.pData, PayLoad.uLen);
					}

					int iRemain = _Data.size() - iPostion;
					if (iRemain > 0)
					{
						_Data = Buffer_type(&_Data[iPostion], iRemain);
						goto gt_begin;
					}
					else
						_Data.update_size(0);
				}
				break;
				case 0x9:	//Ping Frame
				{
					Frame_Head	Head = { 0 };
					Head.FIN = 1;
					Head.opcode = 0xA;

					SendFrame(Head, PayLoad.pData, PayLoad.uLen);
					_Time.update();
				}
				break;
				case 0xA:	//Pong Frame
				{
					_Time.update();
				}
				break;
				}
			}
		}

		void DataSession::HandleDestroy(NETHANDLE hNet)
		{
			Close();
			_Subscribe.Publish(EN_EVENT_CLOSE, shared_from_this(), NULL, NULL);
			_Subscribe.Release();
			if (_hDestroy)
				_hDestroy(shared_from_this());
		}

		bool DataSession::IsOpen(void)
		{
			return _isOpen;
		}

		void DataSession::SetDestroyHandle(const HFNDestroy& hDestroy)
		{
			_hDestroy = hDestroy;
		}

		void DataSession::Update(void)
		{
			_Time.update();
		}
	}

}


