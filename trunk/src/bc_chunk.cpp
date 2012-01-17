//
//  bc_chunk.cpp
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

#include "bc_chunk.h"
#include "bc_statements.h"
#include "bc_utils.h"

using namespace esharp;

bc_chunk* bc_chunk::bc_chunk_create(Token pTree)
{
	switch(GetType(pTree)) {
		case BLOCK:
			return new bc_block_stmt(pTree);
		case CALL:
			return new bc_call_stmt(pTree);
		case ASSIGN:
			return new bc_assign_stmt(pTree);
		case DEL:
			return new bc_del_stmt(pTree);
		case TRY:
			return new bc_try_stmt(pTree);
		case THROW:
			return new bc_throw_stmt(pTree);
		case RETURN:
			return new bc_return_stmt(pTree);
		case WHILE:
			return new bc_while_stmt(pTree);
		case FOR:
			return new bc_for_stmt(pTree);
		case FOREACH:
			return new bc_foreach_stmt(pTree);
		case IF:
			return new bc_ifelse_stmt(pTree);
		case BREAK:
			return new bc_break_stmt(pTree);
		case CONTINUE:
			return new bc_continue_stmt(pTree);
		default:
			return new bc_expr_stmt(pTree);
	}
}

bc_chunk::~bc_chunk()
{
}
