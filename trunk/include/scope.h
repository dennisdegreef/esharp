//
//  scope.h
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

#ifndef E__scope_h
#define E__scope_h

#include "object_nogc.h"
#include <string>

namespace esharp {
	class scope: public object_nogc {
	private:
		scope* parent;
		scope();
    public:
		scope(void*);
        virtual ~scope();
		scope* getScopeForObject(const std::wstring& name, object** retOldValue);
		object* getNamedObject(const std::wstring& name);
		void setNamedObject(const std::wstring& name, object* object_param);
		void delNamedObject(const std::wstring& name);
		void delObject(object* object_param);
		scope* getParent();
		void setParent(scope* parent_param);
	public:
		static scope scope_class;
    };
}

#endif
