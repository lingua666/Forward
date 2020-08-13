
#ifndef __DOUBLELINKLIST_20150721173059_1437471059_H__
#define __DOUBLELINKLIST_20150721173059_1437471059_H__

#include <libMempool/MemPool.h>

namespace	_container_{

	template<typename T, typename Allocator =  MemPool_type>
	class DoubleLinkList
	{
	public:
		struct stListNode
		{
			T pNode;
			stListNode* pDouble;
			stListNode( void )
			{
				pNode = NULL;
				pDouble = NULL;
			}
		};
	public:

		DoubleLinkList( void ) : _u32Size(0)
			, _pool(sizeof(stListNode))
		{
			_Head = reinterpret_cast<stListNode*>(_pool.AllocObj<stListNode>());
			_Next = _Head;
		}

		~DoubleLinkList( void )
		{

		}

		void push_back( const T& Value )
		{
			_Next->pNode = Value;
			_Next->pNext = reinterpret_cast<stListNode*>(_pool.AllocObj<stListNode>());
			_Next = _Next->pNext;
			_u32Size ++;
			return ;
		}

		const T&	pop_front( void )
		{
			if( size() == 0 )
				return NULL;

			const  stListNode* tm = NULL;
			tm = _Head;
			_Head = tm->pNext;
			const  T val = tm->pNode;
			_u32Size --;
			_pool.FreeObj(tm);
			return val;
		}

		void clear( void )
		{
			while( _u32Size > 0 )
			{
				pop_front();
			};	

			_u32Size = 0;
		}

		const UInt32 size( void ) const 
		{
			return _u32Size;
		}

	public:
		UInt32	_u32Size;
		stListNode*	_Head;
		stListNode*	_Next;
		Allocator	_pool;
	};

} //_container_


#endif//__LINKLIST_20150721173059_1437471059_H__
