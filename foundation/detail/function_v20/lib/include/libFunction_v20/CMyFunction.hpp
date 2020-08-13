
#ifndef __CMYFUNCTION_V20_20151120105700_1447988220_H__
#define __CMYFUNCTION_V20_20151120105700_1447988220_H__

#include <libFunction_v20/IFunction.h>
#include <libFunction_v20/list_av.hpp>
#include <libCommon/modafx.h>

namespace	_function_v20_ {

	#define	INVOKE_HANDLE_DECLAR(n)\
		template<typename R, typename T PP_COMMA_IF(n) MACRO_TARGLIST_DEF(n)>\
		class InvokeObj##n##_Handle\
		{\
		public:\
			typedef R (T::*GenericMemFn)(MACRO_ARGLIST_DEF(n));\
		public:\
			InvokeObj##n##_Handle( GenericMemFn pfnFun )\
					: _pfnFun( pfnFun )\
			{}\
			\
			R operator()(T * p PP_COMMA_IF(n) MACRO_TPARAMLIST_DEF(n)) const\
			{\
				return (p->*_pfnFun)(MACRO_PARAMLIST_DEF(n));\
			}\
			\
			R operator()(T & t PP_COMMA_IF(n) MACRO_TPARAMLIST_DEF(n)) const\
			{\
				return (t.*_pfnFun)(MACRO_PARAMLIST_DEF(n));\
			}\
			template<class U> R operator()(U & u PP_COMMA_IF(n) MACRO_TPARAMLIST_DEF(n)) const\
			{\
				return ((u.get())->*_pfnFun)(MACRO_PARAMLIST_DEF(n));\
			}\
			\
			template<class U> R operator()(U const & u PP_COMMA_IF(n) MACRO_TPARAMLIST_DEF(n)) const\
			{\
				return ((u.get())->*_pfnFun)(MACRO_PARAMLIST_DEF(n));\
			}\
			\
		private:\
			GenericMemFn  _pfnFun;\
		};\
		template<typename R PP_COMMA_IF(n) MACRO_TARGLIST_DEF(n)>\
		class Invoke##n##_Handle\
		{\
		public:\
			typedef R (*GenericMemFn)(MACRO_ARGLIST_DEF(n));\
		public:\
			Invoke##n##_Handle( GenericMemFn pfnFun )\
				: _pfnFun( pfnFun )\
			{}\
			\
			R operator()(MACRO_TPARAMLIST_DEF(n)) const\
			{\
				return (*_pfnFun)(MACRO_PARAMLIST_DEF(n));\
			}\
			\
		private:\
			GenericMemFn  _pfnFun;\
		};\
		template<typename R PP_COMMA_IF(n) MACRO_TARGLIST_DEF(n)>\
		class stdcall_Invoke##n##_Handle\
		{\
		public:\
			typedef R (_CALLTYPE *GenericMemFn)(MACRO_ARGLIST_DEF(n));\
		public:\
			stdcall_Invoke##n##_Handle( GenericMemFn pfnFun )\
			: _pfnFun( pfnFun )\
			{}\
			\
			R operator()(MACRO_TPARAMLIST_DEF(n)) const\
			{\
				return (*_pfnFun)(MACRO_PARAMLIST_DEF(n));\
			}\
			\
		private:\
			GenericMemFn  _pfnFun;\
		};

		INVOKE_HANDLE_DECLAR(0)
		INVOKE_HANDLE_DECLAR(1)
		INVOKE_HANDLE_DECLAR(2)
		INVOKE_HANDLE_DECLAR(3)
		INVOKE_HANDLE_DECLAR(4)
		INVOKE_HANDLE_DECLAR(5)
		INVOKE_HANDLE_DECLAR(6)
		INVOKE_HANDLE_DECLAR(7)
		INVOKE_HANDLE_DECLAR(8)
		INVOKE_HANDLE_DECLAR(9)

	#define	INVOKE_HANDLE_CLASSNAME(n)\
			InvokeObj##n##_Handle

	#define	INVOKE_HANDLE_NAME(n)\
			Invoke##n##_Handle

	#define	INVOKE_STDCALL_HANDLE_NAME(n)\
			stdcall_Invoke##n##_Handle

	#define	VOID_BIND_DECLAR(R, f, l, n)\
		template<MACRO_TARGLIST_DEF(n)>\
		void operator()(MACRO_REF_PARAMLIST_DEF(n))\
		{\
			list_av_##n<MACRO_REF_ARGLIST_DEF(n)> a(MACRO_PARAMLIST_DEF(n));\
			l(result_type<void>(), f, a);\
		}\
		\
		template<MACRO_TARGLIST_DEF(n)>\
		void operator()(MACRO_REF_PARAMLIST_DEF(n)) const\
		{\
			list_av_##n<MACRO_REF_ARGLIST_DEF(n)> a(MACRO_PARAMLIST_DEF(n));\
			l(result_type<void>(), f, a);\
		}

	#define BIND_DECLAR(R, f, l, n)\
		template<typename R PP_COMMA_IF(n) MACRO_TARGLIST_DEF(n)>\
		R operator()(result_type<R> PP_COMMA_IF(n) MACRO_REF_PARAMLIST_DEF(n))\
		{\
			list_av_##n<MACRO_REF_ARGLIST_DEF(n)> a(MACRO_PARAMLIST_DEF(n));\
			return l(result_type<R>(), f, a);\
		}\
		\
		template<typename R PP_COMMA_IF(n) MACRO_TARGLIST_DEF(n)>\
		R operator()(result_type<R> PP_COMMA_IF(n) MACRO_REF_PARAMLIST_DEF(n)) const\
		{\
			list_av_##n<MACRO_REF_ARGLIST_DEF(n)> a(MACRO_PARAMLIST_DEF(n));\
			return l(result_type<R>(), f, a);\
		}\
		VOID_BIND_DECLAR(R, f, l, n)

	template<typename R, typename F, typename L>
	class bind_t
	{
	public:
		typedef	bind_t	self_type;

	public:
		bind_t(F f, L l)
			: _f(f)
			, _l(l)
		{

		}

		bind_t( const self_type& self )
			: _f(self._f)
			, _l(self._l)
		{
			
		}

		const UInt32	size( void ) const
		{
			return sizeof(self_type);
		}

		template<typename R1>
		R1 operator()(result_type<R1>)
		{
			list_av_0<> a;
			return _l(result_type<R1>(), _f, a);
		}
		
		template<typename R1>
		R1 operator()(result_type<R1>) const
		{
			list_av_0<> a;
			return _l(result_type<R1>(), _f, a);
		}

		void operator()()
		{
			list_av_0<> a;
			_l(result_type<void>(), _f, a);
		}

		void operator()() const
		{
			list_av_0<> a;
			_l(result_type<void>(), _f, a);
		}

		bool value( void ) const
		{
			return is_ptr(_l);
		}

		BIND_DECLAR(R1, _f, _l, 1)
		BIND_DECLAR(R1, _f, _l, 2)
		BIND_DECLAR(R1, _f, _l, 3)
		BIND_DECLAR(R1, _f, _l, 4)
		BIND_DECLAR(R1, _f, _l, 5)
		BIND_DECLAR(R1, _f, _l, 6)
		BIND_DECLAR(R1, _f, _l, 7)
		BIND_DECLAR(R1, _f, _l, 8)
		BIND_DECLAR(R1, _f, _l, 9)
		BIND_DECLAR(R1, _f, _l, 10)
		BIND_DECLAR(R1, _f, _l, 11)
		BIND_DECLAR(R1, _f, _l, 12)

	protected:
		bool is_ptr( list_av_0<> t1 ) const
		{
			return true;
		}

		bool is_ptr( list_av_0<>& t1 ) const
		{
			return true;
		}

		template<typename T>
		bool is_ptr( T* t1 ) const
		{
			return true;
		}

		template<typename T>
		bool is_ptr( T& t1 ) const
		{
			return t1.is_arg();
		}

		template<typename T>
		bool is_ptr_n( _function_::Value_av<T*>& t1 ) const
		{
			return true;
		}

		template<typename T>
		bool is_ptr_n( const _function_::Value_av<T*>& t1 ) const
		{
			return true;
		}

		template<typename T>
		bool is_ptr_n( _function_::Value_av<T>& t1 ) const
		{
			return false;
		}

		template<typename T>
		bool is_ptr_n( const _function_::Value_av<T>& t1 ) const
		{
			return false;
		}

		template< int I >
		bool is_ptr_n( _function_::Arg<I>& t1 ) const
		{
			return true;
		}

		template< int I >
		bool is_ptr_n( const _function_::Arg<I>& t1 ) const
		{
			return true;
		}

		template< int I >
		bool is_ptr_n( _function_::Arg<I> (*) () ) const
		{
			return true;
		}

	private:
		F _f;
		L _l;
	};

#define ARGS_LIST_NUMBER(n)		((n + 2) * sizeof(IntPtr))

union function_buffer
{
	__M_ALIGNED__(4)	char _buf[ARGS_LIST_NUMBER(4)];
	struct tagObj
	{
		mutable	void* _ptr;
	}Obj;
	// For function pointers of all kinds
	mutable void (*func_ptr)();
};

#undef ARGS_LIST_NUMBER

enum functor_manager_operation_type {
	clone_functor_tag,
	move_functor_tag,
	destroy_functor_tag,
	check_functor_type_tag,
	get_functor_type_tag
};

	
#define	FUNCTION_INVOKE_IMPL(n)\
	template<typename R, typename F PP_COMMA_IF(n)\
			MACRO_TARGLIST_DEF(n) >\
	struct Function_Invoker##n\
	{\
		inline static R invoke_obj(function_buffer& function_ptr PP_COMMA_IF(n)\
						MACRO_TPARAMLIST_DEF(n))\
		{\
			return (*reinterpret_cast<F*>(function_ptr.Obj._ptr))(result_type<R>() PP_COMMA_IF(n) MACRO_PARAMLIST_DEF(n));\
		}\
		inline static void manage_obj( const function_buffer& in_buffer, function_buffer& out_buffer, \
										functor_manager_operation_type op )\
		{\
			switch(op)\
			{\
			case clone_functor_tag:\
				out_buffer.Obj._ptr = reinterpret_cast<void*>(new F(*(F*)(in_buffer.Obj._ptr)));\
				break;\
			case destroy_functor_tag:\
				delete reinterpret_cast<F*>(out_buffer.Obj._ptr);\
				break;\
			}\
		}\
		inline static R invoke(function_buffer& function_ptr PP_COMMA_IF(n)\
						MACRO_TPARAMLIST_DEF(n))\
		{\
			return (*reinterpret_cast<F*>(&function_ptr))(result_type<R>() PP_COMMA_IF(n) MACRO_PARAMLIST_DEF(n));\
		}\
		inline static R invoke_n(function_buffer& function_ptr PP_COMMA_IF(n)\
						MACRO_TPARAMLIST_DEF(n))\
		{\
			return (*reinterpret_cast<F*>(&function_ptr))(MACRO_PARAMLIST_DEF(n));\
		}\
		inline static void manage( const function_buffer& in_buffer, function_buffer& out_buffer, \
										functor_manager_operation_type op )\
		{\
			switch(op)\
			{\
			case clone_functor_tag:\
				new (reinterpret_cast<void*>(&out_buffer)) F(*(F*)(&in_buffer));\
				break;\
			case destroy_functor_tag:\
				reinterpret_cast<F*>(&out_buffer)->~F();\
				break;\
			}\
		}\
	};\
	template<typename F PP_COMMA_IF(n)\
			MACRO_TARGLIST_DEF(n) >\
	struct Function_Void_Invoker##n\
	{\
		inline static void invoke_obj(function_buffer& function_ptr PP_COMMA_IF(n)\
							MACRO_TPARAMLIST_DEF(n))\
		{\
			(*reinterpret_cast<F*>(function_ptr.Obj._ptr))(MACRO_PARAMLIST_DEF(n));\
		}\
		inline static void manage_obj( const function_buffer& in_buffer, function_buffer& out_buffer, \
										functor_manager_operation_type op )\
		{\
			switch(op)\
			{\
			case clone_functor_tag:\
				out_buffer.Obj._ptr = reinterpret_cast<void*>(new F(*(F*)(in_buffer.Obj._ptr)));\
				break;\
			case destroy_functor_tag:\
				delete reinterpret_cast<F*>(out_buffer.Obj._ptr);\
				break;\
			}\
		}\
		inline static void invoke(function_buffer& function_ptr PP_COMMA_IF(n)\
						MACRO_TPARAMLIST_DEF(n))\
		{\
			(*reinterpret_cast<F*>(&function_ptr))(MACRO_PARAMLIST_DEF(n));\
		}\
		inline static void invoke_n(function_buffer& function_ptr PP_COMMA_IF(n)\
						MACRO_TPARAMLIST_DEF(n))\
		{\
			(*reinterpret_cast<F*>(&function_ptr))(MACRO_PARAMLIST_DEF(n));\
		}\
		inline static void manage( const function_buffer& in_buffer, function_buffer& out_buffer, \
										functor_manager_operation_type op )\
		{\
			switch(op)\
			{\
			case clone_functor_tag:\
				new (reinterpret_cast<void*>(&out_buffer)) F(*(F*)(&in_buffer));\
				break;\
			case destroy_functor_tag:\
				reinterpret_cast<F*>(&out_buffer)->~F();\
				break;\
			}\
		}\
	};\
	template<typename R, typename F PP_COMMA_IF(n)\
			MACRO_TARGLIST_DEF(n)>\
	struct Function_Invoker##n##_Type\
	{\
		typedef Function_Invoker##n<R,F PP_COMMA_IF(n) MACRO_ARGLIST_DEF(n)>	type;\
	};\
	template<typename F PP_COMMA_IF(n)\
			MACRO_TARGLIST_DEF(n)>\
	struct Function_Invoker##n##_Type<void, F PP_COMMA_IF(n) MACRO_ARGLIST_DEF(n)>\
	{\
		typedef Function_Void_Invoker##n<F PP_COMMA_IF(n) MACRO_ARGLIST_DEF(n)>	type;\
	};

	FUNCTION_INVOKE_IMPL(0)
	FUNCTION_INVOKE_IMPL(1)
	FUNCTION_INVOKE_IMPL(2)
	FUNCTION_INVOKE_IMPL(3)
	FUNCTION_INVOKE_IMPL(4)
	FUNCTION_INVOKE_IMPL(5)
	FUNCTION_INVOKE_IMPL(6)
	FUNCTION_INVOKE_IMPL(7)
	FUNCTION_INVOKE_IMPL(8)
	FUNCTION_INVOKE_IMPL(9)

# define GET_PLACEHOLDER_DECLARE(r, data, i, field)\
	PP_COMMA_IF(i) PP_CAT(_function_::_, MACRO_NUMBER_INC_##i)

#if defined(PLATFORM_OS_FAMILY_WINDOWS)
	#if (TYPES_ARCH_64_BIT == 1)
		#define STDCALL_FUNCTION_DEC(n)
	#else
		#define STDCALL_FUNCTION_DEC(n)\
		typedef	R (_CALLTYPE *pfnFunc_std)(MACRO_ARGLIST_DEF(n));\
		CMyFunction( const pfnFunc_std& t1 ) \
				: _Manage(0)\
		{\
			*reinterpret_cast<pfnFunc_std*>(&_buffer) = t1;\
			_Invoker = Function_Invoker##n##_Type<R,pfnFunc_std PP_COMMA_IF(n) MACRO_ARGLIST_DEF(n)>::type::invoke;\
		}
	#endif
#else
	#define STDCALL_FUNCTION_DEC(n)
#endif

/*template <typename R, DEFAULT_TEMPLATE_DEF(9)>
class CMyFunction;*/

#define	FUNCTION_HANDLE_DECLAR(n)\
	template<typename R PP_COMMA_IF(n) MACRO_TARGLIST_DEF(n)>\
	class CMyFunction<R (MACRO_ARGLIST_DEF(n))>\
	{\
	public:\
		typedef CMyFunction self_type;\
		typedef	R (*pfnFunc)(MACRO_ARGLIST_DEF(n));\
		typedef R (*pfnInvoker)(function_buffer& PP_COMMA_IF(n)\
						MACRO_ARGLIST_DEF(n));\
		typedef	void (*pfnManage)( const function_buffer&, function_buffer&,\
								functor_manager_operation_type);\
	public:\
		CMyFunction( void )\
				: _Invoker(0)\
				, _Manage(0)\
		{}\
		CMyFunction( const self_type& t1 ) \
				: _Invoker(0)\
				, _Manage(0)\
		{\
			if(t1)\
				Clone(t1);\
		}\
		CMyFunction( const pfnFunc& t1 ) \
				: _Manage(0)\
		{\
			if( t1 == NULL )\
				_Invoker = 0;\
			else\
			{\
				*reinterpret_cast<pfnFunc*>(&_buffer) = t1;\
				_Invoker = Function_Invoker##n##_Type<R,pfnFunc PP_COMMA_IF(n) MACRO_ARGLIST_DEF(n)>::type::invoke_n;\
			}\
		}\
		STDCALL_FUNCTION_DEC(n)\
		template<typename F>\
		CMyFunction( const F& f ) \
		{\
			Clone_f(f);\
		}\
		~CMyFunction( void ) \
		{\
			Release();\
		}\
		void Release( void )\
		{\
			_Invoker = 0;\
			if( _Manage != 0 )\
			{\
				_Manage(_buffer, _buffer, destroy_functor_tag);\
				_Manage = 0;\
			}\
		}\
		inline self_type& operator=( const self_type& t1 )\
		{\
			if( t1 && this != &t1 )\
			{\
				Release();\
				Clone(t1);\
			}\
			return *this;\
		}\
		inline bool operator==( const self_type& t1 )\
		{\
			return this->_buffer.Obj._ptr == t1._buffer.Obj._ptr;\
		}\
		inline bool operator!=( const self_type& t1 )\
		{\
			return this->_buffer.Obj._ptr != t1._buffer.Obj._ptr;\
		}\
		inline R	operator()(MACRO_TPARAMLIST_DEF(n))\
		{\
			return _Invoker(_buffer PP_COMMA_IF(n) MACRO_PARAMLIST_DEF(n));\
		}\
		inline R	operator()(MACRO_TPARAMLIST_DEF(n)) const\
		{\
			return _Invoker(_buffer PP_COMMA_IF(n) MACRO_PARAMLIST_DEF(n));\
		}\
		inline operator bool () \
		{\
			return !this->empty();\
		}\
		inline bool operator!()\
		{ \
			return this->empty();\
		}\
		inline operator bool () const\
		{\
			return !this->empty();\
		}\
		inline bool operator!() const\
		{ \
			return this->empty();\
		}\
		void reset( void )\
		{\
			if( !empty() )\
			{\
				Release();\
			}\
		}\
	protected:\
		inline bool empty( void ) const\
		{\
			return (_Invoker == 0);\
		}\
		void Clone( const self_type& t1 )\
		{\
			_Manage = t1._Manage;\
			_Invoker = t1._Invoker;\
			if( _Manage != 0 )\
				_Manage(t1._buffer, _buffer, clone_functor_tag);\
			else\
				memcpy(&_buffer, &t1._buffer, sizeof(function_buffer));\
		}\
		template<typename F>\
		void Clone_f( const F& f )\
		{\
			if(sizeof(F) > sizeof(function_buffer))\
			{\
				_buffer.Obj._ptr = new F(f);\
				_Invoker = Function_Invoker##n##_Type<R,F PP_COMMA_IF(n) MACRO_ARGLIST_DEF(n)>::type::invoke_obj;\
				_Manage = Function_Invoker##n##_Type<R,F PP_COMMA_IF(n) MACRO_ARGLIST_DEF(n)>::type::manage_obj;\
			}\
			else\
			{\
				new (reinterpret_cast<void*>(&_buffer)) F(f);\
				_Invoker = Function_Invoker##n##_Type<R,F PP_COMMA_IF(n) MACRO_ARGLIST_DEF(n)>::type::invoke;\
				if( !f.value() )\
					_Manage = Function_Invoker##n##_Type<R,F PP_COMMA_IF(n) MACRO_ARGLIST_DEF(n)>::type::manage;\
				else\
					_Manage = 0;\
			}\
		}\
	private:\
		mutable function_buffer	_buffer;\
		pfnInvoker		_Invoker;\
		pfnManage		_Manage;\
	};

	FUNCTION_HANDLE_DECLAR(0)
	FUNCTION_HANDLE_DECLAR(1)
	FUNCTION_HANDLE_DECLAR(2)
	FUNCTION_HANDLE_DECLAR(3)
	FUNCTION_HANDLE_DECLAR(4)
	FUNCTION_HANDLE_DECLAR(5)
	FUNCTION_HANDLE_DECLAR(6)
	FUNCTION_HANDLE_DECLAR(7)
	FUNCTION_HANDLE_DECLAR(8)
	FUNCTION_HANDLE_DECLAR(9)

#define GET_ARGS_LIST_DECLARE(r, data, i, field)\
	PP_COMMA_IF(i) _function_::Arg< PP_CAT(MACRO_NUMBER_INC_,i) >

#define SEQ_FOR_ARGS_LIST_PLACEHOLDER(n)\
	SEQ_FOR_EACH_I( GET_PLACEHOLDER_DECLARE, _, VARIADIC_TO_SEQ( MACRO_ARGLIST_DEF(n) ) )

#define ARGS_LIST_PLACEHOLDER(n)\
	PP_COMMA_IF(n) \
	SEQ_FOR_ARGS_LIST_PLACEHOLDER(n)

#define SEQ_FOR_ARGS_LIST_TYPE(n)\
	SEQ_FOR_EACH_I( GET_ARGS_LIST_DECLARE, _, VARIADIC_TO_SEQ( MACRO_ARGLIST_DEF(n) ) )

#define ARGS_LIST_TYPE(n)\
	PP_COMMA_IF(n) \
	SEQ_FOR_ARGS_LIST_TYPE(n)

#define	ARGS_LIST_TYPE_EMPTY(n)

#define IIF_ARGS_LIST(n,marco)\
		PP_IF(n, marco, ARGS_LIST_TYPE_EMPTY)(n)


#if defined(PLATFORM_OS_FAMILY_WINDOWS)
	#if (TYPES_ARCH_64_BIT == 1)
		#define STDCALL_BIND_V20(n,m)
	#else
		#define STDCALL_BIND_V20(n,m)\
			template<typename R PP_COMMA_IF(n) MACRO_TARGLIST_EX_DEF(n, B)\
					PP_COMMA_IF(n) MACRO_TARGLIST_DEF(n)>\
			bind_t<R, INVOKE_STDCALL_HANDLE_NAME(n)<R PP_COMMA_IF(n) MACRO_ARGLIST_EX_DEF(n,B)>,\
					typename list_av_value##n<MACRO_ARGLIST_DEF(n)>::type > \
			bind(R (_CALLTYPE *f) (MACRO_ARGLIST_EX_DEF(n,B)) PP_COMMA_IF(n) MACRO_TPARAMLIST_DEF(n))\
			{\
				typedef INVOKE_STDCALL_HANDLE_NAME(n)<R PP_COMMA_IF(n) MACRO_ARGLIST_EX_DEF(n,B)> F;\
				typedef typename list_av_value##n<MACRO_ARGLIST_DEF(n)>::type list_type;\
				return bind_t<R, F, list_type>(F(f), list_type(MACRO_PARAMLIST_DEF(n)));\
			}\
			\
			template<typename R PP_COMMA_IF(n) MACRO_TARGLIST_DEF(n)>\
			bind_t<R, INVOKE_STDCALL_HANDLE_NAME(n)<R PP_COMMA_IF(n) MACRO_ARGLIST_DEF(n)>,\
					typename list_av_value##n< IIF_ARGS_LIST(n, SEQ_FOR_ARGS_LIST_TYPE) >::type > \
			bind_Ex(R (_CALLTYPE *f) (MACRO_ARGLIST_DEF(n)))\
			{\
				typedef INVOKE_STDCALL_HANDLE_NAME(n)<R PP_COMMA_IF(n) MACRO_ARGLIST_DEF(n)> F;\
				typedef typename list_av_value##n< IIF_ARGS_LIST(n, SEQ_FOR_ARGS_LIST_TYPE) >::type list_type;\
				return bind_t<R, F, list_type>(F(f), list_type( IIF_ARGS_LIST(n, SEQ_FOR_ARGS_LIST_PLACEHOLDER) ));\
			}
	#endif
#else
	#define STDCALL_BIND_V20(n,m)
#endif


#define	FUNCTION_OBJ_BIND_DECLAR(n,m)\
	template<typename R, typename T PP_COMMA_IF(n) MACRO_TARGLIST_EX_DEF(n, B)\
			, MACRO_TARGLIST_DEF(m)>\
	bind_t<R, INVOKE_HANDLE_CLASSNAME(n)<R,T PP_COMMA_IF(n) MACRO_ARGLIST_EX_DEF(n,B)>,\
		typename list_av_value##m<MACRO_ARGLIST_DEF(m)>::type > \
	bind(R (T::*f) (MACRO_ARGLIST_EX_DEF(n,B)), MACRO_TPARAMLIST_DEF(m))\
	{\
		typedef INVOKE_HANDLE_CLASSNAME(n)<R, T PP_COMMA_IF(n) MACRO_ARGLIST_EX_DEF(n,B)> F;\
		typedef typename list_av_value##m<MACRO_ARGLIST_DEF(m)>::type list_type;\
		return bind_t<R, F, list_type>(F(f), list_type(MACRO_PARAMLIST_DEF(m)));\
	}\
	template<typename R PP_COMMA_IF(n) MACRO_TARGLIST_EX_DEF(n, B)\
		PP_COMMA_IF(n) MACRO_TARGLIST_DEF(n)>\
	bind_t<R, INVOKE_HANDLE_NAME(n)<R PP_COMMA_IF(n) MACRO_ARGLIST_EX_DEF(n,B)>,\
		typename list_av_value##n<MACRO_ARGLIST_DEF(n)>::type > \
	bind(R (*f) (MACRO_ARGLIST_EX_DEF(n,B)) PP_COMMA_IF(n) MACRO_TPARAMLIST_DEF(n))\
	{\
		typedef INVOKE_HANDLE_NAME(n)<R PP_COMMA_IF(n) MACRO_ARGLIST_EX_DEF(n,B)> F;\
		typedef typename list_av_value##n<MACRO_ARGLIST_DEF(n)>::type list_type;\
		return bind_t<R, F, list_type>(F(f), list_type(MACRO_PARAMLIST_DEF(n)));\
	}\
	template<typename R, typename T PP_COMMA_IF(n) MACRO_TARGLIST_DEF(n)>\
	bind_t<R, INVOKE_HANDLE_CLASSNAME(n)<R, T PP_COMMA_IF(n) MACRO_ARGLIST_DEF(n)>,\
			typename list_av_value##m< T* IIF_ARGS_LIST(n, ARGS_LIST_TYPE) >::type > \
	bind_Ex(R (T::*f) (MACRO_ARGLIST_DEF(n)), T* pthis)\
	{\
		typedef INVOKE_HANDLE_CLASSNAME(n)<R, T PP_COMMA_IF(n) MACRO_ARGLIST_DEF(n)> F;\
		typedef typename list_av_value##m<T* IIF_ARGS_LIST(n, ARGS_LIST_TYPE)>::type list_type;\
		return bind_t<R, F, list_type>(F(f), list_type(pthis IIF_ARGS_LIST(n, ARGS_LIST_PLACEHOLDER)));\
	}\
	template<typename R, typename T, typename C PP_COMMA_IF(n) MACRO_TARGLIST_DEF(n)>\
	bind_t<R, INVOKE_HANDLE_CLASSNAME(n)<R, T PP_COMMA_IF(n) MACRO_ARGLIST_DEF(n)>,\
			typename list_av_value##m< C IIF_ARGS_LIST(n, ARGS_LIST_TYPE) >::type > \
	bind_Ex(R (T::*f) (MACRO_ARGLIST_DEF(n)), const C& obj)\
	{\
		typedef INVOKE_HANDLE_CLASSNAME(n)<R, T PP_COMMA_IF(n) MACRO_ARGLIST_DEF(n)> F;\
		typedef typename list_av_value##m<C IIF_ARGS_LIST(n, ARGS_LIST_TYPE)>::type list_type;\
		return bind_t<R, F, list_type>(F(f), list_type(obj IIF_ARGS_LIST(n, ARGS_LIST_PLACEHOLDER)));\
	}\
	template<typename R PP_COMMA_IF(n) MACRO_TARGLIST_DEF(n)>\
	bind_t<R, INVOKE_HANDLE_NAME(n)<R PP_COMMA_IF(n) MACRO_ARGLIST_DEF(n)>,\
			typename list_av_value##n< IIF_ARGS_LIST(n, SEQ_FOR_ARGS_LIST_TYPE) >::type > \
	bind_Ex(R (*f) (MACRO_ARGLIST_DEF(n)))\
	{\
		typedef INVOKE_HANDLE_NAME(n)<R PP_COMMA_IF(n) MACRO_ARGLIST_DEF(n)> F;\
		typedef typename list_av_value##n< IIF_ARGS_LIST(n, SEQ_FOR_ARGS_LIST_TYPE) >::type list_type;\
		return bind_t<R, F, list_type>(F(f), list_type( IIF_ARGS_LIST(n, SEQ_FOR_ARGS_LIST_PLACEHOLDER) ));\
	}\
	STDCALL_BIND_V20(n,m)
	
	FUNCTION_OBJ_BIND_DECLAR(0,1)
	FUNCTION_OBJ_BIND_DECLAR(1,2)
	FUNCTION_OBJ_BIND_DECLAR(2,3)
	FUNCTION_OBJ_BIND_DECLAR(3,4)
	FUNCTION_OBJ_BIND_DECLAR(4,5)
	FUNCTION_OBJ_BIND_DECLAR(5,6)
	FUNCTION_OBJ_BIND_DECLAR(6,7)
	FUNCTION_OBJ_BIND_DECLAR(7,8)
	FUNCTION_OBJ_BIND_DECLAR(8,9)
	FUNCTION_OBJ_BIND_DECLAR(9,10)


/*#define FUNCTION_OBJ_ARG_DECLAR(n)
	template<typename T, typename F>
	const T& function_get_arg##n_v20( const F& f )
	{

	}*/



#undef	SEQ_FOR_ARGS_LIST_PLACEHOLDER
#undef	SEQ_FOR_ARGS_LIST_TYPE
#undef	GET_ARGS_LIST_DECLARE
#undef	ARGS_LIST_PLACEHOLDER
#undef	ARGS_LIST_TYPE
#undef	ARGS_LIST_TYPE_EMPTY
#undef	IIF_ARGS_LIST
#undef	FUNCTION_HANDLE_DECLAR
#undef  GET_PLACEHOLDER_DECLARE
#undef	INVOKE_HANDLE_DECLAR
#undef	INVOKE_HANDLE_CLASSNAME
#undef	BIND_DECLAR
#undef	FUNCTION_INVOKE_IMPL
#undef	FUNCTION_OBJ_BIND_DECLAR
#undef  STDCALL_BIND_V20

} // namespace	_function_



#endif//__CMYFUNCTION_V20_20151120105700_1447988220_H__
