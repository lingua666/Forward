
#include <libSmart_ptr/SmartPtr_Impl.h>


namespace	_smart_ptr_{

	#include "CASCount.cpp"

	struct tagSMTHdr20160617
	{
		long	Weak_Ref;
		long	Ref;
		void*	ptr;
		bool	isAuto;
		SMTFunction	_DelFunc;
	};

	MemPool_type*	GetSmartHdrPool20160728( void )
	{
		static MemPool_type	s_Pool( sizeof(tagSMTHdr20160617) );
		return &s_Pool;
	}

	struct SMartHdr20160622
	{
		static SMARTHANDLE	CreateHdr( void* ptr, bool isAuto = true )
		{
			tagSMTHdr20160617* handle = GetSmartHdrPool20160728()->AllocObj<tagSMTHdr20160617>();
			handle->ptr = ptr;
			handle->Ref = handle->Weak_Ref = 1;
			handle->isAuto = isAuto;
			return reinterpret_cast<SMARTHANDLE>(handle);
		}

		static SMARTHANDLE	CreateHdr( void* ptr, const SMTFunction& Function )
		{
			tagSMTHdr20160617* handle = GetSmartHdrPool20160728()->AllocObj<tagSMTHdr20160617>();
			handle->ptr = ptr;
			handle->Ref = handle->Weak_Ref = 1;
			handle->_DelFunc = Function;
			return reinterpret_cast<SMARTHANDLE>(handle);
		}

		static long	AddWeakRef( SMARTHANDLE handle )
		{
			return CASAddAndFetch(&reinterpret_cast<tagSMTHdr20160617*>(handle)->Weak_Ref);
		}

		static void	SubWeakRef( SMARTHANDLE handle )
		{
			if( CASSubAndFetch(&reinterpret_cast<tagSMTHdr20160617*>(handle)->Weak_Ref) == 0 )
			{
				GetSmartHdrPool20160728()->FreeObj(reinterpret_cast<tagSMTHdr20160617*>(handle));
			}
		}

		static long	AddRef( SMARTHANDLE handle )
		{
			return CASAddAndFetch(&reinterpret_cast<tagSMTHdr20160617*>(handle)->Ref);
		}

		static long	SubRef( SMARTHANDLE handle )
		{
			return CASSubAndFetch(&reinterpret_cast<tagSMTHdr20160617*>(handle)->Ref);
		}

		static void*	get( SMARTHANDLE handle ) 
		{
			return reinterpret_cast<tagSMTHdr20160617*>(handle)->ptr;
		}

		static void	set( SMARTHANDLE handle, void* ptr )
		{
			reinterpret_cast<tagSMTHdr20160617*>(handle)->ptr = reinterpret_cast<void*>(ptr);
		}

		static SMARTHANDLE	Clone( SMARTHANDLE handle )
		{
			AddRef(handle);
			return handle;
		}

		static long	weak_count( SMARTHANDLE handle ) 
		{
			return reinterpret_cast<tagSMTHdr20160617*>(handle)->Weak_Ref;
		}

		static long	use_count( SMARTHANDLE handle ) 
		{
			return reinterpret_cast<tagSMTHdr20160617*>(handle)->Ref;
		}

		static void	Detach( const SMARTHANDLE handle, void (*fpnDelete)( void* ) )
		{
			tagSMTHdr20160617* pHdr = reinterpret_cast<tagSMTHdr20160617*>(handle);
			pHdr->Ref = 0;
			if( !pHdr->_DelFunc )
			{
				if( pHdr->ptr != NULL && pHdr->isAuto )
				{
					fpnDelete( pHdr->ptr );
					pHdr->ptr = NULL;
				}
			}
			else
			{
				pHdr->_DelFunc(pHdr->ptr);
				//pHdr->_DelFunc.reset();
				pHdr->ptr = NULL;
			}
		}
	};

	SmartPtr_Impl::SmartPtr_Impl( void )
		: _pHdr( NULL )
	{

	}

	SmartPtr_Impl::SmartPtr_Impl( int t1 )
		: _pHdr(NULL)
	{

	}

	SmartPtr_Impl::SmartPtr_Impl( void* t1, bool isAuto )
	{
		_pHdr = SMartHdr20160622::CreateHdr(t1, isAuto);
	}

	SmartPtr_Impl::SmartPtr_Impl( void* t1, const SMTFunction& Function )
	{
		_pHdr = SMartHdr20160622::CreateHdr(t1, Function);
	}

	SmartPtr_Impl::SmartPtr_Impl( const SmartPtr_Impl& t1 )
	{
		if(t1)
			_pHdr = SMartHdr20160622::Clone(t1._pHdr);
		else
			_pHdr = NULL;
	}

	SmartPtr_Impl&	SmartPtr_Impl::operator=( const SmartPtr_Impl& t1 )
	{
		if(t1)
			_pHdr = SMartHdr20160622::Clone(t1._pHdr);

		return *this;
	}

	void SmartPtr_Impl::reset( void (*fpnDelete)( void* ) )
	{
		if( !empty() && SMartHdr20160622::SubRef(_pHdr) == 0 )
		{
			SMartHdr20160622::Detach(_pHdr, fpnDelete) ;
			ReleaseHdr();
		}
		else
		{
			_pHdr = NULL;
		}
	}

	void*	SmartPtr_Impl::get( void )
	{
		return SMartHdr20160622::get(_pHdr);
	}

	void*	SmartPtr_Impl::get( void ) const
	{
		return SMartHdr20160622::get(_pHdr);
	}

	UInt32	SmartPtr_Impl::use_count( void ) const
	{
		return SMartHdr20160622::use_count(_pHdr);
	}

	long	SmartPtr_Impl::weak_count( void ) const
	{
		return SMartHdr20160622::weak_count(_pHdr);
	}

	long	SmartPtr_Impl::AddWeakRef( void )
	{
		return SMartHdr20160622::AddWeakRef(_pHdr);
	}

	void	SmartPtr_Impl::ReleaseHdr( void )
	{
		SMartHdr20160622::SubWeakRef(_pHdr);
		_pHdr = NULL;
	}

	bool	SmartPtr_Impl::empty( void ) const
	{
		return _pHdr == NULL;
	}

	void	SmartPtr_Impl::Clone( const SmartPtr_Impl& t1 )
	{
		_pHdr = t1._pHdr;
	}

} // namespace	_smart_ptr_

