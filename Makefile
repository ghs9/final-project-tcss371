# TCSS 371, Spring 2016
# Project 5
# Viveret Steele
# Ver. 2016/6/2, 12:23pm

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
ASMSRCDIR=$(SRCDIR)-asm
SRC=$(wildcard $(SRCDIR)/*.c)
ASMSRC=$(wildcard $(ASMSRCDIR)/*.asm)

# Where object files are stored
OBJDIR := obj
ASMOBJDIR := $(OBJDIR)-asm
$(shell mkdir -p $(OBJDIR) $(ASMOBJDIR))
OBJ=$(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))
ASMOBJ=$(patsubst $(ASMSRCDIR)/%.asm,$(ASMOBJDIR)/%.obj,$(ASMSRC))

# Where dependency files are stored
DEPDIR := $(OBJDIR)
$(shell mkdir -p $(DEPDIR))
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d


compileAll: compileSim compileAsm

compileSim: $(OBJ)
$(PROG_NAME): $(OBJ)
	$(CC) $(OBJ) -o $(PROG_NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPDIR)/%.d
	$(CC) $(DEPFLAGS) $(CFLAGS) -c $< -o $@
	$(POSTCOMPILE)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

compileAsm: $(PROG_NAME) $(ASMOBJ)

$(ASMOBJDIR)/%.obj: $(ASMSRCDIR)/%.asm
	./$(PROG_NAME) -c $< -o $@

clean:
	rm -rf $(DEPDIR) $(OBJDIR) $(ASMOBJDIR)
	rm -f out.out

and-run: compileAll
	./$(PROG_NAME)

and-debug: compileAll
	gdb $(PROG_NAME)


-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS)))
