//
//  bc_binary_expr.h
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

#ifndef E__bc_binary_expr_h
#define E__bc_binary_expr_h

#include "bc_expr.h"
#include "bc_utils.h"

namespace esharp {
	class object;
	struct bc_id;
	struct bc_binary_expr: bc_expr
	{
		bc_binary_expr();
		bc_binary_expr(Token pTree);
		virtual ~bc_binary_expr();
		virtual bc_binary_expr* clone();
		virtual object* evaluate();
		int op;
		bc_expr* operand1;
		bc_expr* operand2;
	};
}

#endif
