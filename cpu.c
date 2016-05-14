/**
 * cpu.c - CPU and ALU simulator.
 * TCSS 371, Spring 2016
 * Final Project
 * Viveret Steele (viveret@uw.edu)
 * Amy Irving
 */

#include "cpu.h"
#include <stdio.h>

#define NO_OF_REGISTERS 8

struct cpu_t {
    Register reg_file[NO_OF_REGISTERS]; // register file
    Register pc, sw; // PC and SW (Status Word)
    Register sext; // sext
    CPU_ALU_p alu; // alu
    CPU_IR_p ir;
    int state;
};


CPU_p malloc_cpu() {
    return NULL;
}

void cpu_dump(CPU_p cpu) {
    printf("This is working\nlalala\n");
}
