
#ifndef __SHARED_COUNT_20160128154200_1453966920_H__
#define __SHARED_COUNT_20160128154200_1453966920_H__

#include <libSmart_ptr/sp_counted_base.hpp>
#include <libSmart_ptr/sp_counted_impl.hpp>

namespace	_smart_ptr_v20_{

	class shared_count
	{
	private:

		sp_counted_base * pi_;

	public:

		shared_count(): pi_(0) // nothrow
		{
		}

		template<class Y> explicit shared_count( Y * p ): pi_( 0 )
		{
			pi_ = sp_alloc_mem<Y>::Alloc(p);
		}

		template<class Y> explicit shared_count( Y * p, const SMTFunction& del ): pi_( 0 )
		{
			pi_ = sp_alloc_mem<Y>::Alloc(p, del);
		}

		~shared_count() // nothrow
		{
			if( pi_ != 0 ) pi_->release();
		}

		shared_count(shared_count const & r): pi_(r.pi_) // nothrow
		{
			if( pi_ != 0 ) pi_->add_ref_copy();
		}

		shared_count&	operator=( const shared_count& r )
		{
			sp_counted_base * tmp = r.pi_;

			if( tmp != pi_ )
			{
				if( tmp != 0 ) tmp->add_ref_copy();
				if( pi_ != 0 ) pi_->release();
				pi_ = tmp;
			}

			return *this;
		}

		long use_count() const // nothrow
		{
			return pi_ != 0 ? pi_->use_count(): 0;
		}

		bool empty() const // nothrow
		{
			return pi_ == 0;
		}

		operator bool ()
		{
			return empty() ? false : true;
		}

		bool operator!()
		{ 
			return !empty();
		}

		operator bool () const
		{
			return empty() ? false : true;
		}

		bool operator!() const
		{ 
			return !empty();
		}
	};

}

#endif

