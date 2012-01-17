//
//  bc_literals.h
//  E#
//
//  Created by Adrian Punga on 01/04/2012.
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

#ifndef E__bc_literals_h
#define E__bc_literals_h

#include "bc_expr.h"
#include "bc_utils.h"

#include <string>

namespace esharp {

	class object;
	struct bc_id;
	struct bc_literal: bc_expr
	{
		bc_literal();
		bc_literal(Token pTree);
		virtual ~bc_literal();
		virtual bc_literal* clone()=0;
	};

	struct bc_none: bc_literal
	{
		bc_none();
		bc_none(Token pTree);
		virtual ~bc_none();
		virtual bc_none* clone();
		virtual object* evaluate();
	};

	struct bc_boolean: bc_literal
	{
		bc_boolean();
		bc_boolean(Token pTree);
		virtual ~bc_boolean();
		virtual bc_boolean* clone();
		virtual object* evaluate();
		bool value;
	};

	struct bc_data: bc_literal
	{
		bc_data();
		bc_data(Token pTree);
		virtual ~bc_data();
		virtual bc_data* clone();
		virtual object* evaluate();
		unsigned char* value;
		unsigned int length;
	};

	struct bc_string: bc_literal
	{
		bc_string();
		bc_string(Token pTree);
		virtual ~bc_string();
		virtual bc_string* clone();
		virtual object* evaluate();
		std::wstring value;
	};

	struct bc_docstring
	{
		bc_docstring();
		bc_docstring(Token pTree);
		virtual ~bc_docstring();
		std::wstring value;
	};

	struct bc_octet: bc_literal
	{
		bc_octet();
		bc_octet(Token pTree);
		virtual ~bc_octet();
		virtual bc_octet* clone();
		virtual object* evaluate();
		unsigned char value;
	};

	struct bc_integer: bc_literal
	{
		bc_integer();
		bc_integer(Token pTree);
		virtual ~bc_integer();
		virtual bc_integer* clone();
		virtual object* evaluate();
		int value;
	};

	struct bc_real: bc_literal
	{
		bc_real();
		bc_real(Token pTree);
		virtual ~bc_real();
		virtual bc_real* clone();
		virtual object* evaluate();
		double value;
	};
}

#endif
