
#ifndef __MACRO_PARAMTER_20160419111505_1438860785_H__
#define __MACRO_PARAMTER_20160419111505_1438860785_H__

#include "Macro_base.hpp"

# define TTL_RET(x) x
# define TTL_APPEND( x, y )					TTL_APPEND1(x,y)
# define TTL_APPEND1( x, y )				x ## y

# define DEFAULT_TEMPLATE_DEF(n)\
	TTL_APPEND(MACRO_PARAMTER_,TTL_APPEND(MACRO_NUMBER_,n))(DEFAULT_TARG_DEF,DEFAULT_TARG_END_DEF,A)

# define DEFAULT_TARG_DEF(t,n)				typename t##n=void
# define DEFAULT_TARG_END_DEF(t,n)			,typename  t##n=void

# define MACRO_TARGLIST_DEF(n)\
	TTL_APPEND(MACRO_PARAMTER_,TTL_APPEND(MACRO_NUMBER_,n))(MACRO_TARG_TYPE_DEF,MACRO_TARG_TYPE_END_DEF,A)
# define MACRO_TARG_TYPE_DEF(t,n)			typename t##n
# define MACRO_TARG_TYPE_END_DEF(t,n)		,typename  t##n

# define MACRO_TARGLIST_EX_DEF(n,p)\
	TTL_APPEND(MACRO_PARAMTER_,TTL_APPEND(MACRO_NUMBER_,n))(MACRO_TARG_TYPE_DEF,MACRO_TARG_TYPE_END_DEF,p)

# define MACRO_ARGLIST_DEF(n)\
	TTL_APPEND(MACRO_PARAMTER_,TTL_APPEND(MACRO_NUMBER_,n))(MACRO_ARG_TYPE_DEF,MACRO_ARG_TYPE_END_DEF,A)
# define MACRO_ARG_TYPE_DEF(t,n)			t##n
# define MACRO_ARG_TYPE_END_DEF(t,n)		,t##n

# define MACRO_ARGLIST_EX_DEF(n,p)\
	TTL_APPEND(MACRO_PARAMTER_,TTL_APPEND(MACRO_NUMBER_,n))(MACRO_ARG_TYPE_DEF,MACRO_ARG_TYPE_END_DEF,p)

# define MACRO_REF_ARGLIST_DEF(n)\
	TTL_APPEND(MACRO_PARAMTER_,TTL_APPEND(MACRO_NUMBER_,n))(MACRO_REF_ARG_TYPE_DEF,MACRO_REF_ARG_TYPE_END_DEF,A)
# define MACRO_REF_ARG_TYPE_DEF(t,n)			t##n&
# define MACRO_REF_ARG_TYPE_END_DEF(t,n)		,t##n&

# define MACRO_CREF_ARGLIST_DEF(n)\
	TTL_APPEND(MACRO_PARAMTER_,TTL_APPEND(MACRO_NUMBER_,n))(MACRO_CREF_ARG_TYPE_DEF,MACRO_CREF_ARG_TYPE_END_DEF,A)
# define MACRO_CREF_ARG_TYPE_DEF(t,n)			const t##n&
# define MACRO_CREF_ARG_TYPE_END_DEF(t,n)		,const t##n&

# define MACRO_CARGLIST_DEF(n)\
	TTL_APPEND(MACRO_PARAMTER_,TTL_APPEND(MACRO_NUMBER_,n))(MACRO_CARG_TYPE_DEF,MACRO_CARG_TYPE_END_DEF,A)

# define MACRO_CARGLIST_EX_DEF(n,p)\
	TTL_APPEND(MACRO_PARAMTER_,TTL_APPEND(MACRO_NUMBER_,n))(MACRO_CARG_TYPE_DEF,MACRO_CARG_TYPE_END_DEF,p)
# define MACRO_CARG_TYPE_DEF(t,n)			const t##n&
# define MACRO_CARG_TYPE_END_DEF(t,n)		,const t##n&

#define MACRO_TPARAMLIST_DEF(n)\
	TTL_APPEND(MACRO_PARAMTER_,TTL_APPEND(MACRO_NUMBER_,n))(MACRO_TPARAM_DEF,MACRO_TPARAM_END_DEF,A)
# define MACRO_TPARAM_DEF(t,n)				t##n a##n
# define MACRO_TPARAM_END_DEF(t,n)			,t##n  a##n

#define MACRO_REF_PARAMLIST_DEF(n)\
	TTL_APPEND(MACRO_PARAMTER_,TTL_APPEND(MACRO_NUMBER_,n))(MACRO_REF_PARAM_DEF,MACRO_REF_PARAM_END_DEF,A)
# define MACRO_REF_PARAM_DEF(t,n)				t##n& a##n
# define MACRO_REF_PARAM_END_DEF(t,n)			,t##n&  a##n

#define MACRO_CREF_PARAMLIST_DEF(n)\
	TTL_APPEND(MACRO_PARAMTER_,TTL_APPEND(MACRO_NUMBER_,n))(MACRO_CREF_PARAM_DEF,MACRO_CREF_PARAM_END_DEF,A)
# define MACRO_CREF_PARAM_DEF(t,n)				const t##n& a##n
# define MACRO_CREF_PARAM_END_DEF(t,n)			,const t##n&  a##n

# define MACRO_PARAMLIST_DEF(n)\
	TTL_APPEND(MACRO_PARAMTER_,TTL_APPEND(MACRO_NUMBER_,n))(MACRO_PARAM_DEF,MACRO_PARAM_END_DEF,A)
# define MACRO_PARAM_DEF(t,n)				a##n
# define MACRO_PARAM_END_DEF(t,n)			,a##n

# define MACRO_TARGLIST_BIT_DEF(n)\
	TTL_APPEND(MACRO_PARAMTER_,TTL_APPEND(MACRO_NUMBER_,n))(MACRO_ARG_BIT_DEF,MACRO_ARG_BIT_END_DEF,A)

# define MACRO_TARGLIST_BIT_EX_DEF(n,p)\
	TTL_APPEND(MACRO_PARAMTER_,TTL_APPEND(MACRO_NUMBER_,n))(MACRO_ARG_BIT_DEF,MACRO_ARG_BIT_END_DEF,p)
# define MACRO_ARG_BIT_DEF(t,n)				_##n
# define MACRO_ARG_BIT_END_DEF(t,n)			,_##n

# define MACRO_PARAMTER_0(macro1,macro2,p)	
# define MACRO_PARAMTER_1(macro1,macro2,p)	MACRO_PARAMTER_0(macro1,macro2,p) macro1(p,1)
# define MACRO_PARAMTER_2(macro1,macro2,p)	MACRO_PARAMTER_1(macro1,macro2,p) macro2(p,2)
# define MACRO_PARAMTER_3(macro1,macro2,p)	MACRO_PARAMTER_2(macro1,macro2,p) macro2(p,3)
# define MACRO_PARAMTER_4(macro1,macro2,p)	MACRO_PARAMTER_3(macro1,macro2,p) macro2(p,4)
# define MACRO_PARAMTER_5(macro1,macro2,p)	MACRO_PARAMTER_4(macro1,macro2,p) macro2(p,5)
# define MACRO_PARAMTER_6(macro1,macro2,p)	MACRO_PARAMTER_5(macro1,macro2,p) macro2(p,6)
# define MACRO_PARAMTER_7(macro1,macro2,p)	MACRO_PARAMTER_6(macro1,macro2,p) macro2(p,7)
# define MACRO_PARAMTER_8(macro1,macro2,p)	MACRO_PARAMTER_7(macro1,macro2,p) macro2(p,8)
# define MACRO_PARAMTER_9(macro1,macro2,p)	MACRO_PARAMTER_8(macro1,macro2,p) macro2(p,9)
# define MACRO_PARAMTER_10(macro1,macro2,p)	MACRO_PARAMTER_9(macro1,macro2,p) macro2(p,10)
# define MACRO_PARAMTER_11(macro1,macro2,p)	MACRO_PARAMTER_10(macro1,macro2,p) macro2(p,11)
# define MACRO_PARAMTER_12(macro1,macro2,p)	MACRO_PARAMTER_11(macro1,macro2,p) macro2(p,12)

# define MACRO_SUB_PARAMTER_0(macro1,macro2,p)	
# define MACRO_SUB_PARAMTER_1(macro1,macro2,p)	MACRO_PARAMTER_0(macro1,macro2,p)
# define MACRO_SUB_PARAMTER_2(macro1,macro2,p)	MACRO_PARAMTER_1(macro1,macro2,p)
# define MACRO_SUB_PARAMTER_3(macro1,macro2,p)	MACRO_PARAMTER_2(macro1,macro2,p)
# define MACRO_SUB_PARAMTER_4(macro1,macro2,p)	MACRO_PARAMTER_3(macro1,macro2,p)
# define MACRO_SUB_PARAMTER_5(macro1,macro2,p)	MACRO_PARAMTER_4(macro1,macro2,p)
# define MACRO_SUB_PARAMTER_6(macro1,macro2,p)	MACRO_PARAMTER_5(macro1,macro2,p)
# define MACRO_SUB_PARAMTER_7(macro1,macro2,p)	MACRO_PARAMTER_6(macro1,macro2,p)
# define MACRO_SUB_PARAMTER_8(macro1,macro2,p)	MACRO_PARAMTER_7(macro1,macro2,p)
# define MACRO_SUB_PARAMTER_9(macro1,macro2,p)	MACRO_PARAMTER_8(macro1,macro2,p)
# define MACRO_SUB_PARAMTER_10(macro1,macro2,p)	MACRO_PARAMTER_9(macro1,macro2,p)
# define MACRO_SUB_PARAMTER_11(macro1,macro2,p)	MACRO_PARAMTER_10(macro1,macro2,p)
# define MACRO_SUB_PARAMTER_12(macro1,macro2,p)	MACRO_PARAMTER_11(macro1,macro2,p)

# if defined(_WIN32) || defined(_WIN64)
	# define MACRO_RANGELIST_DEF(begin,end,p)\
		TTL_APPEND(MACRO_LIST_,TTL_APPEND(NUMBER_,TTL_APPEND(end,TTL_APPEND(_,begin))))(MACRO_ARG_TYPE_DEF,MACRO_ARG_TYPE_END_DEF,end,p##._a)
	# define MACRO_RANGELIST_P_DEF(begin,end,p)\
		TTL_APPEND(MACRO_LIST_,TTL_APPEND(NUMBER_,TTL_APPEND(end,TTL_APPEND(_,begin))))(MACRO_ARG_TYPE_DEF,MACRO_ARG_TYPE_END_DEF,end,p##->_a)

	# define MACRO_MEMBERLIST_DEF(n,p)\
		TTL_APPEND(MACRO_PARAMTER_,TTL_APPEND(MACRO_NUMBER_,n))(MACRO_ARG_TYPE_DEF,MACRO_ARG_TYPE_END_DEF,p##._a)
	# define MACRO_MEMBERLIST_P_DEF(n,p)\
		TTL_APPEND(MACRO_PARAMTER_,TTL_APPEND(MACRO_NUMBER_,n))(MACRO_ARG_TYPE_DEF,MACRO_ARG_TYPE_END_DEF,p##->_a)
# elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__TOS_LINUX__)
	# define MACRO_RANGELIST_DEF(begin,end,p)\
		TTL_APPEND(MACRO_LIST_,TTL_APPEND(NUMBER_,TTL_APPEND(end,TTL_APPEND(_,begin))))(MACRO_ARG_TYPE_DEF,MACRO_ARG_TYPE_END_DEF,end,p._a)
	# define MACRO_RANGELIST_P_DEF(begin,end,p)\
		TTL_APPEND(MACRO_LIST_,TTL_APPEND(NUMBER_,TTL_APPEND(end,TTL_APPEND(_,begin))))(MACRO_ARG_TYPE_DEF,MACRO_ARG_TYPE_END_DEF,end,p->_a)

	# define MACRO_MEMBERLIST_DEF(n,p)\
		TTL_APPEND(MACRO_PARAMTER_,TTL_APPEND(MACRO_NUMBER_,n))(MACRO_ARG_TYPE_DEF,MACRO_ARG_TYPE_END_DEF,p._a)
	# define MACRO_MEMBERLIST_P_DEF(n,p)\
		TTL_APPEND(MACRO_PARAMTER_,TTL_APPEND(MACRO_NUMBER_,n))(MACRO_ARG_TYPE_DEF,MACRO_ARG_TYPE_END_DEF,p->_a)
#endif

# define MACRO_LIST_0(macro1,macro2,end,p)		
# define MACRO_LIST_1(macro1,macro2,end,p)		macro1(p,1) TTL_APPEND(MACRO_LIST_,TTL_APPEND(NUMBER_,TTL_APPEND(end,_2)))(macro2,macro2,end,p)
# define MACRO_LIST_2(macro1,macro2,end,p)		macro1(p,2) TTL_APPEND(MACRO_LIST_,TTL_APPEND(NUMBER_,TTL_APPEND(end,_3)))(macro2,macro2,end,p)
# define MACRO_LIST_3(macro1,macro2,end,p)		macro1(p,3) TTL_APPEND(MACRO_LIST_,TTL_APPEND(NUMBER_,TTL_APPEND(end,_4)))(macro2,macro2,end,p)
# define MACRO_LIST_4(macro1,macro2,end,p)		macro1(p,4) TTL_APPEND(MACRO_LIST_,TTL_APPEND(NUMBER_,TTL_APPEND(end,_5)))(macro2,macro2,end,p)
# define MACRO_LIST_5(macro1,macro2,end,p)		macro1(p,5) TTL_APPEND(MACRO_LIST_,TTL_APPEND(NUMBER_,TTL_APPEND(end,_6)))(macro2,macro2,end,p)
# define MACRO_LIST_6(macro1,macro2,end,p)		macro1(p,6) TTL_APPEND(MACRO_LIST_,TTL_APPEND(NUMBER_,TTL_APPEND(end,_7)))(macro2,macro2,end,p)
# define MACRO_LIST_7(macro1,macro2,end,p)		macro1(p,7) TTL_APPEND(MACRO_LIST_,TTL_APPEND(NUMBER_,TTL_APPEND(end,_8)))(macro2,macro2,end,p)
# define MACRO_LIST_8(macro1,macro2,end,p)		macro1(p,8) TTL_APPEND(MACRO_LIST_,TTL_APPEND(NUMBER_,TTL_APPEND(end,_9)))(macro2,macro2,end,p)
# define MACRO_LIST_9(macro1,macro2,end,p)		macro1(p,9) TTL_APPEND(MACRO_LIST_,TTL_APPEND(NUMBER_,TTL_APPEND(end,_10)))(macro2,macro2,end,p)
# define MACRO_LIST_10(macro1,macro2,end,p)		macro1(p,10) TTL_APPEND(MACRO_LIST_,TTL_APPEND(NUMBER_,TTL_APPEND(end,_11)))(macro2,macro2,end,p)
# define MACRO_LIST_11(macro1,macro2,end,p)		macro1(p,11) TTL_APPEND(MACRO_LIST_,TTL_APPEND(NUMBER_,TTL_APPEND(end,_12)))(macro2,macro2,end,p)
# define MACRO_LIST_12(macro1,macro2,end,p)		macro1(p,12) TTL_APPEND(MACRO_LIST_,TTL_APPEND(NUMBER_,TTL_APPEND(end,_13)))(macro2,macro2,end,p)

# define MACRO_VA_NARG_SIZE(...) \
		MACRO_PP_IF(MACRO_VA_EMPTY_IF(__VA_ARGS__),0,MACRO_VA_NARG_SIZE__(__VA_ARGS__))

# define MACRO_VA_EMPTY_IF(...)	TTL_APPEND(__VA_BOOL_,MACRO_VA_NARG_SIZE__(1,##__VA_ARGS__))

# define MACRO_VA_NARG_SIZE__(...) \
		MACRO_NUMBER_DEC(MACRO_VA_NARG_SIZE_((_0, ## __VA_ARGS__, MACRO_RSEQ_N())))

# define MACRO_VA_NARG_SIZE_(args_list) \
		MACRO_VA_ARG_SIZE_N args_list

# define MACRO_VA_ARG_SIZE_N( \
		_1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
		_11, _12, _13, _14, _15, _16, _17, _18,	\
		_19, _20, _21, _22, _23, _24, _25, _26,	\
		_27, _28, _29, _30, _31, _32, _33, _34,	\
		_35, _36, _37, _38, _39, _40, _41, _42, \
		_43, _44, _45, _46, _47, _48, _49, _50, \
		_51, _52, _53, _54, _55, _56, _57, _58, \
		_59, _60, _61, _62, _63, _64, N, ...) N 

# define MACRO_RSEQ_N() \
		64, 63, 62, 61, 60,\
		59, 58, 57, 56, 55, 54, 53, 52, 51, 50,\
		49, 48, 47, 46, 45, 44, 43, 42, 41, 40,\
		39, 38, 37, 36, 35, 34, 33, 32, 31, 30,\
		29, 28, 27, 26, 25, 24, 23, 22, 21, 20,\
		19, 18, 17, 16, 15, 14, 13, 12, 11, 10,\
		9,  8,  7,  6,  5,  4,  3,  2,  1,  0 

# define __VA_BOOL_1		1
# define __VA_BOOL_2		0
# define __VA_BOOL_3		0
# define __VA_BOOL_4		0
# define __VA_BOOL_5		0
# define __VA_BOOL_6		0
# define __VA_BOOL_7		0
# define __VA_BOOL_8		0
# define __VA_BOOL_9		0
# define __VA_BOOL_10		0
# define __VA_BOOL_11		0
# define __VA_BOOL_12		0
# define __VA_BOOL_13		0
# define __VA_BOOL_14		0
# define __VA_BOOL_15		0
# define __VA_BOOL_16		0
# define __VA_BOOL_17		0
# define __VA_BOOL_18		0
# define __VA_BOOL_19		0
# define __VA_BOOL_20		0
# define __VA_BOOL_21		0
# define __VA_BOOL_22		0
# define __VA_BOOL_23		0
# define __VA_BOOL_24		0
# define __VA_BOOL_25		0
# define __VA_BOOL_26		0
# define __VA_BOOL_27		0
# define __VA_BOOL_28		0
# define __VA_BOOL_29		0
# define __VA_BOOL_31		0
# define __VA_BOOL_32		0
# define __VA_BOOL_33		0
# define __VA_BOOL_34		0
# define __VA_BOOL_35		0
# define __VA_BOOL_36		0
# define __VA_BOOL_37		0
# define __VA_BOOL_38		0
# define __VA_BOOL_39		0
# define __VA_BOOL_40		0
# define __VA_BOOL_41		0
# define __VA_BOOL_42		0
# define __VA_BOOL_43		0
# define __VA_BOOL_44		0
# define __VA_BOOL_45		0
# define __VA_BOOL_46		0
# define __VA_BOOL_47		0
# define __VA_BOOL_48		0
# define __VA_BOOL_49		0
# define __VA_BOOL_50		0
# define __VA_BOOL_51		0
# define __VA_BOOL_52		0
# define __VA_BOOL_53		0
# define __VA_BOOL_54		0
# define __VA_BOOL_55		0
# define __VA_BOOL_56		0
# define __VA_BOOL_57		0
# define __VA_BOOL_58		0
# define __VA_BOOL_59		0
# define __VA_BOOL_60		0
# define __VA_BOOL_61		0
# define __VA_BOOL_62		0
# define __VA_BOOL_63		0
# define __VA_BOOL_64		0


#endif
