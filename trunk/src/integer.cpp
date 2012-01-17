//
//  integer.cpp
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
#include "integer.h"

// Class setup dependencies
#include "list.h"
#include "method.h"
#include "boolean.h"
#include "exception.h"
#include "thrown_exception.h"
#include "rwstream.h"

// Specific dependencies
#include "str.h"
#include "real.h"
#include "octet.h"
#include <sstream>

using namespace esharp;

/********************************
 CLASS SETUP
 ********************************/

integer integer::integer_class;
integer integer::integer_0(0);
integer integer::integer_1(1);

integer::integer()
:object(&object::object_class)
{
	setClassname(L"integer_class");
	setMethod(L"new(0)", new method(new_instance));
	setMethod(L"operator_cast(1)", new method(operator_cast));
	setMethod(L"operator_eq(1)", new method(operator_eq));
	setMethod(L"operator_bigger(1)", new method(operator_bigger));
	setMethod(L"operator_bigger_eq(1)", new method(operator_bigger_eq));
	setMethod(L"operator_lower(1)", new method(operator_lower));
	setMethod(L"operator_lower_eq(1)", new method(operator_lower_eq));
	setMethod(L"operator_add(1)", new method(operator_add));
	setMethod(L"operator_sub(1)", new method(operator_sub));
	setMethod(L"operator_mul(1)", new method(operator_mul));
	setMethod(L"operator_div(1)", new method(operator_div));
	setMethod(L"operator_mod(1)", new method(operator_mod));
	setMethod(L"operator_minus(0)", new method(operator_minus));
	setMethod(L"operator_plus(0)", new method(operator_plus));
	setMethod(L"operator_bit_and(1)", new method(operator_bit_and));
	setMethod(L"operator_bit_xor(1)", new method(operator_bit_xor));
	setMethod(L"operator_bit_or(1)", new method(operator_bit_or));
	setMethod(L"operator_lshift(1)", new method(operator_lshift));
	setMethod(L"operator_rshift(1)", new method(operator_rshift));
	setMethod(L"operator_bit_not(0)", new method(operator_bit_not));
	setMethod(L"read(1)", new method(read));
	setMethod(L"write(1)", new method(write));
}

integer::integer(int integer_param)
:object(&integer::integer_class)
{
	setClassname(L"integer");
	value = integer_param;
}
integer::~integer()
{
}

/********************************
 HELPER METHODS
 ********************************/

static object* get_2_integer_params(list* param, integer*& self, integer*& other) {
	self = dynamic_cast<integer*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	object* arg_test = param->getObject(1);
	if(arg_test->isInstanceOf(&integer::integer_class)) {
		other = dynamic_cast<integer*>(arg_test);
	} else {
		method* cast = arg_test->getMethod(L"operator_cast(1)");
		object* arg_cast_result = cast->call(2, arg_test, &integer::integer_class);
		if(arg_cast_result->isInstanceOf(&thrown_exception::thrown_exception_class)) {
			return arg_cast_result;
		}
		other = dynamic_cast<integer*>(arg_cast_result);
	}
	return 0;
}

int integer::getValue()
{
	return value;
}

unsigned int integer::getHash()
{
	if(!hash) {
		std::wstringstream obj_repr;
		obj_repr << L"i" << value;
		hash = digest(obj_repr.str());
	}
	return hash;
}

/********************************
 NATIVE METHODS
 ********************************/

object* integer::new_instance(list* param)
{
	integer* self = dynamic_cast<integer*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	integer* newInstance = new integer(self->value);
	newInstance->ancestor = self;
	return newInstance;
}

object* integer::operator_cast(list* param)
{
	integer* self = dynamic_cast<integer*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	object* target_class = param->getObject(1);
	if(target_class == &integer::integer_class) {
		return self;
	} else if(target_class->isInstanceOf(&real::real_class)) {
		return new real((double)(self->value));
	} else if(target_class->isInstanceOf(&octet::octet_class)) {
		return octet::get((unsigned char)(self->value));
	} else if(target_class->isInstanceOf(&str::str_class)) {
		std::wstringstream obj_repr;
		obj_repr << self->value;
		return new str(obj_repr.str());
	}
	std::wstringstream err;
	err << L"Cannot cast '" << self->getClassname() << "' to '" << target_class->getClassname() << "'";
	return new thrown_exception(new exception(err.str()));
}

object* integer::operator_eq(list* param)
{
	integer *self, *other;
	object* result = get_2_integer_params(param, self, other);
	if(result) {
		return result;
	}
	if (self->value == other->value) {
		return &boolean::boolean_true;
	} else {
		return &boolean::boolean_false;
	}
}

object* integer::operator_bigger(list* param)
{
	integer *self, *other;
	object* result = get_2_integer_params(param, self, other);
	if(result) {
		return result;
	}
	if (self->value > other->value) {
		return &boolean::boolean_true;
	} else {
		return &boolean::boolean_false;
	}
}

object* integer::operator_bigger_eq(list* param)
{
	integer *self, *other;
	object* result = get_2_integer_params(param, self, other);
	if(result) {
		return result;
	}
	if (self->value >= other->value) {
		return &boolean::boolean_true;
	} else {
		return &boolean::boolean_false;
	}
}

object* integer::operator_lower(list* param)
{
	return (operator_bigger_eq(param) == &boolean::boolean_false) ? &boolean::boolean_true : &boolean::boolean_false;
}

object* integer::operator_lower_eq(list* param)
{
	return (operator_bigger(param) == &boolean::boolean_false) ? &boolean::boolean_true : &boolean::boolean_false;
}

object* integer::operator_minus(list* param)
{
	integer* self = dynamic_cast<integer*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	return new integer(-self->value);
}

object* integer::operator_plus(list* param)
{
	integer* self = dynamic_cast<integer*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	return self;
}

object* integer::operator_add(list* param)
{
	integer *self, *other;
	object* result = get_2_integer_params(param, self, other);
	if(result) {
		return result;
	}
	return new integer(self->value + other->value);
}

object* integer::operator_sub(list* param)
{
	integer *self, *other;
	object* result = get_2_integer_params(param, self, other);
	if(result) {
		return result;
	}
	return new integer(self->value - other->value);
}

object* integer::operator_mul(list* param)
{
	integer *self, *other;
	object* result = get_2_integer_params(param, self, other);
	if(result) {
		return result;
	}
	return new integer(self->value * other->value);
}

object* integer::operator_div(list* param)
{
	integer *self, *other;
	object* result = get_2_integer_params(param, self, other);
	if(result) {
		return result;
	}
	if (other->value == 0) {
		return new thrown_exception(new exception(L"Division by zero"));
	}
	return new integer(self->value / other->value);
}

object* integer::operator_mod(list* param)
{
	integer *self, *other;
	object* result = get_2_integer_params(param, self, other);
	if(result) {
		return result;
	}
	if (other->value == 0) {
		return new thrown_exception(new exception(L"Modulus by zero"));
	}
	return new integer(self->value % other->value);
}

object* integer::operator_lshift(list* param)
{
	integer *self, *other;
	object* result = get_2_integer_params(param, self, other);
	if(result) {
		return result;
	}
	return new integer(self->value << other->value);
}

object* integer::operator_rshift(list* param)
{
	integer *self, *other;
	object* result = get_2_integer_params(param, self, other);
	if(result) {
		return result;
	}
	return new integer(self->value >> other->value);
}

object* integer::operator_bit_and(list* param)
{
	integer *self, *other;
	object* result = get_2_integer_params(param, self, other);
	if(result) {
		return result;
	}
	return new integer(self->value & other->value);
}

object* integer::operator_bit_or(list* param)
{
	integer *self, *other;
	object* result = get_2_integer_params(param, self, other);
	if(result) {
		return result;
	}
	return new integer(self->value | other->value);
}

object* integer::operator_bit_xor(list* param)
{
	integer *self, *other;
	object* result = get_2_integer_params(param, self, other);
	if(result) {
		return result;
	}
	return new integer(self->value ^ other->value);
}

object* integer::operator_bit_not(list* param)
{
	integer* self = dynamic_cast<integer*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	return new integer(~self->value);
}

object* integer::read(list* param)
{
	integer* self = dynamic_cast<integer*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	rwstream* stream = dynamic_cast<rwstream*>(param->getObject(1));
	if(!stream) {
		return new thrown_exception(new exception(L"Invalid arguments"));
	}
	int value_read;
	object* result = stream->readDataToBuffer((char*)&value_read, sizeof(value_read));
	if(result) {
		return result;
	}
	return new integer(value_read);
}

object* integer::write(list* param)
{
	integer* self = dynamic_cast<integer*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	rwstream* stream = dynamic_cast<rwstream*>(param->getObject(1));
	if(!stream) {
		return new thrown_exception(new exception(L"Invalid arguments"));
	}
	int value_write = self->value;
	object* result = stream->writeDataFromBuffer((char*)&value_write, sizeof(value_write));
	if(result) {
		return result;
	}
	return &object::object_none;
}
