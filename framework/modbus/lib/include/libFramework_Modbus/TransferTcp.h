
#ifndef __MODBUS_TRANSFERTCP_20180925172708_1445927228_H__
#define __MODBUS_TRANSFERTCP_20180925172708_1445927228_H__

#include "Tcp_HalfDuplex.h"
#include "ITransfer.h"

namespace _modbus_ {

	namespace _tcp_ {

		class TransferTcp : public ITransfer
		{
		public:
			typedef ITransfer::HFNParse				HFNParse;
			typedef ITransfer::HFNPreOpen			HFNPreOpen;
			typedef SmartPTR<Tcp_HalfDuplex>		Tcp_HalfDuplex_sptr;
			typedef Tcp_HalfDuplex::HFNDestroy		HFNDestroy;

			static int InitEnvironment(UInt16 uIOThreadNum = 1, UInt8 uProcThreadNum = 1, bool isSequence = true);

			static TransferTcp::self_sptr Alloc(void);

		public:
			TransferTcp(void);

			~TransferTcp(void);

			int Init(bool isOriginal = false);

			int Release(void);

			int Open(const _mod_string& sIP, UInt16 uPort,
				const HFNDestroy& hDestroy = HFNDestroy(), bool isRtu = false);

			int Close(void);

			int Send(const char* c_szData, UInt32 uSize,
				const _mod_string& sIdentity,
				const HFNResponds& hResp = HFNResponds(), UInt8 u8Priority = 0,
				Int16 ReSendTimer = -1, Int16 ReSendInterval = -1);

			int RegisterRecvEvent(const HFNRecvEvent& hEvent);

			const TransferTcp::Transfer_INFO& GetInfo(void) const;

			const _mod_string	GetType(void) const;

			const _mod_string	GetID(void) const;

			const _mod_string	GetSessionID(void) const;	//会话ID由ID+Type组成

			const int	GetSessionIDLength(void) const;

			void SetHandleParse(const HFNParse& hParse, const HFNResponds& hFullData)
			{
				if (_Tcp_sptr)
					_Tcp_sptr->SetFullPacket(hParse, hFullData);
			}

			void SetConfig(UInt16 uReSendTime, UInt16 uReSendInterval, UInt32 uOccupyTimer)
			{
				if (_Tcp_sptr)
					_Tcp_sptr->SetConfig(uReSendTime, uReSendInterval, uOccupyTimer);
			}

		protected:
			int PreOpen(const HFNPreOpen& hPre);

			int Open_impl(void);

			int isFullData(const char* c_szData, UInt32 uSize, Int32* iByteRead);

			int HandlePreOpen(const Tcp_HalfDuplex_sptr& sptr, const _mod_string& sIP,
				UInt16 uPort, const HFNDestroy& hDestroy, bool isRtu);

		private:
			Tcp_HalfDuplex_sptr		_Tcp_sptr;
			Transfer_INFO			_Info;
			HFNPreOpen				_hOpenPre;
			_mod_string				_sID;
			_mod_string				_sSessionID;
			bool					_isRtu;
		};

	}

}

#endif
