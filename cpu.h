/**
 * cpu.h - CPU and ALU simulator.
 * TCSS 371, Spring 2016
 * Final Project
 * Viveret Steele (viveret@uw.edu)
 * Amy Irving
 */

#ifndef CPU_H
#define CPU_H

#include "core.h"

CPU_p malloc_cpu();
void free_cpu(CPU_p cpu);

CPU_ALU_p cpu_get_alu(CPU_p cpu);
CPU_IR_p cpu_get_ir(CPU_p cpu);

Register cpu_set_reg(CPU_p cpu, int which);
Register cpu_get_reg(CPU_p cpu, int which);

Register cpu_set_pc(CPU_p cpu);
Register cpu_get_pc(CPU_p cpu);

Register cpu_set_sw(CPU_p cpu);
Register cpu_get_sw(CPU_p cpu);

Register cpu_set_sext(CPU_p cpu);
Register cpu_get_sext(CPU_p cpu);

void cpu_dump(CPU_p cpu);

#endif
