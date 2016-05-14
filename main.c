/**
 * main.c - CPU and ALU simulator.
 * TCSS 371, Spring 2016
 * Final Project
 * Viveret Steele (viveret@uw.edu)
 * Amy Irving
 */

#include "cpu_all.h"

int main(int argc, char *argv[]) {
  CPU_p cpu = malloc_cpu();
  cpu_dump(cpu);
  return 0;
}
