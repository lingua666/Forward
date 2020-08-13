
#ifndef ___STRING_SHARE_COUNT_20160418173307_1438860785_H__
#define ___STRING_SHARE_COUNT_20160418173307_1438860785_H__

#include "count_base.hpp"

namespace	_string_{

	class shared_count
	{
	private:
		sp_counted_base * _pi;

	public:
		shared_count();

		explicit shared_count( UInt32 uSize );

		explicit shared_count( void* pBuf );

		~shared_count();

		shared_count( shared_count const & r );

		shared_count&	operator=( const shared_count& r );

#if defined(_SUPPORTED_CXX11_)
		shared_count&	operator=( const shared_count&& r );
#endif

		void reset( void )
		{
			if( _pi != 0 ) 
			{
				_pi->release();
				_pi = 0;
			}
		}

		void	swap( shared_count& r );

		char*	get( void ) const
		{
			return _pi != 0 ? _pi->get(): 0;
		}

		long use_count() const // nothrow
		{
			return _pi != 0 ? _pi->use_count(): 0;
		}

		bool empty() const // nothrow
		{
			return _pi == 0;
		}

		operator bool ()
		{
			return empty() ? false : true;
		}

		bool operator!()
		{ 
			return empty();
		}

		operator bool () const
		{
			return empty() ? false : true;
		}

		bool operator!() const
		{ 
			return empty();
		}
	};

} //namespace	_foundation_

#endif // __MODAFX_20160418173305_1438860785_H__
