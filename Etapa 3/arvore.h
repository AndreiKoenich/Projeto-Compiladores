#ifndef _ARVORE_H_
#define _ARVORE_H_

#define LITERAL 0
#define IDENTIFIER 1
#define OPERATOR 2
#define CONTROL 3
#define TYPE 4
#define SYNTAX_TOKEN 5

#include <string.h>
#include <stdlib.h>

typedef struct ValorLexico ValorLexico;
struct ValorLexico
{
	int linha_token;
	char *valor_token;
	int tipo_token;
};

/* Estrutura para representar um nodo da árvore. */
typedef struct Nodo Nodo;
struct Nodo{
    struct ValorLexico *info;
    struct Nodo** filho;
    int numeroFilhos;
};

char* concat_call(char* s1);

/* Função para criar um novo nó da árvore. */
Nodo* criaNodo(ValorLexico* info);

/* Função para adicionar um filho a um nó. */
void adicionaNodo(Nodo* pai, Nodo* filho);

/* Função para remover um nó e todos os seus descendentes. */
void removeNodo(Nodo* node);

/* Função para imprimir a árvore usando um percurso em profundidade (DFS) */
void impressaoDFS(Nodo* raiz);

#endif 
