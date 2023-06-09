/* PROJETO DE COMPILADORES - ETAPA 3  */

/* Andrei Pochmann Koenich 	 - Matrícula 00308680 */
/* Izaias Saturnino de Lima Neto - Matrícula 00326872 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"
int yylex(void);
void yyerror (char const *s);
extern void *arvore;
%}

%union
{
	ValorLexico *valor_lexico;
	Nodo *nodo;
}

%token<valor_lexico> TK_PR_INT
%token<valor_lexico> TK_PR_FLOAT
%token<valor_lexico> TK_PR_BOOL
%token<valor_lexico> TK_PR_IF
%token<valor_lexico> TK_PR_ELSE
%token<valor_lexico> TK_PR_WHILE
%token<valor_lexico> TK_PR_RETURN
%token<valor_lexico> TK_OC_LE
%token<valor_lexico> TK_OC_GE
%token<valor_lexico> TK_OC_EQ
%token<valor_lexico> TK_OC_NE
%token<valor_lexico> TK_OC_AND
%token<valor_lexico> TK_OC_OR
%token<valor_lexico> TK_OC_MAP

%token<valor_lexico> '='
%token<valor_lexico> '<'
%token<valor_lexico> '>'
%token<valor_lexico> '+'
%token<valor_lexico> '*'
%token<valor_lexico> '/'
%token<valor_lexico> '%'
%token<valor_lexico> '!'
%token<valor_lexico> '-'

%type<nodo> programa
%type<nodo> lista
%type<nodo> elemento
%type<nodo> definicao_funcao
%type<nodo> lista_parametros
%type<nodo> tupla_tipo_parametro
//%type<nodo> declaracao_global
//%type<nodo> lista_identificadores_globais
%type<nodo> lista_comandos
%type<nodo> comando_simples
%type<nodo> declaracao_local
%type<valor_lexico> tipo
%type<nodo> lista_identificadores
%type<nodo> identificador_local
%type<nodo> bloco_comandos
%type<nodo> atribuicao
%type<nodo> chamada_funcao
%type<nodo> lista_expressoes
%type<nodo> retorno
%type<nodo> clausula_if_com_else_opcional
%type<nodo> iterativo
%type<nodo> expressao
%type<nodo> expressao2
%type<nodo> expressao3
%type<nodo> expressao4
%type<nodo> expressao5
%type<nodo> expressao6
%type<nodo> expressao7
%token<valor_lexico> TK_IDENTIFICADOR
%token<valor_lexico> TK_LIT_INT
%token<valor_lexico> TK_LIT_FLOAT
%token<valor_lexico> TK_LIT_FALSE
%token<valor_lexico> TK_LIT_TRUE
%type<valor_lexico> literal
%token TK_ERRO

%start programa;
%define parse.error detailed;

%%

programa: lista {
	$$ = $1;
	arvore = $$;
};
programa: /* Vazio */ { $$ = NULL; };
lista: elemento lista
{
	if($1 != NULL && $2 != NULL){
		$$ = $1;
		adicionaNodo($$, $2);
	}
	else if($1 != NULL){
		$$ = $1;
	}
	else if($2 != NULL){
		$$ = $2;
	}
	else{
		$$ = NULL;
	}
};
lista: elemento { $$ = $1; };

elemento: definicao_funcao 	{ $$ = $1; };
elemento: declaracao_global { $$ = NULL; };

definicao_funcao: TK_IDENTIFICADOR '(' lista_parametros ')' TK_OC_MAP tipo bloco_comandos
{
	$$ = criaNodo($1);
	//adicionaNodo($$, $3);
	/*
	Nodo* novoNodo = criaNodo($6); // tipo
	adicionaNodo($$, novoNodo);
	*/
	if($7 != NULL){
		adicionaNodo($$, $7);
	}
};
definicao_funcao: TK_IDENTIFICADOR '(' ')' TK_OC_MAP tipo bloco_comandos
{
	$$ = criaNodo($1);
	/*
	Nodo* novoNodo = criaNodo($5); // tipo
	adicionaNodo($$, novoNodo);
	*/
	if($6 != NULL){
		adicionaNodo($$, $6);
	}
};

lista_parametros: tupla_tipo_parametro { $$ = $1; };
lista_parametros: tupla_tipo_parametro ',' lista_parametros { $$ = $1; adicionaNodo($$, $3); };

tupla_tipo_parametro: tipo TK_IDENTIFICADOR { $$ = criaNodo($2); }; //verificar se o tipo vai para a árvore

declaracao_global: tipo lista_identificadores_globais ';';

lista_identificadores_globais: TK_IDENTIFICADOR;
lista_identificadores_globais: TK_IDENTIFICADOR ',' lista_identificadores_globais;

lista_comandos: comando_simples ';' lista_comandos
{
	if($1 != NULL && $3 != NULL){
		$$ = $1;
		adicionaNodo($$, $3);
	}
	else if($1 != NULL){
		$$ = $1;
	}
	else if($3 != NULL){
		$$ = $3;
	}
	else{
		$$ = NULL;
	}
};
lista_comandos:	comando_simples ';' { $$ = $1; };
lista_comandos: bloco_comandos ';' lista_comandos
{
	if($1 != NULL && $3 != NULL){
		$$ = $1;
		concatenate_list($$, $3);
	}
	else if($1 != NULL){
		$$ = $1;
	}
	else if($3 != NULL){
		$$ = $3;
	}
	else{
		$$ = NULL;
	}
};
lista_comandos:	bloco_comandos ';' { $$ = $1; };

lista_comandos: declaracao_local ';' lista_comandos
{
	if($1 != NULL && $3 != NULL){
		$$ = $1;
		concatenate_list($$, $3);
	}
	else if($1 != NULL){
		$$ = $1;
	}
	else if($3 != NULL){
		$$ = $3;
	}
	else{
		$$ = NULL;
	}
};

lista_comandos:	declaracao_local ';' { $$ = $1; };

comando_simples: chamada_funcao 				{ $$ = $1; };
comando_simples: atribuicao 					{ $$ = $1; };
comando_simples: retorno 						{ $$ = $1; };
comando_simples: clausula_if_com_else_opcional 	{ $$ = $1; };
comando_simples: iterativo 						{ $$ = $1; };

declaracao_local: tipo lista_identificadores { $$ = $2; }; 

tipo: TK_PR_INT 	{ $$ = $1; };
tipo: TK_PR_FLOAT 	{ $$ = $1; };
tipo: TK_PR_BOOL 	{ $$ = $1; };

lista_identificadores: identificador_local { $$ = $1; };
lista_identificadores: identificador_local ',' lista_identificadores 
{
	if($1 != NULL && $3 != NULL){
		$$ = $1;
		adicionaNodo($$, $3);
	}
	else if($1 != NULL){
		$$ = $1;
	}
	else if($3 != NULL){
		$$ = $3;
	}
	else{
		$$ = NULL;
	}
};

identificador_local: TK_IDENTIFICADOR { $$ = NULL; };
identificador_local: TK_IDENTIFICADOR TK_OC_LE literal
{
	$$ = criaNodo($2);
	Nodo* novoNodo = criaNodo($1);
	adicionaNodo($$, novoNodo);
	Nodo* novoNodo2 = criaNodo($3);
	adicionaNodo($$, novoNodo2);
};

bloco_comandos:	'{' lista_comandos '}' 	{ $$ = $2; };
bloco_comandos:	'{' '}' 				{ $$ = NULL; };

atribuicao: TK_IDENTIFICADOR '=' expressao
{
	$$ = criaNodo($2);
	Nodo* novoNodo = criaNodo($1);
	adicionaNodo($$, novoNodo);
	adicionaNodo($$, $3);
};

chamada_funcao: TK_IDENTIFICADOR '(' lista_expressoes ')'
{
	$1->tipo_token = 7;
	$$ = criaNodo($1);
	$$->info->valor_token = concat_call($$->info->valor_token);
	adicionaNodo($$, $3);
};
chamada_funcao: TK_IDENTIFICADOR '(' ')'
{
	$1->tipo_token = 7;
	$$ = criaNodo($1);
	$$->info->valor_token = concat_call($$->info->valor_token);
};

lista_expressoes: expressao 						{ $$ = $1; };
lista_expressoes: expressao ',' lista_expressoes	{ $$ = $1; adicionaNodo($$, $3); };

retorno: TK_PR_RETURN expressao { $$ = criaNodo($1); adicionaNodo($$, $2); };

clausula_if_com_else_opcional: TK_PR_IF '(' expressao ')' bloco_comandos
{
	$$ = criaNodo($1);
	adicionaNodo($$, $3);
	if($5 != NULL){
		adicionaNodo($$, $5);
	}
};
clausula_if_com_else_opcional: TK_PR_IF '(' expressao ')' bloco_comandos TK_PR_ELSE bloco_comandos
{
	$$ = criaNodo($1);
	adicionaNodo($$, $3);
	if($5 != NULL){
		adicionaNodo($$, $5);
	}
	if($7 != NULL){
		adicionaNodo($$, $7);
	}
};

iterativo: TK_PR_WHILE '(' expressao ')' bloco_comandos
{
	$$ = criaNodo($1);
	adicionaNodo($$, $3);
	if($5 != NULL){
		adicionaNodo($$, $5);
	}
};

expressao:  expressao2 						{ $$ = $1; };
expressao:  expressao  TK_OC_OR expressao2 	{ $$ = criaNodo($2); adicionaNodo($$, $1); adicionaNodo($$, $3); };

expressao2: expressao3 						{ $$ = $1; };
expressao2: expressao2 TK_OC_AND expressao3 { $$ = criaNodo($2); adicionaNodo($$, $1); adicionaNodo($$, $3); };

expressao3: expressao4 						{ $$ = $1; };
expressao3: expressao3 TK_OC_EQ expressao4 	{ $$ = criaNodo($2); adicionaNodo($$, $1); adicionaNodo($$, $3); };
expressao3: expressao3 TK_OC_NE expressao4 	{ $$ = criaNodo($2); adicionaNodo($$, $1); adicionaNodo($$, $3); };

expressao4: expressao5 						{ $$ = $1; };
expressao4: expressao4 '<' expressao5 		{ $$ = criaNodo($2); adicionaNodo($$, $1); adicionaNodo($$, $3); };
expressao4: expressao4 '>' expressao5 		{ $$ = criaNodo($2); adicionaNodo($$, $1); adicionaNodo($$, $3); };
expressao4: expressao4 TK_OC_LE expressao5 	{ $$ = criaNodo($2); adicionaNodo($$, $1); adicionaNodo($$, $3); };
expressao4: expressao4 TK_OC_GE expressao5 	{ $$ = criaNodo($2); adicionaNodo($$, $1); adicionaNodo($$, $3); };

expressao5: expressao6 						{ $$ = $1; };
expressao5: expressao5 '+' expressao6 		{ $$ = criaNodo($2); adicionaNodo($$, $1); adicionaNodo($$, $3); };
expressao5: expressao5 '-' expressao6 		{ $$ = criaNodo($2); adicionaNodo($$, $1); adicionaNodo($$, $3); };

expressao6: expressao7 						{ $$ = $1; };
expressao6: expressao6 '*' expressao7 		{ $$ = criaNodo($2); adicionaNodo($$, $1); adicionaNodo($$, $3); };
expressao6: expressao6 '/' expressao7 		{ $$ = criaNodo($2); adicionaNodo($$, $1); adicionaNodo($$, $3); };
expressao6: expressao6 '%' expressao7 		{ $$ = criaNodo($2); adicionaNodo($$, $1); adicionaNodo($$, $3); };

expressao7: '(' expressao ')'				{ $$ = $2; };
expressao7: '!' expressao7					{ $$ = criaNodo($1); adicionaNodo($$, $2); };
expressao7: '-' expressao7					{ $$ = criaNodo($1); adicionaNodo($$, $2); };
expressao7: TK_IDENTIFICADOR				{ $$ = criaNodo($1); };
expressao7: chamada_funcao				{ $$ = $1; };
expressao7: literal 						{ $$ = criaNodo($1); };

literal: TK_LIT_INT  	{ $$ = $1; };
literal: TK_LIT_FLOAT 	{ $$ = $1; };
literal: TK_LIT_FALSE 	{ $$ = $1; };
literal: TK_LIT_TRUE  	{ $$ = $1; };

%%

void yyerror(char const *s)
{
	extern int yylineno;
	printf("ERRO - LINHA %d - %s\n", yylineno, s);	
}
