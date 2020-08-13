

#ifndef __MACRO_FOR_20160419111505_1438860785_H__
#define __MACRO_FOR_20160419111505_1438860785_H__

#include "Macro_paramter.hpp"
#include "Macro_variable.h"

#	 define MACRO_PP_VARIADIC_TO_SEQ(...)\
			MACRO_PP_TUPLE_TO_SEQ((__VA_ARGS__))

/*#if defined(PLATFORM_OS_FAMILY_WINDOWS)
	#	 define MACRO_PP_TUPLE_TO_SEQ(...)\
				MACRO_PP_TUPLE_TO_SEQ_I(MACRO_PP_OVERLOAD(MACRO_PP_TUPLE_TO_SEQ_O_, __VA_ARGS__), (__VA_ARGS__))
# elif defined(PLATFORM_OS_FAMILY_UNIX)*/
	#	 define MACRO_PP_TUPLE_TO_SEQ(...)\
				MACRO_PP_OVERLOAD(MACRO_PP_TUPLE_TO_SEQ_O_, __VA_ARGS__)(__VA_ARGS__)
//#endif

#	 define MACRO_PP_OVERLOAD(prefix, ...)\
			MACRO_PP_CAT(prefix, MACRO_VA_NARG_SIZE(__VA_ARGS__))
#    define MACRO_PP_TUPLE_TO_SEQ_O_0(tuple)
#    define MACRO_PP_TUPLE_TO_SEQ_O_1(tuple)\
			MACRO_PP_CAT(MACRO_PP_TUPLE_TO_SEQ_, MACRO_VA_NARG_SIZE tuple) tuple

#	 define MACRO_PP_TUPLE_TO_SEQ_I(m, args)\
			MACRO_PP_CAT(m ## args,)

#	 define MACRO_PP_TUPLE_TO_SEQ_1(e0) (e0)
#	 define MACRO_PP_TUPLE_TO_SEQ_2(e0, e1) (e0)(e1)
#	 define MACRO_PP_TUPLE_TO_SEQ_3(e0, e1, e2) (e0)(e1)(e2)
#	 define MACRO_PP_TUPLE_TO_SEQ_4(e0, e1, e2, e3) (e0)(e1)(e2)(e3)
#	 define MACRO_PP_TUPLE_TO_SEQ_5(e0, e1, e2, e3, e4) (e0)(e1)(e2)(e3)(e4)
#	 define MACRO_PP_TUPLE_TO_SEQ_6(e0, e1, e2, e3, e4, e5) (e0)(e1)(e2)(e3)(e4)(e5)
#	 define MACRO_PP_TUPLE_TO_SEQ_7(e0, e1, e2, e3, e4, e5, e6) (e0)(e1)(e2)(e3)(e4)(e5)(e6)
#	 define MACRO_PP_TUPLE_TO_SEQ_8(e0, e1, e2, e3, e4, e5, e6, e7) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)
#	 define MACRO_PP_TUPLE_TO_SEQ_9(e0, e1, e2, e3, e4, e5, e6, e7, e8) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)
#	 define MACRO_PP_TUPLE_TO_SEQ_10(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)
#	 define MACRO_PP_TUPLE_TO_SEQ_11(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)
#	 define MACRO_PP_TUPLE_TO_SEQ_12(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)
#	 define MACRO_PP_TUPLE_TO_SEQ_13(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)
#	 define MACRO_PP_TUPLE_TO_SEQ_14(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)
#	 define MACRO_PP_TUPLE_TO_SEQ_15(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)
#	 define MACRO_PP_TUPLE_TO_SEQ_16(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)
#	 define MACRO_PP_TUPLE_TO_SEQ_17(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)
#	 define MACRO_PP_TUPLE_TO_SEQ_18(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)
#	 define MACRO_PP_TUPLE_TO_SEQ_19(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)
#	 define MACRO_PP_TUPLE_TO_SEQ_20(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)
#	 define MACRO_PP_TUPLE_TO_SEQ_21(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)
#	 define MACRO_PP_TUPLE_TO_SEQ_22(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)
#	 define MACRO_PP_TUPLE_TO_SEQ_23(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)
#	 define MACRO_PP_TUPLE_TO_SEQ_24(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)
#	 define MACRO_PP_TUPLE_TO_SEQ_25(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)
#	 define MACRO_PP_TUPLE_TO_SEQ_26(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)
#	 define MACRO_PP_TUPLE_TO_SEQ_27(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)
#	 define MACRO_PP_TUPLE_TO_SEQ_28(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)
#	 define MACRO_PP_TUPLE_TO_SEQ_29(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)
#	 define MACRO_PP_TUPLE_TO_SEQ_30(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)
#	 define MACRO_PP_TUPLE_TO_SEQ_31(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)(e30)
#	 define MACRO_PP_TUPLE_TO_SEQ_32(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)(e30)(e31)
#	 define MACRO_PP_TUPLE_TO_SEQ_33(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)(e30)(e31)(e32)
#	 define MACRO_PP_TUPLE_TO_SEQ_34(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)(e30)(e31)(e32)(e33)
#	 define MACRO_PP_TUPLE_TO_SEQ_35(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)(e30)(e31)(e32)(e33)(e34)
#	 define MACRO_PP_TUPLE_TO_SEQ_36(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)(e30)(e31)(e32)(e33)(e34)(e35)
#	 define MACRO_PP_TUPLE_TO_SEQ_37(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)(e30)(e31)(e32)(e33)(e34)(e35)(e36)
#	 define MACRO_PP_TUPLE_TO_SEQ_38(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)(e30)(e31)(e32)(e33)(e34)(e35)(e36)(e37)
#	 define MACRO_PP_TUPLE_TO_SEQ_39(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)(e30)(e31)(e32)(e33)(e34)(e35)(e36)(e37)(e38)
#	 define MACRO_PP_TUPLE_TO_SEQ_40(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)(e30)(e31)(e32)(e33)(e34)(e35)(e36)(e37)(e38)(e39)
#	 define MACRO_PP_TUPLE_TO_SEQ_41(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)(e30)(e31)(e32)(e33)(e34)(e35)(e36)(e37)(e38)(e39)(e40)
#	 define MACRO_PP_TUPLE_TO_SEQ_42(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)(e30)(e31)(e32)(e33)(e34)(e35)(e36)(e37)(e38)(e39)(e40)(e41)
#	 define MACRO_PP_TUPLE_TO_SEQ_43(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)(e30)(e31)(e32)(e33)(e34)(e35)(e36)(e37)(e38)(e39)(e40)(e41)(e42)
#	 define MACRO_PP_TUPLE_TO_SEQ_44(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)(e30)(e31)(e32)(e33)(e34)(e35)(e36)(e37)(e38)(e39)(e40)(e41)(e42)(e43)
#	 define MACRO_PP_TUPLE_TO_SEQ_45(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)(e30)(e31)(e32)(e33)(e34)(e35)(e36)(e37)(e38)(e39)(e40)(e41)(e42)(e43)(e44)
#	 define MACRO_PP_TUPLE_TO_SEQ_46(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)(e30)(e31)(e32)(e33)(e34)(e35)(e36)(e37)(e38)(e39)(e40)(e41)(e42)(e43)(e44)(e45)
#	 define MACRO_PP_TUPLE_TO_SEQ_47(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)(e30)(e31)(e32)(e33)(e34)(e35)(e36)(e37)(e38)(e39)(e40)(e41)(e42)(e43)(e44)(e45)(e46)
#	 define MACRO_PP_TUPLE_TO_SEQ_48(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)(e30)(e31)(e32)(e33)(e34)(e35)(e36)(e37)(e38)(e39)(e40)(e41)(e42)(e43)(e44)(e45)(e46)(e47)
#	 define MACRO_PP_TUPLE_TO_SEQ_49(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)(e30)(e31)(e32)(e33)(e34)(e35)(e36)(e37)(e38)(e39)(e40)(e41)(e42)(e43)(e44)(e45)(e46)(e47)(e48)
#	 define MACRO_PP_TUPLE_TO_SEQ_50(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)(e30)(e31)(e32)(e33)(e34)(e35)(e36)(e37)(e38)(e39)(e40)(e41)(e42)(e43)(e44)(e45)(e46)(e47)(e48)(e49)
#	 define MACRO_PP_TUPLE_TO_SEQ_51(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)(e30)(e31)(e32)(e33)(e34)(e35)(e36)(e37)(e38)(e39)(e40)(e41)(e42)(e43)(e44)(e45)(e46)(e47)(e48)(e49)(e50)
#	 define MACRO_PP_TUPLE_TO_SEQ_52(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)(e30)(e31)(e32)(e33)(e34)(e35)(e36)(e37)(e38)(e39)(e40)(e41)(e42)(e43)(e44)(e45)(e46)(e47)(e48)(e49)(e50)(e51)
#	 define MACRO_PP_TUPLE_TO_SEQ_53(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)(e30)(e31)(e32)(e33)(e34)(e35)(e36)(e37)(e38)(e39)(e40)(e41)(e42)(e43)(e44)(e45)(e46)(e47)(e48)(e49)(e50)(e51)(e52)
#	 define MACRO_PP_TUPLE_TO_SEQ_54(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)(e30)(e31)(e32)(e33)(e34)(e35)(e36)(e37)(e38)(e39)(e40)(e41)(e42)(e43)(e44)(e45)(e46)(e47)(e48)(e49)(e50)(e51)(e52)(e53)
#	 define MACRO_PP_TUPLE_TO_SEQ_55(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)(e30)(e31)(e32)(e33)(e34)(e35)(e36)(e37)(e38)(e39)(e40)(e41)(e42)(e43)(e44)(e45)(e46)(e47)(e48)(e49)(e50)(e51)(e52)(e53)(e54)
#	 define MACRO_PP_TUPLE_TO_SEQ_56(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)(e30)(e31)(e32)(e33)(e34)(e35)(e36)(e37)(e38)(e39)(e40)(e41)(e42)(e43)(e44)(e45)(e46)(e47)(e48)(e49)(e50)(e51)(e52)(e53)(e54)(e55)
#	 define MACRO_PP_TUPLE_TO_SEQ_57(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)(e30)(e31)(e32)(e33)(e34)(e35)(e36)(e37)(e38)(e39)(e40)(e41)(e42)(e43)(e44)(e45)(e46)(e47)(e48)(e49)(e50)(e51)(e52)(e53)(e54)(e55)(e56)
#	 define MACRO_PP_TUPLE_TO_SEQ_58(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)(e30)(e31)(e32)(e33)(e34)(e35)(e36)(e37)(e38)(e39)(e40)(e41)(e42)(e43)(e44)(e45)(e46)(e47)(e48)(e49)(e50)(e51)(e52)(e53)(e54)(e55)(e56)(e57)
#	 define MACRO_PP_TUPLE_TO_SEQ_59(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)(e30)(e31)(e32)(e33)(e34)(e35)(e36)(e37)(e38)(e39)(e40)(e41)(e42)(e43)(e44)(e45)(e46)(e47)(e48)(e49)(e50)(e51)(e52)(e53)(e54)(e55)(e56)(e57)(e58)
#	 define MACRO_PP_TUPLE_TO_SEQ_60(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)(e30)(e31)(e32)(e33)(e34)(e35)(e36)(e37)(e38)(e39)(e40)(e41)(e42)(e43)(e44)(e45)(e46)(e47)(e48)(e49)(e50)(e51)(e52)(e53)(e54)(e55)(e56)(e57)(e58)(e59)
#	 define MACRO_PP_TUPLE_TO_SEQ_61(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)(e30)(e31)(e32)(e33)(e34)(e35)(e36)(e37)(e38)(e39)(e40)(e41)(e42)(e43)(e44)(e45)(e46)(e47)(e48)(e49)(e50)(e51)(e52)(e53)(e54)(e55)(e56)(e57)(e58)(e59)(e60)
#	 define MACRO_PP_TUPLE_TO_SEQ_62(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)(e30)(e31)(e32)(e33)(e34)(e35)(e36)(e37)(e38)(e39)(e40)(e41)(e42)(e43)(e44)(e45)(e46)(e47)(e48)(e49)(e50)(e51)(e52)(e53)(e54)(e55)(e56)(e57)(e58)(e59)(e60)(e61)
#	 define MACRO_PP_TUPLE_TO_SEQ_63(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)(e30)(e31)(e32)(e33)(e34)(e35)(e36)(e37)(e38)(e39)(e40)(e41)(e42)(e43)(e44)(e45)(e46)(e47)(e48)(e49)(e50)(e51)(e52)(e53)(e54)(e55)(e56)(e57)(e58)(e59)(e60)(e61)(e62)
#	 define MACRO_PP_TUPLE_TO_SEQ_64(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) (e0)(e1)(e2)(e3)(e4)(e5)(e6)(e7)(e8)(e9)(e10)(e11)(e12)(e13)(e14)(e15)(e16)(e17)(e18)(e19)(e20)(e21)(e22)(e23)(e24)(e25)(e26)(e27)(e28)(e29)(e30)(e31)(e32)(e33)(e34)(e35)(e36)(e37)(e38)(e39)(e40)(e41)(e42)(e43)(e44)(e45)(e46)(e47)(e48)(e49)(e50)(e51)(e52)(e53)(e54)(e55)(e56)(e57)(e58)(e59)(e60)(e61)(e62)(e63)

#	 define MACRO_PP_SEQ_FOR_EACH_I(macro, data, seq)\
			MACRO_PP_FOR_1((macro, data, seq (nil), 0), MACRO_PP_SEQ_FOR_EACH_I_P, MACRO_PP_SEQ_FOR_EACH_I_O, MACRO_PP_SEQ_FOR_EACH_I_M)

#	 define MACRO_PP_SEQ_FOR_EACH_I_P(r, x)\
			MACRO_NUMBER_DEC(MACRO_PP_SEQ_SIZE(MACRO_PP_TUPLE_ELEM(4, 2, x)))

#    define MACRO_PP_SEQ_SIZE(seq)\
			MACRO_PP_CAT(MACRO_PP_SEQ_SIZE_, MACRO_PP_SEQ_SIZE_0 seq)

#	 define MACRO_PP_SEQ_SIZE_0(_) MACRO_PP_SEQ_SIZE_1
#	 define MACRO_PP_SEQ_SIZE_1(_) MACRO_PP_SEQ_SIZE_2
#	 define MACRO_PP_SEQ_SIZE_2(_) MACRO_PP_SEQ_SIZE_3
#	 define MACRO_PP_SEQ_SIZE_3(_) MACRO_PP_SEQ_SIZE_4
#	 define MACRO_PP_SEQ_SIZE_4(_) MACRO_PP_SEQ_SIZE_5
#	 define MACRO_PP_SEQ_SIZE_5(_) MACRO_PP_SEQ_SIZE_6
#	 define MACRO_PP_SEQ_SIZE_6(_) MACRO_PP_SEQ_SIZE_7
#	 define MACRO_PP_SEQ_SIZE_7(_) MACRO_PP_SEQ_SIZE_8
#	 define MACRO_PP_SEQ_SIZE_8(_) MACRO_PP_SEQ_SIZE_9
#	 define MACRO_PP_SEQ_SIZE_9(_) MACRO_PP_SEQ_SIZE_10
#	 define MACRO_PP_SEQ_SIZE_10(_) MACRO_PP_SEQ_SIZE_11
#	 define MACRO_PP_SEQ_SIZE_11(_) MACRO_PP_SEQ_SIZE_12
#	 define MACRO_PP_SEQ_SIZE_12(_) MACRO_PP_SEQ_SIZE_13
#	 define MACRO_PP_SEQ_SIZE_13(_) MACRO_PP_SEQ_SIZE_14
#	 define MACRO_PP_SEQ_SIZE_14(_) MACRO_PP_SEQ_SIZE_15
#	 define MACRO_PP_SEQ_SIZE_15(_) MACRO_PP_SEQ_SIZE_16
#	 define MACRO_PP_SEQ_SIZE_16(_) MACRO_PP_SEQ_SIZE_17
#	 define MACRO_PP_SEQ_SIZE_17(_) MACRO_PP_SEQ_SIZE_18
#	 define MACRO_PP_SEQ_SIZE_18(_) MACRO_PP_SEQ_SIZE_19
#	 define MACRO_PP_SEQ_SIZE_19(_) MACRO_PP_SEQ_SIZE_20
#	 define MACRO_PP_SEQ_SIZE_20(_) MACRO_PP_SEQ_SIZE_21
#	 define MACRO_PP_SEQ_SIZE_21(_) MACRO_PP_SEQ_SIZE_22
#	 define MACRO_PP_SEQ_SIZE_22(_) MACRO_PP_SEQ_SIZE_23
#	 define MACRO_PP_SEQ_SIZE_23(_) MACRO_PP_SEQ_SIZE_24
#	 define MACRO_PP_SEQ_SIZE_24(_) MACRO_PP_SEQ_SIZE_25
#	 define MACRO_PP_SEQ_SIZE_25(_) MACRO_PP_SEQ_SIZE_26
#	 define MACRO_PP_SEQ_SIZE_26(_) MACRO_PP_SEQ_SIZE_27
#	 define MACRO_PP_SEQ_SIZE_27(_) MACRO_PP_SEQ_SIZE_28
#	 define MACRO_PP_SEQ_SIZE_28(_) MACRO_PP_SEQ_SIZE_29
#	 define MACRO_PP_SEQ_SIZE_29(_) MACRO_PP_SEQ_SIZE_30
#	 define MACRO_PP_SEQ_SIZE_30(_) MACRO_PP_SEQ_SIZE_31
#	 define MACRO_PP_SEQ_SIZE_31(_) MACRO_PP_SEQ_SIZE_32
#	 define MACRO_PP_SEQ_SIZE_32(_) MACRO_PP_SEQ_SIZE_33
#	 define MACRO_PP_SEQ_SIZE_33(_) MACRO_PP_SEQ_SIZE_34
#	 define MACRO_PP_SEQ_SIZE_34(_) MACRO_PP_SEQ_SIZE_35
#	 define MACRO_PP_SEQ_SIZE_35(_) MACRO_PP_SEQ_SIZE_36
#	 define MACRO_PP_SEQ_SIZE_36(_) MACRO_PP_SEQ_SIZE_37
#	 define MACRO_PP_SEQ_SIZE_37(_) MACRO_PP_SEQ_SIZE_38
#	 define MACRO_PP_SEQ_SIZE_38(_) MACRO_PP_SEQ_SIZE_39
#	 define MACRO_PP_SEQ_SIZE_39(_) MACRO_PP_SEQ_SIZE_40
#	 define MACRO_PP_SEQ_SIZE_40(_) MACRO_PP_SEQ_SIZE_41
#	 define MACRO_PP_SEQ_SIZE_41(_) MACRO_PP_SEQ_SIZE_42
#	 define MACRO_PP_SEQ_SIZE_42(_) MACRO_PP_SEQ_SIZE_43
#	 define MACRO_PP_SEQ_SIZE_43(_) MACRO_PP_SEQ_SIZE_44
#	 define MACRO_PP_SEQ_SIZE_44(_) MACRO_PP_SEQ_SIZE_45
#	 define MACRO_PP_SEQ_SIZE_45(_) MACRO_PP_SEQ_SIZE_46
#	 define MACRO_PP_SEQ_SIZE_46(_) MACRO_PP_SEQ_SIZE_47
#	 define MACRO_PP_SEQ_SIZE_47(_) MACRO_PP_SEQ_SIZE_48
#	 define MACRO_PP_SEQ_SIZE_48(_) MACRO_PP_SEQ_SIZE_49
#	 define MACRO_PP_SEQ_SIZE_49(_) MACRO_PP_SEQ_SIZE_50
#	 define MACRO_PP_SEQ_SIZE_50(_) MACRO_PP_SEQ_SIZE_51
#	 define MACRO_PP_SEQ_SIZE_51(_) MACRO_PP_SEQ_SIZE_52
#	 define MACRO_PP_SEQ_SIZE_52(_) MACRO_PP_SEQ_SIZE_53
#	 define MACRO_PP_SEQ_SIZE_53(_) MACRO_PP_SEQ_SIZE_54
#	 define MACRO_PP_SEQ_SIZE_54(_) MACRO_PP_SEQ_SIZE_55
#	 define MACRO_PP_SEQ_SIZE_55(_) MACRO_PP_SEQ_SIZE_56
#	 define MACRO_PP_SEQ_SIZE_56(_) MACRO_PP_SEQ_SIZE_57
#	 define MACRO_PP_SEQ_SIZE_57(_) MACRO_PP_SEQ_SIZE_58
#	 define MACRO_PP_SEQ_SIZE_58(_) MACRO_PP_SEQ_SIZE_59
#	 define MACRO_PP_SEQ_SIZE_59(_) MACRO_PP_SEQ_SIZE_60
#	 define MACRO_PP_SEQ_SIZE_60(_) MACRO_PP_SEQ_SIZE_61
#	 define MACRO_PP_SEQ_SIZE_61(_) MACRO_PP_SEQ_SIZE_62
#	 define MACRO_PP_SEQ_SIZE_62(_) MACRO_PP_SEQ_SIZE_63
#	 define MACRO_PP_SEQ_SIZE_63(_) MACRO_PP_SEQ_SIZE_64
#	 define MACRO_PP_SEQ_SIZE_64(_) MACRO_PP_SEQ_SIZE_65
#
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_0 0
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_1 1
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_2 2
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_3 3
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_4 4
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_5 5
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_6 6
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_7 7
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_8 8
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_9 9
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_10 10
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_11 11
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_12 12
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_13 13
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_14 14
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_15 15
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_16 16
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_17 17
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_18 18
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_19 19
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_20 20
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_21 21
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_22 22
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_23 23
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_24 24
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_25 25
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_26 26
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_27 27
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_28 28
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_29 29
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_30 30
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_31 31
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_32 32
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_33 33
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_34 34
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_35 35
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_36 36
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_37 37
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_38 38
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_39 39
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_40 40
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_41 41
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_42 42
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_43 43
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_44 44
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_45 45
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_46 46
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_47 47
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_48 48
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_49 49
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_50 50
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_51 51
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_52 52
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_53 53
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_54 54
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_55 55
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_56 56
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_57 57
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_58 58
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_59 59
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_60 60
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_61 61
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_62 62
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_63 63
#	 define MACRO_PP_SEQ_SIZE_MACRO_PP_SEQ_SIZE_64 64


#	 define MACRO_PP_SEQ_FOR_EACH_I_O(r, x)\
			MACRO_PP_SEQ_FOR_EACH_I_O_I(MACRO_PP_TUPLE_ELEM(4, 0, x), MACRO_PP_TUPLE_ELEM(4, 1, x), MACRO_PP_TUPLE_ELEM(4, 2, x), MACRO_PP_TUPLE_ELEM(4, 3, x))

#	 define MACRO_PP_SEQ_FOR_EACH_I_O_I(macro, data, seq, i)\
			(macro, data, MACRO_PP_SEQ_TAIL(seq), MACRO_NUMBER_INC(i))

#	 define MACRO_PP_SEQ_TAIL(seq)\
			MACRO_PP_SEQ_TAIL_ID(MACRO_PP_SEQ_TAIL_I seq)

#    define MACRO_PP_SEQ_TAIL_ID(id)			id
#	 define MACRO_PP_SEQ_TAIL_I(x)

#    define MACRO_PP_SEQ_FOR_EACH_I_M(r, x)\
			MACRO_PP_SEQ_FOR_EACH_I_M_I(r, MACRO_PP_TUPLE_ELEM(4, 0, x), MACRO_PP_TUPLE_ELEM(4, 1, x), MACRO_PP_TUPLE_ELEM(4, 2, x), MACRO_PP_TUPLE_ELEM(4, 3, x))

#	 define MACRO_PP_SEQ_FOR_EACH_I_M_I(r, macro, data, seq, i) \
			MACRO_PP_SEQ_FOR_EACH_I_M_II(r, macro, data, MACRO_PP_CAT(MARCO_PP_SEQ_HEAD(seq),), i)

/*#if defined(PLATFORM_OS_FAMILY_WINDOWS)
	#	 define	MARCO_PP_SEQ_HEAD(seq)\
				MARCO_PP_SEQ_HEAD_II(MACRO_PP_CAT(MARCO_PP_SEQ_HEAD_I,(MARCO_PP_SEQ_ELEM_0 seq)))
# elif defined(PLATFORM_OS_FAMILY_UNIX)*/
#	 define	MARCO_PP_SEQ_HEAD(seq)\
			MARCO_PP_SEQ_HEAD_IV(MARCO_PP_SEQ_ELEM_0 seq)

#	 define MARCO_PP_SEQ_HEAD_IV(seq)\
			MARCO_PP_SEQ_HEAD_II(MARCO_PP_SEQ_HEAD_I(seq))
//# endif

#	 define	MARCO_PP_SEQ_HEAD_I(x,_)	x
#	 define MARCO_PP_SEQ_HEAD_II(x)		MACRO_PP_CAT(x,)

#	 define MACRO_PP_SEQ_FOR_EACH_I_M_II(r, macro, data, seq, i) \
			macro(r, data, i, seq )

#	 define MARCO_PP_SEQ_ELEM_0(x) x, nil
#	 define MARCO_PP_SEQ_ELEM_1(_) MARCO_PP_SEQ_ELEM_0
#	 define MARCO_PP_SEQ_ELEM_2(_) MARCO_PP_SEQ_ELEM_1
#	 define MARCO_PP_SEQ_ELEM_3(_) MARCO_PP_SEQ_ELEM_2
#	 define MARCO_PP_SEQ_ELEM_4(_) MARCO_PP_SEQ_ELEM_3
#	 define MARCO_PP_SEQ_ELEM_5(_) MARCO_PP_SEQ_ELEM_4
#	 define MARCO_PP_SEQ_ELEM_6(_) MARCO_PP_SEQ_ELEM_5
#	 define MARCO_PP_SEQ_ELEM_7(_) MARCO_PP_SEQ_ELEM_6
#	 define MARCO_PP_SEQ_ELEM_8(_) MARCO_PP_SEQ_ELEM_7
#	 define MARCO_PP_SEQ_ELEM_9(_) MARCO_PP_SEQ_ELEM_8
#	 define MARCO_PP_SEQ_ELEM_10(_) MARCO_PP_SEQ_ELEM_9
#	 define MARCO_PP_SEQ_ELEM_11(_) MARCO_PP_SEQ_ELEM_10
#	 define MARCO_PP_SEQ_ELEM_12(_) MARCO_PP_SEQ_ELEM_11
#	 define MARCO_PP_SEQ_ELEM_13(_) MARCO_PP_SEQ_ELEM_12
#	 define MARCO_PP_SEQ_ELEM_14(_) MARCO_PP_SEQ_ELEM_13
#	 define MARCO_PP_SEQ_ELEM_15(_) MARCO_PP_SEQ_ELEM_14
# 	 define MARCO_PP_SEQ_ELEM_16(_) MARCO_PP_SEQ_ELEM_15
# 	 define MARCO_PP_SEQ_ELEM_17(_) MARCO_PP_SEQ_ELEM_16
#	 define MARCO_PP_SEQ_ELEM_18(_) MARCO_PP_SEQ_ELEM_17
#	 define MARCO_PP_SEQ_ELEM_19(_) MARCO_PP_SEQ_ELEM_18
#	 define MARCO_PP_SEQ_ELEM_20(_) MARCO_PP_SEQ_ELEM_19
#	 define MARCO_PP_SEQ_ELEM_21(_) MARCO_PP_SEQ_ELEM_20
#	 define MARCO_PP_SEQ_ELEM_22(_) MARCO_PP_SEQ_ELEM_21
#	 define MARCO_PP_SEQ_ELEM_23(_) MARCO_PP_SEQ_ELEM_22
#	 define MARCO_PP_SEQ_ELEM_24(_) MARCO_PP_SEQ_ELEM_23
#	 define MARCO_PP_SEQ_ELEM_25(_) MARCO_PP_SEQ_ELEM_24
#	 define MARCO_PP_SEQ_ELEM_26(_) MARCO_PP_SEQ_ELEM_25
#	 define MARCO_PP_SEQ_ELEM_27(_) MARCO_PP_SEQ_ELEM_26
#	 define MARCO_PP_SEQ_ELEM_28(_) MARCO_PP_SEQ_ELEM_27
#	 define MARCO_PP_SEQ_ELEM_29(_) MARCO_PP_SEQ_ELEM_28
#	 define MARCO_PP_SEQ_ELEM_30(_) MARCO_PP_SEQ_ELEM_29
#	 define MARCO_PP_SEQ_ELEM_31(_) MARCO_PP_SEQ_ELEM_30
#	 define MARCO_PP_SEQ_ELEM_32(_) MARCO_PP_SEQ_ELEM_31
#	 define MARCO_PP_SEQ_ELEM_33(_) MARCO_PP_SEQ_ELEM_32
#	 define MARCO_PP_SEQ_ELEM_34(_) MARCO_PP_SEQ_ELEM_33
#	 define MARCO_PP_SEQ_ELEM_35(_) MARCO_PP_SEQ_ELEM_34
#	 define MARCO_PP_SEQ_ELEM_36(_) MARCO_PP_SEQ_ELEM_35
#	 define MARCO_PP_SEQ_ELEM_37(_) MARCO_PP_SEQ_ELEM_36
#	 define MARCO_PP_SEQ_ELEM_38(_) MARCO_PP_SEQ_ELEM_37
#	 define MARCO_PP_SEQ_ELEM_39(_) MARCO_PP_SEQ_ELEM_38
#	 define MARCO_PP_SEQ_ELEM_40(_) MARCO_PP_SEQ_ELEM_39
#	 define MARCO_PP_SEQ_ELEM_41(_) MARCO_PP_SEQ_ELEM_40
#	 define MARCO_PP_SEQ_ELEM_42(_) MARCO_PP_SEQ_ELEM_41
#	 define MARCO_PP_SEQ_ELEM_43(_) MARCO_PP_SEQ_ELEM_42
#	 define MARCO_PP_SEQ_ELEM_44(_) MARCO_PP_SEQ_ELEM_43
#	 define MARCO_PP_SEQ_ELEM_45(_) MARCO_PP_SEQ_ELEM_44
#	 define MARCO_PP_SEQ_ELEM_46(_) MARCO_PP_SEQ_ELEM_45
#	 define MARCO_PP_SEQ_ELEM_47(_) MARCO_PP_SEQ_ELEM_46
#	 define MARCO_PP_SEQ_ELEM_48(_) MARCO_PP_SEQ_ELEM_47
#	 define MARCO_PP_SEQ_ELEM_49(_) MARCO_PP_SEQ_ELEM_48
#	 define MARCO_PP_SEQ_ELEM_50(_) MARCO_PP_SEQ_ELEM_49
#	 define MARCO_PP_SEQ_ELEM_51(_) MARCO_PP_SEQ_ELEM_50
#	 define MARCO_PP_SEQ_ELEM_52(_) MARCO_PP_SEQ_ELEM_51
#	 define MARCO_PP_SEQ_ELEM_53(_) MARCO_PP_SEQ_ELEM_52
#	 define MARCO_PP_SEQ_ELEM_54(_) MARCO_PP_SEQ_ELEM_53
#	 define MARCO_PP_SEQ_ELEM_55(_) MARCO_PP_SEQ_ELEM_54
#	 define MARCO_PP_SEQ_ELEM_56(_) MARCO_PP_SEQ_ELEM_55
#	 define MARCO_PP_SEQ_ELEM_57(_) MARCO_PP_SEQ_ELEM_56
#	 define MARCO_PP_SEQ_ELEM_58(_) MARCO_PP_SEQ_ELEM_57
#	 define MARCO_PP_SEQ_ELEM_59(_) MARCO_PP_SEQ_ELEM_58
#	 define MARCO_PP_SEQ_ELEM_60(_) MARCO_PP_SEQ_ELEM_59
#	 define MARCO_PP_SEQ_ELEM_61(_) MARCO_PP_SEQ_ELEM_60
#	 define MARCO_PP_SEQ_ELEM_62(_) MARCO_PP_SEQ_ELEM_61
#	 define MARCO_PP_SEQ_ELEM_63(_) MARCO_PP_SEQ_ELEM_62
#	 define MARCO_PP_SEQ_ELEM_64(_) MARCO_PP_SEQ_ELEM_63
#	 define MARCO_PP_SEQ_ELEM_65(_) MARCO_PP_SEQ_ELEM_64
#	 define MARCO_PP_SEQ_ELEM_66(_) MARCO_PP_SEQ_ELEM_65


#	 define MACRO_PP_SEQ_ELEM(e0) e0

# if defined(_WIN32) || defined(_WIN64)
	#	 define	MACRO_PP_TUPLE_ELEM(...)\
				MACRO_PP_TUPLE_ELEM_I(MACRO_PP_OVERLOAD(MACRO_PP_TUPLE_ELEM_O_,__VA_ARGS__),(__VA_ARGS__))
	#    define MACRO_PP_TUPLE_ELEM_I(m, args) MACRO_PP_TUPLE_ELEM_II(m, args)
	#    define MACRO_PP_TUPLE_ELEM_II(m, args) MACRO_PP_CAT(m ## args,)
# elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__TOS_LINUX__)
	#	 define	MACRO_PP_TUPLE_ELEM(...)\
				MACRO_PP_OVERLOAD(MACRO_PP_TUPLE_ELEM_O_,__VA_ARGS__)(__VA_ARGS__)
#endif

#    define MACRO_PP_TUPLE_ELEM_O_3(size, n, tuple)\
			MACRO_PP_TUPLE_ELEM_O_2(n, tuple)

#    define MACRO_PP_TUPLE_ELEM_O_2(n, tuple)\
			MACRO_PP_VARIADIC_ELEM(n, tuple)

#	 define MACRO_PP_VARIADIC_ELEM(n, tuple)\
			MACRO_PP_CAT(MACRO_PP_CAT(MACRO_PP_VARIADIC_ELEM_,n) tuple,)

#    define MACRO_PP_VARIADIC_ELEM_0(e0, ...) e0
#    define MACRO_PP_VARIADIC_ELEM_1(e0, e1, ...) e1
#    define MACRO_PP_VARIADIC_ELEM_2(e0, e1, e2, ...) e2
#    define MACRO_PP_VARIADIC_ELEM_3(e0, e1, e2, e3, ...) e3
#    define MACRO_PP_VARIADIC_ELEM_4(e0, e1, e2, e3, e4, ...) e4
#    define MACRO_PP_VARIADIC_ELEM_5(e0, e1, e2, e3, e4, e5, ...) e5
#    define MACRO_PP_VARIADIC_ELEM_6(e0, e1, e2, e3, e4, e5, e6, ...) e6
#    define MACRO_PP_VARIADIC_ELEM_7(e0, e1, e2, e3, e4, e5, e6, e7, ...) e7
#    define MACRO_PP_VARIADIC_ELEM_8(e0, e1, e2, e3, e4, e5, e6, e7, e8, ...) e8
#    define MACRO_PP_VARIADIC_ELEM_9(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, ...) e9
#    define MACRO_PP_VARIADIC_ELEM_10(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, ...) e10
#    define MACRO_PP_VARIADIC_ELEM_11(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, ...) e11
#    define MACRO_PP_VARIADIC_ELEM_12(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, ...) e12
#    define MACRO_PP_VARIADIC_ELEM_13(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, ...) e13
#    define MACRO_PP_VARIADIC_ELEM_14(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, ...) e14
#    define MACRO_PP_VARIADIC_ELEM_15(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, ...) e15
#    define MACRO_PP_VARIADIC_ELEM_16(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, ...) e16
#    define MACRO_PP_VARIADIC_ELEM_17(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, ...) e17
#    define MACRO_PP_VARIADIC_ELEM_18(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, ...) e18
#    define MACRO_PP_VARIADIC_ELEM_19(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, ...) e19
#    define MACRO_PP_VARIADIC_ELEM_20(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, ...) e20
#    define MACRO_PP_VARIADIC_ELEM_21(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, ...) e21
#    define MACRO_PP_VARIADIC_ELEM_22(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, ...) e22
#    define MACRO_PP_VARIADIC_ELEM_23(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, ...) e23
#    define MACRO_PP_VARIADIC_ELEM_24(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, ...) e24
#    define MACRO_PP_VARIADIC_ELEM_25(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, ...) e25
#    define MACRO_PP_VARIADIC_ELEM_26(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, ...) e26
#    define MACRO_PP_VARIADIC_ELEM_27(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, ...) e27
#    define MACRO_PP_VARIADIC_ELEM_28(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, ...) e28
#    define MACRO_PP_VARIADIC_ELEM_29(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, ...) e29
#    define MACRO_PP_VARIADIC_ELEM_30(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, ...) e30
#    define MACRO_PP_VARIADIC_ELEM_31(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, ...) e31
#    define MACRO_PP_VARIADIC_ELEM_32(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, ...) e32
#    define MACRO_PP_VARIADIC_ELEM_33(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, ...) e33
#    define MACRO_PP_VARIADIC_ELEM_34(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, ...) e34
#    define MACRO_PP_VARIADIC_ELEM_35(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, ...) e35
#    define MACRO_PP_VARIADIC_ELEM_36(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, ...) e36
#    define MACRO_PP_VARIADIC_ELEM_37(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, ...) e37
#    define MACRO_PP_VARIADIC_ELEM_38(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, ...) e38
#    define MACRO_PP_VARIADIC_ELEM_39(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, ...) e39
#    define MACRO_PP_VARIADIC_ELEM_40(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, ...) e40
#    define MACRO_PP_VARIADIC_ELEM_41(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, ...) e41
#    define MACRO_PP_VARIADIC_ELEM_42(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, ...) e42
#    define MACRO_PP_VARIADIC_ELEM_43(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, ...) e43
#    define MACRO_PP_VARIADIC_ELEM_44(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, ...) e44
#    define MACRO_PP_VARIADIC_ELEM_45(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, ...) e45
#    define MACRO_PP_VARIADIC_ELEM_46(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, ...) e46
#    define MACRO_PP_VARIADIC_ELEM_47(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, ...) e47
#    define MACRO_PP_VARIADIC_ELEM_48(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, ...) e48
#    define MACRO_PP_VARIADIC_ELEM_49(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, ...) e49
#    define MACRO_PP_VARIADIC_ELEM_50(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, ...) e50
#    define MACRO_PP_VARIADIC_ELEM_51(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, ...) e51
#    define MACRO_PP_VARIADIC_ELEM_52(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, ...) e52
#    define MACRO_PP_VARIADIC_ELEM_53(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, ...) e53
#    define MACRO_PP_VARIADIC_ELEM_54(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, ...) e54
#    define MACRO_PP_VARIADIC_ELEM_55(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, ...) e55
#    define MACRO_PP_VARIADIC_ELEM_56(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, ...) e56
#    define MACRO_PP_VARIADIC_ELEM_57(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, ...) e57
#    define MACRO_PP_VARIADIC_ELEM_58(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, ...) e58
#    define MACRO_PP_VARIADIC_ELEM_59(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, ...) e59
#    define MACRO_PP_VARIADIC_ELEM_60(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, ...) e60
#    define MACRO_PP_VARIADIC_ELEM_61(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, ...) e61
#    define MACRO_PP_VARIADIC_ELEM_62(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, ...) e62
#	 define MACRO_PP_VARIADIC_ELEM_63(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63, ...) e63


#	 define MACRO_PP_FOR_1(s, p, o, m) MACRO_PP_IF(p(2, s), m, MACRO_PP_TUPLE_EAT_2)(2, s) MACRO_PP_IF(p(2, s), MACRO_PP_FOR_2, MACRO_PP_TUPLE_EAT_4)(o(2, s), p, o, m)
#	 define MACRO_PP_FOR_2(s, p, o, m) MACRO_PP_IF(p(3, s), m, MACRO_PP_TUPLE_EAT_2)(3, s) MACRO_PP_IF(p(3, s), MACRO_PP_FOR_3, MACRO_PP_TUPLE_EAT_4)(o(3, s), p, o, m)
#	 define MACRO_PP_FOR_3(s, p, o, m) MACRO_PP_IF(p(4, s), m, MACRO_PP_TUPLE_EAT_2)(4, s) MACRO_PP_IF(p(4, s), MACRO_PP_FOR_4, MACRO_PP_TUPLE_EAT_4)(o(4, s), p, o, m)
#	 define MACRO_PP_FOR_4(s, p, o, m) MACRO_PP_IF(p(5, s), m, MACRO_PP_TUPLE_EAT_2)(5, s) MACRO_PP_IF(p(5, s), MACRO_PP_FOR_5, MACRO_PP_TUPLE_EAT_4)(o(5, s), p, o, m)
#	 define MACRO_PP_FOR_5(s, p, o, m) MACRO_PP_IF(p(6, s), m, MACRO_PP_TUPLE_EAT_2)(6, s) MACRO_PP_IF(p(6, s), MACRO_PP_FOR_6, MACRO_PP_TUPLE_EAT_4)(o(6, s), p, o, m)
#	 define MACRO_PP_FOR_6(s, p, o, m) MACRO_PP_IF(p(7, s), m, MACRO_PP_TUPLE_EAT_2)(7, s) MACRO_PP_IF(p(7, s), MACRO_PP_FOR_7, MACRO_PP_TUPLE_EAT_4)(o(7, s), p, o, m)
#	 define MACRO_PP_FOR_7(s, p, o, m) MACRO_PP_IF(p(8, s), m, MACRO_PP_TUPLE_EAT_2)(8, s) MACRO_PP_IF(p(8, s), MACRO_PP_FOR_8, MACRO_PP_TUPLE_EAT_4)(o(8, s), p, o, m)
#	 define MACRO_PP_FOR_8(s, p, o, m) MACRO_PP_IF(p(9, s), m, MACRO_PP_TUPLE_EAT_2)(9, s) MACRO_PP_IF(p(9, s), MACRO_PP_FOR_9, MACRO_PP_TUPLE_EAT_4)(o(9, s), p, o, m)
#	 define MACRO_PP_FOR_9(s, p, o, m) MACRO_PP_IF(p(10, s), m, MACRO_PP_TUPLE_EAT_2)(10, s) MACRO_PP_IF(p(10, s), MACRO_PP_FOR_10, MACRO_PP_TUPLE_EAT_4)(o(10, s), p, o, m)
#	 define MACRO_PP_FOR_10(s, p, o, m) MACRO_PP_IF(p(11, s), m, MACRO_PP_TUPLE_EAT_2)(11, s) MACRO_PP_IF(p(11, s), MACRO_PP_FOR_11, MACRO_PP_TUPLE_EAT_4)(o(11, s), p, o, m)
#	 define MACRO_PP_FOR_11(s, p, o, m) MACRO_PP_IF(p(12, s), m, MACRO_PP_TUPLE_EAT_2)(12, s) MACRO_PP_IF(p(12, s), MACRO_PP_FOR_12, MACRO_PP_TUPLE_EAT_4)(o(12, s), p, o, m)
#	 define MACRO_PP_FOR_12(s, p, o, m) MACRO_PP_IF(p(13, s), m, MACRO_PP_TUPLE_EAT_2)(13, s) MACRO_PP_IF(p(13, s), MACRO_PP_FOR_13, MACRO_PP_TUPLE_EAT_4)(o(13, s), p, o, m)
#	 define MACRO_PP_FOR_13(s, p, o, m) MACRO_PP_IF(p(14, s), m, MACRO_PP_TUPLE_EAT_2)(14, s) MACRO_PP_IF(p(14, s), MACRO_PP_FOR_14, MACRO_PP_TUPLE_EAT_4)(o(14, s), p, o, m)
#	 define MACRO_PP_FOR_14(s, p, o, m) MACRO_PP_IF(p(15, s), m, MACRO_PP_TUPLE_EAT_2)(15, s) MACRO_PP_IF(p(15, s), MACRO_PP_FOR_15, MACRO_PP_TUPLE_EAT_4)(o(15, s), p, o, m)
#	 define MACRO_PP_FOR_15(s, p, o, m) MACRO_PP_IF(p(16, s), m, MACRO_PP_TUPLE_EAT_2)(16, s) MACRO_PP_IF(p(16, s), MACRO_PP_FOR_16, MACRO_PP_TUPLE_EAT_4)(o(16, s), p, o, m)
#	 define MACRO_PP_FOR_16(s, p, o, m) MACRO_PP_IF(p(17, s), m, MACRO_PP_TUPLE_EAT_2)(17, s) MACRO_PP_IF(p(17, s), MACRO_PP_FOR_17, MACRO_PP_TUPLE_EAT_4)(o(17, s), p, o, m)
#	 define MACRO_PP_FOR_17(s, p, o, m) MACRO_PP_IF(p(18, s), m, MACRO_PP_TUPLE_EAT_2)(18, s) MACRO_PP_IF(p(18, s), MACRO_PP_FOR_18, MACRO_PP_TUPLE_EAT_4)(o(18, s), p, o, m)
#	 define MACRO_PP_FOR_18(s, p, o, m) MACRO_PP_IF(p(19, s), m, MACRO_PP_TUPLE_EAT_2)(19, s) MACRO_PP_IF(p(19, s), MACRO_PP_FOR_19, MACRO_PP_TUPLE_EAT_4)(o(19, s), p, o, m)
#	 define MACRO_PP_FOR_19(s, p, o, m) MACRO_PP_IF(p(20, s), m, MACRO_PP_TUPLE_EAT_2)(20, s) MACRO_PP_IF(p(20, s), MACRO_PP_FOR_20, MACRO_PP_TUPLE_EAT_4)(o(20, s), p, o, m)
#	 define MACRO_PP_FOR_20(s, p, o, m) MACRO_PP_IF(p(21, s), m, MACRO_PP_TUPLE_EAT_2)(21, s) MACRO_PP_IF(p(21, s), MACRO_PP_FOR_21, MACRO_PP_TUPLE_EAT_4)(o(21, s), p, o, m)
#	 define MACRO_PP_FOR_21(s, p, o, m) MACRO_PP_IF(p(22, s), m, MACRO_PP_TUPLE_EAT_2)(22, s) MACRO_PP_IF(p(22, s), MACRO_PP_FOR_22, MACRO_PP_TUPLE_EAT_4)(o(22, s), p, o, m)
#	 define MACRO_PP_FOR_22(s, p, o, m) MACRO_PP_IF(p(23, s), m, MACRO_PP_TUPLE_EAT_2)(23, s) MACRO_PP_IF(p(23, s), MACRO_PP_FOR_23, MACRO_PP_TUPLE_EAT_4)(o(23, s), p, o, m)
#	 define MACRO_PP_FOR_23(s, p, o, m) MACRO_PP_IF(p(24, s), m, MACRO_PP_TUPLE_EAT_2)(24, s) MACRO_PP_IF(p(24, s), MACRO_PP_FOR_24, MACRO_PP_TUPLE_EAT_4)(o(24, s), p, o, m)
#	 define MACRO_PP_FOR_24(s, p, o, m) MACRO_PP_IF(p(25, s), m, MACRO_PP_TUPLE_EAT_2)(25, s) MACRO_PP_IF(p(25, s), MACRO_PP_FOR_25, MACRO_PP_TUPLE_EAT_4)(o(25, s), p, o, m)
#	 define MACRO_PP_FOR_25(s, p, o, m) MACRO_PP_IF(p(26, s), m, MACRO_PP_TUPLE_EAT_2)(26, s) MACRO_PP_IF(p(26, s), MACRO_PP_FOR_26, MACRO_PP_TUPLE_EAT_4)(o(26, s), p, o, m)
#	 define MACRO_PP_FOR_26(s, p, o, m) MACRO_PP_IF(p(27, s), m, MACRO_PP_TUPLE_EAT_2)(27, s) MACRO_PP_IF(p(27, s), MACRO_PP_FOR_27, MACRO_PP_TUPLE_EAT_4)(o(27, s), p, o, m)
#	 define MACRO_PP_FOR_27(s, p, o, m) MACRO_PP_IF(p(28, s), m, MACRO_PP_TUPLE_EAT_2)(28, s) MACRO_PP_IF(p(28, s), MACRO_PP_FOR_28, MACRO_PP_TUPLE_EAT_4)(o(28, s), p, o, m)
#	 define MACRO_PP_FOR_28(s, p, o, m) MACRO_PP_IF(p(29, s), m, MACRO_PP_TUPLE_EAT_2)(29, s) MACRO_PP_IF(p(29, s), MACRO_PP_FOR_29, MACRO_PP_TUPLE_EAT_4)(o(29, s), p, o, m)
#	 define MACRO_PP_FOR_29(s, p, o, m) MACRO_PP_IF(p(30, s), m, MACRO_PP_TUPLE_EAT_2)(30, s) MACRO_PP_IF(p(30, s), MACRO_PP_FOR_30, MACRO_PP_TUPLE_EAT_4)(o(30, s), p, o, m)
#	 define MACRO_PP_FOR_30(s, p, o, m) MACRO_PP_IF(p(31, s), m, MACRO_PP_TUPLE_EAT_2)(31, s) MACRO_PP_IF(p(31, s), MACRO_PP_FOR_31, MACRO_PP_TUPLE_EAT_4)(o(31, s), p, o, m)
#	 define MACRO_PP_FOR_31(s, p, o, m) MACRO_PP_IF(p(32, s), m, MACRO_PP_TUPLE_EAT_2)(32, s) MACRO_PP_IF(p(32, s), MACRO_PP_FOR_32, MACRO_PP_TUPLE_EAT_4)(o(32, s), p, o, m)
#	 define MACRO_PP_FOR_32(s, p, o, m) MACRO_PP_IF(p(33, s), m, MACRO_PP_TUPLE_EAT_2)(33, s) MACRO_PP_IF(p(33, s), MACRO_PP_FOR_33, MACRO_PP_TUPLE_EAT_4)(o(33, s), p, o, m)
#	 define MACRO_PP_FOR_33(s, p, o, m) MACRO_PP_IF(p(34, s), m, MACRO_PP_TUPLE_EAT_2)(34, s) MACRO_PP_IF(p(34, s), MACRO_PP_FOR_34, MACRO_PP_TUPLE_EAT_4)(o(34, s), p, o, m)
#	 define MACRO_PP_FOR_34(s, p, o, m) MACRO_PP_IF(p(35, s), m, MACRO_PP_TUPLE_EAT_2)(35, s) MACRO_PP_IF(p(35, s), MACRO_PP_FOR_35, MACRO_PP_TUPLE_EAT_4)(o(35, s), p, o, m)
#	 define MACRO_PP_FOR_35(s, p, o, m) MACRO_PP_IF(p(36, s), m, MACRO_PP_TUPLE_EAT_2)(36, s) MACRO_PP_IF(p(36, s), MACRO_PP_FOR_36, MACRO_PP_TUPLE_EAT_4)(o(36, s), p, o, m)
#	 define MACRO_PP_FOR_36(s, p, o, m) MACRO_PP_IF(p(37, s), m, MACRO_PP_TUPLE_EAT_2)(37, s) MACRO_PP_IF(p(37, s), MACRO_PP_FOR_37, MACRO_PP_TUPLE_EAT_4)(o(37, s), p, o, m)
#	 define MACRO_PP_FOR_37(s, p, o, m) MACRO_PP_IF(p(38, s), m, MACRO_PP_TUPLE_EAT_2)(38, s) MACRO_PP_IF(p(38, s), MACRO_PP_FOR_38, MACRO_PP_TUPLE_EAT_4)(o(38, s), p, o, m)
#	 define MACRO_PP_FOR_38(s, p, o, m) MACRO_PP_IF(p(39, s), m, MACRO_PP_TUPLE_EAT_2)(39, s) MACRO_PP_IF(p(39, s), MACRO_PP_FOR_39, MACRO_PP_TUPLE_EAT_4)(o(39, s), p, o, m)
#	 define MACRO_PP_FOR_39(s, p, o, m) MACRO_PP_IF(p(40, s), m, MACRO_PP_TUPLE_EAT_2)(40, s) MACRO_PP_IF(p(40, s), MACRO_PP_FOR_40, MACRO_PP_TUPLE_EAT_4)(o(40, s), p, o, m)
#	 define MACRO_PP_FOR_40(s, p, o, m) MACRO_PP_IF(p(41, s), m, MACRO_PP_TUPLE_EAT_2)(41, s) MACRO_PP_IF(p(41, s), MACRO_PP_FOR_41, MACRO_PP_TUPLE_EAT_4)(o(41, s), p, o, m)
#	 define MACRO_PP_FOR_41(s, p, o, m) MACRO_PP_IF(p(42, s), m, MACRO_PP_TUPLE_EAT_2)(42, s) MACRO_PP_IF(p(42, s), MACRO_PP_FOR_42, MACRO_PP_TUPLE_EAT_4)(o(42, s), p, o, m)
#	 define MACRO_PP_FOR_42(s, p, o, m) MACRO_PP_IF(p(43, s), m, MACRO_PP_TUPLE_EAT_2)(43, s) MACRO_PP_IF(p(43, s), MACRO_PP_FOR_43, MACRO_PP_TUPLE_EAT_4)(o(43, s), p, o, m)
#	 define MACRO_PP_FOR_43(s, p, o, m) MACRO_PP_IF(p(44, s), m, MACRO_PP_TUPLE_EAT_2)(44, s) MACRO_PP_IF(p(44, s), MACRO_PP_FOR_44, MACRO_PP_TUPLE_EAT_4)(o(44, s), p, o, m)
#	 define MACRO_PP_FOR_44(s, p, o, m) MACRO_PP_IF(p(45, s), m, MACRO_PP_TUPLE_EAT_2)(45, s) MACRO_PP_IF(p(45, s), MACRO_PP_FOR_45, MACRO_PP_TUPLE_EAT_4)(o(45, s), p, o, m)
#	 define MACRO_PP_FOR_45(s, p, o, m) MACRO_PP_IF(p(46, s), m, MACRO_PP_TUPLE_EAT_2)(46, s) MACRO_PP_IF(p(46, s), MACRO_PP_FOR_46, MACRO_PP_TUPLE_EAT_4)(o(46, s), p, o, m)
#	 define MACRO_PP_FOR_46(s, p, o, m) MACRO_PP_IF(p(47, s), m, MACRO_PP_TUPLE_EAT_2)(47, s) MACRO_PP_IF(p(47, s), MACRO_PP_FOR_47, MACRO_PP_TUPLE_EAT_4)(o(47, s), p, o, m)
#	 define MACRO_PP_FOR_47(s, p, o, m) MACRO_PP_IF(p(48, s), m, MACRO_PP_TUPLE_EAT_2)(48, s) MACRO_PP_IF(p(48, s), MACRO_PP_FOR_48, MACRO_PP_TUPLE_EAT_4)(o(48, s), p, o, m)
#	 define MACRO_PP_FOR_48(s, p, o, m) MACRO_PP_IF(p(49, s), m, MACRO_PP_TUPLE_EAT_2)(49, s) MACRO_PP_IF(p(49, s), MACRO_PP_FOR_49, MACRO_PP_TUPLE_EAT_4)(o(49, s), p, o, m)
#	 define MACRO_PP_FOR_49(s, p, o, m) MACRO_PP_IF(p(50, s), m, MACRO_PP_TUPLE_EAT_2)(50, s) MACRO_PP_IF(p(50, s), MACRO_PP_FOR_50, MACRO_PP_TUPLE_EAT_4)(o(50, s), p, o, m)
#	 define MACRO_PP_FOR_50(s, p, o, m) MACRO_PP_IF(p(51, s), m, MACRO_PP_TUPLE_EAT_2)(51, s) MACRO_PP_IF(p(51, s), MACRO_PP_FOR_51, MACRO_PP_TUPLE_EAT_4)(o(51, s), p, o, m)
#	 define MACRO_PP_FOR_51(s, p, o, m) MACRO_PP_IF(p(52, s), m, MACRO_PP_TUPLE_EAT_2)(52, s) MACRO_PP_IF(p(52, s), MACRO_PP_FOR_52, MACRO_PP_TUPLE_EAT_4)(o(52, s), p, o, m)
#	 define MACRO_PP_FOR_52(s, p, o, m) MACRO_PP_IF(p(53, s), m, MACRO_PP_TUPLE_EAT_2)(53, s) MACRO_PP_IF(p(53, s), MACRO_PP_FOR_53, MACRO_PP_TUPLE_EAT_4)(o(53, s), p, o, m)
#	 define MACRO_PP_FOR_53(s, p, o, m) MACRO_PP_IF(p(54, s), m, MACRO_PP_TUPLE_EAT_2)(54, s) MACRO_PP_IF(p(54, s), MACRO_PP_FOR_54, MACRO_PP_TUPLE_EAT_4)(o(54, s), p, o, m)
#	 define MACRO_PP_FOR_54(s, p, o, m) MACRO_PP_IF(p(55, s), m, MACRO_PP_TUPLE_EAT_2)(55, s) MACRO_PP_IF(p(55, s), MACRO_PP_FOR_55, MACRO_PP_TUPLE_EAT_4)(o(55, s), p, o, m)
#	 define MACRO_PP_FOR_55(s, p, o, m) MACRO_PP_IF(p(56, s), m, MACRO_PP_TUPLE_EAT_2)(56, s) MACRO_PP_IF(p(56, s), MACRO_PP_FOR_56, MACRO_PP_TUPLE_EAT_4)(o(56, s), p, o, m)
#	 define MACRO_PP_FOR_56(s, p, o, m) MACRO_PP_IF(p(57, s), m, MACRO_PP_TUPLE_EAT_2)(57, s) MACRO_PP_IF(p(57, s), MACRO_PP_FOR_57, MACRO_PP_TUPLE_EAT_4)(o(57, s), p, o, m)
#	 define MACRO_PP_FOR_57(s, p, o, m) MACRO_PP_IF(p(58, s), m, MACRO_PP_TUPLE_EAT_2)(58, s) MACRO_PP_IF(p(58, s), MACRO_PP_FOR_58, MACRO_PP_TUPLE_EAT_4)(o(58, s), p, o, m)
#	 define MACRO_PP_FOR_58(s, p, o, m) MACRO_PP_IF(p(59, s), m, MACRO_PP_TUPLE_EAT_2)(59, s) MACRO_PP_IF(p(59, s), MACRO_PP_FOR_59, MACRO_PP_TUPLE_EAT_4)(o(59, s), p, o, m)
#	 define MACRO_PP_FOR_59(s, p, o, m) MACRO_PP_IF(p(60, s), m, MACRO_PP_TUPLE_EAT_2)(60, s) MACRO_PP_IF(p(60, s), MACRO_PP_FOR_60, MACRO_PP_TUPLE_EAT_4)(o(60, s), p, o, m)
#	 define MACRO_PP_FOR_60(s, p, o, m) MACRO_PP_IF(p(61, s), m, MACRO_PP_TUPLE_EAT_2)(61, s) MACRO_PP_IF(p(61, s), MACRO_PP_FOR_61, MACRO_PP_TUPLE_EAT_4)(o(61, s), p, o, m)
#	 define MACRO_PP_FOR_61(s, p, o, m) MACRO_PP_IF(p(62, s), m, MACRO_PP_TUPLE_EAT_2)(62, s) MACRO_PP_IF(p(62, s), MACRO_PP_FOR_62, MACRO_PP_TUPLE_EAT_4)(o(62, s), p, o, m)
#	 define MACRO_PP_FOR_62(s, p, o, m) MACRO_PP_IF(p(63, s), m, MACRO_PP_TUPLE_EAT_2)(63, s) MACRO_PP_IF(p(63, s), MACRO_PP_FOR_63, MACRO_PP_TUPLE_EAT_4)(o(63, s), p, o, m)
#	 define MACRO_PP_FOR_63(s, p, o, m) MACRO_PP_IF(p(64, s), m, MACRO_PP_TUPLE_EAT_2)(64, s) MACRO_PP_IF(p(64, s), MACRO_PP_FOR_64, MACRO_PP_TUPLE_EAT_4)(o(64, s), p, o, m)


#	 define MACRO_PP_TUPLE_EAT_1(e0)
#	 define MACRO_PP_TUPLE_EAT_2(e0, e1)
#	 define MACRO_PP_TUPLE_EAT_3(e0, e1, e2)
#	 define MACRO_PP_TUPLE_EAT_4(e0, e1, e2, e3)
#	 define MACRO_PP_TUPLE_EAT_5(e0, e1, e2, e3, e4)
#	 define MACRO_PP_TUPLE_EAT_6(e0, e1, e2, e3, e4, e5)
#	 define MACRO_PP_TUPLE_EAT_7(e0, e1, e2, e3, e4, e5, e6)
#	 define MACRO_PP_TUPLE_EAT_8(e0, e1, e2, e3, e4, e5, e6, e7)
#	 define MACRO_PP_TUPLE_EAT_9(e0, e1, e2, e3, e4, e5, e6, e7, e8)
#	 define MACRO_PP_TUPLE_EAT_10(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9)
#	 define MACRO_PP_TUPLE_EAT_11(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10)
#	 define MACRO_PP_TUPLE_EAT_12(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11)
#	 define MACRO_PP_TUPLE_EAT_13(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12)
#	 define MACRO_PP_TUPLE_EAT_14(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13)
#	 define MACRO_PP_TUPLE_EAT_15(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14)
#	 define MACRO_PP_TUPLE_EAT_16(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15)
#	 define MACRO_PP_TUPLE_EAT_17(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16)
#	 define MACRO_PP_TUPLE_EAT_18(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17)
#	 define MACRO_PP_TUPLE_EAT_19(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18)
#	 define MACRO_PP_TUPLE_EAT_20(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19)
#	 define MACRO_PP_TUPLE_EAT_21(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20)
#	 define MACRO_PP_TUPLE_EAT_22(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21)
#	 define MACRO_PP_TUPLE_EAT_23(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22)
#	 define MACRO_PP_TUPLE_EAT_24(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23)
#	 define MACRO_PP_TUPLE_EAT_25(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24)
#	 define MACRO_PP_TUPLE_EAT_26(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25)
#	 define MACRO_PP_TUPLE_EAT_27(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26)
#	 define MACRO_PP_TUPLE_EAT_28(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27)
#	 define MACRO_PP_TUPLE_EAT_29(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28)
#	 define MACRO_PP_TUPLE_EAT_30(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29)
#	 define MACRO_PP_TUPLE_EAT_31(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30)
#	 define MACRO_PP_TUPLE_EAT_32(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31)
#	 define MACRO_PP_TUPLE_EAT_33(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32)
#	 define MACRO_PP_TUPLE_EAT_34(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33)
#	 define MACRO_PP_TUPLE_EAT_35(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34)
#	 define MACRO_PP_TUPLE_EAT_36(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35)
#	 define MACRO_PP_TUPLE_EAT_37(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36)
#	 define MACRO_PP_TUPLE_EAT_38(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37)
#	 define MACRO_PP_TUPLE_EAT_39(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38)
#	 define MACRO_PP_TUPLE_EAT_40(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39)
#	 define MACRO_PP_TUPLE_EAT_41(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40)
#	 define MACRO_PP_TUPLE_EAT_42(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41)
#	 define MACRO_PP_TUPLE_EAT_43(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42)
#	 define MACRO_PP_TUPLE_EAT_44(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43)
#	 define MACRO_PP_TUPLE_EAT_45(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44)
#	 define MACRO_PP_TUPLE_EAT_46(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45)
#	 define MACRO_PP_TUPLE_EAT_47(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46)
#	 define MACRO_PP_TUPLE_EAT_48(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47)
#	 define MACRO_PP_TUPLE_EAT_49(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48)
#	 define MACRO_PP_TUPLE_EAT_50(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49)
#	 define MACRO_PP_TUPLE_EAT_51(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50)
#	 define MACRO_PP_TUPLE_EAT_52(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51)
#	 define MACRO_PP_TUPLE_EAT_53(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52)
#	 define MACRO_PP_TUPLE_EAT_54(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53)
#	 define MACRO_PP_TUPLE_EAT_55(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54)
#	 define MACRO_PP_TUPLE_EAT_56(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55)
#	 define MACRO_PP_TUPLE_EAT_57(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56)
#	 define MACRO_PP_TUPLE_EAT_58(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57)
#	 define MACRO_PP_TUPLE_EAT_59(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58)
#	 define MACRO_PP_TUPLE_EAT_60(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59)
#	 define MACRO_PP_TUPLE_EAT_61(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60)
#	 define MACRO_PP_TUPLE_EAT_62(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61)
#	 define MACRO_PP_TUPLE_EAT_63(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62)
#	 define MACRO_PP_TUPLE_EAT_64(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63)

#endif

