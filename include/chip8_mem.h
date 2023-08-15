#ifndef CHIP8_MEM_H
#define CHIP8_MEM_H
#include "chip8_config.h"

typedef struct chip8_memory{

    char memory[CHIP8_MEMORY_SIZE];

}chip8_memory;


void chip8_mem_set(chip8_memory *memory, int index, unsigned char val);
unsigned char chip8_mem_get(chip8_memory *memory, int index);


#endif
