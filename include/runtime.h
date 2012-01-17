//
//  runtime.h
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

#ifndef E__runtime_h
#define E__runtime_h

/* Types */
#include "object.h"
#include "boolean.h"
#include "octet.h"
#include "integer.h"
#include "real.h"
#include "data.h"
#include "str.h"
#include "list.h"
#include "dict.h"
#include "method.h"
#include "exception.h"
#include "thrown_exception.h"
#include "rwstream.h"
#include "fs.h"

/* Libraries */
#include "console.h"

/* Utils */
#include "scope.h"
#include "gc.h"

namespace esharp {
	class list;
	class runtime: public object {
	private:
		scope* global_scope;
		scope* current_scope;
		object* current_object;
		const wchar_t* current_source;
		runtime();
    public:
        virtual ~runtime();
		scope* getCurrentScope();
		void enterScope(scope* scope_param);
		void exitScope();
		object* getCurrentObject();
		void setCurrentObject(object* object_param);
		const wchar_t* getCurrentSource();
		void setCurrentSource(const wchar_t* source_param);
		static object* import(list* param);
	public:
		static runtime runtime_class;
    };
}

#endif
