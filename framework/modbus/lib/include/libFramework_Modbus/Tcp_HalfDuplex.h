#ifndef __MODBUS_TCP_HALFDUPLEX_20170616104004_1444199361_H__
#define __MODBUS_TCP_HALFDUPLEX_20170616104004_1444199361_H__

#include "External.h"

namespace _modbus_ {

	namespace _tcp_ {

		struct tagDBuffer
		{
			UInt32 uSize;			//在arm下必须保持最小字节数为4的倍数
			char   Data[1];
		} __M_ALIGNED_PACKED__;

		typedef	SmartPTR<tagDBuffer>	DBuffer_sptr;
		typedef function20_handle<void(void*)>	HFNDBDel;

		struct tagDBuffer_INFO
		{
			HFNDBDel hDel;
			char	 Data[1];
		} __M_ALIGNED_PACKED__;

		class DataBufferPool
		{
		public:
			DataBufferPool(void)
				: _pPool(NULL)
			{

			}

			DataBufferPool(UInt32 uSize)
			{
				_pPool = new MemPool_type(uSize + sizeof(tagDBuffer_INFO) + sizeof(tagDBuffer) + 1);
			}

			DBuffer_sptr Alloc(void)
			{
				if (_pPool == NULL)
					return DBuffer_sptr();

				tagDBuffer_INFO* pInfo = _pPool->AllocObj<tagDBuffer_INFO>();
				return DBuffer_sptr((tagDBuffer*)pInfo->Data,
					function20_bind(&DataBufferPool::Free, this,
						pInfo, _function_::_1));
			}

			template<typename T>
			DBuffer_sptr Alloc(void)
			{
				if (_pPool == NULL)
					return DBuffer_sptr();

				tagDBuffer_INFO* pInfo = _pPool->AllocObj<tagDBuffer_INFO>();
				return DBuffer_sptr((tagDBuffer*)pInfo->Data,
					function20_bind(&DataBufferPool::FreeObj<T>, this,
						pInfo, _function_::_1));
			}

			DBuffer_sptr Alloc(const HFNDBDel& hDel)
			{
				if (_pPool == NULL)
					return DBuffer_sptr();

				tagDBuffer_INFO* pInfo = _pPool->AllocObj<tagDBuffer_INFO>();
				pInfo->hDel = hDel;

				return DBuffer_sptr((tagDBuffer*)pInfo->Data,
					function20_bind(&DataBufferPool::Free, this,
						pInfo, _function_::_1));
			}

		protected:

			template<typename T>
			void FreeObj(tagDBuffer_INFO* pInfo, T* ptr)
			{
				ptr->~T();
				_pPool->FreeObj(pInfo);
			}

			void Free(tagDBuffer_INFO* pInfo, void* ptr)
			{
				if (pInfo->hDel)
					pInfo->hDel(ptr);
				_pPool->FreeObj(pInfo);
			}

		private:
			HFNDBDel		_Del;
			MemPool_type*	_pPool;
		};

#define TCP_DATA_OVERTIME_DEF	(2000)		//2秒
#define TCP_DATA_RESEND_DEF		(5)			
#define TCP_RECV_INTERVAL_DEF  (50)
#define TCP_RECV_TURATION_DEF  (5000)

		typedef function20_handle<int(int, const char*, UInt32)>	HFNTcpFinish;

		struct tagTcpData_INFO
		{
			_mod_string	sIdentity;	//身份
			HFNTcpFinish	hFinish;	//发送结束回调函数
			bool	isVaild;			//是否有效
			Int16	ReSendTimer;		//重发次数
			Int16	ReSendInterval;		//重发间隔
			UInt64	EndTime;			//结束时间
			char	Data[1];
		}__M_ALIGNED_PACKED__;

		inline DataBufferPool* GetDataBufferInstance(void)
		{
			return Singleton<DataBufferPool>::instance(1470 + sizeof(tagTcpData_INFO));
		}

		class Tcp_HalfDuplex : public _enable_shared_from_this<Tcp_HalfDuplex>
		{
		public:
			typedef _io_net_::StreamBuf_ptr		StreamBuf_ptr;
			typedef PriorityQueue<DBuffer_sptr>	LList;
			typedef function20_handle<int(const char*, UInt32, Int32*)> HFNFullPacket;

			typedef _signals_slots_::signal<void(THandle, const char*, UInt32)>	HRecvSignal;
			typedef function20_handle<void(THandle, const char*, UInt32)>			HFNRecvEvent;
			typedef function20_handle<void(THandle)>								HFNDestroy;

#pragma pack(push, 1)
			struct tagRecvBuf
			{
				UInt32 uSize;
				char Buf[1470];
			};
#pragma pack(pop)

			static void Tcp_HalfDuplex_Init(void)
			{
				GetAsynStreamClientsInstance()->Init();
			}

			static void Free(void* ptr)
			{
				((tagTcpData_INFO*)((tagDBuffer*)ptr)->Data)->~tagTcpData_INFO();
			}


		public:
			Tcp_HalfDuplex(void);

			~Tcp_HalfDuplex(void);

			void Init(const HFNFullPacket& hFull);

			void Release(void);

			THandle Open(const _mod_string& sIP, UInt16 uPort, const HFNDestroy& hDestroy = HFNDestroy());

			int Close(void);

			int Send(const char* c_szData, UInt32 uSize, const _mod_string& sIdentity,
				const HFNTcpFinish& hFinish = HFNTcpFinish(), UInt8 u8Priority = 0,
				Int16 ReSendTimer = -1, Int16 ReSendInterval = -1);

			bool is_open(void);

			int RegisterRecvEvent(const HFNRecvEvent& hEvent);

			void SetFullPacket(const HFNFullPacket& hFull, const HFNTcpFinish& hFullData)
			{
				_hFull = hFull;
				_hFullData = hFullData;
			}

		protected:
			void HandleRecv(THandle Handle, const StreamBuf_ptr& Buf_ptr, long lRemain);

			void HandleDestroy(const HFNDestroy& hDestroy, THandle Handle);

			int AddEvent(const DBuffer_sptr& sptr, bool isInit = true);

			void EventProcess(const DBuffer_sptr& sptr);

			int Send(const DBuffer_sptr& sptr, bool isAdd = true);

			void FilterPacket(const _mod_string& sIdentity);

			DBuffer_sptr GetPacket(void);

			void HandleConnect(const HFNDestroy& hDestroy, const _mod_string& sIP, int u16Port, NETHANDLE Handle);

		private:
			Timestamp_type::TimeVal	_uTime;
			Timestamp_type::TimeVal	_uTuration;
			HFNFullPacket	_hFull;
			bool			_isDestroy;
			bool			_isSending;
			DBuffer_sptr	_Data;
			LList	_List;
			CLock	_Lock;
			THandle	_Handle;
			tagRecvBuf _Recv;
			HFNTcpFinish _hFullData;
			HRecvSignal	_HRecvEvents;
			HFNDestroy	_hDestroy;
			UInt16		_uSeq;
		};

	}

}

#endif