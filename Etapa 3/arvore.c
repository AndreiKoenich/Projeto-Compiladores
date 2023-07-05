#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"

char* concat_call(char* s1){
    int s_size = strlen(s1);
	char* s_copy = strdup(s1);
	s1 = malloc(sizeof(char)*(s_size+5));
	strcpy(s1, "call ");
	strcpy(s1+5, s_copy);
    return s1;
}

/* Fun��o para criar um novo n� da �rvore. */
Nodo* criaNodo(ValorLexico *info)
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

    printf("%s ", raiz->info->valor_token);

    for (int i = 0; i < raiz->numeroFilhos; i++)
        impressaoDFS(raiz->filho[i]);
}
