/**
 * cpu_alu.h - CPU and ALU simulator.
 * TCSS 371, Spring 2016
 * Final Project
 * Viveret Steele (viveret@uw.edu)
 * Amy Irving
 */

#ifndef CPU_ALU_H
#define CPU_ALU_H

#include "core.h"

CPU_ALU_p malloc_cpu_alu();
void free_cpu_alu(CPU_ALU_p alu);

Register cpu_alu_set_a(CPU_ALU_p alu);
Register cpu_alu_get_a(CPU_ALU_p alu);
Register cpu_alu_set_b(CPU_ALU_p alu);
Register cpu_alu_get_b(CPU_ALU_p alu);
Register cpu_alu_get_r(CPU_ALU_p alu);

#endif
