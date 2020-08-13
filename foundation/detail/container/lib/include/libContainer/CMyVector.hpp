
#ifndef __CMYVECTOR_20150806193305_1438860785_H__
#define __CMYVECTOR_20150806193305_1438860785_H__


/*优点：相当与数组，从后面快速的插入与删除，直接访问任何元素
** 缺点：序列开始或中间插入，删除元素要慢，
**       而且如果一开始分配的空间不够的话，有一个重新分配更大空间，然后拷贝的性能开销.*/
#include <libLock/Lock.hpp>
#include <exception>
#include <vector>

#if !defined INLINE
#define		INLINE		inline
#endif

namespace	_container_{

	template <typename T>
	class CMyVector
	{
	public:
		typedef	T	_imp;
		typedef typename std::vector<T>							tnMyContainer;
		typedef typename std::vector<T>::size_type				size_type;
		typedef typename std::vector<T>::iterator			    iterator;
		typedef typename std::vector<T>::const_iterator			const_iterator;
		typedef typename std::vector<T>::reference			    reference;
		typedef typename std::vector<T>::const_reference	    const_reference;

	public:
		INLINE static CMyVector<T>* Create( void )
		{
			return new CMyVector<T>();
		}

		INLINE static void Delete( CMyVector<T>* lpCMyContainBase )
		{
			IS_INVALID_NONE(lpCMyContainBase);
			delete lpCMyContainBase;
			lpCMyContainBase = NULL;
		}

	public:
		CMyVector( void ) {  };
		~CMyVector( void ) 
		{ 
			//clear();
			tnMyContainer().swap(m_MyContainer);
		}

		CMyVector& operator=(const CMyVector& _Right)
		{
			m_MyContainer = _Right.m_MyContainer;
			return *this;
		}

		INLINE bool at( size_type _Pos, T& Type )
		{
			bool bRet = true;
			LOCK(m_Lock)
			try
			{
				Type = m_MyContainer.at(_Pos); // 越界抛出异常
			}
			catch(const std::exception& e)
			{//越界处理
				bRet = false;
			}
			UNLOCK(m_Lock)
			return bRet;
		}

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

		INLINE iterator erase( T Type )
		{
			iterator iter;
			LOCK(m_Lock)
			iter = m_MyContainer.erase(remove(m_MyContainer.begin(),m_MyContainer.end(),Type),m_MyContainer.end()); //value是要删除的值
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

		INLINE void resize(size_type _Newsize)
		{
			LOCK(m_Lock)
			m_MyContainer.resize(_Newsize);
			UNLOCK(m_Lock)
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

		INLINE T back( void )
		{
			T Type = NULL;
			LOCK(m_Lock)
			if(!m_MyContainer.empty())
				Type = m_MyContainer.back();
			UNLOCK(m_Lock)
			return Type;
		}

		INLINE void	 push_back( const T& _Val )
		{
			LOCK(m_Lock)
			m_MyContainer.push_back(_Val);
			UNLOCK(m_Lock)
		}

		INLINE void pop_back( void )
		{
			LOCK(m_Lock)
			if(!m_MyContainer.empty())
				m_MyContainer.pop_back();
			UNLOCK(m_Lock)
		}

		INLINE const_reference operator[](size_type _Pos)
		{	// subscript nonmutable sequence
			LOCK(m_Lock)
			const_reference c_rf = m_MyContainer[_Pos];
			UNLOCK(m_Lock)
			return c_rf;
		}

	private:
		tnMyContainer      m_MyContainer;
		CLock			   m_Lock;
	};	

} //_container_


#endif