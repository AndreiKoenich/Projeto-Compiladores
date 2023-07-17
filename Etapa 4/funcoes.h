#ifndef _FUNCOES_H_
#define _FUNCOES_H_

#define LITERAL 0
#define VARIABLE 1
#define EXPRESSION_OPERATOR 2
#define LANGUAGE_OPERATOR 3
#define CONTROL 4
#define TYPE 5
#define SYNTAX_TOKEN 6
#define FUNCTION_CALL 7
#define FUNCTION 8

#define MAXIMO_CARACTERES_TIPO 6 /* A palavra "float" é a maior entre os nomes dos tipos, e possui 5 caracteres. */
#define MAXIMO_CARACTERES_NOME 50
#define MAXIMO_CARACTERES_INT 20
#define MAXIMO_CARACTERES_FLOAT 100

#define ERR_UNDECLARED 10
#define ERR_DECLARED 11
#define ERR_VARIABLE 20 
#define ERR_FUNCTION 21
#define ERR_TYPE 22

#define INT 0
#define FLOAT 1
#define BOOL 2

#include <string.h>
#include <stdlib.h>

typedef struct
{
	char *valor_token;
	
	int linha_token;
	int natureza_token;
	int tipo_token;
	
} ValorLexico;

typedef struct nodo 
{
	ValorLexico *info;
	struct nodo** filho;
	int numeroFilhos;
    
} Nodo;

typedef struct tabela
{
	ValorLexico *info;
	struct tabela *proximo;

} Tabela;

typedef struct lista_tabelas
{
	struct lista_tabelas *proximo;
	struct lista_tabelas *anterior;
	Tabela *tabela_simbolos;

} Lista_tabelas;

char* concat_call(char* s1);

/* Função para criar um novo nó da árvore. */
Nodo* criaNodo(ValorLexico* info);

/* Função para adicionar um filho a um nó. */
void adicionaNodo(Nodo* pai, Nodo* filho);

/* Função para remover um nó e todos os seus descendentes. */
void removeNodo(Nodo* node);

/* Função para imprimir a árvore usando um percurso em profundidade (DFS) */
void impressaoDFS(Nodo* raiz);

void* inicializa_lista ();
void insereEntradaTabela (Tabela** tabela, ValorLexico *valor_lexico);
void insereUltimaTabela(Lista_tabelas** lista_tabelas, ValorLexico* valor_lexico);
void popTabela(Lista_tabelas **lista);
void pushTabela(Lista_tabelas** lista, Tabela *nova_tabela);
void destroiTabela(Tabela** tabela);
void destroiListaTabelas(Lista_tabelas** lista_tabelas);
void imprimeTabela(Tabela *tabela);
int infereTipo(int tipo1, int tipo2);
int verificaTipo(char *tipo_token);
int infereTipoExpressao(Nodo *raiz);
void verificaERR_UNDECLARED_FUNCTION(Lista_tabelas *lista_tabelas, ValorLexico* identificador);
void verificaERR_UNDECLARED_FUNCTION_TYPE(Lista_tabelas *lista_tabelas, ValorLexico* identificador, int tipo_atribuido);
void verificaERR_DECLARED(Lista_tabelas *lista_tabelas, ValorLexico* identificador);
void verificaERR_VARIABLE_UNDECLARED_chamadafuncao(Lista_tabelas *lista_tabelas, char *valor_token, int linha_token);
void concatenate_list(Nodo* list1, Nodo* list2);
int obtemTipo(Lista_tabelas *lista_tabelas, ValorLexico* identificador);
char* obtemNomeFuncao(char* nomeChamadaFuncao);
char* obtemNomeTipo (int valor_tipo);

#endif
