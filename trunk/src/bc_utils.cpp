//
//  bc_utils.cpp
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

#include "bc_utils.h"
#include "runtime.h"

using namespace esharp;

static void	reportError_esharp (pANTLR3_BASE_RECOGNIZER recognizer)
{
    if	(recognizer->state->errorRecovery == ANTLR3_TRUE) {
		return;
    }
    recognizer->state->errorRecovery = ANTLR3_TRUE;
	recognizer->state->errorCount++;
    recognizer->displayRecognitionError(recognizer, recognizer->state->tokenNames);
}

Token esharp::ParseLines(const char* line) {
	pANTLR3_INPUT_STREAM pNewStrm = antlr3NewAsciiStringInPlaceStream((pANTLR3_UINT8)line, strlen(line), (pANTLR3_UINT8)"stdin");
	pesharpLexer lex =  esharpLexerNew(pNewStrm);
	lex->pLexer->rec->reportError = reportError_esharp;
	pANTLR3_COMMON_TOKEN_STREAM   tstream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT,TOKENSOURCE(lex));
	pesharpParser ps = esharpParserNew( tstream );
	ps->pParser->rec->reportError = reportError_esharp;
	esharpParser_code_return ret = ps->code(ps);
	if(lex->pLexer->rec->getNumberOfSyntaxErrors(lex->pLexer->rec)>0 || ps->pParser->rec->getNumberOfSyntaxErrors(ps->pParser->rec)>0) {
		return 0;
	}
	return ret.tree;
}

Token esharp::Parse(const char* filename) {
	pANTLR3_INPUT_STREAM pNewStrm = antlr3AsciiFileStreamNew((pANTLR3_UINT8)(filename));
	pesharpLexer lex =  esharpLexerNew(pNewStrm);
	//TODO: Disable ANTLR display of errors when we have a method to do it properly above here ^^^reportError_esharp^^^
	//lex->pLexer->rec->reportError = reportError_esharp;
	pANTLR3_COMMON_TOKEN_STREAM   tstream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT,TOKENSOURCE(lex));
	pesharpParser ps = esharpParserNew( tstream );
	//ps->pParser->rec->reportError = reportError_esharp;
	esharpParser_code_return ret = ps->code(ps);
	if(lex->pLexer->rec->getNumberOfSyntaxErrors(lex->pLexer->rec)>0 || ps->pParser->rec->getNumberOfSyntaxErrors(ps->pParser->rec)>0) {
		return 0;
	}
	return ret.tree;
}

Token esharp::GetChild(Token tree, unsigned int index) {
	return (Token) tree->children->get(tree->children, index);
}
unsigned int esharp::GetChildCount(Token tree) {
	return tree->getChildCount(tree);
}

unsigned int esharp::GetType(Token tree) {
	return tree->getToken(tree)->getType(tree->getToken(tree));
}

const char* esharp::GetText(Token tree) {
	return (const char*)(tree->getText(tree)->chars);
}

unsigned int esharp::GetTextLen(Token tree) {
	return tree->getText(tree)->len;
}

unsigned int esharp::GetLine(Token tree) {
	return tree->getToken(tree)->getLine(tree->getToken(tree));
}

int esharp::GetStartIndex(Token tree) {
	return tree->getToken(tree)->getStartIndex(tree->getToken(tree));
}

int esharp::GetStopIndex(Token tree) {
	return tree->getToken(tree)->getStopIndex(tree->getToken(tree));
}

void esharp::debugPrintTree(int level, Token pTree)
{
    for (int j=0; j<level; j++) {
        std::cout << "  ";
    }
	std::cout << GetText(pTree) << std::endl;
    unsigned int childcount =  GetChildCount(pTree);
    for (unsigned int i=0;i<childcount;i++) {
        Token pChild = GetChild(pTree,i);
        debugPrintTree(level+1, pChild);
    }
}
