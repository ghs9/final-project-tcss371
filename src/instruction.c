/**
 * cpu_ir.c - CPU and ALU simulator.
 * TCSS 371, Spring 2016
 * Final Project
 * Viveret Steele (viveret@uw.edu)
 * Amy Irving
 */

#include "instruction.h"
#include "opcodes.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define VAL_NAME_START "IR ["

static void print_opcode_chopped(Instruction i, int locc, int loca[]) {
  char bin_buf[100];
  char *bb_c = bin_buf;
  sprintf(bb_c, VAL_NAME_START);
  bb_c += strlen(VAL_NAME_START);

  int j, k;
  for (j = 15; j >= 0; j--) {
    for (k = 0; k < locc; k++) {
      if (loca[k] == 15 - j) {
        sprintf(bb_c, "|");
        bb_c ++;
      }
    }
    sprintf(bb_c, "%d", (i.val >> j) & 0x1);
    bb_c ++;
  }
  sprintf(bb_c, "]");
  print_hex(bin_buf, i.val);
  print_hex("OpCode", i.opcode.opcode);
}

void instruction_dump(Instruction i) {
  switch (instruction_type(i)) {
  case INS_IMMED5:
    print_opcode_chopped(i, 4, (int []) {4, 7, 10, 11});
    print_hex("RD", i.immed5.rd);
    print_hex("RS", i.immed5.rs);
    print_hex("flag", i.immed5.flag);
    print_hex("immed5", i.immed5.immed);
    break;
  case INS_IMMED6:
    print_opcode_chopped(i, 3, (int []) {4, 7, 10});
    print_hex("RD", i.immed6.rd);
    print_hex("RS", i.immed6.rs);
    print_hex("immed6", i.immed6.immed);
    break;
  case INS_RS2:
    print_opcode_chopped(i, 4, (int []) {4, 7, 10, 12});
    print_hex("RD", i.rs2.rd);
    print_hex("RS", i.rs2.rs);
    print_hex("pad", i.rs2.pad);
    print_hex("RS2", i.rs2.rs2);
    break;
  case INS_BR:
    print_opcode_chopped(i, 4, (int []) {4, 5, 6, 7});
    print_hex("n", i.br.n);
    print_hex("z", i.br.z);
    print_hex("p", i.br.p);
    print_hex("pcoffset9", i.br.pcoffset);
    break;
  case INS_PCOFF9:
    print_opcode_chopped(i, 2, (int []) {4, 7});
    print_hex("r", i.pcoff9.r);
    print_hex("pcoffset9", i.pcoff9.pcoffset);
    break;
  case INS_PCOFF11:
    print_opcode_chopped(i, 2, (int []) {4, 5});
    print_hex("flag", i.pcoff11.flag);
    print_hex("pcoffset11", i.pcoff11.pcoffset);
    break;
  case INS_VECT8:
    print_opcode_chopped(i, 2, (int []) {4, 8});
    print_hex("r", i.vect8.r);
    print_hex("vect8", i.vect8.vect);
    break;
  }
}


static const struct {
  const char *name;
  const Register opcode;
} instructs[] = {
  {INSTRUCT_ADD, OPCODE_ADD},
  /* {INSTRUCT_ADI, OPCODE_ADI}, */
  /* {INSTRUCT_NAND, OPCODE_NAND}, */
  {INSTRUCT_LDI, OPCODE_LDI},
  {INSTRUCT_LD, OPCODE_LD},
  {INSTRUCT_ST, OPCODE_ST}
  /* {INSTRUCT_BRZ, OPCODE_BRZ}, */
  /* {INSTRUCT_HALT, OPCODE_HALT} */};

Register compile_instruction(int argc, char *argv[]) {
  Instruction i;

  unsigned int j;
  for (j = 0; j < sizeof(instructs); j++) {
    if (strcmp(instructs[j].name, argv[0]) == 0) {
      i.opcode.opcode = instructs[j].opcode;
      break;
    }
  }

  if (j == sizeof(instructs)) {
    printf("Invalid operation\n");
    return -1;
  }

  argv ++; argc --;
  /* switch (ir->fmt.opcode) { */
  /* case OPCODE_ADI: case OPCODE_LD: case OPCODE_ST: */
  /*   ir->fmt.immed7 = strx_toi(argv[2]); */
  /* case OPCODE_ADD: case OPCODE_NAND: */
  /*   ir->fmt.rs = strx_toi(argv[1] + 1); */
  /*   ir->fmt.rd = strx_toi(argv[0] + 1); */
  /*   break; */
  /* case OPCODE_LDI: */
  /*   ir->fmt.immed7 = strx_toi(argv[1]); */
  /*   ir->fmt.rd = strx_toi(argv[0] + 1); */
  /*   break; */
  /* case OPCODE_BRZ: */
  /*   ir->fmt.immed7 = strx_toi(argv[0]); */
  /*   break; */
  /* } */

  return i.val;
}


int instruction_type(Instruction i) {
  int t = -1;
  switch (i.opcode.opcode) {
  case OPCODE_ADD: case OPCODE_AND:
    t = i.immed5.flag ? INS_IMMED5 : INS_RS2;
    break;
  case OPCODE_BR:
    t = INS_BR;
    break;
  case OPCODE_LD: case OPCODE_LDI: case OPCODE_LEA:
  case OPCODE_ST: case OPCODE_STI:
    t = INS_PCOFF9;
    break;
  case OPCODE_JMP: case OPCODE_NOT:/* case OPCODE_RET:*/
    t = INS_IMMED5;
    break;
  case OPCODE_JSR:
    t = INS_PCOFF11;
    break;
  case OPCODE_LDR: case OPCODE_STR:
    t = INS_IMMED6;
    break;
  case OPCODE_TRAP:
    t = INS_VECT8;
    break;
  }

  return t;
}
