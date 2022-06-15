
#ifndef __WEBSOCKET_CLIENTS_DATASESSION_20171226154204_3344199361_H__
#define __WEBSOCKET_CLIENTS_DATASESSION_20171226154204_3344199361_H__

#include "External.h"

namespace _websocket_ {

	namespace _clients_ {

		class DataSession : public _enable_shared_from_this<DataSession>
		{
		public:
			typedef DataSession		self_type;
			typedef SmartPTR<self_type>	self_sptr;
			typedef function20_handle<int(NETHANDLE, const char*, UInt32)>	HFNSend;
			typedef _string_v30_::string< MemPool_macro<5120> >	Buffer_type;
			typedef SubscribeEvents<void(const self_sptr&, void*)>	SubscribeEvents_type;
			typedef SubscribeEvents_type::HFNEvent		HFNSubsEvent;

			typedef enum enSubscribe_Event
			{
				EN_EVENT_RECV,
				EN_EVENT_CLOSE,
				EN_EVENT_COUNT
			}enSubscribe_Event;

			static self_sptr Alloc(void);

		public:
			DataSession(void);

			~DataSession(void);

			int Init(void);

			void Release(void);

			int Open(const _string_t& sSvrIP, const UInt16 uSvrPort);

			void Close(void);

			int SendText(const char* c_szData, UInt64 uSize);

			int SendBinary(const char* c_szData, UInt64 uSize);

			int AddConsumer(enSubscribe_Event Type, const HFNSubsEvent& hEvent);

			NETHANDLE Get(void);

		protected:

			int SendFrame(const Frame_Head& Head, const char* c_szData, UInt64 uSize);

			int Send(const char* c_szData, UInt32 uSize);

			void HandleHeart(void);

			void HandleConnect(const _string_t& sSvrIP, const UInt16 uSvrPort, NETHANDLE hNet);

			void HandleToken(NETHANDLE hNet, const StreamBuf_ptr& Stream_sptr, long lRemain);

			void HandleRecv(NETHANDLE hNet, const StreamBuf_ptr& Stream_sptr, long lRemain);

			void HandleDestroy(NETHANDLE hNet);

			bool IsOpen(void);

		private:
			Timestamp_type			_Time;
			SubscribeEvents_type	_Subscribe;
			_string_t				_sMaskKey;
			_string_t				_Event;
			NETHANDLE	_hNet;
			bool		_isOpen;
			Buffer_type	_Data;
			Buffer_type	_RecvData;
		};

	}

}

#endif
