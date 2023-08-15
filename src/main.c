#include <stdio.h>
#include "SDL.h"
#include "SDL_events.h"
#include "SDL_render.h"

#include "chip8.h"
#include "chip8_config.h"
#include "chip8_display.h"
#include "chip8_keyboard.h"

const char keyboard_map[CHIP8_KEYS_COUNT] = {
    SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4,
    SDLK_5, SDLK_6, SDLK_7, SDLK_8, SDLK_9,
    SDLK_a, SDLK_b, SDLK_c, SDLK_d, SDLK_e, SDLK_f,
};

int main(int argc, char **argv){

    struct chip8 chip8;
    chip8_init(&chip8);

    chip8_dipslay_draw_sprite(&chip8.display, 62, 30, &chip8.memory.memory[0x00], 5);

    for(int i = 0; i < 32; i++){
        for(int j = 0; j < 64; j++){
            printf("%d", chip8.display.display[i][j]);
        }
        printf("\n");
    }

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow( 
        "Chip8", 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED,
        CHIP8_DISPLAY_WIDTH * CHIP8_WINDOW_SCALE,
        CHIP8_DISPLAY_HEIGHT * CHIP8_WINDOW_SCALE,
        SDL_WINDOW_SHOWN
    );



    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);

    while(1){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    goto out;
                break;
                case SDL_KEYDOWN:
                {
                    char key = event.key.keysym.sym;
                    int vkey = chip8_keyboard_map(keyboard_map, key);
                    if(vkey != -1){
                        chip8_keyboard_down(&chip8.keyboard, vkey);
                    }
                }
                break;
                case SDL_KEYUP:
                {
                    char key = event.key.keysym.sym;
                    int vkey = chip8_keyboard_map(keyboard_map, key);
                    if(vkey != -1){
                        chip8_keyboard_up(&chip8.keyboard, vkey);
                    }
                }
                break;

            }
        }
        // clear the screen with black color
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        // set draw color to white
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        // draw the chip8 display
        for(int x = 0; x < CHIP8_DISPLAY_WIDTH; x++){
            for(int y = 0; y < CHIP8_DISPLAY_HEIGHT; y++){
                if(chip8_display_is_set(&chip8.display, x, y)) {
                    // output the scaled pixel
                    SDL_Rect r;
                    r.x = x * CHIP8_WINDOW_SCALE;
                    r.y = y * CHIP8_WINDOW_SCALE;
                    r.w = CHIP8_WINDOW_SCALE;
                    r.h = CHIP8_WINDOW_SCALE;
                    SDL_RenderFillRect(renderer, &r);
                }

            }
        }

        SDL_RenderPresent(renderer);
    }


out:
    SDL_DestroyWindow(window);
    return 0;
}
