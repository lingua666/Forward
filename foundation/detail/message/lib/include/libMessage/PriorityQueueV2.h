
#ifndef __PRIORITYQUEUE_V2_20150803173634_1438594594_H__
#define __PRIORITYQUEUE_V2_20150803173634_1438594594_H__

#include <libContainer/QueueV2.hpp>
#include <libLock/Lock.hpp>

//优先级别允许设置最高为7级   数值越大，优先级别越大
#define	MESSAGE_V2_PRIORITY_NUM	5

namespace _priority_queue_v2_{

	template<typename T>
	class iterator
	{
	public:
		typedef iterator _Myiter;
		typedef _container_::_queue_::iterator<T>	_Exiter;
		typedef function20_handle<void (Int8*, _Exiter*)>	HFNGetNext;

	public:
		iterator( void )
		{
			_CurPriority = -1;
		}

		iterator( const _Myiter& t1 )
		{
			this->_CurPriority = t1._CurPriority;
			this->_iter = t1._iter;
			this->_hGetNext = t1._hGetNext;
		}

		iterator( UInt8 Priority, const _Exiter& iter, const HFNGetNext& hGetNext )
		{
			this->_CurPriority = Priority;
			this->_iter = iter;
			this->_hGetNext = hGetNext;
		}

		iterator& operator=( const _Myiter& _Right )
		{
			this->_CurPriority = _Right._CurPriority;
			this->_iter = _Right._iter;
			this->_hGetNext = _Right._hGetNext;
			return *this;
		}

		const T& operator*() const
		{	// return designated object
			return *this->_iter;
		}

		_Myiter& operator++()
		{	// preincrement
			_hGetNext(&this->_CurPriority, &this->_iter);
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
			return this->_CurPriority < 0 ? true : false;
		}

	private:
		_Exiter _iter;
		Int8 _CurPriority;
		HFNGetNext _hGetNext;
	};

} //_priority_queue_v2_

/*!
* @class  PriorityQueueV2
* @brief   PriorityQueueV2
* @{
*/
template<typename T, typename P>
class PriorityQueueV2
{
public:
	typedef PriorityQueueV2<T, P>				self_type;
	typedef	T									first_type;
	typedef	P									second_type;
	typedef	UInt32								size_type;
	typedef const first_type&					const_f_reference;
	typedef const second_type&					const_s_reference;
	typedef typename _priority_queue_v2_::iterator<T>			iterator;
	typedef typename _priority_queue_v2_::iterator<T>::_Exiter	_Exiter;

public:
	/*!
	* @function   PriorityQueueV2
	* @brief    
	* @return  
	*/
	PriorityQueueV2( void )
		: _CurPriority(0)
		, _UseFalg(0)
		, _u32Size(0)
	{

	}

	/*!
	* @function   ~PriorityQueueV2
	* @brief    
	* @return  
	*/
	~PriorityQueueV2( void )
	{

	}

	inline bool push_back( const first_type& t1, const second_type& p1, UInt8 u8Priority = 0 )
	{
		u8Priority = __min(u8Priority, MESSAGE_V2_PRIORITY_NUM - 1);

		_Queue[u8Priority].push_back(t1, p1);

		if( _CurPriority < u8Priority )
		{
			_CurPriority = u8Priority;
		}

		if( !GetFlag(u8Priority) )
		{
			SetFlag(u8Priority);
		}
		_u32Size ++;
		return true;
	}

	first_type& front_f( void )
	{
start:
		if( GetFlag(_CurPriority) && !_Queue[_CurPriority].empty() )
		{

		}
		else if( _CurPriority != 0 )
		{
			ClearFlag(_CurPriority);
			_CurPriority = NextValidPriority(_CurPriority);
			goto start;
		}
		return _Queue[_CurPriority].front_f();
	}

	second_type& front_s( void )
	{
start:
		if( GetFlag(_CurPriority) && !_Queue[_CurPriority].empty() )
		{

		}
		else if( _CurPriority != 0 )
		{
			ClearFlag(_CurPriority);
			_CurPriority = NextValidPriority(_CurPriority);
			goto start;
		}
		return _Queue[_CurPriority].front_s();
	}

	void	pop( void )
	{
		pop_front();
	}

	void	pop_front( void )
	{
		if( !empty() )
		{
start:
			if( GetFlag(_CurPriority) && !_Queue[_CurPriority].empty() )
			{

			}
			else if( _CurPriority != 0 )
			{
				ClearFlag(_CurPriority);
				_CurPriority = NextValidPriority(_CurPriority);
				goto start;
			}
			_Queue[_CurPriority].pop_front();
			_u32Size --;
		}
	}

	const_f_reference at_f( UInt32 Pos )
	{	// subscript nonmutable sequence
		if( Pos > size() - 1 )
			return _container_::QueueObj<first_type>::RetNull();

		Int32 Priority = _CurPriority;

gt_start:
		if( GetFlag(Priority) && !_Queue[Priority].empty() )
		{
			if( Pos < _Queue[Priority].size() )
			{
				const_f_reference c_rf = _Queue[Priority].at_f(Pos);
				return c_rf;
			}

			Pos -= _Queue[Priority].size();
		}

		Priority --;
		if( Priority >= 0 )
			goto gt_start;
		return _container_::QueueObj<first_type>::RetNull();
	}

	const_s_reference at_s( UInt32 Pos )
	{	// subscript nonmutable sequence
		if( Pos > size() - 1 )
			return _container_::QueueObj<second_type>::RetNull();

		Int32 Priority = _CurPriority;

gt_start:
		if( GetFlag(Priority) && !_Queue[Priority].empty() )
		{
			if( Pos < _Queue[Priority].size() )
			{
				const_s_reference c_rf = _Queue[Priority].at_s(Pos);
				return c_rf;
			}

			Pos -= _Queue[Priority].size();
		}

		Priority --;
		if( Priority >= 0 )
			goto gt_start;
		return _container_::QueueObj<second_type>::RetNull();
	}

	inline iterator begin( void )
	{
		if( size() <= 0 )
			return iterator();

		front_f();
		return iterator(_CurPriority, _Queue[_CurPriority].begin(),
			function20_bind_Ex(&self_type::GetNext, this));
	}

	inline void clear( UInt8 u8Priority )
	{
		if( u8Priority < MESSAGE_V2_PRIORITY_NUM )
		{
			_u32Size -= _Queue[u8Priority].size();
			_Queue[u8Priority].clear();

		}
	}

	inline void clear( void )
	{
		_u32Size = 0;

		for( int i = 0; i < MESSAGE_V2_PRIORITY_NUM; i ++ )
		{
			_Queue[i].clear();
		}
	}

	size_type	size( void ) const
	{
		return _u32Size;
	}

	bool	empty( void ) const
	{
		return (_u32Size == 0);
	}

protected:
	void GetNext( Int8* iPriority, _Exiter* Exiter )
	{
		_Exiter iter = ++ (*Exiter);
		if( !iter )
		{
			(*iPriority) --;

start:
			if( (*iPriority) < 0 )
			{
				*iPriority = -1;
				return ;
			}

			if( GetFlag(*iPriority) && !_Queue[*iPriority].empty() )
			{

			}
			else if( *iPriority != 0 )
			{
				*iPriority = NextValidPriority(*iPriority);
				goto start;
			}

			*Exiter = _Queue[*iPriority].begin();
		}
	}

	inline void	SetFlag( UInt8 u8Priority )
	{
		_UseFalg |= 1 << u8Priority;
	}

	inline void	ClearFlag( UInt8 u8Priority )
	{
		_UseFalg &= ~(1 << u8Priority);
	}

	inline bool	GetFlag( UInt8 u8Priority ) const
	{
		return (_UseFalg >> u8Priority) & 0x01 ? true : false;
	}

	inline UInt8	NextValidPriority( UInt8 u8Priority )
	{
		if( u8Priority > MESSAGE_V2_PRIORITY_NUM - 1 || u8Priority <= 0 )
			return 0;

		do
		{
			-- u8Priority;
		}while (u8Priority > 0 && !GetFlag(u8Priority));

		return u8Priority;
	}

private:
	_container_::QueueV2< first_type, second_type, CNoneLock, MemPool_macro<0, CNoneLock> >	_Queue[MESSAGE_V2_PRIORITY_NUM];
	UInt8 _CurPriority;
	UInt8 _UseFalg;
	UInt32	_u32Size;
	//CLock	_Lock;
};
/** @} end PriorityQueueV2 */

#endif//__PRIORITYQUEUE_20150803173634_1438594594_H__
