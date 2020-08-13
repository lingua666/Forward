
#include <libContainer/Queue_Impl.h>

namespace	_container_ {

	void* Queue_Impl::value( const tagQueueInfo* ptr )
	{
		return &((stChunk*)ptr->_pValue)->_pBuf[ptr->_u16CurPos];
	}

	tagQueueInfo* Queue_Impl::next( tagQueueInfo* ptr )
	{
		ptr->_u16CurPos += ptr->_u16ValueSize;
		if( ptr->_u16CurPos < ptr->_u16ChunkSize )
		{
			return ptr;
		}
		
		if( ((stChunk*)ptr->_pValue)->_pNext == NULL )
			return NULL;
		
		ptr->_pValue = ((stChunk*)ptr->_pValue)->_pNext;
		ptr->_u16CurPos = 0;
		return ptr;
	}

	Queue_Impl::Queue_Impl( int iSize )
		: _u16ChunkSize( QUEQUE_CHUNK_NUMBER * iSize )
		, _u32Size( 0 )
		, _u16CurPos( 0 )
		, _u16ValueSize( iSize )
		, _Pool( sizeof(stChunk) + _u16ChunkSize )
	{
		_pCurChunk = _pHeadChunk = CreateChunk();
	}

	void*	Queue_Impl::push_back( void )
	{
		if( _u16Remain == 0 )
		{
			_pCurChunk->_pNext = CreateChunk();
			_pCurChunk = _pCurChunk->_pNext;
		}

		void* p = reinterpret_cast<void*>(&_pCurChunk->_pBuf[_u16ChunkSize - _u16Remain]);

		_u16Remain -= _u16ValueSize;
		_u32Size ++;
		return p;
	}

	void	Queue_Impl::pop_front( void )
	{
		if( !empty() )
		{
			_u16CurPos += _u16ValueSize;
			if( _u16CurPos >= _u16ChunkSize )
			{
				stChunk*	pTempChunk = _pHeadChunk->_pNext;
				if( pTempChunk != NULL )
				{
					_Pool.Free(_pHeadChunk);
					_pHeadChunk = (stChunk*)pTempChunk;
				}
				else
				{
					_u16Remain = _u16ChunkSize;
					_pHeadChunk->_pNext = NULL;
					_pCurChunk = _pHeadChunk;
				}
				_u16CurPos = 0;
			}
			-- _u32Size;
		}
	}

	UInt32	Queue_Impl::size( void ) const
	{
		return _u32Size;
	}

	bool	Queue_Impl::empty( void ) const
	{
		return (_u32Size == 0);
	}

	void* Queue_Impl::operator[]( UInt32 Pos )
	{
		int Length = Pos * _u16ValueSize;
		stChunk*	pTempChunk = _pHeadChunk;
		UInt16 uCurPos = _u16CurPos;
gt_start:
		if( Length < _u16ChunkSize - uCurPos )
		{
			return &pTempChunk->_pBuf[uCurPos + Length];
		}
		else
		{
			pTempChunk = _pHeadChunk->_pNext;
			if( pTempChunk == NULL )
				return NULL;
			Length -= _u16ChunkSize - uCurPos;
			uCurPos = 0;
			goto gt_start;
		}

		return NULL;
	}

	void Queue_Impl::begin( tagQueueInfo* pInfo )
	{
		pInfo->_pValue = (void*)_pHeadChunk;
		pInfo->_u16ChunkSize = _u16ChunkSize;
		pInfo->_u16CurPos = _u16CurPos;
		pInfo->_u16ValueSize = _u16ValueSize;
	}

	void Queue_Impl::end( tagQueueInfo* pInfo )
	{
		pInfo->_pValue = (void*)_pCurChunk;
		pInfo->_u16ChunkSize = _u16ChunkSize;
		pInfo->_u16CurPos = _u16ChunkSize - _u16Remain;
		pInfo->_u16ValueSize = _u16ValueSize;
	}

	Queue_Impl::stChunk*	Queue_Impl::CreateChunk( void )
	{
		stChunk* ptr = reinterpret_cast<stChunk*>(_Pool.Alloc());
		ptr->_pNext = NULL;
		_u16Remain = _u16ChunkSize;
		return ptr;
	}

} //_container_