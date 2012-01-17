//
//  rwfilestream.cpp
//  E#
//
//  Created by Adrian Punga on 01/11/2012.
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
#include "rwfilestream.h"

// Class setup dependencies
#include "list.h"
#include "method.h"
#include "boolean.h"
#include "exception.h"
#include "thrown_exception.h"

// Specific dependencies
#include "octet.h"
#include "integer.h"
#include "real.h"
#include "data.h"
#include "str.h"
#include "list.h"
#include "dict.h"
#include <sstream>

using namespace esharp;

/********************************
 CLASS SETUP
 ********************************/

rwfilestream rwfilestream::rwfilestream_class;

rwfilestream::rwfilestream()
:rwstream(&rwstream::rwstream_class)
{
	setClassname(L"rwfilestream_class");
	setMethod(L"close(0)", new method(close));
}

rwfilestream::rwfilestream(FILE* file_param)
:rwstream(&rwfilestream::rwfilestream_class)
{
	setClassname(L"rwfilestream");
	file = file_param;
}

rwfilestream::~rwfilestream()
{
}

/********************************
 HELPER METHODS
 ********************************/

object* rwfilestream::availableData(unsigned int* available)
{
	long int cpos = ftell(file);
	if(cpos==-1L) {
		return new thrown_exception(new exception(L"Could not determine available data"));
	}
	int seekr = fseek(file, 0, SEEK_END);
	if(seekr) {
		return new thrown_exception(new exception(L"Could not determine available data"));
	}
	long int fpos = ftell(file);
	if(cpos==-1L) {
		return new thrown_exception(new exception(L"Could not determine available data"));
	}
	seekr = fseek(file, cpos, SEEK_SET);
	if(seekr) {
		return new thrown_exception(new exception(L"Could not determine available data"));
	}
	*available = (unsigned int)(fpos - cpos);
	return 0;
}

object* rwfilestream::readDataToBuffer(char* buffer, unsigned int len)
{
	int result = fread(buffer, 1, len, file);
	if(result != len) {
		if(feof(file)) {
			return new thrown_exception(new exception(L"End of file reached while trying to read"));
		} else {
			return new thrown_exception(new exception(L"Could not read from file"));
		}
	}
	return 0;
}

object* rwfilestream::writeDataFromBuffer(char* buffer, unsigned int len)
{
	int result = fwrite(buffer, 1, len, file);
	if(result != len) {
		return new thrown_exception(new exception(L"Could not write to file"));
	}
	return 0;
}

object* rwfilestream::closeStream()
{
	int result = fclose(file);
	if(result == EOF) {
		return new thrown_exception(new exception(L"Could not close to file"));
	}
	return 0;
}

/********************************
 NATIVE METHODS
 ********************************/

///NONE
