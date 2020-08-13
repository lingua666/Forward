
#ifndef __SMARTLIST_V20_20151027173413_1445938453_H__
#define __SMARTLIST_V20_20151027173413_1445938453_H__

#include <libSmart_ptr_v20/Shared_ptr.h>
#include <libSmart_ptr_v20/enable_shared_from_this.hpp>
#include <libContainer/CMyUnordered_map.hpp>

namespace	_smart_ptr_v20_{

/*!
* @class	SMartList
* @brief	SMartList
* @{
*/

template<typename K, typename P>
class SMartList : public enable_shared_from_this< SMartList<K,P> >
{
public:
	typedef	SMartList<K,P>							self_type;
	typedef	SmartPtr<self_type>						self_type_sptr;

	typedef	SmartPtr<P>								SMPInfo_sptr;
	typedef Container_Map_type<K, SMPInfo_sptr>		Map_type;

	typedef function20_handle<void (void)>	func_destroy_handle;

private:
	static MemPool_type& SmartPool( void )
	{
		static MemPool_type	s_pool( sizeof(self_type) );
		return s_pool;
	}

	static	self_type_sptr Get( void )
	{
		return (self_type_sptr)( SmartPool().template AllocObj<self_type>(), function20_bind_Ex(FreeInstance) );
	}

	static void	FreeInstance( void* ptr )
	{
		SmartPool().FreeObj(reinterpret_cast<self_type*>(ptr));
	}

public:
	/*!
	* @function   SMartList
	* @brief    
	* @return  
	*/
	SMartList( void )
		: _Pool( sizeof(P) )
		, _isDestroy( false )
		, _lUseNum( 0 )
	{
		_fnDestroy = function20_bind( &self_type::DestroyProcess,
								this->shared_from_this(), func_destroy_handle() );
	}

	/*!
	* @function   ~SMartList
	* @brief    
	* @return  
	*/
	~SMartList( void )
	{
		
	}

	void Release( void )
	{
		_isDestroy = true;

		if( _lUseNum <= 0 )
		{
			InvokeDestroy();
		}
		else
		{
			clear();
		}
	}
	
	bool is_destroy( void ) const 
	{
		return _isDestroy;
	}

	void SetFuncDestroyHandle( const func_destroy_handle& fnDestroy )
	{
		_fnDestroy.reset();
		_fnDestroy = function20_bind( &self_type::DestroyProcess,
			this->shared_from_this(), fnDestroy );
	}

	long	size( void ) 
	{
		_Lock.Lock();
		long lSize = _Map.size();
		_Lock.UnLock();
		return lSize;
	}

	void	clear( void )
	{
		typename Map_type::iterator	iter;

		do
		{
			_Lock.Lock();
			iter = _Map.begin();
			if( iter == _Map.end() )
			{
				_Lock.UnLock();
				break;
			}

			const SMPInfo_sptr sptr =  iter->second;
			_Map.erase(iter);
			_Lock.UnLock();
			sptr->Close();
		} while (true);

		_Map.clear();
	}

	SMPInfo_sptr	Insert( const K& Key )
	{
		CASAddAndFetch(&this->_lUseNum);

		if( !is_destroy() )
		{
			P* pObj = _Pool.AllocObj<P>();
			if( pObj )
			{
				const SMPInfo_sptr sptr = SMPInfo_sptr(pObj, 
					function20_bind_Ex(&self_type::Free, this));
				_Lock.Lock();
				_Map.insert(std::make_pair(Key, sptr));
				_Lock.UnLock();
				return sptr;
			}
		}
		
		CASSubAndFetch(&this->_lUseNum);
		return SMPInfo_sptr();
	}
	
	int	Remove( const K& Key )
	{
		_Lock.Lock();
		typename Map_type::iterator iter = _Map.find(Key);
		if( iter != _Map.end() )
		{
			_Map.erase(Key);
			_Lock.UnLock();

			return 1;
		}
		_Lock.UnLock();
		return -1;
	}

	SMPInfo_sptr	Find( const K& Key )
	{
		_Lock.Lock();
		typename Map_type::iterator iter = _Map.find(Key);
		if( iter != _Map.end() )
		{
			const SMPInfo_sptr sptr = iter->second;
			_Lock.UnLock();
			return sptr;
		}
		_Lock.UnLock();
		return SMPInfo_sptr();
	}

protected:
	void	Free( void* ptr )
	{
		_Pool.FreeObj(reinterpret_cast<P*>(ptr));
		if( CASSubAndFetch(&this->_lUseNum) <= 0 && is_destroy() )
		{
			InvokeDestroy();
		}
	}

	void	InvokeDestroy( void )
	{
		const self_type_sptr sptr = this->shared_from_this();
		_Lock.Lock();
		if( _fnDestroy )
		{
			_fnDestroy();
			_fnDestroy.reset();
		}
		_Lock.UnLock();
	}

	static void	DestroyProcess( const self_type_sptr& self_sptr,
							const func_destroy_handle& fnDestroy )
	{
		if( fnDestroy )
		{//Í¨ÖªÉÏ²ã
			fnDestroy();
		}
	}

private:
	Map_type		_Map;
	MemPool_type		_Pool;
	CLock			_Lock;
	bool			_isDestroy;
	long			_lUseNum;
	func_destroy_handle	_fnDestroy;
};
/** @} end SMartList */


} //_server

#endif//__SMPTRLIST_20151027173413_1445938453_H__
