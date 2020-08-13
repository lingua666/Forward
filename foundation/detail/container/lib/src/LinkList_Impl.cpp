
#include <libContainer/LinkList_Impl.h>

namespace	_container_{

	/*!
	* @function   LinkList
	* @brief    
	* @return  
	*/

	void* LinkList_Impl::value( void* ptr )
	{
		return (void*)(((tagListNode*)ptr)->Value);
	}

	void* LinkList_Impl::next( void* ptr )
	{
		return (void*)(((tagListNode*)ptr)->pNext);
	}

	LinkList_Impl::LinkList_Impl( int iSize )
		: _u32Size(0)
		, _Pool(sizeof(tagListNode) + iSize)
	{
		_Head = reinterpret_cast<tagListNode*>(_Pool.Calloc());
		_End = _Head;
	}

	void* LinkList_Impl::push_back( void )
	{
		void* p = reinterpret_cast<void*>(_End->Value);
		_End->pNext = reinterpret_cast<tagListNode*>(_Pool.Calloc());
		_End = _End->pNext;
		_u32Size ++;
		return p;
	}

	void	LinkList_Impl::pop( void )
	{
		_u32Size --;
		tagListNode* pNode = _Head->pNext;
		_Pool.Free(_Head);
		_Head = pNode;
	}

	const UInt32 LinkList_Impl::size( void ) const 
	{
		return _u32Size;
	}

	const bool LinkList_Impl::empty( void ) const
	{
		return _u32Size == 0 ? true : false;
	}

	void* LinkList_Impl::operator[]( UInt32 Pos )
	{
		tagListNode* pNode = _Head;
		for(int i = 0; i < Pos; i ++)
			pNode = pNode->pNext;
		return pNode->Value;
	}

	void* LinkList_Impl::begin( void ) const
	{
		return _Head;
	}

	void* LinkList_Impl::end( void ) const
	{
		return _End;
	}

} // _container_