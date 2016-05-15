/**
 * cpu.c - CPU and ALU simulator.
 * TCSS 371, Spring 2016
 * Final Project
 * Viveret Steele (viveret@uw.edu)
 * Amy Irving
 */

#include "cpu_alu.h"

struct cpu_alu_t {
  Register a, b, r;
};

CPU_ALU_p malloc_cpu_alu() {
  return calloc(1, sizeof(CPU_ALU_s));
}

void free_cpu_alu(CPU_ALU_p alu) {
  free(alu);
}

void cpu_alu_dump(CPU_ALU_p alu) {
  printf("ALU: \n");
  printf("\ta: " REG_PF "\t(%4hd)\n", alu->a, alu->a);
  printf("\tb: " REG_PF "\t(%4hd)\n", alu->b, alu->b);
  printf("\tr: " REG_PF "\t(%4hd)\n", alu->r, alu->r);
}