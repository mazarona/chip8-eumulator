#include "chip8_mem.h"

#include <assert.h>

#include "chip8.h"
#include "chip8_config.h"

static void chip8_mem_is_in_bounds(int index) {
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

unsigned short chip8_mem_get_opcode(chip8 *chip8) {
  int b1 = chip8_mem_get(&chip8->memory, chip8->registers.PC);
  chip8->registers.PC++;
  int b2 = chip8_mem_get(&chip8->memory, chip8->registers.PC);
  chip8->registers.PC++;
  return b1 << 8 | b2;
}
