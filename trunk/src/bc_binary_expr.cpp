//
//  bc_binary_expr.cpp
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

#include "bc_binary_expr.h"
#include "bc_id.h"
#include "str.h"
#include "runtime.h"

using namespace esharp;

#define OP_LOGIC_OR (1)
#define OP_LOGIC_AND (2)
#define OP_BIT_OR (3)
#define OP_BIT_XOR (4)
#define OP_BIT_AND (5)
#define OP_EQ (6)
#define OP_NEQ (7)
#define OP_BIGGER (8)
#define OP_BIGGER_EQ (9)
#define OP_LOWER (10)
#define OP_LOWER_EQ (11)
#define OP_LSHIFT (12)
#define OP_RSHIFT (13)
#define OP_ADD (14)
#define OP_SUB (15)
#define OP_MUL (16)
#define OP_DIV (17)
#define OP_MOD (18)

std::wstring _operator_logic_or(L"operator_logic_or(1)");
std::wstring _operator_logic_and(L"operator_logic_and(1)");
std::wstring _operator_bit_or(L"operator_bit_or(1)");
std::wstring _operator_bit_xor(L"operator_bit_xor(1)");
std::wstring _operator_bit_and(L"operator_bit_and(1)");
std::wstring _operator_eq(L"operator_eq(1)");
std::wstring _operator_neq(L"operator_neq(1)");
std::wstring _operator_bigger(L"operator_bigger(1)");
std::wstring _operator_bigger_eq(L"operator_bigger_eq(1)");
std::wstring _operator_lower(L"operator_lower(1)");
std::wstring _operator_lower_eq(L"operator_lower_eq(1)");
std::wstring _operator_lshift(L"operator_lshift(1)");
std::wstring _operator_rshift(L"operator_rshift(1)");
std::wstring _operator_add(L"operator_add(1)");
std::wstring _operator_sub(L"operator_sub(1)");
std::wstring _operator_mul(L"operator_mul(1)");
std::wstring _operator_div(L"operator_div(1)");
std::wstring _operator_mod(L"operator_mod(1)");

bc_binary_expr::bc_binary_expr()
:bc_expr()
{
}

bc_binary_expr::bc_binary_expr(Token pTree)
:bc_expr(pTree)
{
	bc_id* opid = new bc_id(pTree);
	if(opid->name == L"||") {
		this->op = OP_LOGIC_OR;
	} else if(opid->name == L"&&") {
		this->op = OP_LOGIC_AND;
	} else if(opid->name == L"|") {
		this->op = OP_BIT_OR;
	} else if(opid->name == L"^") {
		this->op = OP_BIT_XOR;
	} else if(opid->name == L"&") {
		this->op = OP_BIT_AND;
	} else if(opid->name == L"==") {
		this->op = OP_EQ;
	} else if(opid->name == L"!=") {
		this->op = OP_NEQ;
	} else if(opid->name == L">") {
		this->op = OP_BIGGER;
	} else if(opid->name == L">=") {
		this->op = OP_BIGGER_EQ;
	} else if(opid->name == L"<") {
		this->op = OP_LOWER;
	} else if(opid->name == L"<=") {
		this->op = OP_LOWER_EQ;
	} else if(opid->name == L"<<") {
		this->op = OP_LSHIFT;
	} else if(opid->name == L">>") {
		this->op = OP_RSHIFT;
	} else if(opid->name == L"+") {
		this->op = OP_ADD;
	} else if(opid->name == L"-") {
		this->op = OP_SUB;
	} else if(opid->name == L"*") {
		this->op = OP_MUL;
	} else if(opid->name == L"/") {
		this->op = OP_DIV;
	} else if(opid->name == L"%") {
		this->op = OP_MOD;
	}
	delete opid;
	Token pOperandLeft = GetChild(pTree,0);
	this->operand1 = bc_expr::bc_expr_create(pOperandLeft);
	Token pOperandRight = GetChild(pTree,1);
	this->operand2 = bc_expr::bc_expr_create(pOperandRight);
}

bc_binary_expr::~bc_binary_expr()
{
	delete this->operand1;
	delete this->operand2;
}

bc_binary_expr* bc_binary_expr::clone()
{
	bc_binary_expr* copy = new bc_binary_expr();
	copy->op = op;
	copy->operand1 = operand1->clone();
	copy->operand2 = operand2->clone();
	copy->line = line;
	return copy;
}

object* bc_binary_expr::evaluate()
{
	std::wstring* operator_name;
	switch(this->op) {
		case OP_LOGIC_OR:
			operator_name = &_operator_logic_or;
			break;
		case OP_LOGIC_AND:
			operator_name = &_operator_logic_and;
			break;
		case OP_BIT_OR:
			operator_name = &_operator_bit_or;
			break;
		case OP_BIT_XOR:
			operator_name = &_operator_bit_xor;
			break;
		case OP_BIT_AND:
			operator_name = &_operator_bit_and;
			break;
		case OP_EQ:
			operator_name = &_operator_eq;
			break;
		case OP_NEQ:
			operator_name = &_operator_neq;
			break;
		case OP_BIGGER:
			operator_name = &_operator_bigger;
			break;
		case OP_BIGGER_EQ:
			operator_name = &_operator_bigger_eq;
			break;
		case OP_LOWER:
			operator_name = &_operator_lower;
			break;
		case OP_LOWER_EQ:
			operator_name = &_operator_lower_eq;
			break;
		case OP_LSHIFT:
			operator_name = &_operator_lshift;
			break;
		case OP_RSHIFT:
			operator_name = &_operator_rshift;
			break;
		case OP_ADD:
			operator_name = &_operator_add;
			break;
		case OP_SUB:
			operator_name = &_operator_sub;
			break;
		case OP_MUL:
			operator_name = &_operator_mul;
			break;
		case OP_DIV:
			operator_name = &_operator_div;
			break;
		case OP_MOD:
			operator_name = &_operator_mod;
			break;
		default:
			return new thrown_exception(new exception(L"Invalid operator"));
	}
	object* operand1Obj = this->operand1->evaluate();
	if(operand1Obj && operand1Obj->isInstanceOf(&thrown_exception::thrown_exception_class)) {
		return operand1Obj;
	}
	object* operand2Obj = this->operand2->evaluate();
	if(operand2Obj && operand2Obj->isInstanceOf(&thrown_exception::thrown_exception_class)) {
		return operand2Obj;
	}
	object* result = operand1Obj->getMethod(*operator_name)->call(2, operand1Obj, operand2Obj);
	if(result && result->isInstanceOf(&thrown_exception::thrown_exception_class)) {
		thrown_exception* exc = dynamic_cast<thrown_exception*>(result);
		std::wstringstream trace_method;
		trace_method << L"  File \"" << runtime::runtime_class.getCurrentSource() << "\", line " << this->line;
		exc->getException()->addTrace(trace_method.str());
	}
	return result;
}
