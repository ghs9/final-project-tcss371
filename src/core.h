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

#define IS_REG_NEG(x) ((x & 0x8000) > 0)
#define IS_REG_POS(x) ((x & 0xEFFF) > 0)

typedef union instruction_t Instruction, *Instruction_p;
typedef struct cpu_alu_t CPU_ALU_s, *CPU_ALU_p;
typedef struct cpu_t CPU_s, *CPU_p;

typedef struct memory_t {
  int size;
  Register *mem;
} Memory_s, *Memory_p;

#endif
