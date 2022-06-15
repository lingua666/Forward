//
// yield.hpp
// ~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "coroutine.hpp"

#ifndef _reenter
# define _reenter(c) COPY_BOOST_ASIO_CORO_REENTER(c)
#endif

#ifndef _yield
# define _yield COPY_BOOST_ASIO_CORO_YIELD
#endif

#ifndef _fork
# define _fork COPY_BOOST_ASIO_CORO_FORK
#endif
