//
//  bc_classdef.h
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

#ifndef E__bc_classdef_h
#define E__bc_classdef_h

#include "bc_expr.h"
#include "bc_utils.h"

#include <vector>

namespace esharp {

	class object;
	class list;
	struct bc_id;
	struct bc_expr;
	struct bc_code;
	struct bc_method
	{
		bc_method();
		bc_method(Token pTree);
		virtual ~bc_method();
		virtual object* call(list* method_params);
		virtual bc_method* clone();
		bc_id* id;
		bool varargs;
		std::vector<bc_id*> params;
		std::wstring docstring;
		bc_code* code;
	};

	struct bc_property
	{
		bc_property();
		bc_property(Token pTree);
		virtual ~bc_property();
		virtual bc_property* clone();
		bc_id* id;
		bc_expr* expr;
	};

	struct bc_classdef: bc_expr
	{
		bc_classdef();
		bc_classdef(Token pTree);
		virtual ~bc_classdef();
		virtual bc_classdef* clone();
		virtual object* evaluate();
		bc_id* id; //weak ref => do not free directly
		bc_expr* inheriting;
		std::vector<bc_property*> properties;
		std::vector<bc_method*> methods;
	};
}

#endif
