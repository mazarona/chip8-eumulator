#include <stdio.h>
#include "SDL.h"
#include "SDL_events.h"
#include "SDL_render.h"

#include "chip8_config.h"
#include "chip8.h"
#include "chip8_registers.h"

int main(int argc, char **argv){

    struct chip8 chip8;
    chip8.registers.SP = 0;
    chip8_stack_push(&chip8, 10);
    printf("%d\n", chip8_stack_pop(&chip8));


    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow( 
        "Chip8", 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED,
        CHIP8_WIDTH * CHIP8_WINDOW_SCALE,
        CHIP8_HEIGHT * CHIP8_WINDOW_SCALE,
        SDL_WINDOW_SHOWN
    );



    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);

    while(1){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                goto out;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        SDL_Rect r;
        r.x =0 ;
        r.y =0 ;
        r.w =40 ;
        r.h =40 ;
        SDL_RenderDrawRect(renderer, &r);
        SDL_RenderPresent(renderer);
    }


out:
    SDL_DestroyWindow(window);
    return 0;
}
