#ifndef CHIP8_REGISTERS_H
#define CHIP8_REGISTERS_H
#include "chip8_config.h"

typedef struct chip8_registers{

    unsigned char V[CHIP8_REGISTERS_COUNT];
    unsigned short I;
    unsigned char DT; // delay timer
    unsigned char ST; // sound timer
    unsigned short PC; // program counter
    unsigned char SP; // stack pointer

}chip8_registers;


#endif
