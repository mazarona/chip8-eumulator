#include "chip8_display.h"

#include <assert.h>
#include <memory.h>

#include "chip8_config.h"

static void chip8_display_in_bounds(int x, int y) {
  assert(x >= 0 && x < CHIP8_DISPLAY_WIDTH && y >= 0 &&
         y < CHIP8_DISPLAY_HEIGHT);
}

void chip8_display_set(chip8_display *display, int x, int y) {
  chip8_display_in_bounds(x, y);
  display->display[y][x] = true;
}
bool chip8_display_is_set(chip8_display *display, int x, int y) {
  chip8_display_in_bounds(x, y);
  return display->display[y][x];
}

bool chip8_dipslay_draw_sprite(chip8_display *display, int x, int y,
                               const char *sprite, int size) {
  bool colision = false;
  for (int i = 0; i < size; i++) {
    char c = sprite[i];
    for (int j = 0; j < 8; j++) {
      if ((c & (0b10000000 >> j))) {
        colision |= chip8_display_is_set(display, (x + j) % CHIP8_DISPLAY_WIDTH,
                                         (y + i) % CHIP8_DISPLAY_HEIGHT);
        display->display[(y + i) % CHIP8_DISPLAY_HEIGHT]
                        [(x + j) % CHIP8_DISPLAY_WIDTH] ^= true;
      }
    }
  }
  return colision;
}

void chip8_display_clear(chip8_display *display) {
  memset(display->display, 0, sizeof(display->display));
}
