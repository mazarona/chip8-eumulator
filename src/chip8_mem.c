#include "chip8_mem.h"
#include "chip8_config.h"
#include <assert.h>

static void chip8_mem_is_in_bounds(int index){
    assert(index >= 0 && index < CHIP8_MEMORY_SIZE);
}

void chip8_mem_set(chip8_memory *memory, int index, unsigned char val) {
    chip8_mem_is_in_bounds(index);
    memory->memory[index] = val;
}

unsigned char chip8_mem_get(chip8_memory *memory, int index) {
    chip8_mem_is_in_bounds(index);
    return memory->memory[index];
}
