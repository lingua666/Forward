
#ifndef ___STRING_V20_SHARE_COUNT_20160418173307_1438860785_H__
#define ___STRING_V20_SHARE_COUNT_20160418173307_1438860785_H__

#include "count_impl.hpp"

namespace	_string_v20_{

	class shared_count
	{
	private:
		sp_counted_base * _pi;

	public:

		shared_count(): _pi(0) // nothrow
		{
		}

		explicit shared_count( UInt32 uSize )
		{
			_pi = sp_counted_impl_p::GetInstance( uSize );
		}

		explicit shared_count( void* pBuf )
		{
			_pi = sp_counted_impl_buf::GetInstance( pBuf );
		}

		~shared_count() // nothrow
		{
			if( _pi != 0 ) _pi->release();
		}

		shared_count( shared_count const & r ): _pi(r._pi) // nothrow
		{
			if( _pi != 0 ) _pi->add_ref_copy(_pi);
		}

		shared_count&	operator=( const shared_count& r )
		{
			if( r._pi != _pi )
			{
				if( _pi != 0 ) _pi->release();
				if( r._pi != 0 )
				{
					if( r._pi->add_ref_copy(_pi) )
						_pi = r._pi;
				}
			}

			return *this;
		}

#if defined(_SUPPORTED_CXX11_)
		shared_count&	operator=( const shared_count&& r )
		{
			if( r._pi != _pi )
			{
				if( _pi != 0 ) _pi->release();
				if( r._pi != 0 ) 
				{
					if( r._pi->add_ref_copy(_pi) )
						_pi = r._pi;
				}
			}

			return *this;
		}
#endif

		void reset( void )
		{
			if( _pi != 0 ) 
			{
				_pi->release();
				_pi = 0;
			}
		}

		void	swap( shared_count& r )
		{
			sp_counted_base * tmp = r._pi;
			r._pi = _pi;
			_pi = tmp;
		}

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
