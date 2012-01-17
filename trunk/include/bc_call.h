//
//  bc_call.h
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

#ifndef E__bc_call_h
#define E__bc_call_h

#include "bc_expr.h"
#include "bc_utils.h"

#include <vector>
#include <string>
#include <sstream>

namespace esharp {
	struct bc_id;
	struct bc_message
	{
		bc_message();
		virtual ~bc_message();
		virtual bc_message* clone();
		bc_id* id;
		std::vector<bc_expr*> args;
	};
	struct bc_call: bc_expr
	{
		bc_call();
		bc_call(Token pTree);
		virtual ~bc_call();
		virtual bc_call* clone();
		virtual object* evaluate();
		bc_expr* receiver;
		std::vector<bc_message*> messages;
	};
}

#endif
