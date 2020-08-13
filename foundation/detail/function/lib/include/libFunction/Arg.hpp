
#ifndef __ARG_20151118115435_1447818875_H__
#define __ARG_20151118115435_1447818875_H__

namespace	_function_ {

	// value

	template<class T> 
	class Value_av
	{
	public:
		Value_av(T const & t): t_(t) {}

		T & get() { return t_; }
		T const & get() const { return t_; }

		Value_av& operator=(Value_av const & rhs)
		{
			t_ = rhs.t_;
			return *this;
		}

		Value_av& operator=(T const & t)
		{
			t_ = t;
			return *this;
		}

		bool operator==(Value_av const & rhs) const
		{
			return t_ == rhs.t_;
		}

	private:
		T t_;
	};

	template< int I > struct Arg
	{
		Arg()
		{
		}

		template< class T > Arg( T const & /* t */ )
		{
			//typedef char T_must_be_placeholder[ I == is_placeholder<T>::value? 1: -1 ];
		}

	private:
		Arg& operator=( const Arg& t1 )
		{
		}
	};

	template< int I > bool operator==( Arg<I> const &, Arg<I> const & )
	{
		return true;
	}

	/*template< int I = 0, int N = 0 > struct is_placeholder;

	template< int I > struct is_placeholder< Arg<I> >
	{
		enum _vt { value = I };
	};

	template< int I > struct is_placeholder< Arg<I> (*) () >
	{
		enum _vt { value = I };
	};*/

	template<typename T> struct Value_type
	{
		typedef Value_av< T > type; 
	};

	template<int I> struct Value_type< Arg<I> >
	{
		typedef Arg<I> type;
	};

	template<int I> struct Value_type< Arg<I> (*) () >
	{
		typedef Arg<I> (*type) ();
	};

	static Arg<1> _1;
	static Arg<2> _2;
	static Arg<3> _3;
	static Arg<4> _4;
	static Arg<5> _5;
	static Arg<6> _6;
	static Arg<7> _7;
	static Arg<8> _8;
	static Arg<9> _9;

} // namespace	_function_


#endif//__ARG_20151118115435_1447818875_H__
