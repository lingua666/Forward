
#ifndef __IFUNCTION_V20_20151118115435_1447818875_H__
#define __IFUNCTION_V20_20151118115435_1447818875_H__

////////////////////////////////////////////////////////////////
//
//使用原则：尽可能多用CMyFunction对象替代AnyFunc对象
//
///////////////////////////////////////////////////////////////

namespace	_function_v20_ {

	template<typename R, typename A1 = void, typename A2 = void, typename A3 = void, typename A4 = void\
		, typename A5 = void, typename A6 = void, typename A7 = void, typename A8 = void, typename A9 = void>
	class CMyFunction;

	template<typename R>
	class CMyFunction<R ()>;

	template<typename R, typename A1>
	class CMyFunction<R (A1)>;

	template<typename R, typename A1, typename A2>
	class CMyFunction<R (A1, A2)>;

	template<typename R, typename A1, typename A2, typename A3>
	class CMyFunction<R (A1, A2, A3)>;

	template<typename R, typename A1, typename A2, typename A3, typename A4>
	class CMyFunction<R (A1, A2, A3, A4)>;

	template<typename R, typename A1, typename A2, typename A3, typename A4\
			, typename A5>
	class CMyFunction<R (A1, A2, A3, A4, A5)>;

	template<typename R, typename A1, typename A2, typename A3, typename A4\
		, typename A5, typename A6>
	class CMyFunction<R (A1, A2, A3, A4, A5, A6)>;

	template<typename R, typename A1, typename A2, typename A3, typename A4\
		, typename A5, typename A6, typename A7>
	class CMyFunction<R (A1, A2, A3, A4, A5, A6, A7)>;

	template<typename R, typename A1, typename A2, typename A3, typename A4\
		, typename A5, typename A6, typename A7, typename A8>
	class CMyFunction<R (A1, A2, A3, A4, A5, A6, A7, A8)>;

	template<typename R, typename A1, typename A2, typename A3, typename A4\
		, typename A5, typename A6, typename A7, typename A8, typename A9>
	class CMyFunction<R (A1, A2, A3, A4, A5, A6, A7, A8, A9)>;

	template<typename R, typename F, typename L>
	class bind_t;
	
}	// namespace _function_


#define function20_handle	_function_v20_::CMyFunction
#define function20_bind		_function_v20_::bind
#define function20_bind_Ex	_function_v20_::bind_Ex

#endif
