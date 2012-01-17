//
//  object.h
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

#ifndef E__object_h
#define E__object_h

#include <string>
#include "collections.h"

namespace esharp {
	class method;
	class scope;
	class list;
	class str;
	class gc;
	unsigned int digest(const std::wstring& key, unsigned int hash=0U);
    class object {
		friend class gc;
    private:
		bool classname_user_defined;
		const wchar_t* classname;
		es_map<std::wstring, method*> *methods;
		es_map<std::wstring, std::wstring> *docstrings;
        es_map<std::wstring, object*> *properties;
        es_set<object*> *properties_reverse;
		object();
	protected:
		unsigned int hash;
    public:
        object(object* ancestor_param, const wchar_t* classname_param=L"object");
//		object(object* ancestor_param, const std::wstring& classname_param);
        virtual ~object();
		
		//New pointer that will be deleted by the GC
		void* operator new(size_t num_bytes);
		void operator delete(void* ptr);

		bool pointerFound(object* needle);
		virtual bool isOrContainsPointer(object* pointer, std::set<object*>& visited, int level=0);

        method* getMethod(const std::wstring& name);
		str* getMethodDoc(const std::wstring& name);
        void setMethod(const std::wstring& name, method* methodObject, const std::wstring& docstring=L"");
        object* getProperty(const std::wstring& name);
        void setProperty(const std::wstring& name, object* propertyValue, object* oldValue=0);
		void delProperty(const std::wstring& name, object* oldValue=0);
		bool isInstanceOf(object* ancestor_param);
		const wchar_t* getClassname();
        void setClassname(const wchar_t* name);
		void setClassname(const std::wstring& name);

		virtual unsigned int getHash();

		static object* new_instance(list* param);
		static object* operator_cast(list* param);
		static object* operator_eq(list* param);
		static object* operator_neq(list* param);
		static object* default_binary_operator(list* param);
		static object* default_unary_operator(list* param);
		static object* doc(list* param);
	public:
		//type info => access with care from C
        object* ancestor;
		//the mother of all objects everywhere
		static object object_class;
		//none is mostly like void in C as it is used only when returning nothing from a successfully executed method
		static object object_none;
		//internal break signal
		static object object_break;
		//internal continue signal
		static object object_continue;
    };
}

#endif
