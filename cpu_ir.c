/**
 * cpu_ir.c - CPU and ALU simulator.
 * TCSS 371, Spring 2016
 * Final Project
 * Viveret Steele (viveret@uw.edu)
 * Amy Irving
 */

#include "cpu_ir.h"
#include <stdlib.h>

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
