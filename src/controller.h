#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "core.h"

void apply_os(Memory_p m);
int controller_main_default();
int controller_main_prog(const char *prog_name);
void controller_signal(int v);

#endif
