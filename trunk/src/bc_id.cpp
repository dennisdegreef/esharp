//
//  bc_id.cpp
//  E#
//
//  Created by Adrian Punga on 01/04/2012.
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

#include "bc_id.h"
#include "runtime.h"

using namespace esharp;

bc_id::bc_id()
:bc_expr()
{
}

bc_id::bc_id(Token pTree)
:bc_expr(pTree)
{
	unsigned int length = GetTextLen(pTree);
	wchar_t* temp = new wchar_t[length+1];
	mbstowcs(temp, GetText(pTree), length+1);
	this->name = temp;
	delete[] temp;
}

bc_id::~bc_id()
{
}

bc_id* bc_id::clone()
{
	bc_id* copy = new bc_id();
	copy->name = name;
	copy->line = line;
	return copy;
}

object* bc_id::evaluate()
{
	object* prop = runtime::runtime_class.getCurrentObject()->getProperty(this->name);
	if(prop) {
		return prop;
	} else {
		object* result = runtime::runtime_class.getCurrentScope()->getNamedObject(this->name);
		if(result->isInstanceOf(&thrown_exception::thrown_exception_class)) {
			thrown_exception* texc = dynamic_cast<thrown_exception*>(result);
			std::wstringstream trace_method;
			trace_method << L"  File \"" << runtime::runtime_class.getCurrentSource() << "\", line " << this->line;
			texc->getException()->addTrace(trace_method.str());
		}
		return result;
	}
}
