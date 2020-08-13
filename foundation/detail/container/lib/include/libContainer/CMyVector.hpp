
#ifndef __CMYVECTOR_20150806193305_1438860785_H__
#define __CMYVECTOR_20150806193305_1438860785_H__


/*�ŵ㣺�൱�����飬�Ӻ�����ٵĲ�����ɾ����ֱ�ӷ����κ�Ԫ��
** ȱ�㣺���п�ʼ���м���룬ɾ��Ԫ��Ҫ����
**       �������һ��ʼ����Ŀռ䲻���Ļ�����һ�����·������ռ䣬Ȼ�󿽱������ܿ���.*/
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
				Type = m_MyContainer.at(_Pos); // Խ���׳��쳣
			}
			catch(const std::exception& e)
			{//Խ�紦��
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
			iter = m_MyContainer.erase(remove(m_MyContainer.begin(),m_MyContainer.end(),Type),m_MyContainer.end()); //value��Ҫɾ����ֵ
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