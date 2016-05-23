/**
 * util.h - CPU and ALU simulator.
 * TCSS 371, Spring 2016
 * Final Project
 * Viveret Steele (viveret@uw.edu)
 * Amy Irving
 */

#ifndef UTIL_H
#define UTIL_H

unsigned int strx_toi(const char *s);
unsigned int strb_toi(const char *s);
unsigned int str_toi(const char *s, int *error);

void print_hex(const char *name, unsigned int val);

int str_in_array(char *s, char *ar[], int stride, int sz);

void swap_endian(void *ptr, int sz, int n);

#endif
