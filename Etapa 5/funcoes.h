/* PROJETO DE COMPILADORES - ETAPA 4  */

/* Andrei Pochmann Koenich 	 - Matrícula 00308680 */
/* Izaias Saturnino de Lima Neto - Matrícula 00326872 */

#ifndef _FUNCOES_H_
#define _FUNCOES_H_

/* Constantes para definir a natureza de um nodo da AST, ou de um identificador da linguagem. */
#define LITERAL 0
#define VARIABLE 1
#define EXPRESSION_OPERATOR 2
#define LANGUAGE_OPERATOR 3
#define CONTROL 4
#define TYPE 5
#define SYNTAX_TOKEN 6
#define FUNCTION_CALL 7
#define FUNCTION 8

/* Constantes para determinar o numero maximo de caracteres dos identificador e dos literais da linguagem. */
#define MAXIMO_CARACTERES_TIPO 6
#define MAXIMO_CARACTERES_NOME 50
#define MAXIMO_CARACTERES_INT 20
#define MAXIMO_CARACTERES_FLOAT 100

/* Constantes para determinar o tamanho de memoria (em bytes) ocupado por cada tipo de dado. */
#define TAMANHO_MEMORIA_INT 4
#define TAMANHO_MEMORIA_FLOAT 8
#define TAMANHO_MEMORIA_BOOL 1

/* Constantes para associar um valor inteiro a cada tipo de erro semantico. */
#define ERR_UNDECLARED 10
#define ERR_DECLARED 11
#define ERR_VARIABLE 20 
#define ERR_FUNCTION 21
#define ERR_TYPE 22

/* Constantes para associar um valor inteiro a cada um dos tres tipos de dados da linguagem. */
#define INT 0
#define FLOAT 1
#define BOOL 2

#define TAMANHO_NOME_OPERANDO 3
#define TAMANHO_NOME_INSTRUCAO 8

#define NOME_REGISTRADOR_GLOBAL "rbss"
#define NOME_REGISTRADOR_LOCAL "rfp"

#include <string.h>
#include <stdlib.h>

typedef struct 
{
	char operando1[TAMANHO_NOME_OPERANDO];
	char operando2[TAMANHO_NOME_OPERANDO];
	char operando3[TAMANHO_NOME_OPERANDO];
	char operacao[TAMANHO_NOME_INSTRUCAO];
	
} Instrucao;

typedef struct codigo
{
	Instrucao *instrucao;
	struct codigo *proxima_instrucao;

} Codigo;

/* Estrutura contendo as informacoes de cada nodo da AST. */
typedef struct 
{
	char *valor_token;
	
	int linha_token;
	int natureza_token;
	int tipo_token;
	int tamanho_token;
	int deslocamento_memoria;
	
} ValorLexico;

/* Estrutura responsavel por representar um nodo da AST, com suas informacoes e seus filhos. */
typedef struct nodo
{
	ValorLexico *info;
	struct nodo** filho;
	int numeroFilhos;
	Codigo *codigo;
  
} Nodo;

/* Estrutura representando cada entrada de uma tabela, implementada como uma lista simplesmente encadeada. */
typedef struct tabela
{
	ValorLexico *info;
	struct tabela *proximo;

} Tabela;

/* Estrutura representando cada tabela de uma lista de tabelas de simbolos, implementada como uma lista duplamente encadeada. */
typedef struct lista_tabelas
{
	struct lista_tabelas *proximo;
	struct lista_tabelas *anterior;
	Tabela *tabela_simbolos;
	int endereco_atual;

} Lista_tabelas;

/* Funcoes para manipulacoes da arvore de sintaxe abstrata. */

Nodo* criaNodo(ValorLexico* info);
void adicionaNodo(Nodo* pai, Nodo* filho);
void removeNodo(Nodo* node);

/* Funcoes para manipulacoes de listas encadeadas. */

char* concat_call(char* s1);
void concatenate_list(Nodo* list1, Nodo* list2);

/* Funcoes para manipulacoes da tabela de simbolos. */

void insereEntradaTabela (Tabela** tabela, ValorLexico *valor_lexico);
void insereUltimaTabela(Lista_tabelas** lista_tabelas, ValorLexico* valor_lexico);
void popTabela(Lista_tabelas **lista);
void pushTabela(Lista_tabelas** lista, Tabela *nova_tabela);
void destroiTabela(Tabela** tabela);
void destroiListaTabelas(Lista_tabelas** lista_tabelas);
void imprimeTabela(Tabela *tabela);
void imprimeUltimaTabela(Lista_tabelas* lista_tabelas);

/* Funcoes para analises semanticas e verificacao de erros. */

void verificaERR_UNDECLARED_FUNCTION(Lista_tabelas *lista_tabelas, ValorLexico* identificador);
void verificaERR_DECLARED(Lista_tabelas *lista_tabelas, ValorLexico* identificador);
void verificaERR_VARIABLE_UNDECLARED_chamadafuncao(Lista_tabelas *lista_tabelas, char *valor_token, int linha_token);
int infereTipo(int tipo1, int tipo2);
int verificaTipo(char *tipo_token);
int infereTipoExpressao(Nodo *raiz);
int obtemTipo(Lista_tabelas *lista_tabelas, ValorLexico* identificador);
int infereTamanho(int tipo_token);
char* obtemNomeFuncao(char* nomeChamadaFuncao);

/* Funcoes para criacao de instrucoes ILOC (linguagem intermediaria). */

Instrucao* criaInstrucao (char *operando1, char *operando2, char *operando3, char *operacao);
void insereInstrucao(Codigo **inicio_codigo, Instrucao *instrucao);
void atualizaNomeRegistrador(Lista_tabelas *lista_tabelas, char *registrador);
void imprimeInstrucao(Instrucao *instrucao);
void concatenaCodigo (Codigo *codigo1, Codigo *codigo2);

#endif
