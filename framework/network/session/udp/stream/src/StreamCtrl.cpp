
#include <libNetFrame_UDP_StreamSession/StreamCtrl.h>

namespace	_session_{

	namespace	_udp_{

		StreamCtrl::StreamCtrl( UInt32 uSendSize,
			UInt32 ReservedSize )
			: _isSending(false)
			, _Pack(sizeof(sockaddr_in), uSendSize, ReservedSize)
		{
			_Pack.SetFunction( function20_bind_Ex(&StreamCtrl::FullPacket, this) );
		}

		StreamCtrl::~StreamCtrl( void )
		{
			Release();
		}

		int StreamCtrl::Init(void)
		{
			return 1;
		}

		void StreamCtrl::Release(void)
		{

		}

		UInt32	StreamCtrl::Send( sockaddr_in* pDest, void* pAddtion, UInt8 u8AddSize,
			const char* pData, UInt32 uSize, UInt16 uBufNum)
		{
			(uSize + u8AddSize) > _Pack.GetSendSize() ? MorePack(pDest, pAddtion, u8AddSize,
				pData, uSize) \
				: OnePack(pDest, pAddtion, u8AddSize, pData, uSize);

			if (_SendList.size() > uBufNum)
			{
				SendData();
			}

			return 1;
		}

		UInt32	StreamCtrl::Send( sockaddr_in* pDest, const char* pData, UInt32 uSize, UInt16 uBufNum)
		{
			uSize > _Pack.GetSendSize() ? MorePack(pDest, pData, uSize) \
				: OnePack(pDest, pData, uSize);

			if (_SendList.size() > uBufNum)
			{
				SendData();
			}

			return 1;
		}

		int	StreamCtrl::MorePack( sockaddr_in* pDest, const char* pData, UInt32 uSize )
		{
			MPList_ptr sptr = _Pack.BigPackage(pData, uSize, pDest, sizeof(sockaddr_in));
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

		int	StreamCtrl::OnePack( sockaddr_in* pDest, const char* pData, UInt32 uSize )
		{
			MediaDBuf_ptr sptr = _Pack.Package(pData, uSize, pDest, sizeof(sockaddr_in));

			if( sptr )
			{
				_SendLock.Lock();
				_SendList.push_back(sptr);
				_SendLock.UnLock();
				return 1;
			}
			return -1;
		}

		int	StreamCtrl::MorePack( sockaddr_in* pDest, void* pAddtion, UInt8 u8AddSize,
			const char* pData, UInt32 uSize )
		{
			MPList_ptr sptr = _Pack.BigPackage(pAddtion, u8AddSize, 
				pData, uSize, pDest, sizeof(sockaddr_in));
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

		int	StreamCtrl::OnePack( sockaddr_in* pDest, void* pAddtion, UInt8 u8AddSize,
			const char* pData, UInt32 uSize )
		{
			MediaDBuf_ptr sptr = _Pack.Package(pAddtion, u8AddSize,
				pData, uSize, pDest, sizeof(sockaddr_in));

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
					EVENT_EXEC(SendEvent)((sockaddr_in*)sptr->_pHead, sptr->_Data, sptr->_uPayload);
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
				EVENT_EXEC(SendEvent)((sockaddr_in*)sptr->_pHead, sptr->_Data, sptr->_uPayload);
			}
			else
			{
				if( _isSending )
					_isSending = false;

				if( _SendComplete )
					_SendComplete();
			}
		}

		void	StreamCtrl::HandleSend( const STREAM_HANDLE& Stream, sockaddr_in* pDest, 
			const char* pData, UInt32 uSize )
		{
			SendQue();
			_Pack.FreeBuf(pData);
		}

		void	StreamCtrl::HandleRecv( const STREAM_HANDLE& Stream, sockaddr_in* pSrc, 
			const StreamBuf_ptr& Buf_ptr )
		{
			_Pack.Parse((void*)pSrc, NULL, 0, Buf_ptr);
		}

		void	StreamCtrl::FullPacket( void* pUser, const StreamBuf_ptr& Buf_ptr )
		{
			EVENT_EXEC_READ((sockaddr_in*)pUser, Buf_ptr);
		}

	} //namespace	_tcp_

} //client