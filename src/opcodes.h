/**
 * opcodes.h - CPU and ALU simulator.
 * TCSS 371, Spring 2016
 * Final Project
 * Viveret Steele (viveret@uw.edu)
 * Amy Irving
 */

#ifndef OPCODES_H
#define OPCODES_H

#define OPCODE_ADD  0x0     /* 0 0 0 */
#define OPCODE_ADI  0x1     /* 0 0 1 */
#define OPCODE_NAND 0x2     /* 0 1 0 */
#define OPCODE_LDI  0x3     /* 0 1 1 */
#define OPCODE_LD   0x4     /* 1 0 0 */
#define OPCODE_ST   0x5     /* 1 0 1 */
#define OPCODE_BRZ  0x6     /* 1 1 0 */
#define OPCODE_HALT 0x7     /* 1 1 1 */

#define INSTRUCT_ADD  "ADD"
#define INSTRUCT_ADI  "ADI"
#define INSTRUCT_NAND "NAND"
#define INSTRUCT_LDI  "LDI"
#define INSTRUCT_LD   "LD"
#define INSTRUCT_ST   "ST"
#define INSTRUCT_BRZ  "BRZ"
#define INSTRUCT_HALT "HALT"

#endif
