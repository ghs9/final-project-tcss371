/**
 * util.c - CPU and ALU simulator.
 * TCSS 371, Spring 2016
 * Final Project
 * Viveret Steele (viveret@uw.edu)
 * Amy Irving
 */

#include "util.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned int strx_toi(const char *s) {
    unsigned int r = 0;
    if (*s == '0' && tolower(*(s + 1)) == 'x')
      s += 2;
    else if (*s == 'x')
      s += 1;

    while (*s) {
      r *= 16;
      if (*s >= '0' && *s <= '9')
        r += *s - '0';
      else
        r += tolower(*s) - 'a' + 10;
      s++;
    }
    return r;
}

unsigned int strb_toi(const char *s) {
    unsigned int r = 0;

    while (*s) {
      r = r << 1 | (*s - '0');
      s++;
    }
    return r;
}

unsigned int str_toi(const char *s, int *error) {
  *error = 0;
  switch (s[0]) {
  case '#': return atoi(s + 1);
  case 'b': return strb_toi(s + 1);
  case 'x': return strx_toi(s + 1);
  case 'R': return strx_toi(s + 1);
  default:
    printf("Failed to convert %s to number\n", s);
    return atoi(s + 1);
    *error = 1;
    return -1;
  }
}

#define REG_PF "0x%04X"
#define INT_PF "%4d"

void print_hex(const char *name, unsigned int val) {
  printf("%s:\t", name);
  if (strlen(name) < 8)
    printf("\t");
  printf(REG_PF " (" INT_PF ")\n", val, val);
}

int str_in_array(char *s, char *ar[], int stride, int sz) {
  int j;
  for (j = 0; j < sz; j++) {
    if (strcmp(ar[j * stride], s) == 0) {
      break;
    }
  }

  if (j >= sz) {
    return 0;
  }

  return j + 1;
}
