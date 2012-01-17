//
//  bc_classdef.cpp
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

#include "bc_classdef.h"
#include "bc_id.h"
#include "bc_code.h"
#include "bc_expr.h"
#include "bc_literals.h"
#include "runtime.h"

using namespace esharp;

#include <iostream>
#include <sstream>
#include <string>

bc_method::bc_method()
{
}

bc_method::bc_method(Token pTree)
{
	Token pID = GetChild(pTree, 0);
	this->id = new bc_id(pID);
	Token pMethodDef = GetChild(pTree, 1);
	Token pParams = GetChild(pMethodDef, 0);
	this->varargs = false;
	if(GetChildCount(pParams)==1) {
		Token pParamVarargs = GetChild(pParams, 0);
		if(GetType(pParamVarargs)==VARARGS) {
			this->varargs = true;
		}
	}
	if(!this->varargs) {
		for (unsigned int j=0;j<GetChildCount(pParams);j++) {
			Token pParamID = GetChild(pParams,j);
			this->params.push_back(new bc_id(pParamID));
		}
	}
	Token pDocString = GetChild(pMethodDef,1);
	if(GetChildCount(pDocString)==1) {
		Token pDocStringContent = GetChild(pDocString,0);
		bc_docstring doc(pDocStringContent);
		this->docstring = doc.value;
	}
	Token pCode = GetChild(pMethodDef,2);
	this->code = new bc_code(pCode);
}

bc_method::~bc_method()
{
	delete this->code;
	for(std::vector<bc_id*>::iterator i=this->params.begin(); i!=this->params.end(); i++) {
		delete (*i);
	}
}

bc_method* bc_method::clone()
{
	bc_method* copy = new bc_method();
	copy->id = id->clone();
	copy->varargs = varargs;
	for(std::vector<bc_id*>::iterator i=params.begin();i!=params.end();i++) {
		copy->params.push_back((*i)->clone());
	}
	copy->docstring = docstring;
	copy->code = code->clone();
	return copy;
}

bc_property::bc_property()
{
}

bc_property::bc_property(Token pTree)
{
	Token pID = GetChild(pTree,0);
	this->id = new bc_id(pID);
	Token pValue = GetChild(pTree,1);
	this->expr = bc_expr::bc_expr_create(pValue);
}

bc_property::~bc_property()
{
	delete this->id;
	delete this->expr;
}

bc_property* bc_property::clone()
{
	bc_property* copy = new bc_property();
	copy->id = id->clone();
	copy->expr = expr->clone();
	return copy;
}

bc_classdef::bc_classdef()
:bc_expr()
{
}

bc_classdef::bc_classdef(Token pTree)
:bc_expr(pTree)
{
	this->id = 0;
	Token pInheriting = GetChild(pTree,0);
	if(GetChildCount(pInheriting)==1) {
		Token pInheritingExpr = GetChild(pInheriting,0);
		this->inheriting = bc_expr::bc_expr_create(pInheritingExpr);
	} else {
		this->inheriting = 0;
	}
	Token pMembers = GetChild(pTree,1);
    for (unsigned int i=0;i<GetChildCount(pMembers);i++) {
        Token pClassMember = GetChild(pMembers,i);
		switch(GetType(pClassMember)) {
			case PROPERTY:
				this->properties.push_back(new bc_property(pClassMember));
				break;
			case METHOD:
				this->methods.push_back(new bc_method(pClassMember));
				break;
		}
    }
}

bc_classdef::~bc_classdef()
{
	delete this->id;
	delete this->inheriting;
	for(std::vector<bc_property*>::iterator i=this->properties.begin(); i!=this->properties.end(); i++) {
		delete (*i);
	}
	for(std::vector<bc_method*>::iterator i=this->methods.begin(); i!=this->methods.end(); i++) {
		delete (*i);
	}
}

bc_classdef* bc_classdef::clone()
{
	bc_classdef* copy = new bc_classdef();
	copy->id = id->clone();
	copy->inheriting = inheriting?inheriting->clone():inheriting;
	for(std::vector<bc_method*>::iterator i=methods.begin();i!=methods.end();i++) {
		copy->methods.push_back((*i)->clone());
	}
	for(std::vector<bc_property*>::iterator i=properties.begin();i!=properties.end();i++) {
		copy->properties.push_back((*i)->clone());
	}
	copy->line = line;
	return copy;
}

object* bc_classdef::evaluate()
{
	object* script_class;
	if(this->inheriting!=0) {
		object* superclass = this->inheriting->evaluate();
		method* superclass_new = superclass->getMethod(L"new(0)");
		script_class = superclass_new->call(1, superclass);
	} else {
		script_class = new object(&object::object_class);
	}
	if(this->id!=0) {
		script_class->setClassname(this->id->name);
	} else {
		script_class->setClassname(L"object<inline>");
	}
	for(std::vector<bc_method*>::iterator i=this->methods.begin();i!=this->methods.end();i++) {
		method* script_method = new method((*i));
		std::wstringstream method_signature_out;
		std::wstring method_signature;
		if((*i)->varargs) {
			method_signature_out << (*i)->id->name << L"(...)";
			method_signature = method_signature_out.str();
		} else {
			method_signature_out << (*i)->id->name << L"(" << (*i)->params.size() << L")";
			method_signature = method_signature_out.str();
		}
		script_class->setMethod(method_signature, script_method, (*i)->docstring);
	}
	for(std::vector<bc_property*>::iterator i=this->properties.begin();i!=this->properties.end();i++) {
		object* script_property_value = (*i)->expr->evaluate();
		script_class->setProperty((*i)->id->name, script_property_value);
	}
	return script_class;
}

//// NATIVE -> SCRIPT METHOD CALLS ////

object* bc_method::call(list* method_params)
{
	scope* method_scope = new scope(0);
	//Populate method scope with values from params
	if(this->varargs) {
		if(method_params->getObject(0)->ancestor) {
			method_scope->setNamedObject(L"super", method_params->getObject(0)->ancestor);
		}
		method_scope->setNamedObject(L"self", method_params->getObject(0));
		method_scope->setNamedObject(L"args", method_params->getSlice(1));
	} else {
		int param_pos = 0;
		for(std::vector<bc_id*>::iterator i=this->params.begin();i!=this->params.end();i++) {
			method_scope->setNamedObject((*i)->name, method_params->getObject(param_pos+1));
			param_pos++;
		}
		if(method_params->getObject(0)->ancestor) {
			method_scope->setNamedObject(L"super", method_params->getObject(0)->ancestor);
		}
		method_scope->setNamedObject(L"self", method_params->getObject(0));
	}
	//Enter method scope
	runtime::runtime_class.enterScope(method_scope);
	//Run
	object* result = this->code->execute();
	//Go back to previous scope
	runtime::runtime_class.exitScope();
	return result;
}
