
#ifndef __MODBUS_ITRANSFER_20180925172708_1445927228_H__
#define __MODBUS_ITRANSFER_20180925172708_1445927228_H__

#include <libCommon/Type_Basis.h>
#include <libFrameWork_Modbus/External.h>

namespace _modbus_ {

	class ITransfer
	{
	public:
		typedef function20_handle<int(void)>							HFNPreOpen;
		typedef function20_handle<int(ITransfer*)>						HFNPreSend;
		typedef function20_handle<void(THandle, const char*, UInt32)>	HFNRecvEvent;
		typedef function20_handle<int(int, const char*, UInt32)>		HFNResponds;
		typedef function20_handle<int(const char*, UInt32, Int32*)>		HFNParse;
		typedef SmartPTR<ITransfer>										self_sptr;

		typedef enum Transfer_TYPE
		{
			enType_RTU,
			enType_TCP
		};

		struct Transfer_INFO
		{
			_mod_string			sName;
			Transfer_TYPE		Type;
			int					iIndex;
		};

	public:
		virtual ~ITransfer(void)
		{}

		virtual int Init(bool isOriginal = true) = 0;

		virtual int Release(void) = 0;

		virtual	int Close(void) = 0;

		virtual	int Send(const char* c_szData, UInt32 uSize, const _mod_string& sIdentity = "",
			const HFNResponds& hResp = HFNResponds(), UInt8 u8Priority = 0,
			Int16 ReSendTimer = -1, Int16 ReSendInterval = -1) = 0;

		virtual int RegisterRecvEvent(const HFNRecvEvent& hEvent) = 0;

		virtual const Transfer_INFO& GetInfo(void) const = 0;

		virtual	const _mod_string	GetType(void) const = 0;

		virtual const _mod_string	GetID(void) const = 0;

		virtual const _mod_string	GetSessionID(void) const = 0;

		virtual const int	GetSessionIDLength(void) const = 0;

		virtual	void SetHandleParse(const HFNParse& hParse, const HFNResponds& hFullData) = 0;

		virtual void SetConfig(UInt16 uReSendTime, UInt16 uReSendInterval, UInt32 uOccupyTimer) = 0;

	protected:
		virtual	int PreOpen(const HFNPreOpen& hPre) = 0;

		virtual	int Open_impl(void) = 0;
	};

}

#endif