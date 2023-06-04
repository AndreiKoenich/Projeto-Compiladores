/* PROJETO DE COMPILADORES - ETAPA 2  */

/* Andrei Pochmann Koenich 	 - Matrícula 00308680 */
/* Izaias Saturnino de Lima Neto - Matrícula 00326872 */


%{
#include <stdio.h>
#include <stdlib.h>
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

%start lista_comandos;

%%

comando_simples:		declaracao | atribuicao | chamada_funcao | retorno | expressao | condicional | iterativo | bloco_comandos;

bloco_comandos:			'{' lista_comandos '}';
lista_comandos:			comando_simples ';' lista_comandos | ;

declaracao: 			tipo TK_IDENTIFICADOR lista_identificadores | tipo TK_IDENTIFICADOR TK_OC_LE literal lista_identificadores;	
tipo: 				TK_PR_INT | TK_PR_FLOAT | TK_PR_BOOL;
lista_identificadores:		',' TK_IDENTIFICADOR lista_identificadores | ',' TK_IDENTIFICADOR TK_OC_LE literal lista_identificadores | ;

atribuicao: 			TK_IDENTIFICADOR '=' expressao;

chamada_funcao: 		TK_IDENTIFICADOR '(' expressao lista_expressoes ')';
lista_expressoes:		',' expressao lista_expressoes | ;

retorno: 			TK_PR_RETURN expressao; 

condicional:			TK_PR_IF '(' expressao ')' bloco_comandos | TK_PR_IF '(' expressao ')' bloco_comandos TK_PR_ELSE bloco_comandos;

iterativo:			TK_PR_WHILE '(' expressao ')' bloco_comandos

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
	printf("%s\n",s);
}

