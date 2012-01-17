//
//  exception.cpp
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
#include "exception.h"

// Class setup dependencies
#include "list.h"
#include "method.h"
#include "boolean.h"
#include "exception.h"
#include "thrown_exception.h"

// Specific dependencies
#include "str.h"
#include "data.h"
#include "console.h"
#include "gc.h"
#include <iostream>
#include <sstream>

using namespace esharp;

/********************************
 CLASS SETUP
 ********************************/

exception exception::exception_class;

exception::exception()
:object(&object::object_class)
{
	setClassname(L"exception_class");
	setMethod(L"new(0)", new method(new_instance));
	setMethod(L"new(1)", new method(new_instance_with_message));
	setMethod(L"operator_cast(1)", new method(operator_cast));
	setMethod(L"print_trace(1)", new method(print_trace));
}

exception::exception(std::wstring message_param, exception* cause_param)
:object(&exception::exception_class)
{
	setClassname(L"exception");
	message = message_param;
	cause = cause_param;
}

exception::~exception()
{
}

/********************************
 HELPER METHODS
 ********************************/

const std::wstring& exception::getMessage()
{
	return message;
}

void exception::addTrace(const std::wstring& trace_param)
{
	trace.push_back(trace_param);
}

exception* exception::getCause()
{
	return cause;
}

/********************************
 NATIVE METHODS
 ********************************/

object* exception::new_instance(list* param)
{
	exception* self = dynamic_cast<exception*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	exception* newInstance = new exception(L"Unknown exception");
	newInstance->ancestor = self;
	return newInstance;
}

object* exception::new_instance_with_message(list* param)
{
	exception* self = dynamic_cast<exception*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	str* message = dynamic_cast<str*>(param->getObject(1));
	if(!message) {
		return new thrown_exception(new exception(L"Exception message can only be a 'str'"));
	}
	exception* newInstance = new exception(message->getValue());
	newInstance->ancestor = self;
	return newInstance;
}

object* exception::operator_cast(list* param)
{
	exception* self = dynamic_cast<exception*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	object* target_class = param->getObject(1);
	if(target_class == &exception::exception_class) {
		return self;
	} else if(target_class->isInstanceOf(&str::str_class)) {
		return new str(self->getMessage());
	}
	std::wstringstream err;
	err << L"Cannot cast '" << self->getClassname() << "' to '" << target_class->getClassname() << "'";
	return new thrown_exception(new exception(err.str()));
}

object* exception::print_trace(list* param)
{
	exception* self = dynamic_cast<exception*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	console* target_console = dynamic_cast<console*>(param->getObject(1));
	if(!target_console) {
		return new thrown_exception(new exception(L"First argument should be a console (eg. stderr or stdout)"));
	}
	target_console->getStream() << "Traceback (most recent call last):" << std::endl;
	for(std::vector<std::wstring>::const_reverse_iterator i=self->trace.rbegin();i!=self->trace.rend();i++) {
		str tr_str((*i));
		data* tr_data = dynamic_cast<data*>(tr_str.getMethod(L"encode_utf8(0)")->call(1, &tr_str));
		target_console->getStream() << tr_data->getValue() << std::endl;
	}
	std::wstringstream tr_wstring;
	tr_wstring << self->getClassname() << L": " << self->getMessage();
	str tr_str(tr_wstring.str());
	data* tr_data = dynamic_cast<data*>(tr_str.getMethod(L"encode_utf8(0)")->call(1, &tr_str));
	target_console->getStream() << tr_data->getValue() << std::endl;
	if(self->getCause()) {
		target_console->getStream() << L"Caused by:" << std::endl;
		list args(0);
		args.appendObject(self->getCause());
		args.appendObject(target_console);
		return print_trace(&args);
	}
	return &object::object_none;
}


