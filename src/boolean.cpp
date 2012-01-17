//
//  boolean.cpp
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
#include "boolean.h"

// Class setup dependencies
#include "list.h"
#include "method.h"
#include "boolean.h"
#include "exception.h"
#include "thrown_exception.h"
#include "rwstream.h"

// Specific dependencies
#include "str.h"

using namespace esharp;

/********************************
 CLASS SETUP
 ********************************/

boolean boolean::boolean_class;
boolean boolean::boolean_true(0);
boolean boolean::boolean_false(0);

boolean::boolean()
:object(&object::object_class)
{
	setClassname(L"boolean_class");
	setMethod(L"operator_cast(1)", new method(operator_cast));
	setMethod(L"operator_eq(1)", new method(operator_eq));
	setMethod(L"operator_logic_and(1)", new method(operator_logic_and));
	setMethod(L"operator_logic_or(1)", new method(operator_logic_or));
	setMethod(L"operator_logic_not(0)", new method(operator_logic_not));
	setMethod(L"read(1)", new method(read));
	setMethod(L"write(1)", new method(write));
}

boolean::boolean(void*)
:object(&boolean::boolean_class)
{
	setClassname(L"boolean");
}
boolean::~boolean()
{
}

/********************************
 HELPER METHODS
 ********************************/

unsigned int boolean::getHash()
{
	if(!hash) {
		std::wstringstream obj_repr;
		obj_repr << L"b" << (this==&boolean::boolean_true?true:false);
		hash = digest(obj_repr.str());
	}
	return hash;
}

/********************************
 NATIVE METHODS
 ********************************/

object* boolean::operator_cast(list* param)
{
	boolean* self = dynamic_cast<boolean*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	object* target_class = param->getObject(1);
	if(target_class == &boolean::boolean_class) {
		return self;
	} else if(target_class->isInstanceOf(&str::str_class)) {
		std::wstringstream obj_repr;
		if(self==&boolean::boolean_false) {
			obj_repr << L"false";
		} else {
			obj_repr << L"true";
		}
		return new str(obj_repr.str());
	}
	std::wstringstream err;
	err << L"Cannot cast '" << self->getClassname() << "' to '" << target_class->getClassname() << "'";
	return new thrown_exception(new exception(err.str()));
}

object* boolean::operator_eq(list* param)
{
	boolean* self = dynamic_cast<boolean*>(param->getObject(0));
	if (!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	boolean* other = dynamic_cast<boolean*>(param->getObject(1));
	if (!other) {
		return new thrown_exception(new exception(L"Invalid arguments"));
	}
	if (self == other) {
		return &boolean::boolean_true;
	} else {
		return &boolean::boolean_false;
	}
}

object* boolean::operator_logic_and(list* param)
{
	boolean* self = dynamic_cast<boolean*>(param->getObject(0));
	if (!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	boolean* other = dynamic_cast<boolean*>(param->getObject(1));
	if (!other) {
		return new thrown_exception(new exception(L"Invalid arguments"));
	}
	if(self==&boolean::boolean_true && other==&boolean::boolean_true) {
		return &boolean::boolean_true;
	}
	return &boolean::boolean_false;
}

object* boolean::operator_logic_or(list* param)
{
	boolean* self = dynamic_cast<boolean*>(param->getObject(0));
	if (!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	boolean* other = dynamic_cast<boolean*>(param->getObject(1));
	if (!other) {
		return new thrown_exception(new exception(L"Invalid arguments"));
	}
	if(self==&boolean::boolean_true || other==&boolean::boolean_true) {
		return &boolean::boolean_true;
	}
	return &boolean::boolean_false;
}

object* boolean::operator_logic_not(list* param)
{
	boolean* self = dynamic_cast<boolean*>(param->getObject(0));
	if (!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	if(self==&boolean::boolean_true) {
		return &boolean::boolean_false;
	}
	return &boolean::boolean_true;
}
object* boolean::read(list* param)
{
	boolean* self = dynamic_cast<boolean*>(param->getObject(0));
	if (!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	rwstream* stream = dynamic_cast<rwstream*>(param->getObject(1));
	if (!stream) {
		return new thrown_exception(new exception(L"Invalid arguments"));
	}
	char buffer[1];
	object* result = stream->readDataToBuffer(buffer, 1);
	if(result) {
		return result;
	}
	if(buffer[0]==0) {
		return &boolean::boolean_false;
	}
	return &boolean::boolean_true;
}
object* boolean::write(list* param)
{
	boolean* self = dynamic_cast<boolean*>(param->getObject(0));
	if (!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	rwstream* stream = dynamic_cast<rwstream*>(param->getObject(1));
	if (!stream) {
		return new thrown_exception(new exception(L"Invalid arguments"));
	}
	char buffer[1];
	if(self==&boolean::boolean_false) {
		buffer[0] = 0;
	} else {
		buffer[0] = 1;
	}
	object* result = stream->writeDataFromBuffer(buffer, 1);
	if(result) {
		return result;
	}
	return &object::object_none;
}

