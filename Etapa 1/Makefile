# PROJETO DE COMPILADORES - ETAPA 1 

# Andrei Pochmann Koenich - Matrícula 00308680
# Izaias Saturnino de Lima Neto - Matrícula 00326872

# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall

# the build target executable:
TARGET = scanner

all: $(TARGET)

$(TARGET): main.c
	flex scanner.l
	$(CC) main.c lex.yy.c $(CFLAGS) -o etapa1 -lfl

clean:
	$(RM) $(TARGET)
