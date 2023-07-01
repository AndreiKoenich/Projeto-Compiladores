#ifndef _ARVORE_H_
#define _ARVORE_H_

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
    char* info;
    struct Nodo** filho;
    int numeroFilhos;
};

/* Função para criar um novo nó da árvore. */
Nodo* criaNodo(ValorLexico* info);

/* Função para adicionar um filho a um nó. */
void adicionaNodo(Nodo* pai, Nodo* filho);

/* Função para remover um nó e todos os seus descendentes. */
void removeNodo(Nodo* node);

/* Função para imprimir a árvore usando um percurso em profundidade (DFS) */
void impressaoDFS(Nodo* raiz);

#endif 
