/**
 * compiler.c - CPU and ALU simulator.
 * TCSS 371, Spring 2016
 * Final Project
 * Viveret Steele (viveret@uw.edu)
 * Amy Irving
 */

#include "compiler.h"
#include "instruction.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_IMG_SIZE 10000
#define MAX_LBL_LEN  30
#define MAX_SYMBOLS  15
#define FIN_BUF_SIZE 200

int compile(const char *file_name_in) {
  FILE *fin = fopen(file_name_in, "r");
  if (!fin) {
    printf("Failed to open '%s'\n", file_name_in);
    return -1;
  }
  char fin_buf[FIN_BUF_SIZE];
  char *finbf = fin_buf;

  char file_name_out[100];
  char *fname_start_ext = strchr(file_name_in, '.');
  if (fname_start_ext) {
    int fname_end = fname_start_ext - file_name_in + 1;
    strncpy(file_name_out, file_name_in, fname_end);
  } else {
    strcpy(file_name_out, file_name_in);
  }
  sprintf(file_name_out + strlen(file_name_out), ".o");
  FILE *fout = fopen(file_name_out, "wb");
  if (!fout) {
    printf("Failed to open '%s'\n", file_name_out);
    return -1;
  }

  Register *img = calloc(1, MAX_IMG_SIZE);
  struct {
    char name[MAX_LBL_LEN];
    Register addr;
  } symbols[MAX_SYMBOLS];
  /* pass 1 */

  int ret, line_num = 0;
  while ((ret = fread(fin_buf, FIN_BUF_SIZE, 1, fin)) != 0 &&
         ret != EOF) {
    char *eol = strchr(fin_buf, '\n');
    if (!eol) {
      eol = strchr(fin_buf, ';');
      if (!eol) {
        printf("Invalid line ending (or no line ending)\n");
        return -1;
      }
    }
    int line_len = eol - fin_buf;
    char *line_tokens[5];
    int j, k;
    for (k = 0; k < 5; k++) {
      for (j = 0; j < 5 && j < line_len; j++) {
        if (!isalnum(*finbf))
          break;
        else
          line_tokens[k][j] = *finbf;
        finbf++;
      }
      if (j >= line_len) {
        printf("Line too short, add some code (line %d)\n", line_num);
        return -1;
      }
    }

    Register i = compile_instruction(k, line_tokens);
    /*if ()*/
    line_num++;
  }

  return 0;
}
