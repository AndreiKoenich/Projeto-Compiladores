#ifndef _ARVORE_H_
#define _ARVORE_H_

#define LITERAL 0
#define IDENTIFIER 1
#define EXPRESSION_OPERATOR 2
#define LANGUAGE_OPERATOR 3
#define CONTROL 4
#define TYPE 5
#define SYNTAX_TOKEN 6
#define FUNCTION_CALL 7

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

void concatenate_list(Nodo* list1, Nodo* list2);
#endif 
