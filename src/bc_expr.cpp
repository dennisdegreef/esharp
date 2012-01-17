//
//  bc_expr.cpp
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

#include "bc_expr.h"
#include "bc_classdef.h"
#include "bc_code.h"
#include "bc_call.h"
#include "bc_id.h"
#include "bc_literals.h"
#include "bc_binary_expr.h"
#include "bc_unary_expr.h"
#include "bc_utils.h"
#include "runtime.h"

using namespace esharp;

bc_expr::bc_expr()
:bc_sourceinfo()
{
}

bc_expr::bc_expr(Token pTree)
:bc_sourceinfo(pTree)
{
}

bc_expr* bc_expr::bc_expr_create(Token pTree)
{
	switch(GetType(pTree)) {
		case CLASSDEF:
			return new bc_classdef(pTree);
		case CALL:
			return new bc_call(pTree); //throws exceptions
		case ID:
			return new bc_id(pTree); //throws exceptions
		case NONE:
			return new bc_none(pTree);
		case TRUELIT:
			return new bc_boolean(pTree);
		case FALSELIT:
			return new bc_boolean(pTree);
		case DATA:
			return new bc_data(pTree);
		case STRING:
			return new bc_string(pTree);
		case OCTET:
			return new bc_octet(pTree);
		case INT:
			return new bc_integer(pTree);
		case FLOAT:
			return new bc_real(pTree);
		case LOGIC_OR:
		case LOGIC_AND:
		case BIT_OR:
		case BIT_XOR:
		case BIT_AND:
		case EQ:
		case NEQ:
		case BIGGER:
		case BIGGER_EQ:
		case LOWER:
		case LOWER_EQ:
		case LSHIFT:
		case RSHIFT:
		case MUL:
		case DIV:
		case MOD:
			return new bc_binary_expr(pTree); //throws exceptions
		case ADD:
		case SUB:
			if (GetChildCount(pTree)==2) {
				return new bc_binary_expr(pTree);
			} else {
				return new bc_unary_expr(pTree);
			}
		case LOGIC_NOT:
		case BIT_NOT:
			return new bc_unary_expr(pTree); //throws exceptions
		default:
			debugPrintTree(0, pTree);
			return 0;
	}
}

bc_expr::~bc_expr()
{
}
