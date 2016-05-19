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

void signal_callback_handler(int signum) {
  controller_signal(signum);
}

int main(int argc, char *argv[]) {
  signal(SIGINT, signal_callback_handler);
  if (argc > 1) {
    if (strcmp(argv[1], "-ch") == 0) {
      printf("%x\n", compile_instruction(argc - 2, argv + 2));
      return 0;
    } else if (strcmp(argv[1], "-c") == 0) {
      return compile(argv[2]);
    }
  }

  return controller_main();
}
