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
  struct /* fmt_immed6 */ {
    Bit immed     : 6;
    Bit rs        : 3;
    Bit rd        : 3;
    Bit opcode    : 4;
  } immed6;
  struct /* fmt_rs2 */ {
    Bit rs2       : 3;
    Bit pad       : 2;
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

int instruct_type(Instruction i);

#endif
