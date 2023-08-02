/* PROJETO DE COMPILADORES - ETAPA 5  */

/* Andrei Pochmann Koenich 	 - Matrícula 00308680 */
/* Izaias Saturnino de Lima Neto - Matrícula 00326872 */

#include <stdio.h>
#include "funcoes.h"

extern int yyparse(void);
extern int yylex_destroy(void);
void *arvore = NULL;
Lista_tabelas *lista_tabelas = NULL;
Tabela *tabela_global = NULL;
Tabela *tabela_escopo = NULL;
int tipo_atual = -1;

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
