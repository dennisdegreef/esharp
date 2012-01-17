//
//  real.cpp
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
#include "real.h"

// Class setup dependencies
#include "list.h"
#include "method.h"
#include "boolean.h"
#include "exception.h"
#include "thrown_exception.h"
#include "rwstream.h"

// Specific dependencies
#include "str.h"
#include "integer.h"
#include "octet.h"
#include <sstream>

using namespace esharp;

/********************************
 CLASS SETUP
 ********************************/

real real::real_class;

real::real()
:object(&object::object_class)
{
	setClassname(L"real_class");
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
	setMethod(L"operator_minus(0)", new method(operator_minus));
	setMethod(L"operator_plus(0)", new method(operator_plus));
	setMethod(L"read(1)", new method(read));
	setMethod(L"write(1)", new method(write));
}

real::real(double real_param)
:object(&real::real_class)
{
	setClassname(L"real");
	value = real_param;
}
real::~real()
{
}

/********************************
 HELPER METHODS
 ********************************/

static object* get_2_real_params(list* param, real*& self, real*& other) {
	self = dynamic_cast<real*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	object* arg_test = param->getObject(1);
	if(arg_test->isInstanceOf(&real::real_class)) {
		other = dynamic_cast<real*>(arg_test);
	} else {
		method* cast = arg_test->getMethod(L"operator_cast(1)");
		object* arg_cast_result = cast->call(2, arg_test, &real::real_class);
		if(arg_cast_result->isInstanceOf(&thrown_exception::thrown_exception_class)) {
			return arg_cast_result;
		}
		other = dynamic_cast<real*>(arg_cast_result);
	}
	return 0;
}

double real::getValue()
{
	return value;
}

unsigned int real::getHash()
{
	if(!hash) {
		std::wstringstream obj_repr;
		obj_repr << L"r" << value;
		hash = digest(obj_repr.str());
	}
	return hash;
}

/********************************
 NATIVE METHODS
 ********************************/

object* real::new_instance(list* param)
{
	real* self = dynamic_cast<real*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	real* newInstance = new real(0.0);
	newInstance->ancestor = self;
	return newInstance;
}

object* real::operator_cast(list* param)
{
	real* self = dynamic_cast<real*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	object* target_class = param->getObject(1);
	if(target_class == &real::real_class) {
		return self;
	} else if(target_class->isInstanceOf(&integer::integer_class)) {
		return new integer((int)(self->value));
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

object* real::operator_eq(list* param)
{
	real *self, *other;
	object* result = get_2_real_params(param, self, other);
	if(result) {
		return result;
	}
	if (self->value == other->value) {
		return &boolean::boolean_true;
	} else {
		return &boolean::boolean_false;
	}
}

object* real::operator_bigger(list* param)
{
	real *self, *other;
	object* result = get_2_real_params(param, self, other);
	if(result) {
		return result;
	}
	if (self->value > other->value) {
		return &boolean::boolean_true;
	} else {
		return &boolean::boolean_false;
	}
}

object* real::operator_bigger_eq(list* param)
{
	real *self, *other;
	object* result = get_2_real_params(param, self, other);
	if(result) {
		return result;
	}
	if (self->value >= other->value) {
		return &boolean::boolean_true;
	} else {
		return &boolean::boolean_false;
	}
}

object* real::operator_lower(list* param)
{
	return (operator_bigger_eq(param) == &boolean::boolean_false) ? &boolean::boolean_true : &boolean::boolean_false;
}

object* real::operator_lower_eq(list* param)
{
	return (operator_bigger(param) == &boolean::boolean_false) ? &boolean::boolean_true : &boolean::boolean_false;
}

object* real::operator_minus(list* param)
{
	real* self = dynamic_cast<real*>(param->getObject(0));
	return new real(-self->value);
}

object* real::operator_plus(list* param)
{
	real* self = dynamic_cast<real*>(param->getObject(0));
	return self;
}

object* real::operator_add(list* param)
{
	real *self, *other;
	object* result = get_2_real_params(param, self, other);
	if(result) {
		return result;
	}
	return new real(self->value + other->value);
}

object* real::operator_sub(list* param)
{
	real *self, *other;
	object* result = get_2_real_params(param, self, other);
	if(result) {
		return result;
	}
	return new real(self->value - other->value);
}

object* real::operator_mul(list* param)
{
	real *self, *other;
	object* result = get_2_real_params(param, self, other);
	if(result) {
		return result;
	}
	return new real(self->value * other->value);
}

object* real::operator_div(list* param)
{
	real *self, *other;
	object* result = get_2_real_params(param, self, other);
	if(result) {
		return result;
	}
	if (other->value == 0.0) {
		return new thrown_exception(new exception(L"Division by zero"));
	}
	return new real(self->value / other->value);
}

object* real::read(list* param)
{
	real* self = dynamic_cast<real*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	rwstream* stream = dynamic_cast<rwstream*>(param->getObject(1));
	if(!stream) {
		return new thrown_exception(new exception(L"Invalid arguments"));
	}
	double value_read;
	object* result = stream->readDataToBuffer((char*)&value_read, sizeof(value_read));
	if(result) {
		return result;
	}
	return new real(value_read);
}

object* real::write(list* param)
{
	real* self = dynamic_cast<real*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	rwstream* stream = dynamic_cast<rwstream*>(param->getObject(1));
	if(!stream) {
		return new thrown_exception(new exception(L"Invalid arguments"));
	}
	double value_write = self->value;
	object* result = stream->writeDataFromBuffer((char*)&value_write, sizeof(value_write));
	if(result) {
		return result;
	}
	return &object::object_none;
}
