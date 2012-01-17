//
//  data.cpp
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
#include "data.h"

// Class setup dependencies
#include "list.h"
#include "method.h"
#include "boolean.h"
#include "exception.h"
#include "thrown_exception.h"
#include "rwstream.h"

// Specific dependencies
#include "integer.h"
#include "octet.h"
#include "str.h"

using namespace esharp;

/********************************
 CLASS SETUP
 ********************************/

data data::data_class;
data data::data_empty((const unsigned char*)"", 0);

data::data()
:object(&object::object_class)
{
	setClassname(L"data_class");
    setMethod(L"new(0)", new method(new_instance));
    setMethod(L"operator_eq(1)", new method(operator_eq));
	setMethod(L"len(0)", new method(len), 
		L"integer <- data.len()"
		L"\n\tReturns the number of octets in this data.");
	setMethod(L"get(1)", new method(get), 
		L"octet <- data.get(integer <pos>)"
		L"\n\tReturns the octet at index <pos>.");
	setMethod(L"set(2)", new method(set), 
		L"none <- data.set(integer <pos>, octet <value>)"
		L"\n\tSets the octet at index <pos> to <value>.");
    setMethod(L"decode_utf8(0)", new method(decode_utf8), 
		L"str <- data.decode_utf8()"
		L"\n\tDecodes this UTF-8 data to str."
		L"\n\tIf data contains non-UTF8 octets an exception is thrown.");
    setMethod(L"decode_ascii(0)", new method(decode_ascii), 
		L"str <- data.decode_ascii()"
		L"\n\tDecodes this ASCII data to str."
		L"\n\tIf data contains non-ASCII octets they're converted to the character '?'.");
    setMethod(L"decode_ascii_strict(0)", new method(decode_ascii_strict), 
		L"str <- data.decode_ascii_strict()"
		L"\n\tDecodes this ASCII data to str."
		L"\n\tIf data contains non-ASCII octets an exception is thrown.");
	setMethod(L"read(1)", new method(read), 
		L"none <- data.read(rwstream <s>)"
		L"\n\tReads the remaining octets from rwstream <s> to this data.");
	setMethod(L"write(1)", new method(write), 
		L"none <- data.write(rwstream <s>)"
		L"\n\tWrites all the octets in this data to rwstream <s>.");
	setMethod(L"read(...)", new method(read), 
		L"none <- data.read(rwstream <s>, integer <length>)"
		L"\n\tReads <length> octets from rwstream <s> to this data.");
	setMethod(L"write(...)", new method(write), 
		L"none <- data.write(rwstream <s>, [integer <offset>,] integer <length>)"
		L"\n\tWrites <length> octets from this data starting at 0 or at the optional <offset> position to rwstream <s>.");
}

data::data(const unsigned char* data_param, unsigned int length)
:object(&data::data_class)
{
	setClassname(L"data");
	value.resize(length+1);
	if(length>0) {
		memcpy(&value[0], data_param, length);
	}
	value[length] = 0;
}

data::~data()
{
}

/********************************
 HELPER METHODS
 ********************************/

const unsigned char* data::getValue()
{
	return &value[0];
}

unsigned int data::getLength()
{
	return value.size()-1;
}

unsigned int data::getHash()
{
	if(!hash) {
		std::wstringstream obj_repr;
		obj_repr << L"'" << (const char*)getValue() << L"'";
		hash = digest(obj_repr.str());
	}
	return hash;
}

/********************************
 NATIVE METHODS
 ********************************/

object* data::new_instance(list* param)
{
	return new data((const unsigned char*)"",0);
}

object* data::operator_eq(list* param)
{
	data* self = dynamic_cast<data*>(param->getObject(0));
	data* other = dynamic_cast<data*>(param->getObject(1));
	if (!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	if (!other) {
		return new thrown_exception(new exception(L"Invalid arguments"));
	}
	if (self->value == self->value) {
		return &boolean::boolean_true;
	} else {
		return &boolean::boolean_false;
	}
}

object* data::len(list* param)
{
	data* self = dynamic_cast<data*>(param->getObject(0));
	if (!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	return new integer(self->getLength());
}

object* data::get(list* param)
{
	data* self = dynamic_cast<data*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	integer* pos_obj = dynamic_cast<integer*>(param->getObject(1));
	if(!pos_obj) {
		return new thrown_exception(new exception(L"Data index has to be 'integer'"));
	}
	if(pos_obj->value<0 || pos_obj->value>=(int)(self->getLength())) {
		return new thrown_exception(new exception(L"Data index outside list bounds"));
	}
	return octet::get(self->value[pos_obj->value]);
}

object* data::set(list* param)
{
	data* self = dynamic_cast<data*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	integer* pos_obj = dynamic_cast<integer*>(param->getObject(1));
	if(!pos_obj) {
		return new thrown_exception(new exception(L"Data index has to be 'integer'"));
	}
	if(pos_obj->value<0 || pos_obj->value>=(int)(self->getLength())) {
		return new thrown_exception(new exception(L"Data index outside list bounds"));
	}
	octet* item = dynamic_cast<octet*>(param->getObject(2));
	if(!item) {
		return new thrown_exception(new exception(L"Invalid argument"));
	}
	self->value[pos_obj->value] = item->value;
	return &object::object_none;
}

static int utf8_decoded_len(const unsigned char* utf8str, unsigned int srclen) {
	char* src = (char*) utf8str;
	int len = 0;
	while(srclen>0) {
		char c = *src;
		if((c&0x80)==0) {
			src++;
			srclen--;
		} else if((c&0xE0)==0xC0) {
			src+=2;
			srclen-=2;
		} else if((c&0xF0)==0xE0) {
			src+=3;
			srclen-=3;
		} else {
			return -1;
		}
		len++;
	}
	return len;
}

object* data::decode_utf8(list* param)
{
	data* self = dynamic_cast<data*>(param->getObject(0));
	if (!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	int len = self->getLength();
	const unsigned char* src = &(self->value[0]);
	int utf8_len = utf8_decoded_len(src, len);
	if(utf8_len==-1) {
		return new thrown_exception(new exception(L"Decode from UTF-8 failed"));
	}
	wchar_t* dst = new wchar_t[utf8_len+1];
	int srcpos = 0;
	int dstpos = 0;
	while(srcpos<len) {
		char c1 = src[srcpos];
		if((c1&0x80)==0) {
			dst[dstpos++] = (c1&0x7F);
		} else if((c1&0xE0)==0xC0) {
			if(srcpos>len-2) {
				return new thrown_exception(new exception(L"Decode from UTF-8 failed"));
			}
			char c2 = src[++srcpos];
			dst[dstpos++] = ((c1&0x1F)<<6) | (c2&0x3F);
		} else if((c1&0xF0)==0xE0) {
			if(srcpos>len-3) {
				return new thrown_exception(new exception(L"Decode from UTF-8 failed"));
			}
			char c2 = src[++srcpos];
			char c3 = src[++srcpos];
			dst[dstpos++] = ((c1&0x0F)<<12) | ((c2&0x3F)<<6) | (c3&0x3F);
		} else {
			return new thrown_exception(new exception(L"Decode from UTF-8 failed"));
		}
		srcpos++;
	}
	dst[dstpos] = 0;
	str* ret = new str(dst);
	delete [] dst;
	return ret;
}

object* data::decode_ascii(list* param)
{
	data* self = dynamic_cast<data*>(param->getObject(0));
	if (!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	int len = self->getLength();
	const unsigned char* src =  &(self->value[0]);
	wchar_t* dst = new wchar_t[len+1];
	int srcpos = 0;
	int dstpos = 0;
	while(srcpos<len) {
		dst[dstpos++] = src[srcpos++];
	}
	dst[dstpos] = 0;
	str* ret = new str(dst);
	delete [] dst;
	return ret;
}

object* data::decode_ascii_strict(list* param)
{
	data* self = dynamic_cast<data*>(param->getObject(0));
	if (!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	int len = self->getLength();
	const unsigned char* src =  &(self->value[0]);
	wchar_t* dst = new wchar_t[len+1];
	int srcpos = 0;
	int dstpos = 0;
	while(srcpos<len) {
		char c = src[srcpos];
		if((c&0x80)==0) {
			dst[dstpos++] = (c&0x7F);
		} else {
			return new thrown_exception(new exception(L"Decode from ASCII failed"));
		}
		srcpos++;
	}
	dst[dstpos] = 0;
	str* ret = new str(dst);
	delete [] dst;
	return ret;
}

object* data::read(list* param)
{
	data* self = dynamic_cast<data*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	rwstream* stream = dynamic_cast<rwstream*>(param->getObject(1));
	if(!stream) {
		return new thrown_exception(new exception(L"Invalid arguments"));
	}
	object* result;
	unsigned int length_read;
	if(param->getLength()>2) {
		if(param->getLength()==3) {
			integer* length = dynamic_cast<integer*>(param->getObject(2));
			if(!length) {
				return new thrown_exception(new exception(L"Invalid length argument"));
			}
			length_read = length->value;
		} else {
			return new thrown_exception(new exception(L"Too many arguments"));
		}
	} else {
		result = stream->availableData(&length_read);
		if(result) {
			return result;
		}
	}
	unsigned char* value_read = new unsigned char[length_read+1];
	result = stream->readDataToBuffer((char*)value_read, sizeof(char)*length_read);
	if(result) {
		delete [] value_read;
		return result;
	}
	value_read[length_read] = 0;
	data* ret = new data(value_read, length_read);
	delete [] value_read;
	return ret;
}

object* data::write(list* param)
{
	data* self = dynamic_cast<data*>(param->getObject(0));
	if(!self) {
		return new thrown_exception(new exception(L"Invalid self argument"));
	}
	rwstream* stream = dynamic_cast<rwstream*>(param->getObject(1));
	if(!stream) {
		return new thrown_exception(new exception(L"Invalid arguments"));
	}
	object* result;
	int offset_write;
	int length_write;
	if(param->getLength()>2) {
		if(param->getLength()==3) {
			integer* length = dynamic_cast<integer*>(param->getObject(2));
			if(!length) {
				return new thrown_exception(new exception(L"Invalid arguments"));
			}
			length_write = length->value;
			if(length_write > (int)(self->getLength())) {
				return new thrown_exception(new exception(L"Data index outside list bounds"));
			}
			offset_write = 0;
		} else if(param->getLength()==4) {
			integer* offset = dynamic_cast<integer*>(param->getObject(2));
			if(!offset) {
				return new thrown_exception(new exception(L"Invalid offset argument"));
			}
			offset_write = offset->value;
			integer* length = dynamic_cast<integer*>(param->getObject(3));
			if(!length) {
				return new thrown_exception(new exception(L"Invalid length argument"));
			}
			length_write = length->value;
			if(offset_write < 0 || offset_write + length_write > (int)(self->getLength())) {
				return new thrown_exception(new exception(L"Data index outside list bounds"));
			}
		} else {
			return new thrown_exception(new exception(L"Too many arguments"));
		}
	} else {
		offset_write = 0;
		length_write = self->getLength();
	}
	const unsigned char* value_write =  &(self->value[0]);
	result = stream->writeDataFromBuffer(((char*)value_write)+offset_write, sizeof(char)*length_write);
	if(result) {
		return result;
	}
	return &object::object_none;
}
