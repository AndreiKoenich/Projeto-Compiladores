/* PROJETO DE COMPILADORES - ETAPA 1  */

/* Andrei Pochmann Koenich - Matrícula 00308680 */
/* Izaias Saturnino de Lima Neto - Matrícula 00326872 */

%option yylineno
%option nounput
%option noinput
%option noyywrap

%{
#include "tokens.h"
%}

%%

[ \t\r\n\v]
"//"((.|\t|\r|\v)*)\n?
"/*"([^*]|"*"+[^*/])*"*"+"/"

"int" {return TK_PR_INT;}
"float" {return TK_PR_FLOAT;}
"bool" {return TK_PR_BOOL;}
"if" {return TK_PR_IF;}
"else" {return TK_PR_ELSE;}
"while" {return TK_PR_WHILE;}
"return" {return TK_PR_RETURN;}

[\-\!\*\%\/\+\<\>\{\}\(\)\=\,\;] {return yytext[0];}

"<=" {return TK_OC_LE;}
">=" {return TK_OC_GE;}
"==" {return TK_OC_EQ;}
"!=" {return TK_OC_NE;}
"&" {return TK_OC_AND;}
"|" {return TK_OC_OR;}
"->" {return TK_OC_MAP;}

"false" {return TK_LIT_FALSE;}
"true" {return TK_LIT_TRUE;}

[a-z_]+ {return TK_IDENTIFICADOR;}

[0-9]+ {return TK_LIT_INT;}
[0-9]*"."[0-9]+ {return TK_LIT_FLOAT;}

. {return TK_ERRO;}

%%

int get_line_number()
{
	return yylineno;
}
