//
//  bc_call.cpp
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

#include "bc_call.h"
#include "bc_id.h"
#include "runtime.h"

using namespace esharp;

bc_message::bc_message()
{
}

bc_message::~bc_message()
{
	delete this->id;
	for(std::vector<bc_expr*>::iterator i=this->args.begin(); i!=this->args.end(); i++) {
		delete (*i);
	}
}

bc_message* bc_message::clone()
{
	bc_message* copy = new bc_message();
	copy->id = id->clone();
	for(std::vector<bc_expr*>::iterator i=args.begin();i!=args.end();i++) {
		copy->args.push_back((*i)->clone());
	}
	return copy;
}

bc_call::bc_call()
:bc_expr()
{
}

bc_call::bc_call(Token pTree)
:bc_expr(pTree)
{
	Token pObject = GetChild(pTree,0);
	this->receiver = bc_expr::bc_expr_create(pObject);
	for (unsigned int m=1;m<GetChildCount(pTree);m++) {
		bc_message* message = new bc_message();
		Token pMessage = GetChild(pTree,m);
		Token pMessageReceiver = GetChild(pMessage,0);
		message->id = new bc_id(pMessageReceiver);
		for (unsigned int i=1;i<GetChildCount(pMessage);i++) {
			Token pArg = GetChild(pMessage,i);
			message->args.push_back(bc_expr::bc_expr_create(pArg));
		}
		this->messages.push_back(message);
	}
}

bc_call::~bc_call()
{
	delete this->receiver;
	for(std::vector<bc_message*>::iterator i=this->messages.begin(); i!=this->messages.end(); i++) {
		delete (*i);
	}
}

bc_call* bc_call::clone()
{
	bc_call* copy = new bc_call();
	copy->receiver = receiver->clone();
	for(std::vector<bc_message*>::iterator i=messages.begin();i!=messages.end();i++) {
		copy->messages.push_back((*i)->clone());
	}
	copy->line = line;
	return copy;
}

object* bc_call::evaluate()
{
	object* receiverObj = this->receiver->evaluate();
	object* currentObj = receiverObj;
	for(std::vector<bc_message*>::iterator m=this->messages.begin();m!=this->messages.end();m++) {
		std::wstringstream method_signature_out;
		method_signature_out << (*m)->id->name << L"(" << (*m)->args.size() << L")";
		std::wstring method_signature = method_signature_out.str();
		method* message = currentObj->getMethod(method_signature);
		if(message==&method::method_class) {
			std::wstringstream method_signature_out;
			method_signature_out << (*m)->id->name << L"(...)";
			std::wstring method_signature = method_signature_out.str();
			message = currentObj->getMethod(method_signature);
			if(message==&method::method_class) {
				std::wstringstream error_out;
				error_out << L"No method called '" << (*m)->id->name << L"' that takes " << (*m)->args.size() << L" parameters or any number of parameters";
				exception *exc = new exception(error_out.str());
				std::wstringstream trace_method;
				trace_method << L"  File \"" << runtime::runtime_class.getCurrentSource() << "\", line " << this->receiver->line;
				exc->addTrace(trace_method.str());
				return new thrown_exception(exc);
			}
		}
		list arguments(0);
		arguments.appendObject(currentObj);
		for(std::vector<bc_expr*>::iterator i=(*m)->args.begin();i!=(*m)->args.end();i++) {
			object* argeval_result = (*i)->evaluate();
			if(argeval_result->isInstanceOf(&thrown_exception::thrown_exception_class)) {
				return argeval_result;
			}
			arguments.appendObject(argeval_result);
			//Take ownership of arguments while the method is called
			gc::gc_class.removePointer(argeval_result);
		}

		//Enter object property scope
		object* previousObject = runtime::runtime_class.getCurrentObject();
		runtime::runtime_class.setCurrentObject(currentObj);

		//Run message
		object* result = message->call(&arguments);

		for(int i=0; i<arguments.getLength(); i++) {
			//Give back ownership of arguments after the method was called
			gc::gc_class.removePointer(arguments.getObject(i));
		}
		if(result && result->isInstanceOf(&thrown_exception::thrown_exception_class)) {
			thrown_exception* exc = dynamic_cast<thrown_exception*>(result);
			std::wstringstream trace_method;
			trace_method << L"  File \"" << runtime::runtime_class.getCurrentSource() << "\", line " << this->receiver->line << ", in " << method_signature;
			exc->getException()->addTrace(trace_method.str());
		}

		//Go back to previous object
		runtime::runtime_class.setCurrentObject(previousObject);

		currentObj = result;

		if(result && result->isInstanceOf(&thrown_exception::thrown_exception_class)) {
			//Exception was thrown so let's get out of executing the pseudo-recursive calls
			break;
		}
	}
	if(currentObj==0) {
		currentObj = &object::object_none;
	}
	return currentObj;
}
