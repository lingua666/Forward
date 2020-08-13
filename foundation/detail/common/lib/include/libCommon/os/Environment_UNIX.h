//
// Environment_UNIX.h
//
// $Id: //poco/1.3/Foundation/include/Poco/Environment_UNIX.h#2 $
//
// Library: Foundation
// Package: Core
// Module:  Environment
//
// Definition of the EnvironmentImpl class for Unix.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//

#ifndef __ENVIRONMENT_UNIX_20160419111505_1438860785_H__
#define __ENVIRONMENT_UNIX_20160419111505_1438860785_H__

#include "Platform.h"
#include "Types.h"
#include <map>

class EnvironmentImpl
{
public:
	typedef UInt8 NodeId[6]; /// Ethernet address.

	static _string_type getImpl(const _string_type& name);
	static bool hasImpl(const _string_type& name);	
	static int setImpl(const _string_type& name, const _string_type& value);
	static _string_type osNameImpl();
	static _string_type osVersionImpl();
	static _string_type osArchitectureImpl();
	static _string_type nodeNameImpl();
	static int nodeIdImpl(NodeId& id);

private:
	typedef std::map<_string_type, _string_type> StringMap;
	
	static StringMap _map;
};


#endif // __CUSTOM_ENVIRONMENT_UNIX_HEAD_DEF__
