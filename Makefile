# TCSS 371, Spring 2016
# Project 5
# Viveret Steele
# Ver. 2016/5/12, 12:13pm


## C Compiler
CC=gcc

## LC3 Assembler locations, first one found is used, ';' separated
LC3AS=../lc3tools/lc3as;

## Compiler options
CFLAGS=-g -Wall -std=c89

## Output program name
PROG_NAME=out.out

SRC=$(wildcard *.c)
OBJ=$(patsubst %.c,%.o,$(wildcard *.c))
HEADERS=$(wildcard *.h)
ASSEMLY=$(wildcard *.asm)

compileAll: $(OBJ) $(HEADERS) $(ASSEMBLY)
	$(CC) $(OBJ) -o $(PROG_NAME)

%.o: *.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm *.o


and-run: compileAll
	./$(PROG_NAME)

and-debug: compileAll
	gdb
