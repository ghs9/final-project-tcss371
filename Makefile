# TCSS 371, Spring 2016
# Project 5
# Viveret Steele
# Ver. 2016/5/12, 12:13pm


## C Compiler
CC=gcc

## LC3 Assembler locations, first one found is used, ';' separated
LC3AS=../lc3tools/lc3as;

## Compiler options
CFLAGS=-g -Wall -std=c99 -Isrc

## Output program name
PROG_NAME=out.out

# Where source files are located
SRCDIR=src
SRC=$(wildcard $(SRCDIR)/*.c)

# Where object files are stored
OBJDIR := obj
$(shell mkdir -p $(OBJDIR))
OBJ=$(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))

# Where dependency files are stored
DEPDIR := $(OBJDIR)
$(shell mkdir -p $(DEPDIR))
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

ASSEMLY=$(wildcard *.asm)

compileAll: $(OBJ) $(ASSEMBLY)
	$(CC) $(OBJ) -o $(PROG_NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPDIR)/%.d
	$(CC) $(DEPFLAGS) $(CFLAGS) -c $< -o $@
	$(POSTCOMPILE)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

clean:
	rm -rf $(DEPDIR) $(OBJDIR)
	rm -f out.out
	rm -f *.sym
	rm -f *.obj


and-run: compileAll
	./$(PROG_NAME)

and-debug: compileAll
	gdb $(PROG_NAME)


-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS)))
