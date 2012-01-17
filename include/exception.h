//
//  exception.h
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

#ifndef E__exception_h
#define E__exception_h

#include "object.h"

#include <vector>

namespace esharp {
	class list;
	class exception: public object {
	private:
		exception* cause;
		std::wstring message;
		std::vector<std::wstring> trace;
		exception();
    public:
		exception(std::wstring message_param, exception* cause_param=0);
        virtual ~exception();
		const std::wstring& getMessage();
		void addTrace(const std::wstring& trace_param);
		exception* getCause();
		static object* new_instance(list* param);
		static object* new_instance_with_message(list* param);
		static object* operator_cast(list* param);
		static object* print_trace(list* param);
	public:
		static exception exception_class;
    };
}

#endif
