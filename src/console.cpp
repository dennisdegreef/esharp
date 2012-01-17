//
//  console.cpp
//  E#
//
//  Created by Adrian Punga on 01/06/2012.
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
#include "console.h"

// Class setup dependencies
#include "list.h"
#include "method.h"
#include "boolean.h"
#include "exception.h"
#include "thrown_exception.h"
#include "gc.h"

// Specific dependencies
#include "str.h"
#include "data.h"

using namespace esharp;

/********************************
 CLASS SETUP
 ********************************/

console console::console_class;
console console::console_stdout(std::cout);
console console::console_stderr(std::cerr);

console::console()
:object(&object::object_class)
{
	setClassname(L"console_class");
	stream = 0;
	setMethod(L"print(...)", new method(print));
	setMethod(L"println(...)", new method(println));
}

console::console(std::ostream& stream_param)
:object(&console::console_class)
{
	setClassname(L"console");
	stream = &stream_param;
}

console::~console()
{
}

/********************************
 HELPER METHODS
 ********************************/

std::ostream& console::getStream() {
	return *stream;
}

/********************************
 NATIVE METHODS
 ********************************/

object* console::print(list* param)
{
	console* self = dynamic_cast<console*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	for(int i=1; i<param->getLength(); i++) {
		object* arg = param->getObject(i);
		method* cast = arg->getMethod(L"operator_cast(1)");
		object* str_arg = cast->call(2, arg, &str::str_class);
		if(str_arg->isInstanceOf(&thrown_exception::thrown_exception_class)) {
			return str_arg;
		}
		method* encode = str_arg->getMethod(L"encode_utf8(0)");
		object* data_arg = encode->call(1, str_arg);
		if(data_arg->isInstanceOf(&thrown_exception::thrown_exception_class)) {
			return data_arg;
		}
		data* data_arg_casted = dynamic_cast<data*>(data_arg);
		self->getStream() << data_arg_casted->getValue();
	}
	return &object::object_none;
}

object* console::println(list* param)
{
	console* self = dynamic_cast<console*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	for(int i=1; i<param->getLength(); i++) {
		object* arg = param->getObject(i);
		method* cast = arg->getMethod(L"operator_cast(1)");
		object* str_arg = cast->call(2, arg, &str::str_class);
		if(str_arg->isInstanceOf(&thrown_exception::thrown_exception_class)) {
			return str_arg;
		}
		method* encode = str_arg->getMethod(L"encode_utf8(0)");
		object* data_arg = encode->call(1, str_arg);
		if(data_arg->isInstanceOf(&thrown_exception::thrown_exception_class)) {
			return data_arg;
		}
		data* data_arg_casted = dynamic_cast<data*>(data_arg);
		self->getStream() << data_arg_casted->getValue();
	}
	self->getStream() << std::endl;
	return &object::object_none;
}
