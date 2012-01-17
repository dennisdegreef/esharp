//
//  str.cpp
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
#include "str.h"

// Class setup dependencies
#include "list.h"
#include "method.h"
#include "boolean.h"
#include "exception.h"
#include "thrown_exception.h"

// Specific dependencies
#include "integer.h"
#include "data.h"

using namespace esharp;

/********************************
 CLASS SETUP
 ********************************/

str str::str_class;
str str::str_empty(L"");

str::str()
:object(&object::object_class)
{
	setClassname(L"str_class");
    setMethod(L"new(0)", new method(new_instance));
    setMethod(L"operator_cast(1)", new method(operator_cast));
    setMethod(L"operator_eq(1)", new method(operator_eq));
    setMethod(L"operator_bigger(1)", new method(operator_bigger));
    setMethod(L"operator_bigger_eq(1)", new method(operator_bigger_eq));
    setMethod(L"operator_lower(1)", new method(operator_lower));
    setMethod(L"operator_lower_eq(1)", new method(operator_lower_eq));
    setMethod(L"len(0)", new method(len));
    setMethod(L"encode_utf8(0)", new method(encode_utf8));
    setMethod(L"encode_ascii(0)", new method(encode_ascii));
    setMethod(L"encode_ascii_strict(0)", new method(encode_ascii_strict));
}

str::str(const std::wstring& string_param)
:object(&str::str_class)
{
	setClassname(L"str");
	value = string_param;
}

str::~str()
{
}

/********************************
 HELPER METHODS
 ********************************/

void str::setValue(const std::wstring& string_param)
{
	value = string_param;
}

const std::wstring& str::getValue()
{
	return value;
}

int str::length()
{
	return value.length();
}

unsigned int str::getHash()
{
	if(!hash) {
		std::wstringstream obj_repr;
		obj_repr << L"\"" << getValue() << L"\"";
		hash = digest(obj_repr.str());
	}
	return hash;
}

/********************************
 NATIVE METHODS
 ********************************/

object* str::new_instance(list* param)
{
	str* self = dynamic_cast<str*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	str* newInstance = new str(L"");
	newInstance->ancestor = self;
	return newInstance;
}

object* str::operator_cast(list* param)
{
	str* self = dynamic_cast<str*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	object* target_class = param->getObject(1);
	if(target_class == &str::str_class) {
		return self;
	}
	std::wstringstream err;
	err << L"Cannot cast '" << self->getClassname() << "' to '" << target_class->getClassname() << "'";
	return new thrown_exception(new exception(err.str()));
}

object* str::operator_eq(list* param)
{
	str* self = dynamic_cast<str*>(param->getObject(0));
	if (!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	str* other = dynamic_cast<str*>(param->getObject(1));
	if (!other) {
		return new thrown_exception(new exception(L"Invalid arguments"));
	}
	if (self->value == other->value) {
		return &boolean::boolean_true;
	} else {
		return &boolean::boolean_false;
	}
}

object* str::operator_bigger(list* param)
{
	str* self = dynamic_cast<str*>(param->getObject(0));
	if (!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	str* other = dynamic_cast<str*>(param->getObject(1));
	if (!other) {
		return new thrown_exception(new exception(L"Invalid arguments"));
	}
	if (self->value > other->value) {
		return &boolean::boolean_true;
	} else {
		return &boolean::boolean_false;
	}
}

object* str::operator_bigger_eq(list* param)
{
	str* self = dynamic_cast<str*>(param->getObject(0));
	if (!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	str* other = dynamic_cast<str*>(param->getObject(1));
	if (!other) {
		return new thrown_exception(new exception(L"Invalid arguments"));
	}
	if (self->value >= other->value) {
		return &boolean::boolean_true;
	} else {
		return &boolean::boolean_false;
	}
}

object* str::operator_lower(list* param)
{
	str* self = dynamic_cast<str*>(param->getObject(0));
	if (!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	str* other = dynamic_cast<str*>(param->getObject(1));
	if (!other) {
		return new thrown_exception(new exception(L"Invalid arguments"));
	}
	if (self->value < other->value) {
		return &boolean::boolean_true;
	} else {
		return &boolean::boolean_false;
	}
}

object* str::operator_lower_eq(list* param)
{
	str* self = dynamic_cast<str*>(param->getObject(0));
	if (!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	str* other = dynamic_cast<str*>(param->getObject(1));
	if (!other) {
		return new thrown_exception(new exception(L"Invalid arguments"));
	}
	if (self->value <= other->value) {
		return &boolean::boolean_true;
	} else {
		return &boolean::boolean_false;
	}
}

object* str::len(list* param)
{
	str* self = dynamic_cast<str*>(param->getObject(0));
	if (!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	return new integer(self->length());
}

static int utf8_encoded_len(const wchar_t* rawstr) {
	wchar_t* src = (wchar_t*)rawstr;
	int len = 0;
	while(*src) {
		wchar_t c = *src;
		if(c>0 && c<=0x7F) {
			len++;
		} else if(c==0 || (c>=0x80 && c<=0x7FF)) {
			len+=2;
		} else {
			len+=3;
		}
		src++;
	}
	return len;
}

object* str::encode_utf8(list* param)
{
	str* self = dynamic_cast<str*>(param->getObject(0));
	if (!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	int len = self->value.size();
	const wchar_t* src = self->value.c_str();
	int utf8len = utf8_encoded_len(src);
	unsigned char* dst = new unsigned char[utf8len];
	int srcpos = 0;
	int dstpos = 0;
	while(srcpos<len) {
		wchar_t c = src[srcpos];
		if(c>0 && c<=0x7F) {
			dst[dstpos++] = (unsigned char)c;
		} else if(c==0 || (c>=0x80 && c<=0x7FF)) {
			dst[dstpos++] = (unsigned char)(0xC0|((c&0x7C0)>>6));
			dst[dstpos++] = (unsigned char)(0x80|(c&0x3F));
		} else {
			dst[dstpos++] = (unsigned char)(0xE0|((c&0xF000)>>12));
			dst[dstpos++] = (unsigned char)(0x80|((c&0xFC0)>>6));
			dst[dstpos++] = (unsigned char)(0x80|(c&0x3F));
		}
		srcpos++;
	}
	data* ret = new data(dst, utf8len);
	delete [] dst;
	return ret;
}

object* str::encode_ascii(list* param)
{
	str* self = dynamic_cast<str*>(param->getObject(0));
	if (!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	int len = self->value.size();
	const wchar_t* src = self->value.c_str();
	unsigned char* dst = new unsigned char[len];
	int srcpos = 0;
	int dstpos = 0;
	while(srcpos<len) {
		wchar_t c = src[srcpos];
		if(c>0 && c<=0xFF) {
			dst[dstpos++] = (unsigned char)c;
		} else {
			dst[dstpos++] = '?';
		}
		srcpos++;
	}
	data* ret = new data(dst, len);
	delete [] dst;
	return ret;
}

object* str::encode_ascii_strict(list* param)
{
	str* self = dynamic_cast<str*>(param->getObject(0));
	if (!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	int len = self->value.size();
	const wchar_t* src = self->value.c_str();
	unsigned char* dst = new unsigned char[len];
	int srcpos = 0;
	int dstpos = 0;
	while(srcpos<len) {
		wchar_t c = src[srcpos];
		if(c>0 && c<=0xFF) {
			dst[dstpos++] = (unsigned char)c;
		} else {
			return new thrown_exception(new exception(L"Encode to ASCII failed"));
		}
		srcpos++;
	}
	data* ret = new data(dst, len);
	delete [] dst;
	return ret;
}
