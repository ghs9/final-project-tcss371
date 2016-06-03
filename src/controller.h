/**
 * controller.h - CPU and ALU simulator.
 * TCSS 371, Spring 2016
 * Final Project
 * Viveret Steele (viveret@uw.edu)
 * Amy Irving
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "core.h"

void apply_os(Memory_p m);
int controller_main(VM_State_p vms);

/*****   Controller drivers   ******/
int controller_driver_default();
int controller_driver_prog(const char *prog_name);
void controller_signal(int v);

#endif
