
#ifndef __QUEUE_20150928214048_1443447648_H__
#define __QUEUE_20150928214048_1443447648_H__

#include <libContainer/Queue_Impl.h>

/*!
* @class  Queue
* @brief   Queue
* @{
*/

namespace	_container_ {

	template<typename T>
	struct QueueObj
	{
		static T RetNull( void )
		{ return T(); }
	};

	template<typename T>
	struct QueueObj<T*>
	{
		static T* RetNull( void )
		{ return NULL; }
	};

	namespace _queue_{

		template<typename T>
		class iterator
		{
		public:
			typedef iterator _Myiter;

		public:
			iterator( void )
			{
				this->_Info._pValue = NULL;
				this->_Info._u16ChunkSize = 0;
				this->_Info._u16CurPos = 0;
				this->_Info._u16ValueSize = 0;
				this->_iSize = 0;
			}

			iterator( const _Myiter& t1 )
			{
				this->_Info._pValue = t1._Info._pValue;
				this->_Info._u16ChunkSize = t1._Info._u16ChunkSize;
				this->_Info._u16CurPos = t1._Info._u16CurPos;
				this->_Info._u16ValueSize = t1._Info._u16ValueSize;
				this->_iSize = t1._iSize;
			}

			iterator( tagQueueInfo* pInfo, UInt32 uSize )
			{
				this->_Info._pValue = pInfo->_pValue;
				this->_Info._u16ChunkSize = pInfo->_u16ChunkSize;
				this->_Info._u16CurPos = pInfo->_u16CurPos;
				this->_Info._u16ValueSize = pInfo->_u16ValueSize;
				this->_iSize = uSize;
			}

			iterator& operator=( const _Myiter& _Right )
			{
				this->_Info._pValue = _Right._Info._pValue;
				this->_Info._u16CurPos = _Right._Info._u16CurPos;
				this->_Info._u16ChunkSize = _Right._Info._u16ChunkSize;
				this->_Info._u16ValueSize = _Right._Info._u16ValueSize;
				this->_iSize = _Right._iSize;
				return *this;
			}

			bool operator!=(const _Myiter& _Right) const
			{	// test for iterator inequality
				return (!(*this == _Right));
			}

			bool operator==( const _Myiter& _Right ) const
			{
				if(this->_Info._pValue == _Right._Info._pValue &&
					this->_Info._u16CurPos == _Right._Info._u16CurPos )
					return true;

				return false;
			}

			const T& operator*() const
			{	// return designated object
				return *((T*)Queue_Impl::value(&_Info));
			}

			_Myiter& operator++()
			{	// preincrement
				if( this->_iSize > 0 )
				{
					this->_iSize --;
					Queue_Impl::next(&_Info);
				}

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

		protected:
			bool end( void )
			{
				return this->_iSize <= 0 ? true : false;
			}

		private:
			tagQueueInfo	_Info;
			int				_iSize;				
		};

	} //_queue_

	template<typename T, typename TLock = CLock, typename Allocator =  MemPool_type>
	class Queue
	{
	public:
		typedef	T						value_type;
		typedef	UInt32					size_type;
		typedef	value_type&				reference;
		typedef const value_type&		const_reference;
		typedef _queue_::iterator<T>	iterator;

	public:
		/*!
		* @function   Queue
		* @brief    
		* @return  
		*/
		Queue( void )
			: _Impl( sizeof(value_type) )
		{
			
		}

		/*!
		* @function   ~Queue
		* @brief    
		* @return  
		*/
		~Queue( void )
		{
			clear();
		}

		void	push_back( const_reference value )
		{
			_Lock.Lock();
			new(_Impl.push_back())	value_type(value);
			_Lock.UnLock();
		}

		reference front( void )
		{
			_Lock.Lock();
			value_type* val = reinterpret_cast<value_type*>(_Impl.front());
			_Lock.UnLock();
			return *val;
		}

		void	pop( void )
		{
			pop_front();
		}

		void	pop_front( void )
		{
			value_type val;
			_Lock.Lock();
			if( _Impl.size() > 0 )
			{
				val = *(reinterpret_cast<value_type*>(_Impl.front()));
				reinterpret_cast<value_type*>(_Impl.front())->~value_type();
				_Impl.pop_front();
			}
			_Lock.UnLock();
		}

		const_reference operator[]( UInt32 _Pos )
		{	// subscript nonmutable sequence
			if( _Pos > size() - 1 )
				return QueueObj<value_type>::RetNull();

			_Lock.Lock();
			const_reference c_rf = *reinterpret_cast<value_type*>(_Impl[_Pos]);
			_Lock.UnLock();
			return c_rf;
		}

		iterator begin( void )
		{
			if( size() <= 0 )
				return iterator();

			tagQueueInfo Info = {0};
			_Impl.begin(&Info);
			return iterator(&Info, _Impl.size());
		}

		iterator end( void )
		{
			if( size() <= 0 )
				return iterator();

			tagQueueInfo Info = {0};
			_Impl.end(&Info);
			return iterator(&Info, _Impl.size());
		}

		size_type	size( void ) const
		{
			return _Impl.size();
		}

		bool	empty( void ) const
		{
			return _Impl.empty();
		}
		
		void	clear( void )
		{
			while( size() > 0 )
			{
				pop_front();
			};
		}
		
	private:
		TLock		_Lock;
		Queue_Impl	_Impl;
	};
	/** @} end Queue */
} //namespace	_foundation_ 

#define	Queue_type	_container_::Queue

#endif//__QUEUE_20150928214048_1443447648_H__
