#ifndef __MODBUS_UARTS_HALFDUPLEX_20170616104004_1444199361_H__
#define __MODBUS_UARTS_HALFDUPLEX_20170616104004_1444199361_H__

#include "External.h"

namespace _modbus_ {

	namespace _rtu_ {

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

#define UART_DATA_OVERTIME_DEF	(2000)		//发送间隔 2s
#define UART_DATA_RESEND_DEF	(5)			//重发送次数 5 
#define UART_RECV_INTERVAL_DEF  (50)		//
#define UART_RECV_TURATION_DEF  (5000)		//占用总线时间报错

		typedef function20_handle<int(int, const char*, UInt32)>	HFNUartFinish;

		struct tagUartData_INFO
		{
			_mod_string	sIdentity;	//身份
			HFNUartFinish	hFinish;	//发送结束回调函数
			bool	isVaild;			//是否有效
			bool	isPush;				//是否入队
			Int16	ReSendTimer;		//重发次数
			Int16	ReSendInterval;		//重发间隔
			UInt64	EndTime;			//结束时间
			char	Data[1];
		}__M_ALIGNED_PACKED__;

		inline DataBufferPool* GetDataBufferInstance(void)
		{
			return Singleton<DataBufferPool>::instance(1470 + sizeof(tagUartData_INFO));
		}

		class Uarts_HalfDuplex : public _enable_shared_from_this<Uarts_HalfDuplex>
		{
		public:
			//typedef LinkList_type<DBuffer_sptr>	LList;

			typedef PriorityQueue<DBuffer_sptr>	LList;
			typedef function20_handle<int(const char*, UInt32, Int32*)> HFNFullPacket;

			typedef _signals_slots_::signal<void(THandle, const char*, UInt32)>	HRecvSignal;
			typedef function20_handle<void(THandle, const char*, UInt32)>		HFNRecvEvent;
			typedef function20_handle<void(THandle)>							HFNDestroy;

#pragma pack(push, 1)
			struct tagRecvBuf
			{
				UInt32 uSize;
				char Buf[1470];
			};
#pragma pack(pop)

			static void Uarts_HalfDuplex_Init(void)
			{
				_async_::_uarts_::GetAsyncUartsInstance()->Init();
			}

			static void Free(void* ptr)
			{
				((tagUartData_INFO*)((tagDBuffer*)ptr)->Data)->~tagUartData_INFO();
			}


		public:
			Uarts_HalfDuplex(void);

			~Uarts_HalfDuplex(void);

			void Init(const HFNFullPacket& hFull);

			void Release(void);

			THandle Open(const _mod_string& ComName, const _mod_string& BaudRate,
				const _mod_string& DataBit, const _mod_string& StopBit, const _mod_string& Parity,
				const HFNDestroy& hDestroy);

			int Close(void);

			int Send(const char* c_szData, UInt32 uSize, const _mod_string& sIdentity,
				const HFNUartFinish& hFinish = HFNUartFinish(), UInt8 u8Priority = 0, 
				Int16 ReSendTimer = -1, Int16 ReSendInterval = -1);

			bool is_open(void);

			int RegisterRecvEvent(const HFNRecvEvent& hEvent);

			void SetConfig(UInt16 uReSendTime, UInt16 uReSendInterval, UInt32 uOccupyTimer)
			{
				_uReSendTime = uReSendTime;
				_uReSendInterval = uReSendInterval;
				_uOccupyTimer = uOccupyTimer;
			}

			void SetFullPacket(const HFNFullPacket& hFull, const HFNUartFinish& hFullData)
			{
				_hFull = hFull;
				_hFullData = hFullData;
			}

		protected:
			void HandleRecv(THandle Handle, const char* szData, UInt32 uSize);

			void HandleDestroy(THandle Handle);

			int AddEvent(const DBuffer_sptr& sptr, bool isInit = true);

			void EventProcess(const DBuffer_sptr& sptr);

			int Send(const DBuffer_sptr& sptr, bool isAdd = true);

			void FilterPacket(const _mod_string& sIdentity);

			DBuffer_sptr GetPacket(void);

		private:
			Timestamp_type::TimeVal	_uTime;
			Timestamp_type::TimeVal	_uTuration;
			UInt16 _uReSendTime;
			UInt16 _uReSendInterval;
			UInt16 _uOccupyTimer;
			HFNFullPacket	_hFull;
			bool			_isDestroy;
			bool			_isSending;
			DBuffer_sptr	_Data;
			LList	_List;
			CLock	_Lock;
			THandle	_Handle;
			tagRecvBuf _Recv;
			HFNUartFinish _hFullData;
			HRecvSignal	_HRecvEvents;
			HFNDestroy	_hDestroy;
		};

	} //_rtu_

}

#endif