#ifndef CHIP8_H
#define CHIP8_H

#include <stddef.h>
#include "chip8_config.h"
#include "chip8_mem.h"
#include "chip8_registers.h"
#include "chip8_stack.h"
#include "chip8_keyboard.h"
#include "chip8_display.h"

typedef struct chip8
{

    chip8_memory memory;
    chip8_registers registers;
    chip8_stack stack;
    chip8_keyboard keyboard;
    chip8_display display;

}chip8;

void chip8_init(chip8 *chip8);
void chip8_load(chip8 *chip8, const char *buf, size_t size);
void chip8_exec(chip8 *chip8, unsigned short opcode);
#endif
