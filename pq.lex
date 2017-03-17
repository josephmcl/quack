%{
#include <cstdio>
#include <iostream>
#include <memory>
#include "ast.hpp"
using namespace std;
#define YY_DECL extern "C" int yylex()
#include "quack.tab.h" 
int mylineno = 0;
%}

%option yylineno

%x BLOCK_COMMENT
%%

\n					{++mylineno;}
[ \t\r\f] 				/* do nothing */

"/*"            			{BEGIN(BLOCK_COMMENT);}
<BLOCK_COMMENT>"*/" 			{BEGIN(INITIAL);}
<BLOCK_COMMENT>\n   			{}
<BLOCK_COMMENT>.    			{}


"\"\"\""([^']|'[^']|''[^'])*"\"\"\""	{yylval.sval = strdup(yytext); return STRING_LIT;}

[/]{2}+.*				{}
\"(\\[0bftnr\\]|[^"\n\\])*\" 		{yylval.sval = strdup(yytext); return STRING_LIT;}
\"(\\.|[^"]|\n)*\"			{return 21;}


class					{yylval.sval = strdup(yytext); return CLASS;}
def					{yylval.sval = strdup(yytext); return DEF;}
extends					{yylval.sval = strdup(yytext); return EXTENDS;}
if					{yylval.sval = strdup(yytext); yylval.sval = strdup(yytext); return IF;}
elif					{yylval.sval = strdup(yytext); return ELIF;}
else					{yylval.sval = strdup(yytext); return ELSE;}
while					{yylval.sval = strdup(yytext); return WHILE;}
return					{yylval.sval = strdup(yytext); return RETURN;}

String					{yylval.sval = strdup(yytext); return IDENT;}	
Integer					{yylval.sval = strdup(yytext); return IDENT;}
Obj					{yylval.sval = strdup(yytext); return IDENT;}
Boolean					{yylval.sval = strdup(yytext); return IDENT;}
true					{yylval.sval = strdup(yytext); return IDENT;}
false					{yylval.sval = strdup(yytext); return IDENT;}
and					{yylval.sval = strdup(yytext); return AND;}
or					{yylval.sval = strdup(yytext); return OR;}
not					{yylval.sval = strdup(yytext); return NOT;}
Nothing					{yylval.sval = strdup(yytext); return IDENT;}
none					{yylval.sval = strdup(yytext); return IDENT;}

[_a-zA-Z][_a-zA-Z0-9]* 			{yylval.sval = strdup(yytext); return IDENT;}
[1-9][0-9]*				{yylval.sval = strdup(yytext); return INT_LIT;}
0					{yylval.sval = strdup(yytext); return INT_LIT;}

"+"					{yylval.sval = strdup(yytext); return '+';}
"-"					{yylval.sval = strdup(yytext); return '-';}
"*"					{yylval.sval = strdup(yytext); return '*';}
"/"					{yylval.sval = strdup(yytext); return '/';}

"="{2}					{yylval.sval = strdup(yytext); return EQUALS;}
"<="					{yylval.sval = strdup(yytext); return ATMOST;}
"<"					{yylval.sval = strdup(yytext); return '<';}
">="					{yylval.sval = strdup(yytext); return ATLEAST;}
">"					{yylval.sval = strdup(yytext); return '>';}

"{"					{yylval.sval = strdup(yytext); return '{';}
"}"					{yylval.sval = strdup(yytext); return '}';}
"("					{yylval.sval = strdup(yytext); return '(';}
")"					{yylval.sval = strdup(yytext); return ')';}
","					{yylval.sval = strdup(yytext); return ',';}
"."					{yylval.sval = strdup(yytext); return '.';}
";"					{yylval.sval = strdup(yytext); return ';';}
":"					{yylval.sval = strdup(yytext); return ':';}
"="					{yylval.sval = strdup(yytext); return '=';}

<<EOF>>					{return 0;}

.					{return 20;}


%%



