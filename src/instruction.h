/**
 * instruction.c - CPU and ALU simulator.
 * TCSS 371, Spring 2016
 * Final Project
 * Viveret Steele (viveret@uw.edu)
 * Amy Irving
 */

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "core.h"

#define INS_IMMED5  0
#define INS_IMMED6  1
#define INS_RS2     2
#define INS_BR      3
#define INS_PCOFF9  4
#define INS_PCOFF11 5
#define INS_VECT8   6

typedef union instruction_t {
  Register val;
  struct /* fmt_opcode */ {
    Bit na        : 12;
    Bit opcode    : 4;
  } opcode;
  struct /* fmt_immed5 */ {
    Bit immed     : 5;
    Bit flag      : 1;
    Bit rs        : 3;
    Bit rd        : 3;
    Bit opcode    : 4;
  } immed5;
  struct /* fmt_offset6 */ {
    Bit offset    : 6;
    Bit rs        : 3;
    Bit rd        : 3;
    Bit opcode    : 4;
  } offset6;
  struct /* fmt_rs2 */ {
    Bit rs2       : 3;
    Bit pad       : 2;
    Bit flag      : 1;
    Bit rs        : 3;
    Bit rd        : 3;
    Bit opcode    : 4;
  } rs2;
  struct /* fmt_br */ {
    Bit pcoffset  : 9;
    Bit p         : 1;
    Bit z         : 1;
    Bit n         : 1;
    Bit opcode    : 4;
  } br;
  struct /* fmt_pcoff9 */ {
    Bit pcoffset  : 9;
    Bit r         : 3;
    Bit opcode    : 4;
  } pcoff9;
  struct /* fmt_pcoff11 */ {
    Bit pcoffset  : 11;
    Bit flag      : 1;
    Bit opcode    : 4;
  } pcoff11;
  struct /* fmt_vect8 */ {
    Bit vect      : 8;
    Bit r         : 4;
    Bit opcode    : 4;
  } vect8;
} Instruction, *Instruction_p;

int instruction_type(Instruction i);
void instruction_dump(Instruction i);
Register compile_instruction(int argc, char *argv[], int *error);
int valid_operation(char *s);
int instruction_to_opcode(char *s);

#define SEXTC_IMMED5     0xFFE0
#define SEXTC_OFFSET6    0xFFC0
#define SEXTC_PCOFF9     0xFE00
#define SEXTC_PCOFF11    0xF800

#define SEXT(x, b, c) ((((Register) x) & (1 << (b-1))) ? ((Register) x) | c : x)
#define SEXT_IMMED5(x)  SEXT(x, 5,  SEXTC_IMMED5)
#define SEXT_OFFSET6(x) SEXT(x, 6,  SEXTC_OFFSET6)
#define SEXT_PCOFF9(x)  SEXT(x, 9,  SEXTC_PCOFF9)
#define SEXT_PCOFF11(x) SEXT(x, 11, SEXTC_PCOFF11)

#endif
