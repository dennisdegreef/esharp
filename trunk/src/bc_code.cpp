//
//  bytecode.cpp
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

#include "bc_code.h"
#include "bc_chunk.h"
#include "runtime.h"

using namespace esharp;

bc_code::bc_code()
{
}

bc_code::bc_code(Token pTree)
{
    for (unsigned int i=0;i<GetChildCount(pTree);i++) {
        Token pChild = GetChild(pTree,i);
		bc_chunk* chunk = bc_chunk::bc_chunk_create(pChild);
		if(chunk) {
			this->chunks.push_back(chunk);
		}
    }
}

bc_code::~bc_code()
{
	for(std::vector<bc_chunk*>::iterator i=this->chunks.begin(); i!=this->chunks.end(); i++) {
		delete (*i);
	}
}

bc_code* bc_code::clone()
{
	bc_code* copy = new bc_code();
	for(std::vector<bc_chunk*>::iterator i=chunks.begin();i!=chunks.end();i++) {
		copy->chunks.push_back((*i)->clone());
	}
	return copy;
}

object* bc_code::execute()
{
	object* result = 0;
	for(std::vector<bc_chunk*>::iterator i=chunks.begin(); i!=chunks.end(); i++) {
		//Only return, throw, break & continue statements are allowed to return a result here
		result = (*i)->execute();
		gc::gc_class.runGC(result); // <- Let GC know that the returned object should not be freed
		if(result) {
			break;
		}
	}
	return result;
}


