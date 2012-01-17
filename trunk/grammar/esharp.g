//
//  esharp.g
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

grammar esharp;

options {
	language = 'C';
	k = 3;
	output=AST;
	ASTLabelType=pANTLR3_BASE_TREE;
}

tokens {
	/* ROOT */
	CODE;
	/* DEFINING CLASSES */
	CLASSDEF;
		INHERITING;
		MEMBERS;
		METHOD;
			DOCSTRING;
			METHODDEF;
				PARAMS;
				VARARGS;
		PROPERTY;
			DEFAULT;
	/* STATEMENTS */
	ASSIGN;
	DEL;
	TRY;
		CATCH;
	THROW;
	RETURN;
	WHILE;
	FOR;
		INIT;
		COND;
		STEP;
	FOREACH;
	IF;
		ELSE;
	BREAK;
	CONTINUE;
	BLOCK;
	/* EXPRESSIONS */
	CALL;
		MESSAGE;
}
	
code
	: chunk* -> ^(CODE chunk*)
	;

chunk
	: stmt
	| classdef
	;

classdef
	: lc='class' ID (':' expr)? '{' member* '}' -> ^(ASSIGN[$lc,"ASSIGN"] ID ^(CLASSDEF[$lc,"CLASSDEF"] ^(INHERITING[$lc,"INHERITING"] expr?) ^(MEMBERS[$lc,"MEMBERS"] member*))) 
	;

member
	: method
	| property
	;

method
	: lc=ID '(' paramlist? ')' '{' docstring? chunk* '}' -> ^(METHOD[$lc,"METHOD"] ID ^(METHODDEF[$lc,"METHODDEF"] ^(PARAMS[$lc,"PARAMS"] paramlist?) ^(DOCSTRING docstring?) ^(CODE[$lc,"CODE"] chunk*)))
	;

paramlist
	: ID (',' ID)* -> ID+
	| lc='...' -> VARARGS[$lc,"VARARGS"]
	;

property
	: lc=ID '=' expr ';' -> ^(PROPERTY[$lc,"PROPERTY"] ID expr)
	| lc=ID ';' -> ^(PROPERTY[$lc,"PROPERTY"] ID NONE)
	| 'readwrite' lc=ID '=' expr ';' -> ^(PROPERTY[$lc,"PROPERTY"] ID expr)
				    ^(METHOD[$lc,"METHOD"] ID ^(METHODDEF[$lc,"METHODDEF"] ^(PARAMS[$lc,"PARAMS"] ID["value"]) ^(DOCSTRING) ^(CODE[$lc,"CODE"] ^(ASSIGN[$lc,"ASSIGN"] ID ID["value"]))))
				    ^(METHOD[$lc,"METHOD"] ID ^(METHODDEF[$lc,"METHODDEF"] ^(PARAMS[$lc,"PARAMS"]) ^(DOCSTRING) ^(CODE[$lc,"CODE"] ^(RETURN[$lc,"RETURN"] ID))))
	| 'readwrite' lc=ID ';' -> ^(PROPERTY[$lc,"PROPERTY"] ID NONE)
				    ^(METHOD[$lc,"METHOD"] ID ^(METHODDEF[$lc,"METHODDEF"] ^(PARAMS[$lc,"PARAMS"] ID["value"]) ^(DOCSTRING) ^(CODE[$lc,"CODE"] ^(ASSIGN[$lc,"ASSIGN"] ID ID["value"]))))
				    ^(METHOD[$lc,"METHOD"] ID ^(METHODDEF[$lc,"METHODDEF"] ^(PARAMS[$lc,"PARAMS"]) ^(DOCSTRING) ^(CODE[$lc,"CODE"] ^(RETURN[$lc,"RETURN"] ID))))
	| 'readonly' lc=ID '=' expr ';' -> ^(PROPERTY[$lc,"PROPERTY"] ID expr)
				    ^(METHOD[$lc,"METHOD"] ID ^(METHODDEF[$lc,"METHODDEF"] ^(PARAMS[$lc,"PARAMS"]) ^(DOCSTRING) ^(CODE[$lc,"CODE"] ^(RETURN[$lc,"RETURN"] ID))))
	| 'readonly' lc=ID ';' -> ^(PROPERTY[$lc,"PROPERTY"] ID NONE)
				    ^(METHOD[$lc,"METHOD"] ID ^(METHODDEF[$lc,"METHODDEF"] ^(PARAMS[$lc,"PARAMS"]) ^(DOCSTRING) ^(CODE[$lc,"CODE"] ^(RETURN[$lc,"RETURN"] ID))))
	| 'writeonly' lc=ID '=' expr ';' -> ^(PROPERTY[$lc,"PROPERTY"] ID expr)
				    ^(METHOD[$lc,"METHOD"] ID ^(METHODDEF[$lc,"METHODDEF"] ^(PARAMS[$lc,"PARAMS"] ID["value"]) ^(DOCSTRING) ^(CODE[$lc,"CODE"] ^(ASSIGN[$lc,"ASSIGN"] ID ID["value"]))))
	| 'writeonly' lc=ID ';' -> ^(PROPERTY[$lc,"PROPERTY"] ID NONE)
				    ^(METHOD[$lc,"METHOD"] ID ^(METHODDEF[$lc,"METHODDEF"] ^(PARAMS[$lc,"PARAMS"] ID["value"]) ^(DOCSTRING) ^(CODE[$lc,"CODE"] ^(ASSIGN[$lc,"ASSIGN"] ID ID["value"]))))
	;

stmt
	: expr_stmt
	| assign_stmt
	| del_stmt
	| return_stmt
	| throw_stmt
	| trycatch_stmt
	| ifelse_stmt
	| while_stmt
	| for_stmt
	| foreach_stmt
	| break_stmt
	| continue_stmt
	| block_stmt
	;

expr_stmt
	: expr ';'!
	;
assign_stmt
	: assign_expr ';'!
	;
assign_expr
	: lc=ID '=' expr -> ^(ASSIGN[$lc,"ASSIGN"] ID expr)
	| '++' lc=ID -> ^(ASSIGN[$lc,"ASSIGN"] ID ^(ADD["+"] ID INT["1"]))
	| '--' lc=ID -> ^(ASSIGN[$lc,"ASSIGN"] ID ^(SUB["-"] ID INT["1"]))
	| lc=ID '++' -> ^(ASSIGN[$lc,"ASSIGN"] ID ^(ADD["+"] ID INT["1"]))
	| lc=ID '--' -> ^(ASSIGN[$lc,"ASSIGN"] ID ^(SUB["-"] ID INT["1"]))
	| lc=ID '+=' expr -> ^(ASSIGN[$lc,"ASSIGN"] ID ^(ADD["+"] ID expr))
	| lc=ID '-=' expr -> ^(ASSIGN[$lc,"ASSIGN"] ID ^(SUB["-"] ID expr))
	| lc=ID '*=' expr -> ^(ASSIGN[$lc,"ASSIGN"] ID ^(MUL["*"] ID expr))
	| lc=ID '/=' expr -> ^(ASSIGN[$lc,"ASSIGN"] ID ^(DIV["/"] ID expr))
	| lc=ID '%=' expr -> ^(ASSIGN[$lc,"ASSIGN"] ID ^(MOD["\%"] ID expr))
	;
del_stmt
	: lc='del' ID ';' -> ^(DEL[$lc,"DEL"] ID)
	;
return_stmt
	: lc='return' expr? ';' -> ^(RETURN[$lc,"RETURN"] expr?)
	;
throw_stmt
	: lc='throw' expr ';' -> ^(THROW[$lc,"THROW"] expr)
	;
trycatch_stmt
	: lc='try' '{' chunk* '}' catch_block+ -> ^(TRY[$lc,"TRY"] ^(CODE[$lc,"CODE"] chunk*) catch_block+)
	;
catch_block
	: lc='catch' '(' expr ID ')' '{' chunk* '}' -> ^(CATCH[$lc,"CATCH"] expr ID ^(CODE[$lc,"CODE"] chunk*))
	;
ifelse_stmt
	: lc='if' '(' expr ')' '{' chunk* '}' else_block? -> ^(IF[$lc,"IF"] expr ^(CODE[$lc,"CODE"] chunk*) else_block?)
	;
else_block
	: lc='else' '{' chunk* '}' -> ^(ELSE[$lc,"ELSE"] ^(CODE[$lc,"CODE"] chunk*))
	;
while_stmt
	: lc='while' '(' expr ')' '{' chunk* '}' -> ^(WHILE[$lc,"WHILE"] expr ^(CODE[$lc,"CODE"] chunk*))
	;
for_stmt
	: lc='for' '(' init=assign_expr_list? ';' expr? ';' step=assign_expr_list? ')' '{' chunk* '}' -> ^(FOR[$lc,"FOR"] ^(INIT[$lc,"INIT"] $init) ^(COND[$lc,"COND"] expr) ^(STEP[$lc,"STEP"] $step) ^(CODE[$lc,"CODE"] chunk*))
	;
assign_expr_list
	: assign_expr (',' assign_expr)* -> assign_expr+
	;
foreach_stmt
	: lc='foreach' '(' ID 'in' expr ')' '{' chunk* '}' -> ^(FOREACH[$lc,"FOREACH"] expr ID ^(CODE[$lc,"CODE"] chunk*))
	;
break_stmt
	: lc='break' ';' -> BREAK[$lc,"BREAK"]
	;
continue_stmt
	: lc='continue' ';' -> CONTINUE[$lc,"CONTINUE"]
	;
block_stmt
	: lc='{' chunk* '}' -> ^(BLOCK[$lc,"BLOCK"] ^(CODE[$lc,"CODE"] chunk*))
	;
		
//Expressions

expr
	: logic_or_expr
	;
	
logic_or_expr
	: logic_and_expr (LOGIC_OR^ logic_and_expr)*
	;
LOGIC_OR : '||';
	
logic_and_expr
	: bit_or_expr (LOGIC_AND^ bit_or_expr)*
	;
LOGIC_AND : '&&';
	
bit_or_expr
	: bit_xor_expr (BIT_OR^ bit_xor_expr)*
	;
BIT_OR : '|';

bit_xor_expr
	: bit_and_expr (BIT_XOR^ bit_and_expr)*
	;
BIT_XOR : '^';

bit_and_expr
	: eq_neq_expr (BIT_AND^ eq_neq_expr)*
	;
BIT_AND : '&';

eq_neq_expr
	: gt_gte_lt_lte_expr (equality_op^ gt_gte_lt_lte_expr)*
	;
	
equality_op
	: (EQ|NEQ)
	;
EQ : '==';
NEQ : '!=';

gt_gte_lt_lte_expr
	: shift_expr (comparison_op^ shift_expr)*
	;
	
comparison_op
	: (BIGGER|BIGGER_EQ|LOWER|LOWER_EQ)
	;
BIGGER : '>';
BIGGER_EQ : '>=';
LOWER : '<';
LOWER_EQ : '<=';
	
shift_expr
	: add_sub_expr (shift_op^ add_sub_expr)*
	;
	
shift_op
	: (LSHIFT|RSHIFT)
	;
LSHIFT : '<<';
RSHIFT : '>>';

add_sub_expr
	: mul_div_mod_expr (add_sub_op^ mul_div_mod_expr)*
	;

add_sub_op
	: (ADD|SUB)
	;
ADD : '+';
SUB : '-';
	
mul_div_mod_expr
	: unary_expr (mul_div_mod_op^ unary_expr)*
	;
	
mul_div_mod_op
	: (MUL|DIV|MOD)
	;
MUL : '*';
DIV : '/';
MOD : '%';
	
unary_expr
	: add_sub_op^ unary_expr
	| not_ops^ unary_expr
	| atom
	;
	
not_ops
	: (LOGIC_NOT|BIT_NOT)
	;

LOGIC_NOT : '!';
BIT_NOT : '~';

atom
	: '('! expr^ ')'!
	| anonclass
	| call
	| ID
	| literal
	;

priority_atom
	: '['! expr^ ']'!
	;

call
	: lc=ID message+ -> ^(CALL[$lc,"CALL"] ID message+)
	| priority_atom message+ -> ^(CALL priority_atom message+)
	;

message
	: lc='.' ID '(' (expr (',' expr)*)? ')' -> ^(MESSAGE[$lc,"MESSAGE"] ID expr*)
	;
	
anonclass
	: lc='class' (':' expr)? '{' member* '}' -> ^(CLASSDEF[$lc,"CLASSDEF"] ^(INHERITING expr?) ^(MEMBERS member*)) 
	;
	

literal
	: NONE
	| boolean
	| data
	| string
	| octet
	| integer
	| real
	;

octet
	: OCTET
	;
	
integer
	: INT
	;

real
	: FLOAT
	;

data
	: DATA
	;
	
string
	: STRING
	;

docstring
	: TRISTRING
	;

boolean
	: TRUELIT
	| FALSELIT
	;
    
NONE
	: 'none'
	;
	
TRUELIT
	: 'true'
	;
	
FALSELIT
	: 'false'
	;
    
ID  :	('a'..'z'|'A'..'Z'|'_') ('a'..'z'|'A'..'Z'|'0'..'9'|'_')*
    ;

INT :	'0'..'9'+
    ;
    
OCTET :	'0x' ('0'..'9'|'a'..'f'|'A'..'F') ('0'..'9'|'a'..'f'|'A'..'F')?
    ;

FLOAT
    :   ('0'..'9')+ '.' ('0'..'9')* EXPONENT?
    |   '.' ('0'..'9')+ EXPONENT?
    |   ('0'..'9')+ EXPONENT
    ;

COMMENT
    :   '//' ~('\n'|'\r')* '\r'? '\n' {$channel=HIDDEN;}
    |   '/*' ( options {greedy=false;} : . )* '*/' {$channel=HIDDEN;}
    ;

DATA
    :  '\'' ( ESCDATA_SEQ | ~('\\'|'\'') )* '\''
    ;

STRING
    :  '"' ( ESC_SEQ | ~('\\'|'"') )* '"'
    ;
    
TRISTRING
    :  '"""' ( ~('\\'|'"') )* '"""'
    ;
    
WS  :   ( ' '
        | '\t'
        | '\r'
        | '\n'
        ) {$channel=HIDDEN;}
    ;

fragment
EXPONENT : ('e'|'E') ('+'|'-')? ('0'..'9')+ ;

fragment
HEX_DIGIT : ('0'..'9'|'a'..'f'|'A'..'F') ;

fragment
ESC_SEQ
    :   '\\' ('b'|'t'|'n'|'f'|'r'|'\"'|'\\')
    |   UNICODE_ESC
    ;
    
fragment
ESCDATA_SEQ
    :   '\\' ('b'|'t'|'n'|'f'|'r'|'\''|'\\')
    |	HEX_ESC
    ;

fragment
HEX_ESC
    :   '\\' 'x'  HEX_DIGIT HEX_DIGIT
    ;

fragment
UNICODE_ESC
    :   '\\' 'u' HEX_DIGIT HEX_DIGIT HEX_DIGIT HEX_DIGIT
    ;

