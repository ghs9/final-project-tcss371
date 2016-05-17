#include "compiler.h"
#include <stdio.h>
#include <string.h>

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

  return 0;
}
