
#ifndef __FUNCTIONIMPL_20150908101909_1441678749_H__
#define __FUNCTIONIMPL_20150908101909_1441678749_H__

#include <libFunction/list_av.hpp>
#include <libFunction/FunctionAddress.hpp>
#include <libCommon/CMyException.h>

using namespace _functionaddress_;

namespace	_function_ {

	//extern const FunctionConvert	g_FunctionConvert_20150909[3];
		
	class GenericClass;

	#define DECLARE_PARAMS(...) __VA_ARGS__ 
	#define DECLARE_ARGS(...)	__VA_ARGS__ 

	template <typename R, DEFAULT_TEMPLATE_DEF(9)>
	class FunctionImpl;

	#define	FUNCTION_HANDLE_DECLAR(n,args)\
	template<args>\
	class FunctionImpl<R (MACRO_ARGLIST_DEF(n))> : public IFunction<R (MACRO_ARGLIST_DEF(n))>\
	{\
	public:\
		typedef FunctionImpl self_type;\
		typedef IFunction<R (MACRO_ARGLIST_DEF(n))>	base_type;\
		typedef R (_functionaddress::GenericClass::*GenericMemFn)(MACRO_ARGLIST_DEF(n));\
	public:\
		FunctionImpl( void )\
				: _pThis(NULL)\
				, _pfnFun(NULL)\
				, _pStaticfnFun(NULL)\
		{\
		}\
		FunctionImpl( const self_type& t1 ) \
		{\
			_pStaticfnFun = t1._pStaticfnFun;\
			_pfnFun = t1._pfnFun;\
			_pThis = _pStaticfnFun == NULL ? t1._pThis : reinterpret_cast<_functionaddress::GenericClass*>(this);\
		}\
		template<typename T>\
		FunctionImpl( R (T::*pfnFun)(MACRO_ARGLIST_DEF(n)), const T* pthis )\
		{\
			Bind(pfnFun, pthis);\
		}\
		explicit FunctionImpl( R (*pfnFun)(MACRO_ARGLIST_DEF(n)) )\
		{\
			Bind( pfnFun );\
		}\
		~FunctionImpl( void )\
		{\
		}\
		inline self_type& operator=( const self_type& t1 )\
		{\
			_pfnFun = t1._pfnFun;\
			_pStaticfnFun = t1._pStaticfnFun;\
			_pThis = _pStaticfnFun == NULL ? t1._pThis : reinterpret_cast<_functionaddress::GenericClass*>(this);\
			return *this;\
		}\
		inline R	operator()(MACRO_TPARAMLIST_DEF(n))\
		{\
			return (_pThis->*_pfnFun)(MACRO_PARAMLIST_DEF(n));\
		}\
		template <typename T>\
		inline void Bind( R (T::*pfnFun)(MACRO_ARGLIST_DEF(n)), const T* pthis )\
		{\
			_pStaticfnFun = NULL;\
			_pThis = _functionaddress::SimplifyMemFunc<sizeof(pfnFun)>\
						::Convert((T*)pthis, pfnFun, _pfnFun);\
		}\
		inline void Bind( R (*pfnFun)(MACRO_ARGLIST_DEF(n)) )\
		{\
			_pThis = (_functionaddress::GenericClass*)this;\
			_pStaticfnFun = pfnFun;\
			_pfnFun = (GenericMemFn)&self_type::StaticFun;\
		}\
		inline void Clone( base_type* p )\
		{\
			self_type* ptr = reinterpret_cast<self_type*>(p);\
			_pfnFun = ptr->_pfnFun;\
			_pStaticfnFun = ptr->_pStaticfnFun;\
			_pThis = _pStaticfnFun == NULL ? ptr->_pThis : reinterpret_cast<_functionaddress::GenericClass*>(this);\
		}\
		inline R	Invoke(MACRO_TPARAMLIST_DEF(n))\
		{\
			return (_pThis->*_pfnFun)(MACRO_PARAMLIST_DEF(n));\
		}\
		bool empty( void )\
		{\
			return (_pThis  == NULL || _pfnFun == NULL);\
		}\
	protected:\
		inline void Bind( IntPtr* pfnFun, const UInt32 u32FuncSize,\
			  char* pthis, IntPtr ProbeFunc )\
		{\
			_pStaticfnFun = NULL;\
			switch(u32FuncSize)\
			{\
			case SINGLE_MEMFUNCPTR_SIZE:\
				_pThis = _functionaddress::GeneraldutyFunc<SINGLE_MEMFUNCPTR_SIZE>\
				::Convert(pthis, pfnFun, reinterpret_cast<IntPtr*>(&_pfnFun) );\
				break;\
			case SINGLE_MEMFUNCPTR_SIZE_INT:\
				_pThis = _functionaddress::GeneraldutyFunc<SINGLE_MEMFUNCPTR_SIZE_INT>\
				::Convert(pthis, pfnFun, reinterpret_cast<IntPtr*>(&_pfnFun), ProbeFunc );\
			break;\
			case SINGLE_MEMFUNCPTR_SIZE_2INT:\
				_pThis = _functionaddress::GeneraldutyFunc<SINGLE_MEMFUNCPTR_SIZE_2INT>\
				::Convert(pthis, pfnFun, reinterpret_cast<IntPtr*>(&_pfnFun), ProbeFunc );\
			break;\
			}\
		}\
		inline R StaticFun(MACRO_TPARAMLIST_DEF(n))\
		{\
			return (*_pStaticfnFun)(MACRO_PARAMLIST_DEF(n));\
		}\
	private:\
		_functionaddress::GenericClass* _pThis;\
		GenericMemFn  _pfnFun;\
		R (*_pStaticfnFun)(MACRO_ARGLIST_DEF(n));\
	};

	#define TEMPLATE_DEF(n)			typename R
	#define TEMPLATE_ARGS_DEF(n)	typename R,MACRO_TARGLIST_DEF(n)


	FUNCTION_HANDLE_DECLAR(0,TEMPLATE_DEF(0))
	FUNCTION_HANDLE_DECLAR(1,TEMPLATE_ARGS_DEF(1))
	FUNCTION_HANDLE_DECLAR(2,TEMPLATE_ARGS_DEF(2))
	FUNCTION_HANDLE_DECLAR(3,TEMPLATE_ARGS_DEF(3))
	FUNCTION_HANDLE_DECLAR(4,TEMPLATE_ARGS_DEF(4))
	FUNCTION_HANDLE_DECLAR(5,TEMPLATE_ARGS_DEF(5))
	FUNCTION_HANDLE_DECLAR(6,TEMPLATE_ARGS_DEF(6))
	FUNCTION_HANDLE_DECLAR(7,TEMPLATE_ARGS_DEF(7))
	FUNCTION_HANDLE_DECLAR(8,TEMPLATE_ARGS_DEF(8))
	FUNCTION_HANDLE_DECLAR(9,TEMPLATE_ARGS_DEF(9))

	#undef TEMPLATE_ARGS_DEF
	#undef TEMPLATE_DEF
	#undef FUNCTION_HANDLE_DECLAR

}// namespace _function_


#endif//__FUNCTIONIMPL_20150908101909_1441678749_H__
