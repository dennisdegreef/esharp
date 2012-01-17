//
//  bc_utils.h
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

#ifndef E__bc_utils_h
#define E__bc_utils_h

#include "esharpLexer.h"
#include "esharpParser.h"

#include <string>
#include <vector>
#include <iostream>

namespace esharp {
	typedef pANTLR3_BASE_TREE Token;

	Token ParseLines(const char* line);
	Token Parse(const char* filename);
	Token GetChild(Token tree, unsigned int index);
	unsigned int GetChildCount(Token tree);
	unsigned int GetType(Token tree);
	const char* GetText(Token tree);
	unsigned int GetTextLen(Token tree);
	unsigned int GetLine(Token tree);
	int GetStartIndex(Token tree);
	int GetStopIndex(Token tree);

	bool hasSourcePointer(Token pTree);
	void debugPrintTree(int level, Token pTree);
}

#endif
