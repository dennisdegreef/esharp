//
//  rwstream.h
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

#ifndef E__rwstream_h
#define E__rwstream_h

#include "object.h"

namespace esharp {
	class list;
	class rwstream: public object {
	private:
		rwstream();
	protected:
		rwstream(object* ancestor_param);
    public:
        virtual ~rwstream();
		virtual object* availableData(unsigned int* available);
		virtual object* readDataToBuffer(char* buffer, unsigned int len);
		virtual object* writeDataFromBuffer(char* buffer, unsigned int len);
		virtual object* closeStream();
		static object* read(list* param);
		static object* write(list* param);
		static object* close(list* param);
	public:
		static rwstream rwstream_class;
    };
}

#endif
