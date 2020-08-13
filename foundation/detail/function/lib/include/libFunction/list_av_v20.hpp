
#ifndef __LIST_AV_V20_20151123174510_1448271910_H__
#define __LIST_AV_V20_20151123174510_1448271910_H__

#include <libFunction/Arg.hpp>
#include <libCommon/macro/Macro_for.hpp>

namespace	_function_v20_ {

	template<typename R = void>
	struct result_type
	{
	};

	template<>
	struct result_type<>
	{
	};

	#define	Arg			_function_::Arg
	#define Value_av	_function_::Value_av
	#define Value_type	_function_::Value_type

	# define LIST_AV_ARGS_DECLARE(r, data, i, field)\
		PP_CAT(A, MACRO_NUMBER_INC_##i) operator[] (Arg<MACRO_NUMBER_INC_##i>) const\
		{ return PP_CAT(_a, MACRO_NUMBER_INC_##i); }\
		PP_CAT(A, MACRO_NUMBER_INC_##i) operator[] (Arg<MACRO_NUMBER_INC_##i> (*) ()) const\
		{ return PP_CAT(_a, MACRO_NUMBER_INC_##i); }\
		template<typename T>\
		void set (Arg<MACRO_NUMBER_INC_##i>, const T & v)\
		{ PP_CAT(_a, MACRO_NUMBER_INC_##i) = v; }\
		template<typename T>\
		const T & get (Arg<MACRO_NUMBER_INC_##i>) const\
		{ return PP_CAT(_a, MACRO_NUMBER_INC_##i).get(); }

	# define GET_AV_ARGS_DECLARE(r, data, i, field)\
			PP_COMMA_IF(i) data[##PP_CAT(_a, MACRO_NUMBER_INC_##i)##]

	# define LIST_AV_VALUE_DECLARE(r, data, i, field)\
			PP_COMMA_IF(i) typename Value_type<field>::type

	#define LIST_AV_NUMBER_MACRO(av)	av.getnum()

	template<typename T = void>
	struct	list_av_0;

	template<>
	struct	list_av_0<>
	{
		explicit list_av_0( void ) {}
		inline const int getnum( void ) const { return 0; }
		template<class T> T & operator[] (Value_av<T> & v) const { return v.get(); }
		template<class T> T const & operator[] (Value_av<T> const & v) const { return v.get(); }
		template<class R, class F, class A> 
		R operator()(result_type<R>, F & f, A & a)
		{
			return f();
		}
		template<class R, class F, class A> 
		R operator()(result_type<R>, F & f, A & a) const
		{
			return f();
		}
		template<class F, class A> 
		void operator()(result_type<void>, F & f, A & a)
		{
			f();
		}
		template<class F, class A> 
		void operator()(result_type<void>, F & f, A & a) const
		{
			f();
		}

		template<class R, class F> 
		R operator()(result_type<R>, F & f)
		{
			return f();
		}
	
		template<class R, class F> 
		R operator()(result_type<R>, F const & f) const
		{
			return f();
		}
	
		template<class F>
		void operator()(result_type<void>, F & f)
		{
			f();
		}
	
		template<class F>
		void operator()(result_type<void>, F const & f) const
		{
			f();
		}
	};

	template<typename T = void>
	struct	list_av_value0;

	template<>
	struct	list_av_value0<>
	{
		typedef list_av_0<> type;
	};

	template<MACRO_TARGLIST_DEF(1)>
	struct	list_av_1
	{
		list_av_1( void ) {}
		explicit list_av_1( MACRO_TPARAMLIST_DEF(1) )
		: _a1( a1 ) 
		{}
		inline const int getnum( void ) const { return 1; }
		template<class T> T & operator[] (Value_av<T> & v) const { return v.get(); }
		template<class T> T const & operator[] (Value_av<T> const & v) const { return v.get(); }
		template<class R, class F, class A> 
		R operator()(result_type<R>, F & f, A & a)
		{
			return f( SEQ_FOR_EACH_I( GET_AV_ARGS_DECLARE, a, VARIADIC_TO_SEQ( MACRO_TPARAMLIST_DEF(1) ) ) );
		}
	
		template<class R, class F, class A> 
		R operator()(result_type<R>, F const & f, A & a) const
		{
			return f( SEQ_FOR_EACH_I( GET_AV_ARGS_DECLARE, a, VARIADIC_TO_SEQ( MACRO_TPARAMLIST_DEF(1) ) ) );
		}
	
		template<class F, class A>
		void operator()(result_type<void>, F & f, A & a)
		{
			f( SEQ_FOR_EACH_I( GET_AV_ARGS_DECLARE, a, VARIADIC_TO_SEQ( MACRO_TPARAMLIST_DEF(1) ) ) );
		}
	
		template<class F, class A>
		void operator()(result_type<void>, F const & f, A & a) const
		{
			f( SEQ_FOR_EACH_I( GET_AV_ARGS_DECLARE, a, VARIADIC_TO_SEQ( MACRO_TPARAMLIST_DEF(1) ) ) );
		}
		SEQ_FOR_EACH_I( LIST_AV_ARGS_DECLARE, _, VARIADIC_TO_SEQ( MACRO_TPARAMLIST_DEF(1) ) )
		A1 _a1;
	};

	template<int I>
	struct	list_av_1< Arg<I> >
	{
		list_av_1( void ) {}
		explicit list_av_1( Arg<I> )
		{}
		inline const int getnum( void ) const { return n; }
		template<class T> T & operator[] (Value_av<T> & v) const { return v.get(); }
		template<class T> T const & operator[] (Value_av<T> const & v) const { return v.get(); }
		template<class R, class F, class A> 
		R operator()(result_type<R>, F & f, A & a)
		{
			return f(SEQ_FOR_EACH_I( GET_AV_ARGS_DECLARE, a, VARIADIC_TO_SEQ( Arg<I> ) ));
		}
	
		template<class R, class F, class A> 
		R operator()(result_type<R>, F const & f, A & a) const
		{
			return f(SEQ_FOR_EACH_I( GET_AV_ARGS_DECLARE, a, VARIADIC_TO_SEQ( Arg<I> ) ));
		}
	
		template<class F, class A>
		void operator()(result_type<void>, F & f, A & a)
		{
			f(SEQ_FOR_EACH_I( GET_AV_ARGS_DECLARE, a, VARIADIC_TO_SEQ( Arg<I> ) ));
		}
	
		template<class F, class A>
		void operator()(result_type<void>, F const & f, A & a) const
		{
			f(SEQ_FOR_EACH_I( GET_AV_ARGS_DECLARE, a, VARIADIC_TO_SEQ( Arg<I> ) ));
		}
		static Arg<I> _a1() { return Arg<I>(); }
	};

	template<MACRO_TARGLIST_DEF(1)>
	struct	list_av_value1
	{
		typedef list_av_1< SEQ_FOR_EACH_I( LIST_AV_VALUE_DECLARE, _, VARIADIC_TO_SEQ( MACRO_ARGLIST_DEF(1) ) ) > type;
	};

	#define	LIST_ARG_MACRO(n,m)\
		template<MACRO_TARGLIST_DEF(n)>\
		struct	list_av_##n : public	list_av_##m<MACRO_ARGLIST_DEF(m)>\
		{\
			list_av_##n( void ) {}\
			list_av_##n( MACRO_TPARAMLIST_DEF(n) )\
				: list_av_##m<MACRO_ARGLIST_DEF(m)>(MACRO_PARAMLIST_DEF(m))\
				,_a##n( a##n ) \
			{}\
			inline const int getnum( void ) const { return n; }\
			template<class T> T & operator[] (Value_av<T> & v) const { return v.get(); }\
			template<class T> T const & operator[] (Value_av<T> const & v) const { return v.get(); }\
			template<class R, class F, class A> \
			R operator()(result_type<R>, F & f, A & a)\
			{\
				return f(SEQ_FOR_EACH_I( GET_AV_ARGS_DECLARE, a, VARIADIC_TO_SEQ( MACRO_TPARAMLIST_DEF(n) ) ));\
			}\
			\
			template<class R, class F, class A> \
			R operator()(result_type<R>, F const & f, A & a) const\
			{\
				return f(SEQ_FOR_EACH_I( GET_AV_ARGS_DECLARE, a, VARIADIC_TO_SEQ( MACRO_TPARAMLIST_DEF(n) ) ));\
			}\
			\
			template<class F, class A>\
			void operator()(result_type<void>, F & f, A & a)\
			{\
				f(SEQ_FOR_EACH_I( GET_AV_ARGS_DECLARE, a, VARIADIC_TO_SEQ( MACRO_TPARAMLIST_DEF(n) ) ));\
			}\
			\
			template<class F, class A>\
			void operator()(result_type<void>, F const & f, A & a) const\
			{\
				f(SEQ_FOR_EACH_I( GET_AV_ARGS_DECLARE, a, VARIADIC_TO_SEQ( MACRO_TPARAMLIST_DEF(n) ) ));\
			}\
			SEQ_FOR_EACH_I( LIST_AV_ARGS_DECLARE, _, VARIADIC_TO_SEQ( MACRO_TPARAMLIST_DEF(n) ) )\
			A##n _a##n;\
		};\
		template<MACRO_TARGLIST_DEF(m) PP_COMMA_IF(m) int I>\
		struct	list_av_##n< MACRO_ARGLIST_DEF(m) PP_COMMA_IF(m) Arg<I> > : public	list_av_##m<MACRO_ARGLIST_DEF(m)>\
		{\
			list_av_##n( void ) {}\
			list_av_##n( MACRO_TPARAMLIST_DEF(m) PP_COMMA_IF(m) Arg<I> )\
					: list_av_##m<MACRO_ARGLIST_DEF(m)>(MACRO_PARAMLIST_DEF(m))\
			{}\
			inline const int getnum( void ) const { return n; }\
			template<class T> T & operator[] (Value_av<T> & v) const { return v.get(); }\
			template<class T> T const & operator[] (Value_av<T> const & v) const { return v.get(); }\
			template<class R, class F, class A> \
			R operator()(result_type<R>, F & f, A & a)\
			{\
				return f(SEQ_FOR_EACH_I( GET_AV_ARGS_DECLARE, a, VARIADIC_TO_SEQ( MACRO_TPARAMLIST_DEF(n) ) ));\
			}\
			\
			template<class R, class F, class A> \
			R operator()(result_type<R>, F const & f, A & a) const\
			{\
				return f(SEQ_FOR_EACH_I( GET_AV_ARGS_DECLARE, a, VARIADIC_TO_SEQ( MACRO_TPARAMLIST_DEF(n) ) ));\
			}\
			\
			template<class F, class A>\
			void operator()(result_type<void>, F & f, A & a)\
			{\
				f(SEQ_FOR_EACH_I( GET_AV_ARGS_DECLARE, a, VARIADIC_TO_SEQ( MACRO_TPARAMLIST_DEF(n) ) ));\
			}\
			\
			template<class F, class A>\
			void operator()(result_type<void>, F const & f, A & a) const\
			{\
				f(SEQ_FOR_EACH_I( GET_AV_ARGS_DECLARE, a, VARIADIC_TO_SEQ( MACRO_TPARAMLIST_DEF(n) ) ));\
			}\
			static Arg<I> _a##n##() { return Arg<I>(); }\
		};\
		template<MACRO_TARGLIST_DEF(n)>\
		struct	list_av_value##n\
		{\
			typedef list_av_##n< SEQ_FOR_EACH_I( LIST_AV_VALUE_DECLARE, _, VARIADIC_TO_SEQ( MACRO_ARGLIST_DEF(n) ) ) > type;\
		};

	//LIST_ARG_MACRO(1,0)
	LIST_ARG_MACRO(2,1)
	LIST_ARG_MACRO(3,2)
	LIST_ARG_MACRO(4,3)
	LIST_ARG_MACRO(5,4)
	LIST_ARG_MACRO(6,5)
	LIST_ARG_MACRO(7,6)
	LIST_ARG_MACRO(8,7)
	LIST_ARG_MACRO(9,8)
	LIST_ARG_MACRO(10,9)
	LIST_ARG_MACRO(11,10)
	LIST_ARG_MACRO(12,11)

# undef Arg
# undef Value_av
# undef Value_type
# undef LIST_AV_ARGS_DECLARE
# undef LIST_AV_NUMBER_MACRO
# undef LIST_ARG_MACRO

} //_function_v20_

#endif//__LIST_AV_V20_20151123174510_1448271910_H__
