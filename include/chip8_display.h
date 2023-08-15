#ifndef CHIP8_DISPLAY_H
#define CHIP8_DISPLAY_H

#include <stdbool.h>
#include "chip8_config.h"

typedef struct chip8_display{
    bool display[CHIP8_DISPLAY_HEIGHT][CHIP8_DISPLAY_WIDTH];
}chip8_display;


void chip8_display_set(chip8_display *display, int x, int y);
bool chip8_display_is_set(chip8_display *display, int x, int y);

#endif
