//
//  gc.cpp
//  E#
//
//  Created by Adrian Punga on 01/07/2012.
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
#include "gc.h"

// Class setup dependencies
#include "list.h"
#include "method.h"
#include "boolean.h"
#include "exception.h"
#include "thrown_exception.h"
#include "gc.h"

// Specific dependencies
#include "runtime.h"
#include <iostream>

using namespace esharp;

/********************************
 CLASS SETUP
 ********************************/

gc gc::gc_class;
int gc::step_count=0;

gc::gc()
:object(&object::object_class)
{
	step = 1;
	setClassname(L"gc_class");
	setMethod(L"set_step(1)", new method(set_step));
}

gc::~gc()
{
}

/********************************
 HELPER METHODS
 ********************************/

void gc::addPointer(object* pointer)
{
#ifdef GC_DEBUG
	std::cout << "addPointer(" << pointer << ")" << std::endl;
#endif /* GC_DEBUG */
	pointers.insert(pointer);
}

void gc::removePointer(object* pointer)
{
#ifdef GC_DEBUG
	std::cout << "removePointer(" << pointer << ")" << std::endl;
#endif /* GC_DEBUG */
	es_set<object*>::iterator i = pointers.find(pointer);
	if(i!=pointers.end()) {
		pointers.erase(i);
	}
}

void gc::runGC(object* except_pointer) {
	if(step_count%step!=0) {
		return;
	}
	step_count++;
	if(except_pointer && except_pointer->isInstanceOf(&thrown_exception::thrown_exception_class)) {
		//What we really want to avoid collecting is the exception returned
		except_pointer = dynamic_cast<thrown_exception*>(except_pointer)->getException();
	}
	//DEBUG//std::cout << "runGC()" << std::endl;
	for(es_set<object*>::iterator i=pointers.begin();i!=pointers.end();) {
		object* pointer = *(i);
#ifdef GC_DEBUG
		std::wcout << L"======GC=CYCLE====[";
		str* pointer_str = dynamic_cast<str*>(pointer);
		if(pointer_str)
			std::wcout << pointer_str->getValue();
		real* pointer_real = dynamic_cast<real*>(pointer);
		if(pointer_real)
			std::wcout << pointer_real->getValue();
		integer* pointer_integer = dynamic_cast<integer*>(pointer);
		if(pointer_integer)
			std::wcout << pointer_integer->getValue();
		std::wcout << L"]<" << pointer << L"-" << pointer->getClassname() << ">======" << std::endl; 
#endif /* GC_DEBUG */
		if(pointer!=except_pointer && !runtime::runtime_class.getCurrentObject()->pointerFound(pointer)) {
#ifdef GC_DEBUG
			std::wcout << L"------GC-COLLECT--" << pointer << L"-" << pointer->getClassname() << "------" << std::endl;
#endif /* GC_DEBUG */
			delete pointer;
#ifdef WIN32
			i = 
#endif /* WIN32 */
            pointers.erase(i);
		}
#ifdef WIN32
		else {
#endif /* WIN32 */
			i++;
#ifdef WIN32
		}
#endif /* WIN32 */
	}
}

/********************************
 NATIVE METHODS
 ********************************/

object* gc::set_step(list* param)
{
	gc* self = dynamic_cast<gc*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	integer* step_param = dynamic_cast<integer*>(param->getObject(1));
	if(step_param->value<=0) {
		return new thrown_exception(new exception(L"Garbage collector step should be a positive number."));
	}
	self->step = step_param->value;
	return &object::object_none;
}
