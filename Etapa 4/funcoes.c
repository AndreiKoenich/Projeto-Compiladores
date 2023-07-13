#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"

char* concat_call(char* s1)
{
    int s_size = strlen(s1);
	char* s_copy = strdup(s1);
	s1 = malloc(sizeof(char)*(s_size+5));
	strcpy(s1, "call ");
	strcpy(s1+5, s_copy);
    return s1;
}

Nodo* criaNodo(ValorLexico *info)
{
    Nodo* novoNodo = (Nodo*)malloc(sizeof(Nodo));
    novoNodo->info = info;
    novoNodo->numeroFilhos = 0;
    novoNodo->filho = NULL;
    return novoNodo;
}

void adicionaNodo(Nodo* pai, Nodo* filho)
{
    pai->numeroFilhos++;
    pai->filho = (Nodo**)realloc(pai->filho, pai->numeroFilhos * sizeof(Nodo*));
    pai->filho[pai->numeroFilhos - 1] = filho;
}

void removeNodo(Nodo* node)
{
    if (node == NULL)
        return;

    for (int i = 0; i < node->numeroFilhos; i++)
        removeNodo(node->filho[i]);

    free(node->filho);
    free(node);
}

Nodo* get_last_valid_node_from_list(Nodo* list)
{
    int last_child_pos = list->numeroFilhos;
    last_child_pos = last_child_pos-1;

    if(list->numeroFilhos == 0)
        return list;

    int child_type = list->filho[last_child_pos]->info->natureza_token;
    char* child_value = list->filho[last_child_pos]->info->valor_token;

    while(child_type == LANGUAGE_OPERATOR || child_type == CONTROL || child_type == FUNCTION_CALL)
    {
        list = list->filho[last_child_pos];
        last_child_pos = list->numeroFilhos;
        last_child_pos = last_child_pos-1;
        if(list->numeroFilhos == 0)
            break;

        child_type = list->filho[last_child_pos]->info->natureza_token;
        child_value = list->filho[last_child_pos]->info->valor_token;
    }

    return list;
}

void insereEntradaTabela (Tabela** tabela, ValorLexico *valor_lexico)
{
    Tabela* novo = (Tabela*)malloc(sizeof(Tabela));
    novo->info = valor_lexico;
    novo->proximo = NULL;

    if (*tabela == NULL)
        *tabela = novo;

    else
    {
        Tabela* atual = *tabela;
        while (atual->proximo != NULL)
            atual = atual->proximo;
        atual->proximo = novo;
    }
}

void pushTabela(Lista_tabelas **lista, Tabela *nova_tabela)
{
    Lista_tabelas* novo = (Lista_tabelas*)malloc(sizeof(Lista_tabelas));
    novo->tabela_simbolos = nova_tabela;
    novo->proximo = NULL;

    if (*lista == NULL)
    {
        novo->anterior = NULL;
        *lista = novo;
    }

    else
    {
        Lista_tabelas* atual = *lista;
        while (atual->proximo != NULL)
            atual = atual->proximo;
        atual->proximo = novo;
        novo->anterior = atual;
    }
}

void popTabela(Lista_tabelas **lista)
{
    if (*lista == NULL)
        return;

    else if ((*lista)->proximo == NULL)
    {
        free(*lista);
        *lista = NULL;
        return;
    }

    Lista_tabelas* atual = *lista;
    while (atual->proximo != NULL)
        atual = atual->proximo;

    atual->anterior->proximo = NULL;
    free(atual);
}

void destroiTabela(Tabela** tabela) 
{
    Tabela* atual = *tabela;
    Tabela* proximo;

    while (atual != NULL) 
    {
        proximo = atual->proximo;
        free(atual->info->valor_token);
        free(atual->info->tipo_token);
        free(atual->info);
        free(atual);
        atual = proximo;
    }

    *tabela = NULL;
}

void destroiListaTabelas(Lista_tabelas** lista_tabelas) 
{
    Lista_tabelas* atual = *lista_tabelas;
    Lista_tabelas* proximo;

    while (atual != NULL) 
    {
        proximo = atual->proximo;
        destroiTabela(&(atual->tabela_simbolos));
        free(atual);
        atual = proximo;
    }

    *lista_tabelas = NULL;
}


void imprimeTabela(Tabela *tabela)
{
	Tabela* atual = tabela;

	while (atual != NULL) 
	{
		printf("VALOR: %s\n", atual->info->valor_token);
		printf("TIPO: %s\n", atual->info->tipo_token);
		printf("NATUREZA: %d\n", atual->info->natureza_token);
		printf("LINHA: %d\n", atual->info->linha_token);
		atual = atual->proximo;
		printf("\n\n");
	}

	printf("\n");
}

void concatenate_list(Nodo* list1, Nodo* list2)
{
    Nodo* last_node_from_list = get_last_valid_node_from_list(list1);
    adicionaNodo(last_node_from_list, list2);
}
