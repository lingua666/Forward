
#ifndef __POOL_IMPL_20150725121654_1437797814_H__
#define __POOL_IMPL_20150725121654_1437797814_H__

#include <libCommon/modafx.h>

namespace _pool_{

	typedef	void*	HNODE;

	class MemChunk
	{
	public:
		friend class MemPool_Impl;

	protected:
		MemChunk( const UInt32 u32Size, const UInt16 u16Num );

		HNODE	Alloc( void );

		void	Init( void );

		void	Release( void );

		UInt32	GetLength( void ) const;

		inline bool	is_valid( void ) const
		{
			return _u32Size > 0;
		}

		HNODE	Reallocate( void );

		const UInt32	GetHeadSize( const UInt32 u32Size ) const;

		const UInt32	GetChunkSize( void ) const;

	private:
		const UInt16	_u16Num;
		const UInt32	_u32Size;
		UInt32	_u32Free;
		HNODE	_NodeList;
	};
	
	class MemPool_Impl
	{
	public:
		typedef MemPool_Impl	self_type;

	public:
		MemPool_Impl( UInt32 u32Size );

		~MemPool_Impl( void );

		void Init( void );

		void Release( void );

		void* Alloc( void );

		void* Calloc( void );

		void Free( void* ptr );

		void* Clone( void* pBuf, UInt32 u32Size );

		const UInt32	GetLength( void ) const;

		inline bool	is_valid( void ) const
		{
			return _Chunk.is_valid();
		}

	private:
		MemChunk	_Chunk;
		HNODE		_Free;
	};
}// namespace _pool_


#endif//__POOL_IMPL_20150725121654_1437797814_H__
