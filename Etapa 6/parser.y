/* PROJETO DE COMPILADORES - ETAPA 6  */

/* Andrei Pochmann Koenich 	 - Matrícula 00308680 */
/* Izaias Saturnino de Lima Neto - Matrícula 00326872 */

/* A geração de código ILOC está sendo feita EM UMA ÚNICA PASSAGEM.
   A impressão das instruções na sintaxe da linguagem intermediária
   é feita no momento em que as instruções são criadas, especificamente
   nas linhas 481, 493, 505 e 517 do arquivo funcoes.c. Os códigos
   gerados em cada nó estão sendo concatenados por meio da função
   concatenaCodigo(), e sendo levados até a raiz da árvore AST. */	

%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"

int yylex(void);
void yyerror (char const *s);

extern void *arvore;

extern Lista_tabelas *lista_tabelas;

extern Tabela *tabela_global;
extern Tabela *tabela_escopo;

extern int tipo_atual;

extern int temporario_atual;
extern int deslocamento_atual;
extern int rotulo_atual;

extern char registrador_escopo[TAMANHO_NOME_OPERANDO];


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

%start inicio_programa;
%define parse.error detailed;

%%

inicio_programa: {
	pushTabela(&lista_tabelas, tabela_global); 
	printProgramStart(lista_tabelas->tabela_simbolos);
}
programa
{
	printProgramEnd();
}

programa: lista 
{
	$$ = $1;
	arvore = $$;
	
	/*printf("TABELA GLOBAL:\n\n");
	imprimeTabela(lista_tabelas->tabela_simbolos);
	printf("------------------\n");*/
	

	//printf("\nLISTA DE INSTRUCOES DA RAIZ:\n");
	popTabela(&lista_tabelas);
};

programa: /* Vazio */ { $$ = NULL; };

lista: elemento lista
{
	if($1 != NULL && $2 != NULL)
	{
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

elemento: definicao_funcao 		{
	$$ = $1;
	if(strcmp($$->info->valor_token, "main") == 0){
		defaultFunctionStackManagement(&($$->info->codigo));
		addFunctionMetaData(&($$->info->codigo), "main");
		imprimeInstrucoesNodo($$);
	}
};
elemento: declaracao_global 	{ $$ = NULL; };

definicao_funcao: TK_IDENTIFICADOR '(' push_tabela_escopo lista_parametros ')' TK_OC_MAP tipo
{
	tipo_atual = verificaTipo($7->valor_token);
	$1->tipo_token = tipo_atual;
	$1->natureza_token = FUNCTION;
	$1->tamanho_token = infereTamanho(tipo_atual);
		
	verificaERR_DECLARED(lista_tabelas,$1);
	insereEntradaTabela(&(lista_tabelas->tabela_simbolos), $1);
}
bloco_comandos
{
	$$ = criaNodo($1);
	if($9 != NULL)
	{
		adicionaNodo($$, $9);
		$$->info->codigo = $9->info->codigo;
	}
}

definicao_funcao: TK_IDENTIFICADOR '(' push_tabela_escopo ')' TK_OC_MAP tipo
{
	tipo_atual = verificaTipo($6->valor_token);
	$1->tipo_token = tipo_atual;
	$1->natureza_token = FUNCTION;
	$1->tamanho_token = infereTamanho(tipo_atual);
		
	verificaERR_DECLARED(lista_tabelas,$1);
	insereEntradaTabela(&(lista_tabelas->tabela_simbolos), $1);
}
bloco_comandos
{
	$$ = criaNodo($1);
	if($8 != NULL)
	{
		adicionaNodo($$, $8);
		$$->info->codigo = $8->info->codigo;
	}
}

lista_parametros: tupla_tipo_parametro { $$ = $1; };
lista_parametros: tupla_tipo_parametro ',' lista_parametros { $$ = $1; adicionaNodo($$, $3); };

tupla_tipo_parametro: tipo TK_IDENTIFICADOR 
{ 
	$$ = criaNodo($2); 
	tipo_atual = verificaTipo($1->valor_token);
	$2->tipo_token = tipo_atual;
	$2->natureza_token = VARIABLE;
	$2->tamanho_token = infereTamanho(tipo_atual);	
	verificaERR_DECLARED(lista_tabelas,$2);
	insereUltimaTabela(&lista_tabelas, $2);
};

declaracao_global: tipo { tipo_atual = verificaTipo($1->valor_token); } lista_identificadores_globais ';'

lista_identificadores_globais: TK_IDENTIFICADOR ',' lista_identificadores_globais 	
{ 
	$1->tipo_token = tipo_atual; 
	$1->tamanho_token = infereTamanho(tipo_atual); 
	verificaERR_DECLARED(lista_tabelas,$1); 
	insereUltimaTabela(&lista_tabelas, $1); 
};

lista_identificadores_globais: TK_IDENTIFICADOR						
{ 
	$1->tipo_token = tipo_atual; 
	$1->tamanho_token = infereTamanho(tipo_atual); 
	verificaERR_DECLARED(lista_tabelas,$1); 
	insereUltimaTabela(&lista_tabelas, $1);
};

lista_comandos: comando_simples ';' lista_comandos
{
	if($1 != NULL && $3 != NULL)
	{
		$$ = $1;
		adicionaNodo($$, $3);
		$$->info->codigo = concatenaCodigo($1->info->codigo, $3->info->codigo);
	}
	
	else if($1 != NULL)
		$$ = $1;
		
	else if($3 != NULL)
		$$ = $3;
		
	else
		$$ = NULL;	
};

lista_comandos:	comando_simples ';' { $$ = $1; };
lista_comandos: push_tabela_escopo  bloco_comandos ';' lista_comandos
{
	if($2 != NULL && $4 != NULL)
	{
		$$ = $2;
		concatenate_list($$, $4);
		$$->info->codigo = concatenaCodigo($2->info->codigo, $4->info->codigo);
	}
	
	else if($2 != NULL)
		$$ = $2;
	
	else if($4 != NULL)
		$$ = $4;

	else
		$$ = NULL;
};

lista_comandos:	push_tabela_escopo bloco_comandos ';' { $$ = $2; };

push_tabela_escopo: /* Vazio */ { pushTabela(&lista_tabelas, tabela_escopo); }

lista_comandos: declaracao_local ';' lista_comandos
{
	if($1 != NULL && $3 != NULL)
	{
		$$ = $1;
		concatenate_list($$, $3);
		$$->info->codigo = concatenaCodigo($1->info->codigo, $3->info->codigo);
	}
	
	else if($1 != NULL)
		$$ = $1;
		
	else if($3 != NULL)
		$$ = $3;
		
	else
		$$ = NULL;
};

lista_comandos:	declaracao_local ';' { $$ = $1; };

comando_simples: chamada_funcao 					{ $$ = $1; };
comando_simples: atribuicao 						{ $$ = $1; };
comando_simples: retorno 							{ $$ = $1; };
comando_simples: clausula_if_com_else_opcional 		{ $$ = $1; };
comando_simples: iterativo 							{ $$ = $1; };

declaracao_local: tipo_local lista_identificadores { $$ = $2; }; 

tipo_local: tipo { tipo_atual = verificaTipo($1->valor_token);} 

tipo: TK_PR_INT 	{ $$ = $1; };
tipo: TK_PR_FLOAT 	{ $$ = $1; };
tipo: TK_PR_BOOL 	{ $$ = $1; };

lista_identificadores: identificador_local { $$ = $1; };

lista_identificadores: identificador_local ',' lista_identificadores 
{
	if($1 != NULL && $3 != NULL)
	{
		$$ = $1;
		adicionaNodo($$, $3);
	}
	
	else if($1 != NULL)
		$$ = $1;
		
	else if($3 != NULL)
		$$ = $3;
		
	else
		$$ = NULL;
};

identificador_local: TK_IDENTIFICADOR
{ 
	$$ = NULL; 
	$1->tipo_token = tipo_atual;
	$1->tamanho_token = infereTamanho(tipo_atual);
	verificaERR_DECLARED(lista_tabelas, $1);
	insereUltimaTabela(&lista_tabelas, $1); 
};

identificador_local: TK_IDENTIFICADOR TK_OC_LE literal
{
	$$ = criaNodo($2);
	Nodo* novoNodo = criaNodo($1);
	adicionaNodo($$, novoNodo);
	Nodo* novoNodo2 = criaNodo($3);
	adicionaNodo($$, novoNodo2);
	
	$1->tipo_token = tipo_atual;
	$1->tamanho_token = infereTamanho(tipo_atual);
	$2->tipo_token = $3->tipo_token;
	
	verificaERR_DECLARED(lista_tabelas, $1);
	insereUltimaTabela(&lista_tabelas, $1);
	
	atualizaRegistradorEscopo(lista_tabelas, registrador_escopo, $1->valor_token);
	deslocamento_atual = achaDeslocamento(lista_tabelas,$1->valor_token);
	
	insereInstrucao(&($$->info->codigo), criaInstrucao_loadI($3->valor_token,$3->temporario));
	insereInstrucao(&($$->info->codigo), criaInstrucao_storeAI($3->temporario,registrador_escopo,deslocamento_atual));
};

bloco_comandos:	'{' lista_comandos '}' {
	/*imprimeUltimaTabela(lista_tabelas);*/
	$$ = $2;
	popTabela(&lista_tabelas);
};
bloco_comandos:	'{' '}' {
	/*imprimeUltimaTabela(lista_tabelas);*/
	$$ = NULL;
	popTabela(&lista_tabelas);
};

atribuicao: TK_IDENTIFICADOR '=' expressao
{
	$$ = criaNodo($2);
	Nodo* novoNodo = criaNodo($1);
	adicionaNodo($$, novoNodo);
	adicionaNodo($$, $3);
	
	$1->tipo_token = infereTipoExpressao($$); 
	$1->tamanho_token = infereTamanho($1->tipo_token);
	verificaERR_UNDECLARED_FUNCTION(lista_tabelas,$1);
	
	atualizaRegistradorEscopo(lista_tabelas, registrador_escopo, $1->valor_token);
	deslocamento_atual = achaDeslocamento(lista_tabelas,$1->valor_token);
	
	$$->info->codigo = $3->info->codigo;
	insereInstrucao(&($$->info->codigo), criaInstrucao_storeAI($3->info->temporario,registrador_escopo,deslocamento_atual));
};

chamada_funcao: TK_IDENTIFICADOR '(' lista_expressoes ')'
{
	$1->natureza_token = FUNCTION_CALL;
	$$ = criaNodo($1);
	$$->info->valor_token = concat_call($$->info->valor_token);
	adicionaNodo($$, $3);
	
	verificaERR_VARIABLE_UNDECLARED_chamadafuncao(lista_tabelas, obtemNomeFuncao($1->valor_token), $1->linha_token);
};

chamada_funcao: TK_IDENTIFICADOR '(' ')'
{
	$1->natureza_token = FUNCTION_CALL;
	$$ = criaNodo($1);
	$$->info->valor_token = concat_call($$->info->valor_token);
	
	verificaERR_VARIABLE_UNDECLARED_chamadafuncao(lista_tabelas, obtemNomeFuncao($1->valor_token), $1->linha_token);
};

lista_expressoes: expressao 				{ $$ = $1; };
lista_expressoes: expressao ',' lista_expressoes	{ $$ = $1; adicionaNodo($$, $3); };

retorno: TK_PR_RETURN expressao { $1->tipo_token = infereTipoExpressao($2); $$ = criaNodo($1); adicionaNodo($$, $2); };

impressaoRotulo: /* Vazio */ { /*imprimeRotulo(rotulo_atual);*/ };
impressaoRotuloElse: /* Vazio */ { /*imprimeRotulo(rotulo_atual+2);*/ };
impressao_cbrRotulo: /* Vazio */ { /*imprimeInstrucao_cbr(temporario_atual-1, rotulo_atual+1, rotulo_atual+2);*/ /*imprimeRotulo(rotulo_atual+1);*/ }

clausula_if_com_else_opcional: TK_PR_IF impressaoRotulo '(' expressao ')' push_tabela_escopo impressao_cbrRotulo bloco_comandos
{
	$1->tipo_token = infereTipoExpressao($4);
	$$ = criaNodo($1);
	adicionaNodo($$, $4);
	
	if($8 != NULL)
		adicionaNodo($$, $8);
		
  	insereInstrucao(&($$->info->codigo), criaRotulo(rotulo_atual));	/* Rotulo da expressao de teste. */
  	rotulo_atual++;
  	
  	$$->info->codigo = concatenaCodigo($$->info->codigo, $4->info->codigo);	/* Carrega o codigo da expressao de teste. */
  	insereInstrucao(&($$->info->codigo), criaInstrucao_cbr ($4->info->temporario, rotulo_atual, rotulo_atual+1));	
  	
   	insereInstrucao(&($$->info->codigo), criaRotulo(rotulo_atual)); /* Rotulo do bloco de comando. */
    	rotulo_atual++;	
   	$$->info->codigo = concatenaCodigo($$->info->codigo, $8->info->codigo); /* Carrega o codigo do bloco de comando. */
	
   	//imprimeRotulo(rotulo_atual);
   	insereInstrucao(&($$->info->codigo), criaRotulo(rotulo_atual)); /* Rotulo de desvio do bloco */
    	rotulo_atual++;	
};

clausula_if_com_else_opcional: TK_PR_IF impressaoRotulo '(' expressao ')' push_tabela_escopo impressao_cbrRotulo bloco_comandos TK_PR_ELSE push_tabela_escopo impressaoRotuloElse bloco_comandos
{
  	$1->tipo_token = infereTipoExpressao($4);
	$$ = criaNodo($1);
	adicionaNodo($$, $4);
	
	if($8 != NULL)
		adicionaNodo($$, $8);
	if($12 != NULL)
		adicionaNodo($$, $12);
  	
  	insereInstrucao(&($$->info->codigo), criaRotulo(rotulo_atual));	/* Rotulo da expressao de teste. */
  	rotulo_atual++;
  	
  	$$->info->codigo = concatenaCodigo($$->info->codigo, $4->info->codigo);	/* Carrega o codigo da expressao de teste. */
  	insereInstrucao(&($$->info->codigo), criaInstrucao_cbr ($4->info->temporario, rotulo_atual, rotulo_atual+1));	
  	
   	insereInstrucao(&($$->info->codigo), criaRotulo(rotulo_atual)); /* Rotulo do bloco de comando IF. */
    	rotulo_atual++;	
   	$$->info->codigo = concatenaCodigo($$->info->codigo, $8->info->codigo); /* Carrega o codigo do bloco de comando IF. */

   	insereInstrucao(&($$->info->codigo), criaRotulo(rotulo_atual)); /* Rotulo do bloco de comando ELSE. */
    	rotulo_atual++;	
   	$$->info->codigo = concatenaCodigo($$->info->codigo, $12->info->codigo); /* Carrega o codigo do bloco de comando ELSE. */
};

iterativo: TK_PR_WHILE impressaoRotulo '(' expressao ')' push_tabela_escopo impressao_cbrRotulo bloco_comandos
{
	$1->tipo_token = infereTipoExpressao($4);
	$$ = criaNodo($1);
	adicionaNodo($$, $4);
	if($8 != NULL)
		adicionaNodo($$, $8);
		
  	insereInstrucao(&($$->info->codigo), criaRotulo(rotulo_atual));	/* Rotulo da expressao de teste. */
  	rotulo_atual++;
  	
  	$$->info->codigo = concatenaCodigo($$->info->codigo, $4->info->codigo);	/* Carrega o codigo da expressao de teste. */
  	insereInstrucao(&($$->info->codigo), criaInstrucao_cbr ($4->info->temporario, rotulo_atual, rotulo_atual+1));	
  	
   	insereInstrucao(&($$->info->codigo), criaRotulo(rotulo_atual)); /* Rotulo do bloco de comando. */
    	rotulo_atual++;	
   	$$->info->codigo = concatenaCodigo($$->info->codigo, $8->info->codigo); /* Carrega o codigo do bloco de comando. */
   	
   	insereInstrucao(&($$->info->codigo), criaInstrucao_jumpI (rotulo_atual-2));	
   		
   	//imprimeRotulo(rotulo_atual);
   	insereInstrucao(&($$->info->codigo), criaRotulo(rotulo_atual)); /* Rotulo da saida do laco. */
    	rotulo_atual++;	
};

expressao:  expressao2	{ $$ = $1; };

expressao:  expressao  TK_OC_OR expressao2
{ 
	$$ = criaNodo($2); 
  	adicionaNodo($$, $1); 
  	adicionaNodo($$, $3); 
  	
  	$$->info->temporario = temporario_atual;
  	temporario_atual++;
  	
  	$$->info->codigo = concatenaCodigo($1->info->codigo, $3->info->codigo);
  	insereInstrucao(&($$->info->codigo), criaInstrucaoAritmeticaLogica("or",$1->info->temporario,$3->info->temporario,$$->info->temporario));
};

expressao2: expressao3 { $$ = $1; };

expressao2: expressao2 TK_OC_AND expressao3
{ 
	$$ = criaNodo($2); 
  	adicionaNodo($$, $1); 
  	adicionaNodo($$, $3); 
  	
  	$$->info->temporario = temporario_atual;
  	temporario_atual++;
  	
  	$$->info->codigo = concatenaCodigo($1->info->codigo, $3->info->codigo);
  	insereInstrucao(&($$->info->codigo), criaInstrucaoAritmeticaLogica("and",$1->info->temporario,$3->info->temporario,$$->info->temporario));
}; 		

expressao3: expressao4 { $$ = $1; };
expressao3: expressao3 TK_OC_EQ expressao4
{ 
	$$ = criaNodo($2); 
  	adicionaNodo($$, $1); 
  	adicionaNodo($$, $3); 
  	
  	$$->info->temporario = temporario_atual;
  	temporario_atual++;
  	
  	$$->info->codigo = concatenaCodigo($1->info->codigo, $3->info->codigo);
  	insereInstrucao(&($$->info->codigo), criaInstrucaoAritmeticaLogica("cmp_EQ",$1->info->temporario,$3->info->temporario,$$->info->temporario));
}; 		
 	
expressao3: expressao3 TK_OC_NE expressao4
{ 
	$$ = criaNodo($2); 
  	adicionaNodo($$, $1); 
  	adicionaNodo($$, $3); 
  	
  	$$->info->temporario = temporario_atual;
  	temporario_atual++;
  	
  	$$->info->codigo = concatenaCodigo($1->info->codigo, $3->info->codigo);
  	insereInstrucao(&($$->info->codigo), criaInstrucaoAritmeticaLogica("cmp_NE",$1->info->temporario,$3->info->temporario,$$->info->temporario));
};		
	
expressao4: expressao5 	{ $$ = $1; };
expressao4: expressao4 '<' expressao5 	
{ 
	$$ = criaNodo($2); 
  	adicionaNodo($$, $1); 
  	adicionaNodo($$, $3); 
  	
  	$$->info->temporario = temporario_atual;
  	temporario_atual++;
  	
  	$$->info->codigo = concatenaCodigo($1->info->codigo, $3->info->codigo);
  	insereInstrucao(&($$->info->codigo), criaInstrucaoAritmeticaLogica("cmp_LT",$1->info->temporario,$3->info->temporario,$$->info->temporario));
};
			
expressao4: expressao4 '>' expressao5
{ 
	$$ = criaNodo($2); 
  	adicionaNodo($$, $1); 
  	adicionaNodo($$, $3); 
  	
  	$$->info->temporario = temporario_atual;
  	temporario_atual++;
  	
  	$$->info->codigo = concatenaCodigo($1->info->codigo, $3->info->codigo);
  	insereInstrucao(&($$->info->codigo), criaInstrucaoAritmeticaLogica("cmp_GT",$1->info->temporario,$3->info->temporario,$$->info->temporario));
};
			
expressao4: expressao4 TK_OC_LE expressao5
{ 
	$$ = criaNodo($2); 
  	adicionaNodo($$, $1); 
  	adicionaNodo($$, $3); 
  	
  	$$->info->temporario = temporario_atual;
  	temporario_atual++;
  	
  	$$->info->codigo = concatenaCodigo($1->info->codigo, $3->info->codigo);
  	insereInstrucao(&($$->info->codigo), criaInstrucaoAritmeticaLogica("cmp_LE",$1->info->temporario,$3->info->temporario,$$->info->temporario));
};

expressao4: expressao4 TK_OC_GE expressao5
{ 
	$$ = criaNodo($2); 
  	adicionaNodo($$, $1); 
  	adicionaNodo($$, $3); 
  	
  	$$->info->temporario = temporario_atual;
  	temporario_atual++;
  	
  	$$->info->codigo = concatenaCodigo($1->info->codigo, $3->info->codigo);
  	insereInstrucao(&($$->info->codigo), criaInstrucaoAritmeticaLogica("cmp_GE",$1->info->temporario,$3->info->temporario,$$->info->temporario));
};

expressao5: expressao6 	{ $$ = $1; };

expressao5: expressao5 '+' expressao6 			
{ 
	$$ = criaNodo($2); 
  	adicionaNodo($$, $1); 
  	adicionaNodo($$, $3); 
  	
  	$$->info->temporario = temporario_atual;
  	temporario_atual++;
  	
  	$$->info->codigo = concatenaCodigo($1->info->codigo, $3->info->codigo);
  	insereInstrucao(&($$->info->codigo), criaInstrucaoAritmeticaLogica("add",$1->info->temporario,$3->info->temporario,$$->info->temporario));
};

expressao5: expressao5 '-' expressao6 		
{ 
	$$ = criaNodo($2); 
  	adicionaNodo($$, $1); 
  	adicionaNodo($$, $3); 
  	
  	$$->info->temporario = temporario_atual;
  	temporario_atual++;
  	
  	$$->info->codigo = concatenaCodigo($1->info->codigo, $3->info->codigo);
  	insereInstrucao(&($$->info->codigo), criaInstrucaoAritmeticaLogica("sub",$1->info->temporario,$3->info->temporario,$$->info->temporario));
};

expressao6: expressao7 	{ $$ = $1; };

expressao6: expressao6 '*' expressao7
{ 
	$$ = criaNodo($2); 
  	adicionaNodo($$, $1); 
  	adicionaNodo($$, $3); 
  	
  	$$->info->temporario = temporario_atual;
  	temporario_atual++;
  	
  	$$->info->codigo = concatenaCodigo($1->info->codigo, $3->info->codigo);
  	insereInstrucao(&($$->info->codigo), criaInstrucaoAritmeticaLogica("mult",$1->info->temporario,$3->info->temporario,$$->info->temporario));
};

expressao6: expressao6 '/' expressao7 
{ 
	$$ = criaNodo($2); 
  	adicionaNodo($$, $1); 
  	adicionaNodo($$, $3); 
  	
  	$$->info->temporario = temporario_atual;
  	temporario_atual++;
  	
  	$$->info->codigo = concatenaCodigo($1->info->codigo, $3->info->codigo);
  	insereInstrucao(&($$->info->codigo), criaInstrucaoAritmeticaLogica("div",$1->info->temporario,$3->info->temporario,$$->info->temporario));
  	
};	
		
expressao6: expressao6 '%' expressao7 			{ $$ = criaNodo($2); adicionaNodo($$, $1); adicionaNodo($$, $3); };

expressao7: '(' expressao ')'				{ $$ = $2; };
expressao7: '!' expressao7				{ $$ = criaNodo($1); adicionaNodo($$, $2); };
expressao7: '-' expressao7				{ $$ = criaNodo($1); adicionaNodo($$, $2); };
expressao7: chamada_funcao				{ $$ = $1; };
expressao7: literal 					{ $$ = criaNodo($1); insereInstrucao(&($$->info->codigo), criaInstrucao_loadI($1->valor_token,$1->temporario)); };

expressao7: TK_IDENTIFICADOR				
{ 
	$$ = criaNodo($1); 
	verificaERR_UNDECLARED_FUNCTION(lista_tabelas,$1);
	$1->tipo_token = obtemTipo(lista_tabelas,$1);
	$1->tamanho_token = infereTamanho($1->tipo_token);
	
	$1->temporario = temporario_atual;
	temporario_atual++;
	atualizaRegistradorEscopo(lista_tabelas, registrador_escopo, $1->valor_token);
	deslocamento_atual = achaDeslocamento(lista_tabelas,$1->valor_token);
	insereInstrucao(&($$->info->codigo), criaInstrucao_loadAI($1->temporario,registrador_escopo,deslocamento_atual));
};

literal: TK_LIT_INT  	
{ 
	$$ = $1;
	$1->tipo_token = INT;
	insereUltimaTabela(&lista_tabelas, $1); 
	
	$1->temporario = temporario_atual;
	temporario_atual++;
};

literal: TK_LIT_FLOAT 	
{ 
	$$ = $1;
	$1->tipo_token = FLOAT;
	insereUltimaTabela(&lista_tabelas, $1); 
};

literal: TK_LIT_FALSE 
{ 
	$$ = $1;
	$1->tipo_token = BOOL;
	insereUltimaTabela(&lista_tabelas, $1); 
};

literal: TK_LIT_TRUE
{ 
	$$ = $1;
	$1->tipo_token = BOOL;
	insereUltimaTabela(&lista_tabelas, $1); 
};

%%

void yyerror(char const *s)
{
	extern int yylineno;
	printf("ERRO DE SINTAXE - LINHA %d - %s\n", yylineno, s);	
}
