#include "chip8.h"

#include <assert.h>
#include <memory.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "SDL.h"
#include "SDL_events.h"
#include "chip8_config.h"
#include "chip8_display.h"
#include "chip8_keyboard.h"
#include "chip8_mem.h"
#include "chip8_stack.h"

const char chip8_default_cs[] = {
    0xf0, 0x90, 0x90, 0x90, 0xf0, 0x20, 0x60, 0x20, 0x20, 0x70, 0xf0, 0x10,
    0xf0, 0x80, 0xf0, 0xf0, 0x10, 0xf0, 0x10, 0xf0, 0x90, 0x90, 0xf0, 0x10,
    0x10, 0xf0, 0x80, 0xf0, 0x10, 0xf0, 0xf0, 0x80, 0xf0, 0x90, 0xf0, 0xf0,
    0x10, 0x20, 0x40, 0x40, 0xf0, 0x90, 0xf0, 0x90, 0xf0, 0xf0, 0x90, 0xf0,
    0x10, 0xf0, 0xf0, 0x90, 0xf0, 0x90, 0x90, 0xe0, 0x90, 0xe0, 0x90, 0xe0,
    0xf0, 0x80, 0x80, 0x80, 0xf0, 0xe0, 0x90, 0x90, 0x90, 0xe0, 0xf0, 0x80,
    0xf0, 0x80, 0xf0, 0xf0, 0x80, 0xf0, 0x80, 0x80};

void chip8_init(chip8 *chip8) {
  memset(chip8, 0, sizeof(struct chip8));
  memcpy(&chip8->memory.memory, chip8_default_cs, sizeof(chip8_default_cs));
}

void chip8_load(chip8 *chip8, const char *buf, size_t size) {
  assert(CHIP8_PROGRAM_LOAD_ADDRESS + size < CHIP8_MEMORY_SIZE);
  memcpy(&chip8->memory.memory[CHIP8_PROGRAM_LOAD_ADDRESS], buf, size);
  chip8->registers.PC = CHIP8_PROGRAM_LOAD_ADDRESS;
}

static char chip8_wait_for_kp(chip8 *chip8) {
  SDL_Event event;
  while (SDL_WaitEvent(&event)) {
    if (event.type != SDL_KEYDOWN) continue;

    char c = event.key.keysym.sym;
    char chip8_key = chip8_keyboard_map(&chip8->keyboard, c);
    if (chip8_key != -1) return chip8_key;
  }
  return -1;
}

static void chip8_exec_ext(chip8 *chip8, unsigned short opcode) {
  unsigned short nnn = opcode & 0x0fff;
  unsigned char n = opcode & 0x000f;
  unsigned char x = (opcode >> 8) & 0x000f;
  unsigned char y = (opcode >> 4) & 0x000f;
  unsigned char kk = opcode & 0x00ff;
  unsigned short alures = 0;
  switch (opcode & 0xf000) {
    // jump to location nnn
    case 0x1000:
      chip8->registers.PC = nnn;
      break;
    // call subroutine at location nnn
    case 0x2000:
      chip8_stack_push(chip8, chip8->registers.PC);
      chip8->registers.PC = nnn;
      break;
    // skip next instruction if Vx==kk
    case 0x3000:
      if (chip8->registers.V[x] == kk) chip8->registers.PC += 2;
      break;
    // skip next instruction if Vx!=kk
    case 0x4000:
      if (chip8->registers.V[x] != kk) chip8->registers.PC += 2;
      break;
    // skip next instruction if Vx == Vy
    case 0x5000:
      if (chip8->registers.V[x] == chip8->registers.V[y])
        chip8->registers.PC += 2;
      break;
    // set Vx = kk;
    case 0x6000:
      chip8->registers.V[x] = kk;
      break;
    // set Vx = Vx + kk
    case 0x7000:
      chip8->registers.V[x] += kk;
      break;
    case 0x8000: {
      switch (opcode & 0x000f) {
        // set Vx = Vy
        case 0x0:
          chip8->registers.V[x] = chip8->registers.V[y];
          break;
        // Set Vx = Vx OR Vy
        case 0x1:
          chip8->registers.V[x] |= chip8->registers.V[y];
          break;
        // Set Vx = Vx AND Vy
        case 0x2:
          chip8->registers.V[x] &= chip8->registers.V[y];
          break;
        // Set Vx = Vx XOR Vy
        case 0x3:
          chip8->registers.V[x] ^= chip8->registers.V[y];
          break;
        // Set Vx = Vx + Vy, Set VF = carry
        case 0x4:
          alures = chip8->registers.V[x] + chip8->registers.V[y];
          chip8->registers.V[0xf] = alures > 0xff;
          chip8->registers.V[x] = alures & 0xff;
          break;
        // Set Vx = Vx - Vy, Set VF = NOT borrow
        case 0x5:
          chip8->registers.V[0xf] =
              chip8->registers.V[x] > chip8->registers.V[y];
          chip8->registers.V[x] -= chip8->registers.V[y];
          break;
        // shift Vx to right by 1, set VF = 1 if LSB of Vx is 1
        case 0x6:
          chip8->registers.V[0xf] = chip8->registers.V[x] & 0x01;
          chip8->registers.V[x] /= 2;
          break;
        // Set Vx = Vy - Vx, Set VF = NOT borrow
        case 0x7:
          chip8->registers.V[0xf] =
              chip8->registers.V[y] > chip8->registers.V[x];
          chip8->registers.V[x] = chip8->registers.V[y] - chip8->registers.V[x];
          break;
        // shift Vx to left by 1, set VF = 1 if MSB of Vx is 1
        case 0xe:
          chip8->registers.V[0xf] = (chip8->registers.V[x] >> 7) & 1;
          chip8->registers.V[x] *= 2;
          break;
      }
    } break;
    // skip next insruction if Vx != Vy
    case 0x9000:
      if (chip8->registers.V[x] != chip8->registers.V[y])
        chip8->registers.PC += 2;
      break;
    // set I register to nnn
    case 0xa000:
      chip8->registers.I = nnn;
      break;
    // jump to location nnn + V0
    case 0xb000:
      chip8->registers.PC = chip8->registers.V[0x0] + nnn;
      break;
    // Set Vx = random byte AND kk
    case 0xc000:
      srand(time(0));
      chip8->registers.V[x] = (rand() % 256) & kk;
      break;
    // Draw n-byte sprite starting at memeory location I at (Vx, Vy), set VG =
    // collision.
    case 0xd000: {
      const char *address = &chip8->memory.memory[chip8->registers.I];
      chip8->registers.V[0xf] =
          chip8_dipslay_draw_sprite(&chip8->display, chip8->registers.V[x],
                                    chip8->registers.V[y], address, n);
    } break;
    case 0xe000: {
      switch (opcode & 0x00ff) {
        // skip the next instruction if Vx key is pressed
        case 0x9e:
          if (chip8_keyboard_is_down(&chip8->keyboard, chip8->registers.V[x]))
            chip8->registers.PC += 2;
          break;
        // skip the next instruction if Vx key is not pressed
        case 0xa1:
          if (!chip8_keyboard_is_down(&chip8->keyboard, chip8->registers.V[x]))
            chip8->registers.PC += 2;
          break;
      }
    } break;

    case 0xf000: {
      switch (opcode & 0x00ff) {
        // Set Vx = DT.
        case 0x07:
          chip8->registers.V[x] = chip8->registers.DT;
          break;
        // wait for a key press, store the value of the key in Vx.
        case 0x0a:
          chip8->registers.V[x] = chip8_wait_for_kp(chip8);
          break;
        // Set DT = Vx
        case 0x15:
          chip8->registers.DT = chip8->registers.V[x];
          break;
        // Set ST = Vx
        case 0x18:
          chip8->registers.ST = chip8->registers.V[x];
          break;
        // Set I = I + Vx
        case 0x1e:
          chip8->registers.I += chip8->registers.V[x];
          break;
        // Set I = location of sprite for digit Vx
        case 0x29:
          chip8->registers.I = chip8->registers.V[x] * CHIP8_CS_HEIGHT;
          break;
        // store BCD representation of Vx in memory location I, I+1, I+2.
        case 0x33: {
          unsigned char h = chip8->registers.V[x] / 100;
          unsigned char t = chip8->registers.V[x] / 10 % 10;
          unsigned char u = chip8->registers.V[x] % 10;
          chip8_mem_set(&chip8->memory, chip8->registers.I, h);
          chip8_mem_set(&chip8->memory, chip8->registers.I + 1, t);
          chip8_mem_set(&chip8->memory, chip8->registers.I + 2, u);
        } break;
        // store registers from V0 to Vx in memory location starting from I
        case 0x55: {
          for (int i = 0; i <= x; i++)
            chip8_mem_set(&chip8->memory, chip8->registers.I + i,
                          chip8->registers.V[i]);
        } break;
        // Read registers [V0, Vx] from memory location starting from I
        case 0x65: {
          for (int i = 0; i <= x; i++)
            chip8->registers.V[i] =
                chip8_mem_get(&chip8->memory, chip8->registers.I + i);
        } break;
      }
    } break;
  }
}

void chip8_exec(chip8 *chip8, unsigned short opcode) {
  switch (opcode) {
    // clear the display
    case 0x00E0:
      chip8_display_clear(&chip8->display);
      break;
    // return from subroutine
    case 0x00EE:
      chip8->registers.PC = chip8_stack_pop(chip8);
      break;

    default:
      chip8_exec_ext(chip8, opcode);
  }
}
