#ifndef CHIP8_KEYBOARD_H
#define CHIP8_KEYBOARD_H
#include <stdbool.h>
#include "chip8_config.h"


typedef struct chip8_keyboard{
    bool keyboard[CHIP8_KEYS_COUNT];
    const char* keyboard_map;
}chip8_keyboard;


void chip8_keyboard_set_map(chip8_keyboard* keyboard, const char *map);
int chip8_keyboard_map(chip8_keyboard *keyboard, char key);
void chip8_keyboard_down(chip8_keyboard *keyboard, int key);
void chip8_keyboard_up(chip8_keyboard *keyboard, int key);
bool chip8_keyboard_is_down(chip8_keyboard *keyboard, int key);

#endif
