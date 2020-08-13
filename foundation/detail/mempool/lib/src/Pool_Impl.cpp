
#include <libMempool/Pool_Impl.h>

namespace _pool_{

#define	MEMCHUNK_SIZE		4//8		//长度必须在1 - 65535之间

	struct	ListNode
	{
#pragma pack(push, 1)
		typedef struct stNode
		{
			HNODE _Next;	// successor node, or first element if head
		} node_type;
#pragma pack(pop)

#define DATA_HEAD_SIZE	sizeof(stNode)

		inline static	const UInt32	GetHeadSize( void )
		{
			return DATA_HEAD_SIZE;
		}

		inline static	void*	Data( HNODE handle )
		{
			return reinterpret_cast<void*>(reinterpret_cast<char*>(handle) + DATA_HEAD_SIZE);
		}

		inline static	HNODE	GetToData( void* pData )
		{
			return reinterpret_cast<HNODE>(reinterpret_cast<char*>(pData) - DATA_HEAD_SIZE);
		}

		inline static	void	SetNext( HNODE hNode, HNODE hNext )
		{
			reinterpret_cast<node_type*>(hNode)->_Next = hNext;
		}

		inline static	HNODE	GetNext( HNODE hNode ) 
		{
			return reinterpret_cast<node_type*>(hNode)->_Next;
		}
	};


	MemChunk::MemChunk( const UInt32 u32Size, const UInt16 u16Num )
		: _u32Size(u32Size)
		, _u16Num(u16Num)
	{
		_NodeList = Reallocate();
		ListNode::SetNext(_NodeList, NULL);
	}

	HNODE	MemChunk::Alloc( void )
	{
		_u32Free -= _u32Size;
		HNODE hNode = reinterpret_cast<HNODE>(reinterpret_cast<char*>(_NodeList) + GetHeadSize(_u32Free));
		ListNode::SetNext(hNode, NULL);
		if( _u32Free == 0 )
		{
			HNODE h = Reallocate();
			ListNode::SetNext(h, _NodeList);
			_NodeList = h;
		}
		return hNode;
	}

	void MemChunk::Init()
	{
		_NodeList = Reallocate();
		ListNode::SetNext(_NodeList, NULL);
	}

	void MemChunk::Release( void )
	{
		HNODE _Next;
		while( _NodeList )
		{
			_Next = ListNode::GetNext(_NodeList);
			MemoryAllocator::Free(_NodeList);
			_NodeList = _Next;
		}
	}

	UInt32 MemChunk::GetLength( void ) const
	{
		return _u32Size;
	}

	HNODE MemChunk::Reallocate( void )
	{
		_u32Free = _u32Size * _u16Num;
		return MemoryAllocator::Alloc( GetHeadSize(_u32Free) );
	}

	const UInt32 MemChunk::GetHeadSize( const UInt32 u32Size ) const
	{
		return ListNode::GetHeadSize() + u32Size;
	}

	const UInt32 MemChunk::GetChunkSize( void ) const
	{
		return _u32Size;
	}


	MemPool_Impl::MemPool_Impl( UInt32 u32Size )
		: _Chunk( __max(u32Size, ListNode::GetHeadSize()), MEMCHUNK_SIZE)
	{
		if( _Chunk.is_valid() )
			_Free = _Chunk.Alloc();
	}

	MemPool_Impl::~MemPool_Impl( void )
	{
		Release();
	}

	void MemPool_Impl::Init( void )
	{
		_Chunk.Release();
		_Chunk.Init();
		_Free = _Chunk.Alloc();
	}

	void MemPool_Impl::Release( void )
	{
		_Free = NULL;
		_Chunk.Release();
	}

	void* MemPool_Impl::Alloc( void )
	{
		HNODE ptr = _Free;
		_Free = ListNode::GetNext(_Free);
		if( _Free == NULL )
		{
			_Free = _Chunk.Alloc();
			ListNode::SetNext(_Free, NULL);
		}
		return ptr;
	}

	void* MemPool_Impl::Calloc( void )
	{
		HNODE ptr = Alloc();
		memset((void*)ptr, 0, _Chunk.GetChunkSize());
		return ptr;
	}

	void MemPool_Impl::Free( void* ptr )
	{
		ListNode::SetNext(ptr, _Free);
		_Free = ptr;
	}

	void* MemPool_Impl::Clone( void* pBuf, UInt32 u32Size )
	{
		void* p = Alloc();
		if( p != NULL )
			memcpy(p, pBuf, u32Size);
		return p;
	}

	const UInt32	MemPool_Impl::GetLength( void ) const
	{
		return _Chunk.GetLength();
	}

	/*bool MemPool_Impl::is_valid( void ) const
	{
		return _Chunk.is_valid();
	}*/

}// namespace _pool_

