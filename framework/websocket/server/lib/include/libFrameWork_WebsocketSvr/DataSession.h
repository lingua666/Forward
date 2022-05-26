
#ifndef __WEBSOCKET_SERVER_DATASESSION_20171226154204_3344199361_H__
#define __WEBSOCKET_SERVER_DATASESSION_20171226154204_3344199361_H__

#include "External.h"

namespace _websocket_ {

	namespace _server_ {

		class WebsocketServer;

		class DataSession : public _enable_shared_from_this<DataSession>
		{
		public:
			friend	class WebsocketServer;

			typedef DataSession		self_type;
			typedef SmartPTR<self_type>	self_sptr;
			typedef function20_handle<int(NETHANDLE, const char*, UInt32)>	HFNSend;
			typedef _string_v30_::string< MemPool_macro<5120> >	Buffer_type;
			typedef SubscribeEvents<void(const self_sptr&, const char*, UInt64)> SubscribeEvents_type;
			typedef SubscribeEvents_type::HFNEvent		HFNSubsEvent;
			typedef function20_handle<void(const self_sptr&)>	HFNDestroy;

			typedef enum enSubscribe_Event
			{
				EN_EVENT_OPEN,
				EN_EVENT_RECV,
				EN_EVENT_CLOSE,
				EN_EVENT_COUNT
			}enSubscribe_Event;

			static self_sptr Alloc(void);

		public:
			DataSession(void);

			~DataSession(void);

			int Init(NETHANDLE hNet, const HFNSend& hSend);

			void Release(void);

			void Close(void);

			int SendText(const char* c_szData, UInt64 uSize);

			int SendBinary(const char* c_szData, UInt64 uSize);

			int AddConsumer(enSubscribe_Event Type, const HFNSubsEvent& hEvent);

			NETHANDLE Get(void);

			void Update(void);

		protected:

			int SendFrame(const Frame_Head& Head, const char* c_szData, UInt64 uSize);

			int Send(const char* c_szData, UInt32 uSize);

			void HandleRecv(NETHANDLE hNet, const StreamBuf_ptr& Stream_sptr, long lRemain);

			void HandleDestroy(NETHANDLE hNet);

			void HandleCheckHeart(void);

			bool IsOpen(void);

			void SetDestroyHandle(const HFNDestroy& hDestroy);

		private:
			Timestamp_type			_Time;
			_string_t				_Event;
			SubscribeEvents_type	_Subscribe;
			NETHANDLE	_hNet;
			HFNSend		_hSend;
			bool		_isOpen;
			Buffer_type	_Data;
			Buffer_type	_RecvData;
			HFNDestroy	_hDestroy;
		};

	}

}

#endif
