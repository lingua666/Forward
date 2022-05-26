
#ifndef __MODBUS_TRANSFERUART_20180925172708_1445927228_H__
#define __MODBUS_TRANSFERUART_20180925172708_1445927228_H__

#include "Uarts_HalfDuplex.h"
#include "ITransfer.h"

namespace _modbus_ {

	namespace _rtu_ {

		class TransferUart : public ITransfer
		{
		public:
			typedef ITransfer::HFNParse				HFNParse;
			typedef ITransfer::HFNPreOpen			HFNPreOpen;
			typedef SmartPTR<Uarts_HalfDuplex>		Uarts_HalfDuplex_sptr;
			typedef Uarts_HalfDuplex::HFNDestroy	HFNDestroy;

			static UInt16 modbus_rtu_crc16(UInt8 *buffer, UInt32 buffer_length);

			static int InitEnvironment(UInt16 uIOThreadNum = 1, UInt8 uProcThreadNum = 1);

			static TransferUart::self_sptr Alloc(void);

		public:
			TransferUart(void);

			~TransferUart(void);

			int Init(bool isOriginal = false);

			int Release(void);

			int Open(const _mod_string& ComName, const _mod_string& BaudRate,
				const _mod_string& DataBit, const _mod_string& StopBit,
				const _mod_string& Parity, const HFNDestroy& hDestroy = HFNDestroy());

			int Close(void);

			int Send(const char* c_szData, UInt32 uSize,
				const _mod_string& sIdentity,
				const HFNResponds& hResp = HFNResponds(), UInt8 u8Priority = 0,
				Int16 ReSendTimer = -1, Int16 ReSendInterval = -1);

			int RegisterRecvEvent(const HFNRecvEvent& hEvent);

			const TransferUart::Transfer_INFO& GetInfo(void) const;

			const _mod_string	GetType(void) const;

			const _mod_string	GetID(void) const;

			const _mod_string	GetSessionID(void) const;	//会话ID由ID+Type组成

			const int	GetSessionIDLength(void) const;

			void SetHandleParse(const HFNParse& hParse, const HFNResponds& hFullData)
			{
				if (_Uart_sptr)
					_Uart_sptr->SetFullPacket(hParse, hFullData);
			}

			void SetConfig(UInt16 uReSendTime, UInt16 uReSendInterval, UInt32 uOccupyTimer)
			{
				if (_Uart_sptr)
					_Uart_sptr->SetConfig(uReSendTime, uReSendInterval, uOccupyTimer);
			}

		protected:
			int PreOpen(const HFNPreOpen& hPre);

			int Open_impl(void);

			int isFullData(const char* c_szData, UInt32 uSize, Int32* iByteRead);

			int HandlePreOpen(const Uarts_HalfDuplex_sptr& sptr, const _mod_string& ComName,
				const _mod_string& BaudRate, const _mod_string& DataBit, const _mod_string& StopBit,
				const _mod_string& Parity, const HFNDestroy& hDestroy);

		private:
			Uarts_HalfDuplex_sptr	_Uart_sptr;
			Transfer_INFO			_Info;
			HFNPreOpen				_hOpenPre;
			_mod_string			_sID;
			_mod_string			_sSessionID;
			bool				_isOriginal;
		};

	}

}

#endif
