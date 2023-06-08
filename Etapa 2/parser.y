/* PROJETO DE COMPILADORES - ETAPA 2  */

/* Andrei Pochmann Koenich 	 - Matrícula 00308680 */
/* Izaias Saturnino de Lima Neto - Matrícula 00326872 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int yylex(void);
void yyerror (char const *s);
%}

%token TK_PR_INT
%token TK_PR_FLOAT
%token TK_PR_BOOL
%token TK_PR_IF
%token TK_PR_ELSE
%token TK_PR_WHILE
%token TK_PR_RETURN
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token TK_OC_AND
%token TK_OC_OR
%token TK_OC_MAP
%token TK_IDENTIFICADOR
%token TK_LIT_INT
%token TK_LIT_FLOAT
%token TK_LIT_FALSE
%token TK_LIT_TRUE
%token TK_ERRO

%start programa;
%define parse.error detailed;

%%

programa: 			lista | /* Vazio */;
lista: 				lista elemento | elemento;
elemento: 			definicao_funcao | declaracao_global;

definicao_funcao: 		TK_IDENTIFICADOR '(' lista_parametros ')' TK_OC_MAP tipo bloco_comandos | TK_IDENTIFICADOR '(' ')' TK_OC_MAP tipo bloco_comandos;
lista_parametros:		tipo TK_IDENTIFICADOR | lista_parametros ',' tipo TK_IDENTIFICADOR;

declaracao_global: 		tipo TK_IDENTIFICADOR lista_identificadores ';';

lista_comandos:			comando_simples  lista_comandos | /* Vazio */;

comando_simples:		declaracao_local ';' | definicao_funcao | chamada_funcao ';' | atribuicao ';' | retorno ';' 
				| condicional_if ';' condicional_else | iterativo ';' | bloco_comandos ';';
				
declaracao_local: 		tipo TK_IDENTIFICADOR lista_identificadores | tipo TK_IDENTIFICADOR TK_OC_LE literal lista_identificadores;	
tipo: 				TK_PR_INT | TK_PR_FLOAT | TK_PR_BOOL;
lista_identificadores:		',' TK_IDENTIFICADOR lista_identificadores | ',' TK_IDENTIFICADOR TK_OC_LE literal lista_identificadores | /* Vazio */;

bloco_comandos:			'{' lista_comandos '}';

atribuicao: 			TK_IDENTIFICADOR '=' expressao;

chamada_funcao: 		TK_IDENTIFICADOR '(' lista_expressoes ')' | TK_IDENTIFICADOR '(' ')';
lista_expressoes: 		expressao | lista_expressoes ',' expressao;

retorno: 			TK_PR_RETURN expressao; 

condicional_if: 		TK_PR_IF '(' expressao ')' bloco_comandos;
condicional_else:		TK_PR_ELSE bloco_comandos ';' | /* Vazio */;

iterativo:			TK_PR_WHILE '(' expressao ')' bloco_comandos;

expressao: 			expressao2 | expressao TK_OC_OR expressao2;
expressao2: 			expressao3 | expressao2 TK_OC_AND expressao3;
expressao3: 			expressao4 | expressao3 TK_OC_EQ expressao4 | expressao3 TK_OC_NE expressao4;
expressao4: 			expressao5 | expressao4 '<' expressao5 | expressao4 '>' expressao5 | expressao4 TK_OC_LE expressao5 | expressao4 TK_OC_GE expressao5;
expressao5: 			expressao6 | expressao5 '+' expressao6 | expressao5 '-' expressao6;
expressao6: 			expressao7 | expressao6 '*' expressao7 | expressao6 '/' expressao7 | expressao6 '%' expressao7;
expressao7: 			'(' expressao ')' | '!' expressao7 | '-' expressao7 | TK_IDENTIFICADOR | literal;

literal:			TK_LIT_INT | TK_LIT_FLOAT | TK_LIT_FALSE | TK_LIT_TRUE;

%%

void yyerror(char const *s)
{
	extern int yylineno;
	printf("ERRO - LINHA %d - %s\n", yylineno, s);	
}
