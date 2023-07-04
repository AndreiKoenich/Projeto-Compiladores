#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"

ValorLexico* vlcpy(ValorLexico *info){
    ValorLexico* info_copy = (ValorLexico*) malloc(sizeof(ValorLexico));
    info_copy->linha_token = info->linha_token;
    info_copy->tipo_token = info->tipo_token;
    info_copy->valor_token = strdup(info->valor_token);
    return info_copy;
}

/* Fun��o para criar um novo n� da �rvore. */
Nodo* criaNodo(ValorLexico *info)
{
    Nodo* novoNodo = (Nodo*)malloc(sizeof(Nodo));
    novoNodo->info = vlcpy(info);
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

    printf("%s ", raiz->info);

    for (int i = 0; i < raiz->numeroFilhos; i++)
        impressaoDFS(raiz->filho[i]);
}
