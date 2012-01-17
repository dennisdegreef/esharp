//
//  bc_unary_expr.cpp
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

#include "bc_unary_expr.h"
#include "bc_id.h"
#include "runtime.h"

using namespace esharp;

#define OP_LOGIC_NOT (1)
#define OP_BIT_NOT (2)
#define OP_MINUS (3)
#define OP_PLUS (4)

std::wstring _operator_logic_not(L"operator_logic_not(0)");
std::wstring _operator_bit_not(L"operator_bit_not(0)");
std::wstring _operator_minus(L"operator_minus(0)");
std::wstring _operator_plus(L"operator_plus(0)");

bc_unary_expr::bc_unary_expr()
:bc_expr()
{
}

bc_unary_expr::bc_unary_expr(Token pTree)
:bc_expr(pTree)
{
	bc_id* opid = new bc_id(pTree);
	if(opid->name == L"!") {
		this->op = OP_LOGIC_NOT;
	} else if(opid->name == L"~") {
		this->op = OP_BIT_NOT;
	} else if(opid->name == L"-") {
		this->op = OP_MINUS;
	} else if(opid->name == L"+") {
		this->op = OP_PLUS;
	}
	delete opid;
	Token pOperand = GetChild(pTree,0);
	this->operand = bc_expr::bc_expr_create(pOperand);
}

bc_unary_expr::~bc_unary_expr()
{
	delete this->operand;
}

bc_unary_expr* bc_unary_expr::clone()
{
	bc_unary_expr* copy = new bc_unary_expr();
	copy->op = op;
	copy->operand = operand->clone();
	copy->line = line;
	return copy;
}

object* bc_unary_expr::evaluate()
{
	std::wstring* operator_name;
	switch(this->op) {
		case OP_LOGIC_NOT:
			operator_name = &_operator_logic_not;
			break;
		case OP_BIT_NOT:
			operator_name = &_operator_bit_not;
			break;
		case OP_MINUS:
			operator_name = &_operator_minus;
			break;
		case OP_PLUS:
			operator_name = &_operator_plus;
			break;
		default:
			return new thrown_exception(new exception(L"Invalid operator"));
	}
	object* operandObj = this->operand->evaluate();
	if(operandObj && operandObj->isInstanceOf(&thrown_exception::thrown_exception_class)) {
		return operandObj;
	}
	object* result = operandObj->getMethod(*operator_name)->call(1, operandObj);
	if(result && result->isInstanceOf(&thrown_exception::thrown_exception_class)) {
		thrown_exception* exc = dynamic_cast<thrown_exception*>(result);
		std::wstringstream trace_method;
		trace_method << L"  File \"" << runtime::runtime_class.getCurrentSource() << "\", line " << this->line;
		exc->getException()->addTrace(trace_method.str());
	}
	return result;
}
