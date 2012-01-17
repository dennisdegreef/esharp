//
//  fs.cpp
//  E#
//
//  Created by Adrian Punga on 01/12/2012.
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
#include "fs.h"

// Class setup dependencies
#include "list.h"
#include "method.h"
#include "boolean.h"
#include "exception.h"
#include "thrown_exception.h"

// Specific dependencies
#include "boolean.h"
#include "octet.h"
#include "integer.h"
#include "real.h"
#include "data.h"
#include "str.h"
#include "list.h"
#include "dict.h"
#include "rwfilestream.h"
#include <sstream>

using namespace esharp;

/********************************
 CLASS SETUP
 ********************************/

fs fs::fs_class;

fs::fs()
:object(&object::object_class)
{
	setClassname(L"fs_class");
	setMethod(L"open(1)", new method(open));
	setMethod(L"open(2)", new method(open));
	setMethod(L"stat(1)", new method(stat));
}

fs::~fs()
{
}

/********************************
 HELPER METHODS
 ********************************/

///NONE

/********************************
 NATIVE METHODS
 ********************************/

object* fs::open(list* param)
{
	fs* self = dynamic_cast<fs*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	str* filename = dynamic_cast<str*>(param->getObject(1));
	if(!filename) {
		return new thrown_exception(new exception(L"Invalid arguments"));
	}
	object* encode_result;
	encode_result = filename->getMethod(L"encode_utf8(0)")->call(1, filename);
	data* filename_utf8 = dynamic_cast<data*>(encode_result);
	if(!filename_utf8) {
		return encode_result;
	}
	str* mode;
	str default_mode(L"r");
	if(param->getLength()==3) {
		mode = dynamic_cast<str*>(param->getObject(2));
		if(!mode) {
			return new thrown_exception(new exception(L"Invalid arguments"));
		}
	} else {
		mode = &default_mode;
	}
	encode_result = mode->getMethod(L"encode_utf8(0)")->call(1, mode);
	data* mode_utf8 = dynamic_cast<data*>(encode_result);
	if(!mode_utf8) {
		return encode_result;
	}
	FILE* file = fopen((const char*)filename_utf8->getValue(), (const char*)mode_utf8->getValue());
	if(!file) {
		std::wstringstream err;
		err << L"Could not open file '" << filename->getValue() << L"'";
		return new thrown_exception(new exception(err.str()));
	}
	return new rwfilestream(file);
}
object* fs::stat(list* param)
{
	return &object::object_none;
}
