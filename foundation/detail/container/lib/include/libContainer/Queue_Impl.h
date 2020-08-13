
#ifndef __QUEUE_IMPL_20150928214048_1443447648_H__
#define __QUEUE_IMPL_20150928214048_1443447648_H__

#include <libMempool/MemPool.h>

#if !defined INLINE
	#define		INLINE		inline
#endif

#define	QUEQUE_CHUNK_NUMBER		16

/*!
* @class  Queue
* @brief   Queue
* @{
*/

namespace	_container_ {

#pragma pack(push, 1)
	struct tagQueueInfo
	{
		void*	_pValue;
		UInt16	_u16ValueSize;
		UInt16	_u16CurPos;
		UInt16	_u16ChunkSize;
	};
#pragma pack(pop)

	class Queue_Impl
	{
	public:
		struct stChunk
		{
			stChunk*	_pNext;
			__M_ALIGNED_PACKED__ char		_pBuf[1];
		};

		static void* value( const tagQueueInfo* ptr );

		static tagQueueInfo* next( tagQueueInfo* ptr );

	public:
		/*!
		* @function   Queue
		* @brief    
		* @return  
		*/
		Queue_Impl( int iSize );

		void*	push_back( void );

		void	pop_front( void );

		UInt32	size( void ) const;

		bool	empty( void ) const;

		void* operator[]( UInt32 Pos );

		inline void* front( void )
		{
			return &_pHeadChunk->_pBuf[_u16CurPos];
		}

		void begin( tagQueueInfo* pInfo );

		void end( tagQueueInfo* pInfo );
		
	protected:
		Queue_Impl::stChunk*	CreateChunk( void );

	private:
		stChunk*	_pHeadChunk;
		stChunk*	_pCurChunk;
		UInt32		_u32Size;
		UInt16		_u16Remain;
		UInt16		_u16CurPos;
		const UInt16	_u16ChunkSize;
		const UInt16	_u16ValueSize;
		MemPool_macro<0, CNoneLock>	_Pool;
	};
	/** @} end Queue */
} //namespace	_foundation_ 

#endif//__QUEUE_20150928214048_1443447648_H__
