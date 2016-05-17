/**
 * controller.c - CPU and ALU simulator.
 * TCSS 371, Spring 2016
 * Final Project
 * Viveret Steele (viveret@uw.edu)
 * Amy Irving
 */

#include "cpu_all.h"
#include "controller.h"
#include "opcodes.h"

#define MEM_SIZE 1000

/********    STATES    *******/
#define FETCH       0
#define FETCH_OP    1
#define DECODE      2
#define EVAL_ADDR   3
#define EXECUTE     4
#define STORE       5

#define OPC cpu_get_ir(cpu).opcode.opcode

int controller_main() {
    Memory_s mem;
    mem.size = MEM_SIZE;
    mem.mem = calloc(1, MEM_SIZE);
    CPU_p cpu = malloc_cpu();
    cpu_dump(cpu);

    Byte opcode, rd, rs, immed7;
    Register branch_taken_addr;

    int state = FETCH;
    switch (state) {
    case FETCH:
        //Sets MAR to PC.
        cpu_set_mar(cpu, cpu_get_pc(cpu));
        //increment PC
        cpu_set_pc(cpu, cpu_get_pc(cpu)++);

        //Set IR to equal mem[MAR]
        //cpu->ir = mem[cpu->mar];
                state
        = DECODE;
        break;
    case DECODE:

        switch (instruction) {
        case INS_IMMED5:
            //        opcode = cpu_get_ir(cpu);
            //        rd = getRd(cpu);
            //        rs = getRs(cpu);
            //        immed7 = getImmed(cpu);
            //        cpu->sext = sext(immed7);

            break;
        case INS_IMMED6:

            break;
        case INS_RS2:

            break;
        case INS_BR:

            break;
        case INS_PCOFF9:

            break;
        case INS_PCOFF11:

            break;
        case INS_VECT8:

            break;
        }

        state = EVAL_ADDR;
        break;
    case EVAL_ADDR:
        // compute effective address, e.g. add sext(immed7) to register
        switch (OPC) {
        case OPCODE_ADD:
            break;
        case OPCODE_AND:
            break;
        case OPCODE_BR:
            break;
        case OPCODE_JMP:
        case OPCODE_JSR:
        case OPCODE_LD:
        case OPCODE_LDI:
        case OPCODE_LDR:
        case OPCODE_LEA:
        case OPCODE_NOT:
        case OPCODE_ST:
        case OPCODE_STI:
        case OPCODE_TRAP:
        case OPCODE_STR:
            break;
        }

        state = FETCH_OP;
        break;
    case FETCH_OP:

        state = EXECUTE;
        break;
    case EXECUTE:
        switch (OPC) {
        case OPCODE_ADD:
            break;
        case OPCODE_AND:
            break;
        case OPCODE_BR:
            break;
        case OPCODE_JMP:
        case OPCODE_JSR:
        case OPCODE_LD:
        case OPCODE_LDI:
        case OPCODE_LDR:
        case OPCODE_LEA:
        case OPCODE_NOT:
        case OPCODE_ST:
        case OPCODE_STI:
        case OPCODE_TRAP:
        case OPCODE_STR:
            break;
        }
        state = STORE;
        break;
    case STORE:
        if (!cpu_is_mdr_on(cpu)) {
            break;
        }

        switch (OPC) {
        case OPCODE_ADD:
            break;
        case OPCODE_AND:
            break;
        case OPCODE_BR:
            break;
        case OPCODE_JMP:
        case OPCODE_JSR:
        case OPCODE_LD:
        case OPCODE_LDI:
        case OPCODE_LDR:
        case OPCODE_LEA:
        case OPCODE_NOT:
        case OPCODE_ST:
        case OPCODE_STI:
        case OPCODE_TRAP:
        case OPCODE_STR:
            break;
        }

        break;
    }

    return 0;
}
