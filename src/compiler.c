/**
 * compiler.c - CPU and ALU simulator.
 * TCSS 371, Spring 2016
 * Final Project
 * Viveret Steele (viveret@uw.edu)
 * Amy Irving
 *
 * Currently does not support Windows line endings,
 * does not quite calculate labels correctly,
 * and has a few hacky parts, but its relatively
 * Bug free and translates assembly to the LC3
 * Exactly how 'lc3as' would for simple programs.
 */

#include "compiler.h"
#include "instruction.h"
#include "util.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_IMG_SIZE 1000
#define MAX_LBL_LEN  30
#define MAX_SYMBOLS  15
#define FIN_BUF_SIZE 3000

typedef struct prog_t {
  Register *img;
  int sz;
  int start_addr;
  struct {
    char name[MAX_LBL_LEN];
    Register addr;
  } symbols[MAX_SYMBOLS];
  int num_symbols;
} Prog_s, *Prog_p;

void skip_whitespace(char **finbf_ptr, char *end);
int scan_file(FILE *fin, Prog_p p, int (*line_call)(int ntoks, char *tokens[], Prog_p p, int effective_line_on));
int output_symbols(const char *fname, Prog_p p);


static const char *directives[] = {
  ".ORIG",
  ".FILL",
  ".STRINGZ",
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

int is_symbol(char *name, Prog_p p) {
  int i;
  for (i = 0; i < p->num_symbols; i++) {
    if (strcmp(name, p->symbols[i].name) == 0) {
      return i + 1;
    }
  }

  return 0;
}

int scan_symbols(int ntoks, char *tokens[], Prog_p p, int effective_line_on) {
  if (ntoks == 0)
    return 0;

  if (ntoks > 0 && valid_label(tokens[0])) {
    if (p->num_symbols > MAX_SYMBOLS) {
      printf("Too many labels in program (last = %s)\n", tokens[0]);
      return 1;
    } else if (is_symbol(tokens[0], p)) {
      printf("Reuse of '%s' is not allowed\n", tokens[0]);
      return 1;
    }
    /* printf("Added symbol %s\n", tokens[0]); */
    strcpy(p->symbols[p->num_symbols].name, tokens[0]);
    p->symbols[p->num_symbols].addr = effective_line_on;
    p->num_symbols++;
  }
  return 0;
}

int create_img(int ntoks, char *tokens[], Prog_p p, int effective_line_on) {
  int i;

  if (ntoks == 0)
    return 0;

  if (is_symbol(tokens[0], p)) {
    tokens ++;
    ntoks --;
  }

  int is_directive = valid_directive(tokens[0]);
  int is_operation = valid_operation(tokens[0]);

  if (!is_directive && !is_operation) {
    printf("Invalid token %s\n", tokens[0]);
    return 1;
  }

  if (is_operation) {
    char *buf = 0;

    int j;
    for (i = 0; i < ntoks; i++) {
      if (!valid_operation(tokens[i]) &&
          !valid_directive(tokens[i]) &&
          (j = is_symbol(tokens[i], p))) {
        buf = malloc(10);
        sprintf(buf, "x%04hX", (Register) (p->symbols[j - 1].addr - effective_line_on - 1));
        free(tokens[i]);
        tokens[i] = buf;
      }
    }

    int err;
    Instruction ins = {.val = compile_instruction(ntoks, tokens, &err)};
    if (err)
      return err;

    p->img[p->sz] = ins.val;
    p->sz++;
  } else if (is_directive) {
    is_directive --;
    int err;
    switch (is_directive) {
    case 0: // .orig
      p->start_addr = str_toi(tokens[1], &err);
      if (err)
        return 1;
      p->img[0] = p->start_addr;
      p->sz = 1;
      break;
    case 1: // .fill
      p->img[p->sz] = str_toi(tokens[1], &err);
      if (err)
        return 1;
      p->sz ++;
      break;
    case 2: { // .stringz
      unsigned int i;
      for (i = 1; i < strlen(tokens[1]) - 1; i++) {
        p->img[p->sz] = tokens[1][i];
        p->sz ++;
      }
      p->img[p->sz] = 0;
      p->sz ++;
    }break;
    }
  } else {
    printf("Invalid operand %s\n", tokens[0]);
    return -1;
  }
  return 0;
}

int compile(const char *file_name_in, const char *file_name_out) {
  FILE *fin = fopen(file_name_in, "r");
  FILE *fout = 0;
  char *used_file_name_out = 0;
  if (!fin) {
    printf("Failed to open '%s' for reading\n", file_name_in);
    return -1;
  }

  if (file_name_out) {
    used_file_name_out = malloc(strlen(file_name_out));
    strcpy(used_file_name_out, file_name_out);
  } else {
    char *fname_start_ext = strchr(file_name_in, '.');
    if (fname_start_ext) {
      int fname_end = fname_start_ext - file_name_in;
      used_file_name_out = calloc(1, fname_end + 5);
      strncpy(used_file_name_out, file_name_in, fname_end);
    } else {
      used_file_name_out = malloc(strlen(file_name_in) + 5);
      strcpy(used_file_name_out, file_name_in);
    }
    sprintf(used_file_name_out + strlen(used_file_name_out), ".obj");
  }

  fout = fopen(used_file_name_out, "wb");
  if (!fout) {
    printf("Failed to open '%s' for writing\n", file_name_out);
    return -1;
  }

  Prog_s p;
  memset(&p, 0, sizeof(p));
  p.img = calloc(1, MAX_IMG_SIZE * sizeof(Register));

  int ret;

  /* pass 1 - calculate symbol addresses / table */
  printf("Starting Pass 1...\n");
  ret = scan_file(fin, &p, scan_symbols);
  printf("Pass 1 - %d Error(s)\n", ret);
  if (ret)
    return -1;

  /* pass 2 - calculate instructions using symbols */
  printf("Starting Pass 2...\n");
  ret = scan_file(fin, &p, create_img);
  printf("Pass 2 - %d Error(s)\n", ret);
  if (ret)
    return -1;

  swap_endian(p.img, sizeof(Register), p.sz);
  fwrite(p.img, p.sz * sizeof(*p.img), 1, fout);
  free(p.img);

  output_symbols(used_file_name_out, &p);
  free(used_file_name_out);
  return 0;
}

void skip_whitespace(char **finbf_ptr, char *end) {
  while (*finbf_ptr < end && (**finbf_ptr == ' ' || **finbf_ptr == '\t'))
    (*finbf_ptr) ++;
}

int scan_file(FILE *fin, Prog_p p, int (*line_call)(int ntoks, char *tokens[], Prog_p p, int effective_line_on)) {
  char *fin_buf = malloc(FIN_BUF_SIZE);

  int ret, line_num = 1, effective_line_on = 0, errors = 0, is_buffering = 1;
  while (0 < (ret = fread(fin_buf, 1, FIN_BUF_SIZE - 1, fin)) && is_buffering) {
    fin_buf[ret] = 0;
    char *finbf = fin_buf;
    char *last_label = 0;

    while (finbf < fin_buf + ret && is_buffering) {
      char *eol = 0; /* End of line */
      char *eos = 0; /* End of statement */
      eol = strchr(finbf, '\n');
      if (!eol) {
        eol = strchr(finbf, '\r');
        if (!eol) {
          printf("Invalid line ending (or no line ending)\n");
          errors ++;
          is_buffering = 0;
          continue;
        }
      }
      eos = strchr(finbf, ';');
      if (eos > eol || eos == 0)
        eos = eol;

      char *tokens[8];
      int ntoks = 0;
      if (last_label) {
        tokens[0] = last_label;
        last_label = 0;
        ntoks ++;
      }

      while (finbf < eos) {
        skip_whitespace(&finbf, eos);
        if (finbf >= eos)
          goto compileTokens;

        int in_quote = *finbf == '\"';
        if (in_quote)
          finbf++;

        char *eot = finbf;
        while (isalnum(*eot) || *eot == '.' || *eot == '#' ||
               *eot == '-' || (in_quote && *eot != '\"'))
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
            errors ++;
            goto lineEnd;
          }
          tokens[ntoks] = malloc(strlen(finbf) + 1);
          strcpy(tokens[ntoks], finbf);
          ntoks++;
        }
        finbf = eot + 1;
      }
    compileTokens:
      if (ntoks == 1) {
        last_label = tokens[0];
        ntoks = 0;
      } else if (ntoks > 0) {
        errors += line_call(ntoks, tokens, p, effective_line_on);
        effective_line_on++;
      }
    lineEnd:
      finbf = eol + 1;
      line_num++;
      int i;
      for (i = 0; i < ntoks; i++) {
        if (tokens[i]) {
          free(tokens[i]);
          tokens[i] = (void *)0xDEADBEEF;
        }
      }
      ntoks = 0;
    }
  }
  rewind(fin);
  free(fin_buf);
  return errors;
}

int output_symbols(const char *file_name_in, Prog_p p) {
  char file_name_out[100] = {0};
  char *fname_start_ext = strchr(file_name_in, '.');
  if (fname_start_ext) {
    int fname_end = fname_start_ext - file_name_in;
    strncpy(file_name_out, file_name_in, fname_end);
  } else {
    strcpy(file_name_out, file_name_in);
  }

  sprintf(file_name_out + strlen(file_name_out), ".sym");
  FILE *fout = fopen(file_name_out, "w");
  if (!fout) {
    printf("Failed to open '%s' for writing\n", file_name_out);
    return -1;
  }

  fprintf(fout, "// Symbol table\n// Scope level 0:\n");
  fprintf(fout, "//\tSymbol Name       Page Address\n");
  fprintf(fout, "//\t----------------  ------------\n");
  int i;
  for (i = 0; i < p->num_symbols; i++)
    fprintf(fout, "//\t%-16s  %X\n", p->symbols[i].name,
            p->symbols[i].addr + p->start_addr - 1);

  fclose(fout);
  return 0;
}
