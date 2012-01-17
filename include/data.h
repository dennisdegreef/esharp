//
//  data.h
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

#ifndef E__data_h
#define E__data_h

#include "object.h"
#include <vector>

namespace esharp {
	class list;
	class data: public object {
	private:
		std::vector<unsigned char> value;
		data();
    public:
		data(const unsigned char* data_param, unsigned int length);
        virtual ~data();
		const unsigned char* getValue();
		unsigned int getLength();

		virtual unsigned int getHash();

		static object* new_instance(list* param);
		static object* operator_eq(list* param);
		static object* len(list* param);
		static object* get(list* param);
		static object* set(list* param);
		static object* decode_utf8(list* param);
		static object* decode_ascii(list* param);
		static object* decode_ascii_strict(list* param);
		static object* read(list* param);
		static object* write(list* param);
	public:
		static data data_class;
		static data data_empty;
    };
}

#endif