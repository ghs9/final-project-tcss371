#ifndef CORE_H
#define CORE_H

#include <stddef.h>

typedef unsigned int Register;
typedef unsigned char Bit;

typedef struct cpu_ir_t CPU_IR_s, *CPU_IR_p;
typedef struct cpu_alu_t CPU_ALU_s, *CPU_ALU_p;
typedef struct cpu_t CPU_s, *CPU_p;

#endif
