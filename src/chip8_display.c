#include "chip8_display.h"
#include "chip8_config.h"
#include <assert.h>

static void chip8_display_in_bounds(int x, int y){
    assert(x >= 0 && x < CHIP8_DISPLAY_WIDTH && y >= 0 && y < CHIP8_DISPLAY_HEIGHT);
}

void chip8_display_set(chip8_display *display, int x, int y){
    chip8_display_in_bounds(x, y);
    display->display[y][x] = true;
}
bool chip8_display_is_set(chip8_display *display, int x, int y){
    chip8_display_in_bounds(x, y);
    return display->display[y][x];
}
