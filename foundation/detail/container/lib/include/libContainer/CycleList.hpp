
#ifndef __CYCLELIST_20160502205400_1462193640_H__
#define __CYCLELIST_20160502205400_1462193640_H__

#include <libContainer/CycleList_Impl.h>
#include <libLock/Lock.hpp>

namespace	_container_{

	/*!
	* @class  CycleList
	* @brief   CycleList
	* @{
	*/

	template<typename T, typename TLock = CLock>
	class CycleList
	{
	public:
		typedef	T					value_type;
		typedef	value_type&			reference;
		typedef const value_type&	const_reference;
		typedef	UInt32				size_type;

	public:
		/*!
		* @function   CycleList
		* @brief    
		* @return  
		*/
		CycleList( void )
			: _Impl( sizeof(value_type) )
		{
			
		}

		/*!
		* @function   ~CycleList
		* @brief    
		* @return  
		*/
		~CycleList( void )
		{
			clear(); 
		}

		void push_back( const_reference Value )
		{
			_Lock.Lock();
			new(_Impl.push_back()) value_type(Value);
			_Lock.UnLock();
		}

		void pop( void )
		{
			value_type val;
			_Lock.Lock();
			if( _Impl.size() > 0 )
			{
				val = *(reinterpret_cast<value_type*>(_Impl.current()));
				reinterpret_cast<value_type*>(_Impl.current())->~value_type();
				_Impl.pop();
			}
			_Lock.UnLock();
		}

		int	next( void )
		{
			_Lock.Lock();
			int iResult = _Impl.next();
			_Lock.UnLock();

			return iResult;
		}

		reference	current( void )
		{
			_Lock.Lock();
			value_type* val = reinterpret_cast<value_type*>(_Impl.current());
			_Lock.UnLock();
			return *val;
		}

		void clear( void )
		{
			while( size() > 0 )
			{
				pop();
			};	
		}

		const size_type size( void ) const 
		{
			return _Impl.size();
		}

		const bool empty( void ) const
		{
			return _Impl.empty();
		}

	private:
		CycleList_Impl	_Impl;
		TLock			_Lock;
	};
	/** @} end CycleList */

} ; //_container_

#endif//__CYCLELIST_20160502205400_1462193640_H__
