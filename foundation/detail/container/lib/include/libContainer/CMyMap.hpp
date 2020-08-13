
#ifndef __CMYMAP_20150806193305_1438860785_H__
#define __CMYMAP_20150806193305_1438860785_H__

#include <libLock/Lock.hpp>
#include <map>

#if !defined INLINE
#define		INLINE		inline
#endif

namespace	_container_{

	/* 优点： 字典库，一个值映射成另一个值，
	**        使用平衡二叉树（红黑树）存储，按照给定的排序规则对map中的key值进行排序；
	**		  查找效率效率高。
	**缺点： 插入和删除操作比较慢，不允许重复值*/


	template <typename K, typename T>
	class CMyMap
	{
	public:
		typedef typename std::map<K,T>					tnMyContainer;
		typedef typename std::map<K,T>::size_type		size_type;
		typedef typename std::map<K,T>::iterator		iterator;
		typedef typename std::map<K,T>::const_iterator	const_iterator;
		typedef typename std::map<K,T>::value_type      value_type;
		typedef typename std::map<K,T>::key_type        key_type;

	public:
		INLINE static CMyMap<K,T>* Create( void )
		{
			return new CMyMap<K,T>();
		}

		INLINE static void Delete( CMyMap<K,T>* lpCMyContainBase )
		{
			if(lpCMyContainBase != NULL)
			{
				delete lpCMyContainBase;
				lpCMyContainBase = NULL;
			}
		}

	public:
		CMyMap( void ) { };
		~CMyMap( void ) { clear(); }

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

		INLINE void erase( iterator _First_arg )
		{
			LOCK(m_Lock)
			m_MyContainer.erase(_First_arg);
			UNLOCK(m_Lock)
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
				if(iter->second != NULL)
				{
				delete iter->second;
				iter->second = NULL;
				}
				}*/
			m_MyContainer.clear();
			UNLOCK(m_Lock)
		}

		INLINE std::pair<iterator,bool> insert1( const value_type& value )
		{
			std::pair<iterator,bool> sp;
			LOCK(m_Lock)
			sp = m_MyContainer.insert(value);
			UNLOCK(m_Lock)
			return sp;
		}

		INLINE	std::pair<iterator,bool> insert2( const value_type& value )
		{
			std::pair<iterator,bool> sp;
			if(!IsExistKey1(value))
			{
				LOCK(m_Lock)
				sp = m_MyContainer.insert(value);
				UNLOCK(m_Lock)
			}
			return sp;
		}

		INLINE T	front( void )
		{
			T Type = NULL;
			LOCK(m_Lock)
			if(!m_MyContainer.empty())
				Type = m_MyContainer.begin()->second;
			UNLOCK(m_Lock)
			return Type;
		}

		INLINE iterator find( const key_type& key )
		{
			iterator iter;
			LOCK(m_Lock)
			iter = m_MyContainer.find(key);
			UNLOCK(m_Lock)
			return iter;
		}

		INLINE bool eraseKey( const key_type& key )
		{
			iterator iter;
			LOCK(m_Lock)
			iter = m_MyContainer.find(key);
			if(iter != m_MyContainer.end())
			{
				m_MyContainer.erase(iter);
				UNLOCK(m_Lock)
				return true;
			}
			UNLOCK(m_Lock)
			return false;
		}

		INLINE bool IsExistKey1( const key_type& key )
		{
			LOCK(m_Lock)
			if(m_MyContainer.find(key) != m_MyContainer.end())
			{
				UNLOCK(m_Lock)
				return true;
			}
			UNLOCK(m_Lock)
			return false;
		}

		INLINE  bool IsExistKey2( const key_type& key, T& Value )
		{
			iterator iter;
			LOCK(m_Lock)
			iter = m_MyContainer.find(key);
			if(iter != m_MyContainer.end())
			{
				Value = iter->second;
				UNLOCK(m_Lock)
				return true;
			}

			UNLOCK(m_Lock)
			return false;
		}

	private:
		tnMyContainer m_MyContainer;
		CLock		  m_Lock;
	};

} //_container_

#define	Container_Map_type	std::map

#endif