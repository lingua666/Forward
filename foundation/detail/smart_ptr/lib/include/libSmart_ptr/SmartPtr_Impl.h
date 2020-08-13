
#ifndef __SMARTPTR_IMPL_20150808113239_1439004759_H__
#define __SMARTPTR_IMPL_20150808113239_1439004759_H__

#include <libFunction_v20/CMyFunction.hpp>
#include <libMempool/MemPool.h>

namespace	_smart_ptr_{
	
	typedef void*	SMARTHANDLE;
	typedef	function20_handle< void (void*) >	SMTFunction;

	template<typename T> class weak_ptr;
	template<typename T> class SmartPtr;

	class SmartPtr_Impl
	{
	public:
		template<typename T> friend class SmartPtr;

		template<typename T> friend class weak_ptr;

	protected:
		SmartPtr_Impl( void );

		SmartPtr_Impl( int t1 );

		SmartPtr_Impl( void* t1, bool isAuto = true );

		SmartPtr_Impl( void* t1, const SMTFunction& Function );

		SmartPtr_Impl( const SmartPtr_Impl& t1 );

		SmartPtr_Impl&	operator=( const SmartPtr_Impl& t1 );

		void reset( void (*fpnDelete)( void* ) );

		void*	get( void ) ;

		void*	get( void ) const;

		UInt32	use_count( void ) const;

		long	weak_count( void ) const;

		long	AddWeakRef( void );

		void	ReleaseHdr( void );

		void	Clone( const SmartPtr_Impl& t1 );

		operator bool ()
		{
			return !this->empty();
		}

		bool operator!()
		{ 
			return this->empty();
		}

		operator bool () const
		{
			return !this->empty();
		}

		bool operator!() const
		{ 
			return this->empty();
		}

	protected:
		bool	empty( void ) const;

	private:
		SMARTHANDLE					_pHdr;
	};

} // namespace	_smart_ptr_

#endif//__SMARTPTR_IMPL_20150808113239_1439004759_H__
