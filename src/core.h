#ifndef CORE_H
#define CORE_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define REG_PF "0x%04X"

typedef unsigned int Register;
typedef unsigned short Bit;

typedef union cpu_ir_t CPU_IR_s, *CPU_IR_p;
typedef struct cpu_alu_t CPU_ALU_s, *CPU_ALU_p;
typedef struct cpu_t CPU_s, *CPU_p;

typedef struct memory_t {
  unsigned long size;
  Register *mem;
} Memory_s, *Memory_p;

#endif
