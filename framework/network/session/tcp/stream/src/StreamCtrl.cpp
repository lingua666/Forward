
#include <libNetFrame_StreamSession/StreamCtrl.h>

namespace	_session_{

	namespace	_tcp_{

		StreamCtrl::StreamCtrl( UInt32 uSendSize,
			UInt32 ReservedSize )
			: _isSending(false)
			, _Pack(uSendSize, ReservedSize)
		{
			_Pack.SetFunction( function20_bind_Ex(&StreamCtrl::FullPacket, this) );
		}

		StreamCtrl::~StreamCtrl( void )
		{

		}

		UInt32	StreamCtrl::Send( void* pAddtion, UInt8 u8AddSize,
			const char* pData, UInt32 uSize )
		{
			(uSize + u8AddSize) > _Pack.GetSendSize() ? MorePack(pAddtion, u8AddSize, pData, uSize) \
				: OnePack(pAddtion, u8AddSize, pData, uSize);
			SendData();
			return 1;
		}

		UInt32	StreamCtrl::Send( const char* pData, UInt32 uSize )
		{
			uSize > _Pack.GetSendSize() ? MorePack(pData, uSize) \
				: OnePack(pData, uSize);
			SendData();
			return 1;
		}

		int	StreamCtrl::MorePack( const char* pData, UInt32 uSize )
		{
			MPList_ptr sptr = _Pack.BigPackage(pData, uSize);
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

		int	StreamCtrl::OnePack( const char* pData, UInt32 uSize )
		{
			MediaDBuf_ptr sptr = _Pack.Package(pData, uSize);

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
			const char* pData, UInt32 uSize )
		{
			MPList_ptr sptr = _Pack.BigPackage(pAddtion, u8AddSize, 
				pData, uSize);
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
			const char* pData, UInt32 uSize )
		{
			MediaDBuf_ptr sptr = _Pack.Package(pAddtion, u8AddSize, pData, uSize);

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

		void	StreamCtrl::SendQue( const char* pHasData,  UInt32 HasSize )
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
					_SendComplete(pHasData, HasSize);
			}
		}

		void	StreamCtrl::HandleSend( const STREAM_HANDLE& Stream, 
			const char* pData, UInt32 u32Size )
		{
			SendQue(pData, u32Size);
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