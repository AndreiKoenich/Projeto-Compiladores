# PROJETO DE COMPILADORES - ETAPA 2

# Andrei Pochmann Koenich 	 - Matrícula 00308680
# Izaias Saturnino de Lima Neto  - Matrícula 00326872

# Makefile

# Comando para gerar o parser com o Bison
parser: parser.y
	bison -d parser.y

# Comando para gerar o scanner com o Flex
scanner: scanner.l
	flex scanner.l

# Comando para compilar os arquivos gerados pelo Bison e pelo Flex
compile: parser scanner
	gcc -c lex.yy.c parser.tab.c

# Comando para linkar os arquivos compilados e gerar o executável
link: compile
	gcc -o etapa2 main.c parser.tab.o lex.yy.o -lfl


# Comando padrão do Makefile, executa o alvo "run" por padrão
.DEFAULT_GOAL := link

# Comando para limpar os arquivos gerados
clean:
	rm -f lex.yy.c lex.yy.o parser.tab.c parser.tab.o parser.tab.h parser.output
