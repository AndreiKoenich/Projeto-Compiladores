/* PROJETO DE COMPILADORES - ETAPA 3  */

/* Andrei Pochmann Koenich 	 - Matrícula 00308680 */
/* Izaias Saturnino de Lima Neto - Matrícula 00326872 */

%option yylineno
%option nounput
%option noinput
%option noyywrap

%{
#include "arvore.h"
#include "parser.tab.h"
#define LITERAL 0
#define IDENTIFIER 1
#define OTHER_TOKEN 2
int get_line_number();
%}

%%

[ \t\r\n\v]
"//"((.|\t|\r|\v)*)\n?
"/*"([^*]|"*"+[^*/])*"*"+"/"

"int" {
	yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = OTHER_TOKEN;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_PR_INT;}
	
"float" {
	yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = OTHER_TOKEN;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_PR_FLOAT;}

"bool" {
	yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = OTHER_TOKEN;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_PR_BOOL;}

"if" {
	yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = OTHER_TOKEN;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_PR_IF;}

"else" {
	yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = OTHER_TOKEN;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_PR_ELSE;}

"while" {
	yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = OTHER_TOKEN;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_PR_WHILE;}
	
"return" {
	yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = OTHER_TOKEN;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_PR_RETURN;}

[\-\!\*\%\/\+\<\>\{\}\(\)\=\,\;] 	{
	yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = OTHER_TOKEN;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return yytext[0];}

"<=" 	{
	yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = OTHER_TOKEN;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_OC_LE;}

">=" 	{
	yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = OTHER_TOKEN;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_OC_GE;}
	
"==" 	{
	yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = OTHER_TOKEN;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_OC_EQ;}

"!=" 	{
	yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = OTHER_TOKEN;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_OC_NE;}

"&" 	{
	yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = OTHER_TOKEN;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_OC_AND;}
		
"|" 	{
	yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = OTHER_TOKEN;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_OC_OR;}

"->" 	{
	yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = OTHER_TOKEN;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_OC_MAP;}

"false" {
	yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = LITERAL;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_LIT_FALSE;}
	
"true" {
	yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = LITERAL;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_LIT_TRUE;}
	 
[a-z_]+ {
	yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = IDENTIFIER;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_IDENTIFICADOR;}
	 
[0-9]+  {
	yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = LITERAL;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_LIT_INT;}	 
	 
[0-9]*"."[0-9]+   {
	yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = LITERAL;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_LIT_FLOAT;}	 		 

. {return TK_ERRO;}

%%

int get_line_number()
{
	return yylineno;
}