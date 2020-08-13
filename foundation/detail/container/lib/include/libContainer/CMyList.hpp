
#ifndef __CMYLIST_20150806193305_1438860785_H__
#define __CMYLIST_20150806193305_1438860785_H__

#include <libLock/Lock.hpp>
#include <list>

#define MYLIST_FUNLIST  void*

#if !defined INLINE
#define		INLINE		inline
#endif

namespace	_container_{

	/* 优点：双向链表，内存空间上可能是不连续的，
	** 无保留内存，堆中分配内存；在任何位置安插和删除元素速度都比较快
	** 缺点：不支持随机存取。*/

	template <typename T>
	class CMyList
	{
	public:
		typedef typename std::list<T>							tnMyContainer;
		typedef typename std::list<T>::size_type				size_type;
		typedef typename std::list<T>::iterator			       iterator;
		typedef typename std::list<T>::const_iterator          const_iterator;
		typedef typename std::list<T>::reference			    reference;
		typedef typename std::list<T>::const_reference	       const_reference;
	public:
		INLINE static CMyList<T>* Create( void )
		{
			return new CMyList<T>();
		}

		INLINE static void Delete( CMyList<T>* lpCMyContainBase )
		{
			IS_INVALID_NONE(lpCMyContainBase);
			delete lpCMyContainBase;
			lpCMyContainBase = NULL;
		}

	public:
		CMyList( void ) { };
		~CMyList( void ) { clear(); }

		INLINE iterator begin( void )
		{
			iterator iter;
			LOCK(m_Lock)
			iter = m_MyContainer.begin();
			UNLOCK(m_Lock)
			return iter;
		}

		INLINE iterator end( void )
		{
			iterator iter;
			LOCK(m_Lock)
			iter = m_MyContainer.end();
			UNLOCK(m_Lock)
			return iter;
		}

		INLINE iterator erase( iterator _First_arg )
		{
			iterator iter;
			LOCK(m_Lock)
			iter = m_MyContainer.erase(_First_arg);
			UNLOCK(m_Lock)
			return iter;
		}

		INLINE bool empty( void )
		{
			bool bl;
			LOCK(m_Lock)
			bl = m_MyContainer.empty();
			UNLOCK(m_Lock)
			return bl;
		}

		INLINE size_type size( void )
		{
			size_type st;
			LOCK(m_Lock)
			st = m_MyContainer.size();
			UNLOCK(m_Lock)
			return st;
		}

		INLINE void clear( void )
		{
			LOCK(m_Lock)
				/*iterator iter;
				for ( iter = m_MyContainer.begin() ; iter != m_MyContainer.end() ; iter++ )
				{
				if(*iter != NULL)
				{
				delete *iter;
				*iter = NULL;
				}
				}*/
			m_MyContainer.clear();
			UNLOCK(m_Lock)
		}

		INLINE T front( void )
		{
			T Type = NULL;
			LOCK(m_Lock)
			if(!m_MyContainer.empty())
				Type = m_MyContainer.front();
			UNLOCK(m_Lock)
			return Type;
		}

		INLINE T back( void )
		{
			T Type = NULL;
			LOCK(m_Lock)
			if(!m_MyContainer.empty())
				Type = m_MyContainer.back();
			UNLOCK(m_Lock)
			return Type;
		}

		INLINE void	 push_front( const T& _Val )
		{
			LOCK(m_Lock)
			m_MyContainer.push_front(_Val);
			UNLOCK(m_Lock)
		}

		INLINE void pop_front1( void )
		{
			LOCK(m_Lock)
			if(!m_MyContainer.empty())
				m_MyContainer.pop_front();
			UNLOCK(m_Lock)
		}

		INLINE T	pop_front2( void )
		{
			T Type = NULL;
			LOCK(m_Lock)
			if(!m_MyContainer.empty())
			{
				Type = m_MyContainer.front();
				m_MyContainer.pop_front();
			}
			UNLOCK(m_Lock)
			return Type;
		}

		INLINE void	 push_back(const T& _Val )
		{
			LOCK(m_Lock)
			m_MyContainer.push_back(_Val);
			UNLOCK(m_Lock)
		}

		INLINE void pop_back1( void )
		{
			LOCK(m_Lock)
			if(!m_MyContainer.empty())
				m_MyContainer.pop_back();
			UNLOCK(m_Lock)
		}

		INLINE T	pop_back2( void )
		{
			T Type = NULL;
			LOCK(m_Lock)
			if(!m_MyContainer.empty())
			{
				Type = m_MyContainer.back();
				m_MyContainer.pop_back();
			}
			UNLOCK(m_Lock)
			return Type;
		}

	private:
		tnMyContainer m_MyContainer;
		CLock		  m_Lock;
		//MYLIST_FUNLIST  m_MyFunList;

	};	

} //_container_



#endif