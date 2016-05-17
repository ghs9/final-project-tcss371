/**
 * opcodes.h - CPU and ALU simulator.
 * TCSS 371, Spring 2016
 * Final Project
 * Viveret Steele (viveret@uw.edu)
 * Amy Irving
 */

#ifndef OPCODES_H
#define OPCODES_H

#define OPCODE_ADD    0x1     /* 0 0 0 1 */
#define OPCODE_AND    0x5     /* 0 1 0 1 */
#define OPCODE_BR     0x0     /* 0 0 0 0 */
#define OPCODE_JMP    0xC     /* 1 1 0 0 */
#define OPCODE_JSR    0x4     /* 0 1 0 0 */
#define OPCODE_LD     0x2     /* 0 0 1 0 */
#define OPCODE_LDI    0xA     /* 1 0 1 0 */
#define OPCODE_LDR    0x6     /* 0 1 1 0 */
#define OPCODE_LEA    0xE     /* 1 1 1 0 */
#define OPCODE_NOT    0x9     /* 1 0 0 1 */
//#define OPCODE_RET    0xC     /* 1 1 0 0 */
#define OPCODE_ST     0x3     /* 0 0 1 1 */
#define OPCODE_STI    0xB     /* 1 0 1 1 */
#define OPCODE_TRAP   0xF     /* 1 1 1 1 */
#define OPCODE_STR    0x7     /* 0 1 1 1 */

#define INSTRUCT_ADD  "ADD"
#define INSTRUCT_AND  "AND"
#define INSTRUCT_BR   "BR"
#define INSTRUCT_JMP  "JMP"
#define INSTRUCT_JSR  "JSR"
#define INSTRUCT_LD   "LD"
#define INSTRUCT_LDI  "LDI"
#define INSTRUCT_LDR  "LDR"
#define INSTRUCT_LEA  "LEA"
#define INSTRUCT_NOT  "NOT"
#define INSTRUCT_ST   "ST"
#define INSTRUCT_STI  "STI"
#define INSTRUCT_TRAP "TRAP"
#define INSTRUCT_STR  "STR"

#define TRAP_GETC     0
#define TRAP_OUT      1
#define TRAP_PUTS     2
#define TRAP_HALT     3

#define INSTRUCT_TRAP_GETC     "GETC"
#define INSTRUCT_TRAP_OUT      "OUT"
#define INSTRUCT_TRAP_PUTS     "PUTS"
#define INSTRUCT_TRAP_HALT     "HALT"

#endif
