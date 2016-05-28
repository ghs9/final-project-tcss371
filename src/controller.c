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

#define CTRLR_DONE    0
#define CTRLR_RUNNING 1
#define CTRLR_MENU    2

static int CONTROLLER_STATE = CTRLR_RUNNING;
static CPU_p cpu;
static Memory_s mem = { 0, 0 };

int controller_main() {
    Register effective_addr = 0;

    Instruction i;
    i.val = 0x1163;

    int state = FETCH;
    while (CONTROLLER_STATE != CTRLR_DONE) {
        if (CONTROLLER_STATE == CTRLR_MENU)
            controller_menu();

        switch (state) {
        case FETCH:

            //Sets MAR to PC.
            cpu_set_mar(cpu, cpu_get_pc(cpu));
            //increment PC
            cpu_set_pc(cpu, cpu_get_pc(cpu) + 1);

            //Set IR to equal mem[MAR]
            i = (Instruction )mem.mem[cpu_get_mar(cpu)];
            cpu_set_ir(cpu, i);

            state = DECODE;
            break;
        case DECODE:

            //Gets the instruction type and sets the SEXT accordingly.
            switch (instruction_type(cpu_get_ir(cpu))) {
            case INS_IMMED5:
                //Set SEXT for immed5
                cpu_set_sext(cpu, cpu_get_ir(cpu).immed5.immed);

                break;
            case INS_IMMED6:
                //Set SEXT for immed6
                cpu_set_sext(cpu, cpu_get_ir(cpu).offset6.offset);

                break;
            case INS_RS2:
                //No SEXT for RS
                break;
            case INS_BR:
                //Set SEXT for PCoffset9 in br
                cpu_set_sext(cpu, cpu_get_ir(cpu).br.pcoffset);

                break;
            case INS_PCOFF9:
                //Set SEXT for PCoffset9
                cpu_set_sext(cpu, cpu_get_ir(cpu).pcoff9.pcoffset);

                break;
            case INS_PCOFF11:
                //Set SEXT for PCoffset11
                cpu_set_sext(cpu, cpu_get_ir(cpu).pcoff11.pcoffset);

                break;
            case INS_VECT8:
                // Set ZEXT (Zero Extension) using the VECT8.
                // Do nothing.
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
            case OPCODE_BR:     //effective_addr = PC + SEXT
            case OPCODE_JSR:
            case OPCODE_LD:
            case OPCODE_LEA:
                effective_addr = (cpu_get_pc(cpu) + cpu_get_sext(cpu));
                cpu_set_mar(cpu, effective_addr);
                break;
            case OPCODE_JMP:    //Mar = BaseR
                cpu_set_mar(cpu, cpu_get_ir(cpu).immed5.rs);
                break;

            case OPCODE_LDI:    //mar = mem[effective_addr = PC + SEXT]
                effective_addr = (cpu_get_pc(cpu) + cpu_get_sext(cpu));
                cpu_set_mar(cpu, mem.mem[effective_addr]);
                break;
            case OPCODE_LDR:    //effective_addr = BaseR + SEXT
                effective_addr =
                        (cpu_get_ir(cpu).offset6.rd + cpu_get_sext(cpu));
                cpu_set_mar(cpu, mem.mem[effective_addr]);
                break;
            case OPCODE_NOT: // addresses of dest and src registers already available
                break;
            case OPCODE_ST: // mem[PC + SEXT] = SR
            case OPCODE_STI:
                effective_addr = (cpu_get_pc(cpu) + cpu_get_sext(cpu));
                cpu_set_mar(cpu, effective_addr);
                break;
            case OPCODE_TRAP:   //mar = trapVect
                cpu_set_mar(cpu, cpu_get_ir(cpu).vect8.vect);
                break;
            case OPCODE_STR:    // mem[BaseR + SEXT] = SR
                effective_addr =
                        (cpu_get_ir(cpu).offset6.rd + cpu_get_sext(cpu));
                cpu_set_mar(cpu, effective_addr);
                break;
            }

            state = FETCH_OP;
            break;
        case FETCH_OP:

            // get operands out of registers into A, B of ALU
            // or get memory for load instr.
            switch (OPC) {
            // Add and And both load the same bit values into the ALU.
            case OPCODE_ADD:
            case OPCODE_AND:

                //If the flag at bit[5] is not zero, then we're adding the SR1 and immed5.
                if (0 != cpu_get_ir(cpu).immed5.flag) {
                    // get first operand
                    cpu_alu_set_a(cpu_get_alu(cpu),
                            cpu_get_reg(cpu, cpu_get_ir(cpu).immed5.rs));
                    // get second operand
                    cpu_alu_set_b(cpu_get_alu(cpu),
                            cpu_get_reg(cpu, cpu_get_ir(cpu).immed5.immed));
                }
                // we're adding the SR1 and SR2.
                else {
                    // get first operand
                    cpu_alu_set_a(cpu_get_alu(cpu),
                            cpu_get_reg(cpu, cpu_get_ir(cpu).rs2.rs));
                    // get second operand
                    cpu_alu_set_a(cpu_get_alu(cpu),
                            cpu_get_reg(cpu, cpu_get_ir(cpu).rs2.rs2));
                }            //end else
                break;
            case OPCODE_BR:     // mdr = mar
            case OPCODE_JMP:
            case OPCODE_JSR:
            case OPCODE_LEA:
            case OPCODE_ST:
            case OPCODE_STI:
            case OPCODE_STR:
                cpu_set_mdr(cpu, cpu_get_mar(cpu));
                break;
            case OPCODE_LD:     //mdr gets mem[cpu- mar]
            case OPCODE_LDI:
            case OPCODE_LDR:
                cpu_set_mdr(cpu, mem.mem[cpu_get_mar(cpu)]);
                break;
            case OPCODE_NOT:
                cpu_alu_set_a(cpu_get_alu(cpu),
                        cpu_get_reg(cpu, cpu_get_ir(cpu).immed5.rs));
                break;
            case OPCODE_TRAP:
                //R7 = pc
                //Pc = mem[trapVect]
                break;
            }

            state = EXECUTE;
            break;
        case EXECUTE:
            switch (OPC) {
            case OPCODE_ADD:
                cpu_alu_add(cpu_get_alu(cpu));
                break;
            case OPCODE_AND:
                cpu_alu_and(cpu_get_alu(cpu));
                break;
            case OPCODE_BR:     //if N,Z, or P = 1, then PC = cpu->mdr
                //If bit 9, 10, or 11 equals one, then Pc = cpu->mdr
                if ((cpu_get_ir(cpu).val & 0x0200)
                        || (cpu_get_ir(cpu).val & 0x0400)
                        || (cpu_get_ir(cpu).val & 0x0600)) {
                    cpu_set_pc(cpu, cpu_get_mdr(cpu));
                }
                break;
            case OPCODE_JSR:    //PC = cpu->mdr
            case OPCODE_LEA:
            case OPCODE_JMP:
                cpu_set_pc(cpu, cpu_get_mdr(cpu));
                break;
            case OPCODE_LD:
            case OPCODE_LDI:
            case OPCODE_LDR:    //Reg[dr] = mdr
                cpu_set_reg(cpu, cpu_get_ir(cpu).pcoff9.r, cpu_get_mdr(cpu));
                break;
            case OPCODE_NOT:
                cpu_alu_not(cpu_get_alu(cpu));
                break;
            case OPCODE_ST:     //Memory [effective address] = SR
            case OPCODE_STR:
            case OPCODE_STI:
                mem.mem[cpu_get_mdr(cpu)] = cpu_get_ir(cpu).pcoff9.r;
                break;
            case OPCODE_TRAP:
                //R7 = pc
                //Pc = mem[trapVect]
                break;

            }                //end switch
            state = STORE;
            break;
        case STORE:
            if (!cpu_is_mdr_on(cpu)) {
                break;
            }

            switch (OPC) {
            case OPCODE_ADD:
            case OPCODE_AND:
            case OPCODE_NOT:
                // We're using the immed5.rd to find the location,
                // it should be the same value as the sr2.rd value.
                cpu_set_reg(cpu, cpu_get_ir(cpu).immed5.rd,
                        cpu_alu_get_r(cpu_get_alu(cpu)));
                break;
            case OPCODE_BR:     //do nothing
            case OPCODE_JMP:
            case OPCODE_JSR:
            case OPCODE_LD:
            case OPCODE_LDI:
            case OPCODE_LDR:
            case OPCODE_LEA:
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
    int r = controller_main();
    free_cpu(cpu);
    free(mem.mem);
    return r;
}

int controller_main_prog(const char *prog_name) {
    FILE *fin = fopen(prog_name, "rb");
    if (!fin) {
        printf("Failed to open program file %s\n", prog_name);
        return -1;
    }

    fseek(fin, 0L, SEEK_END);
    int prog_size = ftell(fin) - sizeof(Register);
    mem.size = prog_size / sizeof(Register);
    rewind(fin);
    /* if (mem.size < DEFAULT_MEM_SIZE) */
    /*     mem.size = DEFAULT_MEM_SIZE; */

    // Find out start address
    Register start_addr;
    size_t ret = fread(&start_addr, sizeof(start_addr), 1, fin);
    if (!ret) {
        printf("Could not read start addr from program file %s\n", prog_name);
        fclose(fin);
        return -1;
    }

    swap_endian(&start_addr, sizeof(start_addr), 1);
    // printf("Start addr = " REG_PF "\n", start_addr);
    mem.size += start_addr + 1000;
    if (mem.size % 8 != 0)
        mem.size += mem.size % 8;

    mem.mem = calloc(1, mem.size * sizeof(Register));
    if (!mem.mem) {
        printf("Could not malloc mem\n");
        fclose(fin);
        return -1;
    }

    // Load program into memory
    ret = fread(&mem.mem[start_addr], prog_size, 1, fin);
    if (!ret) {
        printf("Could not read from program file %s\n", prog_name);
        free(mem.mem);
        fclose(fin);
        return -1;
    }
    fclose(fin);
    swap_endian(mem.mem, sizeof(Register), mem.size);

    cpu = malloc_cpu();
    cpu_set_pc(cpu, start_addr);
    int r = controller_main();

    free_cpu(cpu);
    free(mem.mem);
    return r;
}

void controller_signal(int v) {
    CONTROLLER_STATE = CTRLR_MENU;
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
    printf("\n--- Paused CPU ---\n");
    printf(
            "Menu:\nq) Quit\np) Dump all\nu) Dump cpu\nm) Dump memory\nc) Continue\ns) Step\n");
    char c;
    for (;;) {
        if (c != '\n' && c != '\r')
            printf("> ");
        c = getchar();

        if (c == 'q') {
            CONTROLLER_STATE = CTRLR_DONE;
            return;
        } else if (c == 'u') {
            cpu_dump(cpu);
        } else if (c == 'm') {
            mem_dump(&mem);
        } else if (c == 'p') {
            mem_dump(&mem);
            cpu_dump(cpu);
        } else if (c == 'c') {
            break;
        } else if (c == 's') {
            return;
        } else if (c == '\n' || c == '\r') {
            continue;
        } else {
            printf("Invalid menu selection, please try again.\n");
        }
    }
    CONTROLLER_STATE = CTRLR_RUNNING;
}
