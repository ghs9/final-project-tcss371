#ifndef CPU_IR_H
#define CPU_IR_H

#include "core.h"

CPU_IR_p malloc_cpu_ir();
void free_cpu_ir(CPU_IR_p ir);

Register cpu_ir_set_ir(CPU_IR_p ir, Register val);
Register cpu_ir_get_ir(CPU_IR_p ir);
Register cpu_ir_set_opcode(CPU_IR_p ir, Register val);
Register cpu_ir_get_opcode(CPU_IR_p ir);
Register cpu_ir_set_rd(CPU_IR_p ir, Register val);
Register cpu_ir_get_rd(CPU_IR_p ir);
Register cpu_ir_set_rs(CPU_IR_p ir, Register val);
Register cpu_ir_get_rs(CPU_IR_p ir);
Register cpu_ir_set_immed7(CPU_IR_p ir, Register val);
Register cpu_ir_get_immed7(CPU_IR_p ir);

void cpu_ir_dump(CPU_IR_p ir);
Register compile_instruction(int argc, char *argv[]);

#endif
