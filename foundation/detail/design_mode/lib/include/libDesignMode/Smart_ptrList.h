
#ifndef __SMPTRLIST_20151027173413_1445938453_H__
#define __SMPTRLIST_20151027173413_1445938453_H__

#include "External.h"

namespace	_app_client{

/*!
* @class	SMPtrList
* @brief	SMPtrList
* @{
*/

template<typename K, typename P>
class SMPtrList : public _enable_shared_from_this< SMPtrList<K,P> >
{
public:
	typedef	SMPtrList<K,P>					self_type;
	typedef	_smart_ptr::SmartPtr<self_type>			self_type_sptr;

	typedef	_smart_ptr::SmartPtr<P>				SMPInfo_sptr;
	typedef std::map<K, SMPInfo_sptr>			Map_type;

	typedef function20_handle<void (void)>			func_destroy_handle;

	static	self_type_sptr Get( void )
	{
		return self_type_sptr( new self_type() );
	}

public:
	/*!
	* @function   SMPtrList
	* @brief    
	* @return  
	*/
	SMPtrList( void )
		: _Pool( sizeof(P) )
		, _isDestroy( false )
		, _lUseNum( 0 )
	{
		_fnDestroy = functio20_bind( &self_type::DestroyProcess,
								shared_from_this(), func_destroy_handle() );
	}

	/*!
	* @function   ~SMPtrList
	* @brief    
	* @return  
	*/
	~SMPtrList( void )
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
		_fnDestroy = functio20_bind( &self_type::DestroyProcess,
			shared_from_this(), fnDestroy );
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
		Map_type::iterator	iter;

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
		CASAddAndFetch(&_lUseNum);

		if( !is_destroy() )
		{
			P* pObj = _Pool.AllocObj<P>();
			if( pObj )
			{
				const SMPInfo_sptr sptr = SMPInfo_sptr(pObj, 
					functio20_bind_Ex(&self_type::Free, this));
				_Lock.Lock();
				_Map.insert(std::make_pair(Key, sptr));
				_Lock.UnLock();
				return sptr;
			}
		}
		
		CASSubAndFetch(&_lUseNum);
		return SMPInfo_sptr();
	}
	
	int	Remove( const K& Key )
	{
		_Lock.Lock();
		Map_type::iterator iter = _Map.find(Key);
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
		Map_type::iterator iter = _Map.find(Key);
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
		if( CASSubAndFetch(&_lUseNum) <= 0 && is_destroy() )
		{
			InvokeDestroy();
		}
	}

	void	InvokeDestroy( void )
	{
		const self_type_sptr sptr = shared_from_this();
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
	MemPool			_Pool;
	CLock			_Lock;
	bool			_isDestroy;
	long			_lUseNum;
	func_destroy_handle	_fnDestroy;
};
/** @} end SMPtrList */





/*template<typename K, typename P>
class SMPtrList
{
public:
	typedef	_smart_ptr::SmartPtr<P>		SMPInfo_sptr;
	typedef std::map<K, SMPInfo_sptr>	Map_type;

public:

	SMPtrList( void )
		: _Pool( sizeof(P) )
	{

	}


	~SMPtrList( void )
	{
		Release();
	}

	void Release( void )
	{
		_Lock.Lock();
		_Map.clear();
		_Lock.UnLock();
	}

	SMPInfo_sptr	get( void )
	{
		P* pObj = _Pool.AllocObj<P>();
		if( pObj )
		{
			const SMPInfo_sptr sptr = SMPInfo_sptr(pObj, 
				functio20_bind_Ex(&SMPtrList::Free, this));
			return sptr;
		}
		return SMPInfo_sptr();
	}

	long	size( void ) 
	{
		_Lock.Lock();
		long lSize = _Map.size();
		_Lock.UnLock();
		return lSize;
	}

	int	Insert( const SMPInfo_sptr& sptr, const K& Key )
	{
		if( sptr )
		{
			_Lock.Lock();
			_Map.insert(std::make_pair(Key, sptr));
			_Lock.UnLock();
			return 1;
		}

		return -1;
	}

	SMPInfo_sptr	Insert( const K& Key )
	{
		P* pObj = _Pool.AllocObj<P>();
		if( pObj )
		{
			const SMPInfo_sptr sptr = SMPInfo_sptr(pObj, 
				functio20_bind_Ex(&SMPtrList::Free, this));
			_Lock.Lock();
			_Map.insert(std::make_pair(Key, sptr));
			_Lock.UnLock();
			return sptr;
		}
		return SMPInfo_sptr();
	}
	
	int	Remove( const K& Key )
	{
		_Lock.Lock();
		Map_type::iterator iter = _Map.find(Key);
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
		Map_type::iterator iter = _Map.find(Key);
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
	}

private:
	Map_type		_Map;
	MemPool			_Pool;
	CLock			_Lock;
};*/
/** @} end SMPtrList */

} //_server

#endif//__SMPTRLIST_20151027173413_1445938453_H__
