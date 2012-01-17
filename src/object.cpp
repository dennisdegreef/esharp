//
//  object.cpp
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
#include "object.h"

// Class setup dependencies
#include "list.h"
#include "method.h"
#include "boolean.h"
#include "exception.h"
#include "thrown_exception.h"

// Specific dependencies
#include "runtime.h"
#include "gc.h"
#include "str.h"
#include "scope.h"
#include <sstream>

using namespace esharp;

/********************************
 CLASS SETUP
 ********************************/

object object::object_class;
object object::object_none(&object::object_class, L"none");
object object::object_break(&object::object_class);
object object::object_continue(&object::object_class);

object::object()
:ancestor(0)
{
	methods = 0;
	docstrings = 0;
	properties = 0;
	properties_reverse = 0;
	hash = 0;
	setClassname(L"object_class");
    setMethod(L"new(0)", new method(new_instance),
		L"object <- new()"
		L"\n\tCreates and returns a new instance of this class");
    setMethod(L"operator_cast(1)", new method(operator_cast));
    setMethod(L"operator_logic_or(1)", new method(default_binary_operator));
    setMethod(L"operator_logic_and(1)", new method(default_binary_operator));
    setMethod(L"operator_bit_or(1)", new method(default_binary_operator));
    setMethod(L"operator_bit_xor(1)", new method(default_binary_operator));
    setMethod(L"operator_bit_and(1)", new method(default_binary_operator));
    setMethod(L"operator_eq(1)", new method(operator_eq),
		L"boolean <- operator_eq(object <other>)"
		L"\n\tReturns true if this object is the same as <other>"
		L"\n\tIt should be reimplemented in subclasses to reflect equality of values.");
    setMethod(L"operator_neq(1)", new method(operator_neq),
		L"boolean <- operator_neq(object <other>)"
		L"\n\tReturns true if this object is not the same as <other>"
		L"\n\tIt should be reimplemented in subclasses to reflect inequality of values.");
    setMethod(L"operator_bigger(1)", new method(default_binary_operator));
    setMethod(L"operator_bigger_eq(1)", new method(default_binary_operator));
    setMethod(L"operator_lower(1)", new method(default_binary_operator));
    setMethod(L"operator_lower_eq(1)", new method(default_binary_operator));
    setMethod(L"operator_lshift(1)", new method(default_binary_operator));
    setMethod(L"operator_rshift(1)", new method(default_binary_operator));
    setMethod(L"operator_add(1)", new method(default_binary_operator));
    setMethod(L"operator_sub(1)", new method(default_binary_operator));
    setMethod(L"operator_mul(1)", new method(default_binary_operator));
    setMethod(L"operator_div(1)", new method(default_binary_operator));
    setMethod(L"operator_mod(1)", new method(default_binary_operator));
    setMethod(L"operator_minus(0)", new method(default_unary_operator));
    setMethod(L"operator_plus(0)", new method(default_unary_operator));
    setMethod(L"operator_logic_not(0)", new method(default_unary_operator));
    setMethod(L"operator_bit_not(0)", new method(default_unary_operator));
	setMethod(L"doc(0)", new method(doc), 
		L"str <- object.doc()"
		L"\n\tReturns the documentation, if any, for all methods in the class.");
	setMethod(L"doc(1)", new method(doc), 
		L"str <- object.doc(str <methodName>)"
		L"\n\tReturns the documentation, if any, for a method named <methodName>.");
}

object::object(object* ancestor_param, const wchar_t* classname_param)
:ancestor(ancestor_param)
{
	methods = 0;
	docstrings = 0;
	properties = 0;
	properties_reverse = 0;
	hash = 0;
	setClassname(classname_param);
}
/*
object::object(object* ancestor_param, const std::wstring& classname_param)
:ancestor(ancestor_param)
{
	methods = 0;
	docstrings = 0;
	properties = 0;
	properties_reverse = 0;
	hash = 0;
	setClassname(classname_param);
}
*/
object::~object()
{
	if(methods) {
		for(es_map<std::wstring,method*>::iterator i=methods->begin();i!=methods->end();i++) {
			delete i->second;
		}
	}
	if(classname_user_defined) {
		delete classname;
	}
	delete methods;
	delete docstrings;
	delete properties;
	delete properties_reverse;
}

void * object::operator new(size_t num_bytes) {
	char* pointer = ::new char[num_bytes];
	//DEBUG//std::cout << num_bytes << " bytes" <<std::endl;
	gc::gc_class.addPointer(reinterpret_cast<object*>(pointer));
	return pointer;
}

void object::operator delete(void* ptr) {
	char* pointer = reinterpret_cast<char*>(ptr);
	::delete [] pointer;
}

/********************************
 HELPER METHODS
 ********************************/

bool object::pointerFound(object* needle) {
	std::set<object*> visited;
	// Check in class scope
	if(isOrContainsPointer(needle, visited)) {
		return true;
	}
	// Check recursively in scopes
	scope* s = runtime::runtime_class.getCurrentScope();
	while(s!=0) {
		if(s->isOrContainsPointer(needle, visited)) {
			return true;
		}
		s = s->getParent();
	}
	return false;
}

bool object::isOrContainsPointer(object* pointer, std::set<object*>& visited, int level)
{
#ifdef GC_DEBUG
	for(int i=0;i<level;i++) {
		std::wcout << "  "; 
	}
	std::wcout << L"[";
	str* pointer_str = dynamic_cast<str*>(this);
	if(pointer_str)
		std::wcout << pointer_str->getValue();
	real* pointer_real = dynamic_cast<real*>(this);
	if(pointer_real)
		std::wcout << pointer_real->getValue();
	integer* pointer_integer = dynamic_cast<integer*>(this);
	if(pointer_integer)
		std::wcout << pointer_integer->getValue();
	std::wcout << L"]<" << this->getClassname() << L">" << std::endl;
#endif /* GC_DEBUG */
	if(this==pointer) {
#ifdef GC_DEBUG
		for(int i=0;i<level+1;i++) {
			std::wcout << "  "; 
		}
		std::wcout << "FOUND:S" << std::endl; 
#endif /* GC_DEBUG */
		return true;
	}
	if(properties) {
		for(es_map<std::wstring,object*>::iterator i=properties->begin(); i!=properties->end(); i++) {
			if(visited.find(i->second)==visited.end()) {
				visited.insert(i->second);
				if(i->second==pointer || i->second->isOrContainsPointer(pointer, visited, level+1)) {
#ifdef GC_DEBUG
					for(int i=0;i<level+1;i++) {
						std::wcout << "  "; 
					}
					std::wcout << "FOUND:P" << std::endl;
#endif /* GC_DEBUG */
					return true;
				}
			}
		}
	}
	if(ancestor) {
		if(visited.find(ancestor)==visited.end()) {
			visited.insert(ancestor);
			if(ancestor==pointer || ancestor->isOrContainsPointer(pointer, visited, level+1)) {
#ifdef GC_DEBUG
				for(int i=0;i<level+1;i++) {
					std::wcout << "  "; 
				}
				std::wcout << "FOUND:A" << std::endl; 
#endif /* GC_DEBUG */
				return true;
			}
		}
	}
	return false;
}

method* object::getMethod(const std::wstring& name)
{
	if(!methods) {
        if (ancestor!=0) {
            return ancestor->getMethod(name);
        } else {
			return &method::method_class;
        }
	}
    es_map<std::wstring, method*>::iterator method_iterator = methods->find(name);
    if (method_iterator != methods->end()) {
        return method_iterator->second;
    } else {
        if (ancestor!=0) {
            return ancestor->getMethod(name);
        } else {
			return &method::method_class;
        }
    }
}

str* object::getMethodDoc(const std::wstring& name)
{
	if(!docstrings) {
        if (ancestor!=0) {
            return ancestor->getMethodDoc(name);
        } else {
			return &str::str_class;
        }
	}
    es_map<std::wstring, std::wstring>::iterator docstrings_iterator = docstrings->find(name);
    if (docstrings_iterator != docstrings->end()) {
        return new str(docstrings_iterator->second);
    } else {
        if (ancestor!=0) {
            return ancestor->getMethodDoc(name);
        } else {
			return &str::str_class;
        }
    }
}

void object::setMethod(const std::wstring& name, method* methodObject, const std::wstring& docstring)
{
	if(!methods) {
		methods = new es_map<std::wstring, method*>();
		docstrings = new es_map<std::wstring, std::wstring>();
	}
	(*methods)[name] = methodObject;
	if(docstring!=L"") {
		(*docstrings)[name] = docstring;
	}
}

object* object::getProperty(const std::wstring& name)
{
	if(!properties) {
		return 0;
	}
    es_map<std::wstring, object*>::iterator property_iterator = properties->find(name);
    if (property_iterator != properties->end()) {
        return property_iterator->second;
    } else {
		return 0;
    }
}

void object::setProperty(const std::wstring& name, object* propertyValue, object* oldValue)
{
	if(!properties) {
		properties = new es_map<std::wstring, object*>();
		properties_reverse = new es_set<object*>();
	}
    (*properties)[name] = propertyValue;
	if(oldValue) {
		es_set<object*>::iterator oldPos = properties_reverse->find(oldValue);
		if(oldPos!=properties_reverse->end()) {
			properties_reverse->erase(oldPos);
		}
	}
    properties_reverse->insert(propertyValue);
}

void object::delProperty(const std::wstring& name, object* oldValue)
{
	if(!properties) {
		return;
	}
	es_map<std::wstring,object*>::iterator i = properties->find(name);
	if(i!=properties->end()) {
		properties->erase(i);
	}
	if(oldValue) {
		es_set<object*>::iterator oldPos = properties_reverse->find(oldValue);
		if(oldPos!=properties_reverse->end()) {
			properties_reverse->erase(oldPos);
		}
	}
}

bool object::isInstanceOf(object* ancestor_param)
{
	if(this==ancestor_param) {
		return true;
	}
	if(ancestor) {
		if(ancestor==ancestor_param) {
			return true;
		} else {
			return ancestor->isInstanceOf(ancestor_param);
		}
	}
	return false;
}

const wchar_t* object::getClassname()
{
	if(this->classname) {
		return this->classname;
	} else {
		if(this->ancestor) {
			return this->ancestor->getClassname();
		} else {
			return object::object_class.getClassname();
		}
	}
}

void object::setClassname(const wchar_t* name)
{
	classname_user_defined = false;
	this->classname = name;
}

void object::setClassname(const std::wstring& name)
{
	classname_user_defined = true;
	wchar_t* classname_param_cstr = new wchar_t[name.length()+1];
	memcpy(classname_param_cstr, name.c_str(), sizeof(wchar_t)*(name.length()+1));
	this->classname = classname_param_cstr;
}

unsigned int esharp::digest(const std::wstring& key, unsigned int hash)
{
	const unsigned int mask = 0xF0000000 ;
	for( std::wstring::size_type i = 0 ; i < key.length() ; ++i )
	{
		hash = ( hash << 4U ) + key[i] ;
		unsigned int x = hash & mask ;
		if( x != 0 ) hash ^= ( x >> 24 ) ;
		hash &= ~x ;
	}
	return hash;
};

unsigned int object::getHash()
{
	if(!hash) {
		std::wstringstream obj_repr;
		obj_repr << this;
		hash = digest(obj_repr.str());
	}
	return hash;
}

/********************************
 NATIVE METHODS
 ********************************/

object* object::new_instance(list* param)
{
	object* self = param->getObject(0);
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	return new object(self, 0);
}

object* object::operator_cast(list* param)
{
	object* self = param->getObject(0);
	object* target_class = param->getObject(1);
	if(target_class == &object::object_class) {
		return self;
	} else if(target_class->isInstanceOf(&str::str_class)) {
		if(self==&object::object_none) {
			return new str(L"none");
		}
		std::wstringstream obj_repr;
		obj_repr << L"<" << self->getClassname() << L":" << self->ancestor->getClassname() << "@" << self << L">";
		return new str(obj_repr.str());
	}
	std::wstringstream err;
	err << L"Cannot cast '" << self->getClassname() << "' to '" << target_class->getClassname() << "'";
	return new thrown_exception(new exception(err.str()));
}

object* object::operator_eq(list* param)
{
	if (param->getObject(0) == param->getObject(1)) {
		return &boolean::boolean_true;
	} else {
		return &boolean::boolean_false;
	}
}

object* object::operator_neq(list* param)
{
	return (operator_eq(param) == &boolean::boolean_false) ? &boolean::boolean_true : &boolean::boolean_false;
}

object* object::default_binary_operator(list* param)
{
	return new thrown_exception(new exception(L"Operator not defined for arguments!"));
}

object* object::default_unary_operator(list* param)
{
	return new thrown_exception(new exception(L"Operator not defined for arguments!"));
}

object* object::doc(list* param)
{
	object* self = param->getObject(0);
	if(self->isInstanceOf(&thrown_exception::thrown_exception_class)) {
		return self;
	}
	if(param->getLength()==1) {
		std::wstringstream ret;
		ret << L"################################################################" << std::endl;
		if(self->methods) {
			ret << L" Class '" << self->getClassname() << L"' documents the following methods:" << std::endl;
		} else {
			ret << L" Class '" << self->getClassname() << L"' doesn't have any methods." << std::endl;
		}
		ret << L"################################################################" << std::endl << std::endl;
		if(self->methods) {
			for(es_map<std::wstring, method*>::iterator i=self->methods->begin(); i!=self->methods->end(); i++) {
				es_map<std::wstring, std::wstring>::iterator docstrings_iterator = self->docstrings->find(i->first);
				if (docstrings_iterator != self->docstrings->end()) {
					ret << docstrings_iterator->second << std::endl << std::endl;
				} 
			}
			ret << L"################################################################" << std::endl;
		}
		return new str(ret.str());
	} else if(param->getLength()==2) {
		str* mname = dynamic_cast<str*>(param->getObject(1));
		if(!mname) {
			return new thrown_exception(new exception(L"Invalid arguments!"));
		}
		str* mdoc = self->getMethodDoc(mname->getValue());
		if(mdoc==&str::str_class) {
			return new thrown_exception(new exception(L"Method not found!"));
		}
		return mdoc;
	} else {
		return new thrown_exception(new exception(L"Too many arguments!"));
	}
}
