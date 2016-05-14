/**
 * cpu.c - CPU and ALU simulator.
 * TCSS 371, Spring 2016
 * Final Project
 * Viveret Steele (viveret@uw.edu)
 * Amy Irving
 */

#include "cpu.h"
#include "cpu_ir.h"
#include "cpu_alu.h"
#include <stdio.h>
#include <stdlib.h>

#define NO_OF_REGISTERS 8

struct cpu_t {
  Register reg_file[NO_OF_REGISTERS]; /* register file */
  Register pc, sw; /* PC and SW (Status Word) */
  Register sext; /* sext */
  CPU_ALU_p alu; /* alu */
  CPU_IR_p ir;   /* IR */
  int state;
};


CPU_p malloc_cpu() {
  CPU_p cpu = calloc(1, sizeof(CPU_s));
  cpu->alu = malloc_cpu_alu();
  cpu->ir = malloc_cpu_ir();
  return cpu;
}

void cpu_dump(CPU_p cpu) {
  printf("--------CPU DUMP:-------------\n");
  printf("reg_file: \n");
  int i;
  for (i = 0; i < NO_OF_REGISTERS; i++) {
    printf("\t%2d) " REG_PF "\t(%4hd)\n", i,
           cpu->reg_file[i], cpu->reg_file[i]);
  }

  cpu_ir_dump(cpu->ir);

  printf("SEXT:\t" REG_PF " (%4hd)\n", cpu->sext, cpu->sext);
  printf("PC:\t" REG_PF " (%4hd)\n", cpu->pc, cpu->pc);
  printf("SW:\t" REG_PF " (%4hd)\n", cpu->sw, cpu->sw);

  cpu_alu_dump(cpu->alu);

  /* printf("State:\t0x%hx (%4hd)\n", cpu->state, cpu->state);*/

  printf("--------END OF CPU DUMP-------\n");
}
