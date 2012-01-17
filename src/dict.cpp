//
//  dict.cpp
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
#include "dict.h"

// Class setup dependencies
#include "list.h"
#include "method.h"
#include "boolean.h"
#include "exception.h"
#include "thrown_exception.h"

// Specific dependencies
#include "str.h"

using namespace esharp;

/********************************
 CLASS SETUP
 ********************************/

dict dict::dict_class;

bool ltobject::operator()(object* o1, object* o2) const
{
	return o1->getHash()<o2->getHash();
}

dict::dict()
:object(&object::object_class)
{
	setClassname(L"dict_class");
	setMethod(L"new(0)", new method(new_instance));
	setMethod(L"operator_cast(1)", new method(operator_cast));
	setMethod(L"operator_eq(1)", new method(operator_eq));
	setMethod(L"set(2)", new method(set));
	setMethod(L"get(1)", new method(get));
	setMethod(L"del(1)", new method(del));
}

dict::dict(void*)
:object(&dict::dict_class)
{
	setClassname(L"dict");
}

dict::~dict()
{
}

/********************************
 HELPER METHODS
 ********************************/

bool dict::isOrContainsPointer(object* pointer, std::set<object*>& visited, int level)
{
	if(object::isOrContainsPointer(pointer, visited, level+1)) {
		return true;
	}
	for(es_map<object*,object*,ltobject>::iterator i=values.begin(); i!=values.end(); i++) {
		if(visited.find(i->first)==visited.end()) {
			visited.insert(i->first);
			if(i->first==pointer || i->first->isOrContainsPointer(pointer, visited, level+1)) {
				return true;
			}
		}
		if(visited.find(i->second)==visited.end()) {
			visited.insert(i->second);
			if(i->second==pointer || i->second->isOrContainsPointer(pointer, visited, level+1)) {
				return true;
			}
		}
	}
	return false;
}

/********************************
 NATIVE METHODS
 ********************************/

object* dict::new_instance(list* param)
{
	return new dict(0);
}

object* dict::operator_cast(list* param)
{
	dict* self = dynamic_cast<dict*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	object* target_class = param->getObject(1);
	if(target_class == &dict::dict_class) {
		return self;
	} else if(target_class->isInstanceOf(&str::str_class)) {
		std::wstringstream dict_str;
		dict_str << L"{";
		for(es_map<object*,object*,ltobject>::iterator i=self->values.begin(); i!=self->values.end();) {
			object* key = i->first;
			method* key_cast = key->getMethod(L"operator_cast(1)");
			object* key_cast_result = key_cast->call(2, key, &str::str_class);
			if(key_cast_result->isInstanceOf(&thrown_exception::thrown_exception_class)) {
				return key_cast_result;
			}
			str* key_repr = dynamic_cast<str*>(key_cast_result);

			object* item = i->second;
			method* item_cast = item->getMethod(L"operator_cast(1)");
			object* item_cast_result = item_cast->call(2, item, &str::str_class);
			if(item_cast_result->isInstanceOf(&thrown_exception::thrown_exception_class)) {
				return item_cast_result;
			}
			str* item_repr = dynamic_cast<str*>(item_cast_result);

			dict_str << key_repr->getValue() << L":" << item_repr->getValue();

			i++;
			if(i!=self->values.end()) {
				dict_str << L", ";
			}
		}

		dict_str << L"}";
		return new str(dict_str.str());
	}
	std::wstringstream err;
	err << L"Cannot cast '" << self->getClassname() << "' to '" << target_class->getClassname() << "'";
	return new thrown_exception(new exception(err.str()));
}

object* dict::operator_eq(list* param)
{
	dict* self = dynamic_cast<dict*>(param->getObject(0));
	if (!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	dict* other = dynamic_cast<dict*>(param->getObject(1));
	if (!other) {
		return new thrown_exception(new exception(L"Invalid arguments"));
	}

	if(self->values.size() != other->values.size()) {
		return &boolean::boolean_false;
	}

	es_map<object*,object*,ltobject>::iterator i=self->values.begin();
	es_map<object*,object*,ltobject>::iterator j=other->values.begin();
	for(;i!=self->values.end(); i++, j++) {
		object* self_key = i->first;
		object* self_value = i->second;
		object* other_key = j->first;
		object* other_value = j->second;
		method* self_key_operator_eq = self_key->getMethod(L"operator_eq(1)");
		method* self_value_operator_eq = self_key->getMethod(L"operator_eq(1)");
		
		object* key_comp_result = self_key_operator_eq->call(2, self_key, other_key);
		if(key_comp_result->isInstanceOf(&thrown_exception::thrown_exception_class) || key_comp_result==&boolean::boolean_false) {
			return &boolean::boolean_false;
		}
		object* value_comp_result = self_value_operator_eq->call(2, self_value, other_value);
		if(value_comp_result->isInstanceOf(&thrown_exception::thrown_exception_class) || value_comp_result==&boolean::boolean_false) {
			return &boolean::boolean_false;
		}
	}

	return &boolean::boolean_true;
}

object* dict::set(list* param)
{
	dict* self = dynamic_cast<dict*>(param->getObject(0));
	if (!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	object* key = param->getObject(1);
	object* value = param->getObject(2);
	self->values[key] = value;
	return &object_none;
}

object* dict::get(list* param)
{
	dict* self = dynamic_cast<dict*>(param->getObject(0));
	if (!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	object* key = param->getObject(1);
	es_map<object*,object*,ltobject>::iterator value_iterator = self->values.find(key);
	if (value_iterator != self->values.end()) {
		return value_iterator->second;
	}
	return new thrown_exception(new exception(L"Key error"));
}

object* dict::del(list* param)
{
	dict* self = dynamic_cast<dict*>(param->getObject(0));
	if (!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	object* key = param->getObject(1);
	es_map<object*,object*,ltobject>::iterator value_iterator = self->values.find(key);
	if (value_iterator == self->values.end()) {
		return new thrown_exception(new exception(L"Key error"));
	}
	self->values.erase(value_iterator);
	return &object_none;
}
