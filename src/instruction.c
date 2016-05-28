/**
 * instruction.c - CPU and ALU simulator.
 * TCSS 371, Spring 2016
 * Final Project
 * Viveret Steele (viveret@uw.edu)
 * Amy Irving
 */

#include "instruction.h"
#include "opcodes.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define VAL_NAME_START "IR ["

static void print_opcode_chopped(Instruction i, int locc, int loca[]) {
  char bin_buf[100];
  char *bb_c = bin_buf;
  sprintf(bb_c, VAL_NAME_START);
  bb_c += strlen(VAL_NAME_START);

  int j, k;
  for (j = 15; j >= 0; j--) {
    for (k = 0; k < locc; k++) {
      if (loca[k] == 15 - j) {
        sprintf(bb_c, "|");
        bb_c ++;
      }
    }
    sprintf(bb_c, "%d", (i.val >> j) & 0x1);
    bb_c ++;
  }
  sprintf(bb_c, "]");
  print_hex(bin_buf, i.val);
  print_hex("OpCode", i.opcode.opcode);
}

void instruction_dump(Instruction i) {
  switch (instruction_type(i)) {
  case INS_IMMED5:
    print_opcode_chopped(i, 4, (int []) {4, 7, 10, 11});
    print_hex("RD", i.immed5.rd);
    print_hex("RS", i.immed5.rs);
    print_hex("flag", i.immed5.flag);
    print_hex("immed5", i.immed5.immed);
    break;
  case INS_IMMED6:
    print_opcode_chopped(i, 3, (int []) {4, 7, 10});
    print_hex("RD", i.offset6.rd);
    print_hex("RS", i.offset6.rs);
    print_hex("immed6", i.offset6.offset);
    break;
  case INS_RS2:
    print_opcode_chopped(i, 4, (int []) {4, 7, 10, 13});
    print_hex("RD", i.rs2.rd);
    print_hex("RS", i.rs2.rs);
    print_hex("pad", i.rs2.pad);
    print_hex("RS2", i.rs2.rs2);
    break;
  case INS_BR:
    print_opcode_chopped(i, 4, (int []) {4, 5, 6, 7});
    print_hex("n", i.br.n);
    print_hex("z", i.br.z);
    print_hex("p", i.br.p);
    print_hex("pcoffset9", i.br.pcoffset);
    break;
  case INS_PCOFF9:
    print_opcode_chopped(i, 2, (int []) {4, 7});
    print_hex("r", i.pcoff9.r);
    print_hex("pcoffset9", i.pcoff9.pcoffset);
    break;
  case INS_PCOFF11:
    print_opcode_chopped(i, 2, (int []) {4, 5});
    print_hex("flag", i.pcoff11.flag);
    print_hex("pcoffset11", i.pcoff11.pcoffset);
    break;
  case INS_VECT8:
    print_opcode_chopped(i, 2, (int []) {4, 8});
    print_hex("r", i.vect8.r);
    print_hex("vect8", i.vect8.vect);
    break;
  }
}

int is_br_instruct(char *s);

static const struct {
  const char *name;
  const Register opcode;
} instructs[] = {
  {INSTRUCT_ADD, OPCODE_ADD},
  {INSTRUCT_AND, OPCODE_AND},
  {INSTRUCT_BR, OPCODE_BR},
  {INSTRUCT_JMP, OPCODE_JMP},
  {INSTRUCT_JSR, OPCODE_JSR},
  {INSTRUCT_LD, OPCODE_LD},
  {INSTRUCT_LDR, OPCODE_LDR},
  {INSTRUCT_LEA, OPCODE_LEA},
  {INSTRUCT_NOT, OPCODE_NOT},
  {INSTRUCT_ST, OPCODE_ST},
  {INSTRUCT_STI, OPCODE_STI},
  {INSTRUCT_STR, OPCODE_STR},
  {INSTRUCT_TRAP, OPCODE_TRAP}};

Register compile_instruction(int argc, char *argv[], int *error) {
  Instruction i = {.val = 0};
  i.opcode.opcode = instruction_to_opcode(argv[0]);
  if (i.opcode.opcode < 0) {
    *error = -1;
    return -1;
  }

  /* int j; */
  /* for (j = 0; j < argc; j++) { */
  /*   printf("%s ", argv[j]); */
  /* } */
  /* printf("\n"); */

  argv ++; argc --;
  switch (i.opcode.opcode) {
  case OPCODE_LD: case OPCODE_LDI: case OPCODE_LEA:
  case OPCODE_ST: case OPCODE_STI:
    i.pcoff9.r = str_toi(argv[0], error);
    i.pcoff9.pcoffset = str_toi(argv[1], error);
    break;
  case OPCODE_LDR: case OPCODE_STR:
    i.offset6.rd = str_toi(argv[0], error);
    i.offset6.rs = str_toi(argv[1], error);
    i.offset6.offset = str_toi(argv[2], error);
    break;
  case OPCODE_ADD: case OPCODE_AND:
    i.immed5.rd = str_toi(argv[0], error);
    i.immed5.rs = str_toi(argv[1], error);
    if (argc > 2) {
      if (argv[2][0] == 'R') {
        i.rs2.rs2 = str_toi(argv[2], error);
        i.rs2.pad = 0;
      } else {
        i.immed5.flag = 1;
        i.immed5.immed = str_toi(argv[2], error);
      }
    }
    break;
  case OPCODE_BR:
    i.pcoff9.r = is_br_instruct(argv[-1]) >> 1;
    i.br.pcoffset = str_toi(argv[0], error);
    break;
  case OPCODE_JMP:
    i.immed5.rs = str_toi(argv[0], error);
    break;
  case OPCODE_NOT:
    i.immed5.rd = str_toi(argv[0], error);
    i.immed5.rs = str_toi(argv[1], error);
    i.immed5.flag = 1;
    i.immed5.immed = -1;
    break;
  case OPCODE_TRAP:
    i.vect8.vect = str_toi(argv[0], error);
    break;
  }

  *error = 0;
  return i.val;
}


int instruction_type(Instruction i) {
  int t = -1;
  switch (i.opcode.opcode) {
  case OPCODE_ADD: case OPCODE_AND:
    t = i.immed5.flag ? INS_IMMED5 : INS_RS2;
    break;
  case OPCODE_BR:
    t = INS_BR;
    break;
  case OPCODE_LD: case OPCODE_LDI: case OPCODE_LEA:
  case OPCODE_ST: case OPCODE_STI:
    t = INS_PCOFF9;
    break;
  case OPCODE_JMP: case OPCODE_NOT:/* case OPCODE_RET:*/
    t = INS_IMMED5;
    break;
  case OPCODE_JSR:
    t = INS_PCOFF11;
    break;
  case OPCODE_LDR: case OPCODE_STR:
    t = INS_IMMED6;
    break;
  case OPCODE_TRAP:
    t = INS_VECT8;
    break;
  }

  return t;
}

int valid_operation(char *s) {
  int r = str_in_array(s, (char **) instructs, 2,
                       sizeof(instructs) / sizeof(*instructs));
  return r || is_br_instruct(s);
}

int instruction_to_opcode(char *s) {
  static const unsigned sz = sizeof(instructs) / sizeof(*instructs);
  unsigned j;
  if (is_br_instruct(s)) {
    return OPCODE_BR;
  } else {
    for (j = 0; j < sz; j++) {
      if (strcmp(instructs[j].name, s) == 0) {
        break;
      }
    }
  }

  if (j >= sz) {
    printf("Invalid operation\n");
    return -1;
  }
  return instructs[j].opcode;
}

int is_br_instruct(char *s) {
  int r = 0;
  // B R x x x 0 = 7
  if (strlen(s) >= 2 && s[0] == 'B' && s[1] == 'R') {
    /* printf("%s is a br\n", s); */
    r = 1;
    int i;
    // printf("br + %s\n", s + 2);
    for (i = 0; i < 4 && i + 2 < strlen(s); i++) {
      char c = s[i + 2];
      switch (c) {
      case 'n': r |= 1 << 3; break;
      case 'z': r |= 1 << 2; break;
      case 'p': r |= 1 << 1; break;
      }
    }
  }
  return r;
}
