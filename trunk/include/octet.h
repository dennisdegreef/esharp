//
//  octet.h
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

#ifndef E__octet_h
#define E__octet_h

#include "object.h"

namespace esharp {
	class list;
	class gc;
	class octet: public object {
		friend class data;
	private:
		unsigned char value;
		octet();
		octet(unsigned char octet_param);
    public:
        virtual ~octet();
		static octet* get(unsigned char octet_param);
		
		virtual unsigned int getHash();

		static object* operator_cast(list* param);
		static object* operator_bigger(list* param);
		static object* operator_bigger_eq(list* param);
		static object* operator_lower(list* param);
		static object* operator_lower_eq(list* param);
		static object* operator_add(list* param);
		static object* operator_sub(list* param);
		static object* operator_mul(list* param);
		static object* operator_div(list* param);
		static object* operator_mod(list* param);
		static object* operator_lshift(list* param);
		static object* operator_rshift(list* param);
		static object* operator_bit_and(list* param);
		static object* operator_bit_or(list* param);
		static object* operator_bit_xor(list* param);
		static object* operator_bit_not(list* param);
		static object* read(list* param);
		static object* write(list* param);
	public:
		static octet octet_class;

#define OCTET_STATIC_DECL(prefix) \
		static octet octet_ ## prefix ## 0;\
		static octet octet_##prefix##1;\
		static octet octet_##prefix##2;\
		static octet octet_##prefix##3;\
		static octet octet_##prefix##4;\
		static octet octet_##prefix##5;\
		static octet octet_##prefix##6;\
		static octet octet_##prefix##7;\
		static octet octet_##prefix##8;\
		static octet octet_##prefix##9;\
		static octet octet_##prefix##A;\
		static octet octet_##prefix##B;\
		static octet octet_##prefix##C;\
		static octet octet_##prefix##D;\
		static octet octet_##prefix##E;\
		static octet octet_##prefix##F

OCTET_STATIC_DECL(0x0);
OCTET_STATIC_DECL(0x1);
OCTET_STATIC_DECL(0x2);
OCTET_STATIC_DECL(0x3);
OCTET_STATIC_DECL(0x4);
OCTET_STATIC_DECL(0x5);
OCTET_STATIC_DECL(0x6);
OCTET_STATIC_DECL(0x7);
OCTET_STATIC_DECL(0x8);
OCTET_STATIC_DECL(0x9);
OCTET_STATIC_DECL(0xA);
OCTET_STATIC_DECL(0xB);
OCTET_STATIC_DECL(0xC);
OCTET_STATIC_DECL(0xD);
OCTET_STATIC_DECL(0xE);
OCTET_STATIC_DECL(0xF);

#undef OCTET_STATIC_DECL


    };
}

#endif
