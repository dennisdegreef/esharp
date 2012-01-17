//
//  rwstream.cpp
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
#include "rwstream.h"

// Class setup dependencies
#include "list.h"
#include "method.h"
#include "boolean.h"
#include "exception.h"
#include "thrown_exception.h"

// Specific dependencies
///NONE

using namespace esharp;

/********************************
 CLASS SETUP
 ********************************/

rwstream rwstream::rwstream_class;

rwstream::rwstream()
:object(&object::object_class)
{
	setClassname(L"rwstream_class");
	setMethod(L"read(1)", new method(read));
	setMethod(L"read(...)", new method(read));
	setMethod(L"write(1)", new method(write));
	setMethod(L"write(...)", new method(write));
}

rwstream::rwstream(object* ancestor_param)
:object(ancestor_param)
{
}

rwstream::~rwstream()
{
}

/********************************
 HELPER METHODS
 ********************************/

object* rwstream::availableData(unsigned int* available)
{
	return new thrown_exception(new exception(L"Cannot determine available data from abstract rwstream"));
}

object* rwstream::readDataToBuffer(char* buffer, unsigned int len)
{
	return new thrown_exception(new exception(L"Cannot read from abstract rwstream"));
}

object* rwstream::writeDataFromBuffer(char* buffer, unsigned int len)
{
	return new thrown_exception(new exception(L"Cannot write to abstract rwstream"));
}

object* rwstream::closeStream()
{
	return new thrown_exception(new exception(L"Cannot close abstract rwstream"));
}

/********************************
 NATIVE METHODS
 ********************************/

object* rwstream::read(list* param)
{
	rwstream* self = dynamic_cast<rwstream*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	object* type = param->getObject(1);
	method* read;
	if(param->getLength()>2) {
		read = type->getMethod(L"read(...)");
	} else {
		read = type->getMethod(L"read(1)");
	}
	if(read==&method::method_class) {
		std::wstringstream err;
		err << L"Cannot read objects of type '" << type->getClassname() << "' as they don't have a read(stream, ...) method.";
		return new thrown_exception(new exception(err.str()));
	} else {
		list args(0);
		args.appendObject(type);
		args.appendObject(self);
		for(int i=2;i<param->getLength();i++) {
			args.appendObject(param->getObject(i));
		}
		return read->call(&args);
	}
	return &object::object_none;
}
object* rwstream::write(list* param)
{
	rwstream* self = dynamic_cast<rwstream*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	object* content = param->getObject(1);
	//Try to find a write with any number of params first to allow customization on how to write certain classes
	method* write;
	if(param->getLength()>2) {
		write = content->getMethod(L"write(...)");
	} else {
		write = content->getMethod(L"write(1)");
	}
	if(write==&method::method_class) {
		std::wstringstream err;
		err << L"Cannot write objects of type '" << content->getClassname() << "' as they don't have a write(stream, ...) method.";
		return new thrown_exception(new exception(err.str()));
	} else {
		list args(0);
		args.appendObject(content);
		args.appendObject(self);
		for(int i=2;i<param->getLength();i++) {
			args.appendObject(param->getObject(i));
		}
		return write->call(&args);
	}
	return &object::object_none;
}

object* rwstream::close(list* param)
{
	rwstream* self = dynamic_cast<rwstream*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	object* result = self->closeStream();
	if(result) {
		return result;
	}
	return &object::object_none;
}
