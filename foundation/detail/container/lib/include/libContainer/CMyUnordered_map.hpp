
#ifndef __CMYUNORDERED_20150806193305_1438860785_H__
#define __CMYUNORDERED_20150806193305_1438860785_H__

#include "hash/Fnv_hash.h"
#include "CMyMap.hpp"
#include <libLock/Lock.hpp>

#if !defined INLINE
#define		INLINE		inline
#endif

#if defined(_SUPPORTED_CXX11_)

	//unordered_map的版本兼容性问题
	#if defined(PLATFORM_OS_FAMILY_UNIX)
		#include <tr1/unordered_map>
		//#include <tr1/unordered_set>
		//using std::tr1::unordered_map;
		//using std::tr1::unordered_set;

		#define	unordered_map_tp std::tr1::unordered_map

	#elif defined(PLATFORM_OS_FAMILY_WINDOWS)
		#include <unordered_map>
		//using std::unordered_map;
		#define	unordered_map_tp std::unordered_map
	#endif

	namespace	_container_{

		namespace zl
		{ //{{{
			struct equal_charptr
			{
				bool operator()(const char* s1, const char* s2) const
				{
					return strcmp(s1, s2) == 0;
				}
			};

			struct equal_string
			{
				bool operator()(const _string_type& s1, const _string_type& s2) const
				{
					return (s1==s2);
				}
			};

			struct hash_string
				: public std::unary_function<_string_type, std::size_t>
			{
				std::size_t operator()(const _string_type& __s) const
				{ return _hash_::Fnv_hash<>::hash(__s.data(), __s.length()); }
			};


			struct hash_charptr
				: public std::unary_function<const char*, std::size_t>
			{
				std::size_t operator()(const char* __s) const
				{ return _hash_::Fnv_hash<>::hash(__s, strlen(__s)); }
			};
		}//}}}

		template<class _Key, class _Tp,
		class _Hash = std::hash<_Key>,
		class _Pred = std::equal_to<_Key>,
		class _Alloc = std::allocator<std::pair<const _Key, _Tp> > >
		class CMyunordered_map
		{
		public:
			typedef typename unordered_map_tp<_Key,_Tp,_Hash,_Pred,_Alloc>					self_type;
			typedef typename self_type::size_type		size_type;
			typedef typename self_type::iterator		iterator;
			typedef typename self_type::const_iterator	const_iterator;
			typedef typename self_type::value_type      value_type;
			typedef typename self_type::key_type        key_type;

		public:
			INLINE static self_type* Create( void )
			{
				return new self_type();
			}

			INLINE static void Delete( self_type* lpCMyContainBase )
			{
				if(lpCMyContainBase != NULL)
				{
					delete lpCMyContainBase;
					lpCMyContainBase = NULL;
				}
			}

		public:
			CMyunordered_map( void ) { };
			~CMyunordered_map( void ) { clear(); }

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

			INLINE std::pair<iterator,bool> insert( const value_type& value )
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

			INLINE _Tp	front( void )
			{
				_Tp Type = NULL;
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

			INLINE  bool IsExistKey2( const key_type& key, _Tp& Value )
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
			self_type	 m_MyContainer;
			CLock		 m_Lock;
		};

	} //_container_ 
#else
	#define	unordered_map_tp std::map
#endif

#define	Container_HashMap_type	unordered_map_tp

#endif // __CUSTOM_APIPACKAGE_HEAD_DEF__
