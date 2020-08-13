
#ifndef __POOL_20150725121654_1437797814_H__
#define __POOL_20150725121654_1437797814_H__

#include <libMempool/Pool_Impl.h>
#include <libLock/Lock.hpp>


namespace _pool_{

	template<UInt32 N = 0, typename TLock = CLock>
	class MemPool
	{
	public:
		typedef MemPool	self_type;

		template<typename T>
		static void FreeMemObj( self_type* pObj, void* ptr )
		{
			pObj->FreeObj(reinterpret_cast<T*>(ptr));
		}
	public:
		MemPool( void )
			: _Impl(N)
		{
			
		}

		MemPool( UInt32 u32Size )
			: _Impl(u32Size)
		{

		}

		inline void Init( void )
		{
			_Impl.Release();
			_Impl.Init();
		}

		inline void Release( void )
		{
			_Impl.Release();
		}

		inline void* Alloc( void )
		{
			if( !_Impl.is_valid() )
				return NULL;

			_Lock.Lock();
			void* ptr = _Impl.Alloc();
			_Lock.UnLock();
			return ptr;
		}

		inline void* Calloc( void )
		{
			if( !_Impl.is_valid() )
				return NULL;

			_Lock.Lock();
			void* ptr = _Impl.Calloc();
			_Lock.UnLock();
			return ptr;
		}

		inline void Free( void* ptr )
		{
			IS_INVALID_NONE(ptr);

			_Lock.Lock();
			_Impl.Free(ptr);
			_Lock.UnLock();
		}

		inline void* Clone( void* pBuf, UInt32 u32Size )
		{
			IS_INVALID_ZERO(pBuf);

			_Lock.Lock();
			void* p = _Impl.Clone(pBuf, u32Size);
			_Lock.UnLock();
			return p;
		}
		
				//分配对象
		template<typename T>
		inline T*	AllocObj( void )
		{
			void* vBuf = Alloc();
			if(vBuf != NULL)
			{
				T* pt = new(vBuf) T();
				return pt;
			}
			return NULL;
		}

		template<typename T, typename P1>
		inline T*	AllocObj( P1 p1 )
		{
			void* vBuf = Alloc();
			if(vBuf != NULL)
			{
				T* pt = new(vBuf) T(p1);
				return pt;
			}
			return NULL;
		}

		template<typename T, typename P1, typename P2>
		inline T*	AllocObj( P1 p1, P2 p2 )
		{
			void* vBuf = Alloc();
			if(vBuf != NULL)
			{
				T* pt = new(vBuf) T(p1, p2);
				return pt;
			}
			return NULL;
		}

		template<typename T, typename P1, typename P2, typename P3>
		inline T*	AllocObj( P1 p1, P2 p2, P3 p3 )
		{
			void* vBuf = Alloc();
			if(vBuf != NULL)
			{
				T* pt = new(vBuf) T(p1, p2, p3);
				return pt;
			}
			return NULL;
		}

		//释放对象
		template<typename T>
		inline void	FreeObj( T* ptr )
		{
			IS_INVALID_NONE(ptr);
			ptr->~T(); 
			Free((void*)(ptr));
		}

		template<typename T>
		inline void	FreeObj_v( void* ptr )
		{
			IS_INVALID_NONE(ptr);
			((T*)ptr)->~T(); 
			Free((void*)(ptr));
		}

		inline const UInt32	GetLength( void ) const
		{
			return _Impl.GetLength();
		}

	private:
		MemPool_Impl	_Impl;
		TLock			_Lock;
	};
}// namespace _pool_


#endif//__POOL_20150725121654_1437797814_H__
