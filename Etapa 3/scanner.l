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
	if(yylval.valor_lexico == NULL){
		yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	}
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = OTHER_TOKEN;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_PR_INT;}
	
"float" {
	if(yylval.valor_lexico == NULL){
		yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	}
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = OTHER_TOKEN;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_PR_FLOAT;}

"bool" {
	if(yylval.valor_lexico == NULL){
		yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	}
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = OTHER_TOKEN;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_PR_BOOL;}

"if" {
	if(yylval.valor_lexico == NULL){
		yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	}
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = OTHER_TOKEN;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_PR_IF;}

"else" {
	if(yylval.valor_lexico == NULL){
		yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	}
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = OTHER_TOKEN;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_PR_ELSE;}

"while" {
	if(yylval.valor_lexico == NULL){
		yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	}
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = OTHER_TOKEN;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_PR_WHILE;}
	
"return" {
	if(yylval.valor_lexico == NULL){
		yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	}
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = OTHER_TOKEN;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_PR_RETURN;}

[\-\!\*\%\/\+\<\>\{\}\(\)\=\,\;] 	{
	if(yylval.valor_lexico == NULL){
		yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	}
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = OTHER_TOKEN;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return yytext[0];}

"<=" 	{
	if(yylval.valor_lexico == NULL){
		yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	}
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = OTHER_TOKEN;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_OC_LE;}

">=" 	{
	if(yylval.valor_lexico == NULL){
		yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	}
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = OTHER_TOKEN;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_OC_GE;}
	
"==" 	{
	if(yylval.valor_lexico == NULL){
		yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	}
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = OTHER_TOKEN;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_OC_EQ;}

"!=" 	{
	if(yylval.valor_lexico == NULL){
		yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	}
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = OTHER_TOKEN;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_OC_NE;}

"&" 	{
	if(yylval.valor_lexico == NULL){
		yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	}
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = OTHER_TOKEN;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_OC_AND;}
		
"|" 	{
	if(yylval.valor_lexico == NULL){
		yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	}
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = OTHER_TOKEN;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_OC_OR;}

"->" 	{
	if(yylval.valor_lexico == NULL){
		yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	}
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = OTHER_TOKEN;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_OC_MAP;}

"false" {
	if(yylval.valor_lexico == NULL){
		yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	}
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = LITERAL;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_LIT_FALSE;}
	
"true" {
	if(yylval.valor_lexico == NULL){
		yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	}
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = LITERAL;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_LIT_TRUE;}
	 
[a-z_]+ {
	if(yylval.valor_lexico == NULL){
		yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	}
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = IDENTIFIER;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_IDENTIFICADOR;}
	 
[0-9]+  {
	if(yylval.valor_lexico == NULL){
		yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	}
	yylval.valor_lexico->linha_token = get_line_number();
	yylval.valor_lexico->tipo_token = LITERAL;
	yylval.valor_lexico->valor_token = strdup(yytext);
	return TK_LIT_INT;}	 
	 
[0-9]*"."[0-9]+   {
	if(yylval.valor_lexico == NULL){
		yylval.valor_lexico = (ValorLexico*) malloc(sizeof(ValorLexico));
	}
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