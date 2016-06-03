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
#include <signal.h>

int main(int argc, char *argv[]) {
  if (argc > 1) {
    if (argc > 2 && strcmp(argv[1], "-ch") == 0) {
      int err = 0;
      printf("0x%04X, err = %d\n",
             compile_instruction(argc - 2, argv + 2, &err),
             err);
      return 0;
    } else if (argc > 2 && strcmp(argv[1], "-c") == 0) {
      if (argc > 4 && strcmp(argv[3], "-o") == 0)
        return compile(argv[2], argv[4]);
      else
        return compile(argv[2], 0);
    } else {
      signal(SIGINT, controller_signal);
      return controller_driver_prog(argv[1]);
    }
  } else {
    signal(SIGINT, controller_signal);
    return controller_driver_default();
    /* printf("No input specified. For usage, use -h.\n"); */
    /* return 0; */
  }
}
