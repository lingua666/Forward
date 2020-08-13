
#include <libContainer/CycleList_Impl.h>

namespace	_container_{

	CycleList_Impl::CycleList_Impl( int iSize )
		: _u32Size(0)
		, _Pool( sizeof(tagListNode) + iSize )
	{
		_Current = _End = _Head = reinterpret_cast<tagListNode*>(_Pool.Calloc());
		_Head->pNext = NULL;
		_Head->pPrev = NULL;
	}

	void*	CycleList_Impl::push_back( void )
	{
		_End->pNext = reinterpret_cast<tagListNode*>(_Pool.Calloc());
		_End->pNext->pNext = NULL;
		_End->pNext->pPrev = _End;
		_End = _End->pNext;
		_u32Size ++;
		return _End->pPrev->Value;
	}

	void	CycleList_Impl::pop( void )
	{
		if( _Current == _Head )
		{
			pop_front();
			_Current = _Head;
		}
		else
		{
			if( _u32Size > 0 )
			{
				_u32Size --;
				tagListNode*	pNode = _Current;
				if( _Current->pPrev != NULL )
				{
					_Current->pPrev->pNext = _Current->pNext;
				}

				if( _Current->pNext != NULL && _Current->pNext->pNext != NULL )
				{
					_Current->pNext->pPrev = _Current->pPrev;
					_Current = _Current->pNext;
				}
				else
				{
					_End->pPrev = _Current->pPrev;
					_Current = _Head;
				}

				_Pool.Free(pNode);
			}
		}
	}

	int	CycleList_Impl::next( void )
	{
		if( empty() )
			return -1;

		if( _Current->pNext != NULL &&
			_Current->pNext->pNext != NULL )
		{
			_Current = _Current->pNext;
		}
		else
		{
			_Current = _Head;
		}
		return 1;			
	}

	const UInt32 CycleList_Impl::size( void ) const 
	{
		return _u32Size;
	}

	const bool CycleList_Impl::empty( void ) const
	{
		return _u32Size == 0 ? true : false;
	}

	void CycleList_Impl::pop_front( void )
	{
		if( _u32Size > 0 )
		{
			_u32Size --;

			tagListNode*	pNode = _Head;
			_Head = _Head->pNext;
			_Head->pPrev = NULL;
			_Pool.Free(pNode);
			return ;
		}
	}

} ; //_container_

