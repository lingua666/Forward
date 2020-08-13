
#include <libNetFrame_RawSession/RawStreamCtrl.h>

namespace	_session_{

	namespace	_tcp_{

		RawStreamCtrl::RawStreamCtrl( UInt16 uSendSize,
			UInt16 ReservedSize )
			: _Pool( uSendSize + ReservedSize + sizeof(_buf_hdr) - 1 )
			, _ListPool( sizeof(PKBList) )
			, _isSending(false)
			, _uSendSize(uSendSize)
		{

		}

		RawStreamCtrl::~RawStreamCtrl( void )
		{

		}

		int	RawStreamCtrl::Send( const char* pData, UInt32 u32Size )
		{
			u32Size > _uSendSize ? MorePack(pData, u32Size) : OnePack(pData, u32Size);
			SendData();
			return 0;
		}

		int	RawStreamCtrl::MorePack( const char* pData, UInt32 u32Size )
		{
			UInt32	u32Len = 0, u32Offset = 0;
			_buf_hdr* pHdr = NULL;
			PKBList* pList = _ListPool.AllocObj<PKBList>();
			if( pList == NULL )
				return -1;
			do 
			{//·Ö°ü
				u32Len = __min(u32Size, _uSendSize);
				u32Size -= u32Len;
				pList->push_back(Clone(&pData[u32Offset], u32Len));
				u32Offset += u32Len;
			} while ( u32Size > 0 );

			_SendLock.Lock();
			while(pList->size() > 0)
			{
				_SendList.push_back(pList->front());
				pList->pop();
			}
			_SendLock.UnLock();

			_ListPool.FreeObj(pList);
			return 0;
		}

		_buf_hdr*	RawStreamCtrl::Clone( const char* pData, UInt32 u32Size )
		{
			_buf_hdr* pHdr = reinterpret_cast<_buf_hdr*>(_Pool.Alloc());
			if( pHdr != NULL )
			{
				memcpy(StreamBuf::GetData(pHdr), pData, u32Size);
				StreamBuf::SetSize( pHdr, u32Size );
			}
			return pHdr;
		}

		int	RawStreamCtrl::OnePack( const char* pData, UInt32 u32Size )
		{
			_buf_hdr* pHdr = Clone(pData, u32Size);
			if( pHdr != NULL )
			{
				_SendList.push_back(pHdr);
			}
			return 0;
		}

		void	RawStreamCtrl::SendData( void )
		{
			_Lock.Lock();
			if( !_isSending )
			{
				_isSending = true;
				_Lock.UnLock();
				_buf_hdr* pHdr = _SendList.pop_front();
				if( pHdr != NULL )
				{
					EVENT_EXEC(SendEvent)(StreamBuf::GetData(pHdr),
						StreamBuf::GetSize(pHdr));
				}
				else if( _isSending )
				{
					_isSending = false;
				}
			}
			else
				_Lock.UnLock();
		}

		void	RawStreamCtrl::SendQue( void )
		{
			_buf_hdr* pHdr = NULL;
			_Lock.Lock();
			_isSending = !_SendList.empty();
			if( _isSending )
				pHdr = _SendList.pop_front();
			_Lock.UnLock();

			if( pHdr != NULL )
			{	
				EVENT_EXEC(SendEvent)(StreamBuf::GetData(pHdr), StreamBuf::GetSize(pHdr));	
			}
			else
			{
				if( _isSending )
					_isSending = false;
			}
		}

		void	RawStreamCtrl::HandleSend( const STREAM_HANDLE& Stream,
			const char* pData, UInt32 u32Size )
		{
			SendQue();
			_Pool.Free( reinterpret_cast<void*>(StreamBuf::GetHdrToData(pData)) );
		}

		void	RawStreamCtrl::HandleRecv( const STREAM_HANDLE& Stream,
			const StreamBuf_ptr& Buf_ptr )
		{
			EVENT_EXEC_READ(Stream, Buf_ptr);
		}

	} //namespace	_tcp_

}	//namespace	_client_

