/**
 * cpu_ir.c - CPU and ALU simulator.
 * TCSS 371, Spring 2016
 * Final Project
 * Viveret Steele (viveret@uw.edu)
 * Amy Irving
 */

#include "cpu_ir.h"
#include "opcodes.h"
#include "instruction.h"
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

  printf("\tOpCode: " REG_PF "\n", j->fmt.opcode);
  printf("\tRD: " REG_PF "\n", ir->fmt.rd);
  printf("\tRS: " REG_PF "\n", ir->fmt.rs);
  printf("\timmed7: " REG_PF "\n", ir->fmt.immed7);
}


static const struct {
  const char *name;
  const Register opcode;
} instructs[] = {
  {INSTRUCT_ADD, OPCODE_ADD},
  {INSTRUCT_ADI, OPCODE_ADI},
  {INSTRUCT_NAND, OPCODE_NAND},
  {INSTRUCT_LDI, OPCODE_LDI},
  {INSTRUCT_LD, OPCODE_LD},
  {INSTRUCT_ST, OPCODE_ST},
  {INSTRUCT_BRZ, OPCODE_BRZ},
  {INSTRUCT_HALT, OPCODE_HALT}};

Register compile_instruction(int argc, char *argv[]) {
  CPU_IR_p ir = malloc_cpu_ir();

  unsigned int i;
  for (i = 0; i < sizeof(instructs); i++) {
    if (strcmp(instructs[i].name, argv[0]) == 0) {
      ir->fmt.opcode = instructs[i].opcode;
      break;
    }
  }

  if (i == sizeof(instructs)) {
    printf("Invalid operation\n");
    return -1;
  }

  argv ++; argc --;
  switch (ir->fmt.opcode) {
  case OPCODE_ADI: case OPCODE_LD: case OPCODE_ST:
    ir->fmt.immed7 = strx_toi(argv[2]);
  case OPCODE_ADD: case OPCODE_NAND:
    ir->fmt.rs = strx_toi(argv[1] + 1);
    ir->fmt.rd = strx_toi(argv[0] + 1);
    break;
  case OPCODE_LDI:
    ir->fmt.immed7 = strx_toi(argv[1]);
    ir->fmt.rd = strx_toi(argv[0] + 1);
    break;
  case OPCODE_BRZ:
    ir->fmt.immed7 = strx_toi(argv[0]);
    break;
  }

  return ir->ir;
}


int instruct_type(Instruction i) {
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
  case OPCODE_JMP: case OPCODE_NOT:// case OPCODE_RET:
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
