
#ifndef __QUEUE_V2_20150928214048_1443447648_H__
#define __QUEUE_V2_20150928214048_1443447648_H__

#include <libContainer/QueueV2_Impl.h>

/*!
* @class  Queue
* @brief   Queue
* @{
*/

namespace	_container_ {

	template<typename T>
	struct QueueV2Obj
	{
		static T RetNull( void )
		{ return T(); }
	};

	template<typename T>
	struct QueueV2Obj<T*>
	{
		static T* RetNull( void )
		{ return NULL; }
	};

	namespace _queue_v2_{

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

			iterator( tagQueueV2Info* pInfo, UInt32 uSize )
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
				return *((T*)QueueV2_Impl::value(&_Info));
			}

			_Myiter& operator++()
			{	// preincrement
				if( this->_iSize > 0 )
				{
					this->_iSize --;
					QueueV2_Impl::next(&_Info);
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
			tagQueueV2Info	_Info;
			int				_iSize;				
		};

	} //_queue_

	template<typename T, typename P, typename TLock = CLock, typename Allocator =  MemPool_type>
	class QueueV2
	{
	public:
		typedef	T						first_type;
		typedef P						second_type;
		typedef	UInt32					size_type;
		typedef	first_type&				f_reference;
		typedef	second_type&			s_reference;
		typedef const first_type&		const_f_reference;
		typedef const second_type&		const_s_reference;
		typedef _queue_v2_::iterator<T>	iterator;

	public:
		/*!
		* @function   Queue
		* @brief    
		* @return  
		*/
		QueueV2( void )
			: _Impl( sizeof(first_type) + sizeof(second_type) )
		{
			
		}

		/*!
		* @function   ~Queue
		* @brief    
		* @return  
		*/
		~QueueV2( void )
		{
			clear();
		}

		void	push_back( const_f_reference first, const_s_reference second )
		{
			_Lock.Lock();
			char* pBuf = (char*)_Impl.push_back();
			new((void*)pBuf)	first_type(first);
			new((void*)&pBuf[sizeof(first_type)])	second_type(second);
			_Lock.UnLock();
		}

		f_reference front_f( void )
		{
			_Lock.Lock();
			first_type* val = reinterpret_cast<first_type*>(_Impl.front());
			_Lock.UnLock();
			return *val;
		}

		s_reference front_s( void )
		{
			_Lock.Lock();
			char* pBuf = (char*)_Impl.front();
			second_type* val = reinterpret_cast<second_type*>(&pBuf[sizeof(first_type)]);
			_Lock.UnLock();
			return *val;
		}

		void	pop( void )
		{
			pop_front();
		}

		void	pop_front( void )
		{
			first_type f;
			second_type s;

			_Lock.Lock();
			if( _Impl.size() > 0 )
			{
				char* pBuf = (char*)_Impl.front();
				f = *(reinterpret_cast<first_type*>(pBuf));
				s = *(reinterpret_cast<second_type*>(&pBuf[sizeof(first_type)]));
				reinterpret_cast<first_type*>(pBuf)->~first_type();
				reinterpret_cast<second_type*>(&pBuf[sizeof(first_type)])->~second_type();
				_Impl.pop_front();
			}
			_Lock.UnLock();
		}

		const_f_reference at_f( UInt32 _Pos )
		{	// subscript nonmutable sequence
			if( _Pos > size() - 1 )
				return QueueV2Obj<first_type>::RetNull();

			_Lock.Lock();
			const_f_reference c_rf = *reinterpret_cast<first_type*>(_Impl[_Pos]);
			_Lock.UnLock();
			return c_rf;
		}

		const_s_reference at_s( UInt32 _Pos )
		{	// subscript nonmutable sequence
			if( _Pos > size() - 1 )
				return QueueV2Obj<second_type>::RetNull();

			_Lock.Lock();
			char* pBuf = (char*)_Impl[_Pos];
			const_s_reference c_rf = *reinterpret_cast<second_type*>(&pBuf[sizeof(first_type)]);
			_Lock.UnLock();
			return c_rf;
		}

		iterator begin( void )
		{
			if( size() <= 0 )
				return iterator();

			tagQueueV2Info Info = {0};
			_Impl.begin(&Info);
			return iterator(&Info, _Impl.size());
		}

		iterator end( void )
		{
			if( size() <= 0 )
				return iterator();

			tagQueueV2Info Info = {0};
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
		TLock			_Lock;
		QueueV2_Impl	_Impl;
	};
	/** @} end Queue */
} //namespace	_foundation_ 

#define	Queue_type	_container_::Queue

#endif//__QUEUE_20150928214048_1443447648_H__
