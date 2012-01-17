//
//  runtime.cpp
//  E#
//
//  Created by Adrian Punga on 01/03/2012.
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

// Declarations
#include "runtime.h"

// Class setup dependencies
///NONE

// Specific dependencies
#include "bc_script.h"

using namespace esharp;

/********************************
 CLASS SETUP
 ********************************/

runtime runtime::runtime_class;

runtime::runtime()
:object(&object::object_class)
{
	setClassname(L"runtime_class");
	global_scope = new scope(0);
	/* Types */
	global_scope->setNamedObject(L"object", &object::object_class);
	global_scope->setNamedObject(L"none", &object::object_none);
	global_scope->setNamedObject(L"boolean", &boolean::boolean_class);
	global_scope->setNamedObject(L"true", &boolean::boolean_true);
	global_scope->setNamedObject(L"false", &boolean::boolean_false);
	global_scope->setNamedObject(L"octet", &octet::octet_class);
	global_scope->setNamedObject(L"integer", &integer::integer_class);
	global_scope->setNamedObject(L"real", &real::real_class);
	global_scope->setNamedObject(L"data", &data::data_class);
	global_scope->setNamedObject(L"str", &str::str_class);
	global_scope->setNamedObject(L"list", &list::list_class);
	global_scope->setNamedObject(L"dict", &dict::dict_class);
	//Notice that method is not missing here, it should not be put in userspace
	global_scope->setNamedObject(L"exception", &exception::exception_class);
	//Notice that thrown_exception is not missing here, it should not be put in userspace
	global_scope->setNamedObject(L"rwstream", &rwstream::rwstream_class);
	global_scope->setNamedObject(L"fs", &fs::fs_class);

	/* Libraries */
	global_scope->setNamedObject(L"stdout", &console::console_stdout);
	global_scope->setNamedObject(L"stderr", &console::console_stderr);

	/* Others */
	global_scope->setNamedObject(L"runtime", this);
	global_scope->setNamedObject(L"gc", &gc::gc_class);
	
	current_scope = global_scope;
	current_object = global_scope;
	current_source = 0;

	setMethod(L"import(1)", new method(import), 
		L"none <- runtime.import(str <filename>)"
		L"\n\tRuns the script specified by <filename> in the current runtime."
		L"\n\tAny changes made to the current scope by the script remain active after the script ends.");
}

runtime::~runtime()
{
	delete global_scope;
}

/********************************
 HELPER METHODS
 ********************************/

scope* runtime::getCurrentScope()
{
	return current_scope;
}

void runtime::enterScope(scope* scope_param)
{
	scope_param->setParent(current_scope);
	current_scope = scope_param;
}

void runtime::exitScope()
{
	scope* old_scope = current_scope;
	current_scope = current_scope->getParent();
	delete old_scope;
}

object* runtime::getCurrentObject()
{
	return current_object;
}

void runtime::setCurrentObject(object* object_param)
{
	current_object = object_param;
}

const wchar_t* runtime::getCurrentSource()
{
	return current_source;
}

void runtime::setCurrentSource(const wchar_t* source_param)
{
	current_source = source_param;
}

/********************************
 NATIVE METHODS
 ********************************/

object* runtime::import(list* param)
{
	runtime* self = dynamic_cast<runtime*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	str* filename = dynamic_cast<str*>(param->getObject(1));
	if(!filename) {
		return new thrown_exception(new exception(L"Invalid arguments"));
	}
	data* filename_utf8 = dynamic_cast<data*>(filename->getMethod(L"encode_utf8(0)")->call(1, filename));
	if(!filename_utf8) {
		return filename_utf8;
	}
	FILE* f = fopen((const char*)(filename_utf8->getValue()),"r");
	if(!f) {
		std::wstringstream err;
		err << L"Importing '" << filename->getValue() << "' failed as script was not found";
		return new thrown_exception(new exception(err.str()));
	}
	fclose(f);
	bc_script imported_script((const char*)(filename_utf8->getValue()));
	if(!imported_script.run()) {
		std::wstringstream err;
		err << L"Importing '" << filename->getValue() << "' failed due to uncaught exception";
		return new thrown_exception(new exception(err.str()));
	}
	return &object::object_none;
}
