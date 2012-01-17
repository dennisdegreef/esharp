//
//  object_nogc.cpp
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
#include "object_nogc.h"

// Class setup dependencies
///NONE

// Specific dependencies
///NONE

using namespace esharp;

/********************************
 CLASS SETUP
 ********************************/

object_nogc::object_nogc(object* ancestor_param)
:object(ancestor_param)
{
}

object_nogc::~object_nogc()
{
}

void * object_nogc::operator new(size_t num_bytes) {
	return ::new char[num_bytes];
}

void object_nogc::operator delete(void* ptr) {
	::delete [] reinterpret_cast<char*>(ptr);
}

/********************************
 HELPER METHODS
 ********************************/

///NONE

/********************************
 NATIVE METHODS
 ********************************/

///NONE
