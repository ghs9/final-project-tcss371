/**
 * core.h - CPU and ALU simulator.
 * TCSS 371, Spring 2016
 * Final Project
 * Viveret Steele (viveret@uw.edu)
 * Amy Irving
 */

#ifndef CORE_H
#define CORE_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define REG_PF "0x%04hX"
#define INT_PF "%4d"

typedef unsigned short Register;
typedef unsigned short Bit;
typedef unsigned short Byte;

#define IS_REG_NEG(x) ((x & 0x8000))
#define IS_REG_POS(x) ((x & 0x7FFF))

typedef union instruction_t Instruction, *Instruction_p;
typedef struct cpu_alu_t CPU_ALU_s, *CPU_ALU_p;
typedef struct cpu_t CPU_s, *CPU_p;

typedef struct memory_t {
  int size;
  Register *mem;
} Memory_s, *Memory_p;

typedef struct vm_state_t {
  CPU_p cpu;
  Memory_s mem;
} VM_State_s, *VM_State_p;

#endif
