
#include <libNetFrame_CmdSession/CommandStreamCtrl.h>

namespace	_session_{

	namespace	_tcp_{

		CommandStreamCtrl::CommandStreamCtrl( UInt16 uSendSize,
			UInt16 ReservedSize )
			: _isSending(false)
			, _Pack(uSendSize, ReservedSize)
		{
			_Pack.SetFunction( function20_bind_Ex(&CommandStreamCtrl::FullPacket, this) );
		}

		CommandStreamCtrl::~CommandStreamCtrl( void )
		{

		}

		int	CommandStreamCtrl::Send( void* pAddtion, UInt8 u8AddSize,
			const char* pData, UInt16 u16Size )
		{
			if( (u16Size + u8AddSize) > _Pack.GetSendSize() ? MorePack(pAddtion, u8AddSize, pData, u16Size) \
				: OnePack(pAddtion, u8AddSize, pData, u16Size) )
			{
				SendData();
				return 1;
			}

			return -1;
		}

		int	CommandStreamCtrl::Send( const char* pData, UInt16 u16Size )
		{
			if( u16Size > _Pack.GetSendSize() ? MorePack(pData, u16Size) \
				: OnePack(pData, u16Size) )
			{
				SendData();
				return 1;
			}

			return -1;
		}

		bool	CommandStreamCtrl::MorePack( const char* pData, UInt16 u16Size  )
		{
			if( u16Size > _Pack.GetMaxPayload())
				return false;

			CmdList_ptr sptr = _Pack.BigPackage(pData, u16Size);
			if( !sptr )
			{
				return false;
			}

			_SendLock.Lock();
			while(sptr->size() > 0)
			{
				_SendList.push_back(sptr->front());
				sptr->pop();
			}
			_SendLock.UnLock();
			return true;
		}

		bool	CommandStreamCtrl::OnePack( const char* pData, UInt16 u16Size )
		{
			CmdDBuf_ptr sptr = _Pack.Package(pData, u16Size);

			if( !sptr )
			{
				return false;
			}

			_SendLock.Lock();
			_SendList.push_back(sptr);
			_SendLock.UnLock();
			return true;
		}

		bool	CommandStreamCtrl::MorePack( void* pAddtion, UInt8 u8AddSize,
			const char* pData, UInt16 u16Size )
		{
			if( u16Size > _Pack.GetMaxPayload())
				return false;

			CmdList_ptr sptr = _Pack.BigPackage(pAddtion, u8AddSize, 
				pData, u16Size);
			if( !sptr )
			{
				return false;
			}

			_SendLock.Lock();
			while(sptr->size() > 0)
			{
				_SendList.push_back(sptr->front());
				sptr->pop();
			}
			_SendLock.UnLock();
			return true;
		}

		bool	CommandStreamCtrl::OnePack( void* pAddtion, UInt8 u8AddSize,
			const char* pData, UInt16 u16Size )
		{
			CmdDBuf_ptr sptr = _Pack.Package(pAddtion, u8AddSize, pData, u16Size);

			if( !sptr )
			{
				return false;
			}

			_SendLock.Lock();
			_SendList.push_back(sptr);
			_SendLock.UnLock();
			return true;
		}

		void	CommandStreamCtrl::SendData( void )
		{
			_Lock.Lock();
			if( !_isSending )
			{
				_isSending = true;
				_Lock.UnLock();
				CmdDBuf_ptr sptr = _SendList.pop_front();
				if( sptr )
				{	
					EVENT_EXEC(SendEvent)(sptr->_Data, sptr->_uPayload);
				}
				else if( _isSending )
				{
					_isSending = false;
				}
			}
			else
				_Lock.UnLock();
		}

		void	CommandStreamCtrl::SendQue( void )
		{
			CmdDBuf_ptr sptr;

			_Lock.Lock();
			_isSending = !_SendList.empty();
			if( _isSending )
				sptr = _SendList.pop_front();
			_Lock.UnLock();

			if( sptr )
			{
				EVENT_EXEC(SendEvent)(sptr->_Data, sptr->_uPayload);
			}
			else
			{
				if( _isSending )
					_isSending = false;

				if( _SendComplete )
					_SendComplete();
			}
		}

		void	CommandStreamCtrl::HandleSend( const STREAM_HANDLE& Stream,
			const char* pData, UInt32 u32Size )
		{
			SendQue();
			_Pack.FreeBuf(pData);
		}

		void	CommandStreamCtrl::HandleRecv( const STREAM_HANDLE& Stream,
			const StreamBuf_ptr& Buf_ptr )
		{
			_Pack.Parse(StreamBuf::GetData(Buf_ptr.get()), StreamBuf::GetSize(Buf_ptr.get()));
		}

		void	CommandStreamCtrl::FullPacket( const CmdDBuf_ptr& sptr )
		{
			EVENT_EXEC_READ(sptr);
		}

	} //namespace	_tcp_

}	//namespace	_client_

