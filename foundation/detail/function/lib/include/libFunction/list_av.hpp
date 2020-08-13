
#ifndef __LIST_AV_20150908101909_1441678749_H__
#define __LIST_AV_20150908101909_1441678749_H__

#include <libFunction/FunctionAddress.hpp>
#include <libCommon/macro/Macro_for.hpp>

namespace	_function_ {

	//模板参数计数
	template<typename T>
	struct is_non_template_param
	{
		enum { have_template_param =1 };
	};

	template<>
	struct is_non_template_param<void>
	{
		enum { have_template_param =0 };
	};

	template <typename P1 = void, typename P2 = void, typename P3 = void, typename P4 = void,\
		typename P5 = void, typename P6 = void, typename P7 = void, typename P8 = void>
	struct template_params
	{
		enum { count = is_non_template_param<P1>::have_template_param +
			is_non_template_param<P2>::have_template_param +
			is_non_template_param<P3>::have_template_param +
			is_non_template_param<P4>::have_template_param +
			is_non_template_param<P5>::have_template_param +
			is_non_template_param<P6>::have_template_param +
			is_non_template_param<P7>::have_template_param +
			is_non_template_param<P8>::have_template_param }; 
	};		

#define LIST_AV_NUMBER_MACRO(av)	av.getnum()

	template<typename T = void>
	struct	list_av_0;

	template<>
	struct	list_av_0<>
	{
		explicit list_av_0( void ) {}
		inline const int getnum( void ) const { return 0; }
	};

#define	LIST_ARG_MACRO(n,m)\
	template<MACRO_TARGLIST_DEF(n)>\
	struct	list_av_##n : public	list_av_##m<MACRO_ARGLIST_DEF(m)>\
	{\
	typedef void (_functionaddress_::GenericClass::*GenericMemFn)(MACRO_ARGLIST_DEF(n));\
	list_av_##n( void ) {}\
	explicit list_av_##n( MACRO_TPARAMLIST_DEF(n) )\
	: list_av_##m<MACRO_ARGLIST_DEF(m)>(MACRO_PARAMLIST_DEF(m))\
	,_a##n( a##n ) \
	{}\
	inline const int getnum( void ) const { return n; }\
	A##n _a##n;\
	};

	LIST_ARG_MACRO(1,0)
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

#undef LIST_AV_NUMBER_MACRO
#undef LIST_ARG_MACRO

}

#endif
