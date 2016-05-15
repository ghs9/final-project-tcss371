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

#define MEM_SIZE 1000

/********    STATES    *******/
#define FETCH       0
#define FETCH_OP    1
#define DECODE      2
#define EVAL_ADDR   3
#define EXECUTE     4
#define STORE       5

#define OPC cpu_ir_get_opcode(cpu_get_ir(cpu))

int controller_main() {
  Memory_s mem;
  mem.size = MEM_SIZE;
  mem.mem = calloc(1, MEM_SIZE);
  CPU_p cpu = malloc_cpu();
  cpu_dump(cpu);

  int state = FETCH;
  switch(state) {
  case FETCH:break;
  case EXECUTE:
    switch (OPC) {
    case OPCODE_ADD:
      break;
    }
    break;
  case STORE:
    if (!cpu_is_mdr_on(cpu))
      break;

    switch (OPC) {
    case OPCODE_ST:
      break;
    }
  }

  return 0;
}
