/**
 * main.c - CPU and ALU simulator.
 * TCSS 371, Spring 2016
 * Final Project
 * Viveret Steele (viveret@uw.edu)
 * Amy Irving
 */

#include "cpu_all.h"
#include "controller.h"
#include "compiler.h"
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc > 1) {
    if (strcmp(argv[1], "-ch") == 0) {
      compile_instruction(argc - 2, argv + 2);
      return 0;
    } else if (strcmp(argv[1], "-c") == 0) {
      return compile(argv[2]);
    }
  }

  return controller_main();
}
