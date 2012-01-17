//
//  octet.cpp
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
#include "octet.h"

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
#include "integer.h"
#include <sstream>

using namespace esharp;

/********************************
 CLASS SETUP
 ********************************/

octet octet::octet_class;

#define OCTET_STATIC_INIT(prefix) \
		octet octet::octet_##prefix##0(prefix##0);\
		octet octet::octet_##prefix##1(prefix##1);\
		octet octet::octet_##prefix##2(prefix##2);\
		octet octet::octet_##prefix##3(prefix##3);\
		octet octet::octet_##prefix##4(prefix##4);\
		octet octet::octet_##prefix##5(prefix##5);\
		octet octet::octet_##prefix##6(prefix##6);\
		octet octet::octet_##prefix##7(prefix##7);\
		octet octet::octet_##prefix##8(prefix##8);\
		octet octet::octet_##prefix##9(prefix##9);\
		octet octet::octet_##prefix##A(prefix##A);\
		octet octet::octet_##prefix##B(prefix##B);\
		octet octet::octet_##prefix##C(prefix##C);\
		octet octet::octet_##prefix##D(prefix##D);\
		octet octet::octet_##prefix##E(prefix##E);\
		octet octet::octet_##prefix##F(prefix##F)

OCTET_STATIC_INIT(0x0);
OCTET_STATIC_INIT(0x1);
OCTET_STATIC_INIT(0x2);
OCTET_STATIC_INIT(0x3);
OCTET_STATIC_INIT(0x4);
OCTET_STATIC_INIT(0x5);
OCTET_STATIC_INIT(0x6);
OCTET_STATIC_INIT(0x7);
OCTET_STATIC_INIT(0x8);
OCTET_STATIC_INIT(0x9);
OCTET_STATIC_INIT(0xA);
OCTET_STATIC_INIT(0xB);
OCTET_STATIC_INIT(0xC);
OCTET_STATIC_INIT(0xD);
OCTET_STATIC_INIT(0xE);
OCTET_STATIC_INIT(0xF);

#undef OCTET_STATIC_INIT

octet::octet()
:object(&object::object_class)
{
	setClassname(L"octet_class");
	setMethod(L"operator_cast(1)", new method(operator_cast));
	setMethod(L"operator_bigger(1)", new method(operator_bigger));
	setMethod(L"operator_bigger_eq(1)", new method(operator_bigger_eq));
	setMethod(L"operator_lower(1)", new method(operator_lower));
	setMethod(L"operator_lower_eq(1)", new method(operator_lower_eq));
	setMethod(L"operator_add(1)", new method(operator_add));
	setMethod(L"operator_sub(1)", new method(operator_sub));
	setMethod(L"operator_mul(1)", new method(operator_mul));
	setMethod(L"operator_div(1)", new method(operator_div));
	setMethod(L"operator_mod(1)", new method(operator_mod));
	setMethod(L"operator_bit_and(1)", new method(operator_bit_and));
	setMethod(L"operator_bit_xor(1)", new method(operator_bit_xor));
	setMethod(L"operator_bit_or(1)", new method(operator_bit_or));
	setMethod(L"operator_lshift(1)", new method(operator_lshift));
	setMethod(L"operator_rshift(1)", new method(operator_rshift));
	setMethod(L"operator_bit_not(0)", new method(operator_bit_not));
	setMethod(L"read(1)", new method(read));
	setMethod(L"write(1)", new method(write));
}

octet::octet(unsigned char octet_param)
:object(&octet::octet_class)
{
	setClassname(L"octet");
	value = octet_param;
}
octet::~octet()
{
}

/********************************
 HELPER METHODS
 ********************************/

octet* octet::get(unsigned char octet_param)
{
	switch(octet_param) {
#define OCTET_CASE(prefix) \
		case prefix##0: return &octet::octet_##prefix##0;\
		case prefix##1: return &octet::octet_##prefix##1;\
		case prefix##2: return &octet::octet_##prefix##2;\
		case prefix##3: return &octet::octet_##prefix##3;\
		case prefix##4: return &octet::octet_##prefix##4;\
		case prefix##5: return &octet::octet_##prefix##5;\
		case prefix##6: return &octet::octet_##prefix##6;\
		case prefix##7: return &octet::octet_##prefix##7;\
		case prefix##8: return &octet::octet_##prefix##8;\
		case prefix##9: return &octet::octet_##prefix##9;\
		case prefix##A: return &octet::octet_##prefix##A;\
		case prefix##B: return &octet::octet_##prefix##B;\
		case prefix##C: return &octet::octet_##prefix##C;\
		case prefix##D: return &octet::octet_##prefix##D;\
		case prefix##E: return &octet::octet_##prefix##E;\
		case prefix##F: return &octet::octet_##prefix##F

OCTET_CASE(0x0);
OCTET_CASE(0x1);
OCTET_CASE(0x2);
OCTET_CASE(0x3);
OCTET_CASE(0x4);
OCTET_CASE(0x5);
OCTET_CASE(0x6);
OCTET_CASE(0x7);
OCTET_CASE(0x8);
OCTET_CASE(0x9);
OCTET_CASE(0xA);
OCTET_CASE(0xB);
OCTET_CASE(0xC);
OCTET_CASE(0xD);
OCTET_CASE(0xE);
OCTET_CASE(0xF);
#undef OCTET_CASE
	}
	return &octet::octet_0x00;
}

static object* get_2_octet_params(list* param, octet*& self, octet*& other) {
	self = dynamic_cast<octet*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	object* arg_test = param->getObject(1);
	if(arg_test->isInstanceOf(&octet::octet_class)) {
		other = dynamic_cast<octet*>(arg_test);
	} else {
		method* cast = arg_test->getMethod(L"operator_cast(1)");
		object* arg_cast_result = cast->call(2, arg_test, &octet::octet_class);
		if(arg_cast_result->isInstanceOf(&thrown_exception::thrown_exception_class)) {
			return arg_cast_result;
		}
		other = dynamic_cast<octet*>(arg_cast_result);
	}
	return 0;
}

static object* get_octet_integer_params(list* param, octet*& self, integer*& other) {
	self = dynamic_cast<octet*>(param->getObject(0));
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

unsigned int octet::getHash()
{
	if(!hash) {
		std::wstringstream obj_repr;
		obj_repr << L"o" << value;
		hash = digest(obj_repr.str());
	}
	return hash;
}

/********************************
 NATIVE METHODS
 ********************************/

object* octet::operator_cast(list* param)
{
	octet* self = dynamic_cast<octet*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	object* target_class = param->getObject(1);
	if(target_class == &octet::octet_class) {
		return self;
	} else if(target_class->isInstanceOf(&integer::integer_class)) {
		return new integer((int)(self->value));
	} else if(target_class->isInstanceOf(&real::real_class)) {
		return new real((double)(self->value));
	} else if(target_class->isInstanceOf(&str::str_class)) {
		std::wstringstream obj_repr;
		if(self->value<16) {
			obj_repr << L"0x0" << std::hex << self->value;
		} else {
			obj_repr << L"0x" << std::hex << self->value;
		}
		return new str(obj_repr.str());
	}
	std::wstringstream err;
	err << L"Cannot cast '" << self->getClassname() << "' to '" << target_class->getClassname() << "'";
	return new thrown_exception(new exception(err.str()));
}

object* octet::operator_bigger(list* param)
{
	octet *self, *other;
	object* result = get_2_octet_params(param, self, other);
	if(result) {
		return result;
	}
	if (self->value > other->value) {
		return &boolean::boolean_true;
	} else {
		return &boolean::boolean_false;
	}
}

object* octet::operator_bigger_eq(list* param)
{
	octet *self, *other;
	object* result = get_2_octet_params(param, self, other);
	if(result) {
		return result;
	}
	if (self->value >= other->value) {
		return &boolean::boolean_true;
	} else {
		return &boolean::boolean_false;
	}
}

object* octet::operator_lower(list* param)
{
	return (operator_bigger_eq(param) == &boolean::boolean_false) ? &boolean::boolean_true : &boolean::boolean_false;
}

object* octet::operator_lower_eq(list* param)
{
	return (operator_bigger(param) == &boolean::boolean_false) ? &boolean::boolean_true : &boolean::boolean_false;
}

object* octet::operator_add(list* param)
{
	octet *self, *other;
	object* result = get_2_octet_params(param, self, other);
	if(result) {
		return result;
	}
	return get(self->value + other->value);
}

object* octet::operator_sub(list* param)
{
	octet *self, *other;
	object* result = get_2_octet_params(param, self, other);
	if(result) {
		return result;
	}
	return get(self->value - other->value);
}

object* octet::operator_mul(list* param)
{
	octet *self, *other;
	object* result = get_2_octet_params(param, self, other);
	if(result) {
		return result;
	}
	return get(self->value * other->value);
}

object* octet::operator_div(list* param)
{
	octet *self, *other;
	object* result = get_2_octet_params(param, self, other);
	if(result) {
		return result;
	}
	if (other->value == 0) {
		return new thrown_exception(new exception(L"Division by zero"));
	}
	return get(self->value / other->value);
}

object* octet::operator_mod(list* param)
{
	octet *self, *other;
	object* result = get_2_octet_params(param, self, other);
	if(result) {
		return result;
	}
	if (other->value == 0) {
		return new thrown_exception(new exception(L"Modulus by zero"));
	}
	return get(self->value % other->value);
}

object* octet::operator_lshift(list* param)
{
	octet *self;
	integer *other;
	object* result = get_octet_integer_params(param, self, other);
	if(result) {
		return result;
	}
	return get((unsigned char)(self->value << other->value));
}

object* octet::operator_rshift(list* param)
{
	octet *self;
	integer *other;
	object* result = get_octet_integer_params(param, self, other);
	if(result) {
		return result;
	}
	return get((unsigned char)(self->value >> other->value));
}

object* octet::operator_bit_and(list* param)
{
	octet *self, *other;
	object* result = get_2_octet_params(param, self, other);
	if(result) {
		return result;
	}
	return get(self->value & other->value);
}

object* octet::operator_bit_or(list* param)
{
	octet *self, *other;
	object* result = get_2_octet_params(param, self, other);
	if(result) {
		return result;
	}
	return get(self->value | other->value);
}

object* octet::operator_bit_xor(list* param)
{
	octet *self, *other;
	object* result = get_2_octet_params(param, self, other);
	if(result) {
		return result;
	}
	return get(self->value ^ other->value);
}

object* octet::operator_bit_not(list* param)
{
	octet* self = dynamic_cast<octet*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	return get(~self->value);
}

object* octet::read(list* param)
{
	octet* self = dynamic_cast<octet*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	rwstream* stream = dynamic_cast<rwstream*>(param->getObject(1));
	if(!stream) {
		return new thrown_exception(new exception(L"Invalid arguments"));
	}
	unsigned char value_read;
	object* result = stream->readDataToBuffer((char*)&value_read, sizeof(value_read));
	if(result) {
		return result;
	}
	return get(value_read);
}

object* octet::write(list* param)
{
	octet* self = dynamic_cast<octet*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	rwstream* stream = dynamic_cast<rwstream*>(param->getObject(1));
	if(!stream) {
		return new thrown_exception(new exception(L"Invalid arguments"));
	}
	unsigned char value_write = self->value;
	object* result = stream->writeDataFromBuffer((char*)&value_write, sizeof(value_write));
	if(result) {
		return result;
	}
	return &object::object_none;
}