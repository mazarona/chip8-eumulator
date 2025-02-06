#ifndef CHIP8_STACK_H
#define CHIP8_STACK_H
#include "chip8_config.h"
#include "chip8_registers.h"

struct chip8;  // forward declare chip8 struct to use in module
typedef struct chip8_stack {
  unsigned short stack[CHIP8_STACK_DEPTH];
} chip8_stack;

void chip8_stack_push(struct chip8 *chip8, unsigned short val);
unsigned short chip8_stack_pop(struct chip8 *chip8);

#endif
