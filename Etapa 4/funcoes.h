#ifndef _FUNCOES_H_
#define _FUNCOES_H_

#define LITERAL 0
#define IDENTIFIER 1
#define EXPRESSION_OPERATOR 2
#define LANGUAGE_OPERATOR 3
#define CONTROL 4
#define TYPE 5
#define SYNTAX_TOKEN 6
#define FUNCTION_CALL 7

#define QUANTIDADE_CARACTERES_TIPO 100 /* A palavra "float" é a maior entre os nomes dos tipos, e possui 5 caracteres. */
#define MAXIMO_CARACTERES_NOME 100
#define MAXIMO_CARACTERES_INT 100
#define MAXIMO_CARACTERES_FLOAT 100

#include <string.h>
#include <stdlib.h>

typedef struct
{
	char *valor_token;
	
	int linha_token;
	int natureza_token;
	char *tipo_token;
	
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
char* infereTipo(ValorLexico *operando1, ValorLexico *operando2);

void concatenate_list(Nodo* list1, Nodo* list2);




#endif
