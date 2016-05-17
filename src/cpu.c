/**
 * cpu.c - CPU and ALU simulator.
 * TCSS 371, Spring 2016
 * Final Project
 * Viveret Steele (viveret@uw.edu)
 * Amy Irving
 */

#include "cpu.h"
#include "instruction.h"
#include "cpu_alu.h"
#include <stdio.h>
#include <stdlib.h>

#define NO_OF_REGISTERS 8

#define SEXT_CONST 0xfc00

struct cpu_t {
  Register reg_file[NO_OF_REGISTERS]; /* register file */
  Register pc, sw; /* PC and SW (Status Word) */
  Register sext; /* sext */
  CPU_ALU_p alu; /* alu */
  Instruction ir;   /* IR */
  Register mar, mdr;
  Bit is_mdr_on;
};


CPU_p malloc_cpu() {
  CPU_p cpu = calloc(1, sizeof(CPU_s));
  cpu->alu = malloc_cpu_alu();
  return cpu;
}

void free_cpu(CPU_p cpu) {
  free_cpu_alu(cpu->alu);
  free(cpu);
}

CPU_ALU_p cpu_get_alu(CPU_p cpu) {
  return cpu->alu;
}

Instruction cpu_set_ir(CPU_p cpu, Instruction i) {
  return cpu->ir = i;
}

Instruction cpu_get_ir(CPU_p cpu) {
  return cpu->ir;
}

Register cpu_set_mdr(CPU_p cpu, Register val) {
  return cpu->mdr = val;
}

Register cpu_get_mdr(CPU_p cpu) {
  return cpu->mdr;
}

Register cpu_set_mar(CPU_p cpu, Register val) {
  return cpu->mar = val;
}

Register cpu_get_mar(CPU_p cpu) {
  return cpu->mar;
}

Bit      cpu_is_mdr_on(CPU_p cpu) {
  return cpu->is_mdr_on;
}

Register cpu_set_reg(CPU_p cpu, Register which, Register val) {
  return cpu->reg_file[which] = val;
}

Register cpu_get_reg(CPU_p cpu, Register which) {
  return cpu->reg_file[which];
}

Register cpu_set_pc(CPU_p cpu, Register val) {
  return cpu->pc = val;
}

Register cpu_get_pc(CPU_p cpu) {
  return cpu->pc;
}

Register cpu_set_sw(CPU_p cpu, Register val) {
  return cpu->sw = val;
}

Register cpu_get_sw(CPU_p cpu) {
  return cpu->sw;
}

Register cpu_set_sext(CPU_p cpu, Register val) {
  return cpu->sext = val;
}

Register cpu_get_sext(CPU_p cpu) {
  return cpu->sext;
}

Register cpu_to_sext(Register immed7) {
  return immed7 | SEXT_CONST;
}

void cpu_dump(CPU_p cpu) {
  printf("--------CPU DUMP:-------------\n");
  printf("reg_file: \n");
  int i;
  for (i = 0; i < NO_OF_REGISTERS; i++) {
    printf("\t%2d) " REG_PF "\t(%4hd)\n", i,
           cpu->reg_file[i], cpu->reg_file[i]);
  }

  instruction_dump(cpu->ir);

  printf("SEXT:\t" REG_PF " (%4hd)\n", cpu->sext, cpu->sext);
  printf("PC:\t" REG_PF " (%4hd)\n", cpu->pc, cpu->pc);
  printf("SW:\t" REG_PF " (%4hd)\n", cpu->sw, cpu->sw);
  printf("MDR (%1d):\t" REG_PF " (%4hd)\n", cpu->is_mdr_on,
         cpu->mdr, cpu->mdr);
  printf("MAR:\t" REG_PF " (%4hd)\n", cpu->mar, cpu->mar);

  cpu_alu_dump(cpu->alu);

  printf("--------END OF CPU DUMP-------\n");
}
