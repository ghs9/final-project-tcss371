/**
 * main.c - CPU and ALU simulator.
 * TCSS 371, Spring 2016
 * Final Project
 * Viveret Steele (viveret@uw.edu)
 * Amy Irving
 */

#include "cpu_all.h"

#define MEM_SIZE 1000

int main(int argc, char *argv[]) {
  Memory_s mem;
  mem.size = MEM_SIZE;
  mem.mem = calloc(1, MEM_SIZE);
  CPU_p cpu = malloc_cpu(mem);
  cpu_dump(cpu);
  return 0;
}
