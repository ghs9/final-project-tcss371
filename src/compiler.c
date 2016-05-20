/**
 * compiler.c - CPU and ALU simulator.
 * TCSS 371, Spring 2016
 * Final Project
 * Viveret Steele (viveret@uw.edu)
 * Amy Irving
 */

#include "compiler.h"
#include "instruction.h"
#include "util.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_IMG_SIZE 10000
#define MAX_LBL_LEN  30
#define MAX_SYMBOLS  15
#define FIN_BUF_SIZE 3000

typedef struct prog_t {
  Register *img;
  int sz;
  struct {
    char name[MAX_LBL_LEN];
    Register addr;
  } symbols[MAX_SYMBOLS];
  int num_symbols;
} Prog_s, *Prog_p;

void skip_whitespace(char **finbf_ptr, char *end);
int scan_file(FILE *fin, Prog_p p, int (*line_call)(int ntoks, char *tokens[], Prog_p p, int effective_line_on));

static const char *directives[] = {
  ".ORIG",
  ".END"
};

int valid_directive(char *s) {
  return str_in_array(s, (char **)directives, 1,
                      sizeof(directives) / sizeof(*directives));
}

int valid_label(char *s) {
  if (valid_operation(s) || valid_directive(s))
    return 0;

  while (*s && isalnum(*s))
    s++;

  return *s == 0;
}

int scan_symbols(int ntoks, char *tokens[], Prog_p p, int effective_line_on) {
  if (ntoks > 0 && valid_label(tokens[0])) {
    if (p->num_symbols > MAX_SYMBOLS) {
      printf("Too many symbols in program (last = %s)\n", tokens[0]);
      return -1;
    }
    strcpy(p->symbols[p->num_symbols].name, tokens[0]);
    p->symbols[p->num_symbols].addr = effective_line_on;
    p->num_symbols++;
  } else {
    printf("Invalid label %s\n", tokens[0]);
    return -1;
  }
  return 0;
}

int create_img(int ntoks, char *tokens[], Prog_p p, int effective_line_on) {
  Instruction ins;
  ins.val = 0;

  if (valid_label(tokens[0])) {
    tokens ++;
    ntoks --;
  }
  if (!valid_operation(tokens[0])) {
    printf("Invalid operation %s\n", tokens[0]);
  }

  int i;
  for (i = 0; i < ntoks; i++) {
      if (!valid_operation(tokens[i]) &&
          !valid_directive(tokens[i]) &&
          valid_label(tokens[i])) {
      }
  }
  return 0;
}

int compile(const char *file_name_in) {
  FILE *fin = fopen(file_name_in, "r");
  if (!fin) {
    printf("Failed to open '%s'\n", file_name_in);
    return -1;
  }

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

  Prog_s p;
  memset(&p, 0, sizeof(p));
  p.img = calloc(1, MAX_IMG_SIZE);

  /* pass 1 - calculate symbol addresses / table */
  scan_file(fin, &p, scan_symbols);

  /* pass 2 - calculate instructions using symbols */
  scan_file(fin, &p, create_img);

  return 0;
}

void skip_whitespace(char **finbf_ptr, char *end) {
  while (*finbf_ptr < end && (**finbf_ptr == ' ' || **finbf_ptr == '\t'))
    (*finbf_ptr) ++;
}

int scan_file(FILE *fin, Prog_p p, int (*line_call)(int ntoks, char *tokens[], Prog_p p, int effective_line_on)) {
  char *fin_buf = malloc(FIN_BUF_SIZE);

  int ret, line_num = 1, effective_line_on = 0;
  while (0 < (ret = fread(fin_buf, 1, FIN_BUF_SIZE - 1, fin))) {
    fin_buf[ret] = 0;
    char *finbf = fin_buf;
    char *last_label = 0;

    while (finbf < fin_buf + ret) {
      char *eol = 0; /* End of line */
      char *eos = 0; /* End of statement */
      eol = strchr(finbf, '\n');
      if (!eol) {
        eol = strchr(finbf, '\r');
        if (!eol) {
          printf("Invalid line ending (or no line ending)\n");
          return -1;
        }
      }
      eos = strchr(finbf, ';');
      if (eos > eol || eos == 0)
        eos = eol;

      char *tokens[8];
      int ntoks = 0;
      if (last_label) {
        tokens[0] = last_label;
        // printf("last label = %s\n", last_label);
        last_label = 0;
        ntoks ++;
      }

      while (finbf < eos) {
        skip_whitespace(&finbf, eos);
        if (finbf >= eos)
          goto lineEnd;

        int in_quote = *finbf == '\"';
        if (in_quote)
          finbf++;

        char *eot = finbf;
        while (isalnum(*eot) || *eot == '.' || *eot == '#' || (in_quote && *eot != '\"'))
          eot ++;
        if (in_quote) {
          finbf--;
          eot++;
        }
        *eot = 0;

        if (strlen(finbf) != 0) {
          if (ntoks >= 8) {
            *eot = ' ';
            printf("too many tokens on line %d in statement %s\n", line_num, finbf);
            return -1;
          }
          tokens[ntoks] = malloc(strlen(finbf) + 1);
          strcpy(tokens[ntoks], finbf);
          ntoks++;
        }
        finbf = eot + 1;
      }
      if (ntoks == 1) {
        last_label = tokens[0];
      } else {
        int line_call_ret = line_call(ntoks, tokens, p, effective_line_on);
        if (line_call_ret != 0) {
          return line_call_ret;
        }
        effective_line_on++;
      }
    lineEnd:
      finbf = eol + 1;
      line_num++;
    }
  }
  rewind(fin);
  return 0;
}
