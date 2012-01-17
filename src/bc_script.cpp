//
//  bc_script.cpp
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

#include "bc_script.h"
#include "bc_code.h"
#include "bc_utils.h"
#include "runtime.h"
#include "version.h"

using namespace esharp;

bc_script::bc_script()
{
	this->bytecode = 0;
	this->filename = "stdin";
}

bc_script::bc_script(const std::string& filename_param)
{
	Token pTree = Parse(filename_param.c_str());
	this->bytecode = 0;
	if(pTree) {
		//debugPrintTree(0, pTree);
		this->bytecode = new bc_code(pTree);
		this->filename = filename_param;
	}
}

bc_script::~bc_script()
{
	delete this->bytecode;
}

bool bc_script::run()
{
	if(this->bytecode) {
		//Setup
		std::wstringstream wfilename_s;
		wfilename_s << filename.c_str();
		std::wstring wfilename = wfilename_s.str();
		const wchar_t* prev_source = runtime::runtime_class.getCurrentSource();
		runtime::runtime_class.setCurrentSource(wfilename.c_str());
		//Run
		{
			//Execute code
			object* result = this->bytecode->execute();
			//Check for errors and run final gc round
			if(result && result->isInstanceOf(&thrown_exception::thrown_exception_class)) {
				std::wcerr << std::endl << "Abnormal termination due to uncaught exception:" << std::endl;
				thrown_exception* uncaught = dynamic_cast<thrown_exception*>(result);
				method* print_trace = uncaught->getException()->getMethod(L"print_trace(1)");
				print_trace->call(2, uncaught->getException(), &console::console_stderr);
				delete uncaught;
				//Run final gc round
				gc::gc_class.runGC();
				//Get back to the importing source if any
				runtime::runtime_class.setCurrentSource(prev_source);
				return false;
			}
		}
		//Cleanup
		runtime::runtime_class.setCurrentSource(prev_source);
		gc::gc_class.runGC();
		return true;
	} else {
		return false;
	}
}

bool bc_script::runInteractive()
{
	if(!this->bytecode) {
		//Setup
		std::wstringstream wfilename_s;
		wfilename_s << filename.c_str();
		std::wstring wfilename = wfilename_s.str();
		const wchar_t* prev_source = runtime::runtime_class.getCurrentSource();
		runtime::runtime_class.setCurrentSource(wfilename.c_str());
		//Run
		std::cout << "E# " << ESHARP_VERSION_MAJOR << "." << ESHARP_VERSION_MINOR << "." << ESHARP_VERSION_REV << std::endl;
		std::cout << "Type 'anything.doc()' for help on using E# classes." << std::endl;
		while(true) {
			std::stringstream codeblock_stream;
			std::cout << ">>> ";
			int lines = 0;
			while(true) {
				lines++;
				char line[1024];
				std::cin.getline(line, 1024);
				//Test if we end reading...
				int len = strlen(line);
				if(line[len-1]!='\\') {
					codeblock_stream << line << std::endl;
					break;
				} else {
					//Delete final '\'
					line[len-1] = 0;
					codeblock_stream << line << std::endl;
				}
				std::cout << "... ";
			}
			if(lines==1) {
				//It doesn't hurt to add a ';' so that the interactive user doesn't need to
				codeblock_stream << ';';
			}
			std::string codeblock = codeblock_stream.str();
			Token pTree = ParseLines(codeblock.c_str());
			if(pTree) {
				bc_code code(pTree);
				//Clear interactive return
				runtime::runtime_class.getCurrentScope()->setNamedObject(L"", &object::object_none);
				object* result = code.execute();
				//Check for errors and run between the lines gc
				if(result && result->isInstanceOf(&thrown_exception::thrown_exception_class)) {
					std::wcerr << std::endl << "Uncaught exception:" << std::endl;
					thrown_exception* uncaught = dynamic_cast<thrown_exception*>(result);
					method* print_trace = uncaught->getException()->getMethod(L"print_trace(1)");
					print_trace->call(2, uncaught->getException(), &console::console_stderr);
					delete uncaught;
					//Run between the lines gc
					gc::gc_class.runGC();
				} else {
					//Look for interactive return
					object* interactive_return = runtime::runtime_class.getCurrentScope()->getNamedObject(L"");
					if(interactive_return!=&object::object_none) {
						object* repr = interactive_return->getMethod(L"operator_cast(1)")->call(2, interactive_return, &str::str_class);
						if(repr->isInstanceOf(&thrown_exception::thrown_exception_class)) {
							std::wcerr << std::endl << "Uncaught exception:" << std::endl;
							thrown_exception* uncaught = dynamic_cast<thrown_exception*>(repr);
							method* print_trace = uncaught->getException()->getMethod(L"print_trace(1)");
							print_trace->call(2, uncaught->getException(), &console::console_stderr);
							delete uncaught;
							//Run between the lines gc
							gc::gc_class.runGC();
						}
						//Display returned result
						str* repr_str = dynamic_cast<str*>(repr);
						std::wcout << repr_str->getValue() << std::endl;
					}
					//Mark interactive return as used
					runtime::runtime_class.getCurrentScope()->setNamedObject(L"", &object::object_none);
				}
			} else {
				std::wcerr << "Syntax error" << std::endl;
			}
		}
		//Cleanup
		runtime::runtime_class.setCurrentSource(prev_source);
		gc::gc_class.runGC();
		return true;
	} else {
		return false;
	}
}

