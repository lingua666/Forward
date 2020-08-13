// memory standard header

#ifndef __SHARED_PTR_V20_20160128154200_1453966920_H__
#define __SHARED_PTR_V20_20160128154200_1453966920_H__

	#include <libCommon/modafx.h>

	#if defined(_NCFW_VER_) && (_NCFW_VER_ >= 0x1000)
		#include <libSmart_ptr_v20/enable_shared_from_this.hpp>
		#define SmartPTR					_smart_ptr_v20_::SmartPtr
		#define WeakPTR						_smart_ptr_v20_::weak_ptr
		#define _enable_shared_from_this	_smart_ptr_v20_::enable_shared_from_this
	#endif

#endif /* __CUSTOM_SHARED_PTR_HEAD_DEF__ */


/*
 * Copyright (c) 1992-2008 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 */

/*
 * This file is derived from software bearing the following
 * restrictions:
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this
 * software and its documentation for any purpose is hereby
 * granted without fee, provided that the above copyright notice
 * appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation.
 * Hewlett-Packard Company makes no representations about the
 * suitability of this software for any purpose. It is provided
 * "as is" without express or implied warranty.
 V5.05:0009 */
