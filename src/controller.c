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
#define DECODE      1
#define EVAL_ADDR   2
#define FETCH_OP    3
#define EXECUTE     4
#define STORE       5

#define NUM_COL 8
#define NUM_ROW 20
void mem_dump(Memory_p m, int loc);
void controller_menu();

#define OPCODE cpu_get_ir(cpu).opcode.opcode
#define OPTYPE instruction_type(cpu_get_ir(cpu))

#define CTRLR_DONE    0
#define CTRLR_RUNNING 1
#define CTRLR_MENU    2

static int CONTROLLER_STATE = FETCH;
static int CONTROLLER_MAIN_STATE = CTRLR_RUNNING;
static CPU_p cpu;
static Memory_s mem = { 0, 0 };

int controller_fetch();
int controller_decode();
int controller_eval_addr();
int controller_fetch_op();
int controller_execute();
int controller_store();

int controller_main() {
    CONTROLLER_MAIN_STATE = CTRLR_MENU;
    int ret = 0;
    while (CONTROLLER_MAIN_STATE != CTRLR_DONE) {
        if (CONTROLLER_MAIN_STATE == CTRLR_MENU)
            controller_menu();

        switch (CONTROLLER_STATE) {
        case FETCH: // Retrieve IR from memory
            CONTROLLER_STATE = DECODE; // Next step is decode
            if ((ret = controller_fetch()) != 0)
                return ret;
            break;
        case DECODE: // Gets the instruction type and sets the SEXT accordingly.
            CONTROLLER_STATE = EVAL_ADDR; // Next step is to calculate the effective addr
            if ((ret = controller_decode()) != 0)
                return ret;
            break;
        case EVAL_ADDR: // Compute effective address
            CONTROLLER_STATE = FETCH_OP;
            if ((ret = controller_eval_addr()) != 0)
                return ret;
            break;
        case FETCH_OP:
            CONTROLLER_STATE = EXECUTE;
            if ((ret = controller_fetch_op()) != 0)
                return ret;
            break;
        case EXECUTE:
            CONTROLLER_STATE = STORE;
            if ((ret = controller_execute()) != 0)
                return ret;
            break;
        case STORE:
            CONTROLLER_STATE = FETCH;
            if ((ret = controller_store()) != 0)
                return ret;
            break;
        }
    }
    return 0;
}

int controller_fetch() {
    if (cpu_get_pc(cpu) >= mem.size) {
        printf("ERROR: mem[PC] overflow\n");
        mem_dump(&mem, cpu_get_pc(cpu));
        cpu_dump(cpu);
        return -1;
    }

    //Sets MAR to PC.
    cpu_set_mar(cpu, cpu_get_pc(cpu));
    //increment PC
    cpu_set_pc(cpu, cpu_get_pc(cpu) + 1);
    //Set IR to equal mem[MAR]
    cpu_set_ir(cpu, (Instruction) {.val =
                mem.mem[cpu_get_mar(cpu)]});

    return 0;
}

int controller_decode() {
    switch (OPTYPE) {
    case INS_IMMED5:
        //Set SEXT for immed5
        cpu_set_sext(cpu, SEXT_IMMED5(cpu_get_ir(cpu).immed5.immed));
        break;
    case INS_IMMED6:
        //Set SEXT for immed6
        // TODO: Check if offset needs sext
        cpu_set_sext(cpu, SEXT_OFFSET6(cpu_get_ir(cpu).offset6.offset));
        break;
    case INS_RS2:
        //No SEXT for RS
        break;
    case INS_BR:
    case INS_PCOFF9:
        //Set SEXT for PCoffset9
        cpu_set_sext(cpu, SEXT_PCOFF9(cpu_get_ir(cpu).pcoff9.pcoffset));
        break;
    case INS_PCOFF11:
        //Set SEXT for PCoffset11
        cpu_set_sext(cpu, SEXT_PCOFF11(cpu_get_ir(cpu).pcoff11.pcoffset));
        break;
    case INS_VECT8:
        // Set ZEXT (Zero Extension) using the VECT8.
        // AKA: Do nothing, default C behavior.
        break;
    default:
        printf("Should not be in default for DECODE\n");
        break;
    }

    return 0;
}

int controller_eval_addr() {
    switch (OPCODE) {
    case OPCODE_ADD:    // Do nothing
    case OPCODE_AND:    // Do nothing
    case OPCODE_NOT:    // Do nothing
        cpu_set_mar(cpu, cpu_get_pc(cpu));
        break;

    case OPCODE_BR:     // effective_addr = PC + SEXT
    case OPCODE_JSR:    // effective_addr = PC + SEXT
    case OPCODE_LD:     // effective_addr = PC + SEXT
    case OPCODE_LEA:    // effective_addr = PC + SEXT
        cpu_set_mar(cpu, cpu_get_pc(cpu) + cpu_get_sext(cpu));
        break;

    case OPCODE_JMP:    // MAR = BaseR
        cpu_set_mar(cpu, cpu_get_reg(cpu, cpu_get_ir(cpu).immed5.rs));
        break;

    case OPCODE_LDI:    // MAR = mem[effective_addr = PC + SEXT]
        if (cpu_get_pc(cpu) + cpu_get_sext(cpu) >= mem.size) {
            printf("ERROR: mem[PC] overflow\n");
            mem_dump(&mem, cpu_get_pc(cpu));
            cpu_dump(cpu);
            return -1;
        }
        cpu_set_mar(cpu, mem.mem[cpu_get_pc(cpu) + cpu_get_sext(cpu)]);
        break;

    case OPCODE_LDR:    // effective_addr = BaseR + SEXT
        cpu_set_mar(cpu,
                    cpu_get_reg(cpu, cpu_get_ir(cpu).offset6.rs) +
                    cpu_get_sext(cpu));
        break;

    case OPCODE_ST:     // mem[PC + SEXT] = SR
    case OPCODE_STI:    // mem[PC + SEXT] = SR
        cpu_set_mar(cpu, cpu_get_pc(cpu) + cpu_get_sext(cpu));
        break;

    case OPCODE_TRAP:   // MAR = trapVect (ZEXT)
        cpu_set_mar(cpu, cpu_get_ir(cpu).vect8.vect);
        break;

    case OPCODE_STR:    // mem[BaseR + SEXT] = SR
        cpu_set_mar(cpu,
                    cpu_get_reg(cpu, cpu_get_ir(cpu).offset6.rd)
                    + cpu_get_sext(cpu));
        break;
    default:
        printf("Should not be in default for EVAL ADDR\n");
        break;
    }

    return 0;
}

int controller_fetch_op() {
    // get operands out of registers into A, B of ALU
    // or get memory for load instr.
    switch (OPCODE) {
        // Add and And both load the same bit values into the ALU.
    case OPCODE_ADD:
    case OPCODE_AND:
        // If the flag at bit[5] is not zero, then
        // we're adding the SR1 and SEXT (immed5).
        if (cpu_get_ir(cpu).immed5.flag) {
            // get first operand
            cpu_alu_set_a(cpu_get_alu(cpu),
                          cpu_get_reg(cpu, cpu_get_ir(cpu).immed5.rs));
            // get second operand
            cpu_alu_set_b(cpu_get_alu(cpu), cpu_get_sext(cpu));
        } else {
            // we're adding the SR1 and SR2.
            // get first operand
            cpu_alu_set_a(cpu_get_alu(cpu),
                          cpu_get_reg(cpu, cpu_get_ir(cpu).rs2.rs));
            // get second operand
            cpu_alu_set_b(cpu_get_alu(cpu),
                          cpu_get_reg(cpu, cpu_get_ir(cpu).rs2.rs2));
        }
        break;

    case OPCODE_BR:     // MDR = MAR
    case OPCODE_JMP:
    case OPCODE_JSR:
    case OPCODE_LEA:
    case OPCODE_ST:
    case OPCODE_STI:
    case OPCODE_STR:
        cpu_set_mdr(cpu, cpu_get_mar(cpu));
        break;

    case OPCODE_LD:     // MDR = mem[MAR]
    case OPCODE_LDI:
    case OPCODE_LDR:
        if (cpu_get_mar(cpu) >= mem.size) {
            printf("ERROR: mem[PC] overflow\n");
            mem_dump(&mem, cpu_get_pc(cpu));
            cpu_dump(cpu);
            return -1;
        }
        cpu_set_mdr(cpu, mem.mem[cpu_get_mar(cpu)]);
        break;

    case OPCODE_NOT:
        cpu_alu_set_a(cpu_get_alu(cpu),
                      cpu_get_reg(cpu, cpu_get_ir(cpu).immed5.rs));
        break;

    case OPCODE_TRAP:
        // R7 = pc
        cpu_set_reg(cpu, 7, cpu_get_pc(cpu));
        // Pc = mem[trapVect]
        cpu_set_pc(cpu, mem.mem[cpu_get_mar(cpu)]);
        break;
    default:
        printf("Should not be in default for FETCH OP\n");
        break;
    }

    return 0;
}

int controller_execute() {
    switch (OPCODE) {
    case OPCODE_ADD:
        cpu_alu_add(cpu_get_alu(cpu));
        break;

    case OPCODE_AND:
        cpu_alu_and(cpu_get_alu(cpu));
        break;

    case OPCODE_BR:
        // If N, Z, or P = 1, then PC = MDR
        if ((cpu_get_ir(cpu).br.n &&
             IS_REG_NEG(cpu_get_sw(cpu))) ||
            (cpu_get_ir(cpu).br.z &&
             cpu_get_sw(cpu) == 0) ||
            (cpu_get_ir(cpu).br.p &&
             IS_REG_POS(cpu_get_sw(cpu)))) {
            cpu_set_pc(cpu, cpu_get_mdr(cpu));
            printf("n = %d, z = %d, p = %d, sw = %d\n",
                   cpu_get_ir(cpu).br.n,
                   cpu_get_ir(cpu).br.z,
                   cpu_get_ir(cpu).br.p,
                   cpu_get_sw(cpu));
        }
        break;

    case OPCODE_JSR:    // PC = MDR
    case OPCODE_JMP:
        cpu_set_pc(cpu, cpu_get_mdr(cpu));
        break;

    case OPCODE_NOT:
        //printf(REG_PF " NOT = ", cpu_alu_get_a(cpu_get_alu(cpu)));
        cpu_alu_not(cpu_get_alu(cpu));
        //printf(REG_PF "\n", cpu_alu_get_r(cpu_get_alu(cpu)));
        break;

    case OPCODE_LD:
    case OPCODE_LEA:
    case OPCODE_LDI:
    case OPCODE_LDR:
        // do nothing;
        break;

    case OPCODE_ST:     // Memory[MAR] = SR
    case OPCODE_STR:
    case OPCODE_STI:
        mem.mem[cpu_get_mdr(cpu)] =
            cpu_get_reg(cpu, cpu_get_ir(cpu).pcoff9.r);
        break;

    case OPCODE_TRAP:
        switch (cpu_get_pc(cpu)) {
        case 0x20: { /* GETC */
            /* Read a single character from the keyboard. The
             * character is not echoed onto the console. Its
             * ASCII code is copied into R0. The high eight
             * bits of R0 are cleared.
             */
            printf("Input a character> ");
            char c;
            scanf(" %c", &c);
            cpu_set_reg(cpu, 0, c);
        } break;
        case 0x21: /* OUT */
            /* Write a character in R0[7:0] to the console
             * display.
             */
            printf("%c\n", (char) cpu_get_reg(cpu, 0));
            break;
        case 0x22: { /* PUTS */
            /* Write a string of ASCII characters to the
             * console display. The characters are contained
             * in consecutive memory locations, one character
             * per memory location, starting with the address
             * specified in R0. Writing terminates with the
             * occurrence of x0000 in a memory location.
             */
            Register *s = &mem.mem[cpu_get_reg(cpu, 0)];
            while (*(s++))
                printf("%c", *s);
        } break;
        case 0x23: { /* IN */
            /* Print a prompt on the screen and read a single
             * character from the keyboard. The character is
             * echoed onto the console monitor, and its ASCII
             * code is copied into R0. The high eight bits of
             * R0 are cleared.
             */
            printf("Input a character: ");
            char c;
            scanf(" %c", &c);
            cpu_set_reg(cpu, 0, c);
        } break;
        case 0x24: { /* PUTSP */
            /* Write a string of ASCII characters to the
             * console. The characters are contained in
             * consecutive memory locations, two characters
             * per memory location, starting with the address
             * specified in R0. The ASCII code contained in
             * bits [7:0] of a memory location is written to
             * the console first. Then the ASCII code
             * contained in bits [15:8] of that memory
             * location is written to the console. (A
             * character string consisting of an odd number of
             * characters to be written will have x00 in bits
             * [15:8] of the memory location containing the
             * last character to be written.) Writing
             * terminates with the occurrence of x0000 in a
             * memory location.
             */
            char *s = (char *) &mem.mem[cpu_get_reg(cpu, 0)];
            while (*(s++))
                printf("%c", *s);
        } break;
        case 0x25: /* HALT */
            /* Halt execution and print a message on the
             * console.
             */
            printf("--- halting the LC-3 ---\n");
            return 1;
        default:
            printf("Should not be in default for TRAP\n");
            break;
        }
        break;
    default:
        printf("Should not be in default for EXECUTE\n");
        break;
    }

    return 0;
}

int controller_store() {
    switch (OPCODE) {
    case OPCODE_ADD:
    case OPCODE_AND:
    case OPCODE_NOT:
        // We're using the immed5.rd to find the location,
        // it should be the same value as the sr2.rd value.
        cpu_set_reg(cpu, cpu_get_ir(cpu).immed5.rd,
                    cpu_alu_get_r(cpu_get_alu(cpu)));
        cpu_set_sw(cpu, cpu_alu_get_r(cpu_get_alu(cpu)));
        break;

    case OPCODE_TRAP:
        cpu_set_pc(cpu, cpu_get_reg(cpu, 7));
        break;

    case OPCODE_LD:
    case OPCODE_LEA:
    case OPCODE_LDI:
    case OPCODE_LDR:
        cpu_set_reg(cpu, cpu_get_ir(cpu).pcoff9.r,
                    cpu_get_mdr(cpu));
        cpu_set_sw(cpu, cpu_get_mdr(cpu));
        break;

    case OPCODE_BR:     // Do nothing
    case OPCODE_JMP:
    case OPCODE_JSR:
    case OPCODE_ST:
    case OPCODE_STI:
    case OPCODE_STR:
        // TODO: Remove this stub as it is unecessary
        break;

    default:
        printf("Should not be in default for STORE\n");
        break;
    }

    return 0;
}

int controller_main_default() {
    mem.size = DEFAULT_MEM_SIZE;
    mem.mem = calloc(1, DEFAULT_MEM_SIZE);
    apply_os(&mem);
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
    apply_os(&mem);

    // Load program into memory
    ret = fread(&mem.mem[start_addr], prog_size, 1, fin);
    if (!ret) {
        printf("Could not read from program file %s\n", prog_name);
        free(mem.mem);
        fclose(fin);
        return -1;
    }
    fclose(fin);
    swap_endian(&mem.mem[start_addr], sizeof(Register),
                prog_size / sizeof(Register));

    cpu = malloc_cpu();
    cpu_set_pc(cpu, start_addr);
    int r = controller_main();

    free_cpu(cpu);
    free(mem.mem);
    return r;
}

void controller_signal(int v) {
    CONTROLLER_MAIN_STATE = CTRLR_MENU;
}

#define MEM_LOC_BOUND_SZ (8 * 10)
void mem_dump(Memory_p m, int loc) {
    loc -= loc % NUM_COL;
    int r, c, i;
    for (r = -NUM_ROW / 2; r < NUM_ROW / 2; r++) {
        i = r * NUM_COL + loc;
        if (i < 0 || i >= mem.size)
            continue;

        printf(REG_PF ">", i);
        for (c = 0; c < NUM_COL; c++) {
            i = r * NUM_COL + c + loc;
            if (i < 0 || i >= mem.size)
                continue;
            else
                printf(" " REG_PF, m->mem[i]);
        }
        printf("\n");
        if (i >= mem.size)
            break;
    }
    printf("Mem sz: " REG_PF " (" INT_PF " bytes)\n", m->size,
            m->size * (unsigned) sizeof(Register));
}

void controller_menu() {
    printf("--- Paused State: %d ---\n", CONTROLLER_STATE);
    printf("Menu:\nq) Quit\np) Dump all\nu) Dump cpu\nm) Dump memory\nc) Continue\ns) Step\n");
    char c;
    for (;;) {
        if (c != '\n' && c != '\r')
            printf("> ");
        scanf(" %c", &c);

        if (c == 'q') {
            CONTROLLER_MAIN_STATE = CTRLR_DONE;
            return;
        } else if (c == 'u') {
            cpu_dump(cpu);
        } else if (c == 'm') {
            mem_dump(&mem, cpu_get_mar(cpu));
        } else if (c == 'p') {
            mem_dump(&mem, cpu_get_mar(cpu));
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
    CONTROLLER_MAIN_STATE = CTRLR_RUNNING;
}

void apply_os(Memory_p m) {
    // Set trap addrs to same addr
    int i;
    for (i = 0; i < 0xFF; i++)
        m->mem[i] = i;
}
