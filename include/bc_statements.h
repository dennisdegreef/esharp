//
//  bc_statements.h
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

#ifndef E__bc_statements_h
#define E__bc_statements_h

#include "bc_sourceinfo.h"
#include "bc_utils.h"
#include "bc_chunk.h"

#include <string>
#include <vector>

namespace esharp {
	class object;
	struct bc_id;
	struct bc_expr;
	struct bc_code;
	struct bc_call;
	struct bc_stmt: bc_chunk, bc_sourceinfo
	{
		bc_stmt();
		bc_stmt(Token pTree);
		virtual ~bc_stmt();
		virtual object* execute();
	};

	struct bc_block_stmt: bc_stmt
	{
		bc_block_stmt();
		bc_block_stmt(Token pTree);
		virtual ~bc_block_stmt();
		virtual bc_block_stmt* clone();
		virtual object* execute();
		bc_code* code;
	};

	struct bc_call_stmt: bc_stmt
	{
		bc_call_stmt();
		bc_call_stmt(Token pTree);
		virtual ~bc_call_stmt();
		virtual bc_call_stmt* clone();
		virtual object* execute();
		bc_call* call;
	};

	struct bc_expr_stmt: bc_stmt
	{
		bc_expr_stmt();
		bc_expr_stmt(Token pTree);
		virtual ~bc_expr_stmt();
		virtual bc_expr_stmt* clone();
		virtual object* execute();
		bc_expr* expr;
	};

	struct bc_return_stmt: bc_stmt
	{
		bc_return_stmt();
		bc_return_stmt(Token pTree);
		virtual ~bc_return_stmt();
		virtual bc_return_stmt* clone();
		virtual object* execute();
		bc_expr* expr;
	};

	struct bc_throw_stmt: bc_stmt
	{
		bc_throw_stmt();
		bc_throw_stmt(Token pTree);
		virtual ~bc_throw_stmt();
		virtual bc_throw_stmt* clone();
		virtual object* execute();
		bc_expr* expr;
	};

	struct bc_catch_block
	{
		bc_catch_block();
		bc_catch_block(Token pTree);
		virtual ~bc_catch_block();
		virtual bc_catch_block* clone();
		bc_expr* filter_class;
		bc_id* local_exception_name;
		bc_code* code;
	};

	struct bc_try_stmt: bc_stmt
	{
		bc_try_stmt();
		bc_try_stmt(Token pTree);
		virtual ~bc_try_stmt();
		virtual bc_try_stmt* clone();
		virtual object* execute();
		bc_code* code;
		std::vector<bc_catch_block*> catches;
	};

	struct bc_else_block
	{
		bc_else_block();
		bc_else_block(Token pTree);
		virtual ~bc_else_block();
		virtual bc_else_block* clone();
		bc_code* code;
	};

	struct bc_ifelse_stmt: bc_stmt
	{
		bc_ifelse_stmt();
		bc_ifelse_stmt(Token pTree);
		virtual ~bc_ifelse_stmt();
		virtual bc_ifelse_stmt* clone();
		virtual object* execute();
		bc_expr* condition;
		bc_code* code;
		bc_else_block* else_block;
	};

	struct bc_while_stmt: bc_stmt
	{
		bc_while_stmt();
		bc_while_stmt(Token pTree);
		virtual ~bc_while_stmt();
		virtual bc_while_stmt* clone();
		virtual object* execute();
		bc_expr* condition;
		bc_code* code;
	};

	struct bc_assign_stmt;
	struct bc_for_stmt: bc_stmt
	{
		bc_for_stmt();
		bc_for_stmt(Token pTree);
		virtual ~bc_for_stmt();
		virtual bc_for_stmt* clone();
		virtual object* execute();
		std::vector<bc_assign_stmt*> init;
		bc_expr* condition;
		std::vector<bc_assign_stmt*> step;
		bc_code* code;
	};

	struct bc_foreach_stmt: bc_stmt
	{
		bc_foreach_stmt();
		bc_foreach_stmt(Token pTree);
		virtual ~bc_foreach_stmt();
		virtual bc_foreach_stmt* clone();
		virtual object* execute();
		bc_expr* items;
		bc_id* local_item_name;
		bc_code* code;
	};

	struct bc_continue_stmt: bc_stmt
	{
		bc_continue_stmt();
		bc_continue_stmt(Token pTree);
		virtual ~bc_continue_stmt();
		virtual bc_continue_stmt* clone();
		virtual object* execute();
	};

	struct bc_break_stmt: bc_stmt
	{
		bc_break_stmt();
		bc_break_stmt(Token pTree);
		virtual ~bc_break_stmt();
		virtual bc_break_stmt* clone();
		virtual object* execute();
	};

	struct bc_assign_stmt: bc_chunk, bc_sourceinfo
	{
		bc_assign_stmt();
		bc_assign_stmt(Token pTree);
		virtual ~bc_assign_stmt();
		virtual bc_assign_stmt* clone();
		virtual object* execute();
		bc_id* id;
		bc_expr* expr;
	};

	struct bc_del_stmt: bc_chunk, bc_sourceinfo
	{
		bc_del_stmt();
		bc_del_stmt(Token pTree);
		virtual ~bc_del_stmt();
		virtual bc_del_stmt* clone();
		virtual object* execute();
		bc_id* id;
	};
}

#endif
