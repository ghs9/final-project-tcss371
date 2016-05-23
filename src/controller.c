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
#include "util.h"
#include <signal.h>

#define DEFAULT_MEM_SIZE 0x4000 // take into account +0x3000 offset (.ORIG)

/********    STATES    *******/
#define FETCH       0
#define FETCH_OP    1
#define DECODE      2
#define EVAL_ADDR   3
#define EXECUTE     4
#define STORE       5

#define NUM_COL 8
void mem_dump(Memory_p m);
void controller_menu();

#define OPC cpu_get_ir(cpu).opcode.opcode

static int IS_RUNNING = 1;
static CPU_p cpu;
static Memory_s mem = {0, 0};

int controller_main() {
    Register branch_taken_addr;

    //Temp value, will get changed later.
    int instruction = 0;
    Instruction i;
    i.val = 0x1163;

    int state = FETCH;
    while (IS_RUNNING) {
        controller_menu();

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

                //TEST WHETHER OR NOT BIT[5] = 1 or 0.
//                if (){
//
//                }   else{
//
//                }
//                // get first operand
//                //cpu->alu->a = cpu->reg_file[rd];
//                cpu_alu_set_a(cpu_get_alu(cpu), cpu_get_reg(cpu, cpu_get_ir(cpu).immed5.rd));
//
//                // get second operand
//                //cpu->alu->b = cpu->reg_file[rs];
//                cpu_alu_set_b(cpu_get_alu(cpu), cpu_get_reg(cpu, cpu_get_ir(cpu).immed5.rs));

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
    }                // end loop
    return 0;
}

int controller_main_default() {
    mem.size = DEFAULT_MEM_SIZE;
    mem.mem = calloc(1, DEFAULT_MEM_SIZE);
    cpu = malloc_cpu();
    return controller_main();
}

int controller_main_prog(const char *prog_name) {
    FILE *fin = fopen(prog_name, "rb");
    if (!fin) {
        printf("Failed to open program file %s\n", prog_name);
        return -1;
    }

    int prog_size = -1;
    fseek(fin, 0L, SEEK_END);
    mem.size = (prog_size = ftell(fin) - sizeof(Register)) / sizeof(Register);
    /* if (mem.size < DEFAULT_MEM_SIZE) */
    /*     mem.size = DEFAULT_MEM_SIZE; */
    rewind(fin);

    // Load program into memory
    Register start_addr;
    size_t ret = fread(&start_addr, sizeof(start_addr), 1, fin);
    if (!ret) {
        printf("Could not read start addr from program file %s\n", prog_name);
        return -1;
    }
    swap_endian(&start_addr, sizeof(start_addr), 1);
    printf("Start addr = " REG_PF "\n", start_addr);
    mem.size += start_addr;

    mem.mem = calloc(1, mem.size * sizeof(Register));
    if (!mem.mem) {
        printf("Could not malloc mem\n");
        return -1;
    }
    ret = fread(&mem.mem[start_addr], prog_size, 1, fin);
    if (!ret) {
        printf("Could not read from program file %s\n", prog_name);
        return -1;
    }
    fclose(fin);
    swap_endian(mem.mem, sizeof(Register), mem.size);

    cpu = malloc_cpu();
    cpu_set_pc(cpu, mem.mem[0]);
    return controller_main();
}

void controller_signal(int v) {
    IS_RUNNING = 2;
}

void mem_dump(Memory_p m) {
    unsigned int r, c;
    for (r = 0; r < m->size / NUM_COL; r++) {
        printf(REG_PF ">", r * NUM_COL);
        for (c = 0; c < NUM_COL; c++) {
            printf(" " REG_PF, m->mem[r * NUM_COL + c]);
        }
        printf("\n");
    }
    printf("Mem sz: " REG_PF " (" REG_PF " bytes)\n", m->size,
           m->size * (unsigned) sizeof(Register));
}

void controller_menu() {
    if (IS_RUNNING == 2) {
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
        IS_RUNNING = 1;
    }
}
