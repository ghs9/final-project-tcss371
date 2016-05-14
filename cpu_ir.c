/**
 * cpu_ir.c - CPU and ALU simulator.
 * TCSS 371, Spring 2016
 * Final Project
 * Viveret Steele (viveret@uw.edu)
 * Amy Irving
 */

#include "cpu_ir.h"
#include "opcodes.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

union cpu_ir_t {
  Register ir;
  struct /* fmt */ {
    Bit immed7    : 7;
    Bit rs        : 3;
    Bit rd        : 3;
    Bit opcode    : 3;
  } fmt;
};


CPU_IR_p malloc_cpu_ir() {
  return (CPU_IR_p) calloc(1, sizeof(CPU_IR_s));
}

void free_cpu_ir(CPU_IR_p ir) {
  free(ir);
}

Register cpu_ir_set_ir(CPU_IR_p ir, Register val) {
  return ir->ir = val;
}

Register cpu_ir_get_ir(CPU_IR_p ir) {
  return ir->ir;
}

Register cpu_ir_set_opcode(CPU_IR_p ir, Register val) {
  return ir->fmt.opcode = val;
}

Register cpu_ir_get_opcode(CPU_IR_p ir) {
  return ir->fmt.opcode;
}

Register cpu_ir_set_rd(CPU_IR_p ir, Register val) {
  return ir->fmt.rd = val;
}

Register cpu_ir_get_rd(CPU_IR_p ir) {
  return ir->fmt.rd;
}

Register cpu_ir_set_rs(CPU_IR_p ir, Register val) {
  return ir->fmt.rs = val;
}

Register cpu_ir_get_rs(CPU_IR_p ir) {
  return ir->fmt.rs;
}

Register cpu_ir_set_immed7(CPU_IR_p ir, Register val) {
  return ir->fmt.immed7 = val;
}

Register cpu_ir_get_immed7(CPU_IR_p ir) {
  return ir->fmt.immed7;
}

void cpu_ir_dump(CPU_IR_p ir) {
  printf("IR: " REG_PF " [", ir->ir);
  int i;
  for (i = 15; i >= 0; i--) {
    if (i == 12 || i == 9 || i == 6)
      printf("|");
    printf("%d", (ir->ir >> i) & 0x1);
  }
  printf("]\n");

  printf("\tOpCode: " REG_PF "\n", ir->fmt.opcode);
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
