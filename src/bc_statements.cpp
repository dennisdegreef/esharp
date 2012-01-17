//
//  bc_statements.cpp
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

#include "bc_statements.h"
#include "bc_id.h"
#include "bc_expr.h"
#include "bc_code.h"
#include "bc_call.h"
#include "bc_classdef.h"
#include "runtime.h"

using namespace esharp;

/********************************
 STMT
 ********************************/

bc_stmt::bc_stmt()
:bc_sourceinfo()
{
}
bc_stmt::bc_stmt(Token pTree)
:bc_sourceinfo(pTree)
{
}
bc_stmt::~bc_stmt()
{
}
object* bc_stmt::execute()
{
	return 0;
}

/********************************
 BLOCK
 ********************************/

bc_block_stmt::bc_block_stmt()
:bc_stmt()
{
}

bc_block_stmt::bc_block_stmt(Token pTree)
:bc_stmt(pTree)
{
	Token pCode = GetChild(pTree,0);
	this->code = new bc_code(pCode);
}

bc_block_stmt::~bc_block_stmt()
{
	delete this->code;
}

bc_block_stmt* bc_block_stmt::clone()
{
	bc_block_stmt* copy = new bc_block_stmt();
	copy->code = code->clone();
	copy->line = line;
	return copy;
}

object* bc_block_stmt::execute()
{
	object* result = 0;
	scope* block_scope = new scope(runtime::runtime_class.getCurrentScope());
	//Enter block scope
	runtime::runtime_class.enterScope(block_scope);
	//Run
	result = this->code->execute();
	//Go back to previous scope
	runtime::runtime_class.exitScope();
	return result;
}

/********************************
 CALL
 ********************************/

bc_call_stmt::bc_call_stmt()
:bc_stmt()
{
}

bc_call_stmt::bc_call_stmt(Token pTree)
:bc_stmt(pTree)
{
	this->call = new bc_call(pTree);
}

bc_call_stmt::~bc_call_stmt()
{
	delete this->call;
}

bc_call_stmt* bc_call_stmt::clone()
{
	bc_call_stmt* copy = new bc_call_stmt();
	copy->call = call->clone();
	copy->line = line;
	return copy;
}

object* bc_call_stmt::execute()
{
	object* result = this->call->evaluate();
	if(result && result->isInstanceOf(&thrown_exception::thrown_exception_class)) {
		//Re-throw exception
		return result;
	}
	//Store the last evaluation result hidden in the current scope, interactive mode will look for it
	runtime::runtime_class.getCurrentScope()->setNamedObject(L"", result);
	return 0;
}

/********************************
 EXPR
 ********************************/

bc_expr_stmt::bc_expr_stmt()
:bc_stmt()
{
}

bc_expr_stmt::bc_expr_stmt(Token pTree)
:bc_stmt(pTree)
{
	this->expr = bc_expr::bc_expr_create(pTree);
}

bc_expr_stmt::~bc_expr_stmt()
{
	delete this->expr;
}

bc_expr_stmt* bc_expr_stmt::clone()
{
	bc_expr_stmt* copy = new bc_expr_stmt();
	copy->expr = expr->clone();
	copy->line = line;
	return copy;
}

object* bc_expr_stmt::execute()
{
	object* result = this->expr->evaluate();
	if(result && result->isInstanceOf(&thrown_exception::thrown_exception_class)) {
		//Re-throw exception
		return result;
	}
	//Store the last evaluation result hidden in the current scope, interactive mode will look for it
	runtime::runtime_class.getCurrentScope()->setNamedObject(L"", result);
	return 0;
}

/********************************
 ASSIGN
 ********************************/

bc_assign_stmt::bc_assign_stmt()
:bc_sourceinfo()
{
}

bc_assign_stmt::bc_assign_stmt(Token pTree)
:bc_sourceinfo(pTree)
{
	Token pID = GetChild(pTree,0);
	this->id = new bc_id(pID);
	Token pValue = GetChild(pTree,1);
	this->expr = bc_expr::bc_expr_create(pValue);
	//Add original name to the class defintion to let the programmer know where exceptions occured
	bc_classdef* cdef = dynamic_cast<bc_classdef*>(this->expr);
	if(cdef) {
		cdef->id = this->id->clone();
	}
}

bc_assign_stmt::~bc_assign_stmt()
{
	delete this->id;
	delete this->expr;
}

bc_assign_stmt* bc_assign_stmt::clone()
{
	bc_assign_stmt* copy = new bc_assign_stmt();
	copy->id = id->clone();
	copy->expr = expr->clone();
	copy->line = line;
	return copy;
}

object* bc_assign_stmt::execute()
{
	object* result = this->expr->evaluate();
	if(result->isInstanceOf(&thrown_exception::thrown_exception_class)) {
		return result;
	} else {
		object* prop = runtime::runtime_class.getCurrentObject()->getProperty(this->id->name);
		if(prop) {
			runtime::runtime_class.getCurrentObject()->setProperty(this->id->name, result);
		} else {
			runtime::runtime_class.getCurrentScope()->setNamedObject(this->id->name, result);
		}
		return 0;
	}
}


/********************************
 DEL
 ********************************/

bc_del_stmt::bc_del_stmt()
:bc_sourceinfo()
{
}

bc_del_stmt::bc_del_stmt(Token pTree)
:bc_sourceinfo(pTree)
{
	Token pID = GetChild(pTree,0);
	this->id = new bc_id(pID);
}

bc_del_stmt::~bc_del_stmt()
{
	delete this->id;
}

bc_del_stmt* bc_del_stmt::clone()
{
	bc_del_stmt* copy = new bc_del_stmt();
	copy->id = id->clone();
	copy->line = line;
	return copy;
}

object* bc_del_stmt::execute()
{
	runtime::runtime_class.getCurrentScope()->delNamedObject(this->id->name);
	return 0;
}

/********************************
 TRY CATCH
 ********************************/

bc_try_stmt::bc_try_stmt()
:bc_stmt()
{
}

bc_try_stmt::bc_try_stmt(Token pTree)
:bc_stmt(pTree)
{
	Token pCode = GetChild(pTree,0);
	this->code = new bc_code(pCode);
	for (unsigned int i=1;i<GetChildCount(pTree);i++) {
		Token pCatch = GetChild(pTree,i);
		this->catches.push_back(new bc_catch_block(pCatch));
	}
}

bc_try_stmt::~bc_try_stmt()
{
	delete this->code;
	for(std::vector<bc_catch_block*>::iterator i=this->catches.begin(); i!=this->catches.end(); i++) {
		delete (*i);
	}
}

bc_try_stmt* bc_try_stmt::clone()
{
	bc_try_stmt* copy = new bc_try_stmt();
	copy->code = code->clone();
	for(std::vector<bc_catch_block*>::iterator i=catches.begin();i!=catches.end();i++) {
		copy->catches.push_back((*i)->clone());
	}
	copy->line = line;
	return copy;
}

bc_catch_block::bc_catch_block()
{
}

bc_catch_block::bc_catch_block(Token pTree)
{
	Token pFilterClass = GetChild(pTree,0);
	this->filter_class = bc_expr::bc_expr_create(pFilterClass);
	Token pLocalName = GetChild(pTree,1);
	this->local_exception_name = new bc_id(pLocalName);
	Token pCode = GetChild(pTree,2);
	this->code = new bc_code(pCode);
}

bc_catch_block::~bc_catch_block()
{
	delete this->filter_class;
	delete this->local_exception_name;
	delete this->code;
}

bc_catch_block* bc_catch_block::clone()
{
	bc_catch_block* copy = new bc_catch_block();
	copy->local_exception_name = local_exception_name->clone();
	copy->filter_class = filter_class->clone();
	copy->code = code->clone();
	return copy;
}

object* bc_try_stmt::execute()
{
	object* result = 0;
	scope* try_scope = new scope(0);
	//Enter try scope
	runtime::runtime_class.enterScope(try_scope);
	//Run
	result = this->code->execute();
	//Go back to previous scope
	runtime::runtime_class.exitScope();

	if(result && result->isInstanceOf(&thrown_exception::thrown_exception_class)) {
		thrown_exception* exc_result = dynamic_cast<thrown_exception*>(result);
		for(std::vector<bc_catch_block*>::iterator i=this->catches.begin(); i!=this->catches.end(); i++) {
			object* filter = (*i)->filter_class->evaluate();
			if(exc_result->getException()->isInstanceOf(filter)) {
				scope* catch_scope = new scope(0);
				//Enter catch scope
				runtime::runtime_class.enterScope(catch_scope);
				//Populate catch scope with local_exception_name
				catch_scope->setNamedObject((*i)->local_exception_name->name, exc_result->getException());
				//Run
				result = (*i)->code->execute();
				//Go back to previous scope
				runtime::runtime_class.exitScope();
				//Delete caught exception
				delete exc_result;
				return result;
			}
		}
		return result;
	}
	return result;
}

/********************************
 THROW
 ********************************/

bc_throw_stmt::bc_throw_stmt()
:bc_stmt()
{
}

bc_throw_stmt::bc_throw_stmt(Token pTree)
:bc_stmt(pTree)
{
	Token pValue = GetChild(pTree,0);
	this->expr = bc_expr::bc_expr_create(pValue);
}

bc_throw_stmt::~bc_throw_stmt()
{
	delete this->expr;
}

bc_throw_stmt* bc_throw_stmt::clone()
{
	bc_throw_stmt* copy = new bc_throw_stmt();
	copy->expr = expr->clone();
	copy->line = line;
	return copy;
}

object* bc_throw_stmt::execute()
{
	object* result = this->expr->evaluate();
	if(result->isInstanceOf(&thrown_exception::thrown_exception_class)) {
		//abnormal but may happen when evaluating the expr does a bubu
		return result;
	} else if(result->isInstanceOf(&exception::exception_class)) {
		//normal way to throw exceptions if there is anything normal to it
		return new thrown_exception(dynamic_cast<exception*>(result));
	} else {
		//highly abnormal but anyone can send anything to throw in the end
		return new thrown_exception(new exception(L"Cannot throw objects that are not exceptions"));
	}
}

/********************************
 RETURN
 ********************************/

bc_return_stmt::bc_return_stmt()
:bc_stmt()
{
}

bc_return_stmt::bc_return_stmt(Token pTree)
:bc_stmt(pTree)
{
	this->expr = 0;
	if(GetChildCount(pTree)==1) {
		Token pValue = GetChild(pTree,0);
		this->expr = bc_expr::bc_expr_create(pValue);
	}
}

bc_return_stmt::~bc_return_stmt()
{
	delete this->expr;
}

bc_return_stmt* bc_return_stmt::clone()
{
	bc_return_stmt* copy = new bc_return_stmt();
	copy->expr = expr->clone();
	copy->line = line;
	return copy;
}

object* bc_return_stmt::execute()
{
	if(this->expr) {
		return this->expr->evaluate();
	} else {
		return &object::object_none;
	}
}

/********************************
 WHILE
 ********************************/

bc_while_stmt::bc_while_stmt()
:bc_stmt()
{
}

bc_while_stmt::bc_while_stmt(Token pTree)
:bc_stmt(pTree)
{
	Token pCondition = GetChild(pTree,0);
	this->condition = bc_expr::bc_expr_create(pCondition);
	Token pCode = GetChild(pTree,1);
	this->code = new bc_code(pCode);
}

bc_while_stmt::~bc_while_stmt()
{
	delete this->condition;
	delete this->code;
}

bc_while_stmt* bc_while_stmt::clone()
{
	bc_while_stmt* copy = new bc_while_stmt();
	copy->condition = condition->clone();
	copy->code = code->clone();
	copy->line = line;
	return copy;
}

object* bc_while_stmt::execute()
{
	scope* while_scope = new scope(0);
	//Enter foreach scope
	runtime::runtime_class.enterScope(while_scope);
	object* result = 0;
	while(true) {
		//Test the condition
		object* test = this->condition->evaluate();
		if(test->isInstanceOf(&thrown_exception::thrown_exception_class)) {
			return test;
		}
		if(!test->isInstanceOf(&boolean::boolean_class)) {
			return new thrown_exception(new exception(L"While condition is not boolean!"));
		}
		if(test==&boolean::boolean_false) {
			break;
		}
		//Execute while loop
		result = this->code->execute();
		if(result) {
			if(result==&object::object_break) {
				//Clean result & break
				result = 0;
				break;
			} else if(result==&object::object_continue) {
				//Clean result & continue
				result = 0;
			} else {
				//Simply pass result
				break;
			}
		}
	}
	//Go back to previous scope
	runtime::runtime_class.exitScope();
	return result;
}

/********************************
 FOR
 ********************************/

bc_for_stmt::bc_for_stmt()
:bc_stmt()
{
}

bc_for_stmt::bc_for_stmt(Token pTree)
:bc_stmt(pTree)
{
	Token pInit = GetChild(pTree,0);
	for (unsigned int i=0;i<GetChildCount(pInit);i++) {
		Token pInitItem = GetChild(pInit,i);
		this->init.push_back(new bc_assign_stmt(pInitItem));
	}
	Token pCond = GetChild(pTree,1);
	this->condition = bc_expr::bc_expr_create(GetChild(pCond,0));
	Token pStep = GetChild(pTree,2);
	for (unsigned int i=0;i<GetChildCount(pStep);i++) {
		Token pStepItem = GetChild(pStep,i);
		this->step.push_back(new bc_assign_stmt(pStepItem));
	}
	Token pCode = GetChild(pTree,3);
	this->code = new bc_code(pCode);
}

bc_for_stmt::~bc_for_stmt()
{
	for(std::vector<bc_assign_stmt*>::iterator i=this->init.begin(); i!=this->init.end(); i++) {
		delete (*i);
	}
	delete this->condition;
	for(std::vector<bc_assign_stmt*>::iterator i=this->step.begin(); i!=this->step.end(); i++) {
		delete (*i);
	}
	delete this->code;
}

bc_for_stmt* bc_for_stmt::clone()
{
	bc_for_stmt* copy = new bc_for_stmt();
	for(std::vector<bc_assign_stmt*>::iterator i=init.begin();i!=init.end();i++) {
		copy->init.push_back((*i)->clone());
	}
	copy->condition = condition->clone();
	for(std::vector<bc_assign_stmt*>::iterator i=step.begin();i!=step.end();i++) {
		copy->step.push_back((*i)->clone());
	}
	copy->code = code->clone();
	copy->line = line;
	return copy;
}

object* bc_for_stmt::execute()
{
	scope* for_scope = new scope(0);	
	//Enter for scope
	runtime::runtime_class.enterScope(for_scope);
	object* result = 0;
	//Run init assignment
	for(std::vector<bc_assign_stmt*>::iterator i=this->init.begin(); i!=this->init.end(); i++) {
		result = (*i)->execute();
		if(result) {
			break;
		}
	}
	if(!result) {
		while(true) {
			//Test the condition
			if(this->condition) {
				object* test = this->condition->evaluate();
				if(test->isInstanceOf(&thrown_exception::thrown_exception_class)) {
					return test;
				}
				if(!test->isInstanceOf(&boolean::boolean_class)) {
					return new thrown_exception(new exception(L"For condition is not boolean!"));
				}
				if(test==&boolean::boolean_false) {
					break;
				}
			}
			//Execute for loop
			result = this->code->execute();
			if(result) {
				if(result==&object::object_break) {
					//Clean result & break
					result = 0;
					break;
				} else if(result==&object::object_continue) {
					//Clean result & continue
					result = 0;
				} else {
					//Simply pass result
					break;
				}
			}
			//Execute step assignment
			for(std::vector<bc_assign_stmt*>::iterator i=this->step.begin(); i!=this->step.end(); i++) {
				result = (*i)->execute();
				if(result) {
					break;
				}
			}
			if(result) {
				break;
			}
		}
	}
	//Go back to previous scope
	runtime::runtime_class.exitScope();
	return result;
}


/********************************
 FOREACH
 ********************************/

bc_foreach_stmt::bc_foreach_stmt()
:bc_stmt()
{
}

bc_foreach_stmt::bc_foreach_stmt(Token pTree)
:bc_stmt(pTree)
{
	Token pArray = GetChild(pTree,0);
	this->items = bc_expr::bc_expr_create(pArray);
	Token pItemLocalName = GetChild(pTree,1);
	this->local_item_name = new bc_id(pItemLocalName);
	Token pCode = GetChild(pTree,2);
	this->code = new bc_code(pCode);
}

bc_foreach_stmt::~bc_foreach_stmt()
{
	delete this->items;
	delete this->local_item_name;
	delete this->code;
}

bc_foreach_stmt* bc_foreach_stmt::clone()
{
	bc_foreach_stmt* copy = new bc_foreach_stmt();
	copy->local_item_name = local_item_name->clone();
	copy->items = items->clone();
	copy->code = code->clone();
	copy->line = line;
	return copy;
}

object* bc_foreach_stmt::execute()
{
	scope* foreach_scope = new scope(0);	
	//Enter foreach scope
	runtime::runtime_class.enterScope(foreach_scope);
	list* forlist = dynamic_cast<list*>(this->items->evaluate());
	object* result = 0;
	for(int i=0; i<forlist->getLength(); i++) {
		//Populate foreach scope with local_item_name
		foreach_scope->setNamedObject(this->local_item_name->name, forlist->getObject(i));
		//Run foreach loop
		result = this->code->execute();
		if(result) {
			if(result==&object::object_break) {
				//Clean result & break
				result = 0;
				break;
			} else if(result==&object::object_continue) {
				//Clean result & continue
				result = 0;
			} else {
				//Simply pass result
				break;
			}
		}
	}
	//Go back to previous scope
	runtime::runtime_class.exitScope();
	return result;
}

/********************************
 IF ELSE
 ********************************/

bc_ifelse_stmt::bc_ifelse_stmt()
:bc_stmt()
{
}

bc_ifelse_stmt::bc_ifelse_stmt(Token pTree)
:bc_stmt(pTree)
{
	Token pCondition = GetChild(pTree,0);
	this->condition = bc_expr::bc_expr_create(pCondition);
	Token pCode = GetChild(pTree,1);
	this->code = new bc_code(pCode);
	this->else_block = 0;
	if(GetChildCount(pTree)==3) {
		Token pElse = GetChild(pTree,2);
		this->else_block = new bc_else_block(pElse);
	}
}

bc_ifelse_stmt::~bc_ifelse_stmt()
{
	delete this->condition;
	delete this->code;
}

bc_ifelse_stmt* bc_ifelse_stmt::clone()
{
	bc_ifelse_stmt* copy = new bc_ifelse_stmt();
	copy->condition = condition->clone();
	copy->code = code->clone();
	copy->else_block = 0;
	if(else_block) {
		copy->else_block = else_block->clone();
	}
	copy->line = line;
	return copy;
}

bc_else_block::bc_else_block()
{
}

bc_else_block::bc_else_block(Token pTree)
{
	Token pCode = GetChild(pTree,0);
	this->code = new bc_code(pCode);
}

bc_else_block::~bc_else_block()
{
	delete this->code;
}

bc_else_block* bc_else_block::clone()
{
	bc_else_block* copy = new bc_else_block();
	copy->code = code->clone();
	return copy;
}

object* bc_ifelse_stmt::execute()
{
	object* result = 0;
	//Test the condition
	object* test = this->condition->evaluate();
	if(test->isInstanceOf(&thrown_exception::thrown_exception_class)) {
		return test;
	}
	if(!test->isInstanceOf(&boolean::boolean_class)) {
		return new thrown_exception(new exception(L"If condition is not boolean!"));
	}
	if(test==&boolean::boolean_true) {
		scope* if_scope = new scope(0);
		//Enter if scope
		runtime::runtime_class.enterScope(if_scope);
		result = this->code->execute();
		//Go back to previous scope
		runtime::runtime_class.exitScope();
	} else {
		if(this->else_block) {
			scope* else_scope = new scope(0);
			//Enter else scope
			runtime::runtime_class.enterScope(else_scope);
			result = this->else_block->code->execute();
			//Go back to previous scope
			runtime::runtime_class.exitScope();
		}
	}
	return result;
}

/********************************
 BREAK
 ********************************/

bc_break_stmt::bc_break_stmt()
:bc_stmt()
{
}

bc_break_stmt::bc_break_stmt(Token pTree)
:bc_stmt(pTree)
{
}

bc_break_stmt::~bc_break_stmt()
{
}

bc_break_stmt* bc_break_stmt::clone()
{
	bc_break_stmt* copy = new bc_break_stmt();
	copy->line = line;
	return copy;
}

object* bc_break_stmt::execute()
{
	return &object::object_break;
}


/********************************
 CONTINUE
 ********************************/

bc_continue_stmt::bc_continue_stmt()
:bc_stmt()
{
}

bc_continue_stmt::bc_continue_stmt(Token pTree)
:bc_stmt(pTree)
{
}

bc_continue_stmt::~bc_continue_stmt()
{
}

bc_continue_stmt* bc_continue_stmt::clone()
{
	bc_continue_stmt* copy = new bc_continue_stmt();
	copy->line = line;
	return copy;
}

object* bc_continue_stmt::execute()
{
	return &object::object_continue;
}

