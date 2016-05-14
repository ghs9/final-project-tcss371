/**
 * util.c - CPU and ALU simulator.
 * TCSS 371, Spring 2016
 * Final Project
 * Viveret Steele (viveret@uw.edu)
 * Amy Irving
 */

#include "util.h"
#include <ctype.h>

unsigned int strx_toi(const char *s) {
    unsigned int r = 0;
    if (*s == '0' && tolower(*(s + 1)) == 'x')
        s += 2;

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
