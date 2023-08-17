#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include "SDL.h"

#include "chip8.h"
#include "chip8_config.h"
#include "chip8_display.h"
#include "chip8_keyboard.h"
#include "chip8_mem.h"

const char keyboard_map[CHIP8_KEYS_COUNT] = {
    SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4,
    SDLK_5, SDLK_6, SDLK_7, SDLK_8, SDLK_9,
    SDLK_a, SDLK_b, SDLK_c, SDLK_d, SDLK_e, SDLK_f,
};

void updatedisplay(chip8* chip8, SDL_Renderer* r);
int main(int argc, const char **argv){
    // get file name
    if(argc < 2){
        printf("No file was provided!\n");
        return -1;
    }
    const char *filename = argv[1];

    // open file
    FILE *f= fopen(filename, "rb");
    if(!f){
        printf("Failed to open file\n");
        fclose(f);
        return -1;
    }

    // get buffer size
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char buf[size]; // doesn't work on older c standard

    // read from file int buffer
    if(fread(buf, size, 1, f) == -1){
        printf("Failed to read from file!\n");
        fclose(f);
        return -1;
    }

    // close the file
    fclose(f);

    // create chip8 instance and initialize it
    struct chip8 chip8;
    chip8_init(&chip8);
    chip8_keyboard_set_map(&chip8.keyboard, keyboard_map);

    // load program into chip8 memory
    chip8_load(&chip8, buf, size);

    // init SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    
    // create window
    SDL_Window *window = SDL_CreateWindow( 
        "Chip8", 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED,
        CHIP8_DISPLAY_WIDTH * CHIP8_WINDOW_SCALE,
        CHIP8_DISPLAY_HEIGHT * CHIP8_WINDOW_SCALE,
        SDL_WINDOW_SHOWN
    );

    // create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);

    int tickcnt = 0;
    bool has_graphics_update = false;
    while(1){

        // poll for user input
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    goto out;
                break;
                case SDL_KEYDOWN:
                {
                    char key = event.key.keysym.sym;
                    int vkey = chip8_keyboard_map(&chip8.keyboard, key);
                    if(vkey != -1){
                        chip8_keyboard_down(&chip8.keyboard, vkey);
                    }
                }
                break;
                case SDL_KEYUP:
                {
                    char key = event.key.keysym.sym;
                    int vkey = chip8_keyboard_map(&chip8.keyboard, key);
                    if(vkey != -1){
                        chip8_keyboard_up(&chip8.keyboard, vkey);
                    }
                }
                break;

            }
        }

        // update chip8 display
        if(has_graphics_update) updatedisplay(&chip8, renderer);

        // execute opcode
        unsigned short opcode = chip8_mem_get_opcode(&chip8);
        chip8_exec(&chip8, opcode);
        
        // check if instuction was draw
        has_graphics_update = (opcode & 0xf000) == 0xd000;

        // dec the timers every 8/500 ~= 1/60 sec
        if(tickcnt == 8){
            if(chip8.registers.DT > 0) chip8.registers.DT--;
            if(chip8.registers.ST > 0) chip8.registers.ST--;
            tickcnt = 0;
        }

        tickcnt++;
        usleep(2000); // 500Hz CPU
    }


out:
    SDL_DestroyWindow(window);
    return 0;
}

void updatedisplay(chip8* chip8, SDL_Renderer* renderer){
    // clear the screen with black color
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    // set draw color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    // draw the chip8 display
    for(int x = 0; x < CHIP8_DISPLAY_WIDTH; x++){
        for(int y = 0; y < CHIP8_DISPLAY_HEIGHT; y++){
            if(chip8_display_is_set(&chip8->display, x, y)) {
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
