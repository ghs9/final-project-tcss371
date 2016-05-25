/**
 * cpu.c - CPU and ALU simulator.
 * TCSS 371, Spring 2016
 * Final Project
 * Viveret Steele (viveret@uw.edu)
 * Amy Irving
 */

#include "cpu_alu.h"
#include "util.h"

struct cpu_alu_t {
    Register a, b, r;
};

Register cpu_alu_add(CPU_ALU_p alu) {
    return alu-> r = alu->a + alu->b;
}

Register cpu_alu_and(CPU_ALU_p alu) {
    return alu-> r = alu->a & alu->b;
}

Register cpu_alu_not(CPU_ALU_p alu){
    return alu-> r = ~(alu->a);
}

Register cpu_alu_set_a(CPU_ALU_p alu, Register value) {
    return alu->a = value;
}
Register cpu_alu_get_a(CPU_ALU_p alu) {
    return alu->a;
}
Register cpu_alu_set_b(CPU_ALU_p alu, Register value) {
    return alu->b = value;
}
Register cpu_alu_get_b(CPU_ALU_p alu) {
    return alu->b;
}
Register cpu_alu_set_r(CPU_ALU_p alu, Register value) {
    return alu->r = value;
}
Register cpu_alu_get_r(CPU_ALU_p alu) {
    return alu->r;
}

CPU_ALU_p malloc_cpu_alu() {
    return calloc(1, sizeof(CPU_ALU_s));
}

void free_cpu_alu(CPU_ALU_p alu) {
    free(alu);
}

void cpu_alu_dump(CPU_ALU_p alu) {
    printf("ALU: \n");
    print_hex("a", alu->a);
    print_hex("b", alu->b);
    print_hex("r", alu->r);
}
