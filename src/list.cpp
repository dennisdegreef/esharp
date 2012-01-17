//
//  list.cpp
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
#include "list.h"

// Class setup dependencies
#include "list.h"
#include "method.h"
#include "boolean.h"
#include "exception.h"
#include "thrown_exception.h"

// Specific dependencies
#include "integer.h"
#include "str.h"
#include "dict.h"
#include <sstream>

using namespace esharp;

/********************************
 CLASS SETUP
 ********************************/

list list::list_class;

list::list()
:object(&object::object_class)
{
	setClassname(L"list_class");
	setMethod(L"new(0)", new method(new_instance));
	setMethod(L"new(...)", new method(new_instance_varargs));
	setMethod(L"operator_cast(1)", new method(operator_cast));
	setMethod(L"operator_eq(1)", new method(operator_eq));
	setMethod(L"append(...)", new method(append));
	setMethod(L"len(0)", new method(len));
	setMethod(L"get(1)", new method(get));
	setMethod(L"set(2)", new method(set));
	setMethod(L"del(1)", new method(del));
}

list::list(int initial_size)
:object(&list::list_class)
{
	setClassname(L"list");
	values = std::vector<object*>(initial_size);
}

list::~list()
{
}

/********************************
 HELPER METHODS
 ********************************/

int list::getLength()
{
	return values.size();
}

list* list::getSlice(int position_start, int size)
{
	if(size==-1 || (unsigned int)(size+position_start)>values.size()) {
		size = values.size()-position_start;
	}
	list* slice = new list(0);
	for(int i=0; i<size; i++) {
		slice->values.push_back(values[i+position_start]);
	}
	return slice;
}

object* list::getObject(int position)
{
	return values[position];
}

void list::appendObject(object* param)
{
	values.push_back(param);
}

void list::clearList()
{
	values.clear();
}

bool list::isOrContainsPointer(object* pointer, std::set<object*>& visited, int level)
{
	if(object::isOrContainsPointer(pointer, visited, level+1)) {
		return true;
	}
	for(std::vector<object*>::iterator i=values.begin(); i!=values.end(); i++) {
		if(visited.find(*i)==visited.end()) {
			visited.insert(*i);
			if((*i)==pointer || (*i)->isOrContainsPointer(pointer, visited, level+1)) {
				return true;
			}
		}
	}
	return false;
}

/********************************
 NATIVE METHODS
 ********************************/

object* list::new_instance(list* param)
{
	list* self = dynamic_cast<list*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	list* newInstance = new list(0);
	newInstance->ancestor = self;
	return newInstance;
}

object* list::new_instance_varargs(list* param)
{
	list* self = dynamic_cast<list*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	list* newInstance = new list(0);
	newInstance->ancestor = self;
	for(int i=1;i<param->getLength();i++) {
		object* item = param->getObject(i);
		newInstance->values.push_back(item);
	}
	return newInstance;
}

object* list::operator_cast(list* param)
{
	list* self = dynamic_cast<list*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	object* target_class = param->getObject(1);
	if(target_class == &list::list_class) {
		return self;
	} else if(target_class->isInstanceOf(&str::str_class)) {
		std::wstringstream list_str;
		list_str << L"[";
		for(int i=0;i<self->getLength();i++) {
			object* item = self->getObject(i);
			method* item_cast = item->getMethod(L"operator_cast(1)");
			object* cast_result = item_cast->call(2, item, &str::str_class);
			if(cast_result->isInstanceOf(&thrown_exception::thrown_exception_class)) {
				return cast_result;
			}
			str* item_repr = dynamic_cast<str*>(cast_result);
			list_str << item_repr->getValue();
			if(i<self->getLength()-1) {
				list_str << L", ";
			}
		}
		list_str << L"]";
		return new str(list_str.str());
	}
	std::wstringstream err;
	err << L"Cannot cast '" << self->getClassname() << "' to '" << target_class->getClassname() << "'";
	return new thrown_exception(new exception(err.str()));

}

object* list::operator_eq(list* param)
{
	list* self = dynamic_cast<list*>(param->getObject(0));
	if (!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	list* other = dynamic_cast<list*>(param->getObject(1));
	if (!other) {
		return new thrown_exception(new exception(L"Invalid arguments"));
	}
	if (self->values.size() != other->values.size()) {
		return &boolean::boolean_false;
	}
	std::vector<object*>::iterator i=self->values.begin();
	std::vector<object*>::iterator j=other->values.begin();
	for(;i!=self->values.end(); i++, j++) {
		object* self_value = *(i);
		object* other_value = *(j);
		method* self_value_operator_eq = self_value->getMethod(L"operator_eq(1)");
		
		object* value_comp_result = self_value_operator_eq->call(2, self_value, other_value);
		if(value_comp_result->isInstanceOf(&thrown_exception::thrown_exception_class) || value_comp_result==&boolean::boolean_false) {
			return &boolean::boolean_false;
		}
	}
	return &boolean::boolean_true;
}

object* list::len(list* param)
{
	list* self = dynamic_cast<list*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	return new integer(self->values.size());
}

object* list::append(list* param)
{
	list* self = dynamic_cast<list*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	for(int i=1;i<param->getLength();i++) {
		object* item = param->getObject(i);
		self->values.push_back(item);
	}
	return &object::object_none;
}

object* list::get(list* param)
{
	list* self = dynamic_cast<list*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	integer* pos_obj = dynamic_cast<integer*>(param->getObject(1));
	if(!pos_obj) {
		return new thrown_exception(new exception(L"List index has to be 'integer'"));
	}
	if(pos_obj->value<0 || pos_obj->value>=self->getLength()) {
		return new thrown_exception(new exception(L"List index outside list bounds"));
	}
	return self->values[pos_obj->value];
}

object* list::set(list* param)
{
	list* self = dynamic_cast<list*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	integer* pos_obj = dynamic_cast<integer*>(param->getObject(1));
	if(!pos_obj) {
		return new thrown_exception(new exception(L"List index has to be 'integer'"));
	}
	if(pos_obj->value<0 || pos_obj->value>=self->getLength()) {
		return new thrown_exception(new exception(L"List index outside list bounds"));
	}
	object* item = param->getObject(2);
	self->values[pos_obj->value] = item;
	return &object::object_none;
}

object* list::del(list* param)
{
	list* self = dynamic_cast<list*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	integer* pos_obj = dynamic_cast<integer*>(param->getObject(1));
	if(!pos_obj) {
		return new thrown_exception(new exception(L"List index has to be 'integer'"));
	}
	if(pos_obj->value<0 || pos_obj->value>=self->getLength()) {
		return new thrown_exception(new exception(L"List index outside list bounds"));
	}
	self->values.erase(self->values.begin()+pos_obj->value);
	return &object::object_none;
}

