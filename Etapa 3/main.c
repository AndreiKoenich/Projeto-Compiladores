#include <stdio.h>
#include "arvore.h"
extern int yyparse(void);
extern int yylex_destroy(void);
void *arvore = NULL;
void exporta (void *arvore){
  if(arvore == NULL){
    return;
  }
  else{
    Nodo* nodo = (Nodo*) arvore;
    if(nodo->info->tipo_token != 2){
      printf("%p [label=\"%s\"];\n", nodo, nodo->info->valor_token);
      printf("linha_token: %i\n", nodo->info->linha_token);
      printf("tipo_token: %i\n", nodo->info->tipo_token);
    }
    for (int i = 0; i < nodo->numeroFilhos; i++)
    {
      exporta(nodo->filho[i]);
    }
  }
}
int main (int argc, char **argv)
{
  int ret = yyparse(); 
  exporta (arvore);
  yylex_destroy();
  return ret;
}
