/**
 * controller_driver.c - CPU and ALU simulator.
 * TCSS 371, Spring 2016
 * Final Project
 * Viveret Steele (viveret@uw.edu)
 * Amy Irving
 */

#include "controller.h"
#include "cpu.h"
#include "util.h"

#define DEFAULT_MEM_SIZE 0x4000


int controller_driver_default() {
    VM_State_s vms = {malloc_cpu(), { 0, 0 }};

    vms.mem.size = DEFAULT_MEM_SIZE;
    vms.mem.mem = calloc(1, DEFAULT_MEM_SIZE);
    apply_os(&vms.mem);
    vms.cpu = malloc_cpu();
    int r = controller_main(&vms);
    free_cpu(vms.cpu);
    free(vms.mem.mem);
    return r;
}

int controller_driver_prog(const char *prog_name) {
    VM_State_s vms = {malloc_cpu(), { 0, 0 }};

    FILE *fin = fopen(prog_name, "rb");
    if (!fin) {
        printf("Failed to open program file %s\n", prog_name);
        return -1;
    }

    fseek(fin, 0L, SEEK_END);
    int prog_size = ftell(fin) - sizeof(Register);
    vms.mem.size = prog_size / sizeof(Register);
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
    vms.mem.size += start_addr + 1000;
    if (vms.mem.size % 8 != 0)
        vms.mem.size += vms.mem.size % 8;

    vms.mem.mem = calloc(1, vms.mem.size * sizeof(Register));
    if (!vms.mem.mem) {
        printf("Could not malloc mem\n");
        fclose(fin);
        return -1;
    }
    apply_os(&vms.mem);

    // Load program into memory
    ret = fread(&vms.mem.mem[start_addr], prog_size, 1, fin);
    if (!ret) {
        printf("Could not read from program file %s\n", prog_name);
        free(vms.mem.mem);
        fclose(fin);
        return -1;
    }
    fclose(fin);
    swap_endian(&vms.mem.mem[start_addr], sizeof(Register),
                prog_size / sizeof(Register));

    vms.cpu = malloc_cpu();
    cpu_set_pc(vms.cpu, start_addr);
    int r = controller_main(&vms);

    free_cpu(vms.cpu);
    free(vms.mem.mem);
    return r;
}
