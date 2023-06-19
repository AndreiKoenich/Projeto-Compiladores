#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int linha_token;
    char *tipo_token;
    char *valor_token;

} valor_lexico;

/* Estrutura para representar um nodo da �rvore. */
typedef struct
{
    int info;
    struct Nodo** filho;
    int numeroFilhos;

} Nodo;

/* Fun��o para criar um novo n� da �rvore. */
Nodo* criaNodo(int info)
{
    Nodo* novoNodo = (Nodo*)malloc(sizeof(Nodo));
    novoNodo->info = info;
    novoNodo->numeroFilhos = 0;
    novoNodo->filho = NULL;
    return novoNodo;
}

/* Fun��o para adicionar um filho a um n�. */
void adicionaNodo(Nodo* pai, Nodo* filho)
{
    pai->numeroFilhos++;
    pai->filho = (Nodo**)realloc(pai->filho, pai->numeroFilhos * sizeof(Nodo*));
    pai->filho[pai->numeroFilhos - 1] = filho;
}

/* Fun��o para remover um n� e todos os seus descendentes. */
void removeNodo(Nodo* node) {
    if (node == NULL)
        return;

    for (int i = 0; i < node->numeroFilhos; i++)
        removeNodo(node->filho[i]);

    free(node->filho);
    free(node);
}

/* Fun��o para imprimir a �rvore usando um percurso em profundidade (DFS) */
void impressaoDFS(Nodo* raiz) {
    if (raiz == NULL)
        return;

    printf("%d ", raiz->info);

    for (int i = 0; i < raiz->numeroFilhos; i++)
        impressaoDFS(raiz->filho[i]);
}

int main ()
{
    return 0;
}
