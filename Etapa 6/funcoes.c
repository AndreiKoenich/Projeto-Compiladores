/* PROJETO DE COMPILADORES - ETAPA 6  */

/* Andrei Pochmann Koenich 	 - Matrícula 00308680 */
/* Izaias Saturnino de Lima Neto - Matrícula 00326872 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"

/* Adiciona a string "call" ao inicio de uma outra string. */
char* concat_call(char* s1) 
{
    int s_size = strlen(s1);
	char* s_copy = strdup(s1);
	s1 = malloc(sizeof(char)*(s_size+5));
	strcpy(s1, "call ");
	strcpy(s1+5, s_copy);
    return s1;
}

/* Recebe informacoes a respeito de um novo nodo a ser inserido na arvore, instancia e retorna o nodo. */
Nodo* criaNodo(ValorLexico *info) 
{
    Nodo* novoNodo = (Nodo*)malloc(sizeof(Nodo));
    novoNodo->info = info;
    novoNodo->numeroFilhos = 0;
    novoNodo->filho = NULL;
    return novoNodo;
}

/* Adiciona um novo nodo na arvore AST. */
void adicionaNodo(Nodo* pai, Nodo* filho)
{
    pai->numeroFilhos++;
    pai->filho = (Nodo**)realloc(pai->filho, pai->numeroFilhos*sizeof(Nodo*));
    pai->filho[pai->numeroFilhos - 1] = filho;
}

/* Remove um nodo da arvore AST. */
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

/* Recebe uma tabela de simbolos e uma entrada, e insere as informacoes da entrada na tabela. */
void insereEntradaTabela(Tabela** tabela, ValorLexico* valor_lexico)
{
    Tabela* novo = (Tabela*)malloc(sizeof(Tabela));
    novo->info = (ValorLexico*)malloc(sizeof(ValorLexico));

    novo->info->valor_token = strdup(valor_lexico->valor_token);
    novo->info->linha_token = valor_lexico->linha_token;
    novo->info->natureza_token = valor_lexico->natureza_token;
    novo->info->tipo_token = valor_lexico->tipo_token;
    novo->info->tamanho_token = valor_lexico->tamanho_token;
    novo->info->deslocamento = valor_lexico->deslocamento;

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

/* Recebe uma lista de tabela de simbolos e uma entrada, e insere a entrada na ultima tabela de simbolos da lista. */
void insereUltimaTabela(Lista_tabelas** lista_tabelas, ValorLexico* valor_lexico) 
{   
    if (*lista_tabelas == NULL || valor_lexico == NULL) 
        return;
    
    Lista_tabelas* atual = *lista_tabelas;
    
    while (atual->proximo != NULL) 
        atual = atual->proximo;
    
   valor_lexico->deslocamento = atual->endereco_atual; /* Obtem o endereco atual da variavel (que pode ser local ou global), e armazena na estrutura. */
   insereEntradaTabela(&(atual->tabela_simbolos), valor_lexico);
   atual->endereco_atual += infereTamanho(valor_lexico->tipo_token); /* Atualiza o espaco de memoria ocupado. */
}

/* Insere uma nova tabela de simbolos na lista de tabela de simbolos. */
void pushTabela(Lista_tabelas **lista, Tabela *nova_tabela)
{
    //printf("EMPILHOU\n");
    
    Lista_tabelas* novo = (Lista_tabelas*)malloc(sizeof(Lista_tabelas));
    novo->tabela_simbolos = nova_tabela;
    novo->proximo = NULL;
    novo->endereco_atual = 0; /* Inicializa o endereco atual como sendo zero, sem nenhum deslocamento. */

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

/* Remove a ultima tabela de simbolos que foi adicionada na lista de tabela de simbolos. */
void popTabela(Lista_tabelas **lista_tabelas)
{
    //printf("DESEMPILHOU\n");
    
    if (*lista_tabelas == NULL) 
        return;
    
    Lista_tabelas* atual = *lista_tabelas;
    
    while (atual->proximo != NULL)
        atual = atual->proximo;
    
    if (atual->anterior == NULL) 
        *lista_tabelas = NULL;
        
    else 
        atual->anterior->proximo = NULL;
    
    destroiTabela(&(atual->tabela_simbolos));
    free(atual);
}

/* Destroi uma tabela de simbolos, liberando o espaco de memoria referente a ela. */
void destroiTabela(Tabela** tabela) 
{
    Tabela* atual = *tabela;
    Tabela* proximo;

    while (atual != NULL) 
    {
        proximo = atual->proximo;
        free(atual->info->valor_token);
        free(atual->info);
        free(atual);
        atual = proximo;
    }

    *tabela = NULL;
}

/* Destroi todas as tabelas de simbolos de uma lista de tabelas de simbolos. */
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

/* Verifica os seguintes erros de semantica:
ERR_UNDECLARED - Caso o identificador não tenha sido declarado no seu uso;
ERR_FUNCTION   - Caso o identificador dito como funcao esteja sendo usado como variavel. */
void verificaERR_UNDECLARED_FUNCTION(Lista_tabelas *lista_tabelas, ValorLexico* identificador)
{
    Lista_tabelas *lista_atual = lista_tabelas;
    int achou_funcao = 0;

    while (lista_atual != NULL)
    {
	Tabela *tabela_atual = lista_atual->tabela_simbolos;

	while (tabela_atual != NULL)
	{
	    if (strcmp(identificador->valor_token, tabela_atual->info->valor_token) == 0)
	    {
	    	if (tabela_atual->info->natureza_token == FUNCTION)
			achou_funcao = 1;
	    	
	    	else if (tabela_atual->info->natureza_token == VARIABLE)
			return;	
	    }
	        
	    tabela_atual = tabela_atual->proximo;
	}

	lista_atual = lista_atual->proximo;
    }
    
    if (achou_funcao == 1)
    {
	printf("ERRO DE SEMANTICA - LINHA %d - FUNCAO '%s' SENDO USADA COMO VARIAVEL\n", identificador->linha_token, identificador->valor_token);
	exit(ERR_FUNCTION);   
    }
    
    else
    {
	printf("ERRO DE SEMANTICA - LINHA %d - IDENTIFICADOR '%s' NAO DECLARADO\n", identificador->linha_token, identificador->valor_token);
	exit(ERR_UNDECLARED);   
    }
}

/* Verifica os seguintes erros de semantica, nos casos de uma chama de funcao no codigo sendo analisado:
ERR_VARIABLE   - Caso o identificador dito como variavel esteja sendo usado como funcao.
ERR_UNDECLARED - Caso o identificador não tenha sido declarado no seu uso; */
void verificaERR_VARIABLE_UNDECLARED_chamadafuncao(Lista_tabelas *lista_tabelas, char *valor_token, int linha_token)
{
	Lista_tabelas *lista_atual = lista_tabelas;
	
	int achou_funcao = -1;

	while (lista_atual != NULL)
	{
        	Tabela *tabela_atual = lista_atual->tabela_simbolos;

		while (tabela_atual != NULL)
		{
		    if (strcmp(valor_token, tabela_atual->info->valor_token) == 0)
		    {
		   	if (tabela_atual->info->natureza_token == FUNCTION)
		   		achou_funcao = 1;
		   	else
		   		achou_funcao = 0;	
		    }
		        
		    tabela_atual = tabela_atual->proximo;
		}

        	lista_atual = lista_atual->proximo;
    	}
    	
    	if (achou_funcao == 1)
    		return;
    		
    	else if (achou_funcao == 0)
    	{
		printf("ERRO DE SEMANTICA - LINHA %d - VARIAVEL '%s' SENDO USADA COMO FUNCAO\n", linha_token, valor_token);
		exit(ERR_VARIABLE);    		
    	}
    	
    	else if (achou_funcao == -1)
    	{
		printf("ERRO DE SEMANTICA - LINHA %d - IDENTIFICADOR '%s' NAO DECLARADO\n", linha_token, valor_token);
		exit(ERR_UNDECLARED);	    	
    	}
}

/* Verifica o seguinte erro de semantica:
ERR_DECLARED - Nos casos em que um identificador ja declarado esteja sendo redeclarado. */
void verificaERR_DECLARED(Lista_tabelas *lista_tabelas, ValorLexico* identificador)
{
	Lista_tabelas *lista_atual = lista_tabelas;
	
	if (identificador->natureza_token != FUNCTION)
		while (lista_atual->proximo != NULL)
			lista_atual = lista_atual->proximo;

	Tabela *tabela_atual;
	tabela_atual = lista_atual->tabela_simbolos;

	while (tabela_atual != NULL)
	{
		if (strcmp(identificador->valor_token, tabela_atual->info->valor_token) == 0)
		{
			printf("ERRO DE SEMANTICA - LINHA %d - REDECLARACAO DO IDENTIFICADOR '%s'\n", identificador->linha_token, identificador->valor_token);
			exit(ERR_DECLARED);
		}

		tabela_atual = tabela_atual->proximo;
	}

	return;
}

/* Recebe um valor inteiro referente a um tipo, e retorna uma string indicando o tipo. */
char* obtemNomeTipo (int valor_tipo)
{
	char* nome_tipo = calloc(MAXIMO_CARACTERES_TIPO, sizeof(char));
	if (valor_tipo == INT)
		strcpy(nome_tipo,"int");
	else if (valor_tipo == FLOAT)
		strcpy(nome_tipo,"float");
	else if (valor_tipo == BOOL)
		strcpy(nome_tipo,"bool");
	return nome_tipo;
}

/* Recebe uma string qualificando uma chamada de funcao com "call " concatenado, e obtem o nome da funcao sendo chamada. */
char* obtemNomeFuncao(char* nomeChamadaFuncao)
{
	char *nomeFuncao = calloc(MAXIMO_CARACTERES_NOME,sizeof(char));
	strcpy(nomeFuncao,nomeChamadaFuncao+5);
	return nomeFuncao;
}

/* Recebe uma tabela de simbolos, e imprime cada simbolo com suas caracteristicas. */
void imprimeTabela(Tabela *tabela)
{
	Tabela* atual = tabela;

	while (atual != NULL) 
	{
		printf("VALOR: %s\n", atual->info->valor_token);
		printf("TIPO: %s\n", obtemNomeTipo(atual->info->tipo_token));
		printf("NATUREZA: %d\n", atual->info->natureza_token);
		printf("LINHA: %d\n", atual->info->linha_token);
		printf("TAMANHO: %d\n", atual->info->tamanho_token);
		printf("DESLOCAMENTO: %d\n\n", atual->info->deslocamento);
		atual = atual->proximo;
	}
}

/* Recebe uma lista de tabelas de simbolos, e imprime a ultima tabela inserida na lista. */
void imprimeUltimaTabela(Lista_tabelas *lista_tabelas)
{
	Lista_tabelas *atual = lista_tabelas;
    	while (atual->proximo != NULL)
        	atual = atual->proximo;
	printf("TABELA LOCAL:\n\n");
        imprimeTabela(atual->tabela_simbolos);
	printf("------------------\n");
}

/* Recebe dois inteiros representando um tipo de duas variaveis envolvidas em uma operacao,
e infere qual sera o tipo do resultado da operacao. */
int infereTipo(int tipo1, int tipo2)
{
	if (tipo1 == FLOAT || tipo2 == FLOAT)
	{
		//printf("INFERIU FLOAT\n");
		return FLOAT;
	}
		
	else if (tipo1 == INT || tipo2 == INT)
	{
		//printf("INFERIU INT\n");
		return INT;
	}
	
	else if (tipo1 == BOOL || tipo2 == BOOL)
	{
		//printf("INFERIU BOOL\n");
		return BOOL;
	}
	
	else
		return -1;	
}

/* Recebe um nodo raiz de uma expressao, e decide qual sera o tipo do nodo, a partir
da inferencia de tipos aplicada na expressao. */
int infereTipoExpressao(Nodo *raiz) 
{
    int tipo_encontrado = -1;
    
    if (raiz != NULL) 
    {
        //printf("ACHOU %d\n", raiz->info->tipo_token);
        
        // Atualiza o tipo encontrado com o tipo do nodo atual
        tipo_encontrado = raiz->info->tipo_token;
        
        for (int i = 0; i < raiz->numeroFilhos; i++) 
            tipo_encontrado = infereTipo(tipo_encontrado, infereTipoExpressao(raiz->filho[i]));
    }
    
    return tipo_encontrado;
}

/* Recebe uma string representando um tipo, e retorna o valor inteiro que
corresponde a esse tipo. */
int verificaTipo(char *tipo_token)
{
	if (strcmp(tipo_token,"int") == 0)
		return INT;
	else if (strcmp(tipo_token,"float") == 0)
		return FLOAT;
	else if (strcmp(tipo_token,"bool") == 0)
		return BOOL;
	else
		return -1;
}

/* Recebe um identificador, e obtem seu tipo, que esta informado na ultima tabela de simbolos inserida. */
int obtemTipo(Lista_tabelas *lista_tabelas, ValorLexico* identificador)
{
    Lista_tabelas *lista_atual = lista_tabelas;
    int tipo_atual = -1;

    while (lista_atual != NULL)
    {
        Tabela *tabela_atual = lista_atual->tabela_simbolos;

        while (tabela_atual != NULL)
        {
            if (strcmp(identificador->valor_token, tabela_atual->info->valor_token) == 0)
		tipo_atual = tabela_atual->info->tipo_token; 
            tabela_atual = tabela_atual->proximo;
        }

        lista_atual = lista_atual->proximo;
    }
    
    return tipo_atual;	
}

/* Recebe um inteiro que caracteriza um tipo, e retorna o tamanho em memoria ocupado por esse tipo. */
int infereTamanho(int tipo_token)
{
	if (tipo_token == INT)
		return TAMANHO_MEMORIA_INT;
	else if (tipo_token == FLOAT)
		return TAMANHO_MEMORIA_FLOAT;
	else if (tipo_token == BOOL)
		return TAMANHO_MEMORIA_BOOL;
	else
		return 0;
}

/* Recebe duas listas de nos da AST, e concatena as duas. */
void concatenate_list(Nodo* list1, Nodo* list2)
{
    Nodo* last_node_from_list = get_last_valid_node_from_list(list1);
    adicionaNodo(last_node_from_list, list2);
}

//done
/* Cria uma instrucao ILOC do tipo aritmetica (exceto divisao), e imprime o seu conteudo. */
Instrucao* criaInstrucaoAritmeticaLogica(char *operacao, int operando1, int operando2, int operando3)
{
    Instrucao* instrucao = (Instrucao*)malloc(sizeof(Instrucao));
    sprintf(instrucao->operacao, "%sl", operacao);

    if(strcmp(operacao, "mult") == 0){
        strcpy(instrucao->operacao, "imull");
    }

    if(operando3%2 == 0){
        sprintf(instrucao->operando1, "%ceax", '%');
        sprintf(instrucao->operando2, "%cedx", '%');
    }
    else{
        sprintf(instrucao->operando1, "%cedx", '%');
        sprintf(instrucao->operando2, "%ceax", '%');
    }
    strcpy(instrucao->operando3, "");

    return instrucao;
}

//done
Instrucao* criaInstrucao_cmp(int operando1, int operando2, int operando3){
    Instrucao* instrucao = (Instrucao*)malloc(sizeof(Instrucao));

    strcpy(instrucao->operacao, "cmp");

    if(operando3%2 == 1){
        sprintf(instrucao->operando1, "%ceax", '%');
        sprintf(instrucao->operando2, "%cedx", '%');
    }
    else{
        sprintf(instrucao->operando1, "%cedx", '%');
        sprintf(instrucao->operando2, "%ceax", '%');
    }
    strcpy(instrucao->operando3, "");

    return instrucao;
}

Instrucao* criaInstrucao_cmpI(int operando1, int operando2, int operando3){
    Instrucao* instrucao = (Instrucao*)malloc(sizeof(Instrucao));

    strcpy(instrucao->operacao, "cmp");

    sprintf(instrucao->operando1, "$%d", operando1);
    if(operando3%2 == 1){
        sprintf(instrucao->operando2, "%ceax", '%');
    }
    else{
        sprintf(instrucao->operando2, "%cedx", '%');
    }
    
    strcpy(instrucao->operando3, "");

    return instrucao;
}

Instrucao* criaInstrucao_mov_al(int operando1, int operando2, int operando3){
    Instrucao* instrucao = (Instrucao*)malloc(sizeof(Instrucao));

    strcpy(instrucao->operacao, "movzbl");

    sprintf(instrucao->operando1, "%cal", '%');

    if(operando3%2 == 1){
        sprintf(instrucao->operando2, "%ceax", '%');
    }
    else{
        sprintf(instrucao->operando2, "%cedx", '%');
    }

    strcpy(instrucao->operando3, "");
    
    return instrucao;
}

//done
Instrucao* criaInstrucao_set(char *operacao, int operando1, int operando2, int operando3){
    Instrucao* instrucao = (Instrucao*)malloc(sizeof(Instrucao));

    if(strcmp(operacao, "cmp_NE") == 0){
        strcpy(instrucao->operacao, "sete");
    }
    else if(strcmp(operacao, "cmp_GE") == 0){
        strcpy(instrucao->operacao, "setg");
    }
    else if(strcmp(operacao, "cmp_LE") == 0){
        strcpy(instrucao->operacao, "setl");
    }
    else if(strcmp(operacao, "cmp_LT") == 0){
        strcpy(instrucao->operacao, "setle");
    }
    else if(strcmp(operacao, "cmp_GT") == 0){
        strcpy(instrucao->operacao, "setge");
    }
    else if(strcmp(operacao, "cmp_EQ") == 0){
        strcpy(instrucao->operacao, "setne");
    }

    sprintf(instrucao->operando1, "%cal", '%');

    // if(operando3%2 == 1){
    //     sprintf(instrucao->operando1, "%ceax", '%');
    // }
    // else{
    //     sprintf(instrucao->operando1, "%cedx", '%');
    // }

    strcpy(instrucao->operando2, "");
    strcpy(instrucao->operando3, "");
    
    return instrucao;
}

//done
Instrucao* criaInstrucao_div_clear()
{
    Instrucao* instrucao = (Instrucao*)malloc(sizeof(Instrucao));
    strcpy(instrucao->operacao, "movl");
    strcpy(instrucao->operando1, "$0");
    sprintf(instrucao->operando2, "%cedx", '%');
    strcpy(instrucao->operando3, "");

    return instrucao;
}

Instrucao* criaInstrucao_copyR(int operando1)
{
    Instrucao* instrucao = (Instrucao*)malloc(sizeof(Instrucao));

    strcpy(instrucao->operacao, "movl");
    if(operando1%2 == 1){
        sprintf(instrucao->operando1, "%ceax", '%');
        sprintf(instrucao->operando2, "%cedx", '%');
    }
    else{
        sprintf(instrucao->operando1, "%cedx", '%');
        sprintf(instrucao->operando2, "%ceax", '%');
    }
    strcpy(instrucao->operando3, "");
    
    return instrucao;
}

//done
Instrucao* criaInstrucaoDiv(int operando1)
{
    Instrucao* instrucao = (Instrucao*)malloc(sizeof(Instrucao));

    strcpy(instrucao->operacao, "idivl");
    sprintf(instrucao->operando1, "-%d(%crbp)", operando1, '%');
    strcpy(instrucao->operando2, "");
    strcpy(instrucao->operando2, "");
    
    return instrucao;
}

//done
/* Cria uma instrucao ILOC do tipo "load I", e imprime o seu conteudo. */
Instrucao* criaInstrucao_loadI (char *operando1, int operando2)
{
    Instrucao* instrucao = (Instrucao*)malloc(sizeof(Instrucao));
    // strcpy(instrucao->operacao, "loadI");
    // strcpy(instrucao->operando1, operando1);
    // sprintf(instrucao->operando2, "r%d", operando2);
    // strcpy(instrucao->operando3, "");

    char* new_op2 = malloc(sizeof(char)*TAMANHO_NOME_OPERANDO);
    if(operando2%2 == 1){
        sprintf(new_op2, "%ceax", '%');
    }
    else{
        sprintf(new_op2, "%cedx", '%');
    }

    operando2 = operando2*4;
    strcpy(instrucao->operacao, "movl");
    sprintf(instrucao->operando1, "$%d", atoi(operando1));
    //printf("loadI op1:%d op2:%d\n", operando1, operando2);
    strcpy(instrucao->operando2, new_op2);
    strcpy(instrucao->operando3, "");

    return instrucao;
}

//done
/* Cria uma instrucao ILOC do tipo "load AI", e imprime o seu conteudo. */
Instrucao* criaInstrucao_loadAI (int operando1, char *operando2, int operando3)
{
    Instrucao* instrucao = (Instrucao*)malloc(sizeof(Instrucao));
    // strcpy(instrucao->operacao, "loadAI");
    // sprintf(instrucao->operando1, "r%d", operando1);
    // strcpy(instrucao->operando2, operando2);
    // sprintf(instrucao->operando3, "%d", operando3);

    strcpy(instrucao->operacao, "movl");
    sprintf(instrucao->operando1, "-%d(%crbp)", operando3+4, '%');
    if(operando1%2 == 1){
        sprintf(instrucao->operando2, "%ceax", '%');
    }
    else{
        sprintf(instrucao->operando2, "%cedx", '%');
    }
    strcpy(instrucao->operando3, "");
    
    return instrucao;
}

Instrucao* criaInstrucao_loadAI_ret(int operando1, char *operando2, int operando3)
{
    Instrucao* instrucao = (Instrucao*)malloc(sizeof(Instrucao));

    strcpy(instrucao->operacao, "movl");
    sprintf(instrucao->operando1, "%cedx", '%');
    sprintf(instrucao->operando2, "%ceax", '%');
    strcpy(instrucao->operando3, "");
    
    return instrucao;
}

//done
/* Cria uma instrucao ILOC do tipo "store AI", e imprime o seu conteudo. */
Instrucao* criaInstrucao_storeAI (int operando1, char *operando2, int operando3)
{
    Instrucao* instrucao = (Instrucao*)malloc(sizeof(Instrucao));
    // strcpy(instrucao->operacao, "storeAI");
    // sprintf(instrucao->operando1, "r%d", operando1);
    // strcpy(instrucao->operando2, operando2);
    // sprintf(instrucao->operando3, "%d", operando3);

    int new_op2_i = operando1*4;
    strcpy(instrucao->operacao, "movl");
    if(operando1%2 == 1){
        sprintf(instrucao->operando1, "%ceax", '%');
    }
    else{
        sprintf(instrucao->operando1, "%cedx", '%');
    }
    sprintf(instrucao->operando2, "-%d(%crbp)", operando3+4, '%');
    strcpy(instrucao->operando3, "");
    return instrucao;
}

/* Insere uma instrucao ILOC em uma lista de instrucoes ILOC (ou seja, um codigo). */
void insereInstrucao(Codigo **inicio_codigo, Instrucao *instrucao)
{
    Codigo *nova_instrucao = (Codigo*)malloc(sizeof(Codigo));
    if (nova_instrucao == NULL)
        return;
    nova_instrucao->instrucao = instrucao;
    nova_instrucao->proxima_instrucao = NULL;
    if (*inicio_codigo == NULL)
        *inicio_codigo = nova_instrucao;
    else
    {
        Codigo *ultimo = *inicio_codigo;
        while (ultimo->proxima_instrucao != NULL)
            ultimo = ultimo->proxima_instrucao;
        ultimo->proxima_instrucao = nova_instrucao;
    }
}

/* Imprime a operacao e os tres operandos de uma instrucao ILOC, sem nenhuma formatacao. */
void imprimeInstrucaoPadrao (Instrucao *instrucao)
{
    int opr_len = strlen(instrucao->operacao);
    if(instrucao->operacao[opr_len-1] != ':'){
        printf("\t");
    }
    printf("%s", instrucao->operacao);
    if (strcmp(instrucao->operando1, "") != 0)
    {
        printf("\t%s", instrucao->operando1);
    }
    if (strcmp(instrucao->operando2, "") != 0)
    {
        printf(", %s", instrucao->operando2);
    }
    if (strcmp(instrucao->operando3, "") != 0)
    {
        printf(", %s", instrucao->operando3);
    }
    printf("\n");
}

/* Imprime todas as instrucoes ILOC que estao armazenadas em um nodo da arvore AST. */
void imprimeInstrucoesNodo(Nodo *nodo)
{
	if (nodo == NULL)
	{
		printf("LISTA DE INSTRUCOES VAZIA!\n");
		return;
	}
	Codigo *atual = nodo->info->codigo;
	while(atual != NULL)
	{
		imprimeInstrucaoPadrao (atual->instrucao);
		atual = atual->proxima_instrucao;
	}
}

/* Concatena os codigos ILOC presentes em dois nos diferentes da arvore AST. */
Codigo* concatenaCodigo (Codigo *codigo1, Codigo *codigo2)
{
    if (codigo1 == NULL)
        return codigo2;
    else if (codigo2 == NULL)
    	return codigo1;

    Codigo *ultimo = codigo1;
    while (ultimo->proxima_instrucao != NULL)
        ultimo = ultimo->proxima_instrucao;
    ultimo->proxima_instrucao = codigo2;
    return codigo1;
}

/* Acha o deslocamento de um determinado literal ou identificador, na tabela de simbolos. */
int achaDeslocamento(Lista_tabelas *lista_tabelas, char *valor_token)
{
	Lista_tabelas *lista_atual = lista_tabelas;
    	while (lista_atual->proximo != NULL)
       		lista_atual = lista_atual->proximo;

	while (lista_atual != NULL)
	{
        	Tabela *tabela_atual = lista_atual->tabela_simbolos;

		while (tabela_atual != NULL)
		{
		    if (strcmp(valor_token, tabela_atual->info->valor_token) == 0)
			return tabela_atual->info->deslocamento;	
		    tabela_atual = tabela_atual->proximo;
		}

        	lista_atual = lista_atual->anterior;
    	}
}

/* Determina se o escopo atual e global ou local. */
int achaEscopo(Lista_tabelas *lista_tabelas, char *valor_token)
{
	Lista_tabelas *lista_atual = lista_tabelas;
    	while (lista_atual->proximo != NULL)
       		lista_atual = lista_atual->proximo;

	while (lista_atual != NULL)
	{
        	Tabela *tabela_atual = lista_atual->tabela_simbolos;

		while (tabela_atual != NULL)
		{
		    if (strcmp(valor_token, tabela_atual->info->valor_token) == 0)
		    {
		    	if (lista_atual->anterior != NULL)
		    		return ESCOPO_LOCAL;
		    	else
		    		return ESCOPO_GLOBAL;
		    }

		    tabela_atual = tabela_atual->proximo;
		}

        	lista_atual = lista_atual->anterior;
    	}
}

/* Atualiza algum dos dois registrados (rfp ou rbss), depois de verificar se o escopo da execucao e global ou local. */
void atualizaRegistradorEscopo(Lista_tabelas *lista_tabelas, char *registrador_escopo, char *valor_token)
{
	int escopo = achaEscopo(lista_tabelas,valor_token);
	
	if (escopo == ESCOPO_GLOBAL)
		strcpy(registrador_escopo,NOME_REGISTRADOR_GLOBAL);
	else if (escopo == ESCOPO_LOCAL)
		strcpy(registrador_escopo,NOME_REGISTRADOR_LOCAL);
}

/* Cria um linha de codigo contendo apenas um rotulo. */
Instrucao* criaRotulo(int numero_rotulo)
{
    Instrucao* instrucao = (Instrucao*)malloc(sizeof(Instrucao));
    sprintf(instrucao->operacao, ".L%d:", numero_rotulo);
    strcpy(instrucao->operando1, "");
    strcpy(instrucao->operando2, "");
    strcpy(instrucao->operando3, "");
    //imprimeRotulo(numero_rotulo);
    return instrucao;	
}

/* Recebe um numero de rotulo (referente a uma linha do codigo ILOC), e imprime esse rotulo na saida padrao. */
void imprimeRotulo(int numero_rotulo)
{
	printf(".L%d:\n", numero_rotulo);
}

//done
//maybe have to change order
Instrucao* criaInstrucao_jne(int operando3)
{
    Instrucao* instrucao = (Instrucao*)malloc(sizeof(Instrucao));

    strcpy(instrucao->operacao, "jne");
    sprintf(instrucao->operando1, ".L%d", operando3);
    strcpy(instrucao->operando2, "");
    strcpy(instrucao->operando3, "");
    return instrucao;
}

//done
//maybe have to change order
Instrucao* criaInstrucao_je(int operando2)
{
    Instrucao* instrucao = (Instrucao*)malloc(sizeof(Instrucao));

    strcpy(instrucao->operacao, "je");
    sprintf(instrucao->operando1, ".L%d", operando2);
    strcpy(instrucao->operando2, "");
    strcpy(instrucao->operando3, "");
    return instrucao;
}

//removed
/* Cria uma instrucao ILOC do tipo "cbr", e imprime o seu conteudo na saida padrao. */
Instrucao* criaInstrucao_cbr (int operando1, int operando2, int operando3)
{
    Instrucao* instrucao = (Instrucao*)malloc(sizeof(Instrucao));
    strcpy(instrucao->operacao, "cbr");
    sprintf(instrucao->operando1, "r%d", operando1);
    sprintf(instrucao->operando2, ".L%d", operando2);
    sprintf(instrucao->operando3, ".L%d", operando3);
    return instrucao;
}

void imprimeInstrucao_cbr (int operando1, int operando2, int operando3)
{
	printf("cbr\tr%d => .L%d, .L%d\n", operando1, operando2, operando3);	
}

//done
/* Cria uma instrucao ILOC do tipo "jumpI", e imprime o seu conteudo na saida padrao. */
Instrucao* criaInstrucao_jumpI (int operando1)
{
    Instrucao* instrucao = (Instrucao*)malloc(sizeof(Instrucao));
    //strcpy(instrucao->operacao, "jumpI");
    strcpy(instrucao->operacao, "jmp");
    sprintf(instrucao->operando1, ".L%d", operando1);
    strcpy(instrucao->operando2, "");
    strcpy(instrucao->operando3, "");
    //printf("%s\t%s\n", instrucao->operacao, instrucao->operando1);
    return instrucao;
}

void insereInstrucaoInicio(Codigo **inicio_codigo, Instrucao *instrucao)
{
    Codigo *nova_instrucao = (Codigo*)malloc(sizeof(Codigo));
    if (nova_instrucao == NULL)
        return;
    nova_instrucao->instrucao = instrucao;
    nova_instrucao->proxima_instrucao = *inicio_codigo;
    *inicio_codigo = nova_instrucao;
}

void insereInstrucaoReturn(Codigo **codigo){
    Instrucao* instrucao;

    instrucao = (Instrucao*)malloc(sizeof(Instrucao));
    strcpy(instrucao->operacao, "popq");
    sprintf(instrucao->operando1, "%crbp", '%');
    strcpy(instrucao->operando2, "");
    strcpy(instrucao->operando3, "");
    insereInstrucao(codigo, instrucao);

    instrucao = (Instrucao*)malloc(sizeof(Instrucao));
    strcpy(instrucao->operacao, "ret");
    strcpy(instrucao->operando1, "");
    strcpy(instrucao->operando2, "");
    strcpy(instrucao->operando3, "");
    insereInstrucao(codigo, instrucao);
}

/* Recebe uma lista de tabelas de simbolos, e imprime o código assembly necessário para administrar a pilha. */
void defaultFunctionStackManagement(Codigo** codigo)
{
    Instrucao* instrucao;
    //block start

    //printf("\tmovq\t%%rsp, %%rbp\n");
    instrucao = (Instrucao*)malloc(sizeof(Instrucao));
    strcpy(instrucao->operacao, "movq");
    sprintf(instrucao->operando1, "%crsp", '%');
    sprintf(instrucao->operando2, "%crbp", '%');
    strcpy(instrucao->operando3, "");
    insereInstrucaoInicio(codigo, instrucao);

    //printf("\tpushq\t%%rbp\n");
    instrucao = (Instrucao*)malloc(sizeof(Instrucao));
    strcpy(instrucao->operacao, "pushq");
    sprintf(instrucao->operando1, "%crbp", '%');
    strcpy(instrucao->operando2, "");
    strcpy(instrucao->operando3, "");
    insereInstrucaoInicio(codigo, instrucao);
}

void printDataSegment(Tabela *tabela_global){
    Tabela* atual = tabela_global;

	while (atual != NULL)
	{
        if(atual->info->natureza_token == VARIABLE){
            printf("\t.text\n");
            printf("\t.globl\t%s\n", atual->info->valor_token);
            printf("\t.bss\n");
            printf("\t.align 4\n");
            printf("\t.type\t%s, @object\n", atual->info->valor_token);
            printf("\t.size\t%s, 4\n", atual->info->valor_token);
            printf("%s:\n", atual->info->valor_token);
            printf("\t.zero\t4\n");
        }
		atual = atual->proximo;
	}
}

void addFunctionMetaData(Codigo** codigo, char *func_name){
    Instrucao* instrucao;

    //start of procedure

    //printf("%s:\n", func_name);//label
    instrucao = (Instrucao*)malloc(sizeof(Instrucao));
    sprintf(instrucao->operacao, "%s:", func_name);
    strcpy(instrucao->operando1, "");
    strcpy(instrucao->operando2, "");
    strcpy(instrucao->operando3, "");
    insereInstrucaoInicio(codigo, instrucao);

    //printf("\t.type\t%s, @function\n", func_name);//type
    instrucao = (Instrucao*)malloc(sizeof(Instrucao));
    strcpy(instrucao->operacao, ".type");
    strcpy(instrucao->operando1, func_name);
    strcpy(instrucao->operando2, "@function");
    strcpy(instrucao->operando3, "");
    insereInstrucaoInicio(codigo, instrucao);

    //printf("\t.globl\t%s\n", func_name);
    instrucao = (Instrucao*)malloc(sizeof(Instrucao));
    strcpy(instrucao->operacao, ".globl");
    strcpy(instrucao->operando1, func_name);
    strcpy(instrucao->operando2, "");
    strcpy(instrucao->operando3, "");
    insereInstrucaoInicio(codigo, instrucao);

    //printf("\t.text\n");//data block type
    instrucao = (Instrucao*)malloc(sizeof(Instrucao));
    strcpy(instrucao->operacao, ".text");
    strcpy(instrucao->operando1, "");
    strcpy(instrucao->operando2, "");
    strcpy(instrucao->operando3, "");
    insereInstrucaoInicio(codigo, instrucao);

    //printf("\t.cfi_startproc\n");//procedure start for debugging

    //end of procedure

    //printf("\t.cfi_endproc\n");//procedure end for debugging
    //printf("\t.size\t%s, .-%s\n", func_name, func_name);//procedure size
    instrucao = (Instrucao*)malloc(sizeof(Instrucao));
    strcpy(instrucao->operacao, ".size");
    strcpy(instrucao->operando1, func_name);
    sprintf(instrucao->operando2, ".-%s", func_name);
    strcpy(instrucao->operando3, "");
    insereInstrucao(codigo, instrucao);
}

void printProgramStart(Tabela *tabela_global){
    printDataSegment(tabela_global);
}

void printProgramEnd(){
    printf("\t.ident\t\"UFRGS 2023/1 PLAIN AND BASIC COMPILER 1.0\"\n");//compiler identification
}