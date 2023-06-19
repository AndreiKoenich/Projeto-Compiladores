#ifndef _ARVORE_H_
#define _ARVORE_H_

typedef struct
{
	int linha_token;
	char *valor_token;
	char *tipo_token;
	
} ValorLexico;

/* Estrutura para representar um nodo da árvore. */
typedef struct
{
    struct ValorLexico *info;
    struct Nodo** filho;
    int numeroFilhos;

} Nodo;

/* Função para criar um novo nó da árvore. */
Nodo* criaNodo(ValorLexico* info);

/* Função para adicionar um filho a um nó. */
void adicionaNodo(Nodo* pai, Nodo* filho);

/* Função para remover um nó e todos os seus descendentes. */
void removeNodo(Nodo* node);

/* Função para imprimir a árvore usando um percurso em profundidade (DFS) */
void impressaoDFS(Nodo* raiz);

#endif 
