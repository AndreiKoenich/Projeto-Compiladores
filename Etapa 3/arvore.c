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

Nodo* get_last_valid_node_from_list(Nodo* list){
    int last_child_pos = list->numeroFilhos-1;
    int child_type = list->filho[last_child_pos]->info->tipo_token;
    char* child_value = list->filho[last_child_pos]->info->valor_token;
    while(child_type == LANGUAGE_OPERATOR || child_type == CONTROL){
        list = list->filho[last_child_pos];
        last_child_pos = list->numeroFilhos-1;
        child_type = list->filho[last_child_pos]->info->tipo_token;
        child_value = list->filho[last_child_pos]->info->valor_token;
    }
    return list;
}

void concatenate_list(Nodo* list1, Nodo* list2){
    Nodo* last_node_from_list = get_last_valid_node_from_list(list1);
    adicionaNodo(last_node_from_list, list2);
}