
#ifndef __LINKLIST_IMPL_20150721173059_1437471059_H__
#define __LINKLIST_IMPL_20150721173059_1437471059_H__

#include <libMempool/MemPool.h>

namespace	_container_{

	/*!
	* @class  LinkList
	* @brief  LinkList
	* @{
	*/

	class LinkList_Impl
	{
	public:
		struct tagListNode
		{
			tagListNode* pNext;
			__M_ALIGNED_PACKED__ char Value[1];
		};

		static void* value( void* ptr );

		static void* next( void* ptr );

	public:
		/*!
		* @function   LinkList
		* @brief    
		* @return  
		*/
		LinkList_Impl( int iSize );

		void* push_back( void );

		void	pop( void );

		const UInt32 size( void ) const ;

		const bool empty( void ) const;

		void* operator[]( UInt32 Pos );

		void* begin( void ) const;

		void* end( void ) const;

		template<typename T>
		inline void* forward( void* pSrc, void* pDest )
		{
			tagListNode* pS = (tagListNode*)pSrc;
			tagListNode* pD = (tagListNode*)pDest;
			_u32Size --;
			T t1 = *(reinterpret_cast<T*>(pS->Value));
			reinterpret_cast<T*>(pS->Value)->~T();
			if( _u32Size != 0 )
			{
				//memcpy(pS, pD, sizeof(tagListNode) + sizeof(T));
				memcpy(pS, pD, _Pool.GetLength());
				_Pool.Free(pD);
				if( pD == _End )
					_End = pS;
				return pSrc;
			}
		
			_Pool.Free(pS);
			_Head = pD;
			return _Head;		
		}

		inline void*	front( void )
		{
			return _Head->Value;
		}

	public:
		UInt32			_u32Size;
		tagListNode*	_Head;
		tagListNode*	_End;
		MemPool_macro<0, CNoneLock>	_Pool;
	};
	/** @} end LinkList */

} //_container_

#endif//__LINKLIST_20150721173059_1437471059_H__
