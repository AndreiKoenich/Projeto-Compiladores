#include <stdio.h>
#include "funcoes.h"

extern int yyparse(void);
extern int yylex_destroy(void);
void *arvore = NULL;
Lista_tabelas *lista_tabelas = NULL;
Tabela *tabela_global = NULL;
Tabela *tabela_escopo = NULL;
char tipo_atual[QUANTIDADE_CARACTERES_TIPO];

void exporta (void *arvore)
{
    if(arvore == NULL)
        return;

    else
    {
        Nodo* nodo = (Nodo*) arvore;
        printf("%p [label=\"%s\"];\n", nodo, nodo->info->valor_token);
        for (int i = 0; i < nodo->numeroFilhos; i++)
        {
            printf("%p, %p\n", nodo, nodo->filho[i]);
            exporta(nodo->filho[i]);
        }
    }
}

int main (int argc, char **argv)
{
    int ret = yyparse();
    //exporta (arvore);
    yylex_destroy();
    return ret;
}
