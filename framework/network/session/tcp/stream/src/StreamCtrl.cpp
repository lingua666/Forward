
#include <libNetFrame_StreamSession/StreamCtrl.h>

namespace	_session_{

	namespace	_tcp_{

		StreamCtrl::StreamCtrl( UInt16 uSendSize,
			UInt16 ReservedSize )
			: _isSending(false)
			, _Pack(uSendSize, ReservedSize)
		{
			_Pack.SetFunction( function20_bind_Ex(&StreamCtrl::FullPacket, this) );
		}

		StreamCtrl::~StreamCtrl( void )
		{

		}

		UInt32	StreamCtrl::Send( void* pAddtion, UInt8 u8AddSize,
			const char* pData, UInt16 u16Size )
		{
			(u16Size + u8AddSize) > _Pack.GetSendSize() ? MorePack(pAddtion, u8AddSize, pData, u16Size) \
				: OnePack(pAddtion, u8AddSize, pData, u16Size);
			SendData();
			return 1;
		}

		UInt32	StreamCtrl::Send( const char* pData, UInt16 u16Size )
		{
			u16Size > _Pack.GetSendSize() ? MorePack(pData, u16Size) \
				: OnePack(pData, u16Size);
			SendData();
			return 1;
		}

		int	StreamCtrl::MorePack( const char* pData, UInt16 u16Size )
		{
			MPList_ptr sptr = _Pack.BigPackage(pData, u16Size);
			if( sptr )
			{
				_SendLock.Lock();
				while(sptr->size() > 0)
				{
					_SendList.push_back(sptr->front());
					sptr->pop();
				}
				_SendLock.UnLock();
				return 1;
			}
			return -1;
		}

		int	StreamCtrl::OnePack( const char* pData, UInt16 u16Size )
		{
			MediaDBuf_ptr sptr = _Pack.Package(pData, u16Size);

			if( sptr )
			{
				_SendLock.Lock();
				_SendList.push_back(sptr);
				_SendLock.UnLock();
				return 1;
			}
			return -1;
		}

		int	StreamCtrl::MorePack( void* pAddtion, UInt8 u8AddSize,
			const char* pData, UInt16 u16Size )
		{
			MPList_ptr sptr = _Pack.BigPackage(pAddtion, u8AddSize, 
				pData, u16Size);
			if( sptr )
			{
				_SendLock.Lock();
				while(sptr->size() > 0)
				{
					_SendList.push_back(sptr->front());
					sptr->pop();
				}
				_SendLock.UnLock();
				return 1;
			}
			return -1;
		}

		int	StreamCtrl::OnePack( void* pAddtion, UInt8 u8AddSize,
			const char* pData, UInt16 u16Size )
		{
			MediaDBuf_ptr sptr = _Pack.Package(pAddtion, u8AddSize, pData, u16Size);

			if( sptr )
			{
				_SendLock.Lock();
				_SendList.push_back(sptr);
				_SendLock.UnLock();
				return 1;
			}
			return -1;
		}

		void	StreamCtrl::SendData( void )
		{
			_Lock.Lock();
			if( !_isSending )
			{
				_isSending = true;
				_Lock.UnLock();
				MediaDBuf_ptr sptr = _SendList.pop_front();
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

		void	StreamCtrl::SendQue( void )
		{
			MediaDBuf_ptr sptr;

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

		void	StreamCtrl::HandleSend( const STREAM_HANDLE& Stream, 
			const char* pData, UInt32 u32Size )
		{
			SendQue();
			_Pack.FreeBuf(pData);
		}

		void	StreamCtrl::HandleRecv( const STREAM_HANDLE& Stream,
			const StreamBuf_ptr& Buf_ptr )
		{
			_Pack.Parse(NULL, 0, Buf_ptr);
		}

		void	StreamCtrl::FullPacket( const StreamBuf_ptr& Buf_ptr )
		{
			EVENT_EXEC_READ(Buf_ptr);
		}

	} //namespace	_tcp_

} //client