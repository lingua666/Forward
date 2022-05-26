
#include <libFrameWork_Modbus/TransferTcp.h>
#include <libFrameWork_Modbus/TransferTcp_Init.h>

namespace _modbus_ {

	namespace _tcp_ {

		int TransferTcp::InitEnvironment(UInt16 uIOThreadNum, UInt8 uProcThreadNum, bool isSequence)
		{
			static TransferTcp_Init	Tcp_Init(uIOThreadNum, uProcThreadNum, isSequence);
			return 1;
		}

		TransferTcp::self_sptr TransferTcp::Alloc(void)
		{
			static MemPool_type s_Pool(sizeof(TransferTcp));
			ITransfer* pTransfer = reinterpret_cast<ITransfer*>(s_Pool.AllocObj<TransferTcp>());
			return self_sptr(pTransfer,
				function20_bind(&MemPool_type::FreeMemObj<TransferTcp>, &s_Pool, pTransfer));
		}

		TransferTcp::TransferTcp(void)
		{
			_Info.Type = enType_TCP;
		}

		TransferTcp::~TransferTcp(void)
		{
			Release();
		}

		int TransferTcp::Init(bool isOriginal)
		{
			return 1;
		}

		int TransferTcp::Release(void)
		{
			if (_Tcp_sptr)
				_Tcp_sptr->Release();

			return 1;
		}

		int TransferTcp::Open(const _mod_string& sIP, UInt16 uPort, const HFNDestroy& hDestroy)
		{
			Tcp_HalfDuplex_sptr sptr = Tcp_HalfDuplex_sptr(new Tcp_HalfDuplex());
			sptr->Init(function20_bind_Ex(&TransferTcp::isFullData, this));

			PreOpen(function20_bind(&TransferTcp::HandlePreOpen, this,
				sptr, sIP, uPort, hDestroy));

			if (Open_impl() == -1)
				return -1;

			_Tcp_sptr = sptr;
			_Info.iIndex = -1;
			_Info.sName = sIP;

			_uuid_::MD5Engine Md5;
			Md5.update(_Info.sName.c_str(), _Info.sName.size());
			_sID = _uuid_::DigestEngine::digestToHex(Md5.digest()).c_str();
			_sSessionID = GetType() + _sID;
			return 1;
		}

		int TransferTcp::HandlePreOpen(const Tcp_HalfDuplex_sptr& sptr, const _mod_string& sIP,
			UInt16 uPort, const HFNDestroy& hDestroy)
		{
			Close();

			if (sptr->Open(sIP, uPort, hDestroy) == -1)
				return -1;

			return 1;
		}

		int TransferTcp::Close(void)
		{
			if (!_Tcp_sptr)
				return -1;

			return _Tcp_sptr->Close();
		}

		int TransferTcp::PreOpen(const HFNPreOpen& hPre)
		{
			_hOpenPre = hPre;
			return 1;
		}

		int TransferTcp::Open_impl(void)
		{
			return _hOpenPre ? _hOpenPre() : -1;
		}

		int TransferTcp::Send(const char* c_szData, UInt32 uSize,
			const _mod_string& sIdentity, const HFNResponds& hResp, UInt8 u8Priority,
			Int16 ReSendTimer, Int16 ReSendInterval)
		{
			if (!_Tcp_sptr)
				return -1;

			return _Tcp_sptr->Send(c_szData, uSize, sIdentity, hResp, u8Priority, ReSendTimer, ReSendInterval);
		}

		const TransferTcp::Transfer_INFO& TransferTcp::GetInfo(void) const
		{
			return _Info;
		}

		int TransferTcp::isFullData(const char* c_szData, UInt32 uSize, Int32* iByteRead)
		{
			const int MBAP = 6;
			//modubus数据格式
			//【1字节】   【1字节】  【1字节】  【N字节】   【2字节】
			//【从机地址】【功能号】【数据长度】【数据内容】【CRC校验】 
			if ( /*c_szData[0] == 0x01 &&*/ uSize > 3)
			{
				UInt8 uHead = 0, uLen = 0;
				switch ((UInt8)c_szData[1 + MBAP])
				{
				case 0x01://读线圈寄存器
				case 0x02://读离散输入寄存器
				case 0x03://读保持寄存器
				case 0x04://读输入寄存器
				{
					uHead = 3 + MBAP;
					uLen = uHead + c_szData[2 + MBAP];
				}
				break;
				case 0x05://写单个线圈寄存器
				case 0x06://写单个保持寄存器
				{
					uHead = 4 + MBAP;
					uLen = uHead + 2;
				}
				break;
				case 0x0F://写多个线圈寄存器
				case 0x10://写多个保持寄存器
				{
					uHead = 6 + MBAP;
					uLen = uHead;
				}
				break;
				default:
				{
					printf("unknow code:%d\r\n", c_szData[1 + MBAP]);
					return -1;
				}
				break;
				}

				if (uSize >= uLen)
				{
					*iByteRead = uSize;
					return uHead;
				}
			}

			return -1;
		}

		int TransferTcp::RegisterRecvEvent(const HFNRecvEvent& hEvent)
		{
			if (!_Tcp_sptr)
				return -1;

			return _Tcp_sptr->RegisterRecvEvent(hEvent);
		}

		const _mod_string	TransferTcp::GetType(void) const
		{
			return "00";
		}

		const _mod_string	TransferTcp::GetID(void) const
		{
			return _sID;
		}

		const _mod_string	TransferTcp::GetSessionID(void) const
		{
			return _sSessionID;
		}

		const int	TransferTcp::GetSessionIDLength(void) const
		{
			return _sSessionID.size();
		}

	}

}
