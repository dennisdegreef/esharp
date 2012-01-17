//
//  method.cpp
//  E#
//
//  Created by Adrian Punga on 01/03/2012.
/*
Copyright (c) 2012 Adrian Punga

Permission is hereby granted, free of charge, to any person obtaining a copy of this software
and associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute, 
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is 
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or 
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

// Declarations
#include "method.h"

// Class setup dependencies
#include "list.h"
#include "method.h"
#include "boolean.h"
#include "exception.h"
#include "thrown_exception.h"

// Specific dependencies
#include <stdarg.h>
#include "bc_classdef.h"

using namespace esharp;

/********************************
 CLASS SETUP
 ********************************/

method method::method_class;
list method::arguments(8);

method::method()
:object_nogc(&object::object_class)
{
	setClassname(L"method_class");
}

method::method(esharp_method* method_pointer_param)
:object_nogc(&method::method_class)
{
	setClassname(L"method(native)");
	method_pointer = method_pointer_param;
	method_bytecode = 0;
}

method::method(bc_method* method_bytecode_param)
:object_nogc(&method::method_class)
{
	setClassname(L"method(bytecode)");
	method_pointer = 0;
	method_bytecode = method_bytecode_param->clone();
}

method::~method()
{
	delete method_bytecode;
}

/********************************
 HELPER METHODS
 ********************************/

object* method::call(list* param)
{
	if(method_pointer!=0) {
		return (*method_pointer)(param);
	}
	if(method_bytecode!=0) {
		return method_bytecode->call(param);
	}
	return 0;
}

object* method::call(int param_count, ...)
{
	if(param_count<8) {
		arguments.clearList();
		va_list ap;
		va_start(ap, param_count);
		while(param_count--) {
			arguments.appendObject(va_arg(ap, object*));
		}
		va_end(ap);
		return call(&arguments);
	} else {
		list args(0);
		va_list ap;
		va_start(ap, param_count);
		while(param_count--) {
			args.appendObject(va_arg(ap, object*));
		}
		va_end(ap);
		return call(&args);
	}
}

/********************************
 NATIVE METHODS
 ********************************/

///NONE
