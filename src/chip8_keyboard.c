#include <assert.h>
#include "chip8_keyboard.h"
#include "chip8_config.h"

static void chip8_kb_in_bounds(int key){
    assert(key >= 0 && key < CHIP8_KEYS_COUNT);
}

int chip8_keyboard_map(const char* map, char key){
    for(int i = 0; i < CHIP8_KEYS_COUNT; i++){
        if(map[i] == key){
            return i;
        }
    }
    return -1; // key was not found
}

void chip8_keyboard_down(chip8_keyboard *keyboard, int key){
    chip8_kb_in_bounds(key);
    keyboard->keyboard[key] = true;
}

void chip8_keyboard_up(chip8_keyboard *keyboard, int key){
    chip8_kb_in_bounds(key);
    keyboard->keyboard[key] = false;
}

bool chip8_keyboard_is_down(chip8_keyboard *keyboard, int key){
    chip8_kb_in_bounds(key);
    return keyboard->keyboard[key];
}
