#ifndef __BST_COMMON_20191021232104_1442199360_H__
#define __BST_COMMON_20191021232104_1442199360_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <libCommon/modafx.h>

typedef bool        _mpl_nttp_bool;
typedef int         _mpl_nttp_int;
typedef unsigned    _mpl_nttp_unsigned;
typedef long        _mpl_nttp_long;

#define BOOST_MPL_AUX_NTTP_DECL(T, x) MACRO_PP_CAT(_mpl_nttp_,T) x /**/

namespace boost_wheel{

	namespace mpl{
		template< BOOST_MPL_AUX_NTTP_DECL(int, N) > struct int_;
	}

}

#endif
