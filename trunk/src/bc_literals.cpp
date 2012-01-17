//
//  bc_literals.cpp
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

#include "bc_literals.h"
#include "runtime.h"

using namespace esharp;

bc_literal::bc_literal()
:bc_expr()
{
}

bc_literal::bc_literal(Token pTree)
:bc_expr(pTree)
{
}

bc_literal::~bc_literal()
{
}

bc_none::bc_none()
:bc_literal()
{
}

bc_none::bc_none(Token pTree)
:bc_literal(pTree)
{
}

bc_none::~bc_none()
{
}

bc_none* bc_none::clone()
{
	return new bc_none();
}

bc_boolean::bc_boolean()
:bc_literal()
{
}

bc_boolean::bc_boolean(Token pTree)
:bc_literal(pTree)
{
	if(GetType(pTree)==TRUELIT) {
		this->value = true;
	} else if(GetType(pTree)==FALSELIT) {
		this->value = false;
	}
}

bc_boolean::~bc_boolean()
{
}

bc_boolean* bc_boolean::clone()
{
	bc_boolean* copy = new bc_boolean();
	copy->value = value;
	copy->line = line;
	return copy;
}

bc_octet::bc_octet()
:bc_literal()
{
}

bc_octet::bc_octet(Token pTree)
:bc_literal(pTree)
{
	unsigned int length = GetTextLen(pTree);
	char* strInteger = new char[length+1];
	memcpy(strInteger, GetText(pTree)+2, length-2);
	strInteger[length-2] = 0;
	this->value = (unsigned char) strtol(strInteger, NULL, 16);
	delete [] strInteger;
}

bc_octet::~bc_octet()
{
}

bc_octet* bc_octet::clone()
{
	bc_octet* copy = new bc_octet();
	copy->value = value;
	copy->line = line;
	return copy;
}

bc_integer::bc_integer()
:bc_literal()
{
}

bc_integer::bc_integer(Token pTree)
:bc_literal(pTree)
{
	unsigned int length = GetTextLen(pTree);
	char* strInteger = new char[length+1];
	memcpy(strInteger, GetText(pTree), length);
	strInteger[length] = 0;
	this->value = atoi(strInteger);
	delete [] strInteger;
}

bc_integer::~bc_integer()
{
}

bc_integer* bc_integer::clone()
{
	bc_integer* copy = new bc_integer();
	copy->value = value;
	copy->line = line;
	return copy;
}

bc_real::bc_real()
:bc_literal()
{
}

bc_real::bc_real(Token pTree)
:bc_literal(pTree)
{
	unsigned int length = GetTextLen(pTree);
	char* strDouble = new char[length+1];
	memcpy(strDouble, GetText(pTree), length);
	strDouble[length] = 0;
	this->value = atof(strDouble);
	delete [] strDouble;
}

bc_real::~bc_real()
{
}

bc_real* bc_real::clone()
{
	bc_real* copy = new bc_real();
	copy->value = value;
	copy->line = line;
	return copy;
}

bc_data::bc_data()
:bc_literal()
{
}

bc_data::bc_data(Token pTree)
:bc_literal(pTree)
{
	unsigned int length = GetTextLen(pTree);
	unsigned char* temp = new unsigned char[length-2];
	memcpy(temp, GetText(pTree)+1, length-2);
	unsigned char* tempEsc = new unsigned char[length-2];
	unsigned int rpos = 0;
	unsigned int wpos = 0;
	while(rpos<length-2) {
		switch(temp[rpos]) {
			case '\\':
				rpos++; //'b'|'t'|'n'|'f'|'r'|'\"'|'\\')
				switch(temp[rpos]) {
					case 'x':
						char dg[3];
						rpos++;
						dg[0] = temp[rpos];
						rpos++;
						dg[1] = temp[rpos];
						dg[2] = 0;
						tempEsc[wpos] = (unsigned char)strtol(dg, NULL, 16);
						break;
					case 'b':
						tempEsc[wpos] = '\b';
						break;
					case 't':
						tempEsc[wpos] = '\t';
						break;
					case 'n':
						tempEsc[wpos] = '\n';
						break;
					case 'f':
						tempEsc[wpos] = '\f';
						break;
					case 'r':
						tempEsc[wpos] = '\r';
						break;
					case '\'':
						tempEsc[wpos] = '\'';
						break;
					case '\\':
						tempEsc[wpos] = '\\';
						break;
					default:
						tempEsc[wpos++] = '\\';
						tempEsc[wpos] = temp[rpos];
				}
				break;
			default:
				tempEsc[wpos] = temp[rpos];
		}
		rpos++;
		wpos++;
	}
	this->value = tempEsc;
	this->length = wpos;
	delete[] temp;
}

bc_data::~bc_data()
{
	delete[] this->value;
}

bc_data* bc_data::clone()
{
	bc_data* copy = new bc_data();
	copy->value = new unsigned char[length];
	memcpy(copy->value, value, length);
	copy->length = length;
	copy->line = line;
	return copy;
}

bc_string::bc_string()
:bc_literal()
{
}

bc_string::bc_string(Token pTree)
:bc_literal(pTree)
{
	unsigned int length = GetTextLen(pTree);
	wchar_t* temp = new wchar_t[length-1];
	mbstowcs(temp, GetText(pTree)+1, length-1);
	temp[length-2]=0;
	wchar_t* tempEsc = new wchar_t[length-1];
	unsigned int rpos = 0;
	unsigned int wpos = 0;

	while(rpos<length-1) {
		switch(temp[rpos]) {
			case '\\':
				rpos++; //'b'|'t'|'n'|'f'|'r'|'\"'|'\\')
				switch(temp[rpos]) {
					case 'u':
						char dg[5];
						rpos++;
						dg[0] = (char)temp[rpos];
						rpos++;
						dg[1] = (char)temp[rpos];
						rpos++;
						dg[2] = (char)temp[rpos];
						rpos++;
						dg[3] = (char)temp[rpos];
						dg[4] = 0;
						tempEsc[wpos] = (wchar_t)strtol(dg, NULL, 16);
						break;
					case 'b':
						tempEsc[wpos] = '\b';
						break;
					case 't':
						tempEsc[wpos] = '\t';
						break;
					case 'n':
						tempEsc[wpos] = '\n';
						break;
					case 'f':
						tempEsc[wpos] = '\f';
						break;
					case 'r':
						tempEsc[wpos] = '\r';
						break;
					case '\"':
						tempEsc[wpos] = '\"';
						break;
					case '\\':
						tempEsc[wpos] = '\\';
						break;
					default:
						tempEsc[wpos++] = '\\';
						tempEsc[wpos] = temp[rpos];
				}
				break;
			default:
				tempEsc[wpos] = temp[rpos];
		}
		rpos++;
		wpos++;
	}
	tempEsc[wpos]=0;
	this->value = tempEsc;
	delete[] temp;
}

bc_string::~bc_string()
{
}

bc_string* bc_string::clone()
{
	bc_string* copy = new bc_string();
	copy->value = value;
	copy->line = line;
	return copy;
}

bc_docstring::bc_docstring()
{
}

bc_docstring::bc_docstring(Token pTree)
{
	unsigned int length = GetTextLen(pTree);
	unsigned int slen = length - 6;
	wchar_t* temp = new wchar_t[slen+1];
	mbstowcs(temp, GetText(pTree)+3, slen);
	temp[slen]=0;
	wchar_t* tempEsc = new wchar_t[slen+1];
	memcpy(tempEsc, temp, sizeof(wchar_t)*slen);
	tempEsc[slen]=0;
	this->value = tempEsc;
	delete[] temp;
}

bc_docstring::~bc_docstring()
{
}

/*************************************
	Literal evaluation
 *************************************/

object* bc_none::evaluate()
{
	return &object::object_none;
}
object* bc_boolean::evaluate()
{
	if(this->value) {
		return &boolean::boolean_true;
	} else {
		return &boolean::boolean_false;
	}
}
object* bc_data::evaluate()
{
	if(this->length==0) {
		return &data::data_empty;
	}
	return new data(this->value, this->length);
}
object* bc_string::evaluate()
{
	if(this->value.size()==0) {
		return &str::str_empty;
	}
	return new str(this->value);
}
object* bc_octet::evaluate()
{
	return octet::get(this->value);
}
object* bc_integer::evaluate()
{
	switch(this->value) {
		case 0:
			return &integer::integer_0;
		case 1:
			return &integer::integer_1;
		default:
			return new integer(this->value);
	}
}
object* bc_real::evaluate()
{
	return new real(this->value);
}

