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
    printf("%p [label=\"%s\"];\n", nodo, nodo->info);
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
