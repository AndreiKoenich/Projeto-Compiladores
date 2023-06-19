#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"

/* Função para criar um novo nó da árvore. */
Nodo* criaNodo(ValorLexico *info)
{
    Nodo* novoNodo = (Nodo*)malloc(sizeof(Nodo));
    novoNodo->info = info;
    novoNodo->numeroFilhos = 0;
    novoNodo->filho = NULL;
    return novoNodo;
}

/* Função para adicionar um filho a um nó. */
void adicionaNodo(Nodo* pai, Nodo* filho)
{
    pai->numeroFilhos++;
    pai->filho = (Nodo**)realloc(pai->filho, pai->numeroFilhos * sizeof(Nodo*));
    pai->filho[pai->numeroFilhos - 1] = filho;
}

/* Função para remover um nó e todos os seus descendentes. */
void removeNodo(Nodo* node) {
    if (node == NULL)
        return;

    for (int i = 0; i < node->numeroFilhos; i++)
        removeNodo(node->filho[i]);

    free(node->filho);
    free(node);
}

/* Função para imprimir a árvore usando um percurso em profundidade (DFS) */
void impressaoDFS(Nodo* raiz) {
    if (raiz == NULL)
        return;

    printf("%d ", raiz->info);

    for (int i = 0; i < raiz->numeroFilhos; i++)
        impressaoDFS(raiz->filho[i]);
}
