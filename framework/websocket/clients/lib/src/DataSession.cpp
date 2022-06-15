
#include <libFrameWork_WebsocketClients/DataSession.h>

#define WEBSOCKET_CHECK_HEART_TIME_DEF	20000

namespace _websocket_ {

	namespace _clients_ {

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

		int DataSession::Init(void)
		{
			return -1;
		}

		void DataSession::Release(void)
		{
			Close();
		}

		int DataSession::Open(const _string_t& sSvrIP, const UInt16 uSvrPort)
		{
			if (GetAsynStreamClientsInstance()->Connect(sSvrIP.c_str(), uSvrPort,
				function20_bind(&DataSession::HandleConnect, shared_from_this(), sSvrIP, uSvrPort, _function_::_1)) == -1)
			{//失败
				LOG_Print_Error(libFrameWork_WebsocketClients, "DataSession::Connect(%s:%d): Failed",
					sSvrIP.c_str(), uSvrPort);

				return -1;
			}

			return 1;
		}

		void DataSession::HandleConnect(const _string_t& sSvrIP, const UInt16 uSvrPort, NETHANDLE hNet)
		{
			if (hNet == -1)
			{//连接失败
				LOG_Print_Error(libFrameWork_WebsocketClients, "DataSession::HandleConnect() Failed!!");

			}
			else
			{//连接成功
				_hNet = hNet;
				GetAsynStreamClientsInstance()->SetRecvHandle(_hNet,
					function20_bind(&DataSession::HandleToken, shared_from_this(),
						_function_::_1, _function_::_2, _function_::_3));

				if (GetAsynStreamClientsInstance()->SetDestroyHandle(_hNet,
					function20_bind(&DataSession::HandleDestroy, shared_from_this(),
						_function_::_1)) == -1)
				{//失败
					LOG_Print_Error(libFrameWork_WebsocketClients, "DataSession::HandleConnect() Set Destroy Failed!!");
					Close();
					return;
				}

				_isOpen = true;

				//握手
				CHandSharke Sharke;
				Sharke.Init();
				_sMaskKey = get_mask_key().c_str();
				_string_t s = Sharke.Request("/", (sSvrIP + ":" + _string_t::NumberToStr(uSvrPort)).c_str(), _sMaskKey.c_str());
				Send(s.c_str(), s.size());
			}
		}

		void DataSession::HandleToken(NETHANDLE hNet, const StreamBuf_ptr& Stream_sptr, long lRemain)
		{
			//test
			LOG_Print_Info(libFrameWork_WebsocketClients, "DataSession::HandleToken(%d) Data:%s",
				Stream_sptr->payload, Stream_sptr->data);
			//test

			if (Stream_sptr->payload > 3)
			{
				_websocket_::CHandSharke Sharke;
				Sharke.Init();

				int  iRead = 0;
				_websocket_::_string_t sRet = Sharke.Parse(Stream_sptr->data, Stream_sptr->payload, &iRead);
				if (iRead == -1)
				{
					LOG_Print_Error(libFrameWork_WebsocketClients,
						"WebsocketServer::HandleToken(%s) Parse Failed :%s !!", Stream_sptr->data);
					return;
				}

				if (Sharke.validate_handshake_response(Stream_sptr->data, Stream_sptr->payload,
					_sMaskKey.c_str()) != websocket_ok)
				{//握手失败

					LOG_Print_Error(libFrameWork_WebsocketClients, "WebsocketServer::HandleToken() Hand Sharke Failed!!");
					Close();
					return;
				}
				
				GetAsynStreamClientsInstance()->SetRecvHandle(_hNet,
					function20_bind(&DataSession::HandleRecv, shared_from_this(),
						_function_::_1, _function_::_2, _function_::_3));

				_Event = GetTimerEventKMInstance()->push_back(function20_bind(&DataSession::HandleHeart, shared_from_this()),
					WEBSOCKET_CHECK_HEART_TIME_DEF).c_str();

				_Time.update();
			}
		}

		void DataSession::Close(void)
		{
			if (IsOpen())
			{
				_isOpen = false;
				GetStreamSvrInstance()->Close(_hNet);
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
			Head.opcode = 0x01;

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
			Length.MASK = 1;
			if (uSize < 126)
			{
				Length.Payload = uSize;
				Data.append((char*)&Length, sizeof(Frame_Length));
			}
			else if (uSize < 0xFFFF)
			{
				Length.Payload = 126;
				Data.append((char*)&Length, sizeof(Frame_Length));
				uSize = LittleToBigEndian16(uSize);
				Data.append((char*)&uSize, sizeof(UInt16));
			}
			else
			{
				Length.Payload = 127;
				Data.append((char*)&Length, sizeof(Frame_Length));
				uSize = LittleToBigEndian16(uSize);
				Data.append((char*)&uSize, sizeof(UInt64));
			}

			UInt32 uMask = _hNet + rand();
			Data.append((char*)&uMask, sizeof(UInt32));
			UInt16 uOffset = Data.size();
			Data.append(c_szData, uSize);

			byte_mask((char*)&Data[uOffset], uSize, (char*)&uMask);

			return Send(Data.c_str(), Data.size());
		}

		int DataSession::Send(const char* c_szData, UInt32 uSize)
		{
			if (IsOpen())
				return GetAsynStreamClientsInstance()->Send(_hNet, c_szData, uSize);

			return -1;
		}

		void DataSession::HandleHeart(void)
		{
			Frame_Head	Head = { 0 };
			Head.FIN = 1;
			Head.opcode = 0x9;

			Timestamp_type::TimeVal Vaule = Timestamp_type().epochMicroseconds();
			SendFrame(Head, (char*)&Vaule, sizeof(Timestamp_type::TimeVal));

			_Event = GetTimerEventKMInstance()->push_back(function20_bind(&DataSession::HandleHeart, shared_from_this()),
				WEBSOCKET_CHECK_HEART_TIME_DEF).c_str();
		}

		void DataSession::HandleRecv(NETHANDLE hNet, const StreamBuf_ptr& Stream_sptr, long lRemain)
		{
			if (_Data.size() + Stream_sptr->payload >= 1024 * 1024)
			{//超过最大缓存
				_Data.update_size(0);
				LOG_Print_Error(libFrameWork_WebsocketClients, "DataSession::HandleRecv() Over Maximum Cache Memory(%d)", lRemain);
			}

			//test
			LOG_Print_Info(libFrameWork_WebsocketClients, "DataSession::HandleRecv(%d) Data:%s",
				Stream_sptr->payload, Stream_sptr->data);
			//test

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
							_Subscribe.Publish(EN_EVENT_RECV, shared_from_this(), (void*)&PayLoad);
							_RecvData.update_size(0);
						}
						else
						{
							_Subscribe.Publish(EN_EVENT_RECV, shared_from_this(), (void*)&PayLoad);
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
				}
				break;
				case 0xA:	//Pong Frame
				{
					
				}
				break;
				}
			}
		}

		void DataSession::HandleDestroy(NETHANDLE hNet)
		{
			Close();
			_Subscribe.Publish(EN_EVENT_CLOSE, shared_from_this(), NULL);
			_Subscribe.Release();
		}

		bool DataSession::IsOpen(void)
		{
			return _isOpen;
		}

	}

}
