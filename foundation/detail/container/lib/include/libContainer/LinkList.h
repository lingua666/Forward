
#ifndef __LINKLIST_20150721173059_1437471059_H__
#define __LINKLIST_20150721173059_1437471059_H__

#include <libContainer/LinkList_Impl.h>
#include <libLock/Lock.hpp>

namespace	_container_{

	template<typename T>
	struct ListObj
	{
		static T RetNull( void )
		{ return T(); }
	};

	template<typename T>
	struct ListObj<T*>
	{
		static T* RetNull( void )
		{ return NULL; }
	};

	namespace _linklist_{

		template<typename T>
		class iterator
		{
		public:
			typedef iterator _Myiter;

		public:
			iterator( void )
				: _pValue( NULL )
			{

			}

			iterator( const _Myiter& t1 )
				: _pValue( t1._pValue )
			{

			}

			iterator( void* pValue )
				: _pValue( pValue )
			{

			}

			iterator& operator=( const _Myiter& _Right )
			{
				this->_pValue = _Right._pValue;
				return *this;
			}

			bool operator!=(const _Myiter& _Right) const
			{	// test for iterator inequality
				return (!(*this == _Right));
			}

			bool operator==( const _Myiter& _Right ) const
			{
				return this->_pValue == NULL || this->_pValue == _Right._pValue;
			}

			const T& operator*() const
			{	// return designated object
				return *((T*)LinkList_Impl::value(_pValue));
			}

			_Myiter& operator++()
			{	// preincrement
				if( LinkList_Impl::next(_pValue) != NULL )
					_pValue = LinkList_Impl::next(_pValue);
				else
					_pValue = NULL;

				return (*this);
			}

			_Myiter operator++(int) 
			{  
				_Myiter _Tmp = *this;
				++ (*this);  // 使用已定义的 prefix operator++ ，避免逻辑重复
				return (_Tmp);
			}

			operator bool ()
			{
				return !this->end();
			}

			bool operator!()
			{ 
				return this->end();
			}

			operator bool () const
			{
				return !this->end();
			}

			bool operator!() const
			{ 
				return this->end();
			}

			void* get( void ) const
			{
				return _pValue;
			}

		protected:
			bool end( void )
			{
				return _pValue == NULL || LinkList_Impl::next(_pValue) == NULL ? true : false;
			}

		private:
			void*	_pValue;
		};

	} //_linklist_

	/*!
	* @class  LinkList
	* @brief  LinkList
	* @{
	*/

	template<typename T, typename TLock = CLock>
	class LinkList
	{
	public:
		typedef	T					value_type;
		typedef	value_type&			reference;
		typedef const value_type&	const_reference;
		typedef	UInt32				size_type;
		typedef typename _linklist_::iterator<T>		iterator;

	public:
		/*!
		* @function   LinkList
		* @brief    
		* @return  
		*/
		LinkList( void )
			: _Impl( sizeof(value_type) )
		{

		}

		LinkList(const LinkList& t1)
			: _Impl(sizeof(value_type))
		{
			for (LinkList::iterator iter = t1.begin(); iter != t1.end(); iter ++)
			{
				push_back(*iter);
			}
		}

		~LinkList( void )
		{
			clear();
		}

		LinkList& operator=(const LinkList& t1) 
		{
			for (LinkList::iterator iter = t1.begin(); iter != t1.end(); iter++)
			{
				push_back(*iter);
			}

			return *this;
		}

		void push_back( const_reference Value )
		{
			_Lock.Lock();
			new(_Impl.push_back()) value_type(Value);
			_Lock.UnLock();
		}

		reference	front( void )
		{
			_Lock.Lock();
			value_type* val = reinterpret_cast<value_type*>(_Impl.front());
			_Lock.UnLock();
			return *val;
		}

		void	pop( void )
		{
			value_type val;
			_Lock.Lock();
			if( _Impl.size() > 0 )
			{
				val = *reinterpret_cast<value_type*>(_Impl.front());
				reinterpret_cast<value_type*>(_Impl.front())->~value_type();
				_Impl.pop();
			}
			_Lock.UnLock();
		}

		value_type	pop_front( void )
		{
			_Lock.Lock();
			if( _Impl.size() > 0 )
			{
				value_type val = *reinterpret_cast<value_type*>(_Impl.front());
				reinterpret_cast<value_type*>(_Impl.front())->~value_type();
				_Impl.pop();
				_Lock.UnLock();
				return val;
			}
			_Lock.UnLock();
			return ListObj<value_type>::RetNull();
		}

		const_reference operator[]( UInt32 Pos )
		{	// subscript nonmutable sequence
			if( Pos > size() - 1 )
				return ListObj<value_type>::RetNull();

			_Lock.Lock();
			const_reference c_rf = *reinterpret_cast<value_type*>(_Impl[Pos]);
			_Lock.UnLock();
			return c_rf;
		}

		iterator begin( void ) const
		{
			if( size() <= 0 )
				return iterator();
			return iterator(_Impl.begin());
		}

		iterator end( void ) const
		{
			return iterator(_Impl.end());
		}

		void clear( void )
		{
			while( size() > 0 )
			{
				pop();
			};
		}

		iterator erase( iterator _First_arg )
		{
			if( !_First_arg )
				return iterator();

			void* pCur = (void*)(_First_arg.get());
			void* pNext = (void*)((++ _First_arg).get());
			return iterator(_Impl.template forward<value_type>(pCur, pNext));
		}

		iterator erase( int iIndex )
		{
			int iPos = -1;
			iterator iter;
			iterator iter_next;
			for( iter = this->begin(); iter != this->end(); iter ++ )
			{
				iPos ++;
				if( iPos == iIndex )
				{
					iter_next = erase(iter);
					break;
				}
			}

			return iter_next;
		}

		const size_type size( void ) const 
		{
			return _Impl.size();
		}

		const bool empty( void ) const
		{
			return _Impl.empty();
		}

	public:
		LinkList_Impl	_Impl;
		TLock			_Lock;
	};
	/** @} end LinkList */

} //_container_

#define	LinkList_type	_container_::LinkList

#endif//__LINKLIST_20150721173059_1437471059_H__
