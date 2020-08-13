
#ifndef __CYCLELIST_IMPL_20160502205400_1462193640_H__
#define __CYCLELIST_IMPL_20160502205400_1462193640_H__

#include <libMempool/MemPool.h>

namespace	_container_{

	/*!
	* @class  CycleList
	* @brief   CycleList
	* @{
	*/

	class CycleList_Impl
	{
	public:
		struct tagListNode
		{
			tagListNode* pPrev;
			tagListNode* pNext;
			__M_ALIGNED_PACKED__ char Value[1];
		};

	public:
		/*!
		* @function   CycleList
		* @brief    
		* @return  
		*/
		CycleList_Impl( int iSize );

		void*	push_back( void );

		void pop( void );

		int	next( void );

		const UInt32 size( void ) const ;

		const bool empty( void ) const ;

		inline void*	current( void )
		{
			return _Current->Value;
		}

	protected:
		void pop_front( void );

	private:
		UInt32			_u32Size;
		tagListNode*	_Head;
		tagListNode*	_End;
		tagListNode*	_Current;
		MemPool_macro<0, CNoneLock>	_Pool;
	};
	/** @} end CycleList */

} ; //_container_

#endif//__CYCLELIST_20160502205400_1462193640_H__
