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
#include <signal.h>


#define MEM_SIZE 1000

/********    STATES    *******/
#define FETCH       0
#define FETCH_OP    1
#define DECODE      2
#define EVAL_ADDR   3
#define EXECUTE     4
#define STORE       5

#define OPC cpu_get_ir(cpu).opcode.opcode

static int IS_RUNNING = 1;
static CPU_p cpu;
static Memory_s mem;

int controller_main() {
    mem.size = MEM_SIZE;
    mem.mem = calloc(1, MEM_SIZE);
    cpu = malloc_cpu();

    Register branch_taken_addr;

    //Temp value, will get changed later.
    int instruction = 0;
    Instruction i;
    i.val = 0x1163;

    int state = FETCH;
    for (; IS_RUNNING;) {   // efficient endless loop

        switch (state) {
        case FETCH:
            //Sets MAR to PC.
            cpu_set_mar(cpu, cpu_get_pc(cpu));
            //increment PC
            cpu_set_pc(cpu, cpu_get_pc(cpu) + 1);

            //Set IR to equal mem[MAR]
            //temp Instruction value, i, used
            cpu_set_ir(cpu, i);
            //cpu_set_ir(cpu, (Instruction) {.val = 0x1153});

            state = DECODE;
            break;
        case DECODE:

            switch (instruction) {
            case INS_IMMED5:
                //Set SEXT for immed5
                cpu_set_sext(cpu, i.immed5.immed);

                break;
            case INS_IMMED6:
                //Set SEXT for immed6
                cpu_set_sext(cpu, i.offset6.offset);

                break;
            case INS_RS2:
                //No SEXT for RS
                break;
            case INS_BR:
                //Set SEXT for PCoffset9 in br
                cpu_set_sext(cpu, i.br.pcoffset);

                break;
            case INS_PCOFF9:
                //Set SEXT for PCoffset9
                cpu_set_sext(cpu, i.pcoff9.pcoffset);

                break;
            case INS_PCOFF11:
                //Set SEXT for PCoffset11
                cpu_set_sext(cpu, i.pcoff11.pcoffset);

                break;
            case INS_VECT8:
                //Set ZEXT (Zero Extension) using the VECT8
                break;
            }

            state = EVAL_ADDR;
            break;
        case EVAL_ADDR:

            // compute effective address, e.g. add sext(immed7) to register
            switch (OPC) {

            case OPCODE_ADD: // addresses of dest and src registers already available
            case OPCODE_AND: // addresses of dest and src registers already available
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

            state = FETCH;
            break;
        }                // end switch
        cpu_dump(cpu);
    }                // end loop
    return 0;
}

void mem_dump(Memory_p memptr) {
    printf("aaaaa\n");
}

void controller_signal(int v) {
    printf("\n--- Paused CPU ---\n");
    printf("Menu:\nq) Quit\np) Dump all\nc) Dump cpu\nm) Dump memory\n");
    char c = getchar();
    if (c == 'q')
        IS_RUNNING = 0;
    else if (c == 'c') {
        cpu_dump(cpu);
    } else if (c == 'm') {
        mem_dump(&mem);
    } else if (c == 'p') {
        mem_dump(&mem);
        cpu_dump(cpu);
    }
}
