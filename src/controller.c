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

    Byte opcode;
    Byte rd, sr1, sr2;
    Byte immed5, immed6;
    Byte PCoffset9, PCoffset11;
    Byte vect8;
    Register branch_taken_addr;

    //Temp value, will get changed later.
    int instruction = 0;

    int state = FETCH;
    switch (state) {
    case FETCH:
        //Sets MAR to PC.
        cpu_set_mar(cpu, cpu_get_pc(cpu));
        //increment PC
        int pc = cpu_get_pc(cpu);
        cpu_set_pc(cpu, pc++);

        //Set IR to equal mem[MAR]
        //cpu_set_ir(cpu, );
        //cpu->ir = mem[cpu->mar];

        state = DECODE;
        break;
    case DECODE:

        switch (instruction) {
        case INS_IMMED5:
            //        opcode = cpu_get_ir(cpu);
            //        rd = getRd(cpu);
            //        sr1 = getRs(cpu);
            //        immed5 = getImmed(cpu);
            //        cpu->sext = sext(immed5);

            break;
        case INS_IMMED6:
            //        opcode = cpu_get_ir(cpu);
            //        rd = getRd(cpu);
            //        sr1 = getRs(cpu);
            //        immed6 = getImmed(cpu);
            //        cpu->sext = sext(immed6);

            break;
        case INS_RS2:
            //        opcode = cpu_get_ir(cpu);
            //        rd = getRd(cpu);
            //        sr1 = getRs(cpu);
            //        sr2

            break;
        case INS_BR:
            //        opcode = cpu_get_ir(cpu);
            //        PCoffset9 =

            break;
        case INS_PCOFF9:
            //        opcode = cpu_get_ir(cpu);
            //        rd = getRd(cpu);
            //        PCoffset9 =

            break;
        case INS_PCOFF11:
            //        opcode = cpu_get_ir(cpu);
            //         PCoffset11 =

            break;
        case INS_VECT8:
            //        opcode = cpu_get_ir(cpu);
            //         vect8

            break;
        }

        state = EVAL_ADDR;
        break;
    case EVAL_ADDR:

        // compute effective address, e.g. add sext(immed7) to register
        switch (OPC) {

        case OPCODE_ADD:     // addresses of dest and src registers already available
        case OPCODE_AND:     // addresses of dest and src registers already available
            break;
        case OPCODE_BR:
            break;
        case OPCODE_JMP:
        case OPCODE_JSR:
        case OPCODE_LD:
            //cpu->mar = cpu->reg_file[rs] + cpu->sext;   // compute effective address
            break;
        case OPCODE_LDI:   // addresses of dest register already available
            break;
        case OPCODE_LDR:
        case OPCODE_LEA:
        case OPCODE_NOT:
            break;
        case OPCODE_ST:
            //cpu->mar = cpu->reg_file[rd] + cpu->sext; // compute effective address
            break;
        case OPCODE_STI:
        case OPCODE_TRAP:
        case OPCODE_STR:
            break;
        }

        state = FETCH_OP;
        break;
    case FETCH_OP:

        // get operands out of registers into A, B of ALU
        // or get memory for load instr.
        switch (OPC) {
        case OPCODE_ADD:
            //pu->alu->a = cpu->reg_file[rd]; // get first operand
            //cpu->alu->b = cpu->reg_file[rs];    // get second operand
            break;
        case OPCODE_AND:
            break;
        case OPCODE_BR:
            break;
        case OPCODE_JMP:
            break;
        case OPCODE_JSR:
            break;
        case OPCODE_LD:
            //cpu->mdr = mem[cpu->mdr];
            break;
        case OPCODE_LDI:
            //cpu->mdr = cpu->sext;
            break;
        case OPCODE_LDR:
        case OPCODE_LEA:
        case OPCODE_NOT:
            break;
        case OPCODE_ST:
            //cpu->mdr = cpu->reg_file[rs];
            break;
        case OPCODE_STI:
        case OPCODE_TRAP:
        case OPCODE_STR:
            break;
        }

        state = EXECUTE;
        break;
    case EXECUTE:
        switch (OPC) {
        case OPCODE_ADD:
            //alu add(), store result in alu_r
            break;
        case OPCODE_AND:
            //alu and(), store result in alu_r
            break;
        case OPCODE_BR:
            break;
        case OPCODE_JMP:
        case OPCODE_JSR:
        case OPCODE_LD:
        case OPCODE_LDI:
        case OPCODE_LDR:
        case OPCODE_LEA:
            break;
        case OPCODE_NOT:
            //alu not(), store result in alu_r
            break;
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
