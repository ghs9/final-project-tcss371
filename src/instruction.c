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

void instruction_dump(Instruction i) {
  printf("Val: " REG_PF " [", i.val);
  int j;
  for (j = 15; j >= 0; j--) {
    if (j == 12 || j == 9 || j == 6)
      printf("|");
    printf("%d", (i.val >> j) & 0x1);
  }
  printf("]\n");
  printf("\tOpCode: " REG_PF "\n", i.opcode.opcode);

  switch (instruction_type(i)) {
  case INS_IMMED5:
    printf("\tRD: " REG_PF "\n", i.immed5.rd);
    printf("\tRS: " REG_PF "\n", i.immed5.rs);
    printf("\tflag: " REG_PF "\n", i.immed5.flag);
    printf("\timmed5: " REG_PF "\n", i.immed5.immed);
    break;
  case INS_IMMED6:
    printf("\tRD: " REG_PF "\n", i.immed6.rd);
    printf("\tRS: " REG_PF "\n", i.immed6.rs);
    printf("\timmed6: " REG_PF "\n", i.immed6.immed);
    break;
  case INS_RS2:
    printf("\tRD: " REG_PF "\n", i.rs2.rd);
    printf("\tRS: " REG_PF "\n", i.rs2.rs);
    printf("\tpad: " REG_PF "\n", i.rs2.pad);
    printf("\tRS2: " REG_PF "\n", i.rs2.rs2);
    break;
  case INS_BR:
    printf("\tn: " REG_PF "\n", i.br.n);
    printf("\tz: " REG_PF "\n", i.br.z);
    printf("\tp: " REG_PF "\n", i.br.p);
    printf("\tpcoffset9: " REG_PF "\n", i.br.pcoffset);
    break;
  case INS_PCOFF9:
    printf("\tr: " REG_PF "\n", i.pcoff9.r);
    printf("\tpcoffset9: " REG_PF "\n", i.pcoff9.pcoffset);
    break;
  case INS_PCOFF11:
    printf("\tflag: " REG_PF "\n", i.pcoff11.flag);
    printf("\tpcoffset11: " REG_PF "\n", i.pcoff11.pcoffset);
    break;
  case INS_VECT8:
    printf("\tr: " REG_PF "\n", i.vect8.r);
    printf("\tvect8: " REG_PF "\n", i.vect8.vect);
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
