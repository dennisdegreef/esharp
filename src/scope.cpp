//
//  scope.cpp
//  E#
//
//  Created by Adrian Punga on 01/05/2012.
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
#include "scope.h"

// Class setup dependencies
#include "exception.h"
#include "thrown_exception.h"

// Specific dependencies
#include <sstream>

using namespace esharp;

/********************************
 CLASS SETUP
 ********************************/

scope scope::scope_class;

scope::scope()
:object_nogc(&object::object_class)
{
	setClassname(L"scope_class");
	parent = 0;
}

scope::scope(void*)
:object_nogc(&scope::scope_class)
{
	setClassname(L"scope");
	parent = 0;
}

scope::~scope()
{
}

/********************************
 HELPER METHODS
 ********************************/

object* scope::getNamedObject(const std::wstring& name)
{
    object* prop = getProperty(name);
	if (prop) {
        return prop;
    } else {
        if (parent!=0) {
            return parent->getNamedObject(name);
        } else {
			std::wstringstream errs;
			errs << "Variable '" << name << "' could not be resolved.";
			return new thrown_exception(new exception(errs.str()));
        }
    }
}

scope* scope::getScopeForObject(const std::wstring& name, object** retOldValue)
{
    *retOldValue = getProperty(name);
	if (*retOldValue) {
        return this;
    } else {
        if (parent!=0) {
            return parent->getScopeForObject(name, retOldValue);
        } else {
			return 0;
        }
    }
}

void scope::setNamedObject(const std::wstring& name, object* object_param)
{
	object* old_value;
	scope* final_scope = getScopeForObject(name, &old_value);
	if(final_scope) {
		final_scope->setProperty(name, object_param, old_value);
	} else {
		setProperty(name, object_param);
	}
}

void scope::delNamedObject(const std::wstring& name)
{
	object* old_value;
	scope* final_scope = getScopeForObject(name, &old_value);
	if(final_scope) {
		final_scope->delProperty(name, old_value);
	} else {
		delProperty(name, old_value);
	}
}

scope* scope::getParent()
{
	return parent;
}

void scope::setParent(scope* parent_param)
{
	parent = parent_param;
}

/********************************
 NATIVE METHODS
 ********************************/

///NONE


