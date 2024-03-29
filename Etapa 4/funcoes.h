/* PROJETO DE COMPILADORES - ETAPA 4  */

/* Andrei Pochmann Koenich 	 - Matrícula 00308680 */
/* Izaias Saturnino de Lima Neto - Matrícula 00326872 */

#ifndef _FUNCOES_H_
#define _FUNCOES_H_

/* Constantes para definir a natureza de um nodo da AST, ou de um identificador da linguagem. */
#define LITERAL 0
#define VARIABLE 1
#define EXPRESSION_OPERATOR 2
#define LANGUAGE_OPERATOR 3
#define CONTROL 4
#define TYPE 5
#define SYNTAX_TOKEN 6
#define FUNCTION_CALL 7
#define FUNCTION 8

/* Constantes para determinar o numero maximo de caracteres dos identificador e dos literais da linguagem. */
#define MAXIMO_CARACTERES_TIPO 6
#define MAXIMO_CARACTERES_NOME 50
#define MAXIMO_CARACTERES_INT 20
#define MAXIMO_CARACTERES_FLOAT 100

/* Constantes para determinar o tamanho de memoria (em bytes) ocupado por cada tipo de dado. */
#define TAMANHO_MEMORIA_INT 4
#define TAMANHO_MEMORIA_FLOAT 4
#define TAMANHO_MEMORIA_BOOL 1

/* Constantes para associar um valor inteiro a cada tipo de erro semantico. */
#define ERR_UNDECLARED 10
#define ERR_DECLARED 11
#define ERR_VARIABLE 20 
#define ERR_FUNCTION 21
#define ERR_TYPE 22

/* Constantes para associar um valor inteiro a cada um dos tres tipos de dados da linguagem. */
#define INT 0
#define FLOAT 1
#define BOOL 2

#include <string.h>
#include <stdlib.h>

/* Estrutura contendo as informacoes de cada nodo da AST. */
typedef struct 
{
	char *valor_token;
	
	int linha_token;
	int natureza_token;
	int tipo_token;
	int tamanho_token;
	
} ValorLexico;

/* Estrutura responsavel por representar um nodo da AST, com suas informacoes e seus filhos. */
typedef struct nodo
{
	ValorLexico *info;
	struct nodo** filho;
	int numeroFilhos;
    
} Nodo;

/* Estrutura representando cada entrada de uma tabela, implementada como uma lista simplesmente encadeada. */
typedef struct tabela
{
	ValorLexico *info;
	struct tabela *proximo;

} Tabela;

/* Estrutura representando cada tabela de uma lista de tabelas de simbolos, implementada como uma lista duplamente encadeada. */
typedef struct lista_tabelas
{
	struct lista_tabelas *proximo;
	struct lista_tabelas *anterior;
	Tabela *tabela_simbolos;

} Lista_tabelas;

/* FUNCOES PARA MANIPULACOES DA ARVORE DE SINTAXE ABSTRATA */

Nodo* criaNodo(ValorLexico* info);
void adicionaNodo(Nodo* pai, Nodo* filho);
void removeNodo(Nodo* node);

/* FUNCOES PARA MANIPULACOES DE LISTAS ENCADEADAS */

char* concat_call(char* s1);
void concatenate_list(Nodo* list1, Nodo* list2);

/* FUNCOES PARA MANIPULACOES DAS TABELAS DE SIMBOLOS */

void insereEntradaTabela (Tabela** tabela, ValorLexico *valor_lexico);
void insereUltimaTabela(Lista_tabelas** lista_tabelas, ValorLexico* valor_lexico);
void popTabela(Lista_tabelas **lista);
void pushTabela(Lista_tabelas** lista, Tabela *nova_tabela);
void destroiTabela(Tabela** tabela);
void destroiListaTabelas(Lista_tabelas** lista_tabelas);
void imprimeTabela(Tabela *tabela);
void imprimeUltimaTabela(Lista_tabelas* lista_tabelas);

/* FUNCOES PARA ANALISES SEMANTICAS E VERIFICACOES DE ERROS */

void verificaERR_UNDECLARED_FUNCTION(Lista_tabelas *lista_tabelas, ValorLexico* identificador);
void verificaERR_DECLARED(Lista_tabelas *lista_tabelas, ValorLexico* identificador);
void verificaERR_VARIABLE_UNDECLARED_chamadafuncao(Lista_tabelas *lista_tabelas, char *valor_token, int linha_token);
int infereTipo(int tipo1, int tipo2);
int verificaTipo(char *tipo_token);
int infereTipoExpressao(Nodo *raiz);
int obtemTipo(Lista_tabelas *lista_tabelas, ValorLexico* identificador);
int infereTamanho(int tipo_token);
char* obtemNomeFuncao(char* nomeChamadaFuncao);

#endif
